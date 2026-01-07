dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.script_path.."command_defs.lua")

package.cpath             = package.cpath..";".. LockOn_Options.script_path.. "..\\..\\bin\\?.dll"
require('avSimplestRadio')

local dev = GetSelf()

function post_initialize()
    avSimplestRadio.SetupRadios(devices.ELECTRIC_SYSTEM, devices.INTERCOM, 1, devices.UHF_RADIO)
end

dev:listen_command(Keys.COM1)

function SetCommand(command,value)
    if command==Keys.COM1 and value == 1 then
        avSimplestRadio.PTT(1)
    end
end

need_to_be_closed = false