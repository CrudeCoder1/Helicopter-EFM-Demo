#pragma once
#include "../FlightModel/aero.h"


// N1 gas producer rpm will be main measure of engine power
// N2 is handled in the rotor degree of freedom section in Aero.cpp since it is connected to the rotor through the clutch
//
//TODO: accurate fuel flow

// TODO validate numbers (oil temp, oil press, TOT, fuel flow...)
// fig 7-30 (pg.7-17) idle fuel flow for 65%N2 (64%N1 true idle) and 100%N2 (throttle is open, N1~=75% estimate)
// fig 7-33 (pg.7-19) TOT based on torque, and torque-horsepower conversion

// engine data tables
constexpr static const int _N1_Points = 9;
static double _N1percent[_N1_Points] = { 0.0, 20.0,  62.0,  80.0,  95.0,  98.0,  100.0,  105.0, 106.0};
static double _HP_data[_N1_Points] =   { 0.0,  0.0,  45.0, 135.0, 312.0, 370.0,  400.0,  425.0, 575.0};

class TurboshaftEngine
{
private:

    EFMData& p_EFMdata;
    FlightControls& p_flightControl;

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

    EDPARAM cockpitAPI;
    void* VIDS_TRQ = cockpitAPI.getParamHandle("VIDS_TRQ"); // for use in digital VIDS gauge
    void* VIDS_TOT = cockpitAPI.getParamHandle("VIDS_TOT"); // for use in digital VIDS gauge

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
    bool starterButtonOn = false;

    TurboshaftEngine(EFMData& ptr_EFMdata, FlightControls& ptr_fltCntrl)
        : p_EFMdata(ptr_EFMdata)
        , p_flightControl(ptr_fltCntrl)
    {}
    ~TurboshaftEngine() {}

    void initCold()
    {
        engineState = ENG_Off;
        TargetN2 = 98.0;
        throttleInput = 0;
        starterButtonOn = false;
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
        starterButtonOn = false;
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
            if (starterButtonOn && N1_PCT < ENG_RPM_SUSTAIN_PCT)//TODO add req. electric power
            {
                engineState = ENG_Motor;
            }
            N1Cmd = 0.0;
            N1rate = limit(0.025 * pow(N1_PCT, 1.5), 0.75, 12);
            TOTcmd = p_EFMdata.ambientTemp_C;
            TOTrate = LinInterp(N1_PCT, 60.0, 5.0, 25.0, 1.5);
            fuelFlow = 0.0;

            break;
        }
        case ENG_Motor: //engine turning due to electrical starter. No fuel or ignition
        {
            if (starterButtonOn)
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
            TOTcmd = p_EFMdata.ambientTemp_C;
            TOTrate = LinInterp(N1_PCT, 5.0, 60.0, 1.5, 25.0);
            fuelFlow = 0.0;

            break;
        }
        case ENG_Ignition://20-40 sec from 19 to 58
        {
            if (starterButtonOn && hasFuel)
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

            double N1Target = LinInterp(p_flightControl.CollectiveInput, 0.0, 1.0, 75.0, 102.0); ;// not the best way for base N1 target, but works for now

            //TargetN2 += 0.02 * N2Adjustment;
            //TargetN2 = limit(TargetN2, 96.0, 103.0);

            //An increase in collective from idle collective causes 1-1.5% increase N2
            double TargetN2Final = TargetN2 + LinInterp(p_flightControl.CollectiveInput, 0.0, 0.4, 0.0, 2.0);
            double N2error = TargetN2Final - currentN2;
            N1Cmd = N1Target + 3.0 * N2error;
            N1Cmd = limit(N1Cmd, ENG_RPM_IDLE_PCT, ENG_RPM_IDLE_PCT + throttleInput * (ENG_RPM_MAX_LIMIT_PCT - ENG_RPM_IDLE_PCT));

            N1rate = limit(abs(N1_PCT - N1Cmd) * 1.5, 0.25, 15);//non-linear change rate
             //-----------------------------------------------------------------------------------

            // the manual only lists 78lb/h at 65% and 138lb/hr at 79%. max FF is 300lb/hr
            fuelFlow = LinInterp(N1_PCT, ENG_RPM_IDLE_PCT, ENG_RPM_MAX_LIMIT_PCT, 78.0, 300.0);

            updateTOT();
            break;
        }
        default:
            break;
        }


        N1Cmd = limit(N1Cmd, 0.0, ENG_RPM_MAX_LIMIT_PCT);
        N1_PCT = deltaLimit(N1_PCT, N1Cmd, N1rate);

        double EnginePower_HP = fn_engHP.interpnf1(N1_PCT);// engine horsepower
        //EnginePower_HP -= engAntiIceO-n ? 15.0 : 0.0;//15 SHP loss from anti ice on
        engTorque_ftlb = EnginePower_HP * 550.0 / OmegaT;//engine torque conversion for HP, [lb-ft]
        
        //conversion for torque[lb-ft] to pressure[psi] based on chapter 7 in manual, says 500shp=70psi & 575=80.9psi. 500hp=5529ftlb; 575hp=6358ftlb <- what page??
        Torque_PSI = EnginePower_HP * (59.0 / 425.0);// conversion from fig.7-33 (pg.7-19)
        cockpitAPI.setParamNumber(VIDS_TRQ, Torque_PSI);


        cockpitAPI.setCockpitDrawArg(INT_N1rpmNeedle, (float)(N1_PCT / 100));
        double intPart;
        double fractPart = modf(N1_PCT / 10.0, &intPart);
        cockpitAPI.setCockpitDrawArg(INT_N1rpmNeedleSmall, (float)fractPart);

        updateEngineOil();
        turbineOutletTemp = deltaLimit(turbineOutletTemp, TOTcmd, TOTrate);
        cockpitAPI.setParamNumber(VIDS_TOT, turbineOutletTemp);
    }

    void updateTOT()
    {
        // TODO added heat with svac air on
        // 10deg increase when anti-ice is on
        // TOT data from from fig.7-33 (pg.7-19)
        double startTmp = LinInterp(p_EFMdata.altitudeMSL_FT, 0, 10000, 435, 460);
        double endTmp = LinInterp(p_EFMdata.altitudeMSL_FT, 0, 10000, 705, 835);
        double baseTemp = LinInterp(Torque_PSI, 0, 70, startTmp, endTmp);// ambient temp 15C
        double ambientBias = (p_EFMdata.ambientTemp_C - 15) * 2.5;// based on chart, 10deg change in ambient~=25deg change in TOT
        TOTcmd = baseTemp + ambientBias;
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
        engOilTempCmd_degC += p_EFMdata.ambientTemp_C;
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
    }
};
