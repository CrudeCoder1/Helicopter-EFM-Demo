dofile(LockOn_Options.common_script_path.."Fonts/symbols_locale.lua")
dofile(LockOn_Options.common_script_path.."Fonts/fonts_cmn.lua")

-------MATERIALS-------
materials = {}   

materials["DIGIT_GREEN"]		= {0,255,0,215}
materials["DIGIT_ORANGE"]		= {255,40,20,220}
materials["RWR_GREEN"]			= {0,255,0,255}


-------TEXTURES-------
textures = {}
--textures["Clock_line_material"]	= {nil, materials["DIGIT_GREEN"]}
--textures["argus_symbols_1"]	= {LockOn_Options.script_path.."../Textures/Avionics/Argus7000_Symbols.tga", 	materials["DIGIT_GREEN"]}
textures["RWR_Material"]	= {nil, materials["RWR_GREEN"]}


-------FONTS----------
fontdescription = {}
--fontdescription["font_general_loc"]	= fontdescription_cmn["font_general_loc"]
--[[ not using this font method, because it creates artifacts at the edges of the digits
local segmentDigit_x = 144--#pixels wide
local segmentDigit_y = 248
fontdescription["font_7seg"] = {
	texture		= LockOn_Options.script_path.."../Textures/Avionics/font7segment.tga",
	size      = {4, 4},--# of items across and down
	resolution = {1024, 1024},
	default    = {segmentDigit_x, segmentDigit_y},
	chars	   = {
		[1] = {symbol[' '], segmentDigit_x, segmentDigit_y}, -- SPACE
		[2] = {symbol['0'], segmentDigit_x, segmentDigit_y}, -- 0
		[3] = {symbol['1'], segmentDigit_x, segmentDigit_y}, -- 1
		[4] = {symbol['2'], segmentDigit_x, segmentDigit_y}, -- 2
		[5] = {symbol['3'], segmentDigit_x, segmentDigit_y}, -- 3
		[6] = {symbol['4'], segmentDigit_x, segmentDigit_y}, -- 4
		[7] = {symbol['5'], segmentDigit_x, segmentDigit_y}, -- 5
		[8] = {symbol['6'], segmentDigit_x, segmentDigit_y}, -- 6
		[9] = {symbol['7'], segmentDigit_x, segmentDigit_y}, -- 7
		[10] = {symbol['8'], segmentDigit_x, segmentDigit_y}, -- 8
		[11] = {symbol['9'], segmentDigit_x, segmentDigit_y}, -- 9 
		[12] = {symbol[':'], segmentDigit_x, segmentDigit_y}, -- :		
	} 
}
--[[
local symbol_pixels_x =   88 
local symbol_pixels_y =  144
fontdescription["font_RWR"]  = {
	texture     = LockOn_Options.script_path.."IndicationTextures/font_RWR_AH6J.tga",
	size        = {7, 7},
	resolution  = {1024, 1024},
	default     = {symbol_pixels_x, symbol_pixels_y},
	chars	    = {
		 [1]   = {32, symbol_pixels_x, symbol_pixels_y}, -- [space]
		 [2]   = {42, symbol_pixels_x, symbol_pixels_y}, -- *
		 [3]   = {43, symbol_pixels_x, symbol_pixels_y}, -- +
		 [4]   = {45, symbol_pixels_x, symbol_pixels_y}, -- -
		 [5]   = {59, symbol_pixels_x, symbol_pixels_y}, -- ;
		 [6]   = {46, symbol_pixels_x, symbol_pixels_y}, -- .
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
		 [18]  = {65, 120			 , symbol_pixels_y}, -- Airplane symbol ("A")
		 [19]  = {66, symbol_pixels_x, symbol_pixels_y}, -- B
		 [20]  = {67, symbol_pixels_x, symbol_pixels_y}, -- C
		 [21]  = {68, symbol_pixels_x, symbol_pixels_y}, -- D
		 [22]  = {69, symbol_pixels_x, symbol_pixels_y}, -- E
		 [23]  = {70, symbol_pixels_x, symbol_pixels_y}, -- F
		 [24]  = {71, 105			, symbol_pixels_y}, -- Gun Symbol "G"
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
		 [43]  = {90, 105			, symbol_pixels_y}, -- Special Gun Symbol "Z"
	}
}
--]]

fonts = {}
-- GENERAL FONTS
--fonts["font_general_keys"]		= {fontdescription["font_general_loc"], 10, {255,75,75,255}}
--fonts["font_hints_kneeboard"]	= {fontdescription["font_general_loc"], 10, {100,0,100,255}}

font_DHI = MakeFont(fontdescription["font_7seg"],materials["DIGIT_GREEN"],"font_DHI")


fonts["font_Clock"]				= {fontdescription["font_7seg"], 10, materials["DIGIT_GREEN"]}
--fonts["font_DHI"]				= {fontdescription["font_7seg"], 10, materials["DIGIT_GREEN"]}
fonts["font_VID"]				= {fontdescription["font_7seg"], 10, materials["DIGIT_GREEN"]}
fonts["font_7segment_orange"]	= {fontdescription["font_7seg"], 10, materials["DIGIT_ORANGE"]}

fonts["RWR_indication_font"]	= {fontdescription["font_RWR"], 10, materials["RWR_GREEN"]}

