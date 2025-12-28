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
local TurbineOutletTemp           = CreateElement "ceStringPoly"
TurbineOutletTemp.name            = create_guid_string()
TurbineOutletTemp.material        = font7segment	
TurbineOutletTemp.alignment       = "RightCenter"
TurbineOutletTemp.init_pos		  = {-0.004, 0, 0}
TurbineOutletTemp.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
TurbineOutletTemp.formats         = {"%.0f"} 
TurbineOutletTemp.element_params  = {"VIDS_TOT","VIDS_BRIGHTNESS","VIDS_MODE"}
TurbineOutletTemp.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1},{"parameter_in_range",2,-0.1,1.1}}
TurbineOutletTemp.h_clip_relation = h_clip_relations.compare
TurbineOutletTemp.level			  = 6
TurbineOutletTemp.parent_element  = base.name
Add(TurbineOutletTemp)

local Torque           = CreateElement "ceStringPoly"
Torque.name            = create_guid_string()
Torque.material        = font7segment	
Torque.alignment       = "RightCenter"
Torque.init_pos		   = {0.020, 0, 0}
Torque.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Torque.formats         = {"%.0f"} 
Torque.element_params  = {"VIDS_TRQ","VIDS_BRIGHTNESS","VIDS_MODE"}
Torque.controllers     = {{"text_using_parameter",0,0},{"opacity_using_parameter",1},{"parameter_in_range",2,-0.1,1.1}}
Torque.h_clip_relation = h_clip_relations.compare
Torque.level		   = 6
Torque.parent_element  = base.name
Add(Torque)


--========== Vertical bars  ===================
local redMat = MakeMaterial(nil,{200,0,0,215})
local yellowMat = MakeMaterial(nil,{255,255,0,215})
local greenMat = MakeMaterial(nil,{0,255,0,215})

local segmentSize = 0.0027

local Xsize = 0.002
local Ysize = 0.001
function addSegment(side, pos, param, minVal, material)
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
	element.controllers  	= {{"parameter_in_range",0, minVal,2000},{"opacity_using_parameter",1}}
	element.element_params  = param
	Add(element)
	return element
end

for i = 18,74,2  do -- Torque 16-74 (2 psi increment)
	mat = greenMat
	if i<=62 and i>=60 then mat=yellowMat end
	if i>=63 then mat=redMat end
	addSegment('R',-8*segmentSize+ i/2*segmentSize, {"VIDS_TRQ","VIDS_BRIGHTNESS"}, i, mat)
end

for i = 275,975,25  do -- TOT (25 deg increment)
	mat = greenMat
	if i<=905 and i>694 then mat=yellowMat end
	if i>905 then mat=redMat end
	addSegment('L',-10*segmentSize+ i/25*segmentSize, {"VIDS_TOT","VIDS_BRIGHTNESS"}, i, mat)
end

----- for digit test only ------
local tst	         = CreateElement "ceSimple"
tst.name 		     = "tst"
tst.parent_element 	 = base.name
tst.element_params  = {"VIDS_MODE"}  
tst.controllers     = {{"parameter_in_range",0,0.9,1.1}}
Add(tst)

for i = 275,975,25  do -- TOT (25 deg increment)
	mat = greenMat
	if i<=905 and i>694 then mat=yellowMat end
	if i>905 then mat=redMat end
	L = addSegment('L',-10*segmentSize+ i/25*segmentSize, {"VIDS_TOT","VIDS_BRIGHTNESS"}, i-100, mat)
	L.parent_element = tst.name
end
