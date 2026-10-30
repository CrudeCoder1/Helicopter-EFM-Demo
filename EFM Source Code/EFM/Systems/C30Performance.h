#pragma once
#include <cmath>
#include "C30Data.h"

// Rolls-Royce 250-C30 engine performance lookups, from the MD 530F Plus Rotorcraft Flight Manual figure 5-15
// "Engine Power Check Chart" (see C30Data.h). The chart is a nomogram: torque versus pressure altitude in the upper grid,
// turbine outlet temperature versus ambient temperature in the lower grid, both sharing one horizontal coordinate x.
//   TOT(torque, PA, OAT):     x from the altitude curves at the given torque, TOT from the OAT curves at that x
//   torque available at a TOT limit: x where the OAT curve reaches the limit, torque from the altitude curves at that x
namespace C30
{
    inline double clampd(double v, double lo, double hi) { return v < lo ? lo : (v > hi ? hi : v); }

    // index i and fraction f so value lies between axis[i] and axis[i+1] (clamped to the axis ends)
    inline void axisPos(const double* axis, int n, double v, int& i, double& f)
    {
        if (v <= axis[0]) { i = 0; f = 0.0; return; }
        if (v >= axis[n - 1]) { i = n - 2; f = 1.0; return; }
        i = 0;
        while (v > axis[i + 1]) { i++; }
        f = (v - axis[i]) / (axis[i + 1] - axis[i]);
    }

    // value of a sampled curve at x, extrapolating linearly past the ends
    inline double curveAt(const double* s, double x)
    {
        double u = (x - C30Data::X0) / C30Data::X_STEP;
        int n = C30Data::X_POINTS;
        int i = (int)std::floor(u);
        if (i < 0) i = 0;
        if (i > n - 2) i = n - 2;
        double f = u - i;
        return s[i] + (s[i + 1] - s[i]) * f;
    }

    // x where a (monotone decreasing) sampled curve equals value, extrapolating linearly past the ends
    inline double xAtValue(const double* s, double value)
    {
        int n = C30Data::X_POINTS;
        if (value >= s[0])
        {
            double slope = (s[1] - s[0]) / C30Data::X_STEP;		// negative
            return C30Data::X0 + (value - s[0]) / slope;
        }
        if (value <= s[n - 1])
        {
            double slope = (s[n - 1] - s[n - 2]) / C30Data::X_STEP;
            return C30Data::X0 + (n - 1) * C30Data::X_STEP + (value - s[n - 1]) / slope;
        }
        int i = 0;
        while (value < s[i + 1]) { i++; }
        double f = (s[i] - value) / (s[i] - s[i + 1]);
        return C30Data::X0 + (i + f) * C30Data::X_STEP;
    }

    // torque [psi] on the altitude curves at x (interpolated between the two bracketing altitudes)
    inline double torqueAtX(double pa_ft, double x)
    {
        int i; double f;
        axisPos(C30Data::PA_FT, C30Data::PA_POINTS, pa_ft, i, f);
        double a = curveAt(C30Data::TORQUE_AT_X[i], x);
        double b = curveAt(C30Data::TORQUE_AT_X[i + 1], x);
        return a + (b - a) * f;
    }

    // x on the nomogram for a given torque and pressure altitude
    inline double xAtTorque(double torque_psi, double pa_ft)
    {
        int i; double f;
        axisPos(C30Data::PA_FT, C30Data::PA_POINTS, pa_ft, i, f);
        double a = xAtValue(C30Data::TORQUE_AT_X[i], torque_psi);
        double b = xAtValue(C30Data::TORQUE_AT_X[i + 1], torque_psi);
        return a + (b - a) * f;
    }

    // turbine outlet temperature [C] for torque [psi], pressure altitude [ft] and ambient temperature [C]
    inline double tot(double torque_psi, double pa_ft, double oat_C)
    {
        double x = xAtTorque(torque_psi, pa_ft);
        int i; double f;
        axisPos(C30Data::OAT_C, C30Data::OAT_POINTS, oat_C, i, f);
        double a = curveAt(C30Data::TOT_AT_X[i], x);
        double b = curveAt(C30Data::TOT_AT_X[i + 1], x);
        return a + (b - a) * f;
    }

    // torque [psi] available before the TOT limit is reached, never more than torqueCap [psi]
    inline double torqueAvailable(double totLimit_C, double pa_ft, double oat_C, double torqueCap_psi)
    {
        int i; double f;
        axisPos(C30Data::OAT_C, C30Data::OAT_POINTS, oat_C, i, f);
        double xa = xAtValue(C30Data::TOT_AT_X[i], totLimit_C);
        double xb = xAtValue(C30Data::TOT_AT_X[i + 1], totLimit_C);
        double ta = torqueAtX(pa_ft, xa);
        double tb = torqueAtX(pa_ft, xb);
        double t = ta + (tb - ta) * f;
        return t > torqueCap_psi ? torqueCap_psi : t;
    }

    // VNE [kt IAS] from the flight manual placards (gross weight band, OAT and pressure altitude); no flight area = floor
    inline double vne(double pa_ft, double oat_C, double weight_lb)
    {
        constexpr static double OAT_ROWS[9] = { -30, -20, -10, 0, 10, 20, 30, 40, 45 };
        constexpr static double PA_COLS[10] = { 0, 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000 };
        constexpr static double NO_FLIGHT_VNE = 50.0;
        int b = 0;
        while (b < C30Data::VNE_BANDS - 1 && weight_lb > C30Data::VNE_BAND_MAX_LB[b]) { b++; }
        int r, c; double fr, fc;
        axisPos(OAT_ROWS, 9, oat_C, r, fr);
        axisPos(PA_COLS, 10, pa_ft, c, fc);
        auto cell = [&](int rr, int cc) { double v = C30Data::VNE_TABLE[b][rr][cc]; return v <= 0.0 ? NO_FLIGHT_VNE : v; };
        double top = cell(r, c) + (cell(r, c + 1) - cell(r, c)) * fc;
        double bot = cell(r + 1, c) + (cell(r + 1, c + 1) - cell(r + 1, c)) * fc;
        return top + (bot - top) * fr;
    }
}
