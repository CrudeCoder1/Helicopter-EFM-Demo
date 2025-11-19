dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local update_time_step = 0.01 --update will be called 10 times per second
make_default_activity(update_time_step)

elec_dc_ok  = get_param_handle("DC_POWER_AVAIL")

function post_initialize()

end


dev:listen_command(Keys.LandingLight)
dev:listen_command(device_commands.LandingLightSw)
dev:listen_command(device_commands.PositionLights)
dev:listen_command(device_commands.Formation)
dev:listen_command(device_commands.Beacon)


local extLDG = 0
local navBrightness = 0
local formationBrightness = 0

function SetCommand(command,value)
    if command == device_commands.LandingLightSw then
        extLDG = value
    elseif command == device_commands.PositionLights then
        navBrightness=value
    elseif command == device_commands.Formation then
		formationBrightness = value
	elseif command == Keys.LandingLight then
		dev:performClickableAction(device_commands.LandingLightSw, (1-extLDG),true)
	end
end


function update() 
--local lightState = get_aircraft_draw_argument_value(208)    
 --print_message_to_user(":"..lightState)
	if elec_dc_ok:get()==1 then
	--	set_aircraft_draw_argument_value(51,extlight_taxi) -- 51 is animation to move landing lights open, 208 for actual light beam
		set_aircraft_draw_argument_value(208,extLDG) 
		set_aircraft_draw_argument_value(190,navBrightness)
	--	set_aircraft_draw_argument_value(88,formationBrightness)
	else		-- no electrical power, turn lights off
	--	set_aircraft_draw_argument_value(51,0) 
		set_aircraft_draw_argument_value(208,0) 
		set_aircraft_draw_argument_value(190,0)
	--	set_aircraft_draw_argument_value(88,0)
	end
end

need_to_be_closed = false 