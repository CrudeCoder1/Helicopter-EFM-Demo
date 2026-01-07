local function counter()
	count = count + 1
	return count
end

count = 10000

Keys =
{
	BattSwitch	 	 = counter(),
	ExtPwrSwitch	 = counter(),
	ThrottleCutoff 	 = counter(),
	ThrottleIncrease = counter(),
	ThrottleDecrease = counter(),
	ThrottleStop 	 = counter(),
	LandingLight	 = counter(),
	
	TriggerFireOn	= counter(),
	TriggerFireOff 	= counter(),
	MasterArmToggle	= counter(),
	
	COM1			= counter(),
	
	iCommandPlane_ShowControls = 851;--need this bc show controls is in common_keyboard_binding
}

count = 3200
device_commands = { -- commands for lua
	CautionTest		= counter();
	AuxPowerSw  	= counter();
	
	FuelShutoffSw	= counter();
	FuelPumpSw 		= counter();
	
	AMSPwrSw		= counter();
	AMSbuttonBrght  = counter();
	MasterArm		= counter();
	RippleSw		= counter();
	PairSw			= counter();
	JettSw			= counter();
	JettSwCover		= counter();
	RocketSelector	= counter();

	
	LftGunPwr		= counter();
	LftGunArm		= counter();
	RhtGunPwr		= counter();
	RhtGunArm		= counter();
	
	RocketFireButton= counter();
	GunTrigger		= counter();
	
	PositionLights	= counter();
	CovertLight		= counter();
	AntiCollision	= counter();
	LandingLightSw	= counter();
	RadioLightKnob  = counter();
	PanelLightKnob  = counter();
	AMSBacklightKnob= counter();
	LightKillSw		= counter();
	
	VIDSdigitSw  	= counter();
	VIDSbrtKnob  	= counter();
	
	RWRpower		= counter();
	RWRBrightness	= counter();
	
	AltimeterSet	= counter();
	ADIadjust		= counter();
	LOset			= counter();
	HIset			= counter();
	DHItest			= counter();
	DHIbrightness	= counter();
	RadAltBrightness= counter();
	M880Select		= counter();
	M880Control		= counter();
	M880Brightness	= counter();
	AttIndPwrSw		= counter();
	PitotHeatSw		= counter();
	AntiIceSw		= counter();
	ScavAirSw		= counter();
	MasterRadioSw	= counter();
	FQIbrtKnob		= counter();
	FQIdayNhtSw		= counter();
	
	ARC182_freqTens 	= counter(),
	ARC182_freqOnes 	= counter(),
	ARC182_freqTenths 	= counter(),
	ARC182_freqHundredths = counter(),
	ARC182_AMFM 		= counter(),
	ARC182_vol 			= counter(),
	ARC182_mode 		= counter(),
	ARC182_brightness 	= counter(),
	ARC182_FreqSelType 	= counter(),
	ARC182_ChannelSel 	= counter(),
	
}

EFM_commands = 	-- commands for use in EFM (make sure to copy to GlobalData.h)
{
	starterButton 		= 3010,
	throttleIdleCutoff	= 3011,
	throttle			= 3012,
	batterySwitch 		= 3013,
	generatorSwitch 	= 3014,
	inverterSwitch 		= 3015,
	throttleAxis		= 3016,
	trimUp				= 3017,
	trimDown			= 3018,
	trimLeft			= 3019,
	trimRight			= 3020,
	
	rotorBrake			= 3021,
	KeyRudderLeft		= 3022,
	KeyRudderRight		= 3023,
	KeyRudderStop		= 3024,
	KeyCollectiveUp		= 3025,
	KeyCollectiveDown	= 3026,
	KeyCyclicForward	= 3027,
	KeyCyclicBack		= 3028,
	KeyCyclicLeft		= 3029,
	KeyCyclicRight		= 3030,
}


