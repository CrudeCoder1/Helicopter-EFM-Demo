#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// AH6J five-blade main rotor; resolved values preserve the accepted configuration.
// SI units. Army FM 3-05.60 J diameter; Ouellette, NPS 2002, 369D/E blade-mass proxy.
namespace AH6J_UH_EFM {
inline UhEfmRotorSystemParameters mainRotorParameters() {
    UhEfmRotorSystemParameters p{};
    p.id = "ah6j_main_rotor";
    p.role = UH_EFM_ROTOR_ROLE_MAIN;
    p.aerodynamicsModel = UH_EFM_ROTOR_AERODYNAMICS_MODEL_KIND_BLADE_ELEMENT16;
    p.dynamicsModel = UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_FULLY_ARTICULATED;
    p.inflowModel = UH_EFM_ROTOR_INFLOW_MODEL_KIND_PITT_PETERS_THREE_STATE;
    p.inflowWakeMomentModel = UH_EFM_WAKE_MOMENT_MODEL_NON_RIGID;
    p.inflowVrsTransientModel = UH_EFM_VRS_TRANSIENT_MODEL_WEN_CONCENTRATED_VORTICITY;
    p.uniformJohnsonMeanInflowEnabled = false;
    p.uniformInflowAzimuthSamples = 1;
    p.inflowWakeCurvatureGain = 0;
    p.groundEffectModel = UH_EFM_GROUND_EFFECT_MODEL_CHEESEMAN_BENNETT;
    p.hubType = UH_EFM_ROTOR_HUB_TYPE_ARTICULATED;
    p.controlDevice = UH_EFM_ROTOR_CONTROL_DEVICE_KIND_NONE;
    p.geometry.bladeCount = 5;
    p.geometry.radiusM = 4.1656;
    p.geometry.hingeOffsetM = 0.15239999999999998;
    p.geometry.bladeStartFromHingeM = 0.3429;
    p.geometry.fullAerodynamicSectionRadiusM = 0;
    p.geometry.chordM = 0.171;
    p.geometry.outboardChordM = 0;
    p.geometry.chordTransitionRadiusM = 0;
    p.geometry.featheringAxisFractionChord = 0.25;
    p.geometry.bladeTwistRad = -0.13962634015954636;
    p.geometry.twistStationCount = 0;
    p.geometry.tipLiftCutoffRadiusFraction = 1;
    p.geometry.tipLossFactor = 0.97;
    p.geometry.unresolvedInducedPowerFactor = 1;
    p.hubLocation.stationM = 2.54;
    p.hubLocation.waterlineM = 2.1082;
    p.hubLocation.buttlineM = 0;
    p.frameOrientation.localXInBody.x = 1;
    p.frameOrientation.localXInBody.y = 0;
    p.frameOrientation.localXInBody.z = 0;
    p.frameOrientation.localYInBody.x = 0;
    p.frameOrientation.localYInBody.y = 1;
    p.frameOrientation.localYInBody.z = 0;
    p.frameOrientation.localZInBody.x = 0;
    p.frameOrientation.localZInBody.y = 0;
    p.frameOrientation.localZInBody.z = 1;
    p.shaftTiltForwardRad = 0.05235987755982989;
    p.nominalSpeedRadps = 49.95132319207771;
    p.poweredSpeedRange.minimumRadps = 48.69468613064179;
    p.poweredSpeedRange.maximumRadps = 50.579641722795664;
    p.powerOffSpeedRange.minimumRadps = 41.8879020478639;
    p.powerOffSpeedRange.maximumRadps = 53.82595413150512;
    p.bladeFlapInertiaKgM2 = 69.72940028932484;
    p.bladeMassKg = 15.543319142422616;
    p.bladeFirstMassMomentKgM = 25.177460420935677;
    p.lockNumber = 5.8;
    p.attachedFlowIndicial.enabled = true;
    p.attachedFlowIndicial.firstDeficiencyWeight = 0.165;
    p.attachedFlowIndicial.secondDeficiencyWeight = 0.335;
    p.attachedFlowIndicial.firstReducedTimeDecay = 0.0455;
    p.attachedFlowIndicial.secondReducedTimeDecay = 0.3;
    p.attachedFlowIndicial.apparentMassPitchRateEnabled = true;
    p.attachedFlowIndicial.apparentMassAccelerationEnabled = false;
    p.flapSpringNmPerRad = 0;
    p.flapDampingNmsPerRad = 0;
    p.pitchFlapCouplingRad = 0;
    p.coningPerThrustRadPerNAtNominalSpeed = 0;
    p.teeterHingeUnderslingM = 0;
    p.teeterConingNaturalFrequencyPerRev = 0;
    p.teeterConingDampingRatio = 0;
    p.teeterPreconeRad = 0;
    p.gearRatioToMainRotor = 1;
    p.mainRotorWakeInteractionEnabled = false;
    return p;
}

inline UhEfmAirfoilParameters mainAirfoilParameters() {
    UhEfmAirfoilParameters p{};p.kind=UH_EFM_AIRFOIL_KIND_NACA0015;return p;
}
}
