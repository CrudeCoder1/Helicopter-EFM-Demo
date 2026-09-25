dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")

update_time_step = 0.1
make_default_activity(update_time_step) 
local dev = GetSelf()
local mainPanelDev = GetDevice(0)
local sensor_data = get_base_data()
local Terrain = require('terrain')

-- AN/ARN-154(V) TACAN via F-3849 control panel

local tacanRange = get_param_handle("TACAN_RANGE")
tacanRange:set(0)
local tacanBearing = get_param_handle("TACAN_BEARING")
tacanBearing:set(0)
local TACAN_IDENT = get_param_handle("TACAN_IDENT")
local TACAN_TTG_Hour = get_param_handle("TACAN_TTG_Hour")
local TACAN_TTG_Min = get_param_handle("TACAN_TTG_Min")
local TACAN_TTG_Sec = get_param_handle("TACAN_TTG_Sec")

local channelOnes = 1
local channelTens = 0
local TACANchannel = 1
local TACANvolume = 0.5


local nm2meter = 1852
local mps_to_knot = 1.94384
local degrees_per_radian = 57.2957795

function post_initialize()
	
	local birth = LockOn_Options.init_conditions.birth_place	--"GROUND_COLD","GROUND_HOT","AIR_HOT"
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then   
	
    elseif birth=="GROUND_COLD" then
        	
    end
	
	dev:performClickableAction(device_commands.TACAN1s, 0.6)
	dev:performClickableAction(device_commands.TACAN10s, 0.05)
	dev:performClickableAction(device_commands.TACANPwrVol, 0.5)
	
	--ref file: DCS World OpenBeta\Mods\terrains\Caucasus\Beacons.lua
	fileName =  get_terrain_related_data("beacons") or get_terrain_related_data("beaconsFile")
	if fileName then 
		local f = loadfile(fileName)
		if f then
			f()
		end
	end
	
end


function SetCommand(command,value)
    if command == device_commands.TACAN1s then
		channelOnes = math.floor(value*10+0.1)
		TACANchannel = channelTens + channelOnes	
	elseif command == device_commands.TACAN10s then
		channelTens = math.floor(value*20+0.1)*10
		TACANchannel = channelTens + channelOnes
	elseif command == device_commands.TACANPwrVol then
		TACANvolume = value
	end	
end


function getTACAN_BearingRange()
    local selfx, selfy, selfz = sensor_data.getSelfCoordinates()
    
    for k,bcn in pairs(beacons) do        		
        if TACANchannel == bcn.channel or getTACANFrequency(TACANchannel, 'X') == bcn.frequency then	
            if bcn.type == BEACON_TYPE_TACAN or bcn.type == BEACON_TYPE_VORTAC then
			
				local hasLOS = Terrain.isVisible(selfx,selfy,selfz,bcn.position[1],bcn.position[2]+15,bcn.position[3])
				local beaconRange = math.sqrt((bcn.position[1] - selfx)^2 + (bcn.position[2] - selfy)^2 + (bcn.position[3] - selfz)^2)
				
                if beaconRange < 370000 and hasLOS then -- TACAN beacon range is about 370km(200nm)
					local magVariation = sensor_data.getMagneticHeading()*degrees_per_radian -(360-sensor_data.getHeading()*degrees_per_radian)
					local beaconBearing = math.deg(math.atan2((bcn.position[3]-selfz),(bcn.position[1]-selfx))) %360 + magVariation
					if beaconBearing < 0 then beaconBearing = beaconBearing + 360 end--fix wrap-around from magVar
					if beaconBearing > 360 then beaconBearing = beaconBearing - 360 end
					
                    return beaconBearing, beaconRange/nm2meter, bcn.callsign
				else
					return -1, -1, " "
                end
            end           
        end
    end

    return -1, -1, " "
end

local function calculateTTG(range, bearing)
	if range<0 then range=0 end
	local Vx, Vy, Vz = sensor_data.getSelfVelocity()--- DCS world axis: x is +north, y is +up, z is +east
	
	local groundDir = math.deg(math.atan2(Vz,Vx))
	if groundDir < 0 then
		groundDir = groundDir +360
	end
	
	local groundSpeed = math.sqrt((Vx^2)+(Vz^2))*mps_to_knot
	if groundSpeed<5 then 
		--groundSpeed = 5
		groundDir = bearing
		return 0,0,0
	end
			
	local directVelocity = groundSpeed*math.cos(math.rad(groundDir-bearing))	--speed toward nav mark 
	if directVelocity<0.1 then 
		directVelocity=0.1 
		return 0,0,0	
	end
	
	local hoursToGo = (range/directVelocity)		
    local HrInt,frac = math.modf(hoursToGo)
    local minutesTG = math.floor(frac*60)
	local int1,frac1 = math.modf(frac*60)
	local secondsTG = frac1*60
	
	HrInt = limit(HrInt, 0, 99)
	if hoursToGo>99 then
		HrInt = 99
		minutesTG = 0
		secondsTG = 0
	end

	return HrInt, minutesTG, secondsTG
end

function update()

	-- animate number dials
	mainPanelDev:set_argument_value(324,channelOnes/10)
	if channelTens >= 100 then
		mainPanelDev:set_argument_value(326,0.1)
		mainPanelDev:set_argument_value(325,(channelTens-100)/100)
	else
		mainPanelDev:set_argument_value(326,0)
		mainPanelDev:set_argument_value(325,channelTens/100)
	end

	if TACANvolume>0.1 then -- must be ON
		local TCNbearing, TCNrange, ID = getTACAN_BearingRange()
		TACAN_IDENT:set(ID)
		tacanRange:set(TCNrange)
		tacanBearing:set(TCNbearing)
	else
		TACAN_IDENT:set(" ")
		tacanRange:set(-1)
		tacanBearing:set(-1)
	end

	local TCNHour, TCNMin, TCNSec = calculateTTG(tacanRange:get(), tacanBearing:get())
	TACAN_TTG_Hour:set(TCNHour)
	TACAN_TTG_Min:set(TCNMin)
	TACAN_TTG_Sec:set(TCNSec)

end

need_to_be_closed = false