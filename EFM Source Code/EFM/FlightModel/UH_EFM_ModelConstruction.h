#ifndef UH_EFM_MODEL_CONSTRUCTION_H
#define UH_EFM_MODEL_CONSTRUCTION_H
#include <stdint.h>

/* Typed construction SDK. SI units unless a field names another unit.
   Geometry vectors use UH_EFM body axes: +X forward, +Y right, +Z down.
   Fuselage locations use station aft, waterline up, buttline right.
   This header declares data only; all construction and physics run in UH_EFM.
   Input strings/structures are copied. Component handles are immutable and
   may be released after aircraft creation; the aircraft retains its data. */
#if defined(UH_EFM_EXPORTS)
#define UH_MODEL_API __declspec(dllexport)
#else
#define UH_MODEL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t UhEfmRotorRole;
enum {
    UH_EFM_ROTOR_ROLE_MAIN = 0,
    UH_EFM_ROTOR_ROLE_ANTI_TORQUE = 1,
    UH_EFM_ROTOR_ROLE_COAXIAL_UPPER = 2,
    UH_EFM_ROTOR_ROLE_COAXIAL_LOWER = 3,
    UH_EFM_ROTOR_ROLE_TANDEM_FRONT = 4,
    UH_EFM_ROTOR_ROLE_TANDEM_REAR = 5
};

typedef uint32_t UhEfmRotorAerodynamicsModelKind;
enum {
    UH_EFM_ROTOR_AERODYNAMICS_MODEL_KIND_BLADE_ELEMENT16 = 0,
    UH_EFM_ROTOR_AERODYNAMICS_MODEL_KIND_UNIFORM_DISK = 1,
    UH_EFM_ROTOR_AERODYNAMICS_MODEL_KIND_TABULATED = 2
};

typedef uint32_t UhEfmRotorDynamicsModelKind;
enum {
    UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_PRESCRIBED_STATE = 0,
    UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_FULLY_ARTICULATED = 1,
    UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_TEETERING = 2,
    UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_HINGELESS = 3,
    UH_EFM_ROTOR_DYNAMICS_MODEL_KIND_RIGID = 4
};

typedef uint32_t UhEfmRotorInflowModelKind;
enum {
    UH_EFM_ROTOR_INFLOW_MODEL_KIND_PRESCRIBED_UNIFORM = 0,
    UH_EFM_ROTOR_INFLOW_MODEL_KIND_PITT_PETERS_ONE_STATE = 1,
    UH_EFM_ROTOR_INFLOW_MODEL_KIND_PITT_PETERS_THREE_STATE = 2,
    UH_EFM_ROTOR_INFLOW_MODEL_KIND_MANGLER_SQUIRE_STEADY = 3,
    UH_EFM_ROTOR_INFLOW_MODEL_KIND_PETERS_HE_FINITE_STATE = 4
};

typedef uint32_t UhEfmWakeMomentModel;
enum {
    UH_EFM_WAKE_MOMENT_MODEL_NON_RIGID = 0,
    UH_EFM_WAKE_MOMENT_MODEL_RIGID = 1
};

typedef uint32_t UhEfmVrsTransientModel;
enum {
    UH_EFM_VRS_TRANSIENT_MODEL_JOHNSON_FIXED_LAG = 0,
    UH_EFM_VRS_TRANSIENT_MODEL_WEN_CONCENTRATED_VORTICITY = 1
};

typedef uint32_t UhEfmGroundEffectModel;
enum {
    UH_EFM_GROUND_EFFECT_MODEL_NONE = 0,
    UH_EFM_GROUND_EFFECT_MODEL_CHEESEMAN_BENNETT = 1
};

typedef uint32_t UhEfmRotorHubType;
enum {
    UH_EFM_ROTOR_HUB_TYPE_ARTICULATED = 0,
    UH_EFM_ROTOR_HUB_TYPE_TEETERING = 1,
    UH_EFM_ROTOR_HUB_TYPE_HINGELESS = 2,
    UH_EFM_ROTOR_HUB_TYPE_RIGID = 3
};

typedef uint32_t UhEfmAirfoilKind;
enum {
    UH_EFM_AIRFOIL_KIND_NACA0015 = 0,
    UH_EFM_AIRFOIL_KIND_NACA0012 = 1,
    UH_EFM_AIRFOIL_KIND_NACA63015 = 2,
    UH_EFM_AIRFOIL_KIND_NACA63415 = 3,
    UH_EFM_AIRFOIL_KIND_ATTACHED_POLYNOMIAL = 4
};

typedef uint32_t UhEfmRotorControlDeviceKind;
enum {
    UH_EFM_ROTOR_CONTROL_DEVICE_KIND_NONE = 0,
    UH_EFM_ROTOR_CONTROL_DEVICE_KIND_STABILIZER_BAR = 1,
    UH_EFM_ROTOR_CONTROL_DEVICE_KIND_PADDLE_CONTROL_ROTOR = 2
};

typedef uint32_t UhEfmFuselageAerodynamicsModel;
enum {
    UH_EFM_FUSELAGE_AERODYNAMICS_MODEL_EQUIVALENT_AREAS = 0,
    UH_EFM_FUSELAGE_AERODYNAMICS_MODEL_CROSSFLOW = 1
};

typedef uint32_t UhEfmAerodynamicSurfaceOrientation;
enum {
    UH_EFM_AERODYNAMIC_SURFACE_ORIENTATION_HORIZONTAL = 0,
    UH_EFM_AERODYNAMIC_SURFACE_ORIENTATION_VERTICAL = 1
};

typedef uint32_t UhEfmAerodynamicSurfacePolarKind;
enum {
    UH_EFM_AERODYNAMIC_SURFACE_POLAR_KIND_LEGACY_ATTACHED_INDUCED = 0,
    UH_EFM_AERODYNAMIC_SURFACE_POLAR_KIND_VITERNA360 = 1
};

typedef uint32_t UhEfmAerodynamicSurfaceControlInput;
enum {
    UH_EFM_AERODYNAMIC_SURFACE_CONTROL_INPUT_NONE = 0,
    UH_EFM_AERODYNAMIC_SURFACE_CONTROL_INPUT_MAIN_ROTOR_LONGITUDINAL_CYCLIC = 1
};

typedef struct UhEfmFuselageReferenceLocation {
    double stationM;
    double waterlineM;
    double buttlineM;
} UhEfmFuselageReferenceLocation;

typedef struct UhEfmInertiaTensor {
    double xxKgM2;
    double yyKgM2;
    double zzKgM2;
    double xzKgM2;
} UhEfmInertiaTensor;

typedef struct UhEfmControlAxisMapping {
    double linkageRadPerPercent;
    double flexibilityFactor;
} UhEfmControlAxisMapping;

typedef struct UhEfmMechanicalControlParameters {
    UhEfmControlAxisMapping collective;
    UhEfmControlAxisMapping longitudinalCyclic;
    UhEfmControlAxisMapping lateralCyclic;
    UhEfmControlAxisMapping pedal;
    uint32_t reversible;
    uint32_t forceFeedbackModeled;
} UhEfmMechanicalControlParameters;

typedef struct UhEfmStabilizerBarParameters {
    double timeConstantS;
    double rollRateToCosineCyclicS;
    double pitchRateToSineCyclicS;
} UhEfmStabilizerBarParameters;

typedef struct UhEfmSwashplateActuatorParameters {
    double bandwidthRadps;
    double maximumRateRadps;
} UhEfmSwashplateActuatorParameters;

typedef struct UhEfmNormalizedControlMapping {
    double minimumInput;
    double maximumInput;
    double outputAtMinimumRad;
    double outputAtMaximumRad;
    uint32_t hasNeutralPoint;
    double neutralInput;
    double outputAtNeutralRad;
} UhEfmNormalizedControlMapping;

typedef struct UhEfmSimulatorControlParameters {
    UhEfmNormalizedControlMapping mainRotorCollective;
    UhEfmNormalizedControlMapping lateralCyclic;
    UhEfmNormalizedControlMapping longitudinalCyclic;
    UhEfmNormalizedControlMapping tailRotorCollective;
} UhEfmSimulatorControlParameters;

typedef struct UhEfmVector3 {
    double x;
    double y;
    double z;
} UhEfmVector3;

typedef struct UhEfmFuselageCrossflowParameters {
    double axialDragAreaM2;
    double normalToAxialCrossflowAreaM2;
    double lateralQuadraticDragAreaM2;
    double normalQuadraticDragAreaM2;
    double lateralForwardDampingAreaM2;
    double pitchCrossflowMomentVolumeM3;
    double yawCrossflowMomentVolumeM3;
} UhEfmFuselageCrossflowParameters;

typedef struct UhEfmFuselageAerodynamicsParameters {
    UhEfmFuselageAerodynamicsModel modelKind;
    UhEfmVector3 aerodynamicCenterOffsetBodyM;
    double axialDragAreaM2;
    double sideForceSlopeAreaM2PerRad;
    double normalForceSlopeAreaM2PerRad;
    double pitchMomentSlopeM3PerRad;
    double yawMomentSlopeM3PerRad;
    UhEfmFuselageCrossflowParameters crossflow;
} UhEfmFuselageAerodynamicsParameters;

typedef struct UhEfmAirframeParameters {
    const char* id;
    const char* displayName;
    const char* referenceDocument;
    double referenceMassKg;
    double maximumGrossMassKg;
    UhEfmFuselageReferenceLocation referenceCenterOfGravity;
    UhEfmInertiaTensor inertia;
    UhEfmMechanicalControlParameters controls;
    UhEfmStabilizerBarParameters stabilizerBar;
    UhEfmSwashplateActuatorParameters mainRotorSwashplateActuator;
    UhEfmSimulatorControlParameters simulatorControls;
    UhEfmFuselageAerodynamicsParameters fuselageAerodynamics;
} UhEfmAirframeParameters;

typedef struct UhEfmRotorGeometry {
    uint32_t bladeCount;
    double radiusM;
    double hingeOffsetM;
    double bladeStartFromHingeM;
    double fullAerodynamicSectionRadiusM;
    double chordM;
    double outboardChordM;
    double chordTransitionRadiusM;
    double featheringAxisFractionChord;
    double bladeTwistRad;
    double twistStationRadiusM[8];
    double twistStationAngleRad[8];
    uint32_t twistStationCount;
    double tipLiftCutoffRadiusFraction;
    double tipLossFactor;
    double unresolvedInducedPowerFactor;
} UhEfmRotorGeometry;

typedef struct UhEfmRotorFrameOrientation {
    UhEfmVector3 localXInBody;
    UhEfmVector3 localYInBody;
    UhEfmVector3 localZInBody;
} UhEfmRotorFrameOrientation;

typedef struct UhEfmRotorSpeedRange {
    double minimumRadps;
    double maximumRadps;
} UhEfmRotorSpeedRange;

typedef struct UhEfmParameters {
    uint32_t enabled;
    double firstDeficiencyWeight;
    double secondDeficiencyWeight;
    double firstReducedTimeDecay;
    double secondReducedTimeDecay;
    uint32_t apparentMassPitchRateEnabled;
    uint32_t apparentMassAccelerationEnabled;
} UhEfmParameters;

typedef struct UhEfmRotorSystemParameters {
    const char* id;
    UhEfmRotorRole role;
    UhEfmRotorAerodynamicsModelKind aerodynamicsModel;
    UhEfmRotorDynamicsModelKind dynamicsModel;
    UhEfmRotorInflowModelKind inflowModel;
    UhEfmWakeMomentModel inflowWakeMomentModel;
    UhEfmVrsTransientModel inflowVrsTransientModel;
    uint32_t uniformJohnsonMeanInflowEnabled;
    uint32_t uniformInflowAzimuthSamples;
    double inflowWakeCurvatureGain;
    UhEfmGroundEffectModel groundEffectModel;
    UhEfmRotorHubType hubType;
    UhEfmRotorControlDeviceKind controlDevice;
    UhEfmRotorGeometry geometry;
    UhEfmFuselageReferenceLocation hubLocation;
    UhEfmRotorFrameOrientation frameOrientation;
    double shaftTiltForwardRad;
    double nominalSpeedRadps;
    UhEfmRotorSpeedRange poweredSpeedRange;
    UhEfmRotorSpeedRange powerOffSpeedRange;
    double bladeFlapInertiaKgM2;
    double bladeMassKg;
    double bladeFirstMassMomentKgM;
    double lockNumber;
    UhEfmParameters attachedFlowIndicial;
    double flapSpringNmPerRad;
    double flapDampingNmsPerRad;
    double pitchFlapCouplingRad;
    double coningPerThrustRadPerNAtNominalSpeed;
    double teeterHingeUnderslingM;
    double teeterConingNaturalFrequencyPerRev;
    double teeterConingDampingRatio;
    double teeterPreconeRad;
    double gearRatioToMainRotor;
    uint32_t mainRotorWakeInteractionEnabled;
} UhEfmRotorSystemParameters;

typedef struct UhEfmPowerRatingParameters {
    const char* id;
    double maximumPowerW;
    double timeLimitS;
    double torqueGaugePsi;
} UhEfmPowerRatingParameters;

typedef struct UhEfmPowerplantParameters {
    const char* id;
    double ratedPowerW;
    UhEfmPowerRatingParameters powerRatings[4];
    uint32_t powerRatingCount;
    double mainShaftTorqueNmPerCalibratedPsi;
    double torqueIndicationReferenceMainShaftTorqueNm;
    double indicatedPsiAtReferenceTorque;
} UhEfmPowerplantParameters;

typedef struct UhEfmDrivetrainParameters {
    double equivalentMainShaftInertiaKgM2;
    double constantLossTorqueNm;
    double viscousLossTorqueNms;
    double quadraticLossTorqueNms2;
    double engineDriveEfficiency;
    double tailDriveEfficiency;
    double maximumPowerW;
    double maximumMainShaftTorqueNm;
    uint32_t freewheelClutchInstalled;
    double governorNaturalFrequencyRadps;
    double governorDampingRatio;
    double governorTorqueResponseTimeS;
    double governorMinimumFullScaleTorqueRiseTimeS;
    double governorMinimumFullScaleTorqueFallTimeS;
} UhEfmDrivetrainParameters;

typedef struct UhEfmCrossflowBodyParameters {
    const char* id;
    UhEfmFuselageReferenceLocation centerOfPressure;
    double forwardSideForceSlopeAreaM2PerRad;
    double downwashSideForceSlopeAreaM2PerRad;
    double broadsideDragAreaM2;
    double forwardNormalForceSlopeAreaM2PerRad;
    double normalDragAreaM2;
    uint32_t mainRotorWakeEnabled;
} UhEfmCrossflowBodyParameters;

typedef struct UhEfmTailRotorWakeSurfaceParameters {
    uint32_t enabled;
    uint32_t spanwisePanelCount;
    uint32_t chordwisePanelCount;
} UhEfmTailRotorWakeSurfaceParameters;

typedef struct UhEfmAerodynamicSurfaceControlSchedulePoint {
    double inputRad;
    double incidenceDeltaRad;
} UhEfmAerodynamicSurfaceControlSchedulePoint;

typedef struct UhEfmAerodynamicSurfaceParameters {
    const char* id;
    UhEfmAerodynamicSurfaceOrientation orientation;
    double areaM2;
    double aspectRatio;
    double incidenceRad;
    double dihedralRad;
    double liftCurveSlopePerRad;
    double zeroLiftDragCoefficient;
    double oswaldEfficiency;
    UhEfmAerodynamicSurfacePolarKind polarKind;
    double postStallBlendStartAngleRad;
    double postStallMatchAngleRad;
    double maximumDragCoefficient;
    UhEfmTailRotorWakeSurfaceParameters tailRotorWake;
    UhEfmAerodynamicSurfaceControlInput controlInput;
    UhEfmAerodynamicSurfaceControlSchedulePoint controlIncidenceSchedule[16];
    uint32_t controlIncidenceScheduleCount;
    UhEfmFuselageReferenceLocation centerOfPressure;
} UhEfmAerodynamicSurfaceParameters;

typedef struct UhEfmPerformanceMap {
    double standardTemperatureK;
    double standardPressurePa;
    double gasProducerRpmAt100Percent;
    double correctedN1Percent[12];
    double referredPowerFraction[12];
    uint32_t pointCount;
} UhEfmPerformanceMap;

typedef struct UhEfmInstalledTorqueLimitMap {
    double pressureAltitudeM[12];
    double freeAirTemperatureC[12];
    double maximumMainShaftTorqueNm[144];
    uint32_t altitudeCount;
    uint32_t temperatureCount;
} UhEfmInstalledTorqueLimitMap;

typedef struct UhEfmTurboshaftParameters {
    const char* id;
    double mainRotorNominalSpeedRadps;
    double groundIdleN2Ratio;
    double groundIdleMaximumThrottleCommand;
    double flightDetentMinimumThrottleCommand;
    double governedN2TargetPercent;
    double collectiveN2RisePercent;
    double collectiveN2RiseAtInput;
    double n2ErrorToN1Gain;
    double motorMinimumN1Percent;
    double starterMotoringN1Percent;
    double selfSustainingN1Percent;
    double idleN1Percent;
    double maximumN1Percent;
    double hotStartN1Percent;
    double starterAccelerationRatePercentPerS;
    double ignitionAccelerationRatePercentPerS;
    double n1ResponseGainPerS;
    double minimumN1RatePercentPerS;
    double maximumN1RatePercentPerS;
    double ignitionFuelFlowAtSustainKgPerHour;
    double ignitionFuelFlowExponent;
    double runningIdleFuelFlowKgPerHour;
    double runningMaximumFuelFlowKgPerHour;
    double hotStartTorqueNm;
    double hotStartTorqueGaugePsi;
    double hotStartOilTemperatureC;
    double hotStartOilPressurePa;
    double hotStartTurbineOutletTemperatureC;
    double ignitionPeakN1Percent;
    double ignitionPeakTemperatureC;
    double ignitionIdleTemperatureC;
    double earlyLightoffMinimumN1Percent;
    double earlyLightoffMaximumN1Percent;
    double earlyLightoffTemperatureRiseC;
    double runningTotSeaLevelStartC;
    double runningTotSeaLevelAtReferenceTorqueC;
    double runningTotHighAltitudeStartC;
    double runningTotHighAltitudeAtReferenceTorqueC;
    double runningTotHighAltitudeM;
    double runningTotReferenceTorquePsi;
    double runningTotAmbientGain;
    double runningTotRiseRateCPerS;
    double runningTotFallRateCPerS;
    double oilHeatingStartN1Percent;
    double oilTemperatureAtHeatingStartAboveAmbientC;
    double oilTemperatureAtMaximumN1AboveAmbientC;
    double oilTemperatureRateCPerS;
    double oilColdReferenceC;
    double oilWarmReferenceC;
    double oilColdPressureMultiplier;
    double oilWarmPressureMultiplier;
    double oilPressureLowN1Percent;
    double oilPressureMidN1Percent;
    double oilPressureHighN1Percent;
    double oilPressureAtLowN1Pa;
    double oilPressureAtMidN1Pa;
    double oilPressureAtHighN1Pa;
    double oilPressureAtMaximumN1Pa;
    double oilPressureRatePaPerS;
    double lowOilPressureCautionPa;
    UhEfmPerformanceMap performance;
    UhEfmInstalledTorqueLimitMap installedTorqueLimit;
} UhEfmTurboshaftParameters;

typedef struct UhEfmAttachedAirfoilParameters {
    double liftCurveSlopePerRad;
    double referenceLiftCurveSlopePerRad;
    double constantDragCoefficient;
    double linearDragCoefficient;
    double quadraticDragCoefficient;
    double attachedBlendStartRad;
    double attachedBlendEndRad;
} UhEfmAttachedAirfoilParameters;

typedef struct UhEfmRotorPresentationParameters {
    uint32_t runtimeBladeIndices[5];
    double negativeFlapFullScaleRad;
    double positiveFlapFullScaleRad;
    double droopClearSpeedFraction;
    double brakeNominalSpeedFraction;
    double brakeNominalTorqueNm;
    double brakeStoppedTorqueNm;
} UhEfmRotorPresentationParameters;

typedef struct UhEfmRotorControls {
    double collectiveRad;
    double cyclicCosRad;
    double cyclicSinRad;
    double swashplatePhaseRad;
    double collectiveRateRadps;
    double cyclicCosRateRadps;
    double cyclicSinRateRadps;
    double collectiveAccelerationRadps2;
    double cyclicCosAccelerationRadps2;
    double cyclicSinAccelerationRadps2;
} UhEfmRotorControls;

typedef struct UhEfmPeriodicConvergenceSettings {
    uint32_t stepsPerRevolution;
    uint32_t minimumRevolutions;
    uint32_t maximumRevolutions;
    double flapAngleToleranceRad;
    double flapRateToleranceRadps;
    double inflowRatioTolerance;
    double relativeMeanLoadTolerance;
    double sectionLiftCoefficientTolerance;
    double maximumAbsFlapAngleRad;
    double maximumAbsFlapRateRadps;
    double maximumAbsInflowRatio;
} UhEfmPeriodicConvergenceSettings;

typedef struct UhEfmInitializationParameters {
    double hotUniformInflowRatio;
    UhEfmRotorControls hotControls;
    UhEfmPeriodicConvergenceSettings periodic;
    uint32_t tailInflowMaximumIterations;
    double tailInflowToleranceMps;
} UhEfmInitializationParameters;

/* Each kind selects a working UH_EFM evaluator on either rotor. NACA 63-415
   reuses the existing R44 tail-section evaluator. Only ATTACHED_POLYNOMIAL
   uses the coefficients below, within the NACA0015 nonlinear envelope. */
typedef struct UhEfmAirfoilParameters {
    UhEfmAirfoilKind kind;
    UhEfmAttachedAirfoilParameters attached;
} UhEfmAirfoilParameters;

typedef struct UhEfmRotorComponent UhEfmRotorComponent;
typedef struct UhEfmTurboshaftComponent UhEfmTurboshaftComponent;
typedef struct UhEfmAirframeComponent UhEfmAirframeComponent;
typedef struct UhEfmDrivetrainComponent UhEfmDrivetrainComponent;
typedef struct UhEfmSurfaceComponent UhEfmSurfaceComponent;
typedef struct UhEfmCrossflowComponent UhEfmCrossflowComponent;
typedef struct UhEfmAircraftModel UhEfmAircraftModel;

UH_MODEL_API UhEfmRotorComponent* uh_efm_rotor_component_create(
    const UhEfmRotorSystemParameters*, const UhEfmAirfoilParameters*);
UH_MODEL_API UhEfmTurboshaftComponent* uh_efm_turboshaft_component_create(
    const UhEfmPowerplantParameters*, const UhEfmTurboshaftParameters*);
UH_MODEL_API UhEfmAirframeComponent* uh_efm_airframe_component_create(const UhEfmAirframeParameters*);
UH_MODEL_API UhEfmDrivetrainComponent* uh_efm_drivetrain_component_create(const UhEfmDrivetrainParameters*);
UH_MODEL_API UhEfmSurfaceComponent* uh_efm_surface_component_create(const UhEfmAerodynamicSurfaceParameters*);
UH_MODEL_API UhEfmCrossflowComponent* uh_efm_crossflow_component_create(const UhEfmCrossflowBodyParameters*);

UH_MODEL_API void uh_efm_rotor_component_destroy(UhEfmRotorComponent*);
UH_MODEL_API void uh_efm_turboshaft_component_destroy(UhEfmTurboshaftComponent*);
UH_MODEL_API void uh_efm_airframe_component_destroy(UhEfmAirframeComponent*);
UH_MODEL_API void uh_efm_drivetrain_component_destroy(UhEfmDrivetrainComponent*);
UH_MODEL_API void uh_efm_surface_component_destroy(UhEfmSurfaceComponent*);
UH_MODEL_API void uh_efm_crossflow_component_destroy(UhEfmCrossflowComponent*);

typedef struct UhEfmAircraftAssembly {
    const UhEfmAirframeComponent* airframe;
    const UhEfmDrivetrainComponent* drivetrain;
    const UhEfmRotorComponent* const* rotors;
    uint32_t rotorCount;
    const UhEfmTurboshaftComponent* const* engines;
    uint32_t engineCount;
    const UhEfmSurfaceComponent* const* surfaces;
    uint32_t surfaceCount;
    const UhEfmCrossflowComponent* const* crossflowBodies;
    uint32_t crossflowBodyCount;
    UhEfmRotorPresentationParameters presentation;
    UhEfmInitializationParameters initialization;
} UhEfmAircraftAssembly;

/* This companion currently exposes the five-blade, main/tail, single-engine
   DCS stepping contract. Final construction checks component compatibility.
   Arrays may be null only when their count is zero. No handle is consumed.
   On failure all existing handles remain valid and the result is null. */
UH_MODEL_API UhEfmAircraftModel* uh_efm_aircraft_create(const UhEfmAircraftAssembly*);
/* Last construction failure on this thread; valid until the next constructor. */
UH_MODEL_API const char* uh_efm_model_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* UH_EFM_MODEL_CONSTRUCTION_H */
