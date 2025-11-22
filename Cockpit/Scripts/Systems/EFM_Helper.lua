dofile(LockOn_Options.script_path.."command_defs.lua")

-- This device is used to help initialize clickable switches and to interface keyboard bindings with clickables
-- performClickableAction doesn't seem to send the command to the EFM, so dispatch_action is used for that

local update_rate = 0.1
make_default_activity(update_rate)
local dev = GetSelf()

local SHOW_CONTROLS  = get_param_handle("SHOW_CONTROLS")
local option_aimingMarkRemove = get_plugin_option_value("AH-6J","aimingMarkRemove","local")


function post_initialize()
	SHOW_CONTROLS:set(1)
    local birth = LockOn_Options.init_conditions.birth_place
    if birth=="AIR_HOT" or birth=="GROUND_HOT" then
		dev:performClickableAction(EFM_commands.throttleIdleCutoff,0)
		dev:performClickableAction(EFM_commands.throttle,1)
		dev:performClickableAction(EFM_commands.batterySwitch,1) 
		dev:performClickableAction(EFM_commands.generatorSwitch,1)
		dev:performClickableAction(EFM_commands.inverterSwitch,1)
    elseif birth=="GROUND_COLD" then
		dev:performClickableAction(EFM_commands.throttleIdleCutoff, 1)
		dev:performClickableAction(EFM_commands.throttle,-1)
		dev:performClickableAction(EFM_commands.batterySwitch,0) 
		dev:performClickableAction(EFM_commands.generatorSwitch,0)
		dev:performClickableAction(EFM_commands.inverterSwitch,0)
    end
	
	if option_aimingMarkRemove == true then
		set_aircraft_draw_argument_value(510,1)
	else
		set_aircraft_draw_argument_value(510,0)
	end
	
	--show_param_handles_list()--see all param handles in-game
end



dev:listen_command(Keys.BattSwitch)
dev:listen_command(Keys.ExtPwrSwitch)
dev:listen_command(Keys.ThrottleIncrease)
dev:listen_command(Keys.ThrottleDecrease)
dev:listen_command(Keys.ThrottleCutoff)
dev:listen_command(Keys.showControlInd)

function SetCommand(command,value)
	PwrSwpos = get_cockpit_draw_argument_value(17)
	Throtpos = get_cockpit_draw_argument_value(4)
	CutOffpos = get_cockpit_draw_argument_value(5)
	if command == Keys.BattSwitch then
		if PwrSwpos == 1 then
			dev:performClickableAction(EFM_commands.batterySwitch,0)
			dispatch_action(nil,EFM_commands.batterySwitch,0)
		elseif PwrSwpos < 1 then
			dev:performClickableAction(EFM_commands.batterySwitch,1)
			dispatch_action(nil,EFM_commands.batterySwitch,1)
		end
	elseif command == Keys.ExtPwrSwitch then
		if PwrSwpos == -1 then
			dev:performClickableAction(EFM_commands.batterySwitch,0)
			dispatch_action(nil,EFM_commands.batterySwitch,0)
		elseif PwrSwpos > -1 then
			dev:performClickableAction(EFM_commands.batterySwitch,-1)
			dispatch_action(nil,EFM_commands.batterySwitch,-1)
		end
	elseif command==Keys.ThrottleIncrease then
		local amount = Throtpos + 0.002
		if amount > 0.998 then
			amount = 0.998
		end
		dev:performClickableAction(EFM_commands.throttle,amount)
		dispatch_action(nil,EFM_commands.throttle,amount)
	elseif command==Keys.ThrottleDecrease then
		dev:performClickableAction(EFM_commands.throttle,Throtpos - 0.002)
		dispatch_action(nil,EFM_commands.throttle,Throtpos - 0.002)
	elseif command==Keys.ThrottleCutoff then
		dev:performClickableAction(EFM_commands.throttleIdleCutoff,1-CutOffpos)
		dispatch_action(nil,EFM_commands.throttleIdleCutoff,1-CutOffpos)
	elseif command == Keys.showControlInd then
		SHOW_CONTROLS:set(1-SHOW_CONTROLS:get())
		
		
	elseif command == EFM_commands.throttle then
		if CutOffpos==0 and value<0 then
			dev:performClickableAction(EFM_commands.throttle,0)--stop throttle from moving past idle stop
		elseif CutOffpos==1 and value>0 then
			dev:performClickableAction(EFM_commands.throttleIdleCutoff,0)--cutoff snaps into locked position
		end

	end
end

external_power = get_param_handle("EXTERNAL_POWER")
external_power:set(0)

dev:listen_event("GroundPowerOn")
dev:listen_event("GroundPowerOff")
function CockpitEvent(event,val)
    if event == "GroundPowerOn" then
        external_power:set(1)
    elseif event == "GroundPowerOff" then
        external_power:set(0)
    end
end

function update()
--print_message_to_user(FMtest:get())
end


need_to_be_closed = false -- close lua state after initialization