dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"},{255,0,0,255}) --(R,G,B,opacity)


verts = {}
dx=.022
dy=.0075
verts [1]= {-dx,-dy}
verts [2]= {-dx,dy}
verts [3]= {dx,dy}
verts [4]= {dx,-dy}
	
local base 			 = CreateElement "ceMeshPoly"
base.name 			 = "base"
base.vertices 		 = verts
base.indices 		 = {0,1,2,2,3,0}
--base.init_pos		 = {0,-0.473,-0.527}  --- {L/R,U/D,forward/back}
base.init_rot		 = {0,0,80.77}     
base.material		 = MakeMaterial(nil,{2,225,2,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = 5
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_Bus_Voltage","ARC182_ON"}  
base.controllers     = {{"parameter_in_range",0,20,29},{"parameter_in_range",1,1}} 
Add(base)

local frequency           = CreateElement "ceStringPoly"
frequency.name            = create_guid_string()
frequency.material        = font7segment	
frequency.alignment       = "CenterCenter"
frequency.stringdefs      = {0.008,0.75*0.008, 0.002, 0}  -- {size vertical, horizontal, 0, 0}
frequency.formats         = {"%.0f"} 
frequency.element_params  = {"ARC182_FREQUENCY","ARC182_BRIGHTNESS"}
frequency.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}
frequency.h_clip_relation = h_clip_relations.compare
frequency.level			  = 6
frequency.parent_element  = "base"
Add(frequency)