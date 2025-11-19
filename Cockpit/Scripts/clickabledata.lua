dofile(LockOn_Options.script_path.."clickable_defs.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."devices.lua")
--dofile(LockOn_Options.script_path.."sounds.lua")

local gettext = require("i_18n")
_ = gettext.translate

elements = {}
-- Electric system
elements["PNT-017"]	= default_3_position_tumb(_("Power Selector Switch, BATT/OFF/EXT"),	devices.EFM_HELPER, EFM_commands.batterySwitch,		17, nil, nil, true)
elements["PNT-016"]	= default_2_position_tumb(_("Generator Switch, ON/OFF"),			devices.EFM_HELPER, EFM_commands.generatorSwitch,	16)
elements["PNT-015"]	= default_2_position_tumb(_("Inverter Switch, ON/OFF"),				devices.EFM_HELPER, EFM_commands.inverterSwitch,	15)
--elements["PNT-020"]	= default_2_position_tumb(_("Auxilary Power Switch, ON/OFF"),		devices.EFM_HELPER, device_commands.AuxPowerSw,	20)

-- Fuel System
--elements["PNT-018"]	= multiposition_switch(_("Fuel Selector Switch, OFF/MAIN/BOTH/AUX"),	devices.FUEL_SYSTEM,	device_commands.FuelShutoffSw,	18, 4, 0.25, true)
--elements["PNT-022"]	= default_2_position_tumb(_("Start Pump Switch, ON/OFF"),		devices.FUEL_SYSTEM,	device_commands.FuelPumpSw,		22)

-- Engines
elements["PNT-004"]	= default_axis(_("Throttle"),	devices.EFM_HELPER, EFM_commands.throttle, 0, nil, 0.04) -- using blank arg # so actual arg can be controlled later
elements["PNT-005"]	= default_2_position_tumb(_("Throttle Cutoff"),	devices.EFM_HELPER, EFM_commands.throttleIdleCutoff, 5)

-- Weapons panel
elements["PNT-043"]	= default_2_position_tumb(_("Master Arm Switch, ARM/SAFE"),			devices.WEAPON_SYSTEM,	device_commands.MasterArm,	43)
elements["PNT-044"]	= default_2_position_tumb(_("Rocket Single/Ripple Mode Switch, SINGLE/RIPPLE"),	devices.WEAPON_SYSTEM, 	device_commands.SalvoSw,	44)
elements["PNT-045"]	= default_2_position_tumb(_("Pylon Jettison Switch, SAFE/JETTISON"),	devices.WEAPON_SYSTEM, 	device_commands.JettSw,		45)
elements["PNT-046"]	= default_red_cover(_("Pylon Jettison Switch Cover"),	devices.WEAPON_SYSTEM, 	device_commands.JettSwCover,		46)
elements["PNT-047"]	= multiposition_switch(_("Rocket Station Selector, OFF/LEFT/BOTH/RIGHT"),	devices.WEAPON_SYSTEM,	device_commands.RocketSelector,	47, 4, 0.33)
elements["PNT-048"]	= multiposition_switch(_("Gun Station Selector, OFF/LEFT/BOTH/RIGHT"),	devices.WEAPON_SYSTEM,	device_commands.GunSelector,	48, 4, 0.33)

-- RWR
elements["PNT-025"]	= default_2_position_tumb(_("RWR Power Switch, OFF/ON"),		devices.RWR, device_commands.RWRpower,		25)
elements["PNT-030"]	= default_axis_limited(_("RWR Display Brightness Knob"), 		devices.RWR, device_commands.RWRBrightness, 30, 1)

-- External Lights
elements["PNT-011"] = default_2_position_tumb(_("Position Light Switch"),					devices.EXTLIGHTS, device_commands.PositionLights,	11)
--elements["PNT-027"] = default_3_position_tumb(_("Anti-Collision Light Switch, BOTTOM/OFF/TOP"),	devices.EXTLIGHTS, device_commands.AntiCollision,27)
--elements["PNT-010"] = default_2_position_tumb(_("Covert Light Switch, NORM/OFF"),			devices.EXTLIGHTS, device_commands.CovertLight,	10)
elements["PNT-026"]	= default_2_position_tumb(_("Landing Light Switch, ON/OFF"),			devices.EXTLIGHTS, device_commands.LandingLightSw,	26)


--Avionics
elements["PNT-028"]	= default_axis(_("Altimeter Setting Knob"), devices.AVIONICS, device_commands.AltimeterSet, 28, 0.04, 1, false, true)
elements["PNT-029"]	= default_axis_limited(_("ADI Adjustment Knob"), devices.AVIONICS, device_commands.ADIadjust, 29, nil, nil, nil, nil, {-1,1})
elements["PNT-031"]	= default_axis(_("LO flag setting knob"), devices.AVIONICS, device_commands.LOset, 31)
elements["PNT-032"]	= default_axis(_("HI flag setting knob"), devices.AVIONICS, device_commands.HIset, 32)
