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

local MAG_HEADING = get_param_handle("MAG_HEADING")

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

local alt_setting = ALT_PRESSURE_STD
local DHI_test = 0
local AttIndSw = 0


-- ENGINE OUT / low rotor horn (MD 530F RFM 1-12, 3-3). The EFM sets ENG_OUT_WARN when N1 < 55% or rotor < 453 rpm. On the
-- late generic system the horn is disabled through the generator switch, so it only sounds with the generator on.
local ENG_OUT_WARN = get_param_handle("ENG_OUT_WARN")
local DC_BUS_VOLTAGE = get_param_handle("DC_Bus_Voltage")
local engOutHorn = nil
local function updateEngineOutHorn()
	if engOutHorn == nil then return end
	local genOn = get_cockpit_draw_argument_value(18) > 0.5
	if ENG_OUT_WARN:get() > 0.5 and genOn and DC_BUS_VOLTAGE:get() > 12 then
		engOutHorn:play_continue()
	else
		engOutHorn:stop()
	end
end

function post_initialize()

	local hornHost = create_sound_host("AH6J_HORN","HEADPHONES",-1.0,1.0,0)
	engOutHorn = hornHost:create_sound("AH6J_EngineWarning")

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
    if command == device_commands.DebugToggle then
		debugReadoutOn = not debugReadoutOn
		print_message_to_user("AH-6J debug readout " .. (debugReadoutOn and "ON (values also written to dcs.log, tag AH6J_DBG)" or "OFF"), 4)
	elseif command == device_commands.DebugDensityToggle then
		-- debug: switch the air density effect on the rotor/airframe off (old fixed density) or on, in flight
		debugDensityOff = (debugDensityOff == 1) and 0 or 1
		densityOffParam:set(debugDensityOff)
		print_message_to_user("AH-6J air density effect " .. (debugDensityOff == 1 and "OFF (fixed density, original behaviour)" or "ON (altitude and temperature change the rotor)"), 5)
	elseif command == device_commands.AltimeterSet then
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


-- Operating limit messages. The EFM (OperatingLimits.h) sets LIM_* parameters from the MD 530F Plus flight manual limits
-- (VNE placards, torque 48.9 / 59.6 psi, TOT 694 / 768 C, rotor rpm, N1 105%).
-- Set LIMIT_MESSAGES to false to turn the on-screen messages off.
local LIMIT_MESSAGES = true
local LIMIT_MESSAGE_REPEAT_S = 10
local limitChecks = {
	{ param = get_param_handle("LIM_VNE"),         text = "VNE EXCEEDED - reduce airspeed",            last = -1000 },
	{ param = get_param_handle("LIM_TOT_MAX"),     text = "TOT OVER 768 C",                            last = -1000 },
	{ param = get_param_handle("LIM_TORQUE_MAX"),  text = "TORQUE OVER 59.6 PSI",                      last = -1000 },
	{ param = get_param_handle("LIM_TORQUE_5MIN"), text = "TORQUE OVER CONTINUOUS LIMIT FOR 5 MIN",   last = -1000 },
	{ param = get_param_handle("LIM_NR_LOW"),      text = "LOW ROTOR RPM",                             last = -1000 },
	{ param = get_param_handle("LIM_NR_HIGH"),     text = "ROTOR RPM HIGH",                            last = -1000 },
	{ param = get_param_handle("LIM_TOT_CONT"),    text = "TOT above 694 C continuous limit",          last = -1000 },
	{ param = get_param_handle("LIM_TORQUE_CONT"), text = "Torque above continuous limit",             last = -1000 },
	{ param = get_param_handle("LIM_N1_HIGH"),     text = "N1 above 105%",                             last = -1000 },
}
-- Rotor brake misuse (MD 530F RFM 2-5: apply only after shutdown, rotor at or below 195 rpm). Set by the EFM.
local brakeWarn = get_param_handle("ROTOR_BRAKE_WARN")
local brakeWarnLast = -1000
function updateRotorBrakeMessage()
	-- debug aid only: the real aircraft has no such warnings, so show these only while the debug readout is on
	if not LIMIT_MESSAGES or not debugReadoutOn then return end
	local w = brakeWarn:get()
	if w < 0.5 then brakeWarnLast = -1000 return end
	local now = get_model_time()
	if now - brakeWarnLast >= 5 then
		brakeWarnLast = now
		if w > 5.5 then
			print_message_to_user("ROTOR BRAKE - release it now, last revolution (sudden stoppage damages the rotor)", 5)
		elseif w > 4.5 then
			print_message_to_user("STRAP PACK FAILURE - main rotor blade lost", 8)
		elseif w > 3.5 then
			print_message_to_user("COLLECTIVE UP WITH ROTOR BRAKE - strap pack damaged, lower the collective and release the brake", 5)
		elseif w > 2.5 then
			print_message_to_user("COLLECTIVE NOT FULL DOWN WITH ROTOR BRAKE - strap pack at risk, lower the collective", 5)
		elseif w > 1.5 then
			print_message_to_user("ROTOR BRAKE MISUSE - blades / strap packs damaged, release the brake", 5)
		else
			print_message_to_user("ROTOR BRAKE APPLIED ABOVE 195 RPM - release, apply only after shutdown", 5)
		end
	end
end
function updateLimitMessages()
	-- debug aid only: the real aircraft has gauges and a low-rotor horn, not text warnings, so show these only while the debug readout is on
	if not LIMIT_MESSAGES or not debugReadoutOn then return end
	local now = get_model_time()
	local shown = false
	for _, c in ipairs(limitChecks) do
		if c.param:get() > 0.5 then
			if not shown and now - c.last >= LIMIT_MESSAGE_REPEAT_S then
				print_message_to_user(c.text, 4)
				c.last = now
				shown = true -- only one message per update, most severe first
			end
		end
	end
end

-- Debug readout, toggled with LCtrl+LAlt+D (rebindable in Controls: "Debug Readout On/Off").
-- Shows the main flight model values on screen once a second and writes one line per second to dcs.log (tag AH6J_DBG)
-- so test flights can be compared afterwards. Values come from the EFM (DBG_*, LIM_*, ENGINE_* parameters).
debugReadoutOn = false
debugDensityOff = 0
densityOffParam = get_param_handle("DBG_DENSITY_OFF")	-- read by the EFM (LCtrl+LAlt+Y toggles it)
densityOffParam:set(0)
local DEBUG_PERIOD_S = 0.5
local debugLast = -1000
local lastPA, lastPAtime = nil, nil
local dbg = {}
for _, n in ipairs({"DBG_PA_FT","DBG_FAT_C","DBG_SIGMA","DBG_WEIGHT_LB","DBG_AGL_FT","DBG_COLLECTIVE","DBG_PEDAL","DBG_FUEL_FLOW",
	"DBG_DERATE","DBG_N1","DBG_TOT","DBG_PITCH","DBG_ROLL","DBG_CYC_PITCH","DBG_CYC_ROLL","DBG_DENSITY_ON","DBG_STICK_PITCH","DBG_STICK_ROLL","DBG_TRIM_PITCH","DBG_TRIM_ROLL",
	"ENGINE_TRQ","ENGINE_TRQ_CONT_LIMIT","EFM_N2_RPM","EFM_NR_RPM","LIM_IAS_KT","LIM_VNE_KT","DBG_M_HT","DBG_A_HT","DBG_A_F","DBG_M_F","ROTOR_BRAKE_WARN","ENG_OUT_WARN"}) do
	dbg[n] = get_param_handle(n)
end
function updateDebugReadout()
	if not debugReadoutOn then return end
	local now = get_model_time()
	if now - debugLast < DEBUG_PERIOD_S then return end
	debugLast = now
	local function v(n) return dbg[n]:get() or 0 end
	local nr_pct = v("EFM_NR_RPM")
	-- vertical speed from the change in pressure altitude since the last update [ft/min]
	local vs = 0
	if lastPA and now > lastPAtime then vs = (v("DBG_PA_FT") - lastPA) / (now - lastPAtime) * 60 end
	lastPA, lastPAtime = v("DBG_PA_FT"), now
	local text = string.format(
		"AH-6J DEBUG  (density effect %s)\nPA %5.0f ft  FAT %5.1f C  sigma %.3f  weight %4.0f lb  AGL %4.0f ft  VS %5.0f fpm\n" ..
		"Pitch %+5.1f  Roll %+5.1f deg   Cyclic pitch %+4.0f%%  roll %+4.0f%%  (stick %+4.0f/%+4.0f  trim %+4.0f/%+4.0f)   Coll %3.0f%%  Pedal %4.0f%%\n" ..
		"Torque %5.1f psi (cont limit %4.1f)  N1 %5.1f%%  N2 %5.1f%%  Nr %5.1f%% (%3.0f rpm)\n" ..
		"TOT %4.0f C  Fuel %4.0f lb/hr  Power available %3.0f%%  IAS %4.0f kt (VNE %4.0f)   Rotor brake %s (warn %d)",
		v("DBG_DENSITY_ON") > 0.5 and "ON" or "OFF",
		v("DBG_PA_FT"), v("DBG_FAT_C"), v("DBG_SIGMA"), v("DBG_WEIGHT_LB"), v("DBG_AGL_FT"), vs,
		v("DBG_PITCH"), v("DBG_ROLL"), v("DBG_CYC_PITCH"), v("DBG_CYC_ROLL"), v("DBG_STICK_PITCH"), v("DBG_STICK_ROLL"), v("DBG_TRIM_PITCH"), v("DBG_TRIM_ROLL"), v("DBG_COLLECTIVE"), v("DBG_PEDAL"),
		v("ENGINE_TRQ"), v("ENGINE_TRQ_CONT_LIMIT"), v("DBG_N1"), v("EFM_N2_RPM"), nr_pct, nr_pct * 4.75,
		v("DBG_TOT"), v("DBG_FUEL_FLOW"), v("DBG_DERATE"), v("LIM_IAS_KT"), v("LIM_VNE_KT"), get_cockpit_draw_argument_value(151) > 0.5 and "ON" or "off", v("ROTOR_BRAKE_WARN"))
	print_message_to_user(text, DEBUG_PERIOD_S + 0.5)
	if log and log.write then
		log.write("AH6J_DBG", log.INFO, string.format(
			"t=%.1f dens=%d pa_ft=%.0f fat_c=%.1f sigma=%.3f wt_lb=%.0f agl_ft=%.0f vs_fpm=%.0f pitch=%.1f roll=%.1f cyc_p=%.0f cyc_r=%.0f stick_p=%.0f stick_r=%.0f trim_p=%.0f trim_r=%.0f coll_pct=%.1f pedal_pct=%.1f trq_psi=%.1f trq_cont_psi=%.1f n1=%.1f n2=%.1f nr=%.1f tot_c=%.0f ff_lbhr=%.0f pwr_avail_pct=%.0f ias_kt=%.0f vne_kt=%.0f m_ht=%.0f a_ht=%.1f a_f=%.1f m_f=%.0f brake_warn=%d brake_arg=%.0f eng_out=%d gen_sw=%.0f",
			now, v("DBG_DENSITY_ON") > 0.5 and 1 or 0, v("DBG_PA_FT"), v("DBG_FAT_C"), v("DBG_SIGMA"), v("DBG_WEIGHT_LB"), v("DBG_AGL_FT"), vs,
			v("DBG_PITCH"), v("DBG_ROLL"), v("DBG_CYC_PITCH"), v("DBG_CYC_ROLL"), v("DBG_STICK_PITCH"), v("DBG_STICK_ROLL"), v("DBG_TRIM_PITCH"), v("DBG_TRIM_ROLL"), v("DBG_COLLECTIVE"), v("DBG_PEDAL"),
			v("ENGINE_TRQ"), v("ENGINE_TRQ_CONT_LIMIT"), v("DBG_N1"), v("EFM_N2_RPM"), nr_pct, v("DBG_TOT"), v("DBG_FUEL_FLOW"),
			v("DBG_DERATE"), v("LIM_IAS_KT"), v("LIM_VNE_KT"), v("DBG_M_HT"), v("DBG_A_HT"), v("DBG_A_F"), v("DBG_M_F"), v("ROTOR_BRAKE_WARN"), get_cockpit_draw_argument_value(151), v("ENG_OUT_WARN"), get_cockpit_draw_argument_value(18)))
	end
end

function update()
	update_altimeter()
	update_radar_altitude()
	updateAttInd()
	updateLimitMessages()
	updateRotorBrakeMessage()
	updateEngineOutHorn()
	updateDebugReadout()
	
	local maghdg = sensor_data.getMagneticHeading()*radian_to_degree
	if maghdg < 0 then maghdg = maghdg + 360 end
	if DHI_test>0 then
		DHI_HEADING:set(888)
	else
		DHI_HEADING:set(maghdg)
	end
	
	MAG_HEADING:set(maghdg)
		
	set_aircraft_draw_argument_value(38,0.9)-- to see if this affects ground crew
end

need_to_be_closed = false 