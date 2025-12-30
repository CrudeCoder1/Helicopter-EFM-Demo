dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local sensor_data = get_base_data()
local update_time_step = 0.2  
make_default_activity(update_time_step)

local GMThour = get_param_handle("GMT_HOURS")
local GMTmin = get_param_handle("GMT_MINS")
local GMTsec = get_param_handle("GMT_SECS")
local CLOCK_HOURS = get_param_handle("CLOCK_HOURS")
local CLOCK_MINS = get_param_handle("CLOCK_MINS")
local CLOCK_MODE = get_param_handle("CLOCK_MODE") -- 0:GMT, 1:LocalTime, 2: ElapsedTime
local CLOCK_BRIGHTNESS = get_param_handle("CLOCK_BRIGHTNESS")


--theatre  = get_terrain_related_data("name")
local Terrain = require("terrain")
local GMToffset = 0
function post_initialize()
	local timeDelta = Terrain.GetTerrainConfig("SummerTimeDelta")
	GMToffset = -timeDelta
	--[[if theatre == 'Caucasus' then
		GMToffset = -3
	elseif theatre == 'Nevada' then
		GMToffset = 8
	elseif theatre == 'Persian Gulf' then
		GMToffset = -4
	--elseif theatre == 'Syria' then -- syria doesnt seem to be registered as a theatre 
		--GMToffset = -2
	end--]]

	dev:performClickableAction(device_commands.M880Brightness, 1)
	CLOCK_MODE:set(1)
end

local localTime = 0
local ETisCounting = false
local elapsedTime = 0

dev:listen_command(device_commands.AltimeterSet)
function SetCommand(command,value)   
    if command == device_commands.M880Select and value >0 then
		CLOCK_MODE:set(CLOCK_MODE:get()+1)
		if CLOCK_MODE:get()>2 then
			CLOCK_MODE:set(0)
		end
	elseif command == device_commands.M880Control and value >0 then
		if CLOCK_MODE:get()==2 then
			ETisCounting = not ETisCounting
		end		
	elseif command == device_commands.M880Brightness then
		CLOCK_BRIGHTNESS:set(value)
	end
	
end



function update()
	local abstime = get_absolute_model_time() -- gives local time of day in seconds
	
    local hour = abstime/3600.0
    localTime =hour
    local int,frac = math.modf(hour)
    GMTmin:set(math.floor(frac*60))
	local int1,frac1 = math.modf(frac*60)
	GMTsec:set(frac1*60)
	
	-- fix wraparound
	if (hour+GMToffset) < 0 then 
		GMThour:set(hour + 24 + GMToffset)
	elseif (hour+GMToffset) > 24 then
		GMThour:set(hour - 24 + GMToffset)
	else
		GMThour:set(hour + GMToffset)
	end
	
	
	if ETisCounting then
		elapsedTime = elapsedTime + update_time_step
	else
		elapsedTime = 0
	end
	
	local ET_Hours = elapsedTime/3600
	local int3,frac3 = math.modf(ET_Hours)
	local ET_Mins = math.floor(frac3*60)
	local int4,frac4 = math.modf(frac3*60)
	local ET_Secs = math.floor(frac4*60)
	
	if CLOCK_MODE:get()==1 then
		CLOCK_HOURS:set(localTime)
		CLOCK_MINS:set(GMTmin:get())
	elseif CLOCK_MODE:get()==2 then
		if elapsedTime>=3600 then
			CLOCK_HOURS:set(ET_Hours)
			CLOCK_MINS:set(ET_Mins)
		else
			CLOCK_HOURS:set(ET_Mins)
			CLOCK_MINS:set(ET_Secs)
		end	
	else
		CLOCK_HOURS:set(GMThour:get())
		CLOCK_MINS:set(GMTmin:get())
	end

end

need_to_be_closed = false 