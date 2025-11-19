dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

DEFAULT_LEVEL = 6
NOCLIP_LEVEL  = DEFAULT_LEVEL - 1
local FONT    = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,255,0,215})
center={-0.112,-0.283,0.362}  --- {L/R,U/D,forward/back}


function AddElement(object)
	object.use_mipfilter    = true
	object.h_clip_relation  = h_clip_relations.compare
	object.level			= DEFAULT_LEVEL
    Add(object)
end

verts = {}
dx=.037
dy=.0147
verts [1]= {-dx,-dy}
verts [2]= {-dx,dy}
verts [3]= {dx,dy}
verts [4]= {dx,-dy}

base 			 	 = CreateElement "ceMeshPoly"
base.name 			 = "base"
base.vertices 		 = verts
base.indices 		 = {0,1,2,2,3,0}
base.init_pos		 = center   
base.material		 = MakeMaterial(nil,{30,3,3,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = NOCLIP_LEVEL
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_POWER_AVAIL"}  
base.controllers     = {{"opacity_using_parameter",0}} 
Add(base)




local GMT_hours           = CreateElement "ceStringPoly"
GMT_hours.name            = create_guid_string()
GMT_hours.material        = FONT
GMT_hours.init_pos        = {center[1]-.01,center[2],center[3]}		
GMT_hours.alignment       = "RightBottom"
GMT_hours.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
GMT_hours.formats         = {"%02.0f"} 
GMT_hours.element_params  = {"GMT_HOURS","DC_POWER_AVAIL"}
GMT_hours.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(GMT_hours)

local GMT_mins           = CreateElement "ceStringPoly"
GMT_mins.name            = create_guid_string()
GMT_mins.material        = FONT
GMT_mins.init_pos        = center 
GMT_mins.alignment       = "CenterBottom"
GMT_mins.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
GMT_mins.formats         = {"%02.0f"} 
GMT_mins.element_params  = {"GMT_MINS","DC_POWER_AVAIL"}
GMT_mins.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(GMT_mins)

local GMT_sec           = CreateElement "ceStringPoly"
GMT_sec.name            = create_guid_string()
GMT_sec.material        = FONT
GMT_sec.init_pos        = {center[1]+.01,center[2],center[3]}
GMT_sec.alignment       = "LeftBottom"
GMT_sec.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
GMT_sec.formats         = {"%02.0f"} 
GMT_sec.element_params  = {"GMT_SECS","DC_POWER_AVAIL"}
GMT_sec.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(GMT_sec)


local LT_hours           = CreateElement "ceStringPoly"
LT_hours.name            = create_guid_string()
LT_hours.material        = FONT
LT_hours.init_pos        = {center[1]-.01,center[2]-0.005,center[3]}		
LT_hours.alignment       = "RightTop"
LT_hours.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
LT_hours.formats         = {"%02.0f"} 
LT_hours.element_params  = {"LT_HOURS","DC_POWER_AVAIL"}
LT_hours.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(LT_hours)

local LT_mins           = CreateElement "ceStringPoly"
LT_mins.name            = create_guid_string()
LT_mins.material        = FONT
LT_mins.init_pos        = {center[1],center[2]-0.005,center[3]}	 
LT_mins.alignment       = "CenterTop"
LT_mins.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
LT_mins.formats         = {"%02.0f"} 
LT_mins.element_params  = {"GMT_MINS","DC_POWER_AVAIL"}
LT_mins.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(LT_mins)