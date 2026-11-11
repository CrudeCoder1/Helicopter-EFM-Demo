#pragma once
#include "../../UH_EFM_ModelConstruction.h"

// AH6J fuselage/boom strips and military-family T-tail surfaces.
// SI units. Army AEFA AD-A218253 family T-tail; J station/rigging extrapolations provisional.
namespace AH6J_UH_EFM {
constexpr unsigned CrossflowBodyCount = 11;
constexpr unsigned SurfaceCount = 4;

inline UhEfmCrossflowBodyParameters crossflowBodyParameters(unsigned index) {
    UhEfmCrossflowBodyParameters p{};
    switch(index) {
    case 0:
        p.id = "ah6j_cabin_nose";
        p.centerOfPressure.stationM = 1.2065;
        p.centerOfPressure.waterlineM = 1.143;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 1.5888217063237333;
        p.downwashSideForceSlopeAreaM2PerRad = 0;
        p.broadsideDragAreaM2 = 0.4112995520181461;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = false;
        break;
    case 1:
        p.id = "ah6j_cabin_forward";
        p.centerOfPressure.stationM = 1.8415;
        p.centerOfPressure.waterlineM = 1.2191999999999998;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 2.4299626096715925;
        p.downwashSideForceSlopeAreaM2PerRad = 0;
        p.broadsideDragAreaM2 = 0.6290463736748118;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = false;
        break;
    case 2:
        p.id = "ah6j_cabin_mid";
        p.centerOfPressure.stationM = 2.4764999999999997;
        p.centerOfPressure.waterlineM = 1.27;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 2.897263111531514;
        p.downwashSideForceSlopeAreaM2PerRad = 0;
        p.broadsideDragAreaM2 = 0.7500168301507372;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = false;
        break;
    case 3:
        p.id = "ah6j_cabin_aft";
        p.centerOfPressure.stationM = 3.1115;
        p.centerOfPressure.waterlineM = 1.2953999999999999;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 2.523422710043577;
        p.downwashSideForceSlopeAreaM2PerRad = 0;
        p.broadsideDragAreaM2 = 0.6532404649699968;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = false;
        break;
    case 4:
        p.id = "ah6j_cabin_cowling";
        p.centerOfPressure.stationM = 3.7464999999999997;
        p.centerOfPressure.waterlineM = 1.397;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 1.6822818066957177;
        p.downwashSideForceSlopeAreaM2PerRad = 0;
        p.broadsideDragAreaM2 = 0.43549364331333124;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = false;
        break;
    case 5:
        p.id = "ah6j_boom_1";
        p.centerOfPressure.stationM = 4.064;
        p.centerOfPressure.waterlineM = 1.524;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 0;
        p.downwashSideForceSlopeAreaM2PerRad = 0.3249168;
        p.broadsideDragAreaM2 = 0.3249168;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = true;
        break;
    case 6:
        p.id = "ah6j_boom_2";
        p.centerOfPressure.stationM = 4.572;
        p.centerOfPressure.waterlineM = 1.6256;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 0;
        p.downwashSideForceSlopeAreaM2PerRad = 0.2999232;
        p.broadsideDragAreaM2 = 0.2999232;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = true;
        break;
    case 7:
        p.id = "ah6j_boom_3";
        p.centerOfPressure.stationM = 5.08;
        p.centerOfPressure.waterlineM = 1.651;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 0;
        p.downwashSideForceSlopeAreaM2PerRad = 0.26243279999999997;
        p.broadsideDragAreaM2 = 0.26243279999999997;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = true;
        break;
    case 8:
        p.id = "ah6j_boom_4";
        p.centerOfPressure.stationM = 5.588;
        p.centerOfPressure.waterlineM = 1.6256;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 0;
        p.downwashSideForceSlopeAreaM2PerRad = 0.2249424;
        p.broadsideDragAreaM2 = 0.2249424;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = true;
        break;
    case 9:
        p.id = "ah6j_boom_5";
        p.centerOfPressure.stationM = 6.096;
        p.centerOfPressure.waterlineM = 1.5748;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 0;
        p.downwashSideForceSlopeAreaM2PerRad = 0.187452;
        p.broadsideDragAreaM2 = 0.187452;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = true;
        break;
    case 10:
        p.id = "ah6j_boom_6";
        p.centerOfPressure.stationM = 6.604;
        p.centerOfPressure.waterlineM = 1.4986;
        p.centerOfPressure.buttlineM = 0;
        p.forwardSideForceSlopeAreaM2PerRad = 0;
        p.downwashSideForceSlopeAreaM2PerRad = 0.1499616;
        p.broadsideDragAreaM2 = 0.1499616;
        p.forwardNormalForceSlopeAreaM2PerRad = 0;
        p.normalDragAreaM2 = 0;
        p.mainRotorWakeEnabled = true;
        break;
    default: break;
    }
    return p;
}

inline UhEfmAerodynamicSurfaceParameters surfaceParameters(unsigned index) {
    UhEfmAerodynamicSurfaceParameters p{};
    switch(index) {
    case 0:
        p.id = "ah6j_t_tail_horizontal";
        p.orientation = UH_EFM_AERODYNAMIC_SURFACE_ORIENTATION_HORIZONTAL;
        p.areaM2 = 0.7599468672;
        p.aspectRatio = 3.472970660146699;
        p.incidenceRad = -0.01231318515194986;
        p.dihedralRad = 0;
        p.liftCurveSlopePerRad = 3.9871067431438822;
        p.zeroLiftDragCoefficient = 0.015;
        p.oswaldEfficiency = 0.8;
        p.polarKind = UH_EFM_AERODYNAMIC_SURFACE_POLAR_KIND_VITERNA360;
        p.postStallBlendStartAngleRad = 0.2617993877991494;
        p.postStallMatchAngleRad = 0.3490658503988659;
        p.maximumDragCoefficient = 1.1725134718826407;
        p.tailRotorWake.enabled = false;
        p.tailRotorWake.spanwisePanelCount = 1;
        p.tailRotorWake.chordwisePanelCount = 1;
        p.controlInput = UH_EFM_AERODYNAMIC_SURFACE_CONTROL_INPUT_NONE;
        p.controlIncidenceScheduleCount = 0;
        p.centerOfPressure.stationM = 7.366;
        p.centerOfPressure.waterlineM = 2.286;
        p.centerOfPressure.buttlineM = 0;
        break;
    case 1:
        p.id = "ah6j_t_tail_central_fin";
        p.orientation = UH_EFM_AERODYNAMIC_SURFACE_ORIENTATION_VERTICAL;
        p.areaM2 = 0.5620633920000001;
        p.aspectRatio = 5.445884297520661;
        p.incidenceRad = 0;
        p.dihedralRad = 0;
        p.liftCurveSlopePerRad = 4.595491795940964;
        p.zeroLiftDragCoefficient = 0.015;
        p.oswaldEfficiency = 0.8;
        p.polarKind = UH_EFM_AERODYNAMIC_SURFACE_POLAR_KIND_VITERNA360;
        p.postStallBlendStartAngleRad = 0.2617993877991494;
        p.postStallMatchAngleRad = 0.3490658503988659;
        p.maximumDragCoefficient = 1.208025917355372;
        p.tailRotorWake.enabled = true;
        p.tailRotorWake.spanwisePanelCount = 8;
        p.tailRotorWake.chordwisePanelCount = 2;
        p.controlInput = UH_EFM_AERODYNAMIC_SURFACE_CONTROL_INPUT_NONE;
        p.controlIncidenceScheduleCount = 0;
        p.centerOfPressure.stationM = 7.239;
        p.centerOfPressure.waterlineM = 1.4507951008264461;
        p.centerOfPressure.buttlineM = 0;
        break;
    case 2:
        p.id = "ah6j_t_tail_left_tip_plate";
        p.orientation = UH_EFM_AERODYNAMIC_SURFACE_ORIENTATION_VERTICAL;
        p.areaM2 = 0.1319223168;
        p.aspectRatio = 0.9999999999999999;
        p.incidenceRad = 0;
        p.dihedralRad = 0;
        p.liftCurveSlopePerRad = 2.0943951023931953;
        p.zeroLiftDragCoefficient = 0.015;
        p.oswaldEfficiency = 0.8;
        p.polarKind = UH_EFM_AERODYNAMIC_SURFACE_POLAR_KIND_VITERNA360;
        p.postStallBlendStartAngleRad = 0.2617993877991494;
        p.postStallMatchAngleRad = 0.3490658503988659;
        p.maximumDragCoefficient = 1.1280000000000001;
        p.tailRotorWake.enabled = false;
        p.tailRotorWake.spanwisePanelCount = 1;
        p.tailRotorWake.chordwisePanelCount = 1;
        p.controlInput = UH_EFM_AERODYNAMIC_SURFACE_CONTROL_INPUT_NONE;
        p.controlIncidenceScheduleCount = 0;
        p.centerOfPressure.stationM = 7.366;
        p.centerOfPressure.waterlineM = 2.286;
        p.centerOfPressure.buttlineM = -0.812292;
        break;
    case 3:
        p.id = "ah6j_t_tail_right_tip_plate";
        p.orientation = UH_EFM_AERODYNAMIC_SURFACE_ORIENTATION_VERTICAL;
        p.areaM2 = 0.1319223168;
        p.aspectRatio = 0.9999999999999999;
        p.incidenceRad = 0;
        p.dihedralRad = 0;
        p.liftCurveSlopePerRad = 2.0943951023931953;
        p.zeroLiftDragCoefficient = 0.015;
        p.oswaldEfficiency = 0.8;
        p.polarKind = UH_EFM_AERODYNAMIC_SURFACE_POLAR_KIND_VITERNA360;
        p.postStallBlendStartAngleRad = 0.2617993877991494;
        p.postStallMatchAngleRad = 0.3490658503988659;
        p.maximumDragCoefficient = 1.1280000000000001;
        p.tailRotorWake.enabled = false;
        p.tailRotorWake.spanwisePanelCount = 1;
        p.tailRotorWake.chordwisePanelCount = 1;
        p.controlInput = UH_EFM_AERODYNAMIC_SURFACE_CONTROL_INPUT_NONE;
        p.controlIncidenceScheduleCount = 0;
        p.centerOfPressure.stationM = 7.366;
        p.centerOfPressure.waterlineM = 2.286;
        p.centerOfPressure.buttlineM = 0.812292;
        break;
    default: break;
    }
    return p;
}

}
