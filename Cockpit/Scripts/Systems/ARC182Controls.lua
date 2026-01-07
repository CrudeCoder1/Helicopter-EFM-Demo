dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")
dofile(LockOn_Options.common_script_path..'Radio.lua')

dev = GetSelf()
local update_time_step = 1 --update will be called once per second
make_default_activity(update_time_step) 

local display_on = get_param_handle("ARC182_ON")
display_on:set(0)
local brightness = get_param_handle("ARC182_BRIGHTNESS")
brightness:set(1)
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
local radio1Channels

local INTERCOM_PTT = get_param_handle("INTERCOM_PTT")

function post_initialize()
	dev:performClickableAction(device_commands.ARC182_AMFM, 1)
	dev:performClickableAction(device_commands.ARC182_vol, 0.8)
	dev:performClickableAction(device_commands.ARC182_brightness, 1)
	dev:performClickableAction(device_commands.ARC182_FreqSelType, 0.2)
	dev:performClickableAction(device_commands.ICSvol, 0.8)
	--dev:performClickableAction(device_commands.COMM1Switch, 1) -- these switches are always left on
	--dev:performClickableAction(device_commands.COMM2Switch, 1) 
	
	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then
		dev:performClickableAction(device_commands.ARC182_mode, 0.5)
    elseif birth=="GROUND_COLD" then
    end
	
	radio1Channels = get_aircraft_mission_data("Radio")[1].channels
end

dev:listen_command(device_commands.ARC182_freqTens)
dev:listen_command(device_commands.ARC182_freqOnes)
dev:listen_command(device_commands.ARC182_freqTenths)
dev:listen_command(device_commands.ARC182_freqHundredths)
dev:listen_command(device_commands.ARC182_AMFM)
dev:listen_command(device_commands.ARC182_mode) 
dev:listen_command(device_commands.ARC182_brightness) 

dev:listen_command(Keys.COMM1ModeCW)
dev:listen_command(Keys.COMM1ModeCCW)
dev:listen_command(Keys.COMM1FreqModeCW)
dev:listen_command(Keys.COMM1FreqModeCCW)
dev:listen_command(Keys.COMM1ChanCW)
dev:listen_command(Keys.COMM1ChanCCW)

dev:listen_command(Keys.IntercomPTT) 

function SetCommand(command,value)
local radioDevice = GetDevice(devices.UHF_RADIO)
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
		radioDevice:set_modulation(1- value) 
	elseif command == device_commands.ARC182_mode then
		if value > 0 then
			display_on:set(1)
		else
			display_on:set(0)
		end
		if value == 1 then
			testOn = true
		else
			testOn = false
		end
	elseif command == device_commands.ARC182_brightness then
		brightness:set(value)
	elseif command == device_commands.ARC182_FreqSelType then
		modeSelected = math.ceil((value-0.21)/0.2)	
	elseif command == device_commands.ARC182_ChannelSel then
		channelSelected = math.floor(value*30+1.01)
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
	elseif command == Keys.IntercomPTT then
		INTERCOM_PTT:set(value)
	end
	
		
	frequency = freqTens + freqOnes + freqTenths + freqHundredths
	
	if modeSelected==0 then	
		display_frequency:set(frequency)	
		radioDevice:set_frequency(frequency*1000)
	elseif modeSelected==1 then	
		display_frequency:set(243000)	
		radioDevice:set_frequency(243000*1000)
	elseif modeSelected==2 then	
		display_frequency:set(channelSelected)
		radioDevice:set_frequency(radio1Channels[channelSelected]*1000000)
	elseif modeSelected==3 then
		display_frequency:set(radio1Channels[channelSelected]*1000)
		radioDevice:set_frequency(radio1Channels[channelSelected]*1000000)
	end
	if testOn then
		display_frequency:set(888888)
	end
	--print_message_to_user(radioDevice:get_frequency())
end


function update()
	
end

need_to_be_closed = false