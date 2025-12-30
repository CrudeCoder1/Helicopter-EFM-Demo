dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local update_time_step = 0.1 --update will be called 10 times per second
make_default_activity(update_time_step)

local cockpitDev = GetDevice(0)



DCbusVoltage  = get_param_handle("DC_Bus_Voltage")

local LdgingLghtSw = 0
local posLightSw = 0
--local formationBrightness = 0
local radioBrght = 0
local panelBrght = 0
local AMSbackBrght = 0
local LightingKill = false

function post_initialize()
	local birth = LockOn_Options.init_conditions.birth_place
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 	 
        dev:performClickableAction(device_commands.PositionLights, -1)
    elseif birth=="GROUND_COLD" then
		dev:performClickableAction(device_commands.PositionLights, 0)
    end
	dev:performClickableAction(device_commands.RadioLightKnob, 0.8)
	dev:performClickableAction(device_commands.AMSBacklightKnob, 0.8)
	dev:performClickableAction(device_commands.PanelLightKnob, 0.8)
end


dev:listen_command(Keys.LandingLight)
function SetCommand(command,value)
    if command == device_commands.LandingLightSw then
        LdgingLghtSw = value
    elseif command == device_commands.PositionLights then
        posLightSw=value
   -- elseif command == device_commands.Formation then
		--formationBrightness = value
	elseif command == Keys.LandingLight then
		dev:performClickableAction(device_commands.LandingLightSw, 1-LdgingLghtSw)
	elseif command == device_commands.RadioLightKnob then
		radioBrght = value
	elseif command == device_commands.PanelLightKnob then
		panelBrght = value
	elseif command == device_commands.AMSBacklightKnob then
		AMSbackBrght = value
	elseif command == device_commands.LightKillSw and value > 0 then
		LightingKill = not LightingKill
	end
end


local function updateExternalLights()
	if DCbusVoltage:get()>=15 then
	--	set_aircraft_draw_argument_value(51,extlight_taxi) -- 51 is animation to move landing lights open, 208 for actual light beam
		set_aircraft_draw_argument_value(208,LdgingLghtSw) 
		
		if posLightSw<=0 then
			set_aircraft_draw_argument_value(190,-posLightSw)
			set_aircraft_draw_argument_value(201, 0)
		else -- covert
			set_aircraft_draw_argument_value(190,0)
			set_aircraft_draw_argument_value(201, 1)
		end
	--	set_aircraft_draw_argument_value(88,formationBrightness)
	else		-- no electrical power, turn lights off
	--	set_aircraft_draw_argument_value(51,0) 
		set_aircraft_draw_argument_value(208,0) 
		set_aircraft_draw_argument_value(190,0)
	--	set_aircraft_draw_argument_value(88,0)
	end
	
	if LightingKill then
		set_aircraft_draw_argument_value(190,0)
		set_aircraft_draw_argument_value(208,0)
	end
end

local function updateInternalLights()

	if DCbusVoltage:get()>=15 then
		cockpitDev:set_argument_value(453, radioBrght)
		cockpitDev:set_argument_value(400, panelBrght)
		cockpitDev:set_argument_value(450, AMSbackBrght)
	else -- no electrical power, turn lights off
		cockpitDev:set_argument_value(453, 0)
		cockpitDev:set_argument_value(400, 0)
		cockpitDev:set_argument_value(450, 0)
	end
end


function update() 
	updateExternalLights()
	updateInternalLights()
end


need_to_be_closed = false 