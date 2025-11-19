dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local fontDotMatrix = MakeFont({used_DXUnicodeFontData = "fontDotMatrix"},{0,255,0,255}) --(R,G,B,opacity)
local center = {0.1189,-0.2212,1.3665-1}  --- {L/R,U/D,forward/back}

verts = {}
dx=.02
dy=.006
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

local Ralt_xxxN           = CreateElement "ceStringPoly"
Ralt_xxxN.name            = create_guid_string()
Ralt_xxxN.material        = fontDotMatrix	
Ralt_xxxN.parent_element  = "base"
Ralt_xxxN.alignment       = "CenterCenter"
Ralt_xxxN.stringdefs      = {0.009,0.75*0.009, 0.0036, 0}  -- {size vertical, size horizontal, horizontal spacing, 0}
Ralt_xxxN.formats         = {"%4.0f"} 
Ralt_xxxN.element_params  = {"CURRENT_RALT"}
Ralt_xxxN.controllers     = {{"text_using_parameter",0,0}}
Ralt_xxxN.h_clip_relation = h_clip_relations.compare
Ralt_xxxN.level			  = 6
Add(Ralt_xxxN)