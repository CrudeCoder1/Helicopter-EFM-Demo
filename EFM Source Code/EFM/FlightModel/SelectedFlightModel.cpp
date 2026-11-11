#include "SelectedFlightModel.h"
#include "../Systems/Engine.h"
#include "UH_EFM_Ah6jApi.h"
#include "UH_EFM/ConstructAircraft.h"
#include <stdexcept>
#include <string>

// Fork-owned aircraft data configures the shared physics/stepping DLL.
struct SelectedFlightModel::AH6J_UH_EFM
{
    UhEfmAh6jModel* model = nullptr;
    UhEfmAh6jInput input{};
    UhEfmAh6jOutput output{};
    AH6J_UH_EFM()
    {
        if (uh_efm_ah6j_api_version() != UH_AH6J_API_VERSION)
            throw std::runtime_error("Unsupported AH6J_UH_EFM interface version");
        model = ::AH6J_UH_EFM::createAircraft();
        if (!model)
            throw std::runtime_error(std::string("Cannot create AH6J_UH_EFM model: ")+uh_efm_model_last_error());
        input.orientationQuaternion[3] = 1;
        input.surfaceNormalWorld[1] = 1;
    }
    ~AH6J_UH_EFM()
    {
        uh_efm_ah6j_destroy(model);
    }
    void reset(bool hot, const EFMData& data)
    {
        UhEfmAh6jReset state{};
        state.hot = hot ? 1 : 0;
        state.ambientTemperatureC = data.ambientTemp_C;
        state.densityKgM3 = data.rho;
        state.speedOfSoundMps = data.speedOfSound;
        input.rotorBrakeEngaged = 0;
        if (!uh_efm_ah6j_reset(model, &state, &output))
            throw std::runtime_error("Cannot initialize AH6J_UH_EFM model");
    }
};
SelectedFlightModel::SelectedFlightModel(EFMData& data, AH6JDamage& damage,
                                         FlightControls& controls)
    : AH6Aero(data, damage, controls)
{
    selectModel();
}
SelectedFlightModel::~SelectedFlightModel() = default;
void SelectedFlightModel::selectModel()
{
    ah6jUhEfm_.reset();
    if (Ah6jModelSelection::ah6jUhEfmSelected())
        ah6jUhEfm_.reset(new AH6J_UH_EFM);
}
void SelectedFlightModel::InitializeOff()
{
    AH6Aero::InitializeOff();
    if (ah6jUhEfm_)
        ah6jUhEfm_->reset(false, *p_EFMdata);
}
void SelectedFlightModel::InitializeOn()
{
    AH6Aero::InitializeOn();
    if (ah6jUhEfm_)
        ah6jUhEfm_->reset(true, *p_EFMdata);
}
void SelectedFlightModel::setPose(double height, double x, double y, double z, double w)
{
    if (ah6jUhEfm_)
    {
        auto& in = ah6jUhEfm_->input;
        in.heightM = height;
        in.orientationQuaternion[0] = x;
        in.orientationQuaternion[1] = y;
        in.orientationQuaternion[2] = z;
        in.orientationQuaternion[3] = w;
    }
}
void SelectedFlightModel::setGroundPlane(double x, double y, double z)
{
    if (ah6jUhEfm_)
    {
        auto& in = ah6jUhEfm_->input;
        in.surfaceNormalWorld[0] = x;
        in.surfaceNormalWorld[1] = y;
        in.surfaceNormalWorld[2] = z;
    }
}
void SelectedFlightModel::setBodyStates(double ax, double ay, double az, double vx, double vy,
                                        double vz, double wx, double wy, double wz, double adx,
                                        double ady, double adz, double rx, double ry, double rz,
                                        double yaw, double pitch, double roll)
{
    AH6Aero::setBodyStates(ax, ay, az, vx, vy, vz, wx, wy, wz, adx, ady, adz, rx, ry, rz, yaw,
                           pitch, roll);
    if (ah6jUhEfm_)
    {
        auto& in = ah6jUhEfm_->input;
        in.bodyAirVelocityMps[0] = vx - wx;
        in.bodyAirVelocityMps[1] = vy - wy;
        in.bodyAirVelocityMps[2] = vz - wz;
        in.bodyLinearAccelerationMps2[0] = ax;
        in.bodyLinearAccelerationMps2[1] = ay;
        in.bodyLinearAccelerationMps2[2] = az;
        in.bodyAngularVelocityRadps[0] = rx;
        in.bodyAngularVelocityRadps[1] = ry;
        in.bodyAngularVelocityRadps[2] = rz;
        in.bodyAngularAccelerationRadps2[0] = adx;
        in.bodyAngularAccelerationRadps2[1] = ady;
        in.bodyAngularAccelerationRadps2[2] = adz;
    }
}
void SelectedFlightModel::setRotorBrake(float value)
{
    AH6Aero::setRotorBrake(value);
    if (ah6jUhEfm_)
        ah6jUhEfm_->input.rotorBrakeEngaged = value > 0 ? 1 : 0;
}
double SelectedFlightModel::getMRomega()
{
    return ah6jUhEfm_ ? ah6jUhEfm_->output.mainRotorSpeedRadps : AH6Aero::getMRomega();
}
double SelectedFlightModel::getTRomega()
{
    return ah6jUhEfm_ ? ah6jUhEfm_->output.tailRotorSpeedRadps : AH6Aero::getTRomega();
}
double SelectedFlightModel::getN2omega()
{
    return ah6jUhEfm_ ? ah6jUhEfm_->output.engineShaftSpeedRadps : AH6Aero::getN2omega();
}
double SelectedFlightModel::getN2PCT()
{
    return ah6jUhEfm_ ? ah6jUhEfm_->output.engineN2Percent : AH6Aero::getN2PCT();
}
void SelectedFlightModel::updateFlightModel(double dt, bool hasFuel, TurboshaftEngine& instruments,
                                            ElectricSystem& electrics)
{
    if (!ah6jUhEfm_)
    {
        instruments.update(dt, hasFuel, AH6Aero::getN2PCT());
        AH6Aero::update(instruments.getEngTorque());
        return;
    }
    auto& runtime = *ah6jUhEfm_;
    auto& in = runtime.input;
    aeroForces.clear();
    aeroMoments.clear();
    if (!(dt > 0) || !std::isfinite(dt) || !(p_EFMdata->rho > 0) || !(p_EFMdata->speedOfSound > 0))
        return;
    in.timeStepS = dt;
    in.hasFuel = hasFuel ? 1 : 0;
    in.starterEngaged = electrics.isStarterEngaged() ? 1 : 0;
    in.throttle = instruments.throttleInput;
    in.collective = p_flightControl->CollectiveInput;
    in.lateralCyclic = p_flightControl->rollOutput;
    in.longitudinalCyclic = p_flightControl->pitchOutput;
    in.pedal = p_flightControl->PedalInput;
    in.densityKgM3 = p_EFMdata->rho;
    in.speedOfSoundMps = p_EFMdata->speedOfSound;
    in.ambientTemperatureC = p_EFMdata->ambientTemp_C;
    in.ambientPressurePa = p_EFMdata->airPressure;
    in.altitudeMslM = p_EFMdata->altitudeMSL;
    in.surfaceHeightM = p_EFMdata->surfaceAlt;
    for (int blade = 0; blade < 5; ++blade)
        in.mainBladeIntegrity[blade] = p_Damage->elementIntegrity[BLADE_1_CENTER + 3 * blade];
    UhEfmAh6jOutput result{};
    if (!uh_efm_ah6j_step(runtime.model, &in, &result))
        return;
    runtime.output = result;
    ForceComponent force;
    force.dir = Vec3(result.forceN[0], result.forceN[1], result.forceN[2]);
    force.pos = Vec3(0, 0, 0);
    aeroForces.push_back(force);
    aeroMoments.push_back(Vec3(result.momentNm[0], result.momentNm[1], result.momentNm[2]));
    instruments.updateFromFlightModel(result);
    cockpitAPI.setParamNumber(EFM_N2_RPM, result.engineN2Percent);
    cockpitAPI.setParamNumber(EFM_NR_RPM, result.rotorNrPercent);
    cockpitAPI.setExternalDrawArg(EXT_RotorSpin, static_cast<float>(result.mainRotorSpinArgument));
    cockpitAPI.setExternalDrawArg(EXT_TRspin, static_cast<float>(result.tailRotorSpinArgument));
    cockpitAPI.setExternalDrawArg(EXT_RotorDroop, static_cast<float>(result.droopArgument));
    for (int blade = 0; blade < 5; ++blade)
        cockpitAPI.setExternalDrawArg(EXT_Blade1Flap + blade,
                                      static_cast<float>(result.bladeFlapArgument[blade]));
}
