dofile(LockOn_Options.common_script_path..'Radio.lua')
dofile(LockOn_Options.common_script_path.."mission_prepare.lua")

local gettext = require("i_18n")
_ = gettext.translate

GUI = {
	range = {min = 108E6, max = 399.975E6, step = 25E3}, --Hz
	displayName = _('V/UHF Radio AN/ARC-182'),
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



local display_on = get_param_handle("ARC182_ON")
display_on:set(0)
local brightness = get_param_handle("ARC182_BRIGHTNESS")
brightness:set(1)
local ARC182_DECIMAL = get_param_handle("ARC182_DECIMAL")
ARC182_DECIMAL:set(1)
local display_frequency = get_param_handle("ARC182_FREQUENCY") -- for digital freqency readout
local frequency = 256000
display_frequency:set(frequency)
local freqTens = 250000
local freqOnes = 6000
local freqTenths = 0
local freqHundredths = 0

local modeSelected = 0 -- 0:man, 1:guard, 2:preset(display channel), 3:read(display channel freq)
local channelSelected = 1
local testOn = false
local radio2Channels = get_aircraft_mission_data("Radio")[2].channels
local squelchOn = false
local switchOn = false

function post_initialize()
	dev:performClickableAction(device_commands.ARC182_AMFM, 1)
	dev:performClickableAction(device_commands.ARC182_vol, 1.0)
	dev:performClickableAction(device_commands.ARC182_brightness, 1)
	dev:performClickableAction(device_commands.ARC182_FreqSelType, 0.2)
	dev:performClickableAction(device_commands.ARC182_squelch, 1)

	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then
		dev:performClickableAction(device_commands.ARC182_mode, 0.5)
    elseif birth=="GROUND_COLD" then
    end

	--radio2Channels = get_aircraft_mission_data("Radio")[2].channels
end


--[[
dev:listen_command(Keys.COMM1ModeCW)
dev:listen_command(Keys.COMM1ModeCCW)
dev:listen_command(Keys.COMM1FreqModeCW)
dev:listen_command(Keys.COMM1FreqModeCCW)
dev:listen_command(Keys.COMM1ChanCW)
dev:listen_command(Keys.COMM1ChanCCW)
--]]
function SetCommand(command,value)
	if command == device_commands.ARC182_freqTens then
		freqTens = freqTens + 10000*value
		freqTens = limit(freqTens, 0, 400000)
		--print_message_to_user(value)
	elseif command == device_commands.ARC182_freqOnes then
		freqOnes = freqOnes + 1000*value
		freqOnes = limit(freqOnes, 0, 9000)
	elseif command == device_commands.ARC182_freqTenths then
		freqTenths = freqTenths + 100*value
		freqTenths = limit(freqTenths, 0, 900)
	elseif command == device_commands.ARC182_freqHundredths then
		freqHundredths = freqHundredths + 25*value
		freqHundredths = limit(freqHundredths, 0, 75)
	elseif command == device_commands.ARC182_AMFM then
		dev:set_modulation(1- value) 
	elseif command == device_commands.ARC182_mode then
		switchOn = value > 0
		testOn = value == 1
	elseif command == device_commands.ARC182_brightness then
		brightness:set(value)
	elseif command == device_commands.ARC182_FreqSelType then
		modeSelected = math.ceil((value-0.21)/0.2)
	elseif command == device_commands.ARC182_ChannelSel then
		channelSelected = math.floor(value*30+1.01)
	elseif command == device_commands.ARC182_squelch then
		squelchOn = value == 1
		dev:set_squelch(squelchOn)
	elseif command == device_commands.ARC182_vol then
		dev:set_volume(value)
	--[[elseif command == Keys.COMM1ModeCW then
		dev:performClickableAction(device_commands.ARC182_mode, 0.5)--TODO step through all options
	elseif command == Keys.COMM1ModeCCW then
		dev:performClickableAction(device_commands.ARC182_mode, 0.0)
	elseif command == Keys.COMM1FreqModeCW then
		dev:performClickableAction(device_commands.ARC182_FreqSelType, limit(modeSelected*0.22+0.22,0,1))
	elseif command == Keys.COMM1FreqModeCCW then
		dev:performClickableAction(device_commands.ARC182_FreqSelType, limit(modeSelected*0.22-0.22,0,1))
	elseif command == Keys.COMM1ChanCW then
		dev:performClickableAction(device_commands.ARC182_ChannelSel, limit((channelSelected-1)/30+1/30,0,1))
	elseif command == Keys.COMM1ChanCCW then
		dev:performClickableAction(device_commands.ARC182_ChannelSel, limit((channelSelected-1)/30-1/30,0,1))
		--]]
	end

	frequency = freqTens + freqOnes + freqTenths + freqHundredths
	local chanFreq = radio2Channels[channelSelected]

	if modeSelected==0 then	-- manual
		display_frequency:set(frequency)
		ARC182_DECIMAL:set(1)
		dev:set_frequency(frequency*1000)
	elseif modeSelected==1 then	-- guard
		display_frequency:set(243000)
		ARC182_DECIMAL:set(1)
		dev:set_frequency(243000*1000)
	elseif modeSelected==2 then	-- preset
		display_frequency:set(channelSelected)
		ARC182_DECIMAL:set(0)
		dev:set_frequency(chanFreq*1000000)
	elseif modeSelected==3 then -- read
		display_frequency:set(chanFreq*1000)
		ARC182_DECIMAL:set(1)
		dev:set_frequency(chanFreq*1000000)
	end
	if testOn then
		display_frequency:set(888888)
		ARC182_DECIMAL:set(1)
	end
	--print_message_to_user(dev:get_frequency())
	--print_message_to_user(chanFreq)
end

function update()
	if RadioBusVoltage:get() > 20 and switchOn then
		dev:set_on(true)
		display_on:set(1)
	else
		dev:set_on(false)
		display_on:set(0)
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