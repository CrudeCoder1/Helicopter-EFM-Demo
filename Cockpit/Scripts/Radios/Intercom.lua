dofile(LockOn_Options.script_path.."devices.lua")

local gettext = require("i_18n")
_ = gettext.translate


DeviceNames = {-- Device names can be any name but needs to match here
	radio1_device = devices.RADIO_1,
    radio2_device = devices.RADIO_2,
    --radio3_device = devices.RADIO_3,
    --radio4_device = devices.RADIO_4,
}

need_to_be_closed = true



--[[
available functions:

set_master_volume() 0-1
set_receiver1_Volume() 0-1
set_receiver2_Volume() 0-1
set_receiver3_Volume() 0-1
set_receiver4_Volume() 0-1
set_mode() 0:ICS only, 1: receiver 1, 2: receiver 2, 3: receiver 3, 4: receiver 4

]]