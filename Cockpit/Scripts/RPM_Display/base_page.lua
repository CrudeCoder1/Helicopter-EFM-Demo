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

local N2rpm           = CreateElement "ceStringPoly"
N2rpm.name            = create_guid_string()
N2rpm.material        = font7segment	
N2rpm.alignment       = "CenterCenter"
N2rpm.init_pos		  = {-0.012, 0, 0}
N2rpm.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
N2rpm.formats         = {"%.0f"} 
N2rpm.element_params  = {"N2_RPM"}
N2rpm.controllers     = {{"text_using_parameter",0,0}}
N2rpm.h_clip_relation = h_clip_relations.compare
N2rpm.level			  = 6
N2rpm.parent_element  = base.name
Add(N2rpm)

local NRrpm           = CreateElement "ceStringPoly"
NRrpm.name            = create_guid_string()
NRrpm.material        = font7segment	
NRrpm.alignment       = "CenterCenter"
NRrpm.init_pos		  = {0.012, 0, 0}
NRrpm.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
NRrpm.formats         = {"%.0f"} 
NRrpm.element_params  = {"N2_RPM"}
NRrpm.controllers     = {{"text_using_parameter",0,0}}
NRrpm.h_clip_relation = h_clip_relations.compare
NRrpm.level			  = 6
NRrpm.parent_element  = base.name
Add(NRrpm)


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
	element.element_params  = {"N2_RPM"}
	Add(element)
end

local numSegments = 26 

for i = 0,1 do -- 0-40
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + i*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 1+i*38,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + i*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 1+i*38,110}} 
	addSegment(segmentR)
end

for i = 0,1 do -- 40-70
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + (i+2)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 55+i*15,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + (i+2)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 55+i*15,110}} 
	addSegment(segmentR)
end

for i = 0,4 do -- 70-80
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + (i+4)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 72+i*2,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + (i+4)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 72+i*2,110}} 
	addSegment(segmentR)
end

for i = 0,4 do -- 80-90
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + (i+9)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 82+i*2,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + (i+9)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 82+i*2,110}} 
	addSegment(segmentR)
end

for i = 0,4 do -- 90-98
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + (i+14)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 92+i*1.6,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + (i+14)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 92+i*1.6,110}} 
	addSegment(segmentR)
end

	local segmentL		  = CreateElement "ceMeshPoly" -- 100
	segmentL.name		  = "segmentL_"
	segmentL.init_pos	  = { -0.012, -0.085 + (19)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 100,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"
	segmentR.init_pos	  = { 0.012, -0.085 + (19)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 100,110}} 
	addSegment(segmentR)
	
for i = 0,4 do -- 100-104
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + (i+20)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 100.8+i*0.8,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + (i+20)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 100.8+i*0.8,110}} 
	addSegment(segmentR)
end

for i = 0,1 do -- 104-109
	local segmentL		  = CreateElement "ceMeshPoly"
	segmentL.name		  = "segmentL_"..i
	segmentL.init_pos	  = { -0.012, -0.085 + (i+25)*0.0029, 0}
	segmentL.controllers  = {{"parameter_in_range",0, 104.5+i*3.5,110}} 
	addSegment(segmentL)
	
	local segmentR 		  = CreateElement "ceMeshPoly"
	segmentR.name		  = "segmentR_"..i
	segmentR.init_pos	  = { 0.012, -0.085 + (i+25)*0.0029, 0}
	segmentR.controllers  = {{"parameter_in_range",0, 104.5+i*3.5,110}} 
	addSegment(segmentR)
end