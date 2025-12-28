dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

DEFAULT_LEVEL = 6
NOCLIP_LEVEL  = DEFAULT_LEVEL - 1
local FONT    = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,255,0,215})
center={0,-0.006,0}  --- {L/R,U/D,forward/back}


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
base.element_params  = {"DC_Bus_Voltage"}  
base.controllers     = {{"parameter_in_range",0,18,30}} 
Add(base)




local GMT_hours           = CreateElement "ceStringPoly"
GMT_hours.name            = create_guid_string()
GMT_hours.material        = FONT
GMT_hours.parent_element  = "base"
GMT_hours.init_pos        = {-.01,0,0}		
GMT_hours.alignment       = "RightBottom"
GMT_hours.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
GMT_hours.formats         = {"%02.0f"} 
GMT_hours.element_params  = {"GMT_HOURS","CLOCK_BRIGHTNESS"}
GMT_hours.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(GMT_hours)

local GMT_mins           = CreateElement "ceStringPoly"
GMT_mins.name            = create_guid_string()
GMT_mins.material        = FONT
GMT_mins.parent_element  = "base"
GMT_mins.init_pos        = {0,0,0} 
GMT_mins.alignment       = "CenterBottom"
GMT_mins.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
GMT_mins.formats         = {"%02.0f"} 
GMT_mins.element_params  = {"GMT_MINS","CLOCK_BRIGHTNESS"}
GMT_mins.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(GMT_mins)

local GMT_sec           = CreateElement "ceStringPoly"
GMT_sec.name            = create_guid_string()
GMT_sec.material        = FONT
GMT_sec.parent_element  = "base"
GMT_sec.init_pos        = {0.01,0,0}
GMT_sec.alignment       = "LeftBottom"
GMT_sec.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
GMT_sec.formats         = {"%02.0f"} 
GMT_sec.element_params  = {"GMT_SECS","CLOCK_BRIGHTNESS"}
GMT_sec.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(GMT_sec)


local digit_hours           = CreateElement "ceStringPoly"
digit_hours.name            = create_guid_string()
digit_hours.material        = FONT
digit_hours.parent_element  = "base"
digit_hours.init_pos        = {-.01,-0.005,0}		
digit_hours.alignment       = "RightTop"
digit_hours.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
digit_hours.formats         = {"%02.0f"} 
digit_hours.element_params  = {"CLOCK_HOURS","CLOCK_BRIGHTNESS"}
digit_hours.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(digit_hours)

local digit_mins           = CreateElement "ceStringPoly"
digit_mins.name            = create_guid_string()
digit_mins.material        = FONT
digit_mins.parent_element  = "base"
digit_mins.init_pos        = {0,-0.005,}	 
digit_mins.alignment       = "CenterTop"
digit_mins.stringdefs      = {0.012,0.75 * 0.012, 0, 0}  -- {size vertical, horizontal, 0, 0}
digit_mins.formats         = {"%02.0f"} 
digit_mins.element_params  = {"CLOCK_MINS","CLOCK_BRIGHTNESS"}
digit_mins.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
AddElement(digit_mins)