#pragma once
#include "Aero.h"
#include "ModelSelection.h"
#include <memory>

class TurboshaftEngine;
class ElectricSystem;

// Preserve AH6Aero and its public callback queues. The default route delegates
// directly to it; only the explicitly selected AH6J_UH_EFM owns a UH_EFM runtime.
class SelectedFlightModel : public AH6Aero
{
  public:
    SelectedFlightModel(EFMData&, AH6JDamage&, FlightControls&);
    ~SelectedFlightModel();
    void selectModel();
    void InitializeOff();
    void InitializeOn();
    void updateFlightModel(double dt, bool hasFuel, TurboshaftEngine&, ElectricSystem&);
    void setBodyStates(double ax, double ay, double az, double vx, double vy, double vz, double wx,
                       double wy, double wz, double adx, double ady, double adz, double rx,
                       double ry, double rz, double yaw, double pitch, double roll);
    void setGroundPlane(double nx, double ny, double nz);
    void setPose(double height, double qx, double qy, double qz, double qw);
    void setRotorBrake(float);
    double getMRomega();
    double getTRomega();
    double getN2omega();
    double getN2PCT();

  private:
    struct AH6J_UH_EFM;
    std::unique_ptr<AH6J_UH_EFM> ah6jUhEfm_;
};
