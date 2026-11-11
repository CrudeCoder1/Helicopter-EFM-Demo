#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// AH6J tail rotor geometry/gearing and attached airfoil coefficients.
// SI units. MD530F diameter proxy; MIL-STD-810H Ch1 J frequencies; OH-6 airfoil surrogate.
namespace AH6J_UH_EFM {
inline UhEfmRotorSystemParameters tailRotorParameters() {
    UhEfmRotorSystemParameters p{};
    p.id = "ah6j_tail_rotor";
    p.role = UH_EFM_ROTOR_ROLE_ANTI_TORQUE;
    p.aerodynamicsModel = UH_EFM_ROTOR_AERODYNAMICS_MODEL_KIND_BLADE_ELEMENT16;
    p.dynamicsModel = UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_PRESCRIBED_STATE;
    p.inflowModel = UH_EFM_ROTOR_INFLOW_MODEL_KIND_PITT_PETERS_ONE_STATE;
    p.inflowWakeMomentModel = UH_EFM_WAKE_MOMENT_MODEL_NON_RIGID;
    p.inflowVrsTransientModel = UH_EFM_VRS_TRANSIENT_MODEL_WEN_CONCENTRATED_VORTICITY;
    p.uniformJohnsonMeanInflowEnabled = false;
    p.uniformInflowAzimuthSamples = 1;
    p.inflowWakeCurvatureGain = 0;
    p.groundEffectModel = UH_EFM_GROUND_EFFECT_MODEL_NONE;
    p.hubType = UH_EFM_ROTOR_HUB_TYPE_TEETERING;
    p.controlDevice = UH_EFM_ROTOR_CONTROL_DEVICE_KIND_NONE;
    p.geometry.bladeCount = 2;
    p.geometry.radiusM = 0.775;
    p.geometry.hingeOffsetM = 0;
    p.geometry.bladeStartFromHingeM = 0;
    p.geometry.fullAerodynamicSectionRadiusM = 0;
    p.geometry.chordM = 0.12;
    p.geometry.outboardChordM = 0;
    p.geometry.chordTransitionRadiusM = 0;
    p.geometry.featheringAxisFractionChord = 0.25;
    p.geometry.bladeTwistRad = -0.13962634015954636;
    p.geometry.twistStationCount = 0;
    p.geometry.tipLiftCutoffRadiusFraction = 1;
    p.geometry.tipLossFactor = 0.92;
    p.geometry.unresolvedInducedPowerFactor = 1;
    p.hubLocation.stationM = 7.1628;
    p.hubLocation.waterlineM = 1.37922;
    p.hubLocation.buttlineM = -0.29463999999999996;
    p.frameOrientation.localXInBody.x = 1;
    p.frameOrientation.localXInBody.y = 0;
    p.frameOrientation.localXInBody.z = 0;
    p.frameOrientation.localYInBody.x = 0;
    p.frameOrientation.localYInBody.y = 0;
    p.frameOrientation.localYInBody.z = 1;
    p.frameOrientation.localZInBody.x = 0;
    p.frameOrientation.localZInBody.y = -1;
    p.frameOrientation.localZInBody.z = 0;
    p.shaftTiltForwardRad = 0;
    p.nominalSpeedRadps = 297.19466502959443;
    p.poweredSpeedRange.minimumRadps = 289.71806968293794;
    p.poweredSpeedRange.maximumRadps = 300.9329627029226;
    p.powerOffSpeedRange.minimumRadps = 249.21984488854875;
    p.powerOffSpeedRange.maximumRadps = 320.2475006817852;
    p.bladeFlapInertiaKgM2 = 0;
    p.bladeMassKg = 0;
    p.bladeFirstMassMomentKgM = 0;
    p.lockNumber = 0;
    p.attachedFlowIndicial.enabled = false;
    p.attachedFlowIndicial.firstDeficiencyWeight = 0.165;
    p.attachedFlowIndicial.secondDeficiencyWeight = 0.335;
    p.attachedFlowIndicial.firstReducedTimeDecay = 0.0455;
    p.attachedFlowIndicial.secondReducedTimeDecay = 0.3;
    p.attachedFlowIndicial.apparentMassPitchRateEnabled = false;
    p.attachedFlowIndicial.apparentMassAccelerationEnabled = false;
    p.flapSpringNmPerRad = 0;
    p.flapDampingNmsPerRad = 0;
    p.pitchFlapCouplingRad = 0.7504915783575618;
    p.coningPerThrustRadPerNAtNominalSpeed = 1.6754012166229147e-05;
    p.teeterHingeUnderslingM = 0;
    p.teeterConingNaturalFrequencyPerRev = 0;
    p.teeterConingDampingRatio = 0;
    p.teeterPreconeRad = 0;
    p.gearRatioToMainRotor = 5.949685534591195;
    p.mainRotorWakeInteractionEnabled = true;
    return p;
}

inline UhEfmAirfoilParameters tailAirfoilParameters() {
    UhEfmAirfoilParameters p{};p.kind=UH_EFM_AIRFOIL_KIND_ATTACHED_POLYNOMIAL;
    p.attached.liftCurveSlopePerRad = 5.73;
    p.attached.referenceLiftCurveSlopePerRad = 6.302535746439056;
    p.attached.constantDragCoefficient = 0.0087;
    p.attached.linearDragCoefficient = -0.0216;
    p.attached.quadraticDragCoefficient = 0.4;
    p.attached.attachedBlendStartRad = 0.20943951023931953;
    p.attached.attachedBlendEndRad = 0.3141592653589793;
    return p;
}
}
