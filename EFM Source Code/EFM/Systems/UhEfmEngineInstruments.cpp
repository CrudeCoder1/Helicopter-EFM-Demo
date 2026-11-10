#include "Engine.h"
#include "../FlightModel/UH_EFM_Ah6jApi.h"

// Feed physical powerplant state into the original cockpit implementation.
// The existing TOT/oil equations, caution wiring and draw arguments stay here;
// no UH_EFM air-data, avionics or instrument host is imported.
void TurboshaftEngine::updateFromFlightModel(const UhEfmAh6jOutput& state)
{
    const auto previousState = engineState;
    N1_PCT = state.n1Percent;
    fuelFlow = state.fuelFlowKgPerHour / Convert::lb_to_kg;
    engTorque_ftlb = state.engineTorqueNm / Convert::lbft_to_Nm;
    Torque_PSI = state.torqueGaugePsi;
    engineState = state.engineState == UH_AH6J_RUNNING    ? ENG_Running
                  : state.engineState == UH_AH6J_IGNITION ? ENG_Ignition
                  : state.engineState == UH_AH6J_MOTORING ? ENG_Motor
                                                          : ENG_Off;
    cockpitAPI.setParamNumber(ENGINE_TRQ, Torque_PSI);
    cockpitAPI.setCockpitDrawArg(INT_N1rpmNeedle, static_cast<float>(N1_PCT / 100));
    double whole;
    cockpitAPI.setCockpitDrawArg(INT_N1rpmNeedleSmall,
                                 static_cast<float>(std::modf(N1_PCT / 10, &whole)));
    // Retain the original instrument temperature schedules, including light-off.
    if (engineState == ENG_Running)
        updateTOT();
    else if (engineState == ENG_Ignition)
    {
        if (previousState != ENG_Ignition)
        {
            TOTwhenIgnited = turbineOutletTemp;
            addTemp = LinInterp(N1_PCT, 5, 11, 250, 0);
        }
        if (N1_PCT < 35)
        {
            TOTcmd = 690 + TOTwhenIgnited + addTemp;
            TOTrate = LinInterp(N1_PCT, ENG_RPM_MOTOR_MIN_PCT, 35, 100, 50);
        }
        else
        {
            TOTcmd = LinInterp(N1_PCT, 35, ENG_RPM_IDLE_PCT, 690, 500) + TOTwhenIgnited + addTemp;
            TOTrate = LinInterp(N1_PCT, 35, ENG_RPM_IDLE_PCT, 50, 75);
        }
    }
    else
    {
        TOTcmd = p_EFMdata->ambientTemp_C;
        TOTrate = engineState == ENG_Motor ? LinInterp(N1_PCT, 5, 60, 1.5, 25)
                                           : LinInterp(N1_PCT, 60, 5, 25, 1.5);
    }
    turbineOutletTemp = deltaLimit(turbineOutletTemp, TOTcmd, TOTrate);
    cockpitAPI.setParamNumber(ENGINE_TOT, turbineOutletTemp);
    updateEngineOil();
    G_Params.cautionLight[CL_EngOut] = engineState == ENG_Off;
}
