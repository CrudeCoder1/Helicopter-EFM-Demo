dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local sensor_data = get_base_data()
local update_time_step = 0.05  
make_default_activity(update_time_step)

local meters_to_feet = 3.2808399
local feet_per_meter_per_minute = meters_to_feet * 60
local radian_to_degree = 57.2957795
local KG_TO_POUNDS = 2.20462


local ALT_PRESSURE_MAX = 30.99 -- in Hg
local ALT_PRESSURE_MIN = 29.10 -- in Hg
local ALT_PRESSURE_STD = 29.92 -- in Hg

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
local current_Ralt = get_param_handle("CURRENT_RALT")
local Ralt_Off = get_param_handle("RALT_OFF")
local current_hdg = get_param_handle("CURRENT_HDG")
local current_fuelT  = get_param_handle("CURRENT_FUELT")


local alt_setting = ALT_PRESSURE_STD

alt_10k:set(0.0)
alt_1k:set(0.0)
alt_100s:set(0.0)
current_Ralt:set(0)
Ralt_Off:set(0)
current_hdg:set(0)
current_fuelT:set(0)


function post_initialize()
	current_fuelT:set(sensor_data.getTotalFuelWeight()*KG_TO_POUNDS)
	current_hdg:set(360-(sensor_data.getHeading()*radian_to_degree))
	update_altimeter()
	local dev = GetSelf()
    local birth = LockOn_Options.init_conditions.birth_place	
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then 			  
        		
    elseif birth=="GROUND_COLD" then
        		
    end
end

dev:listen_command(device_commands.AltimeterSet)
function SetCommand(command,value)   
    if command == device_commands.AltimeterSet then
		alt_setting = alt_setting + value/10
		if alt_setting > ALT_PRESSURE_MAX then
			alt_setting = ALT_PRESSURE_MAX
		elseif alt_setting < ALT_PRESSURE_MIN then
			alt_setting = ALT_PRESSURE_MIN
		end
	end
end


function update_altimeter()
    local alt = sensor_data.getBarometricAltitude()*meters_to_feet
	 -- first update the selected setting value displayed
    local altNxxx = math.floor(alt_setting/10)         
	local altxNxx = math.floor(alt_setting) % 10         
    local altxxNx = math.floor(alt_setting*10) % 10
    local altxxxN = math.floor(alt_setting*100) % 10
    alt_adj_Nxxx:set(altNxxx)
	alt_adj_xNxx:set(altxNxx)
    alt_adj_xxNx:set(altxxNx)
    alt_adj_xxxN:set(altxxxN)

	local settingMB = alt_setting * 33.8638867 -- convert inches of mercury to millibars
    local MBNxxx = math.floor(settingMB/1000)         
	local MBxNxx = math.floor(settingMB/100) % 10         
    local MBxxNx = math.floor(settingMB/10) % 10
    local MBxxxN = math.floor(settingMB) % 10
    alt_adj_MBNxxx:set(MBNxxx)
	alt_adj_MBxNxx:set(MBxNxx)
    alt_adj_MBxxNx:set(MBxxNx)
    alt_adj_MBxxxN:set(MBxxxN)
	
    -- based on setting, adjust displayed altitude
    local alt_adj = (alt_setting - ALT_PRESSURE_STD)*1000   -- 1000 feet per inHg / 10 feet per .01 inHg -- if we set higher pressure than actual => altimeter reads higher

    alt_10k:set((alt+alt_adj) % 100000)
    alt_1k:set((alt+alt_adj)  % 10000)
    alt_100s:set((alt+alt_adj) % 1000)
end

function update_radar_altitude()
	local pitch = (sensor_data.getPitch()*radian_to_degree)
	local roll = (sensor_data.getRoll()*radian_to_degree)
	if (pitch < 35 and pitch > -35) and (roll < 45 and roll > -45) then -- limits radar alt to only work when the radar is pointing the ground
		current_Ralt:set(sensor_data.getRadarAltitude()*meters_to_feet-6)--gives alt from center of model, so removing 6ft to account for this height
		Ralt_Off:set(0)	
		if current_Ralt:get()>1500 then -- max altitude is 1500ft
			current_Ralt:set(0)
			Ralt_Off:set(1)
		end
	else
		current_Ralt:set(0)
		Ralt_Off:set(1)
	end
end


function update()
	update_altimeter()
	update_radar_altitude()
	current_fuelT:set(sensor_data.getTotalFuelWeight()*KG_TO_POUNDS)
	current_hdg:set(360-(sensor_data.getHeading()*radian_to_degree))

	
	set_aircraft_draw_argument_value(38,0.9)-- to see if this affects ground crew
end

need_to_be_closed = false 