#pragma once

// Auto-generated from command_defs.lua by gen_command_defs.py
// DO NOT EDIT MANUALLY

#include <type_traits>

template <class E>
constexpr std::underlying_type_t<E> to_int(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}

enum class Keys
{
	Keys_starts = 10000, // base 'count' before first counter()
	BattSwitch = 10001,
	ExtPwrSwitch = 10002,
	ThrottleCutoff = 10003,
	ThrottleIncrease = 10004,
	ThrottleDecrease = 10005,
	ThrottleStop = 10006,
	LandingLight = 10007,

	TriggerFireOn = 10008,
	TriggerFireOff = 10009,
	MasterArmToggle = 10010,

	KeyRudderLeft = 10011,
	KeyRudderRight = 10012,
	KeyRudderStop = 10013,
	KeyCollectiveUp = 10014,
	KeyCollectiveDown = 10015,
	KeyCyclicForward = 10016,
	KeyCyclicBack = 10017,
	KeyCyclicLeft = 10018,
	KeyCyclicRight = 10019,
	throttleAxis = 10020,

	trimUp = 10021,
	trimDown = 10022,
	trimLeft = 10023,
	trimRight = 10024,

	iCommandPlane_ShowControls = 851, // need this bc show controls is in common_keyboard_binding
};

enum class device_commands // commands for lua
{
	device_commands_starts = 3020, // base 'count' before first counter()
	starterButton = 3021,
	throttleIdleCutoff = 3022,
	throttle = 3023,
	batterySwitch = 3024,
	generatorSwitch = 3025,
	inverterSwitch = 3026,
	MasterRadioSw = 3027,

	rotorBrake = 3028,
	AuxHandle = 3029,

	CautionTest = 3030,
	AuxPowerSw = 3031,

	FuelShutoffSw = 3032,
	FuelPumpSw = 3033,


	AMSPwrSw = 3034,
	AMSbuttonBrght = 3035,
	MasterArm = 3036,
	RippleSw = 3037,
	PairSw = 3038,
	JettSw = 3039,
	JettSwCover = 3040,
	RocketSelector = 3041,


	LftGunPwr = 3042,
	LftGunArm = 3043,
	RhtGunPwr = 3044,
	RhtGunArm = 3045,

	RocketFireButton = 3046,
	GunTrigger = 3047,

	PositionLights = 3048,
	CovertLight = 3049,
	AntiCollision = 3050,
	P_LandingLightSw = 3051,
	CP_LandingLightSw = 3052,
	RadioLightKnob = 3053,
	PanelLightKnob = 3054,
	AMSBacklightKnob = 3055,
	LightKillSw = 3056,

	VIDSdigitSw = 3057,
	VIDSbrtKnob = 3058,

	RWRpower = 3059,
	RWRBrightness = 3060,

	AltimeterSet = 3061,
	ADIadjust = 3062,
	LOset = 3063,
	HIset = 3064,
	DHItest = 3065,
	DHIbrightness = 3066,
	RadAltBrightness = 3067,
	M880Select = 3068,
	M880Control = 3069,
	M880Brightness = 3070,
	P_CLKreset = 3071,
	CP_CLKreset = 3072,
	AttIndPwrSw = 3073,
	PitotHeatSw = 3074,
	AntiIceSw = 3075,
	ScavAirSw = 3076,
	FQIbrtKnob = 3077,
	FQIdayNhtSw = 3078,
	FQIselectSw = 3079,

	ARC182_freqTens = 3080,
	ARC182_freqOnes = 3081,
	ARC182_freqTenths = 3082,
	ARC182_freqHundredths = 3083,
	ARC182_AMFM = 3084,
	ARC182_vol = 3085,
	ARC182_mode = 3086,
	ARC182_brightness = 3087,
	ARC182_FreqSelType = 3088,
	ARC182_ChannelSel = 3089,
	ARC182_squelch = 3090,

	ArgusDEPbutton = 3091,
	ArgusENRbutton = 3092,
	ArgusARRbutton = 3093,
	ArgusAUXbutton = 3094,
	ArgusSELbutton = 3095,
	ArgusINFObutton = 3096,
	ArgusEMERbutton = 3097,
	ArgusBrightness = 3098,

	TACAN10s = 3099,
	TACAN1s = 3100,
	TACANPwrVol = 3101,

	P_ICS_MastVol = 3102,
	P_ICS_MON1 = 3103,
	P_ICS_MON2 = 3104,
	P_ICS_MON3 = 3105,
	P_ICS_MON5 = 3106,
	P_ICS_MONA = 3107,
	P_ICS_Function = 3108,
	P_ICS_Mode = 3109,

	ARC186_10MHz = 3110,
	ARC186_1MHz = 3111,
	ARC186_tenthMHz = 3112,
	ARC186_quartMHz = 3113,
	ARC186_mode = 3114,
	ARC186_FreqMode = 3115,
	ARC186_vol = 3116,
	ARC186_chan = 3117,
	ARC186_load = 3118,
	ARC186_SquelchToneSw = 3119,


};
