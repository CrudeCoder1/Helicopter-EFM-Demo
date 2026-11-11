#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// Shaft inertia, limits and provisional mechanical losses.
// SI units. NASA TM-83726 OH-58 losses and MD530F topology proxy; low-speed losses provisional.
namespace AH6J_UH_EFM {
// maximumPowerW = 0 disables the installed-power cap (425 shp = 316922.44542246481 W).
// Rated engine power normalizes the map; maximumMainShaftTorqueNm is a separate cap.
inline UhEfmDrivetrainParameters drivetrainParameters() {
    UhEfmDrivetrainParameters p{};
    p.equivalentMainShaftInertiaKgM2 = 388.8224781281567;
    p.constantLossTorqueNm = 9.70354508257257;
    p.viscousLossTorqueNms = 0;
    p.quadraticLossTorqueNms2 = 0.002393222452356821;
    p.engineDriveEfficiency = 0.990025;
    p.tailDriveEfficiency = 0.990025;
    p.maximumPowerW = 0;
    p.maximumMainShaftTorqueNm = 0;
    p.freewheelClutchInstalled = true;
    p.governorNaturalFrequencyRadps = 2;
    p.governorDampingRatio = 0.9;
    p.governorTorqueResponseTimeS = 0.2;
    p.governorMinimumFullScaleTorqueRiseTimeS = 0.25;
    p.governorMinimumFullScaleTorqueFallTimeS = 0.25;
    return p;
}

}
