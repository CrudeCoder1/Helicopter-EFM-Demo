

Common_origin          		= CreateElement "ceSimple"
Common_origin.name			= "Common_origin"
--Common_origin.element_params = {"_POWER"}
--Common_origin.controllers 	= {{"parameter_in_range",0,1}}
Add(Common_origin)	
	
------- Top Info Bar ------------------
boxX = horzPos(100)
boxY = vertPos(12)
local WindowMask   = CreateElement "ceMeshPoly" 
WindowMask.name 	= "WindowMask"
WindowMask.parent_element = "Common_origin"
WindowMask.vertices = {{-boxX, boxY}, -- affects sizing
					{ boxX, boxY},
					{ boxX,-boxY},
					{-boxX,-boxY}}
WindowMask.indices			= {0,1,2,2,3,0}
WindowMask.tex_coords	 	= {{0,0},{1,0},{1,1},{0,1}}
WindowMask.init_pos		= {0,vertPos(89)}
WindowMask.material		= MakeMaterial(nil,{205,5,5,150})	
WindowMask.h_clip_relation = h_clip_relations.INCREASE_LEVEL  
WindowMask.level			= DEFAULT_LEVEL
WindowMask.isvisible		= false
--WindowMask.element_params  = {""}  
--WindowMask.controllers     = {{"parameter_in_range",0,-0.1,1.9}}
Add(WindowMask)

local topLine = addLine("topBarLine", horzPos(200), {horzPos(-100), vertPos(78)}, -90)
topLine.level = WINDOW_LEVEL

local txtgroundSpeed           = CreateElement "ceStringPoly"
txtgroundSpeed.name            = "txtgroundSpeed"
txtgroundSpeed.material        = Argus_indication_font	
txtgroundSpeed.parent_element  = "Common_origin"
txtgroundSpeed.alignment       = "RightCenter"
txtgroundSpeed.init_pos			= {horzPos(91), vertPos(93), 0}
txtgroundSpeed.stringdefs      = {0.0035,0.0035, 0.00, 0}  -- {size vertical, size horizontal, horizontal spacing, 0}
txtgroundSpeed.formats         = {"%.0f KTS"} 
txtgroundSpeed.controllers     = {{"text_using_parameter",0,0}}
txtgroundSpeed.element_params = {"ARGUS_GndSpd"}
txtgroundSpeed.h_clip_relation    = h_clip_relations.COMPARE
txtgroundSpeed.level			  = WINDOW_LEVEL
Add(txtgroundSpeed)

local txtMagHeading           = CreateElement "ceStringPoly"
txtMagHeading.name            = "txtMagHeading"
txtMagHeading.material        = Argus_indication_font	
txtMagHeading.parent_element  = "Common_origin"
txtMagHeading.alignment       = "CenterCenter"
txtMagHeading.init_pos			= {0, vertPos(87), 0}
txtMagHeading.stringdefs      = {0.0035,0.0035, 0.00, 0}  -- {size vertical, size horizontal, horizontal spacing, 0}
txtMagHeading.formats         = {"MAG\n%03.0f°"} 
txtMagHeading.controllers     = {{"text_using_parameter",0,0}}
txtMagHeading.element_params = {"MAG_HEADING"}
txtMagHeading.h_clip_relation    = h_clip_relations.COMPARE
txtMagHeading.level			  = WINDOW_LEVEL
Add(txtMagHeading)

------- Bottom Info Bar ------------------


boxX = horzPos(100)
boxY = vertPos(11)
local WindowMask2   = CreateElement "ceMeshPoly" 
WindowMask2.name 	= "WindowMask2"
WindowMask2.parent_element = "Common_origin"
WindowMask2.vertices = {{-boxX, boxY}, -- affects sizing
					{ boxX, boxY},
					{ boxX,-boxY},
					{-boxX,-boxY}}
WindowMask2.indices			= {0,1,2,2,3,0}
WindowMask2.tex_coords	 	= {{0,0},{1,0},{1,1},{0,1}}
WindowMask2.init_pos		= {0,vertPos(-89)}
WindowMask2.material		= MakeMaterial(nil,{205,5,5,150})	
WindowMask2.h_clip_relation = h_clip_relations.INCREASE_LEVEL  
WindowMask2.level			= DEFAULT_LEVEL
WindowMask2.isvisible		= false
--WindowMask2.element_params  = {""}  
--WindowMask2.controllers     = {{"parameter_in_range",0,-0.1,1.9}}
Add(WindowMask2)

local bottLine = addLine("bottomBarLine", horzPos(200), {horzPos(-100), vertPos(-78)}, -90)
bottLine.level = WINDOW_LEVEL
