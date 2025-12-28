//--------------------------------------------------------------------------
// Helicopter External Flight Model for DCS World using Blade Element Theory 
//--------------------------------------------------------------------------
// IMPORTANT!  COORDINATE CONVENTION:
//
// DCS WORLD Convention:
// Xbody: Out the front of the nose
// Ybody: Out the top of the aircraft
// Zbody: Out the right wing
//
// Normal Aerodynamics/Control Convention:
// Xbody: Out the front of the nose
// Ybody: Out the right wing
// Zbody: Out the bottom of the aircraft
//
// This means that if you are referincing from any aerodynamic, stabilty, or control document
// they are probably using the second set of directions.  Which means you always need to switch
// the Y and the Z and reverse the Y prior to output to DCS World
//---------------------------------------------------------------------------
#include "AH6.h"



//====================================================================================
// Functions called before ed_fm_simulate
//====================================================================================
 
//called before simulation to set up your environment for the next step
//give parameters of surface under your aircraft
void ed_fm_set_surface( double		h,//height of surface under the center of aircraft (meters)
						double		h_obj,//height of surface with objects
						unsigned	surface_type,
						double		normal_x,//components of normal vector to surface
						double		normal_y,//components of normal vector to surface
						double		normal_z//components of normal vector to surface
)
{
	EFMdata.setSurface(h_obj, surface_type);
}

// called before simulation to set up your environment for the next step
void ed_fm_set_atmosphere(double h,	//altitude above sea level	(meters)
						  double t,	//current atmosphere temperature   (Kelvin)
						  double a,	//speed of sound					(meters/sec)
						  double ro,// atmosphere density				(kg/m^3)
						  double p,	// atmosphere pressure				(N/m^2)
						  double wind_vx,//components of velocity vector, including turbulence in world coordinate system (meters/sec)
						  double wind_vy,//components of velocity vector, including turbulence in world coordinate system (meters/sec)
						  double wind_vz //components of velocity vector, including turbulence in world coordinate system (meters/sec)
)
{
	EFMdata.setAtmosphere(h, t, a, ro, p);
}

void ed_fm_set_current_mass_state(double mass,// "dry" mass in [kg] (Doesn't include fuel or pylons, includes weapons); fuel mass must be added using ed_fm_change_mass()
								double center_of_mass_x,
								double center_of_mass_y,
								double center_of_mass_z,
								double moment_of_inertia_x,
								double moment_of_inertia_y,
								double moment_of_inertia_z
)
{
	EFMdata.setMassState(mass, center_of_mass_x, center_of_mass_y, center_of_mass_z, moment_of_inertia_x, moment_of_inertia_y, moment_of_inertia_z);
}

//called before simulation to set up your environment for the next step
// World axis/orientation and coordinates
void ed_fm_set_current_state(double ax,//linear acceleration component in world coordinate system
							double ay,//linear acceleration component in world coordinate system
							double az,//linear acceleration component in world coordinate system
							double vx,//linear velocity component in world coordinate system
							double vy,//linear velocity component in world coordinate system
							double vz,//linear velocity component in world coordinate system
							double px,//center of the body position in world coordinate system
							double py,//center of the body position in world coordinate system
							double pz,//center of the body position in world coordinate system
							double omegadotx,//angular accelearation components in world coordinate system
							double omegadoty,//angular accelearation components in world coordinate system
							double omegadotz,//angular accelearation components in world coordinate system
							double omegax,//angular velocity components in world coordinate system
							double omegay,//angular velocity components in world coordinate system
							double omegaz,//angular velocity components in world coordinate system
							double quaternion_x,//orientation quaternion components in world coordinate system
							double quaternion_y,//orientation quaternion components in world coordinate system
							double quaternion_z,//orientation quaternion components in world coordinate system
							double quaternion_w //orientation quaternion components in world coordinate system
)
{

}

// Body axis
void ed_fm_set_current_state_body_axis(double ax,//linear acceleration component in body coordinate system (meters/sec^2)
									double ay,//linear acceleration component in body coordinate system (meters/sec^2)
									double az,//linear acceleration component in body coordinate system (meters/sec^2)
									double vx,//linear velocity component in body coordinate system (meters/sec)
									double vy,//linear velocity component in body coordinate system (meters/sec)
									double vz,//linear velocity component in body coordinate system (meters/sec)
									double wind_vx,//wind linear velocity component in body coordinate system (meters/sec)
									double wind_vy,//wind linear velocity component in body coordinate system (meters/sec)
									double wind_vz,//wind linear velocity component in body coordinate system (meters/sec)
									double omegadotx,//angular accelearation components in body coordinate system (rad/sec^2)
									double omegadoty,//angular accelearation components in body coordinate system (rad/sec^2)
									double omegadotz,//angular accelearation components in body coordinate system (rad/sec^2)
									double omegax,//angular velocity components in body coordinate system (rad/sec)
									double omegay,//angular velocity components in body coordinate system (rad/sec)
									double omegaz,//angular velocity components in body coordinate system (rad/sec)
									double yaw,  //radians (rad)
									double pitch,//radians (rad)
									double roll, //radians (rad)
									double common_angle_of_attack, //AoA  (rad)
									double common_angle_of_slide   //AoS  (rad)
)
{
	Aero.setBodyStates(ax, ay, az, vx, vy, vz, wind_vx, wind_vy, wind_vz, omegadotx, omegadoty, omegadotz, omegax, omegay, omegaz, yaw, pitch, roll);	
	EFMdata.setCurrentBodyState(ax, ay, az, vx, vy, vz, wind_vx, wind_vy, wind_vz, omegadotx, omegadoty, omegadotz, omegax, omegay, omegaz, yaw, pitch, roll);
}


//====================================================================================
// ed_fm_simulate
// This is where your code gets called for each run frame. 
// Each run frame last for a duration of "dt" (delta time) = 0.006 seconds.
//====================================================================================
void ed_fm_simulate(double dt)
{
	EFMdata.update(dt);

	flightControls.update();
	Engine.update(dt, Fuel.isFuelFlow, Aero.getN2PCT());
	Aero.update(Engine.getEngTorque());
	Fuel.update(Engine.getFuelFlow(), dt); // note, still uses fuel when sim is paused
	Electrics.update(dt, Engine.getN1RPM());

}

//====================================================================================
// Functions called after ed_fm_simulate
//====================================================================================

// Send individual forces after each run frame, function will be called until return value is true
bool ed_fm_add_local_force_component(double& x, double& y, double& z, double& pos_x, double& pos_y, double& pos_z)
{
	if (!Aero.aeroForces.empty())
	{
		ForceComponent Frc = Aero.aeroForces.back();//load last force vector into local variable
		Aero.aeroForces.pop_back();//delete last force

		x = Frc.dir.x;//send forces to DCS
		y = Frc.dir.y;
		z = Frc.dir.z;
		pos_x = Frc.pos.x;
		pos_y = Frc.pos.y;
		pos_z = Frc.pos.z;
		return true;
	}
	return false;
}

// Send individual moments after each run frame, function will be called until return value is true
bool ed_fm_add_local_moment_component(double& x, double& y, double& z)
{
	if (!Aero.aeroMoments.empty())
	{
		Vec3 mnt = Aero.aeroMoments.back();//load last moment vector into local variable
		Aero.aeroMoments.pop_back();//delete last moment

		x = mnt.x;//send moments to DCS
		y = mnt.y;
		z = mnt.z;
		return true;
	}
	return false;
}

// This is where the simulation send the accumulated forces to the DCS Simulation after each run frame
void ed_fm_add_local_force(double& x, double& y, double& z, double& pos_x, double& pos_y, double& pos_z)
{
	// Only one force can be sent here, therefore in this EFM template we will use force_component
	// so we can have localized forces
}
// This is where the simulation send the accumulated moments to the DCS Simulation after each run frame
void ed_fm_add_local_moment(double& x, double& y, double& z)
{
}

bool ed_fm_change_mass(double& delta_mass,
	double& delta_mass_pos_x,
	double& delta_mass_pos_y,
	double& delta_mass_pos_z,
	double& delta_mass_moment_of_inertia_x,
	double& delta_mass_moment_of_inertia_y,
	double& delta_mass_moment_of_inertia_z
)
{// NOTE: total fuel mass must be added on first frame or initialization
	if (!Fuel.fuelMassDelta.empty())
	{
		double Fd = Fuel.fuelMassDelta.back();
		Fuel.fuelMassDelta.pop_back();
		delta_mass = Fd;
		return true;
	}
	return false;
}

//====================================================================================
// Functions called on initialization
//====================================================================================

// These 3 functions called at beginning of every mission, use to initialize systems
void ed_fm_cold_start()
{
	Engine.initCold();
	Fuel.initCold();
	Electrics.initCold();
	damageModel.init();
	Aero.InitializeOff();
}

void ed_fm_hot_start()
{
	Engine.initHot();
	Fuel.initHot();
	Electrics.initHot();
	damageModel.init();
	Aero.InitializeOn();
}

void ed_fm_hot_start_in_air()
{
	Engine.initHot();
	Fuel.initHot();
	Electrics.initHot();
	damageModel.init();
	Aero.InitializeOn();
}

//	set internal fuel volume, called on initialization, 
//	you should distribute it inside at different fuel tanks
void ed_fm_set_internal_fuel(double fuel)
{
	Fuel.setInternalFuel(fuel);
}

//set external fuel volume for each payload station, called for weapon init and on reload
void ed_fm_set_external_fuel(int station, double fuel, double x, double y, double z)
{
}

// inform about invulnerability settings
void ed_fm_set_immortal(bool value)
{
	damageModel.setImmortal(value);
}

// inform about unlimited fuel
void ed_fm_unlimited_fuel(bool value)
{
	Fuel.setUnlimitedFuel(value);
}

// inform about simplified flight model request 
void ed_fm_set_easy_flight(bool value)
{
}

/*
for experts only : called  after ed_fm_hot_start_in_air for balance FM at actual speed and height,
it is directly force aircraft dynamic data in case of success
*/
bool ed_fm_make_balance(double& ax,//linear acceleration component in world coordinate system);
	double& ay,//linear acceleration component in world coordinate system
	double& az,//linear acceleration component in world coordinate system
	double& vx,//linear velocity component in world coordinate system
	double& vy,//linear velocity component in world coordinate system
	double& vz,//linear velocity component in world coordinate system
	double& omegadotx,//angular accelearation components in world coordinate system
	double& omegadoty,//angular accelearation components in world coordinate system
	double& omegadotz,//angular accelearation components in world coordinate system
	double& omegax,//angular velocity components in world coordinate system
	double& omegay,//angular velocity components in world coordinate system
	double& omegaz,//angular velocity components in world coordinate system
	double& yaw,  //radians
	double& pitch,//radians
	double& roll)//radians
{
	return false;
}

//====================================================================================
// Functions called when required
//====================================================================================

// See Inputs.h
// Command = Command Index (See Cockpit\Scripts\command_defs.lua), Value = Signal Value (-1 to 1 for Joystick Axis)
void ed_fm_set_command(int command, float value)
{
	float device_id = 0; // See Cockpit\Scripts\devices.lua
	if (value > 1.0) // if the command comes from clickabledata.lua it adds the device id to the value and changes the value
	{
		float normalized = modf(value, &device_id);
		value = normalized * 8.f - 2.f;
	}

	//switch ((int)device_id) // sort inputs by device
	//{
	//case ELECTRIC_SYSTEM_DEVICE:
		//Electrics.setCommand(command, value);
		//break;
	//default:
	//	break;
	//}

	switch (command)
	{
	case JoystickRoll:
		flightControls.RollInput = limit(value, -1.0, 1.0);
		break;

	case JoystickPitch:
		flightControls.PitchInput = limit(-value, -1.0, 1.0);
		break;

	case JoystickYaw:
		flightControls.PedalInput = limit(-value, -1.0, 1.0);
		break;

	case JoystickThrottle:
		flightControls.CollectiveInput = limit(((-value + 1.0) / 2.0) , 0.0, 1.0);
		break;

	case trimUp:
		flightControls.pitchTrim = limit(flightControls.pitchTrim - 0.0015, -1, 1);
		break;
	case trimDown:
		flightControls.pitchTrim = limit(flightControls.pitchTrim + 0.0015, -1, 1);
		break;
	case trimLeft:
		flightControls.rollTrim = limit(flightControls.rollTrim - 0.0015, -1, 1);
		break;
	case trimRight:
		flightControls.rollTrim = limit(flightControls.rollTrim + 0.0015, -1, 1);
		break;
	
	case starterButton:
		Engine.starterButtonOn = value > 0.0;
		break;
	

	case throttle:
		Engine.throttleInput = value;
		Fuel.setThrottle(value);
		break;
	case throttleAxis:
		//Engine.setThrottleInput((-value + 1.0) / 2.0);
		break;
		
	case batterySwitch:
		Electrics.setPowerSw(value);
			break;

	case generatorSwitch:
		Electrics.setGeneratorSw(value);
		break;

	case inverterSwitch:
		Electrics.setInverterSw(value);
		break;

	case rotorBrake:
		Aero.setRotorBrake(value);

	default:
		break;
	}
}


// called on ground refuel
void ed_fm_refueling_add_fuel(double fuel)
{
	return Fuel.refuelAdd(fuel);
}

// called when fm not needed anymore: aircraft death, mission quit etc.; use to reset flight model
void ed_fm_release()
{
	flightControls.release();
}

// callback when damage occurs for airframe element 
void ed_fm_on_damage(int Element, double element_integrity_factor)
{
	// keep integrity information in airframe
	damageModel.onAirframeDamage(Element, element_integrity_factor);
}

// called in case of repair routine 
void ed_fm_repair()
{
	damageModel.onRepair();
}

//====================================================================================
// Output Functions to DCS (not specified when called)
//====================================================================================

// external model draw arguments.  size: count of elements in array
void ed_fm_set_draw_args_v2(float* drawargs, size_t size)
{
	drawargs[EXT_TRcollective] = (float)-flightControls.PedalInput;
	drawargs[EXT_Collective] = (float)flightControls.CollectiveInput;
	drawargs[EXT_CyclicRoll] = (float)flightControls.rollOutput;
	drawargs[EXT_CyclicPitch] = (float)flightControls.pitchOutput;
	
}

// cockpit draw arguments
void ed_fm_set_fc3_cockpit_draw_args_v2(float * drawargs,size_t size)
{
	drawargs[INT_StickPitch] = (float)flightControls.pitchOutput;
	drawargs[INT_StickRoll] = (float)flightControls.rollOutput;
	drawargs[INT_Collective] = (float)flightControls.CollectiveInput;
	drawargs[INT_Pedals] = (float)-flightControls.PedalInput;
	drawargs[INT_OATNeedle] = (float) limit(EFMdata.ambientTemp_C /60 , 0, 1.0);
	
}

// send DCS internal fuel volume 
double ed_fm_get_internal_fuel()
{
	return Fuel.getInternalFuel();
}

// send DCS external fuel volume 
double ed_fm_get_external_fuel()
{
	return 0;
}

// shake level amplitude for head simulation  
double ed_fm_get_shake_amplitude ()
{
	return 0;
}

// see enum ed_fm_param_enum in wHumanCustomPhysicsAPI.h
double ed_fm_get_param(unsigned param_enum)
{
	switch (param_enum)
	{
	case ED_FM_PROPELLER_0_RPM:	// this is neccesary for rotor sound, rpm should match definition in AH-6.lua rotor_RPM
		return Aero.getMRomega() * Convert::radSecToRPM;
	case ED_FM_PROPELLER_0_PITCH:  // propeller blade pitch
		
	case ED_FM_PROPELLER_0_TILT:   // for helicopter
		return flightControls.CollectiveInput * 19;
	case ED_FM_PROPELLER_0_INTEGRITY_FACTOR:   // for 0 to 1 , 0 is fully broken 
		return 1;

	case ED_FM_PROPELLER_1_RPM:	// this is neccesary for rotor sound, rpm should match definition in AH-6.lua tail_rotor_RPM
		return Aero.getTRomega() * Convert::radSecToRPM;
	case ED_FM_PROPELLER_1_PITCH:  // propeller blade pitch

	case ED_FM_PROPELLER_1_TILT:   // for helicopter
		return flightControls.PedalInput * 16;
	case ED_FM_PROPELLER_1_INTEGRITY_FACTOR:   // for 0 to 1 , 0 is fully broken 
		return 1;

	case ED_FM_ENGINE_1_RPM:
		return Aero.getN2omega() * 6016.0;
	case ED_FM_ENGINE_1_RELATED_RPM:
		return Aero.getN2omega();	
	case ED_FM_ENGINE_1_CORE_RPM:
		return Engine.getN1RPM() * 6016.0;
	case ED_FM_ENGINE_1_CORE_RELATED_RPM:		// This is important to use for engine sounds, heatblur, and for other internal functions like a functioning RWR
		return Engine.getN1RPM();

	case ED_FM_ENGINE_1_TEMPERATURE:
		return Engine.getTOT();
	case ED_FM_ENGINE_1_OIL_PRESSURE:
		return Engine.getOilPress_Pa();
	case ED_FM_ENGINE_1_FUEL_FLOW:
		return 0;//Helicopter::Engine.getFuelFlow();
	case ED_FM_ENGINE_0_TORQUE:
	case ED_FM_ENGINE_1_TORQUE:					// Engine torque, [N*m]
		return Engine.getTorque_Nm();
	case ED_FM_ENGINE_0_STARTER_RELATED_TORQUE:
	case ED_FM_ENGINE_1_STARTER_RELATED_TORQUE:
	case ED_FM_ENGINE_0_RELATIVE_TORQUE:
	case ED_FM_ENGINE_1_RELATIVE_TORQUE:		// Relative engine torque
		return Engine.getTorqueRelative();

	case ED_FM_ENGINE_1_THRUST:
	case ED_FM_ENGINE_1_RELATED_THRUST:
	case ED_FM_ENGINE_1_CORE_THRUST:
	case ED_FM_ENGINE_1_CORE_RELATED_THRUST:
		return 0;

	case ED_FM_SUSPENSION_0_GEAR_POST_STATE: // from 0 to 1 : from fully retracted to full released
	case ED_FM_SUSPENSION_1_GEAR_POST_STATE: 
	case ED_FM_SUSPENSION_2_GEAR_POST_STATE: 
		return 1;

	case ED_FM_FLOW_VELOCITY:
		return 0;

	// Groups for fuel indicator
	case ED_FM_FUEL_FUEL_TANK_GROUP_0_LEFT:			// Values from different group of tanks
	case ED_FM_FUEL_FUEL_TANK_GROUP_0_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_1_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_1_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_2_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_2_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_3_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_3_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_4_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_4_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_5_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_5_RIGHT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_6_LEFT:
	case ED_FM_FUEL_FUEL_TANK_GROUP_6_RIGHT:
		return 0;

	case ED_FM_FUEL_INTERNAL_FUEL:
	case ED_FM_FUEL_TOTAL_FUEL:
		return Fuel.getInternalFuel();
	case ED_FM_FUEL_LOW_SIGNAL:
		return Fuel.isLowFuel();

	case ED_FM_ANTI_SKID_ENABLE:
	case ED_FM_COCKPIT_PRESSURIZATION_OVER_EXTERNAL: 
		return 0;

	case ED_FM_STICK_FORCE_CENTRAL_PITCH:  // i.e. trimmered position where force feeled by pilot is zero
		return flightControls.pitchTrim;//Trim values you programmed to trim aircraft out (0 to 1)
	case ED_FM_STICK_FORCE_FACTOR_PITCH:
		return 1.0;//Force factor range from 0 to 1. Make it 1 and rather change the force factor in your aircraft setup controls menu (0 - 100 percent).
	case ED_FM_STICK_FORCE_SHAKE_AMPLITUDE_PITCH:
	case ED_FM_STICK_FORCE_SHAKE_FREQUENCY_PITCH:
		return 0;

	case ED_FM_STICK_FORCE_CENTRAL_ROLL:   // i.e. trimmered position where force feeled by pilot is zero
		return flightControls.rollTrim;//Trim values you programmed to trim aircraft out (0 to 1)
	case ED_FM_STICK_FORCE_FACTOR_ROLL:
		return 1.0;//Force factor range from 0 to 1. Make it 1 and rather change the force factor in your aircraft setup controls menu (0 - 100 percent).
	case ED_FM_STICK_FORCE_SHAKE_AMPLITUDE_ROLL:
	case ED_FM_STICK_FORCE_SHAKE_FREQUENCY_ROLL:
		return 0;

	default:
		// silence compiler warning(s)
		break;
	}
	return 0;	
}

// in case of some internal damages or system failures this function return true, to switch on repair process
bool ed_fm_need_to_be_repaired()
{
	return damageModel.isRepairNeeded();
}

// enable debug information like force vector and velocity vector visualization
bool ed_fm_enable_debug_info()
{
	return false;
}




// Gives path to config file as defined in entry.lua (EFM.config_path)
// Note: this config file is not used in the AH-6
void ed_fm_configure(const char* cfg_path)
{ 
// most commonly used location is /Mods/aircraft/*AC name*/Config/config.lua
}

// path to your plugin installed
void ed_fm_set_plugin_data_install_path(const char * path)
{
// gives file path to ...\Saved Games\DCS.openbeta\Mods\aircraft\AH-6J
}

// damages and failures
// callback when preplaneed failure triggered from mission 
void ed_fm_on_planned_failure(const char * data)
{
}

// Provides settings from "Additional Aircraft Properties" in Mission Editor
// ex: for AH-6 this will give the setting for "SoloFlight" and "NetCrewControlPriority"
// These are added in the main aircraft.lua under AddPropAircraft = {} section
void ed_fm_set_property_numeric(const char * property_name,float value)
{
	//char str[64];
	//sprintf_s(str, 64, "%s", property_name);
	//cockpitAPI.setParamString(TEST_PARAM, str);
}

// Same as ed_fm_set_property_numeric but for string format
void ed_fm_set_property_string(const char * property_name,const char * value)
{
}


//inform DCS about internal simulation event, like structure damage , failure , or effect
bool ed_fm_pop_simulation_event(ed_fm_simulation_event & out)
{	
	return false;	
}

// DCS will inform you about events here, such as damage to the aircraft
bool ed_fm_push_simulation_event(const ed_fm_simulation_event& in)
{
	return false;
}

// feedback to your fm about suspension state
void ed_fm_suspension_feedback(int idx,const ed_fm_suspension_info * info)
{
	// not currently used for skids
	// 
	// idx is gear # (0 is nose, 1 normally Left...)
	// info->acting_force;
	// info->acting_force_point;
	// info->integrity_factor; // (health)
	// info->struct_compression;
	// info->wheel_speed_X
}

// give DCS LERX vortex info
// Not applicable for helicopters typically
bool ed_fm_LERX_vortex_update(unsigned idx, LERX_vortex& out)
{
	return false;
}
