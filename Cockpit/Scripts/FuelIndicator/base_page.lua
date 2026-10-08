dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 
local orangeColor = {255,40,20,220}
local font7segment = MakeFont({used_DXUnicodeFontData = "AH6J_font7segment"}, orangeColor)

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
base.material		 = MakeMaterial(nil,{3,30,3,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = 5
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_Bus_Voltage"}  
base.controllers     = {{"parameter_in_range",0,15,30}}
Add(base)

local FuelAmount           = CreateElement "ceStringPoly"
FuelAmount.name            = create_guid_string()
FuelAmount.material        = font7segment
FuelAmount.alignment       = "CenterCenter"
FuelAmount.stringdefs      = {0.0125,0.7 * 0.0125, 0.0007, 0}  -- {size vertical, horizontal, 0, 0}
FuelAmount.formats         = {"%03.0f"}
FuelAmount.element_params  = {"FQI_Quantity","FQI_Brightness"}
FuelAmount.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1}}  
FuelAmount.h_clip_relation  = h_clip_relations.compare
FuelAmount.level			= 6
FuelAmount.parent_element  = base.name
Add(FuelAmount)

local decimal		   = CreateElement "ceMeshPoly"
decimal.name 		   = create_guid_string()
decimal.init_pos 	   = {-0.005, -0.006}
decimal.material 	   = MakeMaterial(nil,orangeColor) 
decimal.element_params = {"FQI_Brightness"}
decimal.controllers    = {{"opacity_using_parameter", 0}}	
decimal.parent_element = base.name
decimal.h_clip_relation  = h_clip_relations.compare
decimal.level			 = 6 
set_circle(decimal, 0.0007, 0, nil, 16)  -- name, outer R, inner R, arc, sides
Add(decimal)

local AftLetter           = CreateElement "ceStringPoly"
AftLetter.name            = create_guid_string()
AftLetter.material        = font7segment
AftLetter.alignment       = "CenterCenter"
AftLetter.init_pos 	   	  = {0.004, -0.0115}
AftLetter.stringdefs      = {0.006,0.7 * 0.006, 0.0007, 0}  -- {size vertical, horizontal, 0, 0}
--AftLetter.formats         = {"%03.0f"}
AftLetter.value			  = "A"
AftLetter.element_params  = {"FQI_AftSel","FQI_Brightness"}
AftLetter.controllers     = {{"parameter_in_range",0,1},{"opacity_using_parameter",1}}  
AftLetter.h_clip_relation  = h_clip_relations.compare
AftLetter.level			= 6
AftLetter.parent_element  = base.name
Add(AftLetter)


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
	element.element_params  = {"MainFuelTank_lb","FQI_Brightness"}
	Add(element)
end

local radius = 0.0195
local numSegments = 19 -- actually 20
for i = 0,numSegments do
	local segment1			= CreateElement "ceMeshPoly"
	segment1.name		   	= "segment_"..i
	segment1.init_pos	   	= { -radius*math.cos((i/numSegments)*math.pi), -radius*math.sin((i/numSegments)*math.pi)-0.0020, -0.0001}
	segment1.init_rot		= {(i/numSegments)*180}
	segment1.controllers  = {{"parameter_in_range",0,i*20,402},{"opacity_using_parameter",1}} 
	addSegment(segment1)
end


-- fuel transfer indicator
local FTI		   = CreateElement "ceMeshPoly"
FTI.name 		   = create_guid_string()
FTI.init_pos 	   = {0.0075, -0.0155, -0.001}
FTI.material 	   = MakeMaterial(nil,orangeColor)
FTI.element_params = {"FQI_FTI","FQI_Brightness"}
FTI.controllers    = {{"parameter_in_range", 0, 1},{"opacity_using_parameter",1}}
FTI.parent_element = base.name
FTI.h_clip_relation= h_clip_relations.compare
FTI.level		   = 6
set_circle(FTI, 0.0011, 0, nil, 16)  -- name, outer R, inner R, arc, sides
Add(FTI)