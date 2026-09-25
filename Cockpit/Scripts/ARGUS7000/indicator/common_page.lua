

Common_origin          		= CreateElement "ceSimple"
Common_origin.name			= "Common_origin"
Common_origin.element_params = {"ARGUS_Power","ARGUS_Brightness"}
Common_origin.controllers 	= {{"parameter_in_range",0,1},{"parameter_in_range",1,0.1,1.1}}
Add(Common_origin)	
	
----------- Top Info Bar ------------------
boxX = horzPos(100)
boxY = vertPos(13)
local WindowMask   = CreateElement "ceMeshPoly" 
WindowMask.name 	= "WindowMask"
WindowMask.parent_element = "Common_origin"
WindowMask.vertices = {{-boxX, boxY}, -- affects sizing
					{ boxX, boxY},
					{ boxX,-boxY},
					{-boxX,-boxY}}
WindowMask.indices			= {0,1,2,2,3,0}
WindowMask.tex_coords	 	= {{0,0},{1,0},{1,1},{0,1}}
WindowMask.init_pos		= {0,vertPos(87)}
WindowMask.material		= MakeMaterial(nil,{205,5,5,150})	
WindowMask.h_clip_relation = h_clip_relations.INCREASE_LEVEL  
WindowMask.level			= DEFAULT_LEVEL
WindowMask.isvisible		= false
--WindowMask.element_params  = {""}  
--WindowMask.controllers     = {{"parameter_in_range",0,-0.1,1.9}}
Add(WindowMask)

local topLine = addLine("topBarLine", horzPos(200), {horzPos(-100), vertPos(75)}, -90)
topLine.level = WINDOW_LEVEL

addText("MagBearing", Common_origin.name, {horzPos(-91), vertPos(92)}, {"%03.0f° BRG"}, {"ARGUS_Bearing"}, {{"text_using_parameter",0,0},{"parameter_in_range",0,0,361}}, WINDOW_LEVEL, "LeftCenter")
addText("distance", Common_origin.name, {horzPos(-91), vertPos(82)}, {"%.1f NM"}, {"ARGUS_Distance"}, {{"text_using_parameter",0,0},{"parameter_in_range",0,0,999}}, WINDOW_LEVEL, "LeftCenter")

local HDGBox = MakeMaterial("ArgusHdgWindow",ARGUS_Green)
local box = addTexPoly("hdgBox",  {horzPos(2), vertPos(87)}, vertPos(16), HDGBox, Common_origin.name)
box.level = WINDOW_LEVEL
addText("magText", Common_origin.name, {0, vertPos(93)}, {"MAG"}, {"MAG_HEADING"}, {{"text_using_parameter",0,0}}, WINDOW_LEVEL, "CenterCenter",stringdefSmall)
addText("magHeading", Common_origin.name, {0, vertPos(85)}, {"%03.0f°"}, {"MAG_HEADING"}, {{"text_using_parameter",0,0}}, WINDOW_LEVEL, "CenterCenter")

addText("grndSpd", Common_origin.name, {horzPos(91), vertPos(92)}, {"%.0f KTS"}, {"ARGUS_GndSpd"}, {{"text_using_parameter",0,0}}, WINDOW_LEVEL, "RightCenter")
addText("TTGs", Common_origin.name, {horzPos(91), vertPos(82)}, {"%02.0f:","%02.0f"}, {"ARGUS_TTG_Hour","ARGUS_TTG_Min","ARGUS_TTG_Sec"}, {{"text_using_parameter",0,0},{"text_using_parameter",1,0},{"text_using_parameter",2,1}}, WINDOW_LEVEL, "RightCenter")

------------ Bottom Info Bar ------------------


boxX = horzPos(100)
boxY = vertPos(12)
local WindowMask2   = CreateElement "ceMeshPoly" 
WindowMask2.name 	= "WindowMask2"
WindowMask2.parent_element = "Common_origin"
WindowMask2.vertices = {{-boxX, boxY}, -- affects sizing
					{ boxX, boxY},
					{ boxX,-boxY},
					{-boxX,-boxY}}
WindowMask2.indices			= {0,1,2,2,3,0}
WindowMask2.tex_coords	 	= {{0,0},{1,0},{1,1},{0,1}}
WindowMask2.init_pos		= {0,vertPos(-88)}
WindowMask2.material		= MakeMaterial(nil,{205,5,5,150})	
WindowMask2.h_clip_relation = h_clip_relations.INCREASE_LEVEL  
WindowMask2.level			= DEFAULT_LEVEL
WindowMask2.isvisible		= false
--WindowMask2.element_params  = {""}  
--WindowMask2.controllers     = {{"parameter_in_range",0,-0.1,1.9}}
Add(WindowMask2)

local bottLine = addLine("bottomBarLine", horzPos(200), {horzPos(-100), vertPos(-76)}, -90)
bottLine.level = WINDOW_LEVEL

----- Lines and box for CDI
local boxLine1 = addLine("boxLine1", horzPos(68), {horzPos(-34), vertPos(-89)}, -90)
boxLine1.level = WINDOW_LEVEL
local boxLine2 = addLine("boxLine2", horzPos(14), {horzPos(-34), vertPos(-89)}, 0)
boxLine2.level = WINDOW_LEVEL
local boxLine3 = addLine("boxLine3", horzPos(14), {horzPos(34), vertPos(-89)}, 0)
boxLine3.level = WINDOW_LEVEL

local CDIlineR1 = addLine("CDIlineR1", horzPos(10), {horzPos(12), vertPos(-87)}, 0)
CDIlineR1.level = WINDOW_LEVEL
local CDIlineR2 = addLine("CDIlineR2", horzPos(10), {horzPos(18), vertPos(-87)}, 0)
CDIlineR2.level = WINDOW_LEVEL
local CDIlineR3 = addLine("CDIlineR3", horzPos(10), {horzPos(24), vertPos(-87)}, 0)
CDIlineR3.level = WINDOW_LEVEL
local CDIlineR4 = addLine("CDIlineR4", horzPos(10), {horzPos(30), vertPos(-87)}, 0)
CDIlineR4.level = WINDOW_LEVEL

local circle		   = CreateElement "ceMeshPoly"
circle.name 		   = create_guid_string()
circle.init_pos 	   = {0, vertPos(-83)}
circle.material 	   = MakeMaterial(nil,ARGUS_Green)
circle.parent_element = Common_origin.name
set_circle(circle, vertPos(5), vertPos(3.6), nil, 16)  -- name, outer R, inner R, arc, sides
Add_Argus_Element(circle)
circle.level			 = WINDOW_LEVEL

local CDIlineL1 = addLine("CDIlineL1", horzPos(10), {horzPos(-12), vertPos(-87)}, 0)
CDIlineL1.level = WINDOW_LEVEL
local CDIlineL2 = addLine("CDIlineL2", horzPos(10), {horzPos(-18), vertPos(-87)}, 0)
CDIlineL2.level = WINDOW_LEVEL
local CDIlineL3 = addLine("CDIlineL3", horzPos(10), {horzPos(-24), vertPos(-87)}, 0)
CDIlineL3.level = WINDOW_LEVEL
local CDIlineL4 = addLine("CDIlineL4", horzPos(10), {horzPos(-30), vertPos(-87)}, 0)
CDIlineL4.level = WINDOW_LEVEL
------------

addText("ObjIdent", Common_origin.name, {horzPos(0), vertPos(-96)}, {"%s -T"}, {"TACAN_IDENT"}, {{"text_using_parameter",0,0}}, WINDOW_LEVEL, "CenterCenter")
