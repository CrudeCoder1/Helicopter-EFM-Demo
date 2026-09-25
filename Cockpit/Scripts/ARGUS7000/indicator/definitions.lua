dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) --FOV, Milliradians, Meters
DEFAULT_LEVEL = 8
NOCLIP_LEVEL = DEFAULT_LEVEL - 1
WINDOW_LEVEL = DEFAULT_LEVEL+1

box_indices = { 0,1,2; 0,2,3 }

degreeToRadian = 0.0174533

--------- Materials ------------
argus7000_symbols_1 = "argus_symbols_1"
heliSymbol = MakeMaterial("ArgusHeliSymbol",ARGUS_Green)
tacanSymbol = MakeMaterial("ArgusTacanSymbol",ARGUS_Green)


ARGUS_Green = {10,230,10,220}

symbol_pixels_x =  44.0 * 2
symbol_pixels_y =  72.0 * 2
local font_desc = {
	texture     = LockOn_Options.script_path.."../Textures/Avionics/font_Argus7000.tga",
	size        = {7, 7},
	resolution  = {1024, 1024},
	default     = {symbol_pixels_x, symbol_pixels_y},
	chars	    = {

		 [1]   = {32, symbol_pixels_x, symbol_pixels_y}, -- [space]
		 [2]   = {42, symbol_pixels_x, symbol_pixels_y}, -- *
		 [3]   = {43, symbol_pixels_x, symbol_pixels_y}, -- +
		 [4]   = {45, symbol_pixels_x, symbol_pixels_y}, -- -
		 [5]   = {46, symbol_pixels_x, symbol_pixels_y}, -- .
		 [6]   = {47, symbol_pixels_x, symbol_pixels_y}, -- /
		 [7]   = {48, symbol_pixels_x, symbol_pixels_y}, -- 0
		 [8]   = {49, symbol_pixels_x, symbol_pixels_y}, -- 1
		 [9]   = {50, symbol_pixels_x, symbol_pixels_y}, -- 2
		 [10]  = {51, symbol_pixels_x, symbol_pixels_y}, -- 3
		 [11]  = {52, symbol_pixels_x, symbol_pixels_y}, -- 4
		 [12]  = {53, symbol_pixels_x, symbol_pixels_y}, -- 5
		 [13]  = {54, symbol_pixels_x, symbol_pixels_y}, -- 6
		 [14]  = {55, symbol_pixels_x, symbol_pixels_y}, -- 7
		 [15]  = {56, symbol_pixels_x, symbol_pixels_y}, -- 8
		 [16]  = {57, symbol_pixels_x, symbol_pixels_y}, -- 9
		 [17]  = {58, symbol_pixels_x, symbol_pixels_y}, -- :
		 [18]  = {65, symbol_pixels_x, symbol_pixels_y}, -- A
		 [19]  = {66, symbol_pixels_x, symbol_pixels_y}, -- B
		 [20]  = {67, symbol_pixels_x, symbol_pixels_y}, -- C
		 [21]  = {68, symbol_pixels_x, symbol_pixels_y}, -- D
		 [22]  = {69, symbol_pixels_x, symbol_pixels_y}, -- E
		 [23]  = {70, symbol_pixels_x, symbol_pixels_y}, -- F
		 [24]  = {71, symbol_pixels_x, symbol_pixels_y}, -- G
		 [25]  = {72, symbol_pixels_x, symbol_pixels_y}, -- H
		 [26]  = {73, symbol_pixels_x, symbol_pixels_y}, -- I
		 [27]  = {74, symbol_pixels_x, symbol_pixels_y}, -- J
		 [28]  = {75, symbol_pixels_x, symbol_pixels_y}, -- K
		 [29]  = {76, symbol_pixels_x, symbol_pixels_y}, -- L
		 [30]  = {77, symbol_pixels_x, symbol_pixels_y}, -- M
		 [31]  = {78, symbol_pixels_x, symbol_pixels_y}, -- N
		 [32]  = {79, symbol_pixels_x, symbol_pixels_y}, -- O
		 [33]  = {80, symbol_pixels_x, symbol_pixels_y}, -- P
		 [34]  = {81, symbol_pixels_x, symbol_pixels_y}, -- Q
		 [35]  = {82, symbol_pixels_x, symbol_pixels_y}, -- R
		 [36]  = {83, symbol_pixels_x, symbol_pixels_y}, -- S
		 [37]  = {84, symbol_pixels_x, symbol_pixels_y}, -- T
		 [38]  = {85, symbol_pixels_x, symbol_pixels_y}, -- U
		 [39]  = {86, symbol_pixels_x, symbol_pixels_y}, -- V
		 [40]  = {87, symbol_pixels_x, symbol_pixels_y}, -- W
		 [41]  = {88, symbol_pixels_x, symbol_pixels_y}, -- X
		 [42]  = {89, symbol_pixels_x, symbol_pixels_y}, -- Y
		 [43]  = {90, symbol_pixels_x, symbol_pixels_y}, -- Z
		 [44]  = {91, symbol_pixels_x, symbol_pixels_y}, -- [
		 [45]  = {93, symbol_pixels_x, symbol_pixels_y}, -- ]
		 [46]  = {62, symbol_pixels_x, symbol_pixels_y}, -- >
		 [47]  = {176, symbol_pixels_x, symbol_pixels_y}, -- degree °
		 [48]  = {94 ,  symbol_pixels_x, symbol_pixels_y}} -- ^
}

Argus_indication_font 	= MakeFont(font_desc,ARGUS_Green,"Argus_indication_font")

stringdefSmall = {0.0031*0.8, 0.0031, 0.00, 0}
stringdefMedium = {0.004*0.8, 0.004, 0.00, 0}

----------- Functions -------------
function vertPos(percent)
	local pos = (percent/100)*GetHalfHeight()
	return pos
end
function horzPos(percent)
	local pos = (percent/100)*GetHalfWidth()
	return pos
end

function Add_Argus_Element(object, elementParams, controllers)
	elementParams, controllers = addBrightness(elementParams, controllers)
	object.use_mipfilter    = true
	object.h_clip_relation  = h_clip_relations.COMPARE
	object.level			= DEFAULT_LEVEL
	object.additive_alpha   = true --additive blending
	object.collimated 		= false
	object.element_params 	= elementParams
	object.controllers    	= controllers
	Add(object)
	return object
end

-- note:"opacity_using_parameter" does not work well with colored ceTexPoly
function addBrightness(elementParams, controllers)
	if elementParams and controllers then
		elementParams[#elementParams+1]="ARGUS_Brightness"
		controllers[#controllers+1]={"opacity_using_parameter",#elementParams-1}
	else
		elementParams = {"ARGUS_Brightness"}
		controllers = {{"opacity_using_parameter",0}}
	end
	return elementParams, controllers
end

-- line
-- rot (CCW in degrees from up)
-- pos (position of beginning of the line)
function addLine(name, length, pos, rot, parent, controllers, material, _thickness, _fuzziness)
	elementParams, controllers = addBrightness(elementParams, controllers)
	local line      	= CreateElement "ceSMultiLine"
	line.name           = name
	line.material       = material or MakeMaterial(nil,ARGUS_Green)
	line.additive_alpha	= true
	line.use_mipfilter  = true
	line.parent_element = parent
	line.h_clip_relation= h_clip_relations.COMPARE
	line.level 		    = DEFAULT_LEVEL
	line.element_params = elementParams
	line.controllers    = controllers
	pos = pos or {0, 0}
	line.init_pos       	  = {pos[1], pos[2], 0}
	if rot ~= nil then
		line.init_rot   = {rot}
	end
		
	line.vertices   = {{0, 0}, {0, length}}
	line.indices    = {0, 1}	
	line.thickness  = _thickness or 1
	line.fuzziness  = _fuzziness or 0.75
	Add(line)
	return line
end

function addTexPoly(name, pos, size, material, parent, elementParams, controllers)
	elementParams, controllers = addBrightness(elementParams, controllers)
	local tex          = CreateElement "ceTexPoly"
	tex.name           = name
	tex.material       = material
	tex.vertices   	   = {{-size, size}, -- affects sizing (4 corners of tga file)
					  { size, size},
					  { size,-size},
					  {-size,-size}}
	tex.indices		   = {0,1,2,2,3,0}
	tex.tex_coords	   = {{0,0},{1,0},{1,1},{0,1}}
	tex.init_pos       = pos
	tex.alignment      = "CenterCenter"
	tex.element_params = elementParams
	tex.controllers    = controllers 
	tex.parent_element = parent
	tex.use_mipfilter 	= true
	tex.additive_alpha 	= false
	tex.collimated		= false
	tex.h_clip_relation = h_clip_relations.COMPARE
	tex.level			= DEFAULT_LEVEL 
	Add(tex)
	return tex
end

function addText(name, parent, pos, format, elementParams, controllers, level, alignment, stringdef, value)
	elementParams, controllers = addBrightness(elementParams, controllers)
	pos = pos or {0, 0}
	local txt           = CreateElement "ceStringPoly"
	txt.name            = name
	txt.material        = Argus_indication_font	
	txt.parent_element  = parent
	txt.alignment       = alignment or "CenterCenter"
	txt.init_pos		= pos
	txt.stringdefs      = stringdef or stringdefMedium  -- {size vertical, size horizontal, horizontal spacing, 0}
	txt.formats         = format
	txt.controllers     = controllers
	txt.element_params  = elementParams
	txt.h_clip_relation = h_clip_relations.COMPARE
	txt.level			= level or DEFAULT_LEVEL
	txt.value			= value
	Add(txt)
	return txt
end