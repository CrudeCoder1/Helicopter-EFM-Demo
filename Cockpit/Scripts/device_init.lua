dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.common_script_path.."tools.lua")
	
MainPanel = {"ccMainPanel",LockOn_Options.script_path.."mainpanel_init.lua"}
	
creators = {}
creators[devices.ELECTRIC_SYSTEM]	= {"avSimpleElectricSystem"} -- needed for simpleRWR to work
creators[devices.WEAPON_SYSTEM]		= {"avSimpleWeaponSystem"	,LockOn_Options.script_path.."Systems/Weapons.lua"}
creators[devices.RWR]	 			= {"avSimpleRWR"			,LockOn_Options.script_path.."RWR/device/RWR_device.lua"}	
creators[devices.EXTLIGHTS]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/ExternalLights.lua"}
creators[devices.AVIONICS]    		= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/Avionics.lua"}
creators[devices.DIGITAL_CLOCK]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."M880A_digitalClock/M880A_device.lua"}
creators[devices.EFM_HELPER]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/EFM_Helper.lua"} 
creators[devices.INTERCOM]     		= {"avIntercom"             ,LockOn_Options.script_path.."Systems/Intercom.lua", {devices.UHF_RADIO} }
creators[devices.UHF_RADIO]     	= {"avUHF_ARC_164"          ,LockOn_Options.script_path.."Systems/UHF_radio.lua", {devices.INTERCOM, devices.ELECTRIC_SYSTEM} } 
creators[devices.HELMET_DEVICE] 	= {"avNightVisionGoggles"}

indicators = {}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."FuelIndicator/init.lua",nil}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."BearingIndicator/init.lua",nil}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RadarAltitude/init.lua",nil}
indicators[#indicators + 1] = {"ccIndicator",LockOn_Options.script_path.."RPM_Display/init.lua", nil,--id of parent device
  {{"ENG_PNT_CENTER",nil,nil}, {nil}}
} 
indicators[#indicators + 1] = {"ccIndicator",LockOn_Options.script_path.."tempTorqDisplay/init.lua", nil,--id of parent device
  {{"ENG_PNT2_CENTER",nil,nil}, {nil}}
}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RWR/Indicator/init.lua",nil}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."M880A_digitalClock/init.lua",nil}
indicators[#indicators + 1] = {"ccControlsIndicatorBase", LockOn_Options.script_path.."ControlsIndicator/ControlsIndicator.lua", nil}


dofile(LockOn_Options.common_script_path.."KNEEBOARD/declare_kneeboard_device.lua")
