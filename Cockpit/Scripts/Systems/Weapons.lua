dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.common_script_path.."devices_defs.lua")
dofile(LockOn_Options.common_script_path.."../../../Database/wsTypes.lua")

local dev = GetSelf()
local update_rate = 0.06 -- 60ms fire rate between rockets
make_default_activity(update_rate)
local mainPanelDev = GetDevice(0)


DCbusVoltage  = get_param_handle("DC_Bus_Voltage")

local FireRocket = false
local triggerOn = false
local PwrSwOn = false
local rippleOn = false
local PairOn = true
local rocketSelect = 0
local AMSbuttonBrt = 1
local LeftGunPower = false
local LeftGunArmed = false
local RightGunPower = false
local RightGunArmed = false

local isArmed = false
local AMS_hasPower = false
local L_StationReady = false
local R_StationReady = false

local L_OB_Station = 0
local L_IB_Station = 1
local R_IB_Station = 3
local R_OB_Station = 4

function post_initialize()	
	dev:performClickableAction(device_commands.AMSbuttonBrght,1)
    dev:performClickableAction(device_commands.RippleSw,0)
	
    local birth = LockOn_Options.init_conditions.birth_place	--"GROUND_COLD","GROUND_HOT","AIR_HOT"
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 	 
		dev:performClickableAction(device_commands.AMSPwrSw,1)
        dev:performClickableAction(device_commands.MasterArm,1)
		dev:performClickableAction(device_commands.LftGunPwr,1)
		dev:performClickableAction(device_commands.RhtGunPwr,1)
    elseif birth=="GROUND_COLD" then
		dev:performClickableAction(device_commands.AMSPwrSw,0)
        dev:performClickableAction(device_commands.MasterArm,0)
		dev:performClickableAction(device_commands.LftGunPwr,0)
		dev:performClickableAction(device_commands.RhtGunPwr,0)
    end
end

local singleFired = false
local salvoLeft = true

function updateGuns()
	if isArmed and AMS_hasPower and triggerOn then
		if LeftGunPower and LeftGunArmed then
				local gunInfo = dev:get_station_info(L_OB_Station)
				if gunInfo.weapon.level2 == wsType_Shell then -- prevent shooting rockets if those are mounted
					dev:launch_station(L_OB_Station)
				end
				dev:launch_station(L_IB_Station)
			end
		if RightGunPower and RightGunArmed then
			local gunInfo = dev:get_station_info(R_OB_Station)
			if gunInfo.weapon.level2 == wsType_Shell then -- prevent shooting rockets if those are mounted
				dev:launch_station(R_OB_Station)
			end
			dev:launch_station(R_IB_Station)
		end
	end
	
	-- animation controls
	local gun0Info = dev:get_station_info(L_OB_Station).weapon.level2
	local gun1Info = dev:get_station_info(L_IB_Station).weapon.level2
	if gun1Info== wsType_Shell or gun0Info== wsType_Shell then
		set_aircraft_draw_argument_value(1001,1)	-- draws the left ammo box if the gun is mounted
	else 
		set_aircraft_draw_argument_value(1001,0)
	end
	
	local gun3Info = dev:get_station_info(R_IB_Station).weapon.level2
	local gun4Info = dev:get_station_info(R_OB_Station).weapon.level2
	if gun3Info== wsType_Shell or gun4Info== wsType_Shell then
		set_aircraft_draw_argument_value(1002,1)	-- draws the right ammo box if the gun is mounted
	else 		
		set_aircraft_draw_argument_value(1002,0)	
	end
end

function updateRockets()
	local L_isRkt = dev:get_station_info(L_OB_Station).weapon.level2==wsType_NURS
	local R_isRkt = dev:get_station_info(R_OB_Station).weapon.level2==wsType_NURS

	if isArmed and AMS_hasPower and FireRocket then
		if PairOn then
			if rippleOn then
				if L_isRkt and L_StationReady then
					dev:launch_station(L_OB_Station)
				end
				if R_isRkt and R_StationReady then
					dev:launch_station(R_OB_Station)
				end
			elseif not singleFired then
				if L_isRkt and L_StationReady then
					dev:launch_station(L_OB_Station)
				end
				if R_isRkt and R_StationReady then
					dev:launch_station(R_OB_Station)
				end
				singleFired = true
			end
		else
			if rippleOn then				
				if L_isRkt and L_StationReady and salvoLeft then
					dev:launch_station(L_OB_Station)
				end
				if R_isRkt and R_StationReady and not salvoLeft then
					dev:launch_station(R_OB_Station)
				end
				salvoLeft = not salvoLeft
			elseif not singleFired then
				if L_isRkt and L_StationReady and salvoLeft then
					dev:launch_station(L_OB_Station)
				end
				if R_isRkt and R_StationReady and not salvoLeft then
					dev:launch_station(R_OB_Station)
				end
				singleFired = true
			end
		end
	end
	
end

function update() 

	AMS_hasPower = PwrSwOn and DCbusVoltage:get() >= 20

	if isArmed and AMS_hasPower then		
		L_StationReady = rocketSelect<=0
		R_StationReady = rocketSelect>=0
	else
		L_StationReady=false
		R_StationReady=false
	end
	
	mainPanelDev:set_argument_value(430, AMSbuttonBrt)
	mainPanelDev:set_argument_value(431, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(439, L_StationReady and 1 or 0)
	mainPanelDev:set_argument_value(440, R_StationReady and 1 or 0)
	-- todo zone control
	mainPanelDev:set_argument_value(432, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(433, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(434, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(435, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(436, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(437, AMS_hasPower and 1 or 0)
	mainPanelDev:set_argument_value(438, AMS_hasPower and 1 or 0)
	
	updateGuns()
	updateRockets()

end


dev:listen_command(Keys.MasterArmToggle)
function SetCommand(command,value)
	if command == device_commands.JettSw then
		if value == 1 then -- direct battery connection
			dev:emergency_jettison(0)
			dev:emergency_jettison(4)		
		end
	elseif command == device_commands.AMSPwrSw then	
		PwrSwOn = value == 1
	elseif command == device_commands.AMSbuttonBrght then		
		AMSbuttonBrt = value
    elseif command == device_commands.MasterArm then
		isArmed = value == 1
	elseif command == device_commands.RippleSw then
		rippleOn = value == 1
	elseif command == device_commands.PairSw then
		PairOn = value == 0
	elseif command == device_commands.RocketSelector then
		rocketSelect = value		
	elseif command == device_commands.LftGunPwr then
		LeftGunPower = value > 0.0
	elseif command == device_commands.LftGunArm then
		LeftGunArmed = value > 0.0
	elseif command == device_commands.RhtGunPwr then
		RightGunPower = value > 0.0
	elseif command == device_commands.RhtGunArm then
		RightGunArmed = value > 0.0	
	elseif command == device_commands.RocketFireButton then
		FireRocket = value==1
        if value==0 then
			singleFired = false
			salvoLeft = not salvoLeft
		end 
    elseif command == device_commands.GunTrigger then
		triggerOn = value > 0		
	elseif command == Keys.MasterArmToggle then
        dev:performClickableAction(device_commands.MasterArm,not isArmed)
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
