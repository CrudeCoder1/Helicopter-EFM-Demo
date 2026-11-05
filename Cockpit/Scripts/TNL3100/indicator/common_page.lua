
Common_origin          		= CreateElement "ceSimple"
Common_origin.name			= "Common_origin"
Common_origin.element_params = {"TNL3100_Power"}
Common_origin.controllers 	= {{"parameter_in_range",0,1}}
Add(Common_origin)


addText("topRow", Common_origin.name, {0, vertPos(45)}, {"0|||||||||||||||||20"}, {"MAG_HEADING"}, {{"text_using_parameter",0,0}})
addText("botRow", Common_origin.name, {0, vertPos(-35)}, {"0|||||||||||||||||20"}, {"MAG_HEADING"}, {{"text_using_parameter",0,0}})
