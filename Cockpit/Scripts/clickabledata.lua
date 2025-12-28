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
--elements["PNT-020"]	= default_2_position_tumb(_("Auxilary Power Switch, ON/OFF"),		devices.EFM_HELPER, device_commands.AuxPowerSw,	20)

-- Fuel System
--elements["PNT-018"]	= multiposition_switch(_("Fuel Selector Switch, OFF/MAIN/BOTH/AUX"),	devices.FUEL_SYSTEM,	device_commands.FuelShutoffSw,	18, 4, 0.25, true)
--elements["PNT-022"]	= default_2_position_tumb(_("Start Pump Switch, ON/OFF"),		devices.FUEL_SYSTEM,	device_commands.FuelPumpSw,		22)

-- Engines
--elements["PNT-004"]	= default_axis(_("Throttle"),	devices.EFM_HELPER, EFM_commands.throttle, 4, nil, 0.06) -- using blank arg # so actual arg can be controlled later
elements["PNT_154"]	= default_axis_limited(_("Throttle"),devices.EFM_HELPER,EFM_commands.throttle,154,nil,0.1,true,false,{-1,1})
elements["PNT_155"]	= default_2_position_tumb(_("Throttle Cutoff"),	devices.EFM_HELPER, EFM_commands.throttleIdleCutoff, 155)

-- Weapons panel
elements["PNT_61"]	= default_2_position_tumb(_("Master Arm Switch, ARM/SAFE"),			devices.WEAPON_SYSTEM,	device_commands.MasterArm,	61)
elements["PNT_66"]	= default_2_position_tumb(_("Rocket Single/Ripple Mode Switch, SINGLE/RIPPLE"),	devices.WEAPON_SYSTEM, 	device_commands.SalvoSw,	66)
elements["PNT_68"]	= default_2_position_tumb(_("Pylon Jettison Switch, SAFE/JETTISON"),	devices.WEAPON_SYSTEM, 	device_commands.JettSw,		68)
elements["PNT_67"]	= default_red_cover(_("Pylon Jettison Switch Cover"),	devices.WEAPON_SYSTEM, 	device_commands.JettSwCover,		67)
elements["PNT_63"]	= default_3_position_tumb(_("Rocket Station Selector, LEFT/BOTH/RIGHT"),	devices.WEAPON_SYSTEM,	device_commands.RocketSelector,	63)

elements["PNT_113"]	= default_2_position_tumb(_("Left Gun Power Switch, OFF/POWER"), devices.WEAPON_SYSTEM, 	device_commands.LftGunPwr,		113)
elements["PNT_114"]	= default_2_position_tumb(_("Left Gun Arm Switch, OFF/ARM"),	 devices.WEAPON_SYSTEM, 	device_commands.LftGunArm,		114)
elements["PNT_116"]	= default_2_position_tumb(_("Right Gun Power Switch, OFF/POWER"),devices.WEAPON_SYSTEM, 	device_commands.RhtGunPwr,		116)
elements["PNT_117"]	= default_2_position_tumb(_("Right Gun Arm Switch, OFF/ARM"),	 devices.WEAPON_SYSTEM, 	device_commands.RhtGunArm,		117)

-- RWR
elements["PNT_144"]	= default_2_position_tumb(_("RWR Power Switch, OFF/ON"),		devices.RWR, device_commands.RWRpower,		144)
elements["PNT_47"]	= default_axis_limited(_("RWR Display Brightness Knob"), 		devices.RWR, device_commands.RWRBrightness, 47, 1)

-- External Lights
elements["PNT_23"] = default_3_position_tumb(_("Position Light Switch, POS CVRT/OFF/NORM"),			devices.LIGHTING, device_commands.PositionLights,	23)
--elements["PNT-027"] = default_3_position_tumb(_("Anti-Collision Light Switch, BOTTOM/OFF/TOP"),	devices.LIGHTING, device_commands.AntiCollision,27)
--elements["PNT-010"] = default_2_position_tumb(_("Covert Light Switch, NORM/OFF"),			devices.LIGHTING, device_commands.CovertLight,	10)
elements["PNT_159"]	= default_2_position_tumb(_("Landing Light Switch, ON/OFF"),			devices.LIGHTING, device_commands.LandingLightSw,	159)

-- Internal Lights
elements["PNT_28"]	= default_axis_limited(_("Radio Backlighting Knob"), devices.LIGHTING, device_commands.RadioLightKnob, 28, 1)
elements["PNT_29"]	= default_axis_limited(_("Post Lights Knob"), 		 devices.LIGHTING, device_commands.PanelLightKnob, 29, 1)
elements["PNT_33"]	= default_axis_limited(_("AMS Backlighting Knob"), 		 devices.LIGHTING, device_commands.AMSBacklightKnob, 33, 1)

--Avionics
elements["PNT_45"]	= default_axis(_("Altimeter Setting Knob"), devices.AVIONICS, device_commands.AltimeterSet, 45, 0.04, 1, false, true)
elements["PNT_46"]	= default_axis_limited(_("ADI Adjustment Knob"), devices.AVIONICS, device_commands.ADIadjust, 46, nil, nil, nil, nil, {-1,1})
elements["PNT_43"]	= default_axis(_("LO flag setting knob"), devices.AVIONICS, device_commands.LOset, 43)
elements["PNT_44"]	= default_axis(_("HI flag setting knob"), devices.AVIONICS, device_commands.HIset, 44)
elements["PNT_37"]	= default_button(_("DHI Digit Test Button"),devices.AVIONICS,device_commands.DHItest,37)
elements["PNT_30"]	= default_axis_limited(_("DHI Brightness Knob"), 	devices.AVIONICS, device_commands.DHIbrightness, 30, 1)
elements["PNT_32"]	= default_axis_limited(_("Radar Altitude Brightness Knob"), devices.AVIONICS, device_commands.RadAltBrightness, 32, 1)

elements["PNT_50"]	= default_button(_("Clock Select Button"),devices.DIGITAL_CLOCK,device_commands.M880Select,50)
elements["PNT_49"]	= default_button(_("Clock Control Button"),devices.DIGITAL_CLOCK,device_commands.M880Control,49)
elements["PNT_48"]	= default_axis_limited(_("Clock Brightness Knob"), devices.DIGITAL_CLOCK, device_commands.M880Brightness, 48, 1)

-- VIDS
elements["PNT_26"] = default_3_position_tumb(_("VIDS Digit Switch, OFF/NORM/TST"),	devices.VIDS, device_commands.VIDSdigitSw,	26)
elements["PNT_27"]	= default_axis_limited(_("VIDS Brightness Knob"), 		 		devices.VIDS, device_commands.VIDSbrtKnob, 27, 1)


elements["PNT_151"]	= default_2_position_tumb(_("Rotor Brake Handle, ON/OFF"),			devices.EFM_HELPER, EFM_commands.rotorBrake,	151)