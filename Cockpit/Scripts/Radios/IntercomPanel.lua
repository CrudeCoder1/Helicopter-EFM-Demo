dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")
dofile(LockOn_Options.common_script_path..'Radio.lua')


dev = GetSelf()
local update_time_step = 0.5 --update will be called once per second
make_default_activity(update_time_step)

RadioBusVoltage  = get_param_handle("Radio_Bus_Voltage")


function post_initialize()
	dev:performClickableAction(device_commands.P_ICS_MastVol, 0.8)
	dev:performClickableAction(device_commands.P_ICS_MON1, 0.9)
	dev:performClickableAction(device_commands.P_ICS_MON2, 0.9)
	dev:performClickableAction(device_commands.P_ICS_MON3, 0.9)
	dev:performClickableAction(device_commands.P_ICS_Mode, 2/7)

	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then
		
    elseif birth=="GROUND_COLD" then
    end

end

--dev:listen_command(Keys.COMM1Chan)

function SetCommand(command,value)
local ICSDevice = GetDevice(devices.INTERCOM)
	if command == device_commands.P_ICS_MastVol then
		ICSDevice:set_master_volume(value)
	elseif command == device_commands.P_ICS_MON1 then
		ICSDevice:set_receiver1_Volume(value)
	elseif command == device_commands.P_ICS_MON2 then
		ICSDevice:set_receiver2_Volume(value)
	elseif command == device_commands.P_ICS_MON3 then
		ICSDevice:set_receiver3_Volume(value)
	elseif command == device_commands.P_ICS_Mode then
		local modVal=math.floor(value*7+0.01)
		if modVal == 1 then
			ICSDevice:set_mode(0)
		elseif modVal == 2 then
			ICSDevice:set_mode(1)
		elseif modVal == 3 then
			ICSDevice:set_mode(2)
		elseif modVal == 4 then
			ICSDevice:set_mode(3)
		else
			ICSDevice:set_mode(0)
		end
		--print_message_to_user(modVal)
	end
end

function update()

end

need_to_be_closed = false