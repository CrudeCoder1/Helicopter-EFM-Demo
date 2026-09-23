dofile(LockOn_Options.script_path.."clickable_defs.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."devices.lua")
--dofile(LockOn_Options.script_path.."sounds.lua")

local gettext = require("i_18n")
_ = gettext.translate

elements = {}
-- Electric system
elements["PNT_17"]	= default_3_position_tumb(_("Power Selector Switch, BATT/OFF/EXT"),	devices.EFM_HELPER, EFM_commands.batterySwitch,		17)
elements["PNT_18"]	= default_2_position_tumb(_("Generator Switch, ON/OFF"),			devices.EFM_HELPER, EFM_commands.generatorSwitch,	18)
elements["PNT_19"]	= default_2_position_tumb(_("Inverter Switch, ON/OFF"),				devices.EFM_HELPER, EFM_commands.inverterSwitch,	19)
elements["PNT_21"]	= default_2_position_tumb(_("Master Radio Switch, ON/OFF"),		    devices.EFM_HELPER, EFM_commands.MasterRadioSw,	21)


-- Fuel System
--elements["PNT-018"]	= multiposition_switch(_("Fuel Selector Switch, OFF/MAIN/BOTH/AUX"),	devices.FUEL_SYSTEM,	device_commands.FuelShutoffSw,	18, 4, 0.25, true)
elements["PNT_12"]	= default_2_position_tumb(_("Start Pump Switch, ON/OFF"),		devices.FUEL_SYSTEM,	device_commands.FuelPumpSw,		12)
--elements["PNT_150"]	= default_2_position_tumb(_("Fuel Cutoff Valve, PULL TO CLOSE"),	devices.EFM_HELPER,	device_commands.FuelCutoffSw,		150)
elements["PNT_31"]	= default_axis_limited(_("Fuel Qty Brightness Knob"), 		devices.FUEL_SYSTEM, device_commands.FQIbrtKnob, 31, 1)
elements["PNT_34"]	= default_2_position_tumb(_("FQI Day/Night Switch, DAY/NIGHT"),		devices.FUEL_SYSTEM,	device_commands.FQIdayNhtSw,		34)


-- Engines
elements["PNT_154"]	= default_axis_limited(_("Throttle"),devices.EFM_HELPER,EFM_commands.throttle,154,nil,0.1,true,false,{-1,1})
elements["PNT_155"]	= default_2_position_tumb(_("Throttle Cutoff"),	devices.EFM_HELPER, EFM_commands.throttleIdleCutoff, 155)

-- Weapons panel
elements["PNT_59"]	= default_2_position_tumb(_("AMS Power Switch, OFF/ON"),			devices.WEAPON_SYSTEM,	device_commands.AMSPwrSw,	59)
elements["PNT_61"]	= default_2_position_tumb(_("Master Arm Switch, SAFE/ARM"),			devices.WEAPON_SYSTEM,	device_commands.MasterArm,	61)
elements["PNT_66"]	= default_2_position_tumb(_("Rocket Single/Ripple Switch, SINGLE/RIPPLE"),	devices.WEAPON_SYSTEM, 	device_commands.RippleSw,	66)
elements["PNT_65"]	= default_2_position_tumb(_("Rocket Pair/Single Switch, PAIR/SINGLE"),	devices.WEAPON_SYSTEM, 	device_commands.PairSw,	65)
elements["PNT_68"]	= default_2_position_tumb(_("Pylon Jettison Switch, SAFE/JETTISON"),	devices.WEAPON_SYSTEM, 	device_commands.JettSw,		68)
elements["PNT_67"]	= default_red_cover(_("Pylon Jettison Switch Cover"),	devices.WEAPON_SYSTEM, 	device_commands.JettSwCover,		67)
elements["PNT_63"]	= default_3_position_tumb(_("Rocket Side Selector, LEFT/BOTH/RIGHT"),	devices.WEAPON_SYSTEM,	device_commands.RocketSelector,	63)
elements["PNT_60"]	= default_axis_limited(_("AMS Button Brightness Knob"), 		devices.WEAPON_SYSTEM, device_commands.AMSbuttonBrght, 60, 1)

elements["PNT_113"]	= default_2_position_tumb(_("Left Gun Power Switch, OFF/POWER"), devices.WEAPON_SYSTEM, 	device_commands.LftGunPwr,		113)
elements["PNT_114"]	= default_2_position_tumb(_("Left Gun Arm Switch, OFF/ARM"),	 devices.WEAPON_SYSTEM, 	device_commands.LftGunArm,		114)
elements["PNT_116"]	= default_2_position_tumb(_("Right Gun Power Switch, OFF/POWER"),devices.WEAPON_SYSTEM, 	device_commands.RhtGunPwr,		116)
elements["PNT_117"]	= default_2_position_tumb(_("Right Gun Arm Switch, OFF/ARM"),	 devices.WEAPON_SYSTEM, 	device_commands.RhtGunArm,		117)

elements["PNT_170"]	= default_button(_("Rocket Fire Button"),devices.WEAPON_SYSTEM,device_commands.RocketFireButton,170)
elements["PNT_171"]	= default_button(_("Gun Trigger"),devices.WEAPON_SYSTEM,device_commands.GunTrigger,171)

-- RWR
elements["PNT_144"]	= default_2_position_tumb(_("RWR Power Switch, OFF/ON"),		devices.RWR, device_commands.RWRpower,		144)
elements["PNT_47"]	= default_axis_limited(_("RWR Display Brightness Knob"), 		devices.RWR, device_commands.RWRBrightness, 47, 1)

-- External Lights
elements["PNT_23"] = default_3_position_tumb(_("Position Light Switch, POS CVRT/OFF/NORM"),			devices.LIGHTING, device_commands.PositionLights,	23)
--elements["PNT-027"] = default_3_position_tumb(_("Anti-Collision Light Switch, BOTTOM/OFF/TOP"),	devices.LIGHTING, device_commands.AntiCollision,27)
--elements["PNT-010"] = default_2_position_tumb(_("Covert Light Switch, NORM/OFF"),			devices.LIGHTING, device_commands.CovertLight,	10)
elements["PNT_159"]	= default_2_position_tumb(_("Landing Light Switch, ON/OFF"),			devices.LIGHTING, device_commands.LandingLightSw,	159)
elements["PNT_167"]	= default_button(_("Lighting Kill Switch"),	devices.LIGHTING,device_commands.LightKillSw,167)

-- Internal Lights
elements["PNT_28"]	= default_axis_limited(_("Radio Backlighting Knob"), devices.LIGHTING, device_commands.RadioLightKnob, 28, 1)
elements["PNT_29"]	= default_axis_limited(_("Post Lights Knob"), 		 devices.LIGHTING, device_commands.PanelLightKnob, 29, 1)
elements["PNT_33"]	= default_axis_limited(_("AMS Backlighting Knob"), 		 devices.LIGHTING, device_commands.AMSBacklightKnob, 33, 1)

elements["PNT_69"]	= default_button(_("Caution Lights Test, PUSH TO TEST"), devices.LIGHTING, device_commands.CautionTest, 69)

--Avionics
elements["PNT_45"]	= default_axis(_("Altimeter Setting Knob"), devices.AVIONICS, device_commands.AltimeterSet, 45, 0.0, 0.6667, false, true)
elements["PNT_46"]	= default_axis_limited(_("ADI Adjustment Knob"), devices.AVIONICS, device_commands.ADIadjust, 46, nil, nil, nil, nil, {-1,1})
elements["PNT_43"]	= default_axis(_("LO flag setting knob"), devices.AVIONICS, device_commands.LOset, 43)
elements["PNT_44"]	= default_axis(_("HI flag setting knob"), devices.AVIONICS, device_commands.HIset, 44)
elements["PNT_37"]	= default_button(_("DHI Digit Test Button"),devices.AVIONICS,device_commands.DHItest,37)
elements["PNT_30"]	= default_axis_limited(_("DHI Brightness Knob"), 	devices.AVIONICS, device_commands.DHIbrightness, 30, 1)
elements["PNT_32"]	= default_axis_limited(_("Radar Altitude Brightness Knob"), devices.AVIONICS, device_commands.RadAltBrightness, 32, 1)
elements["PNT_20"]	= default_2_position_tumb(_("Attitude Indicator Power Switch, OFF/ON"),		devices.AVIONICS, device_commands.AttIndPwrSw,		20)

-- Clock
elements["PNT_50"]	= default_button(_("Clock Select Button"),devices.DIGITAL_CLOCK,device_commands.M880Select,50)
elements["PNT_49"]	= default_button(_("Clock Control Button"),devices.DIGITAL_CLOCK,device_commands.M880Control,49)
elements["PNT_48"]	= default_axis_limited(_("Clock Brightness Knob"), devices.DIGITAL_CLOCK, device_commands.M880Brightness, 48, 1)

-- VIDS
elements["PNT_26"] = switch_button_3pos_2(_("VIDS Digit Switch, OFF/NORM/TST"),	devices.VIDS, device_commands.VIDSdigitSw,	26)
elements["PNT_27"] = default_axis_limited(_("VIDS Brightness Knob"), 		 		devices.VIDS, device_commands.VIDSbrtKnob, 27, 1)


elements["PNT_151"]	= default_2_position_tumb(_("Rotor Brake Handle, ON/OFF"),			devices.EFM_HELPER, EFM_commands.rotorBrake,	151)


--- VHF AN/ARC-186 control panel
elements["PNT_90"] = default_2_position_tumb(_("ARC-186 Mode Switch, TR/OFF"),	devices.ARC186, device_commands.ARC186_mode,		90)
elements["PNT_87"] = multiposition_switch(_("ARC-186 Frequency Mode Switch, PRE/MAN/EMER AM/EMER FM"),devices.ARC186, device_commands.ARC186_FreqMode, 87, 4, 1/3, true, 0, 3, false)
elements["PNT_91"] = multiposition_switch(_("ARC-186 10MHz Selector Knob"),devices.ARC186, device_commands.ARC186_10MHz, 91, 13, 1/13, false, 0, 1, true)
elements["PNT_92"] = multiposition_switch(_("ARC-186 1MHz Selector Knob"),devices.ARC186, device_commands.ARC186_1MHz, 92, 10, 1/10, false, 0, nil, true)
elements["PNT_93"] = multiposition_switch(_("ARC-186 0.1MHz Selector Knob"),devices.ARC186, device_commands.ARC186_tenthMHz, 93, 10, 1/10, false, 0, 1, true)
elements["PNT_94"] = multiposition_switch(_("ARC-186 0.025MHz Selector Knob"),devices.ARC186, device_commands.ARC186_quartMHz, 94, 4, 1/4, false, 0, 1, true)
elements["PNT_89"] = multiposition_switch(_("ARC-186 Channel Selector Knob"),devices.ARC186, device_commands.ARC186_chan, 89, 20, 1/20, false, 0, 1, true)
elements["PNT_85"] = default_axis(_("ARC-186 Volume"),						devices.ARC186, device_commands.ARC186_vol,		85)
--elements["PNT_88"] = default_button(_("ARC-186 Load Button"), devices.ARC186, device_commands.ARC186_load, 88)
elements["PNT_86"] = switch_button_3pos_2(_("ARC-186 Squelch/Tone Switch, SQ DIS/NORM/TONE"), devices.ARC186, device_commands.ARC186_SquelchToneSw, 86)



--- V/UHF AN/ARC-182 control panel
elements["PNT_105"] = default_2_position_tumb(_("ARC-182 Squelch Switch, SQUELCH/OFF"),	devices.ARC182, device_commands.ARC182_squelch,		105)
elements["PNT_104"] = springloaded_3_pos_tumb(_("ARC-182 Frequency Tens"),	devices.ARC182,	device_commands.ARC182_freqTens, device_commands.ARC182_freqTens, 104)
elements["PNT_103"] = springloaded_3_pos_tumb(_("ARC-182 Frequency Ones"),	devices.ARC182, device_commands.ARC182_freqOnes,	device_commands.ARC182_freqOnes, 103)
elements["PNT_102"] = springloaded_3_pos_tumb(_("ARC-182 Frequency Tenths"),	devices.ARC182, device_commands.ARC182_freqTenths,	device_commands.ARC182_freqTenths,  102)
elements["PNT_101"] = springloaded_3_pos_tumb(_("ARC-182 Frequency Hundredths"),	devices.ARC182, device_commands.ARC182_freqHundredths,	device_commands.ARC182_freqHundredths, 101)
elements["PNT_100"] = default_2_position_tumb(_("ARC-182 AM/FM Mode Switch, AM/FM"),	devices.ARC182, device_commands.ARC182_AMFM,		100)
elements["PNT_95"] = default_axis(_("ARC-182 Volume"),						devices.ARC182, device_commands.ARC182_vol,		95)
elements["PNT_98"] = multiposition_switch(_("ARC-182 Mode Control Selector, OFF/T+R/T+R&G/DF/TEST"),	devices.ARC182, device_commands.ARC182_mode,	98, 5, 0.25, false, 0, 3, false)
elements["PNT_99"] = default_axis(_("ARC-182 Brightness"),					devices.ARC182, device_commands.ARC182_brightness,		99)
elements["PNT_96"] = multiposition_switch(_("ARC-182 Frequency Mode Knob"),	 devices.ARC182, device_commands.ARC182_FreqSelType, 96, 4, 0.2, false, 0.2, 3, false)
elements["PNT_97"] = multiposition_switch(_("ARC-182 Channel Selector Knob"),devices.ARC182, device_commands.ARC182_ChannelSel, 97, 30, 1/30, false, 0, 3, true)

-- Pilot ICS Panel
elements["PNT_128"] = default_axis(_("Pilot ICS Master Volume"),		    devices.P_INTERCOM_PANEL, device_commands.P_ICS_MastVol,	128)
elements["PNT_129"] = default_axis(_("Pilot Monitor 1 Volume (ARC-186)"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_MON1,		129)
elements["PNT_130"] = default_axis(_("Pilot Monitor 2 Volume (ARC-182)"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_MON2,		130)
elements["PNT_131"] = default_axis(_("Pilot Monitor 3 Volume (ARC-210)"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_MON3,		131)
--elements["PNT_132"] = default_axis(_("Pilot Monitor 5 Volume (SABER)(No function)"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_MON5,		132)
--elements["PNT_133"] = default_axis(_("Pilot Monitor NAV A Volume (VAW)(No function)"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_MONA,		133)
--elements["PNT_124"] = multiposition_switch(_("Pilot ICS Function Selector, OFF/NORM/VOX/HOT MIC"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_Function,	124, 4, 0.25, false, 0, 3, false)
elements["PNT_125"] = multiposition_switch(_("Pilot ICS Mode Selector, PVT/ICS/1/2/3/4/5/RMT"),	devices.P_INTERCOM_PANEL, device_commands.P_ICS_Mode,	125, 8, 1/7, false, 0, 3, false)


-- ARGUS7000
elements["PNT_42"]	= default_button(_("DEParture Button"), devices.ARGUS7000, device_commands.ArgusDEPbutton, 42)
elements["PNT_41"]	= default_button(_("ENRoute Button"), devices.ARGUS7000, device_commands.ArgusENRbutton, 41)
elements["PNT_40"]	= default_button(_("ARRival Button"), devices.ARGUS7000, device_commands.ArgusARRbutton, 40)
elements["PNT_39"]	= default_button(_("AUXilary Button"), devices.ARGUS7000, device_commands.ArgusAUXbutton, 39)

-- not buttons but require pushing 2 buttons at the same time. Using clickable on text position instead
--elements["PNT_70"]	= default_button(_("SELect"), devices.ARGUS7000, device_commands.ArgusSELbutton, 70)
--elements["PNT_71"]	= default_button(_("INFOrmation"), devices.ARGUS7000, device_commands.ArgusINFObutton, 71)
--elements["PNT_72"]	= default_button(_("EMERgency"), devices.ARGUS7000, device_commands.ArgusEMERbutton, 72)

-- TACAN
elements["PNT_121"]	= multiposition_switch(_("TACAN Channel Selector Tens"), devices.TACAN, device_commands.TACAN10s, 121,20,0.05,false,0,3,true)
elements["PNT_122"]	= multiposition_switch(_("TACAN Channel Selector Ones"), devices.TACAN, device_commands.TACAN1s, 122,10,0.1,false,0,3,true)
elements["PNT_123"]	= default_axis_limited(_("TACAN Power/Volume Knob"), 	 devices.TACAN, device_commands.TACANPwrVol, 123, 1)

-- non functional
elements["PNT_11"]	= default_2_position_tumb(_("Pitot Heat Switch, ON/OFF (No function)"),		devices.EFM_HELPER, device_commands.PitotHeatSw,	11)
elements["PNT_14"]	= default_2_position_tumb(_("Auxilary Power Switch, ON/OFF (No function)"),	devices.EFM_HELPER, device_commands.AuxPowerSw,	14)
elements["PNT_15"]	= default_2_position_tumb(_("Engine Anti-Ice Switch, ON/OFF (No function)"),	devices.EFM_HELPER, device_commands.AntiIceSw,	15)
elements["PNT_10"]	= default_2_position_tumb(_("Scav Air Switch, ON/OFF (No function)"),			devices.EFM_HELPER, device_commands.ScavAirSw,	10)
