dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local greenColor = {0,255,0,215}
local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"},greenColor) --(R,G,B,opacity)


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
frequency.init_pos 	      = {0.001, 0}
frequency.stringdefs      = {0.009,0.75*0.009, 0.002, 0}  -- {size vertical, horizontal, 0, 0}
frequency.formats         = {"%.0f"} 
frequency.element_params  = {"ARC182_FREQUENCY","ARC182_BRIGHTNESS"}
frequency.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}
frequency.h_clip_relation = h_clip_relations.compare
frequency.level			  = 6
frequency.parent_element  = "base"
Add(frequency)

local decimal		   = CreateElement "ceMeshPoly"
decimal.name 		   = create_guid_string()
decimal.init_pos 	   = {0, -0.004}
decimal.material 	   = MakeMaterial(nil,greenColor) 
decimal.element_params = {"ARC182_DECIMAL"}
decimal.controllers    = {{"parameter_in_range", 0, 1}}	
decimal.parent_element = base.name
decimal.h_clip_relation  = h_clip_relations.compare
decimal.level			 = 6 
set_circle(decimal, 0.0007, 0, nil, 16)  -- name, outer R, inner R, arc, sides
--decimal.use_mipfilter    = true
--decimal.additive_alpha   = false
Add(decimal)
