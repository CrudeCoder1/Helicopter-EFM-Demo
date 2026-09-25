dofile(LockOn_Options.common_script_path..'Radio.lua')
dofile(LockOn_Options.common_script_path.."mission_prepare.lua")

local gettext = require("i_18n")
_ = gettext.translate


GUI = {
	range = {min = 108E6, max = 399.975E6, step = 25E3}, --Hz
	displayName = _('V/UHF Radio AN/ARC-182'),
	AM = true,
	FM = true,
}

need_to_be_closed = true


--[[
available functions:

set_frequency() in hz
get_frequency() returns freq in hz
set_modulation() AM:0, FM:1
is_frequency_in_range() returns boolean
set_squelch() boolean
set_volume() 0-1
set_on() boolean

use in another device with: 
local radioDevice = GetDevice(devices.RADIO_1)
radioDevice:set_frequency(freq_in_hz)

]]