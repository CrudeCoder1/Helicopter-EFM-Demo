dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()

local update_time_step = 1 

if make_default_activity then
    make_default_activity(update_time_step)
end

function update()
end

function post_initialize()
end

function SetCommand(command,value)
    
end


need_to_be_closed = false -- close lua state after initialization

