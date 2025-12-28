dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
--local sensor_data = get_base_data()
local update_time_step = 0.1  
make_default_activity(update_time_step)

-- VIDS (Vertical Instrument Displays)

local VIDS_MODE = get_param_handle("VIDS_MODE")
local VIDS_BRIGHTNESS = get_param_handle("VIDS_BRIGHTNESS")
local VIDS_TOT = get_param_handle("VIDS_TOT")
local VIDS_TRQ = get_param_handle("VIDS_TRQ")
local VIDS_N2 = get_param_handle("VIDS_N2")
local VIDS_NR = get_param_handle("VIDS_NR")


function post_initialize()
	dev:performClickableAction(device_commands.VIDSbrtKnob, 1)
end


function SetCommand(command,value)   
    if command == device_commands.VIDSdigitSw then
		VIDS_MODE:set(value)
	elseif command == device_commands.VIDSbrtKnob then
		VIDS_BRIGHTNESS:set(value)
	end
end



function update()
	VIDS_TOT:set(get_param_handle("ENGINE_TOT"):get())
	VIDS_TRQ:set(get_param_handle("ENGINE_TRQ"):get())
	
	VIDS_N2:set(get_param_handle("EFM_N2_RPM"):get())
	VIDS_NR:set(get_param_handle("EFM_NR_RPM"):get())
	
	if VIDS_MODE:get()==1 then
		VIDS_TOT:set(888)
		VIDS_TRQ:set(888)
		VIDS_N2:set(888)
		VIDS_NR:set(888)
	end
end

need_to_be_closed = false 