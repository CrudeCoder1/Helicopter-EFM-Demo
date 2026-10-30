#pragma once
#include "Systems/C30Performance.h"

// Operating limits and exceedance tracking, from the MD 530F Plus (Model 369FF) Rotorcraft Flight Manual CSP-FF-1
// (Rolls-Royce 250-C30), section 2 limitations:
//  - VNE placards (gross weight bands, pressure altitude, OAT), 152 kt maximum
//  - torque: 48.9 psi continuous (lower at altitude/temperature where the 694 C TOT limit is reached first), 59.6 psi 5 minutes
//  - TOT: 694 C continuous, 768 C 5 minutes
//  - N1: 105% continuous, 106% for 10 seconds
//  - rotor rpm: power on 473 to 477 rpm (99 to 100% N2), power off 410 to 508 rpm
//
// Results are exported as numeric parameters (0 = ok, 1 = exceeded) for Lua / gauges:
//   LIM_VNE_KT         VNE for the current weight / altitude / temperature [kt IAS]
//   LIM_IAS_KT         indicated airspeed used for the check [kt]
//   LIM_VNE            IAS above VNE
//   LIM_TORQUE_CONT    torque above the continuous limit
//   LIM_TORQUE_5MIN    torque above the continuous limit for more than 5 minutes (accumulated while above)
//   LIM_TORQUE_MAX     torque above 59.6 psi
//   LIM_TOT_CONT       TOT above 694 C
//   LIM_TOT_MAX        TOT above 768 C
//   LIM_NR_LOW         rotor rpm below the limit (power on: 473, power off: 410) while airborne
//   LIM_NR_HIGH        rotor rpm above the limit (power on: 477, power off: 508)
//   LIM_N1_HIGH        N1 above 105%

class OperatingLimits
{
private:
    EDPARAM cockpitAPI;
    void* P_VNE_KT = cockpitAPI.getParamHandle("LIM_VNE_KT");
    void* P_IAS_KT = cockpitAPI.getParamHandle("LIM_IAS_KT");
    void* P_VNE = cockpitAPI.getParamHandle("LIM_VNE");
    void* P_TORQUE_CONT = cockpitAPI.getParamHandle("LIM_TORQUE_CONT");
    void* P_TORQUE_5MIN = cockpitAPI.getParamHandle("LIM_TORQUE_5MIN");
    void* P_TORQUE_MAX = cockpitAPI.getParamHandle("LIM_TORQUE_MAX");
    void* P_TOT_CONT = cockpitAPI.getParamHandle("LIM_TOT_CONT");
    void* P_TOT_MAX = cockpitAPI.getParamHandle("LIM_TOT_MAX");
    void* P_NR_LOW = cockpitAPI.getParamHandle("LIM_NR_LOW");
    void* P_NR_HIGH = cockpitAPI.getParamHandle("LIM_NR_HIGH");
    void* P_N1_HIGH = cockpitAPI.getParamHandle("LIM_N1_HIGH");

    constexpr static double TQ_MAX_PSI = 59.6;
    constexpr static double TQ_5MIN_S = 300.0;
    constexpr static double TOT_CONT_C = 694.0;
    constexpr static double TOT_MAX_C = 768.0;
    constexpr static double N1_CONT_MAX_PCT = 105.0;
    // rotor rpm limits. The manual's power-on range is 473 to 477 rpm; a 3 rpm tolerance is added here because this model's
    // governor does not hold the rotor as tightly as the real one.
    constexpr static double NR_ON_MIN_RPM = 470.0;
    constexpr static double NR_ON_MAX_RPM = 480.0;
    constexpr static double NR_OFF_MIN_RPM = 410.0;
    constexpr static double NR_OFF_MAX_RPM = 508.0;

    // ---- debug values (read by Avionics.lua when the debug readout is switched on) ----
    void* P_DBG_PA_FT = cockpitAPI.getParamHandle("DBG_PA_FT");
    void* P_DBG_FAT_C = cockpitAPI.getParamHandle("DBG_FAT_C");
    void* P_DBG_SIGMA = cockpitAPI.getParamHandle("DBG_SIGMA");
    void* P_DBG_WEIGHT_LB = cockpitAPI.getParamHandle("DBG_WEIGHT_LB");
    void* P_DBG_AGL_FT = cockpitAPI.getParamHandle("DBG_AGL_FT");
    void* P_DBG_COLLECTIVE = cockpitAPI.getParamHandle("DBG_COLLECTIVE");
    void* P_DBG_PEDAL = cockpitAPI.getParamHandle("DBG_PEDAL");
    void* P_DBG_FUEL_FLOW = cockpitAPI.getParamHandle("DBG_FUEL_FLOW");
    void* P_DBG_DERATE = cockpitAPI.getParamHandle("DBG_DERATE");
    void* P_DBG_N1 = cockpitAPI.getParamHandle("DBG_N1");
    void* P_DBG_TOT = cockpitAPI.getParamHandle("DBG_TOT");
    void* P_DBG_PITCH = cockpitAPI.getParamHandle("DBG_PITCH");
    void* P_DBG_ROLL = cockpitAPI.getParamHandle("DBG_ROLL");
    void* P_DBG_CYC_PITCH = cockpitAPI.getParamHandle("DBG_CYC_PITCH");
    void* P_DBG_CYC_ROLL = cockpitAPI.getParamHandle("DBG_CYC_ROLL");
    void* P_DBG_DENSITY_ON = cockpitAPI.getParamHandle("DBG_DENSITY_ON");
    void* P_DBG_STICK_PITCH = cockpitAPI.getParamHandle("DBG_STICK_PITCH");
    void* P_DBG_STICK_ROLL = cockpitAPI.getParamHandle("DBG_STICK_ROLL");
    void* P_DBG_TRIM_PITCH = cockpitAPI.getParamHandle("DBG_TRIM_PITCH");
    void* P_DBG_TRIM_ROLL = cockpitAPI.getParamHandle("DBG_TRIM_ROLL");

    double torqueTimeAbove_s = 0.0;

public:
    // never exceed airspeed [kt IAS] for the given conditions (MD 530F VNE placards)
    static double getVNE_kt(double pressureAlt_ft, double fat_C, double weight_lb)
    {
        return C30::vne(pressureAlt_ft, fat_C, weight_lb);
    }

    void init()
    {
        torqueTimeAbove_s = 0.0;
    }

    // values for the debug readout (cheap: a handful of parameters per frame)
    void updateDebug(const EFMData& d, double collective, double pedal, double fuelFlowLbHr, double powerDerate, double tot_C, double n1Pct,
                     double pitchDeg, double rollDeg, double cycPitch, double cycRoll,
                     double stickPitch, double stickRoll, double trimPitch, double trimRoll)
    {
        double pressureAlt_ft = d.airPressure > 0.0 ? 145366.45 * (1.0 - pow(d.airPressure / 101325.0, 0.190284)) : 0.0;
        cockpitAPI.setParamNumber(P_DBG_PA_FT, pressureAlt_ft);
        cockpitAPI.setParamNumber(P_DBG_FAT_C, d.ambientTemp_C);
        cockpitAPI.setParamNumber(P_DBG_SIGMA, d.rho / EFMData::SEA_LEVEL_RHO_KGM3);
        cockpitAPI.setParamNumber(P_DBG_WEIGHT_LB, d.mass_kg * 2.20462);
        cockpitAPI.setParamNumber(P_DBG_AGL_FT, d.altitudeAGL_ft);
        cockpitAPI.setParamNumber(P_DBG_COLLECTIVE, collective * 100.0);
        cockpitAPI.setParamNumber(P_DBG_PEDAL, pedal * 100.0);
        cockpitAPI.setParamNumber(P_DBG_FUEL_FLOW, fuelFlowLbHr);
        cockpitAPI.setParamNumber(P_DBG_DERATE, powerDerate * 100.0);
        cockpitAPI.setParamNumber(P_DBG_N1, n1Pct);
        cockpitAPI.setParamNumber(P_DBG_TOT, tot_C);
        cockpitAPI.setParamNumber(P_DBG_PITCH, pitchDeg);
        cockpitAPI.setParamNumber(P_DBG_ROLL, rollDeg);
        cockpitAPI.setParamNumber(P_DBG_CYC_PITCH, cycPitch * 100.0);
        cockpitAPI.setParamNumber(P_DBG_CYC_ROLL, cycRoll * 100.0);
        cockpitAPI.setParamNumber(P_DBG_DENSITY_ON, d.densityEffect ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_DBG_STICK_PITCH, stickPitch * 100.0);
        cockpitAPI.setParamNumber(P_DBG_STICK_ROLL, stickRoll * 100.0);
        cockpitAPI.setParamNumber(P_DBG_TRIM_PITCH, trimPitch * 100.0);
        cockpitAPI.setParamNumber(P_DBG_TRIM_ROLL, trimRoll * 100.0);
    }

    // rotorOmega [rad/s], torque [psi], torqueContLimit [psi], n1 [%], tot [C], n2 [%], rotor speed [%] for power on/off
    void update(double dt, const EFMData& d, double rotorOmega, double torquePsi, double torqueContLimitPsi, double n1Pct, double totC, double n2Pct)
    {
        double pressureAlt_ft = 145366.45 * (1.0 - pow(d.airPressure / 101325.0, 0.190284));
        double weight_lb = d.mass_kg * 2.20462;
        double ias_kt = d.totalVelocity_MPS * sqrt(d.rho / EFMData::SEA_LEVEL_RHO_KGM3) * 1.94384;
        double vne_kt = getVNE_kt(pressureAlt_ft, d.ambientTemp_C, weight_lb);

        // Placard note: for autorotation (rotor not driven by the engine) VNE is reduced by 22 kt, but not below 85 kt at 2000 lb
        // and up to 96 kt at 3100 lb (one knot more per 100 lb, taken from the placards).
        // In this model N2 follows the rotor speed in autorotation, so N2 versus Nr can't tell. Use the engine instead: at idle gas producer speed
        // with almost no torque the engine is not driving the rotor.
        bool rotorDriven = !(n1Pct < 70.0 && torquePsi < 11.0);
        if (!rotorDriven && d.altitudeAGL_ft > 6.0)
        {
            double autoFloor = 85.0 + (weight_lb > 2000.0 ? std::ceil((weight_lb - 2000.0) / 100.0) : 0.0);
            double reduced = vne_kt - 22.0;
            vne_kt = reduced > autoFloor ? reduced : (autoFloor < vne_kt ? autoFloor : vne_kt);
        }

        bool aboveCont = torquePsi > torqueContLimitPsi;
        if (aboveCont) { torqueTimeAbove_s += dt; }
        else { torqueTimeAbove_s -= dt * 2.0; if (torqueTimeAbove_s < 0.0) torqueTimeAbove_s = 0.0; }

        double rotorRpm = rotorOmega * 60.0 / (2.0 * M_PI);
        double nrPct = rotorOmega / (475.0 * 2.0 * M_PI / 60.0) * 100.0;
        bool airborne = d.altitudeAGL_ft > 6.0;
        bool powerOn = n1Pct >= 62.0 && n2Pct >= nrPct - 1.5;		// engine driving the rotor (clutch engaged)
        double nrMin = powerOn ? NR_ON_MIN_RPM : NR_OFF_MIN_RPM;
        double nrMax = powerOn ? NR_ON_MAX_RPM : NR_OFF_MAX_RPM;
        bool nrLow = airborne && n1Pct >= 62.0 && rotorRpm < nrMin;
        bool nrHigh = rotorRpm > nrMax;

        cockpitAPI.setParamNumber(P_VNE_KT, vne_kt);
        cockpitAPI.setParamNumber(P_IAS_KT, ias_kt);
        cockpitAPI.setParamNumber(P_VNE, (d.airPressure > 0.0 && ias_kt > vne_kt) ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_TORQUE_CONT, aboveCont ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_TORQUE_5MIN, torqueTimeAbove_s > TQ_5MIN_S ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_TORQUE_MAX, torquePsi > TQ_MAX_PSI ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_TOT_CONT, totC > TOT_CONT_C ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_TOT_MAX, totC > TOT_MAX_C ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_NR_LOW, nrLow ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_NR_HIGH, nrHigh ? 1.0 : 0.0);
        cockpitAPI.setParamNumber(P_N1_HIGH, n1Pct > N1_CONT_MAX_PCT ? 1.0 : 0.0);
    }
};
