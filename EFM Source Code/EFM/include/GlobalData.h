#pragma once

// Items used across the EFM
// Animation arguments, input commands, conversions, deivce IDs

namespace Convert
{
	const double		degToRad = M_PI / 180.0;	// 
	const double		radToDeg = 180.0 / M_PI;	// radians to degrees Conversion factor
	const double		metersToKnots = 1.943844;		// m/s to knots
	const double		knotsToMeters = 0.514444;		// knots to m/s
	const double		meterToFoot = 3.28084;		// Meter to foot conversion factor
	const double		feetToMeter = 0.3048;		// multiplier, ft to m
	//const double		inchesToCentim = 2.54;			// multiplier, in to cm
	const double		lb_to_kg = 0.45359237; // multiplier, lb to kg
	const double		kg_to_lb = 2.20462262; // multiplier, kg to lb
	const double		lbf_to_N = 4.44822162825; // multiplier, pound force to Newtons
	const double		lbft_to_Nm = 1.35581795; // multiplier, "pound-foot" to Newtonmeters
	//const double		gallon_to_liter = 3.7854118;
	const double		standard_gravity = 9.80665002864; // "average", [m/s^2]
	const double		gravity_fts2 = 32.174; // "average", [ft/s^2]
	const double		radSecToRPM = 60 / (2 * M_PI);// radians per second to RPM
	const double		PsiToPascal = 6894.7572931783;
	const double		kgm3ToSlugFt3 = 0.0019403203;// kg/m^3 to slug/ft^3
}


enum InternalAnimationArgs
{
	// Cockpit draw args
	INT_StickPitch = 1,
	INT_StickRoll = 2,
	INT_Collective = 3,
	INT_Pedals = 4,
	INT_Throttle = 154,
	INT_IdleCutoff = 155,

	INT_N1rpmNeedle = 320,
	INT_N1rpmNeedleSmall = 321,
	INT_OilPressNeedle = 319,
	INT_OilTempNeedle = 318,
	INT_DCAmpsNeedle = 322,
	//123 slip ball
	INT_OATNeedle = 328,

	INT_CautionBrightness = 410,
};


enum ExternalAnimationArgs
{
	EXT_Collective = 9,
	EXT_CyclicRoll = 11,
	EXT_CyclicPitch = 15,
	//EXT_Pedals = 17,
	EXT_RotorDroop = 36,
	EXT_RotorSpin = 37,//this is control arg, actual arg is 40
	EXT_TRspin = 42,//this is control arg, actual arg is 41
	EXT_NavLights = 190,
	EXT_LandingLight = 208,
	EXT_TRcollective = 17,
	//510 aiming mark
};

// Used in ed_fm_set_command() 
// These must match Cockpit\Scripts\command_defs.lua
enum AH6InputCommands
{
	// commands from command_defs.lua
	starterButton = 3010,
	throttleIdleCutoff = 3011,
	throttle = 3012,
	batterySwitch = 3013,
	generatorSwitch = 3014,
	inverterSwitch = 3015,
	throttleAxis = 3016,
	trimUp = 3017,
	trimDown = 3018,
	trimLeft = 3019,
	trimRight = 3020,
	rotorBrake = 3021,

	cautionTest = 3201,



	// joystick axis commands
	JoystickPitch = 2001,
	JoystickRoll = 2002,
	JoystickYaw = 2003,
	JoystickThrottle = 2004,

};

// needs to match devices.lua
// useful for separating commands by device
enum Devices
{
	ELECTRIC_SYSTEM_DEVICE = 1,
	WEAPON_SYSTEM_DEVICE = 2,
	RWR_DEVICE = 3,
	LIGHTING_DEVICE = 4,
	UHF_RADIO_DEVICE = 5,
	INTERCOM_DEVICE = 6,
	HELMET_DEVICE_DEVICE = 7,
	AVIONICS_DEVICE = 8,
	DIGITAL_CLOCK_DEVICE = 9,
	KNEEBOARD_DEVICE = 10,
	EFM_HELPER_DEVICE = 11,
	VIDS_DEVICE = 12,
	FUEL_SYSTEM_DEVICE = 13,
};

// from Scripts\Aircrafts\_Common\Damage.lua
enum damageCells
{
	FRONT = 0,
	NOSE_LEFT_SIDE = 1,
	NOSE_RIGHT_SIDE = 2,

	COCKPIT = 3,
	CABIN_LEFT_SIDE = 4,
	CABIN_RIGHT_SIDE = 5,
	CABIN_BOTTOM = 6,

	FRONT_GEAR_BOX = 8,
	GEAR_F = 8,

	FUSELAGE_LEFT_SIDE = 9,
	FUSELAGE_RIGHT_SIDE = 10,

	ENGINE = 11,
	ENGINE_L = 11,
	ENGINE_R = 12,
	MTG_L_BOTTOM = 13,
	MTG_R_BOTTOM = 14,

	LEFT_GEAR_BOX = 15,
	GEAR_L = 15,
	RIGHT_GEAR_BOX = 16,
	GEAR_R = 16,

	AIR_BRAKE_L = 19,

	WING_L_OUT = 23,
	WING_R_OUT = 24,

	AILERON_L = 25,
	AILERON_R = 26,

	WING_L_PART_CENTER = 27,
	WING_R_PART_CENTER = 28,

	WING_L_CENTER = 29,
	WING_R_CENTER = 30,

	WING_L_IN = 35,
	WING_R_IN = 36,

	FLAP_L = 37,
	FLAP_R = 38,

	FIN_L_CENTER = 41,
	FIN_R_CENTER = 42,

	STABILIZATOR_L = 47,
	STABILIZATOR_R = 48,

	RUDDER = 53,

	TAIL = 55,
	TAIL_BOTTOM = 58,

	NOSE_BOTTOM = 59,

	BLADE_1_IN = 64,
	BLADE_1_CENTER = 65,
	BLADE_2_IN = 67,
	BLADE_2_CENTER = 68,
	BLADE_3_IN = 70,
	BLADE_3_CENTER = 71,
	BLADE_4_IN = 73,
	BLADE_4_CENTER = 74,
	BLADE_5_IN = 76,
	BLADE_5_CENTER = 77,


	FUSELAGE_BOTTOM = 82,

	WHEEL_F = 83,
	WHEEL_L = 84,
	WHEEL_R = 85,

	PYLON1 = 86,
	PYLONL = 86,
	PYLON2 = 87,
	PYLONR = 87,
	PYLON3 = 88,
	PYLON4 = 89,

	TAIL_TOP = 100,

};

enum Caution_Lights
{
	CL_Test,
	CL_ReIGN,
	CL_IFF,
	CL_AirFilter,
	CL_Blank,
	CL_FuelFilter,
	CL_FuelLow,
	CL_TRChips,
	CL_MRChips,
	CL_EngChips,
	CL_GenOut,
	CL_BattTemp140,
	CL_BattTemp160,
	CL_XmsnTemp,
	CL_XmsnPress,
	CL_EngOut,
	
	CL_MaxNumber,
};