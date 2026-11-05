dofile(LockOn_Options.common_script_path.."elements_defs.lua")
dofile(LockOn_Options.common_script_path.."Fonts/symbols_locale.lua")

SetScale(METERS) --FOV, Milliradians, Meters
DEFAULT_LEVEL = 8
NOCLIP_LEVEL = DEFAULT_LEVEL - 1
WINDOW_LEVEL = DEFAULT_LEVEL+1

box_indices = { 0,1,2; 0,2,3 }

degreeToRadian = 0.0174533

--------- Materials ------------
TRIMBLE_Green = {10,230,10,220}

symbol_pixels_x =  85
symbol_pixels_y =  128--108
local fontMatrix_desc = {
	texture     = LockOn_Options.script_path.."../Textures/Avionics/AH6J_LED_MATRIX_FONT.tga",
	size        = {8, 12},-- rows, columns
	resolution  = {1024, 1024},
	default     = {symbol_pixels_x, symbol_pixels_y},
	chars	    = {

		 [1]   = {latin['A'], symbol_pixels_x, symbol_pixels_y},
		 [2]   = {latin['B'], symbol_pixels_x, symbol_pixels_y},
		 [3]   = {latin['C'], symbol_pixels_x, symbol_pixels_y},
		 [4]   = {latin['D'], symbol_pixels_x, symbol_pixels_y},
		 [5]   = {latin['E'], symbol_pixels_x, symbol_pixels_y},
		 [6]   = {latin['F'], symbol_pixels_x, symbol_pixels_y},
		 [7]   = {latin['G'], symbol_pixels_x, symbol_pixels_y},
		 [8]   = {latin['H'], symbol_pixels_x, symbol_pixels_y},
		 [9]   = {latin['I'], symbol_pixels_x, symbol_pixels_y},
		 [10]  = {latin['J'], symbol_pixels_x, symbol_pixels_y},
		 [11]  = {latin['K'], symbol_pixels_x, symbol_pixels_y},
		 [12]  = {latin['L'], symbol_pixels_x, symbol_pixels_y},
		 [13]  = {latin['M'], symbol_pixels_x, symbol_pixels_y},
		 [14]  = {latin['N'], symbol_pixels_x, symbol_pixels_y},
		 [15]  = {latin['O'], symbol_pixels_x, symbol_pixels_y},
		 [16]  = {latin['P'], symbol_pixels_x, symbol_pixels_y},
		 [17]  = {latin['Q'], symbol_pixels_x, symbol_pixels_y},
		 [18]  = {latin['R'], symbol_pixels_x, symbol_pixels_y},
		 [19]  = {latin['S'], symbol_pixels_x, symbol_pixels_y},
		 [20]  = {latin['T'], symbol_pixels_x, symbol_pixels_y},
		 [21]  = {latin['U'], symbol_pixels_x, symbol_pixels_y},
		 [22]  = {latin['V'], symbol_pixels_x, symbol_pixels_y},
		 [23]  = {latin['W'], symbol_pixels_x, symbol_pixels_y},
		 [24]  = {latin['X'], symbol_pixels_x, symbol_pixels_y},
		 [25]  = {latin['Y'], symbol_pixels_x, symbol_pixels_y},
		 [26]  = {latin['Z'], symbol_pixels_x, symbol_pixels_y},
		 [27]  = {symbol['0'], symbol_pixels_x, symbol_pixels_y},
		 [28]  = {symbol['1'], symbol_pixels_x, symbol_pixels_y},
		 [29]  = {symbol['2'], symbol_pixels_x, symbol_pixels_y},
		 [30]  = {symbol['3'], symbol_pixels_x, symbol_pixels_y},
		 [31]  = {symbol['4'], symbol_pixels_x, symbol_pixels_y},
		 [32]  = {symbol['5'], symbol_pixels_x, symbol_pixels_y},
		 [33]  = {symbol['6'], symbol_pixels_x, symbol_pixels_y},
		 [34]  = {symbol['7'], symbol_pixels_x, symbol_pixels_y},
		 [35]  = {symbol['8'], symbol_pixels_x, symbol_pixels_y},
		 [36]  = {symbol['9'], symbol_pixels_x, symbol_pixels_y},
		 [37]  = {symbol[' '], symbol_pixels_x, symbol_pixels_y},
		 [38]  = {symbol['_'], symbol_pixels_x, symbol_pixels_y},
		 [39]  = {symbol['['], symbol_pixels_x, symbol_pixels_y},
		 [40]  = {symbol[']'], symbol_pixels_x, symbol_pixels_y},
		 [41]  = {symbol['°'], symbol_pixels_x, symbol_pixels_y},
		 [42]  = {symbol['-'], symbol_pixels_x, symbol_pixels_y},
		 [43]  = {symbol['<'], symbol_pixels_x, symbol_pixels_y},
		 [44]  = {symbol['>'], symbol_pixels_x, symbol_pixels_y},
		 [45]  = {symbol['@'], symbol_pixels_x, symbol_pixels_y},-- down arrow
		 [46]  = {symbol['|'], symbol_pixels_x, symbol_pixels_y},
		 [47]  = {symbol['*'], symbol_pixels_x, symbol_pixels_y},
		 [48]  = {symbol[':'], symbol_pixels_x, symbol_pixels_y},
		 [49]  = {latin['t'], symbol_pixels_x, symbol_pixels_y},-- TK
		 [50]  = {latin['k'], symbol_pixels_x, symbol_pixels_y},-- KT
		 [51]  = {symbol['`'], symbol_pixels_x, symbol_pixels_y},--NM
		 [52]  = {latin['o'], symbol_pixels_x, symbol_pixels_y},--TO
		 [53]  = {symbol['.'], symbol_pixels_x, symbol_pixels_y},
		 [54]  = {symbol['='], symbol_pixels_x, symbol_pixels_y},--right arrow
		 [55]  = {symbol['~'], symbol_pixels_x, symbol_pixels_y},--KF
		 [56]  = {latin['d'], symbol_pixels_x, symbol_pixels_y},--DT
		 [57]  = {latin['f'], symbol_pixels_x, symbol_pixels_y},--Fr
		 [58]  = {latin['l'], symbol_pixels_x, symbol_pixels_y},--AT
		 [59]  = {latin['z'], symbol_pixels_x, symbol_pixels_y},
		 [60]  = {latin['a'], symbol_pixels_x, symbol_pixels_y},
		 [61]  = {latin['m'], symbol_pixels_x, symbol_pixels_y},
		 [62]  = {latin['v'], symbol_pixels_x, symbol_pixels_y},
		 [63]  = {latin['n'], symbol_pixels_x, symbol_pixels_y},
		 [64]  = {latin['i'], symbol_pixels_x, symbol_pixels_y},
		 [65]  = {latin['u'], symbol_pixels_x, symbol_pixels_y},
		 [66]  = {symbol['('], symbol_pixels_x, symbol_pixels_y},
		 [67]  = {symbol[')'], symbol_pixels_x, symbol_pixels_y},
		 [68]  = {symbol['!'], symbol_pixels_x, symbol_pixels_y},-- CDI "needle"
		}
}

TrimbleFont = MakeFont(fontMatrix_desc, TRIMBLE_Green, "TrimbleFont")


stringdefDefault = {0.007, 0.007*0.65, -0.0003, 0}-- {size vertical, size horizontal, horizontal spacing, vert spacing}

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
	line.material       = material or MakeMaterial(nil,TRIMBLE_Green)
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
	txt.material        = TrimbleFont	
	txt.parent_element  = parent
	txt.alignment       = alignment or "CenterCenter"
	txt.init_pos		= pos
	txt.stringdefs      = stringdef or stringdefDefault  -- {size vertical, size horizontal, horizontal spacing, 0}
	txt.formats         = format
	txt.controllers     = controllers
	txt.element_params  = elementParams
	txt.h_clip_relation = h_clip_relations.COMPARE
	txt.level			= level or DEFAULT_LEVEL
	txt.value			= value
	Add(txt)
	return txt
end