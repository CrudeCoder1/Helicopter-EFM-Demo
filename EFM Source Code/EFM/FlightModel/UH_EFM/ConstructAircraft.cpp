#include "ConstructAircraft.h"
#include "Parameters/Airframe.h"
#include "Parameters/MainRotor.h"
#include "Parameters/TailRotor.h"
#include "Parameters/Engine.h"
#include "Parameters/Drivetrain.h"
#include "Parameters/Aerodynamics.h"
#include "Parameters/ModelSettings.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
template<class T>
std::unique_ptr<T,void(*)(T*)> own(T* component,void(*destroy)(T*)) {
    if(!component)throw std::runtime_error(std::string("Cannot construct AH6J_UH_EFM component: ")
        +uh_efm_model_last_error());
    return {component,destroy};
}
}
namespace AH6J_UH_EFM {
UhEfmAh6jModel* createAircraft() {
    const auto airframeData=airframeParameters();
    auto airframe=own(uh_efm_airframe_component_create(&airframeData),uh_efm_airframe_component_destroy);
    const auto drivetrainData=drivetrainParameters();
    auto drivetrain=own(uh_efm_drivetrain_component_create(&drivetrainData),uh_efm_drivetrain_component_destroy);

    const auto mainData=mainRotorParameters();const auto mainAirfoil=mainAirfoilParameters();
    auto main=own(uh_efm_rotor_component_create(&mainData,&mainAirfoil),uh_efm_rotor_component_destroy);
    const auto tailData=tailRotorParameters();const auto tailAirfoil=tailAirfoilParameters();
    auto tail=own(uh_efm_rotor_component_create(&tailData,&tailAirfoil),uh_efm_rotor_component_destroy);
    const UhEfmRotorComponent* rotors[]={main.get(),tail.get()};

    const auto installation=powerplantParameters();const auto engineData=engineParameters();
    auto engine=own(uh_efm_turboshaft_component_create(&installation,&engineData),uh_efm_turboshaft_component_destroy);
    const UhEfmTurboshaftComponent* engines[]={engine.get()};

    using Surface=std::unique_ptr<UhEfmSurfaceComponent,void(*)(UhEfmSurfaceComponent*)>;
    std::vector<Surface> surfaces;
    std::vector<const UhEfmSurfaceComponent*> surfaceHandles;
    for(unsigned i=0;i<AH6J_UH_EFM::SurfaceCount;++i) {
        const auto data=surfaceParameters(i);
        surfaces.push_back(own(uh_efm_surface_component_create(&data),uh_efm_surface_component_destroy));
        surfaceHandles.push_back(surfaces.back().get());
    }
    using Crossflow=std::unique_ptr<UhEfmCrossflowComponent,void(*)(UhEfmCrossflowComponent*)>;
    std::vector<Crossflow> crossflow;
    std::vector<const UhEfmCrossflowComponent*> crossflowHandles;
    for(unsigned i=0;i<AH6J_UH_EFM::CrossflowBodyCount;++i) {
        const auto data=crossflowBodyParameters(i);
        crossflow.push_back(own(uh_efm_crossflow_component_create(&data),uh_efm_crossflow_component_destroy));
        crossflowHandles.push_back(crossflow.back().get());
    }

    UhEfmAircraftAssembly aircraft{};
    aircraft.airframe=airframe.get();aircraft.drivetrain=drivetrain.get();
    aircraft.rotors=rotors;aircraft.rotorCount=2;
    aircraft.engines=engines;aircraft.engineCount=1;
    aircraft.surfaces=surfaceHandles.data();aircraft.surfaceCount=AH6J_UH_EFM::SurfaceCount;
    aircraft.crossflowBodies=crossflowHandles.data();aircraft.crossflowBodyCount=AH6J_UH_EFM::CrossflowBodyCount;
    aircraft.presentation=presentationParameters();
    aircraft.initialization=initializationParameters();
    // UH_EFM assembles/validates the model and retains all component data.
    // Temporary handles below are released by their matching DLL destructors.
    return uh_efm_aircraft_create(&aircraft);
}
}
