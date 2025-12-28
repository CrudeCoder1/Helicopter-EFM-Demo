dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,255,0,215}) --(R,G,B,opacity)


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
base.material		 = MakeMaterial(nil,{3,3,30,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = 5
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_Bus_Voltage"}  
base.controllers     = {{"parameter_in_range",0,15,30}}
Add(base)



--========== Digits ===================
local N2rpm           = CreateElement "ceStringPoly"
N2rpm.name            = create_guid_string()
N2rpm.material        = font7segment	
N2rpm.alignment       = "RightCenter"
N2rpm.init_pos		  = {-0.004, 0, 0}
N2rpm.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
N2rpm.formats         = {"%.0f"} 
N2rpm.element_params  = {"VIDS_N2","VIDS_BRIGHTNESS","VIDS_MODE"}
N2rpm.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1},{"parameter_in_range",2,-0.1,1.1}}
N2rpm.h_clip_relation = h_clip_relations.compare
N2rpm.level			  = 6
N2rpm.parent_element  = base.name
Add(N2rpm)

local NRrpm           = CreateElement "ceStringPoly"
NRrpm.name            = create_guid_string()
NRrpm.material        = font7segment	
NRrpm.alignment       = "RightCenter"
NRrpm.init_pos		  = {0.020, 0, 0}
NRrpm.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
NRrpm.formats         = {"%.0f"} 
NRrpm.element_params  = {"VIDS_NR","VIDS_BRIGHTNESS","VIDS_MODE"}
NRrpm.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1},{"parameter_in_range",2,-0.1,1.1}}
NRrpm.h_clip_relation = h_clip_relations.compare
NRrpm.level			  = 6
NRrpm.parent_element  = base.name
Add(NRrpm)


--========== Vertical bars  ===================
local redMat = MakeMaterial(nil,{200,0,0,215})
local yellowMat = MakeMaterial(nil,{255,255,0,215})
local greenMat = MakeMaterial(nil,{0,255,0,215})

local segmentSize = 0.0027

local Xsize = 0.002
local Ysize = 0.0011
function addSegment(side, pos, param, minVal, maxVal, material)
	local element		  	= CreateElement "ceMeshPoly"
	element.name		  	= create_guid_string()
	element.vertices	   	= {{-Xsize , Ysize}, 
							   { Xsize , Ysize},
							   { Xsize ,-Ysize},
							   {-Xsize ,-Ysize}}
	element.indices	   		= {0,1,2,2,3,0}
	element.material    	= material or greenMat
	element.h_clip_relation = h_clip_relations.REWRITE_LEVEL
	element.level 			= 6
	element.parent_element 	= base.name
	if side=='L' then 
		element.init_pos	  	= {-0.0115, -0.0877 + pos, 0}
	else
		element.init_pos	  	= {0.0115, -0.0877 + pos, 0}
	end
	element.controllers  	= {{"parameter_in_range",0, minVal-0.1,maxVal},{"opacity_using_parameter",1}}
	element.element_params  = param
	Add(element)
	return element
end



for i = 10,40,10  do -- 0-40 (10% increment)
	addSegment('L',i/10*segmentSize, {"VIDS_N2","VIDS_BRIGHTNESS"}, i, 96,redMat)
	addSegment('R',i/10*segmentSize, {"VIDS_NR","VIDS_BRIGHTNESS"}, i, 87,redMat)
end
for i = 60,80,20 do -- 40-80 (20% increment)
	addSegment('L',2*segmentSize+i/20*segmentSize, {"VIDS_N2","VIDS_BRIGHTNESS"}, i, 96,redMat)
	addSegment('R',2*segmentSize+i/20*segmentSize, {"VIDS_NR","VIDS_BRIGHTNESS"}, i, 87,redMat)
end
for i = 87,109,1 do -- 87-109 (1% increment)
	mat = greenMat
	maxVal = 130
	if i<=95 then mat=redMat maxVal = 96 end
	if i<=98 and i>=96 then mat=yellowMat maxVal = 99 end
	if i<=106 and i>=101 then mat=yellowMat end
	if i>=107 then mat=redMat end
	addSegment('L',-80*segmentSize+i*segmentSize, {"VIDS_N2","VIDS_BRIGHTNESS"}, i, maxVal, mat)
	
	mat = greenMat
	if i>=107 then mat=redMat end
	addSegment('R',-80*segmentSize+i*segmentSize, {"VIDS_NR","VIDS_BRIGHTNESS"}, i, 130, mat)
end

----- for digit test only ------
local tst	         = CreateElement "ceSimple"
tst.name 		     = "tst"
tst.parent_element 	 = base.name
tst.element_params  = {"VIDS_MODE"}  
tst.controllers     = {{"parameter_in_range",0,0.9,1.1}}
Add(tst)

for i = 10,40,10  do -- 0-40 (10% increment)
	L = addSegment('L',i/10*segmentSize, {"VIDS_N2","VIDS_BRIGHTNESS"}, i, 889,redMat)
	L.parent_element = tst.name
	R = addSegment('R',i/10*segmentSize, {"VIDS_NR","VIDS_BRIGHTNESS"}, i, 889,redMat)
	R.parent_element = tst.name
end
for i = 60,80,20 do -- 40-80 (20% increment)
	L = addSegment('L',2*segmentSize+i/20*segmentSize, {"VIDS_N2","VIDS_BRIGHTNESS"}, i, 889,redMat)
	L.parent_element = tst.name
	R = addSegment('R',2*segmentSize+i/20*segmentSize, {"VIDS_NR","VIDS_BRIGHTNESS"}, i, 889,redMat)
	R.parent_element = tst.name
end
for i = 87,109,1 do -- 87-109 (1% increment)
	mat = greenMat
	maxVal = 889
	if i<=95 then mat=redMat maxVal = 889 end
	if i<=98 and i>=96 then mat=yellowMat maxVal = 889 end
	if i<=106 and i>=101 then mat=yellowMat end
	if i>=107 then mat=redMat end
	L=addSegment('L',-80*segmentSize+i*segmentSize, {"VIDS_N2","VIDS_BRIGHTNESS"}, i, maxVal, mat)
	L.parent_element = tst.name
	
	mat = greenMat
	if i>=107 then mat=redMat end
	R=addSegment('R',-80*segmentSize+i*segmentSize, {"VIDS_NR","VIDS_BRIGHTNESS"}, i, 889, mat)
	R.parent_element = tst.name
end

