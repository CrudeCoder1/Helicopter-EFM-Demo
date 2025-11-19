dofile(LockOn_Options.common_script_path..'wsTypes_SAM.lua')
dofile(LockOn_Options.common_script_path..'wsTypes_Airplane.lua')
dofile(LockOn_Options.common_script_path..'wsTypes_Ship.lua')
dofile(LockOn_Options.common_script_path..'wsTypes_Missile.lua')
dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")

update_time_step = 0.03
make_default_activity(update_time_step) 
local dev = GetSelf()

--render_debug_info = true

rwrPower = get_param_handle("RWR_POWER")
rwrBrightness = get_param_handle("RWR_BRIGHTNESS")
elec_dc_ok  = get_param_handle("DC_POWER_AVAIL")
rwrPower:set(0)
rwrBrightness:set(1)

-----------------------------------------------------------------------------
MaxThreats          = 10
EmitterLiveTime     = 7.0
EmitterSoundTime    = 0.5
LaunchSoundDelay    = 15.0
DefaultType          = 100
RWR_detection_coeff = 0.85

eyes ={}	-- sensor locations
eyes[1] =
	{
		position      = {x = 1.479,y = -0.472,z =  0.674}, --{Forward/Back,U/D,L/R}
		orientation   = {azimuth  = math.rad(45),elevation = math.rad(0.0)},
		field_of_view = math.rad(120) 
	}
eyes[2] =
	{
		position      = {x = 1.479,y = -0.472,z = -0.674},
		orientation   = {azimuth  = math.rad(-45),elevation = math.rad(0.0)},
		field_of_view = math.rad(120) 
	}
eyes[3] =
	{
		position      = {x = -1.196,y = 0.519,z =  0.21},
		orientation   = {azimuth  = math.rad(135),elevation = math.rad(0.0)},
		field_of_view = math.rad(120) 
	}
eyes[4] =
	{
		position      = {x = -1.196,y = 0.519,z =  -0.21},
		orientation   = {azimuth  = math.rad(-135),elevation = math.rad(0.0)},
		field_of_view = math.rad(120) 
	}

rwr = {}
for i = 1,10 do	
	rwr[i] = {
				-- from CockpitBase.dll
				elevation 	= get_param_handle("RWR_CONTACT_0" .. i .. "_ELEVATION"),
				time		= get_param_handle("RWR_CONTACT_0" .. i .. "_TIME"),
				source		= get_param_handle("RWR_CONTACT_0" .. i .. "_SOURCE"),
				priority	= get_param_handle("RWR_CONTACT_0" .. i .. "_PRIORITY"),
				unit_type	= get_param_handle("RWR_CONTACT_0" .. i .. "_UNIT_TYPE"), -- f-16=16, su-27=27 etc				
				general_type= get_param_handle("RWR_CONTACT_0" .. i .. "_GENERAL_TYPE"), -- 1:plane, 2:SAM+AAA radar, 3:ship
				signal		= get_param_handle("RWR_CONTACT_0" .. i .. "_SIGNAL"), -- 1:search, 2:lock, 3: launch
				azimuth 	= get_param_handle("RWR_CONTACT_0" .. i .. "_AZIMUTH"),
				power 		= get_param_handle("RWR_CONTACT_0" .. i .. "_POWER"), -- 0-1
					
				-- custom
				unit_type_sym	= get_param_handle("RWR_CONTACT_0" .. i .. "_UNIT_TYPE_SYM"),
				adjusted_power 	= get_param_handle("RWR_CONTACT_0" .. i .. "_POWER_ADJUSTED"),
				lock_sym 		= get_param_handle("RWR_CONTACT_0" .. i .. "_LOCK_SYM"),
			 }
end


function post_initialize()
	--GetDevice(devices.RWR):set_power(true)	
	dev:performClickableAction(device_commands.RWRBrightness,1,true)
	local birth = LockOn_Options.init_conditions.birth_place	--"GROUND_COLD","GROUND_HOT","AIR_HOT"
    if birth=="GROUND_HOT" or birth=="AIR_HOT" then   
        dev:performClickableAction(device_commands.RWRpower,1,true) --true or false do action	 	
    elseif birth=="GROUND_COLD" then
        dev:performClickableAction(device_commands.RWRpower,0,true)	
    end
end

dev:listen_command(device_commands.RWRpower)
dev:listen_command(device_commands.RWRBrightness)

function SetCommand(command,value)
    if command == device_commands.RWRpower then
		RWR_Power = (value==1) and true or false
		GetDevice(devices.RWR):set_power(RWR_Power) -- actives the RWR script internally	
	elseif command == device_commands.RWRBrightness then
		rwrBrightness:set(value)
	end
end


function update()
local tmp_type
	for i = 1,10 do
		if rwr[i].signal:get() == 2 then				-- 2 Lock
			rwr[i].lock_sym:set(1)
		elseif rwr[i].signal:get() == 3 then			-- 3 Launch
			rwr[i].lock_sym:set(1-rwr[i].lock_sym:get()) --flashes symbol
		else	-- 1 Search or 0 nothing
			rwr[i].lock_sym:set(0)
		end

		rwr[i].adjusted_power:set(1-rwr[i].power:get() + 0.1)
		
		tmp_type = rwr[i].unit_type:get()
		tmp_type = string.gsub(tmp_type,'%W','') -- filters unit name down to only numbers so they can be easily used
		tmp_type = string.gsub(tmp_type,'%a','')
		if rwr[i].general_type:get() == 2 then -- SAM or AAA
			if tonumber(tmp_type) == 9331 or tonumber(tmp_type)== 933 then -- short range, sa-15 sa-8
				rwr[i].unit_type_sym:set("a8".."a") -- for some reason numbers must be concatenated with a letter or number to work. lowercase letters dont appear with font_RWR
			elseif tonumber(tmp_type) == 191 or tonumber(tmp_type) == 119181 or tonumber(tmp_type) == 1193101 then -- medium range, sa-11 sa-6
				rwr[i].unit_type_sym:set("a6".."a")
			--elseif tonumber(tmp_type) == 1 then -- long range, sa-10
			--	rwr[i].unit_type_sym:set("a4".."a")
			elseif tonumber(tmp_type) == 26 then -- gun, sa-19(gun+IR)
				rwr[i].unit_type_sym:set("aA".."a") --TODO: change x to triangle
			else
				rwr[i].unit_type_sym:set("U")
			end
		elseif rwr[i].general_type:get() == 3 then -- ships
			rwr[i].unit_type_sym:set("U")
		end
		
		
		if rwr[i].general_type:get() > 0 then
			--print_message_to_user(rwr[i].source:get()) --debug use
		end
		
	end

	if elec_dc_ok:get()==1 and RWR_Power then
		rwrPower:set(1) -- used for the indicator/display
	else
		rwrPower:set(0)
	end
end

--[[
		RWR_CONTACT_01_ELEVATION:-0.000844
		RWR_CONTACT_01_SOURCE:16778240		16777472
		RWR_CONTACT_01_TIME:3.200000
		RWR_CONTACT_01_UNIT_TYPE:"MiG-29S"
		RWR_CONTACT_01_GENERAL_TYPE:1.000000
		RWR_CONTACT_01_PRIORITY:160.910660
		RWR_CONTACT_01_AZIMUTH:1.671934
		RWR_CONTACT_01_SIGNAL:1.000000
		RWR_CONTACT_01_POWER:0.910663
]]--