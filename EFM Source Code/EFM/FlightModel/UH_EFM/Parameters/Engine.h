#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// C30-family engine installation, performance map and provisional schedules.
// SI units. Rolls-Royce CAE900098 (NTSB docket 40375412); fuel/TOT/oil schedules provisional.
namespace AH6J_UH_EFM {
inline UhEfmPowerplantParameters powerplantParameters() {
    UhEfmPowerplantParameters p{};
    p.id = "rolls_royce_250_c30_civil_530f_installation_proxy";
    p.ratedPowerW = 484704.91652847565;
    p.powerRatings[0].id = "gross_engine_ceiling";
    p.powerRatings[0].maximumPowerW = 484704.91652847565;
    p.powerRatings[0].timeLimitS = 0;
    p.powerRatings[0].torqueGaugePsi = 0;
    p.powerRatingCount = 1;
    p.mainShaftTorqueNmPerCalibratedPsi = 106.45345018310056;
    p.torqueIndicationReferenceMainShaftTorqueNm = 6344.625630912793;
    p.indicatedPsiAtReferenceTorque = 59.6;
    return p;
}

inline UhEfmTurboshaftParameters engineParameters() {
    UhEfmTurboshaftParameters p{};
    p.id = "250-C30 / provisional 530F installation; partial measured Ng map";
    p.mainRotorNominalSpeedRadps = 49.95132319207771;
    p.groundIdleN2Ratio = 0.65;
    p.groundIdleMaximumThrottleCommand = 0.001;
    p.flightDetentMinimumThrottleCommand = 0.998;
    p.governedN2TargetPercent = 98;
    p.collectiveN2RisePercent = 2;
    p.collectiveN2RiseAtInput = 0.4;
    p.n2ErrorToN1Gain = 3;
    p.motorMinimumN1Percent = 12;
    p.starterMotoringN1Percent = 19;
    p.selfSustainingN1Percent = 58;
    p.idleN1Percent = 64;
    p.maximumN1Percent = 105;
    p.hotStartN1Percent = 75;
    p.starterAccelerationRatePercentPerS = 9;
    p.ignitionAccelerationRatePercentPerS = 2;
    p.n1ResponseGainPerS = 1.5;
    p.minimumN1RatePercentPerS = 0.25;
    p.maximumN1RatePercentPerS = 15;
    p.ignitionFuelFlowAtSustainKgPerHour = 29.48350405;
    p.ignitionFuelFlowExponent = 2;
    p.runningIdleFuelFlowKgPerHour = 35.38020486;
    p.runningMaximumFuelFlowKgPerHour = 136.077711;
    p.hotStartTorqueNm = 1898.145126;
    p.hotStartTorqueGaugePsi = 15;
    p.hotStartOilTemperatureC = 50;
    p.hotStartOilPressurePa = 586054.36992;
    p.hotStartTurbineOutletTemperatureC = 600;
    p.ignitionPeakN1Percent = 35;
    p.ignitionPeakTemperatureC = 690;
    p.ignitionIdleTemperatureC = 500;
    p.earlyLightoffMinimumN1Percent = 5;
    p.earlyLightoffMaximumN1Percent = 11;
    p.earlyLightoffTemperatureRiseC = 250;
    p.runningTotSeaLevelStartC = 435;
    p.runningTotSeaLevelAtReferenceTorqueC = 705;
    p.runningTotHighAltitudeStartC = 460;
    p.runningTotHighAltitudeAtReferenceTorqueC = 835;
    p.runningTotHighAltitudeM = 3048;
    p.runningTotReferenceTorquePsi = 70;
    p.runningTotAmbientGain = 2.5;
    p.runningTotRiseRateCPerS = 40;
    p.runningTotFallRateCPerS = 25;
    p.oilHeatingStartN1Percent = 20;
    p.oilTemperatureAtHeatingStartAboveAmbientC = 5;
    p.oilTemperatureAtMaximumN1AboveAmbientC = 80;
    p.oilTemperatureRateCPerS = 0.3;
    p.oilColdReferenceC = 5;
    p.oilWarmReferenceC = 50;
    p.oilColdPressureMultiplier = 1.5;
    p.oilWarmPressureMultiplier = 1;
    p.oilPressureLowN1Percent = 13;
    p.oilPressureMidN1Percent = 79;
    p.oilPressureHighN1Percent = 94;
    p.oilPressureAtLowN1Pa = 0;
    p.oilPressureAtMidN1Pa = 620528.156385;
    p.oilPressureAtHighN1Pa = 792897.088714;
    p.oilPressureAtMaximumN1Pa = 896318.448112;
    p.oilPressureRatePaPerS = 34473.786466;
    p.lowOilPressureCautionPa = 103421.359398;
    p.performance.standardTemperatureK = 288.15;
    p.performance.standardPressurePa = 101325;
    p.performance.gasProducerRpmAt100Percent = 51000;
    p.performance.correctedN1Percent[0] = 62.6;
    p.performance.correctedN1Percent[1] = 82.55401574035626;
    p.performance.correctedN1Percent[2] = 94.55719442283488;
    p.performance.correctedN1Percent[3] = 96.48363056241449;
    p.performance.correctedN1Percent[4] = 98.30934226523466;
    p.performance.correctedN1Percent[5] = 100.12086239063557;
    p.performance.correctedN1Percent[6] = 101.61872770097223;
    p.performance.referredPowerFraction[0] = 0.05384615384615385;
    p.performance.referredPowerFraction[1] = 0.24188463142486308;
    p.performance.referredPowerFraction[2] = 0.7260225323934419;
    p.performance.referredPowerFraction[3] = 0.8170807392684372;
    p.performance.referredPowerFraction[4] = 0.8990820107463926;
    p.performance.referredPowerFraction[5] = 0.9733516120495058;
    p.performance.referredPowerFraction[6] = 1.0467342657144625;
    p.performance.pointCount = 7;
    p.installedTorqueLimit.altitudeCount = 0;
    p.installedTorqueLimit.temperatureCount = 0;
    return p;
}

}
