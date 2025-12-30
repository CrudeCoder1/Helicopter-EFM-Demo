dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.common_script_path.."tools.lua")
	
MainPanel = {"ccMainPanel",LockOn_Options.script_path.."mainpanel_init.lua"}
	
creators = {}
creators[devices.ELECTRIC_SYSTEM]	= {"avSimpleElectricSystem"} -- needed for simpleRWR to work
creators[devices.WEAPON_SYSTEM]		= {"avSimpleWeaponSystem"	,LockOn_Options.script_path.."Systems/Weapons.lua"}
creators[devices.RWR]	 			= {"avSimpleRWR"			,LockOn_Options.script_path.."RWR/device/RWR_device.lua"}	
creators[devices.LIGHTING]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Lighting.lua"}
creators[devices.AVIONICS]    		= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/Avionics.lua"}
creators[devices.DIGITAL_CLOCK]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."M880A_digitalClock/M880A_device.lua"}
creators[devices.EFM_HELPER]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/EFM_Helper.lua"} 
creators[devices.INTERCOM]     		= {"avIntercom"             ,LockOn_Options.script_path.."Systems/Intercom.lua", {devices.UHF_RADIO} }
creators[devices.UHF_RADIO]     	= {"avUHF_ARC_164"          ,LockOn_Options.script_path.."Systems/UHF_radio.lua", {devices.INTERCOM, devices.ELECTRIC_SYSTEM} } 
creators[devices.VIDS]    			= {"avLuaDevice"            ,LockOn_Options.script_path.."VIDS/VIDS_device.lua"}
creators[devices.FUEL_SYSTEM]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/FuelSystem.lua"}
creators[devices.NVGs] 				= {"avNightVisionGoggles"}

indicators = {}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."FuelIndicator/init.lua",nil,{{"FG_PTR_CENTER",nil,nil}}}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."DigitalHeadingIndicator/init.lua",nil,{{"DHI_PTR_CENTER",nil,nil}}}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RadarAltitude/init.lua",nil,{{"RADALT_PTR_CENTER",nil,nil}}}
indicators[#indicators + 1] = {"ccIndicator",LockOn_Options.script_path.."VIDS/RightDisplay/init.lua", nil,{{"VIDS2_PTR_CENTER",nil,nil}}} 
indicators[#indicators + 1] = {"ccIndicator",LockOn_Options.script_path.."VIDS/LeftDisplay/init.lua", nil,{{"VIDS_PTR_CENTER",nil,nil}, {nil}}}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RWR/Indicator/init.lua",nil,{{"RWR_PTR_CENTER",nil,nil}}}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."M880A_digitalClock/init.lua",nil,{{"CLOCK_PTR_CENTER",nil,nil}}}
indicators[#indicators + 1] = {"ccControlsIndicatorBase", LockOn_Options.script_path.."ControlsIndicator/ControlsIndicator.lua", nil}


dofile(LockOn_Options.common_script_path.."KNEEBOARD/declare_kneeboard_device.lua")
