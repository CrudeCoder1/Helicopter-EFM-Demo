dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")


update_time_step = 0.1
make_default_activity(update_time_step) 
local dev = GetSelf()
local sensor_data = get_base_data()
local Terrain = require("terrain")
local mps_to_knot = 1.94384
local degrees_per_radian = 57.2957795


local TNL3100_Power = get_param_handle("TNL3100_Power")
TNL3100_Power:set(1)
local TNL3100_Line1 = get_param_handle("TNL3100_Line1")
local TNL3100_Line2 = get_param_handle("TNL3100_Line2")


local pwrKnob = 0


function post_initialize()

	local birth = LockOn_Options.init_conditions.birth_place	--"GROUND_COLD","GROUND_HOT","AIR_HOT"
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then   
		dev:performClickableAction(device_commands.TNL_PWR, 1)
    elseif birth=="GROUND_COLD" then

    end
end

function SetCommand(command,value)
	if command == device_commands.TNL_PWR then
		pwrKnob = value

	end

end

function update()
	local hasPower = get_param_handle("DC_Bus_Voltage"):get()>16 and get_param_handle("AC_26_Bus_Voltage"):get()>16 and pwrKnob==1-- and add power knob too
	TNL3100_Power:set(hasPower and 1 or 0)


	
end

need_to_be_closed = false