dofile(LockOn_Options.common_script_path..'Radio.lua')
dofile(LockOn_Options.common_script_path.."mission_prepare.lua")

local gettext = require("i_18n")
_ = gettext.translate

GUI = {
	range = {min = 30E6, max = 151.975E6, step = 25E3}, --Hz
	displayName = _('VHF Radio AN/ARC-186'),
	AM = true,
	FM = true,
}


dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")


--TODO: add LOAD mode to change presets in game

dev = GetSelf()
local update_time_step = 0.5 --update will be called once per second
make_default_activity(update_time_step)

RadioBusVoltage  = get_param_handle("Radio_Bus_Voltage")


local frequency = 133000
local freqTens = 130000
local freqOnes = 3000
local freqTenths = 0
local freqHundredths = 0

local modeSelected = 1 -- 0:preset, 1:manual, 2:emergency AM, 3:emergency FM
local channelSelected = 1
local radio1Channels = get_aircraft_mission_data("Radio")[1].channels
local squelchOn = false
local switchOn = false

function post_initialize()
	dev:performClickableAction(device_commands.ARC186_vol, 1.0)
	dev:performClickableAction(device_commands.ARC186_FreqMode, 1/3)
	dev:performClickableAction(device_commands.ARC186_SquelchToneSw, 0)
	dev:performClickableAction(device_commands.ARC186_10MHz, 0.7692)
	dev:performClickableAction(device_commands.ARC186_1MHz, 0.3)

	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then
		dev:performClickableAction(device_commands.ARC186_mode, 1)
    elseif birth=="GROUND_COLD" then
    end

	dev:set_modulation(0)-- AM modulation

	--radio1Channels = get_aircraft_mission_data("Radio")[1].channels
end

--dev:listen_command(device_commands.ARC186_10MHz)

function SetCommand(command,value)
	if command == device_commands.ARC186_10MHz then
		freqTens = LinInterp(value, 0, 0.923, 3, 15)-- this isn't very accurate
		freqTens = round(freqTens)*10000 -- fix the bad accuracy here
		freqTens = limit(freqTens, 0, 160000)
	elseif command == device_commands.ARC186_1MHz then
		freqOnes = 10000*value
		freqOnes = limit(freqOnes, 0, 9000)
	elseif command == device_commands.ARC186_tenthMHz then
		freqTenths = 1000*value
		freqTenths = limit(freqTenths, 0, 900)	
	elseif command == device_commands.ARC186_quartMHz then
		freqHundredths = 100*value
		freqHundredths = limit(freqHundredths, 0, 75)
	elseif command == device_commands.ARC186_mode then
		switchOn = value > 0
	elseif command == device_commands.ARC186_FreqMode then
		modeSelected = math.floor(value*3+0.01)
	elseif command == device_commands.ARC186_chan then
		channelSelected = math.floor(value*20+1.01)
	elseif command == device_commands.ARC186_SquelchToneSw then
		squelchOn = value > -1
		dev:set_squelch(squelchOn)
	elseif command == device_commands.ARC186_vol then
		dev:set_volume(value)
	end

	frequency = freqTens + freqOnes + freqTenths + freqHundredths

	if modeSelected==0 then	-- preset
		dev:set_frequency(radio1Channels[channelSelected]*1000000)
	elseif modeSelected==1 then	-- manual
		dev:set_frequency(frequency*1000)
	elseif modeSelected==2 then	-- AM guard
		dev:set_frequency(121500*1000)
	elseif modeSelected==3 then -- FM guard
		dev:set_frequency(40500*1000)
	end
	--print_message_to_user("radio device: "..dev:get_frequency())
	--print_message_to_user(radio1Channels[channelSelected])
end

function update()
	if RadioBusVoltage:get() > 20 and switchOn then
		dev:set_on(true)
	else
		dev:set_on(false)
	end
end




need_to_be_closed = false


--[[
available functions:

set_frequency() in hz
get_frequency() returns freq in hz
set_modulation() AM:0, FM:1
is_frequency_in_range() returns boolean
set_squelch() boolean
set_volume() 0-1
set_on() boolean

use like this: 
dev = GetSelf()
dev:set_frequency(freq_in_hz)

]]