dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")

-- TODO add animation for ON knob to control power

update_time_step = 0.1
make_default_activity(update_time_step) 
local dev = GetSelf()
local sensor_data = get_base_data()

local ARGUS_Brightness = get_param_handle("ARGUS_Brightness")
ARGUS_Brightness:set(1)
local ARGUS_Power = get_param_handle("ARGUS_Power")
ARGUS_Power:set(1)
local ARGUS_Mode = get_param_handle("ARGUS_Mode")
local ARGUS_Scale = get_param_handle("ARGUS_Scale")
ARGUS_Scale:set(2)

local ARGUS_GndSpd = get_param_handle("ARGUS_GndSpd")
local ARGUS_Distance = get_param_handle("ARGUS_Distance")
local ARGUS_Bearing = get_param_handle("ARGUS_Bearing")
local ARGUS_TTG_Hour = get_param_handle("ARGUS_TTG_Hour")
local ARGUS_TTG_Min = get_param_handle("ARGUS_TTG_Min")
local ARGUS_TTG_Sec = get_param_handle("ARGUS_TTG_Sec")


local mps_to_knot = 1.94384
local degrees_per_radian = 57.2957795

local DEPmode = 0
local ENRmode = 1
local ARRmode = 2

local currentMode = ENRmode
ARGUS_Mode:set(currentMode)

function post_initialize()
	dev:performClickableAction(device_commands.ArgusBrightness, 1)

	local birth = LockOn_Options.init_conditions.birth_place	--"GROUND_COLD","GROUND_HOT","AIR_HOT"
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then   
	
    elseif birth=="GROUND_COLD" then

    end
end

local holdTime = 0.5
local DEPpressed = false
local ENRpressed = false

function SetCommand(command,value)
    if command == device_commands.ArgusDEPbutton then
		if currentMode == DEPmode then
			if value==1 then
				DEPpressed = true
			else
				DEPpressed = false
				if holdTime >0 then
					incScale()
				else
					decScale()
				end
				holdTime=0.5
			end
		elseif value == 0 then
			currentMode = DEPmode
			ARGUS_Scale:set(limit(ARGUS_Scale:get(),2,40))
		end
	elseif command == device_commands.ArgusENRbutton then
		if currentMode == ENRmode then
			if value==1 then
				ENRpressed = true
			else
				ENRpressed = false
				if holdTime >0 then
					incScale()
				else
					decScale()
				end
				holdTime=0.5
			end
		elseif value == 0 then
			currentMode = ENRmode
		end
	elseif command == device_commands.ArgusARRbutton then
		currentMode = ARRmode
	elseif command == device_commands.ArgusAUXbutton then
	elseif command == device_commands.ArgusSELbutton then
	elseif command == device_commands.ArgusINFObutton then
	elseif command == device_commands.ArgusEMERbutton then
	elseif command == device_commands.ArgusBrightness then
		ARGUS_Brightness:set(value)
	end

	ARGUS_Mode:set(currentMode)
end

function incScale()
	if ARGUS_Scale:get()<=2 then
		ARGUS_Scale:set(5)
	elseif ARGUS_Scale:get()==5 then
		ARGUS_Scale:set(10)
	elseif ARGUS_Scale:get()==10 then
		ARGUS_Scale:set(15)
	elseif ARGUS_Scale:get()==15 then
		ARGUS_Scale:set(20)
	elseif ARGUS_Scale:get()==20 then
		ARGUS_Scale:set(30)
	elseif ARGUS_Scale:get()==30 then
		ARGUS_Scale:set(40)
	elseif ARGUS_Scale:get()==40 and currentMode==ENRmode then
		ARGUS_Scale:set(60)
	elseif ARGUS_Scale:get()==60 and currentMode==ENRmode then
		ARGUS_Scale:set(120)
	elseif ARGUS_Scale:get()==120 and currentMode==ENRmode then
		ARGUS_Scale:set(240)
	else
		ARGUS_Scale:set(2)
	end
end
function decScale()
	if ARGUS_Scale:get()==240 and currentMode==ENRmode then
		ARGUS_Scale:set(120)
	elseif ARGUS_Scale:get()==120 and currentMode==ENRmode then
		ARGUS_Scale:set(60)
	elseif ARGUS_Scale:get()==60 and currentMode==ENRmode then
		ARGUS_Scale:set(40)
	elseif ARGUS_Scale:get()>=40 then
		ARGUS_Scale:set(30)
	elseif ARGUS_Scale:get()==30 then
		ARGUS_Scale:set(20)
	elseif ARGUS_Scale:get()==20 then
		ARGUS_Scale:set(15)
	elseif ARGUS_Scale:get()==15 then
		ARGUS_Scale:set(10)
	elseif ARGUS_Scale:get()==10 then
		ARGUS_Scale:set(5)
	elseif ARGUS_Scale:get()==5 then
		ARGUS_Scale:set(2)
	elseif currentMode==ENRmode then
		ARGUS_Scale:set(240)
	else
		ARGUS_Scale:set(40)
	end
end


function update()
	local hasPower = get_param_handle("DC_Bus_Voltage"):get()>16 and get_param_handle("AC_26_Bus_Voltage"):get()>16 -- and add power knob too
	ARGUS_Power:set(hasPower)

	local Vx, Vy, Vz = sensor_data.getSelfVelocity()--- DCS world axis: x is +north, y is +up, z is +east
	ARGUS_GndSpd:set(math.sqrt((Vx^2)+(Vz^2))*mps_to_knot)
	--groundspeed_track:set(math.atan2(Vz,Vx)+sensor_data.getHeading())
	--if ARGUS_GndSpd:get()<1 then groundspeed_track:set(0) end

	ARGUS_Distance:set(get_param_handle("TACAN_RANGE"):get())
	ARGUS_Bearing:set(get_param_handle("TACAN_BEARING"):get())
	ARGUS_TTG_Hour:set(get_param_handle("TACAN_TTG_Hour"):get())
	ARGUS_TTG_Min:set(get_param_handle("TACAN_TTG_Min"):get())
	ARGUS_TTG_Sec:set(get_param_handle("TACAN_TTG_Sec"):get())

	
	if DEPpressed or ENRpressed then
		holdTime=holdTime-update_time_step
	end
	
end

