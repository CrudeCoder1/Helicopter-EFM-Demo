#pragma once
#include "../FlightModel/aero.h"
#include "Systems/ElectricSystem.h"
#include "Systems/C30Performance.h"


// N1 gas producer rpm will be main measure of engine power
// N2 is handled in the rotor degree of freedom section in Aero.cpp since it is connected to the rotor through the clutch
//
//TODO: accurate fuel flow

// TODO validate numbers (oil temp, oil press, TOT, fuel flow...)
// fig 7-30 (pg.7-17) idle fuel flow for 65%N2 (64%N1 true idle) and 100%N2 (throttle is open, N1~=75% estimate)
// fig 7-33 (pg.7-19) TOT based on torque, and torque-horsepower conversion

// Engine data source: MD 530F Plus (Model 369FF) Rotorcraft Flight Manual CSP-FF-1, Rolls-Royce 250-C30 (this helicopter's
// torque scale: 59.6 psi = 5 minute takeoff limit, about 430 shp; 43 psi = 310 shp, i.e. about 7.2 shp per psi).
//  - limits: torque 59.6 psi (5 min) and 48.9 psi continuous; TOT 768 C (5 min) and 694 C continuous; N1 105% continuous,
//    106% for 10 s; ground idle N1 64%
//  - power available versus pressure altitude and temperature comes from the Engine Power Check Chart (figure 5-15),
//    see C30Performance.h / C30Data.h. The engine is flat rated: full torque until the TOT limit is reached.
//  - TOT as a function of torque, altitude and temperature comes from the same chart
//  - the manual has no fuel flow data; fuel flow shape comes from the OH-6A manual (TM 55-1520-214-10) charts, see below
// engine data tables
constexpr static const int _N1_Points = 9;
static double _N1percent[_N1_Points] = { 0.0, 20.0,  62.0,  80.0,  95.0,  98.0,  100.0,  105.0, 106.0};
static double _HP_data[_N1_Points] =   { 0.0,  0.0,  45.0, 135.0, 312.0, 370.0,  400.0,  425.0, 432.0};

class TurboshaftEngine
{
private:

    EFMData* p_EFMdata;
    FlightControls* p_flightControl;
    ElectricSystem* p_ElectricSystem;

    enum engineStates
    {
        ENG_Off,
        ENG_Motor,
        ENG_Ignition,
        ENG_Running
    } engineState = ENG_Off;

    constexpr static double ENG_RPM_MOTOR_MIN_PCT = 12.0;
    constexpr static double ENG_RPM_MOTOR_PCT = 19.0;
    constexpr static double ENG_RPM_SUSTAIN_PCT = 58.0;
    constexpr static double ENG_RPM_IDLE_PCT = 64.0;
    constexpr static double ENG_RPM_MAX_LIMIT_PCT = 106.0;

    MatrixFunction fn_engHP{ _HP_data, _N1_Points, _N1percent };

    constexpr static double ENG_TQ_LIMIT_PSIG = 59.6;		// maximum takeoff (5 minute) torque
    constexpr static double ENG_TQ_CONT_PSIG = 48.9;		// maximum continuous torque
    constexpr static double ENG_TOT_TAKEOFF_C = 768.0;		// 5 minute TOT limit (the chart shows 767.78)
    constexpr static double ENG_TOT_CONT_C = 694.0;		// maximum continuous TOT
    constexpr static double ENG_DERATE_MIN = 0.2;

    double getPressureAltitude_ft() const
    {
        return 145366.45 * (1.0 - pow(p_EFMdata->airPressure / 101325.0, 0.190284));
    }

    // continuous torque limit [psi]: 48.9 psi, lower where the 694 C continuous TOT limit is reached first
    double getContinuousTorqueLimit() const
    {
        if (p_EFMdata->airPressure <= 0.0)
        {
            return ENG_TQ_CONT_PSIG;
        }
        return C30::torqueAvailable(ENG_TOT_CONT_C, getPressureAltitude_ft(), p_EFMdata->ambientTemp_C, ENG_TQ_CONT_PSIG);
    }

    // Fuel flow while running [lb/hr]. Shape and altitude effect come from the OH-6A manual (TM 55-1520-214-10):
    //  - fig 7-20 ground fuel flow: idle and flat pitch fuel flow fall with altitude (about 75% of sea level at 8400 ft,
    //    about 61% at 14000 ft, little change with temperature)
    //  - fig 7-7a cruise: 114 lb/hr at 33.7 psi, 155 lb/hr at 59.6 psi (SL, 20 C), idle about 57 lb/hr, flat pitch about 84 lb/hr
    // The OH-6A engine is smaller than this sim's engine, so only the SHAPE is used: fuel flow is expressed as a fraction
    // of the maximum power (torque / 59.6 psi here, torque / 80 psi on the OH-6A) and scaled to this model's own idle
    // (about 73 lb/hr) and maximum takeoff power fuel flow (280 lb/hr).
    // The altitude factor is applied at low load and fades out by about 33 psi (altitude mainly lowers no-load fuel flow).
    constexpr static int FF_POINTS = 5;
    constexpr static double FF_TQ_FRACTION[FF_POINTS] = { 0.0, 0.125, 0.42, 0.745, 1.0 };
    constexpr static double FF_FRACTION_OF_MAX[FF_POINTS] = { 0.26, 0.45, 0.61, 0.83, 1.0 };
    constexpr static double FF_MAX_LBHR = 280.0;
    constexpr static int FF_ALT_POINTS = 3;
    constexpr static double FF_ALT_FT[FF_ALT_POINTS] = { 0.0, 8400.0, 14000.0 };
    constexpr static double FF_ALT_FACTOR[FF_ALT_POINTS] = { 1.0, 0.755, 0.61 };

    static double piecewiseLinear(const double* xs, const double* ys, int n, double x)
    {
        if (x <= xs[0]) return ys[0];
        if (x >= xs[n - 1]) return ys[n - 1];
        int i = 0;
        while (x > xs[i + 1]) { i++; }
        return ys[i] + (ys[i + 1] - ys[i]) * (x - xs[i]) / (xs[i + 1] - xs[i]);
    }

    double getRunningFuelFlow() const
    {
        double loadFraction = Torque_PSI / ENG_TQ_LIMIT_PSIG;
        double flow = FF_MAX_LBHR * piecewiseLinear(FF_TQ_FRACTION, FF_FRACTION_OF_MAX, FF_POINTS, loadFraction);
        if (p_EFMdata->airPressure > 0.0)
        {
            double pressureAlt_ft = 145366.45 * (1.0 - pow(p_EFMdata->airPressure / 101325.0, 0.190284));
            double altFactor = piecewiseLinear(FF_ALT_FT, FF_ALT_FACTOR, FF_ALT_POINTS, pressureAlt_ft);
            // full effect at/below 10% load, none from 42% load up
            double w = (0.42 - loadFraction) / (0.42 - 0.125);
            w = w < 0.0 ? 0.0 : (w > 1.0 ? 1.0 : w);
            flow *= 1.0 - (1.0 - altFactor) * w;
        }
        return flow;
    }

    // fraction of full power available (1.0 = full): torque available at the 768 C TOT limit (flat rated at 59.6 psi)
    double getPowerDerate() const
    {
        if (p_EFMdata->airPressure <= 0.0) // atmosphere not set yet
        {
            return 1.0;
        }
        double available = C30::torqueAvailable(ENG_TOT_TAKEOFF_C, getPressureAltitude_ft(), p_EFMdata->ambientTemp_C, ENG_TQ_LIMIT_PSIG);
        double derate = available / ENG_TQ_LIMIT_PSIG;
        return derate > 1.0 ? 1.0 : (derate < ENG_DERATE_MIN ? ENG_DERATE_MIN : derate);
    }

    EDPARAM cockpitAPI;
    void* ENGINE_TRQ = cockpitAPI.getParamHandle("ENGINE_TRQ"); // for use in digital VIDS gauge
    void* ENGINE_TOT = cockpitAPI.getParamHandle("ENGINE_TOT"); // for use in digital VIDS gauge
    void* ENGINE_TRQ_CONT_LIMIT = cockpitAPI.getParamHandle("ENGINE_TRQ_CONT_LIMIT"); // continuous torque limit [psi]

    double TargetN2 = 98.0;// adjustable governor target to be used for collective switch
    double N1Cmd = 100.0;// target N1 before delta limit
    double N1rate = 15.0;
    double N1_PCT = 100.0;// current N1 speed [%] (gas producer speed)
    double fuelFlow = 0.0; // [lb/hr]
    double Torque_PSI = 0.0;
    double engTorque_ftlb = 0.0;
    double oilPressure = 0.0; // [PSI]
    double oilTemp = 0.0; // [deg C]
    double TOTcmd = 15.0; // [deg C]
    double TOTrate = 10.0;
    double turbineOutletTemp = 15.0; // current TOT [deg C]
    double addTemp = 0.0;
    double TOTwhenIgnited = 0.0;

public:
    double throttleInput = 0.0;	// Throttle input command

    TurboshaftEngine(EFMData& ptr_EFMdata, FlightControls& ptr_fltCntrl, ElectricSystem& ptr_elec)
        : p_EFMdata(&ptr_EFMdata)
        , p_flightControl(&ptr_fltCntrl)
        , p_ElectricSystem(&ptr_elec)
    {}
    ~TurboshaftEngine() {}

    void initCold()
    {
        engineState = ENG_Off;
        TargetN2 = 98.0;
        throttleInput = 0;
        N1_PCT = 0.0;
        N1Cmd = 0.0;
        N1rate = 15.0;
        fuelFlow = 0.0;
        Torque_PSI = 0.0;
        engTorque_ftlb = 0.0;
        oilTemp = 15.0;
        oilPressure = 0.0;
        TOTcmd = 15.0;
        turbineOutletTemp = 15;
    }
    void initHot()
    {
        engineState = ENG_Running;
        TargetN2 = 98.0;
        throttleInput = 1;
        N1_PCT = 75.0;
        N1Cmd = 75.0;
        N1rate = 15.0;
        fuelFlow = 0.0;
        Torque_PSI = 15.0;
        engTorque_ftlb = 1400.0;
        oilTemp = 50.0;
        oilPressure = 85.0;
        TOTcmd = 600.0;
        turbineOutletTemp = 600;
    }
    /*
    void SetCommand(int command, const float value)
    {
        if (command == throttle)
        {
            throttleInput = value;
        }
        else if (command == starterButton)
        {
            starterButtonOn = value > 0.0;
        }
    }*/

    double getN1RPM()
    {
        return N1_PCT / 100.0;
    }
    double getTOT()
    {
        return turbineOutletTemp;
    }
    double getOilPress_Pa()
    {
        return oilPressure * Convert::PsiToPascal;
    }
    double getTorque_Nm()
    {
        return engTorque_ftlb * Convert::lbft_to_Nm;
    }
    double getTorqueRelative()
    {
        return Torque_PSI / 59.0;
    }
    double getTorquePSI() const { return Torque_PSI; }
    double getN1PCT() const { return N1_PCT; }
    double getThrottleInput() const { return throttleInput; }
    double getTorqueLimitContinuousPSI() const { return getContinuousTorqueLimit(); }
    double getPowerDerateValue() const { return getPowerDerate(); }	// 1.0 = full power available (C30 power check chart)
    double getFuelFlowLbHr() const { return fuelFlow; }
    double getFuelFlow()// [kg/hr]
    {
        return fuelFlow * Convert::lb_to_kg;
    }
    double getEngTorque()
    {
        return engTorque_ftlb;
    }

    void update(const double frameTime, const bool hasFuel, const double currentN2)
    {
        if ((!hasFuel || throttleInput < -0.2) && engineState!= ENG_Motor)
        {//add flameout from thin air and damaged engine
            engineState = ENG_Off;
        }

        switch (engineState)
        {
        case ENG_Off:
        {
            if (p_ElectricSystem->isStarterEngaged() && N1_PCT < ENG_RPM_SUSTAIN_PCT)//TODO add req. electric power
            {
                engineState = ENG_Motor;
            }
            N1Cmd = 0.0;
            N1rate = limit(0.025 * pow(N1_PCT, 1.5), 0.75, 12);
            TOTcmd = p_EFMdata->ambientTemp_C;
            TOTrate = LinInterp(N1_PCT, 60.0, 5.0, 25.0, 1.5);
            fuelFlow = 0.0;

            break;
        }
        case ENG_Motor: //engine turning due to electrical starter. No fuel or ignition
        {
            if (p_ElectricSystem->isStarterEngaged())
            {
                N1Cmd = ENG_RPM_MOTOR_PCT;
                double engineAccelFactor = 1.0 - limit(pow(N1_PCT / N1Cmd, 2.0), 0.0, 0.9);
                N1rate = 9.0 * engineAccelFactor;
            }
            else
            {
                engineState = ENG_Off;
            }

            if (throttleInput >= 0.0 && (hasFuel)) 
            {
                //igniterEngaged = true;
                engineState = ENG_Ignition;
                TOTwhenIgnited = turbineOutletTemp;
                addTemp = LinInterp(N1_PCT, 5.0, 11.0, 250.0, 0.0);// additional TOT rise due to early light off (creating a hot start if RPM too low)
            }
            TOTcmd = p_EFMdata->ambientTemp_C;
            TOTrate = LinInterp(N1_PCT, 5.0, 60.0, 1.5, 25.0);
            fuelFlow = 0.0;

            break;
        }
        case ENG_Ignition://20-40 sec from 19 to 58
        {
            if (p_ElectricSystem->isStarterEngaged() && hasFuel)
            {
                N1Cmd = ENG_RPM_IDLE_PCT;
                N1rate = 2.0;
            }
            else
            {
                engineState = ENG_Off;
            }

            if (N1_PCT >= ENG_RPM_SUSTAIN_PCT)
            {
                engineState = ENG_Running;
            }

            fuelFlow = 65.0 * pow((N1_PCT - ENG_RPM_MOTOR_MIN_PCT) / (ENG_RPM_SUSTAIN_PCT - ENG_RPM_MOTOR_MIN_PCT), 2.0);

            if (N1_PCT < 35.0) // TOT peaks around 35% then starts to go down
            {
                TOTcmd = 690 + TOTwhenIgnited + addTemp;
                TOTrate = LinInterp(N1_PCT, ENG_RPM_MOTOR_MIN_PCT, 35, 100.0, 50.0);
            }
            else
            {
                TOTcmd = LinInterp(N1_PCT, 35, ENG_RPM_IDLE_PCT, 690.0, 500.0) + TOTwhenIgnited + addTemp;
                TOTrate = LinInterp(N1_PCT, 35, N1Cmd, 50.0, 75.0);
            }

            break;
        }       
        case ENG_Running:
        {
            //---------- Engine governor ------------------------------------------------------------
            // modulates engine speed based on torque needed to keep rotor speed constant

            double N1Target = LinInterp(p_flightControl->CollectiveInput, 0.0, 1.0, 75.0, 102.0); ;// not the best way for base N1 target, but works for now

            //TargetN2 += 0.02 * N2Adjustment;
            //TargetN2 = limit(TargetN2, 96.0, 103.0);

            //An increase in collective from idle collective causes 1-1.5% increase N2
            double TargetN2Final = TargetN2 + LinInterp(p_flightControl->CollectiveInput, 0.0, 0.4, 0.0, 2.0);
            double N2error = TargetN2Final - currentN2;
            N1Cmd = N1Target + 3.0 * N2error;
            N1Cmd = limit(N1Cmd, ENG_RPM_IDLE_PCT, ENG_RPM_IDLE_PCT + throttleInput * (ENG_RPM_MAX_LIMIT_PCT - ENG_RPM_IDLE_PCT));

            N1rate = limit(abs(N1_PCT - N1Cmd) * 1.5, 0.25, 15);//non-linear change rate
             //-----------------------------------------------------------------------------------

            // the manual only lists 78lb/h at 65% and 138lb/hr at 79%. max FF is 300lb/hr
            fuelFlow = getRunningFuelFlow();

            updateTOT();
            break;
        }
        default:
            break;
        }


        N1Cmd = limit(N1Cmd, 0.0, ENG_RPM_MAX_LIMIT_PCT);
        N1_PCT = deltaLimit(N1_PCT, N1Cmd, N1rate);

        double EnginePower_HP = fn_engHP.interpnf1(N1_PCT) * getPowerDerate();// engine horsepower, with altitude/temperature derate (C30 power check chart)
        //EnginePower_HP -= engAntiIceO-n ? 15.0 : 0.0;//15 SHP loss from anti ice on
        engTorque_ftlb = EnginePower_HP * 550.0 / OmegaT;//engine torque conversion for HP, [lb-ft]
        
        //conversion for torque[lb-ft] to pressure[psi] based on chapter 7 in manual, says 500shp=70psi & 575=80.9psi. 500hp=5529ftlb; 575hp=6358ftlb <- what page??
        Torque_PSI = EnginePower_HP / 7.209;// 310 shp at 43 psi (MD 530F RFM figure 5-15, 'engine power check chart')
        cockpitAPI.setParamNumber(ENGINE_TRQ, Torque_PSI);
        cockpitAPI.setParamNumber(ENGINE_TRQ_CONT_LIMIT, getContinuousTorqueLimit());


        cockpitAPI.setCockpitDrawArg(INT_N1rpmNeedle, (float)(N1_PCT / 100));
        double intPart;
        double fractPart = modf(N1_PCT / 10.0, &intPart);
        cockpitAPI.setCockpitDrawArg(INT_N1rpmNeedleSmall, (float)fractPart);

        updateEngineOil();
        turbineOutletTemp = deltaLimit(turbineOutletTemp, TOTcmd, TOTrate);
        cockpitAPI.setParamNumber(ENGINE_TOT, turbineOutletTemp);

        G_Params.cautionLight[CL_EngOut] = engineState == ENG_Off;
    }

    void updateTOT()
    {
        // TOT from the Engine Power Check Chart (MD 530F RFM figure 5-15): torque, pressure altitude and ambient temperature.
        // The chart starts at 10 psi, below that the TOT is blended towards the idle value used before.
        // TODO added heat with scav air on, 10 degrees increase when anti-ice is on
        double pa_ft = p_EFMdata->airPressure > 0.0 ? getPressureAltitude_ft() : p_EFMdata->altitudeMSL_FT;
        double startTmp = LinInterp(p_EFMdata->altitudeMSL_FT, 0, 10000, 435, 460);
        double chartAt10 = C30::tot(10.0, pa_ft, p_EFMdata->ambientTemp_C);
        if (Torque_PSI >= 10.0)
        {
            TOTcmd = C30::tot(Torque_PSI, pa_ft, p_EFMdata->ambientTemp_C);
        }
        else
        {
            TOTcmd = startTmp + (chartAt10 - startTmp) * (Torque_PSI / 10.0);
        }
        TOTcmd = limit(TOTcmd, 300.0, 950.0);
        if (TOTcmd < turbineOutletTemp)
        {
            TOTrate = 25.0;
        }
        else
        {
            TOTrate = 40.0;
        }
    }

    void updateEngineOil()
    {
        //------- Oil temp ----------------------
        double engOilTempCmd_degC = 0.0;
        if (N1_PCT >= 20)
        {
            engOilTempCmd_degC = LinInterp(N1_PCT, 20.0, ENG_RPM_MAX_LIMIT_PCT, 5.0, 80.0);
        }
        engOilTempCmd_degC += p_EFMdata->ambientTemp_C;
        oilTemp = deltaLimit(oilTemp, engOilTempCmd_degC, 0.3);      
        cockpitAPI.setCockpitDrawArg(INT_OilTempNeedle, (float)LinInterp(oilTemp, 15.0, 125.0, 0.0, 1.0));

        //------ Oil pressure ----------------------
        double engOilPress_tempBias = LinInterp(oilTemp, 5, 50, 1.5, 1.0);//make oil press higher when oil temp is cold

        double engOilPressCmd_psi = 0.0;
        //Engine Oil Pressure as a function of engine RPM 
        if (N1_PCT >= 94.0)
        {
            engOilPressCmd_psi = LinInterp(N1_PCT, 94.0, ENG_RPM_MAX_LIMIT_PCT, 115.0, 130.0);
        }
        else if (N1_PCT >= 79.0)
        {
            engOilPressCmd_psi = LinInterp(N1_PCT, 79.0, 94.0, 90.0, 115.0);
        }
        else
        {
            engOilPressCmd_psi = LinInterp(N1_PCT, 13.0, 79.0, 0.0, 90.0);
        }

        engOilPressCmd_psi *= engOilPress_tempBias;
        oilPressure = deltaLimit(oilPressure, engOilPressCmd_psi, 5.0);
        cockpitAPI.setCockpitDrawArg(INT_OilPressNeedle, (float)(oilPressure / 200.0));
        G_Params.cautionLight[CL_XmsnPress] = oilPressure < 15;
    }
};
