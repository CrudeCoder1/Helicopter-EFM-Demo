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

	KeyRudderLeft		= counter(),
	KeyRudderRight		= counter(),
	KeyRudderStop		= counter(),
	KeyCollectiveUp		= counter(),
	KeyCollectiveDown	= counter(),
	KeyCyclicForward	= counter(),
	KeyCyclicBack		= counter(),
	KeyCyclicLeft		= counter(),
	KeyCyclicRight		= counter(),
	throttleAxis		= counter(),

	trimUp				= counter(),
	trimDown			= counter(),
	trimLeft			= counter(),
	trimRight			= counter(),

	iCommandPlane_ShowControls = 851;--need this bc show controls is in common_keyboard_binding
}

count = 3020
device_commands = { -- commands for lua
	starterButton 		= counter(),
	throttleIdleCutoff	= counter(),
	throttle			= counter(),
	batterySwitch 		= counter(),
	generatorSwitch 	= counter(),
	inverterSwitch 		= counter(),
	MasterRadioSw		= counter(),

	rotorBrake			= counter(),
	AuxHandle 			= counter(),
	
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
	P_LandingLightSw= counter();
	CP_LandingLightSw= counter();
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
	P_CLKreset		= counter();
	CP_CLKreset		= counter();
	AttIndPwrSw		= counter();
	PitotHeatSw		= counter();
	AntiIceSw		= counter();
	ScavAirSw		= counter();
	FQIbrtKnob		= counter();
	FQIdayNhtSw		= counter();
	FQIselectSw		= counter();
	
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
	ARC182_squelch		= counter(),
	
	ArgusDEPbutton 	= counter(),
	ArgusENRbutton 	= counter(),
	ArgusARRbutton 	= counter(),
	ArgusAUXbutton 	= counter(),
	ArgusSELbutton	= counter(),
	ArgusINFObutton	= counter(),
	ArgusEMERbutton	= counter(),
	ArgusBrightness	= counter(),
	
	TACAN10s	= counter(),
	TACAN1s		= counter(),
	TACANPwrVol	= counter(),

	P_ICS_MastVol = counter(),
	P_ICS_MON1 = counter(),
	P_ICS_MON2 = counter(),
	P_ICS_MON3 = counter(),
	P_ICS_MON5 = counter(),
	P_ICS_MONA = counter(),
	P_ICS_Function = counter(),
	P_ICS_Mode = counter(),

	ARC186_10MHz = counter(),
	ARC186_1MHz = counter(),
	ARC186_tenthMHz = counter(),
	ARC186_quartMHz = counter(),
	ARC186_mode = counter(),
	ARC186_FreqMode = counter(),
	ARC186_vol = counter(),
	ARC186_chan = counter(),
	ARC186_load = counter(),
	ARC186_SquelchToneSw = counter(),


}


