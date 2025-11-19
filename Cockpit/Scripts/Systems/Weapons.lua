dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.common_script_path.."devices_defs.lua")
dofile(LockOn_Options.common_script_path.."../../../Database/wsTypes.lua")

local WeaponSystem = GetSelf()
local update_rate = 0.01
make_default_activity(update_rate)


elec_dc_ok  = get_param_handle("DC_POWER_AVAIL")

local pickle_engaged = false


function post_initialize()

    WeaponSystem:performClickableAction(device_commands.SalvoSw,0,true)
    local birth = LockOn_Options.init_conditions.birth_place	--"GROUND_COLD","GROUND_HOT","AIR_HOT"
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 	 
        WeaponSystem:performClickableAction(device_commands.MasterArm,1,true) 	
    elseif birth=="GROUND_COLD" then
        WeaponSystem:performClickableAction(device_commands.MasterArm,0,true)
    end
	set_aircraft_draw_argument_value(1000,0)
end


local release_timer = 0
local release_interval = 0.10 -- time between each shot
local singleFired = 0
local rocketSelect = 0
local gunSelect = 0
function update() 
    if master_arm == 1 and elec_dc_ok:get() == 1 then
		for i=1,5,1 do
			local station = WeaponSystem:get_station_info(i-1)
			if rocketSelect > 0 and rocketSelect < 0.9 and i==1 then
				allowRocketShoot=true
			elseif rocketSelect > 0.34 and i==5 then
				allowRocketShoot=true
			else 
				allowRocketShoot=false
			end
			if pickle_engaged and station.weapon.level2 == wsType_NURS and station.count > 0 and allowRocketShoot then 
				if ripple == 1 then
					release_timer = release_timer + update_rate	
					if release_timer >= release_interval then
					WeaponSystem:launch_station(i-1)
					release_timer = 0
					end
				elseif ripple == 0 and singleFired == 0 then
					singleFired = 1
					WeaponSystem:launch_station(i-1)
				end
			end
		end
	end
	

	local gun0Info = WeaponSystem:get_station_info(0)
	local gun1Info = WeaponSystem:get_station_info(1)
	if gun1Info.weapon.level2== wsType_Shell then
		set_aircraft_draw_argument_value(501,1)	-- draws the right ammo box if the gun is mounted
	else 
		if gun0Info.weapon.level2== wsType_Shell then
			set_aircraft_draw_argument_value(501,1)	-- draws the right ammo box if the gun is mounted
		else
			set_aircraft_draw_argument_value(501,0)
		end
	end
	
	local gun3Info = WeaponSystem:get_station_info(3)
	local gun4Info = WeaponSystem:get_station_info(4)
	if gun3Info.weapon.level2== wsType_Shell then
		set_aircraft_draw_argument_value(502,1)	-- draws the right ammo box if the gun is mounted
	else 
		if gun4Info.weapon.level2== wsType_Shell then
			set_aircraft_draw_argument_value(502,1)	-- draws the right ammo box if the gun is mounted
		else
			set_aircraft_draw_argument_value(502,0)
		end
	end
	
	
	--syncSwitches()
end

local previousSwitchState = 0
function syncSwitches()
local syncTest=	get_aircraft_draw_argument_value(1000)
	if previousSwitchState~=syncTest then
		WeaponSystem:performClickableAction(device_commands.JettSwCover,syncTest,true)
	end
	previousSwitchState=syncTest
end


WeaponSystem:listen_command(Keys.PickleOn)
WeaponSystem:listen_command(Keys.PickleOff)
WeaponSystem:listen_command(Keys.TriggerFireOn)
WeaponSystem:listen_command(Keys.TriggerFireOff)
WeaponSystem:listen_command(Keys.MasterArm)
WeaponSystem:listen_command(device_commands.MasterArm)
WeaponSystem:listen_command(device_commands.SalvoSw)
WeaponSystem:listen_command(device_commands.JettSw)
WeaponSystem:listen_command(device_commands.RocketSelector)
WeaponSystem:listen_command(device_commands.GunSelector)
WeaponSystem:listen_command(device_commands.JettSwCover)

function SetCommand(command,value)
local syncTest=	get_aircraft_draw_argument_value(1000)
	if command == device_commands.JettSw then
		if value == 1 then
		WeaponSystem:emergency_jettison(0)
		WeaponSystem:emergency_jettison(4)		
		end
    elseif command == device_commands.MasterArm then
		master_arm = value
	elseif command == device_commands.SalvoSw then
		ripple = value
	elseif command == device_commands.RocketSelector then
		rocketSelect = value
	elseif command == device_commands.GunSelector then
		gunSelect = value
	elseif command == Keys.PickleOn then
        pickle_engaged = true
    elseif command == Keys.PickleOff then
        pickle_engaged = false 
		singleFired = 0
    elseif command == Keys.TriggerFireOn and master_arm == 1 and elec_dc_ok:get() == 1 then
		
		if gunSelect > 0 and gunSelect < 0.9 then
			local gunInfo = WeaponSystem:get_station_info(0)
			if gunInfo.weapon.level2 == wsType_Shell then -- prevent shooting rockets if those are mounted
				WeaponSystem:launch_station(0)
			end
			WeaponSystem:launch_station(1)
		end
		if gunSelect > 0.65 then
			local gunInfo = WeaponSystem:get_station_info(4)
			if gunInfo.weapon.level2 == wsType_Shell then -- prevent shooting rockets if those are mounted
				WeaponSystem:launch_station(4)
			end
			WeaponSystem:launch_station(3)
		end
    --elseif command == Keys.TriggerFireOff then
 
	elseif command == Keys.MasterArm then
        WeaponSystem:performClickableAction(device_commands.MasterArm,1-master_arm,true)

	elseif command == device_commands.JettSwCover then
		if value==1 and syncTest~=1 then
		set_aircraft_draw_argument_value(1000,1)
		elseif value==0 and syncTest~=0 then
		set_aircraft_draw_argument_value(1000,0)
		end
	--print_message_to_user(get_aircraft_draw_argument_value(1000))
	end
end

need_to_be_closed = false -- close lua state after initialization
--[[
available functions:
["get_station_info"] 
["set_ECM_status"] 
["get_ECM_status"]  
["launch_station"] 
["select_station"] 
["emergency_jettison"]  
["emergency_jettison_rack"] 
["set_target_range"]  
["set_target_span"]  
["get_target_range"]  
["get_target_span"]  
["get_flare_count"]  
["drop_flare"] 
["get_chaff_count"] 
["drop_chaff"] 

["listen_event"]  
["performClickableAction"] 
["SetDamage"] 
["listen_command"] 
["SetCommand"] 
--]]
