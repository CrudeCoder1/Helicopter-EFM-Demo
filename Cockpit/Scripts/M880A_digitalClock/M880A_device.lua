dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local sensor_data = get_base_data()
local update_time_step = 0.2  
make_default_activity(update_time_step)

local GMThour = get_param_handle("GMT_HOURS")
local GMTmin = get_param_handle("GMT_MINS")
local GMTsec = get_param_handle("GMT_SECS")

local LThour = get_param_handle("LT_HOURS")

local clockDisplayMode = get_param_handle("clockDisplayMode") -- 0:GMT, 1:LocalTime, 2: ElapsedTime


theatre  = get_terrain_related_data("name")
local GMToffset = 0
function post_initialize()
	if theatre == 'Caucasus' then
		GMToffset = -3
	elseif theatre == 'Nevada' then
		GMToffset = 8
	elseif theatre == 'Persian Gulf' then
		GMToffset = -4
	--elseif theatre == 'Syria' then -- syria doesnt seem to be registered as a theatre 
		--GMToffset = -2
	end
	--print_message_to_user(theatre)
end

dev:listen_command(device_commands.AltimeterSet)
function SetCommand(command,value)   
    if command == device_commands.AltimeterSet then
		
		if alt_setting > ALT_PRESSURE_MAX then
			
		elseif alt_setting < ALT_PRESSURE_MIN then
			
		end
	end
end



function update()
	local abstime = get_absolute_model_time() -- gives local time of day in seconds
	
    local hour = abstime/3600.0
    LThour:set(hour)
    local int,frac = math.modf(hour)
    GMTmin:set(math.floor(frac*60))
	local int1,frac1 = math.modf(frac*60)
	GMTsec:set(frac1*60)
	
	GMThour:set(hour + GMToffset)

end

need_to_be_closed = false 