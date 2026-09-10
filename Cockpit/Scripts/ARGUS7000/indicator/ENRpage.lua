dofile(LockOn_Options.script_path.."ARGUS7000/indicator/definitions.lua")

local ENRorigin          = CreateElement "ceSimple"
ENRorigin.name			  = "ENRorigin"
ENRorigin.element_params = {"ARGUS_Mode"}
ENRorigin.controllers 	  = {{"parameter_in_range",0,1}}
Add(ENRorigin)	


local txtModeScale          = CreateElement "ceStringPoly"
txtModeScale.name           = "txtModeScale"
txtModeScale.material       = Argus_indication_font	
txtModeScale.parent_element = "ENRorigin"
txtModeScale.alignment      = "RightCenter"
txtModeScale.init_pos		= {horzPos(91), vertPos(-93), 0}
txtModeScale.stringdefs     = {0.0035,0.0035, 0.00, 0}  -- {size vertical, size horizontal, horizontal spacing, 0}
txtModeScale.formats        = {"ENR/%.0f"} 
txtModeScale.element_params = {"ARGUS_Scale"}
txtModeScale.controllers    = {{"text_using_parameter",0,0}}
txtModeScale.h_clip_relation = h_clip_relations.COMPARE
txtModeScale.level			  = WINDOW_LEVEL
Add(txtModeScale)


addTexPoly("heli", {0,vertPos(-45)}, horzPos(13), heliSymbol, "ENRorigin", nil, nil)

------ Compass Ring ------------------------------------
local ENR_ringOrigin	       = CreateElement "ceSimple"
ENR_ringOrigin.name 		   = "ENR_ringOrigin"
ENR_ringOrigin.init_pos	   = {0.0, vertPos(-45), 0.0}
ENR_ringOrigin.parent_element = "ENRorigin"
ENR_ringOrigin.controllers    = {{"rotate_using_parameter", 0, degreeToRadian}}
ENR_ringOrigin.element_params = {"MAG_HEADING"}
Add(ENR_ringOrigin)

local compassRadius = horzPos(135)

for i = 0, 35 do
	local name
	local heading = i * 10
	local radius = compassRadius
	xpos = radius * math.sin(math.rad(heading))
	ypos = radius * math.cos(math.rad(heading))
	if i % 3 == 0 then -- every 30 deg has longer line
		if i % 9 == 0 then
			if i==0 then i="N" elseif i==9 then i="E" elseif i==18 then i="S" elseif i==27 then i="W" end
			local CardinalDir           = CreateElement "ceStringPoly"
			CardinalDir.name            = "CardinalDir"
			CardinalDir.material        = Argus_indication_font	
			CardinalDir.parent_element  = "ENR_ringOrigin"
			CardinalDir.alignment       = "CenterCenter"
			CardinalDir.init_pos		= {xpos,ypos,0}
			CardinalDir.stringdefs      = {0.0035,0.0035, 0.00, 0}  -- {size vertical, size horizontal, horizontal spacing, 0}
			CardinalDir.formats         = {"%.0f"} 
			CardinalDir.value			= i
			CardinalDir.element_params = {"MAG_HEADING"}
			CardinalDir.controllers     = {{"rotate_using_parameter", 0, -degreeToRadian}}
			Add_Argus_Element(CardinalDir)
		else
			addLine("long_compass_dash_"..heading, -horzPos(7), {xpos, ypos}, -heading, "ENR_ringOrigin",nil,nil,0.4,0.4)
		end
	else -- all others, short line
		addLine("short_compass_dash_"..heading, -horzPos(3.5), {xpos, ypos}, -heading, "ENR_ringOrigin",nil,nil,0.4,0.4)	
	end
end
---------------------------------------
--------- TACAN Symbol ------------------------------
local TCNorigin	         = CreateElement "ceSimple"
TCNorigin.name 		     = "TCNorigin"
TCNorigin.parent_element = "ENR_ringOrigin"
TCNorigin.element_params = {"TACAN_BEARING","TACAN_RANGE"}
TCNorigin.controllers    = {{"rotate_using_parameter",0,-(2*math.pi/360)},{"parameter_in_range",1,0,225}}
Add(TCNorigin)


addTexPoly("TACANsymbol_2", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/2},{"parameter_in_range",1,2}})
addTexPoly("TACANsymbol_5", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/5},{"parameter_in_range",1,5}})
addTexPoly("TACANsymbol_10", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/10},{"parameter_in_range",1,10}})
addTexPoly("TACANsymbol_15", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/15},{"parameter_in_range",1,15}})
addTexPoly("TACANsymbol_20", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/20},{"parameter_in_range",1,20}})
addTexPoly("TACANsymbol_30", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/30},{"parameter_in_range",1,30}})
addTexPoly("TACANsymbol_40", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/40},{"parameter_in_range",1,40}})
addTexPoly("TACANsymbol_60", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/60},{"parameter_in_range",1,60}})
addTexPoly("TACANsymbol_120", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/120},{"parameter_in_range",1,120}})
addTexPoly("TACANsymbol_240", nil, horzPos(8), tacanSymbol, "TCNorigin", {"TACAN_RANGE","ARGUS_Scale"}, {{"move_up_down_using_parameter",0, compassRadius/240},{"parameter_in_range",1,240}})

--[[
local numAirports = 50 --current max # of airports is Syria with 33
for i = 0,numAirports do
	local airport_ICAO			= CreateElement "ceStringPoly"
	airport_ICAO.name			= "airport_ICAO"..i
	airport_ICAO.material		= Argus_indication_font
	airport_ICAO.alignment		= "CenterCenter"
	airport_ICAO.formats		= {"%s"}
	airport_ICAO.stringdefs		= {0.004,0.004}
	airport_ICAO.parent_element = mapBase.name 
	--airport_ICAO.controllers 	= {{"airport_ICAO",i,1.0}}
	--Add_Argus_Element(airport_ICAO)
end--]]