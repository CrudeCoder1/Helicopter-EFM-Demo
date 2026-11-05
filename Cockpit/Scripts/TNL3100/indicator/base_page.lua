dofile(LockOn_Options.script_path.."TNL3100/indicator/definitions.lua")

local screen = MakeMaterial(nil,{0,25,0,50})

verts = {}
verts [1]= {-GetHalfWidth(),-GetHalfHeight()}
verts [2]= {-GetHalfWidth(),GetHalfHeight()}
verts [3]= {GetHalfWidth(),GetHalfHeight()}
verts [4]= {GetHalfWidth(),-GetHalfHeight()}

base 			 	 = CreateElement "ceMeshPoly"
base.name 			 = "base"
base.vertices 		 = verts
base.indices 		 = box_indices
base.material		 = screen
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = NOCLIP_LEVEL 
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
Add(base)


background					= CreateElement "ceMeshPoly"
background.name			 	= "background"
background.primitivetype 	= "triangles"
background.vertices 		= verts
background.indices			= box_indices
background.material			= screen
background.element_params 	= {"TNL3100_Power"}--, "ARGUS_Brightness"} 
background.controllers   	= {{"parameter_in_range",0,1}}--,{"opacity_using_parameter",1}}
background.h_clip_relation  = h_clip_relations.INCREASE_IF_LEVEL  
background.level			= NOCLIP_LEVEL   
background.change_opacity	= false
background.isvisible		= true
Add(background)

dofile(LockOn_Options.script_path.."TNL3100/indicator/common_page.lua")
--dofile(LockOn_Options.script_path.."TNL3100/indicator/ENRpage.lua")
--dofile(LockOn_Options.script_path.."TNL3100/indicator/DEPpage.lua")