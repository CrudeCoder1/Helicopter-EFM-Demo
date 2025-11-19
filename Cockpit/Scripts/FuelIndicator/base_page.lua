dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 
local orangeColor = {255,40,20,220}
local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"}, orangeColor)
local center = {0.0625,-0.575,0.364}  --- {L/R,U/D,forward/back}

verts = {}
dx=.0145
dy=.0084
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

local FuelAmount           = CreateElement "ceStringPoly"
FuelAmount.name            = create_guid_string()
FuelAmount.material        = font7segment
FuelAmount.alignment       = "CenterCenter"
FuelAmount.stringdefs      = {0.0125,0.75 * 0.0125, 0, 0}  -- {size vertical, horizontal, 0, 0}
FuelAmount.formats         = {"%.0f"} 
FuelAmount.element_params  = {"CURRENT_FUELT"}
FuelAmount.controllers     = {{"text_using_parameter",0,0}}  
FuelAmount.h_clip_relation  = h_clip_relations.compare
FuelAmount.level			= 6
FuelAmount.parent_element  = base.name
Add(FuelAmount)



local Xsize = 0.0025
local Ysize = Xsize*0.53
function addSegment(element)
	element.vertices	   	= {{-Xsize , Ysize*1.3}, -- segments not perfectly square due to circular arc
							   { Xsize , Ysize},
							   { Xsize ,-Ysize},
							   {-Xsize ,-Ysize*1.3}}
	element.indices	   		= {0,1,2,2,3,0}
	element.material    	= MakeMaterial(nil,orangeColor)
	element.h_clip_relation = h_clip_relations.REWRITE_LEVEL
	element.level 			= 6
	element.parent_element 	= base.name
	element.additive_alpha	= false
	element.element_params  = {"CURRENT_FUELT"}
	Add(element)
end

local numSegments = 19 -- actually 20
for i = 0,numSegments do
	local segment1			= CreateElement "ceMeshPoly"
	segment1.name		   	= "segment_"..i
	segment1.init_pos	   	= { -0.021*math.cos((i/numSegments)*math.pi), -0.002 - .019*math.sin((i/numSegments)*math.pi), 0.0001}
	segment1.init_rot		= {(i/numSegments)*180}
	segment1.controllers  = {{"parameter_in_range",0,i*40,801}} 
	addSegment(segment1)
end