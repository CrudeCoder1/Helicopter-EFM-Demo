dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,255,0,215}) --(R,G,B,opacity)
local center = {0.031,-0.348,0.366}  --- {L/R,U/D,forward/back}

verts = {}
dx=.0125
dy=.0075
verts [1]= {-dx,-dy}
verts [2]= {-dx,dy}
verts [3]= {dx,dy}
verts [4]= {dx,-dy}
	
local base 			 = CreateElement "ceMeshPoly"
base.name 			 = "base"
base.vertices 		 = verts
base.indices 		 = {0,1,2,2,3,0}
base.init_pos		 = center   
base.material		 = MakeMaterial(nil,{3,3,3,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = 5
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_POWER_AVAIL"}  
base.controllers     = {{"parameter_in_range",0,0.9,1.1}} 
Add(base)

local bearing           = CreateElement "ceStringPoly"
bearing.name            = create_guid_string()
bearing.material        = font7segment	
bearing.alignment       = "CenterCenter"
bearing.stringdefs      = {0.01,0.75*0.01, 0, 0}  -- {size vertical, horizontal, 0, 0}
bearing.formats         = {"%.0f"} 
bearing.element_params  = {"CURRENT_HDG"}
bearing.controllers     = {{"text_using_parameter",0,0}}
bearing.h_clip_relation = h_clip_relations.compare
bearing.level			= 6
bearing.parent_element  = "base"
Add(bearing)