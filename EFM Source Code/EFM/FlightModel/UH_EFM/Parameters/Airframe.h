#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// AH6J mass properties, body aerodynamics and control rigging.
// SI units. CR-3144 inherited inertia/controls; Army FM 3-05.60 AH6J weight metadata.
namespace AH6J_UH_EFM {
inline UhEfmAirframeParameters airframeParameters() {
    UhEfmAirframeParameters p{};
    p.id = "ah6j_oh6_cr3144_derivative";
    p.displayName = "AH6J_UH_EFM / CR-3144 derivative";
    p.referenceDocument = "CR-3144 derivative; Army J dimensions; ADA218253 T-tail; NPS 369D/E blade mass; MD530F tail proxy; MIL-STD-810H Ch1 J frequencies";
    p.referenceMassKg = 1406.1363470000001;
    p.maximumGrossMassKg = 1791.6898615;
    p.referenceCenterOfGravity.stationM = 2.54;
    p.referenceCenterOfGravity.waterlineM = 1.25984;
    p.referenceCenterOfGravity.buttlineM = 0;
    p.inertia.xxKgM2 = 446;
    p.inertia.yyKgM2 = 1219;
    p.inertia.zzKgM2 = 979;
    p.inertia.xzKgM2 = 128;
    p.controls.collective.linkageRadPerPercent = 0.0027052603405912107;
    p.controls.collective.flexibilityFactor = 0.88;
    p.controls.longitudinalCyclic.linkageRadPerPercent = 0.0041887902047863905;
    p.controls.longitudinalCyclic.flexibilityFactor = 0.75;
    p.controls.lateralCyclic.linkageRadPerPercent = 0.002356194490192345;
    p.controls.lateralCyclic.flexibilityFactor = 0.75;
    p.controls.pedal.linkageRadPerPercent = 0.006806784082777886;
    p.controls.pedal.flexibilityFactor = 0.67;
    p.controls.reversible = true;
    p.controls.forceFeedbackModeled = false;
    p.stabilizerBar.timeConstantS = 0;
    p.stabilizerBar.rollRateToCosineCyclicS = 0;
    p.stabilizerBar.pitchRateToSineCyclicS = 0;
    p.mainRotorSwashplateActuator.bandwidthRadps = 0;
    p.mainRotorSwashplateActuator.maximumRateRadps = 0;
    p.simulatorControls.mainRotorCollective.minimumInput = 0;
    p.simulatorControls.mainRotorCollective.maximumInput = 1;
    p.simulatorControls.mainRotorCollective.outputAtMinimumRad = 0.10297442586766545;
    p.simulatorControls.mainRotorCollective.outputAtMaximumRad = 0.3735004599267865;
    p.simulatorControls.mainRotorCollective.hasNeutralPoint = false;
    p.simulatorControls.mainRotorCollective.neutralInput = 0;
    p.simulatorControls.mainRotorCollective.outputAtNeutralRad = 0;
    p.simulatorControls.lateralCyclic.minimumInput = -1;
    p.simulatorControls.lateralCyclic.maximumInput = 1;
    p.simulatorControls.lateralCyclic.outputAtMinimumRad = 0.1499237827463129;
    p.simulatorControls.lateralCyclic.outputAtMaximumRad = -0.12060225131280818;
    p.simulatorControls.lateralCyclic.hasNeutralPoint = true;
    p.simulatorControls.lateralCyclic.neutralInput = 0;
    p.simulatorControls.lateralCyclic.outputAtNeutralRad = 0;
    p.simulatorControls.longitudinalCyclic.minimumInput = -1;
    p.simulatorControls.longitudinalCyclic.maximumInput = 1;
    p.simulatorControls.longitudinalCyclic.outputAtMinimumRad = 0.17889624832941878;
    p.simulatorControls.longitudinalCyclic.outputAtMaximumRad = -0.2399827721492203;
    p.simulatorControls.longitudinalCyclic.hasNeutralPoint = false;
    p.simulatorControls.longitudinalCyclic.neutralInput = 0;
    p.simulatorControls.longitudinalCyclic.outputAtNeutralRad = 0;
    p.simulatorControls.tailRotorCollective.minimumInput = -1;
    p.simulatorControls.tailRotorCollective.maximumInput = 1;
    p.simulatorControls.tailRotorCollective.outputAtMinimumRad = -0.053494341573626224;
    p.simulatorControls.tailRotorCollective.outputAtMaximumRad = 0.40256019197249215;
    p.simulatorControls.tailRotorCollective.hasNeutralPoint = false;
    p.simulatorControls.tailRotorCollective.neutralInput = 0;
    p.simulatorControls.tailRotorCollective.outputAtNeutralRad = 0;
    p.fuselageAerodynamics.modelKind = UH_EFM_FUSELAGE_AERODYNAMICS_MODEL_EQUIVALENT_AREAS;
    p.fuselageAerodynamics.aerodynamicCenterOffsetBodyM.x = 0;
    p.fuselageAerodynamics.aerodynamicCenterOffsetBodyM.y = 0;
    p.fuselageAerodynamics.aerodynamicCenterOffsetBodyM.z = 0;
    p.fuselageAerodynamics.axialDragAreaM2 = 0.59253514;
    p.fuselageAerodynamics.sideForceSlopeAreaM2PerRad = 0;
    p.fuselageAerodynamics.normalForceSlopeAreaM2PerRad = 1.038;
    p.fuselageAerodynamics.pitchMomentSlopeM3PerRad = 0;
    p.fuselageAerodynamics.yawMomentSlopeM3PerRad = 0;
    p.fuselageAerodynamics.crossflow.axialDragAreaM2 = 0;
    p.fuselageAerodynamics.crossflow.normalToAxialCrossflowAreaM2 = 0;
    p.fuselageAerodynamics.crossflow.lateralQuadraticDragAreaM2 = 0;
    p.fuselageAerodynamics.crossflow.normalQuadraticDragAreaM2 = 0;
    p.fuselageAerodynamics.crossflow.lateralForwardDampingAreaM2 = 0;
    p.fuselageAerodynamics.crossflow.pitchCrossflowMomentVolumeM3 = 0;
    p.fuselageAerodynamics.crossflow.yawCrossflowMomentVolumeM3 = 0;
    return p;
}

}
