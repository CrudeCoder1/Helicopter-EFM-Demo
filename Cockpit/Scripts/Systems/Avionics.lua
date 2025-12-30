dofile(LockOn_Options.script_path.."command_defs.lua")
dofile(LockOn_Options.script_path.."utilityFunctions.lua")

local dev = GetSelf()
local mainPanelDev = GetDevice(0)
local sensor_data = get_base_data()
local update_time_step = 0.05  
make_default_activity(update_time_step)

DCbusVoltage  = get_param_handle("DC_Bus_Voltage")

local meters_to_feet = 3.2808399
local feet_per_meter_per_minute = meters_to_feet * 60
local radian_to_degree = 57.2957795
local KG_TO_POUNDS = 2.20462
local InHg_to_Pa = 3386.39


local ALT_PRESSURE_MAX = 31.00 -- in Hg
local ALT_PRESSURE_MIN = 28.10 -- in Hg
local ALT_PRESSURE_STD = 29.92 -- in Hg
local alt_setting = ALT_PRESSURE_STD

local COMPASS_HDG = get_param_handle("COMPASS_HDG")

local alt_10k = get_param_handle("ALT_10000") -- 0 to 100,000
local alt_1k = get_param_handle("ALT_1000") -- 0 to 10,000
local alt_100s = get_param_handle("ALT_100") -- 0 to 1000
local alt_adj_Nxxx = get_param_handle("ALT_ADJ_Nxxx") -- 1st digit, 0-10 is input
local alt_adj_xNxx = get_param_handle("ALT_ADJ_xNxx") -- 2nd digit, 0-10 input
local alt_adj_xxNx = get_param_handle("ALT_ADJ_xxNx") -- 3rd digit, 0-10 input
local alt_adj_xxxN = get_param_handle("ALT_ADJ_xxxN") -- 4th digit, 0-10 input
local alt_adj_MBNxxx = get_param_handle("ALT_ADJ_MBNxxx") -- 
local alt_adj_MBxNxx = get_param_handle("ALT_ADJ_MBxNxx") -- 
local alt_adj_MBxxNx = get_param_handle("ALT_ADJ_MBxxNx") -- 
local alt_adj_MBxxxN = get_param_handle("ALT_ADJ_MBxxxN") -- 
alt_10k:set(0.0)
alt_1k:set(0.0)
alt_100s:set(0.0)

local current_Ralt = get_param_handle("CURRENT_RALT")
local RADALT_BRIGHTNESS = get_param_handle("RADALT_BRIGHTNESS")
current_Ralt:set(0)


local DHI_HEADING = get_param_handle("DHI_HEADING")
DHI_HEADING:set(0)
local DHI_BRIGHTNESS  = get_param_handle("DHI_BRIGHTNESS")
local current_fuelT  = get_param_handle("CURRENT_FUELT")
current_fuelT:set(0)

local alt_setting = ALT_PRESSURE_STD
local DHI_test = 0
local AttIndSw = 0


function post_initialize()
	current_fuelT:set(sensor_data.getTotalFuelWeight()*KG_TO_POUNDS)
	DHI_HEADING:set(360-(sensor_data.getHeading()*radian_to_degree))
	COMPASS_HDG:set(360-(sensor_data.getHeading()*radian_to_degree))
	update_altimeter()
	update()

    local birth = LockOn_Options.init_conditions.birth_place	
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 			  
        local self_x,self_y,self_z = sensor_data.getSelfCoordinates()
		local temp, press = require('Weather').getTemperatureAndPressureAtPoint({position = {x = self_x, y = 0, z = self_z}})
		alt_setting = press/InHg_to_Pa
		
		dev:performClickableAction(device_commands.AttIndPwrSw, 1)
    elseif birth=="GROUND_COLD" then
        		
    end
	dev:performClickableAction(device_commands.DHIbrightness, 1)
	dev:performClickableAction(device_commands.RadAltBrightness, 1)
	dev:performClickableAction(device_commands.LOset, 0.167)
	dev:performClickableAction(device_commands.HIset, 0.87)
end


function SetCommand(command,value)   
    if command == device_commands.AltimeterSet then
		alt_setting = alt_setting + value/10
		if alt_setting > ALT_PRESSURE_MAX then
			alt_setting = ALT_PRESSURE_MAX
		elseif alt_setting < ALT_PRESSURE_MIN then
			alt_setting = ALT_PRESSURE_MIN
		end
	elseif command == device_commands.DHItest then
		DHI_test = value
	elseif command == device_commands.DHIbrightness then
		DHI_BRIGHTNESS:set(value)
	elseif command == device_commands.RadAltBrightness then
		RADALT_BRIGHTNESS:set(value)
	elseif command == device_commands.AttIndPwrSw then
		AttIndSw = value
	end
end


function update_altimeter()
	local self_x,self_y,self_z = sensor_data.getSelfCoordinates()
	local temp, press = require('Weather').getTemperatureAndPressureAtPoint({position = {x = self_x, y = self_y, z = self_z}})
	local adjustedBaroAlt = 145366.45*(1-math.pow(press/(alt_setting*InHg_to_Pa),0.190284))-- pressure altitude [ft] with altimeter adjustment

	alt_adj_Nxxx:set(alt_setting/10)
	alt_adj_xNxx:set(alt_setting % 10)
	alt_adj_xxNx:set((alt_setting*10) % 10)
	alt_adj_xxxN:set((alt_setting*100) % 10)
	
	local settingMB = alt_setting * 33.8638867 -- convert inches of mercury to millibars
    local MBNxxx = math.floor(settingMB/1000)         
	local MBxNxx = math.floor(settingMB/100) % 10         
    local MBxxNx = math.floor(settingMB/10) % 10
    local MBxxxN = math.floor(settingMB) % 10
    alt_adj_MBNxxx:set(MBNxxx)
	alt_adj_MBxNxx:set(MBxNxx)
    alt_adj_MBxxNx:set(MBxxNx)
    alt_adj_MBxxxN:set(MBxxxN)

    alt_10k:set(adjustedBaroAlt % 100000)
    alt_1k:set(adjustedBaroAlt  % 10000)
    alt_100s:set(adjustedBaroAlt % 1000)
end

local RALT_Off = false
local LO_On = false
local HI_On = false
function update_radar_altitude()
	local pitch = (sensor_data.getPitch()*radian_to_degree)
	local roll = (sensor_data.getRoll()*radian_to_degree)
	if (pitch < 35 and pitch > -35) and (roll < 45 and roll > -45) then -- limits radar alt to only work when the radar is pointing the ground
		current_Ralt:set(sensor_data.getRadarAltitude()*meters_to_feet-5)--gives alt from center of model, so removing 6ft to account for this height
		RALT_Off = false
		if current_Ralt:get()>1500 then -- max altitude is 1500ft
			current_Ralt:set(0)
			RALT_Off = true
		end
	else
		current_Ralt:set(0)
		RALT_Off = true
	end
	
	if DCbusVoltage:get() <= 18 then
		RALT_Off = true
	end
	
	mainPanelDev:set_argument_value(323, RALT_Off and 1 or 0)
	mainPanelDev:set_argument_value(445, RADALT_BRIGHTNESS:get())
	
	local LO_setting = get_cockpit_draw_argument_value(43)
	if LO_setting<0.667 then
		if current_Ralt:get()<LO_setting*(200/0.667) then
			LO_On = true
		else
			LO_On = false
		end
	else 
		if current_Ralt:get()< LinInterp(LO_setting, 0.667, 1, 200, 1500) then
			LO_On = true
		else
			LO_On = false
		end
	end
	mainPanelDev:set_argument_value(446, LO_On and 1 or 0)
	
	local HI_setting = get_cockpit_draw_argument_value(44)
	if HI_setting<0.667 then
		if current_Ralt:get()>HI_setting*(200/0.667) then
			HI_On = true
		else
			HI_On = false
		end
	else 
		if current_Ralt:get()> LinInterp(HI_setting, 0.667, 1, 200, 1500) then
			HI_On = true
		else
			HI_On = false
		end
	end
	mainPanelDev:set_argument_value(447, HI_On and 1 or 0)

end

local AttInd_Pitch = get_param_handle("AttInd_Pitch")
local AttInd_Roll = get_param_handle("AttInd_Roll")
function updateAttInd()
	if AttIndSw==1 and DCbusVoltage:get() >= 21 then
		AttInd_Pitch:set(sensor_data.getPitch())
		AttInd_Roll:set(sensor_data.getRoll())
	else
		AttInd_Pitch:set(0)
		AttInd_Roll:set(0)
	end
end


function update()
	update_altimeter()
	update_radar_altitude()
	updateAttInd()
	current_fuelT:set(sensor_data.getTotalFuelWeight()*KG_TO_POUNDS)
	
	if DHI_test>0 then
		DHI_HEADING:set(888)
	else
		DHI_HEADING:set(360-(sensor_data.getHeading()*radian_to_degree))
	end
	
	COMPASS_HDG:set(360-(sensor_data.getHeading()*radian_to_degree))
		
	set_aircraft_draw_argument_value(38,0.9)-- to see if this affects ground crew
end

need_to_be_closed = false 