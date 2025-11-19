dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,255,0,215}) --(R,G,B,opacity)
local center = {0,0,0}  --- {L/R,U/D,forward/back}

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
base.material		 = MakeMaterial(nil,{3,3,30,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = 5
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_POWER_AVAIL"}  
base.controllers     = {{"parameter_in_range",0,0.9,1.1}} 
Add(base)

local TurbineOutletTemp           = CreateElement "ceStringPoly"
TurbineOutletTemp.name            = create_guid_string()
TurbineOutletTemp.material        = font7segment	
TurbineOutletTemp.alignment       = "CenterCenter"
TurbineOutletTemp.init_pos		  = {-0.012, 0, 0}
TurbineOutletTemp.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
TurbineOutletTemp.formats         = {"%.0f"} 
TurbineOutletTemp.element_params  = {"TOT"}
TurbineOutletTemp.controllers     = {{"text_using_parameter",0,0}}
TurbineOutletTemp.h_clip_relation = h_clip_relations.compare
TurbineOutletTemp.level			  = 6
TurbineOutletTemp.parent_element  = base.name
Add(TurbineOutletTemp)

local Torque           = CreateElement "ceStringPoly"
Torque.name            = create_guid_string()
Torque.material        = font7segment	
Torque.alignment       = "CenterCenter"
Torque.init_pos		   = {0.012, 0, 0}
Torque.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Torque.formats         = {"%.0f"} 
Torque.element_params  = {"TRQ"}
Torque.controllers     = {{"text_using_parameter",0,0}}
Torque.h_clip_relation = h_clip_relations.compare
Torque.level		   = 6
Torque.parent_element  = base.name
--Add(Torque)  -- not finished yet


local Xsize = 0.002
local Ysize = Xsize*0.52
function addSegment(element)
	element.vertices	   	= {{-Xsize , Ysize}, 
							   { Xsize , Ysize},
							   { Xsize ,-Ysize},
							   {-Xsize ,-Ysize}}
	element.indices	   		= {0,1,2,2,3,0}
	element.material    	= MakeMaterial(nil,{0,255,0,215})
	element.h_clip_relation = h_clip_relations.REWRITE_LEVEL
	element.level 			= 6
	element.parent_element 	= base.name
	Add(element)
end

local numSegments = 26 
for i = 0,numSegments do
	local TOTbar		   = CreateElement "ceMeshPoly"
	TOTbar.name		 	   = "segment_"..i
	TOTbar.init_pos	 	   = { -0.012, -0.085 + i*0.0029, 0}
	TOTbar.controllers 	   = {{"parameter_in_range",0,i*25+300,1200}}
	TOTbar.element_params  = {"TOT"}	
	addSegment(TOTbar)
end

local numSegments = 26 
for i = 0,numSegments do
	local TRQbar		   = CreateElement "ceMeshPoly"
	TRQbar.name		 	   = "segment_"..i
	TRQbar.init_pos	 	   = { 0.012, -0.085 + i*0.0029, 0}
	TRQbar.controllers 	   = {{"parameter_in_range",0,i*1.85+20,85}}
	TRQbar.element_params  = {"TRQ"}	
	--addSegment(TRQbar)
end