#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// DCS blade mapping, brake/droop settings and hot-start initialization.
// SI units. Original AH6J animation/brake scales; hot-start initialization remains provisional.
namespace AH6J_UH_EFM {
inline UhEfmRotorPresentationParameters presentationParameters() {
    UhEfmRotorPresentationParameters p{};
    p.runtimeBladeIndices[1] = 4;
    p.runtimeBladeIndices[2] = 3;
    p.runtimeBladeIndices[3] = 2;
    p.runtimeBladeIndices[4] = 1;
    p.negativeFlapFullScaleRad = 0.05235987755982989;
    p.positiveFlapFullScaleRad = 0.19198621771937624;
    p.droopClearSpeedFraction = 0.1;
    p.brakeNominalSpeedFraction = 0.4;
    p.brakeNominalTorqueNm = 271.1635896662801;
    p.brakeStoppedTorqueNm = 677.9089741657002;
    return p;
}

inline UhEfmInitializationParameters initializationParameters() {
    UhEfmInitializationParameters p{};
    p.hotUniformInflowRatio = 0.05;
    p.hotControls.collectiveRad = 0.2565634000431664;
    p.hotControls.cyclicCosRad = -0.0038397243543875246;
    p.hotControls.cyclicSinRad = 0.021118483949131385;
    p.hotControls.swashplatePhaseRad = 0;
    p.hotControls.collectiveRateRadps = 0;
    p.hotControls.cyclicCosRateRadps = 0;
    p.hotControls.cyclicSinRateRadps = 0;
    p.hotControls.collectiveAccelerationRadps2 = 0;
    p.hotControls.cyclicCosAccelerationRadps2 = 0;
    p.hotControls.cyclicSinAccelerationRadps2 = 0;
    p.periodic.stepsPerRevolution = 180;
    p.periodic.minimumRevolutions = 2;
    p.periodic.maximumRevolutions = 60;
    p.periodic.flapAngleToleranceRad = 3e-07;
    p.periodic.flapRateToleranceRadps = 3e-05;
    p.periodic.inflowRatioTolerance = 3e-08;
    p.periodic.relativeMeanLoadTolerance = 3e-07;
    p.periodic.sectionLiftCoefficientTolerance = 1e-07;
    p.periodic.maximumAbsFlapAngleRad = 0.7;
    p.periodic.maximumAbsFlapRateRadps = 200;
    p.periodic.maximumAbsInflowRatio = 1;
    p.tailInflowMaximumIterations = 60;
    p.tailInflowToleranceMps = 1e-05;
    return p;
}

}
