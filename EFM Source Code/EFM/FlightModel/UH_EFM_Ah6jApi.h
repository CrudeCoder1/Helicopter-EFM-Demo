#ifndef UH_EFM_AH6J_API_H
#define UH_EFM_AH6J_API_H
#include <stdint.h>
#include "UH_EFM_ModelConstruction.h"

/* Declaration-only SDK. No physics implementation or C++ library types cross
   this boundary. Step input/output vectors use DCS body axes:
   +X forward, +Y up, +Z right. Construction geometry uses the separate
   conventions documented in UH_EFM_ModelConstruction.h. */
#if defined(UH_EFM_EXPORTS)
#define UH_AH6J_API __declspec(dllexport)
#else
#define UH_AH6J_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef UhEfmAircraftModel UhEfmAh6jModel;

/* Read-only rotor-side loss data from the model's aircraft configuration.
   All torques/speeds are referred to the main rotor shaft. Efficiencies
   must be in (0,1]; zero torque coefficients and unit efficiencies disable
   extra losses. Querying this data does not change the model. */
typedef struct UhEfmAh6jDrivetrainLosses {
    double constantTorqueNm;
    double viscousTorqueNms;
    double quadraticTorqueNms2;
    double engineDriveEfficiency;
    double tailDriveEfficiency;
} UhEfmAh6jDrivetrainLosses;
enum { UH_AH6J_API_VERSION=5, UH_AH6J_OFF=0, UH_AH6J_MOTORING=1,
       UH_AH6J_IGNITION=2, UH_AH6J_RUNNING=3 };

typedef struct UhEfmAh6jReset {
    uint32_t hot;
    double ambientTemperatureC;
    double densityKgM3;
    double speedOfSoundMps;
} UhEfmAh6jReset;

typedef struct UhEfmAh6jInput {
    double timeStepS;
    double densityKgM3;
    double speedOfSoundMps;
    double ambientTemperatureC;
    double ambientPressurePa;
    double altitudeMslM;
    double heightM;
    double surfaceHeightM;
    double orientationQuaternion[4]; /* x,y,z,w; DCS body to world */
    double surfaceNormalWorld[3];
    double bodyAirVelocityMps[3];
    double bodyLinearAccelerationMps2[3];
    double bodyAngularVelocityRadps[3];
    double bodyAngularAccelerationRadps2[3];
    double collective; /* [0,1]; remaining flight controls [-1,1] */
    double lateralCyclic;
    double longitudinalCyclic;
    double pedal;
    double throttle; /* -1 cutoff, 0 idle, 1 full */
    double mainBladeIntegrity[5]; /* DCS/EDM blade order; reordered inside the DLL */
    uint32_t hasFuel;
    uint32_t starterEngaged;
    uint32_t rotorBrakeEngaged;
} UhEfmAh6jInput;

typedef struct UhEfmAh6jOutput {
    uint32_t valid;
    uint32_t engineState;
    double forceN[3];
    double momentNm[3]; /* complete moment about configuration reference origin */
    double mainRotorSpeedRadps;
    double tailRotorSpeedRadps;
    double engineShaftSpeedRadps;
    double engineN2Percent;
    double rotorNrPercent;
    double n1Percent;
    double fuelFlowKgPerHour;
    double engineTorqueNm;
    double torqueGaugePsi;
    double mainRotorSpinArgument;
    double tailRotorSpinArgument;
    double droopArgument;
    double bladeFlapArgument[5]; /* DCS arguments 280..284, in EDM blade order */
} UhEfmAh6jOutput;

UH_AH6J_API unsigned uh_efm_ah6j_api_version(void);
/* Construct the model with the typed component API in UH_EFM_ModelConstruction.h.
   The returned aircraft handle uses these DCS reset/step/output operations. */
/* Returns 1 on success, 0 for a null model/output. The output is a copy. */
UH_AH6J_API int uh_efm_ah6j_get_drivetrain_losses(const UhEfmAh6jModel*, UhEfmAh6jDrivetrainLosses*);
UH_AH6J_API void uh_efm_ah6j_destroy(UhEfmAh6jModel*);
UH_AH6J_API int uh_efm_ah6j_reset(UhEfmAh6jModel*, const UhEfmAh6jReset*, UhEfmAh6jOutput*);
UH_AH6J_API int uh_efm_ah6j_step(UhEfmAh6jModel*, const UhEfmAh6jInput*, UhEfmAh6jOutput*);

#ifdef __cplusplus
}
#endif

#endif /* UH_EFM_AH6J_API_H */
