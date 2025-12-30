dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local update_time_step = 0.1 --update will be called 10 times per second
make_default_activity(update_time_step)

local cockpitDev = GetDevice(0)

DCbusVoltage  = get_param_handle("DC_Bus_Voltage")

FQI_Brightness  = get_param_handle("FQI_Brightness")

local FQIbrt = 1
local DayNightVal = 1


function post_initialize()
	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 	 
		dev:performClickableAction(device_commands.FuelPumpSw, 1)
    elseif birth=="GROUND_COLD" then
		
    end
	dev:performClickableAction(device_commands.FQIbrtKnob, 0.9)
	dev:performClickableAction(device_commands.FQIdayNhtSw, 1)
end


dev:listen_command(Keys.LandingLight)
function SetCommand(command,value)
    if command == device_commands.FQIbrtKnob then
        FQIbrt = value		
	elseif command == device_commands.FQIdayNhtSw then
		DayNightVal = value/2+0.5
	end
	FQI_Brightness:set(FQIbrt*DayNightVal)
end




function update() 
	
end


need_to_be_closed = false 