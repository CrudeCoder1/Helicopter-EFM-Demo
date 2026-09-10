dofile(LockOn_Options.common_script_path.."devices_defs.lua")

local fileLocation = LockOn_Options.script_path.."ARGUS7000/indicator/"

indicator_type = indicator_types.COMMON
init_pageID    = 1
purposes 	   = {render_purpose.GENERAL,render_purpose.HUD_ONLY_VIEW}

--subset ids
BASE    = 1

page_subsets  = {
[BASE]    		= fileLocation.."base_page.lua",
}
pages = {{BASE}}

dofile(LockOn_Options.common_script_path.."ViewportHandling.lua")
try_find_assigned_viewport("LEFT_MFCD")
--update_screenspace_diplacement(SelfWidth/SelfHeight,false)
--dedicated_viewport_arcade = dedicated_viewport
