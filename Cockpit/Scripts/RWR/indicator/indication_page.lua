dofile(LockOn_Options.script_path.."RWR/Indicator/definitions.lua")

contact_scale = 0.03
green_color			= MakeMaterial(nil,{0, 255, 5, 215})
GREEN_SOFT		= MakeMaterial(nil,{25, 255, 25, 128})
	
RWR_base          		= CreateElement "ceSimple"
RWR_base.name			= "RWR_base"
RWR_base.init_pos		= center
RWR_base.element_params = {"RWR_POWER"}
RWR_base.controllers 	= {{"parameter_in_range",0,1}}
Add(RWR_base)	
	

for ia = 1,20 do
	if ia  < 10 then
		i = "0"..ia
	else
		i = ia
	end
		
	local	RWR_contact			   			= CreateElement "ceSimple"
			RWR_contact.name		   		= "RWR_contact_" .. i .. "_name"
			RWR_contact.h_clip_relation 	= h_clip_relations.COMPARE
			RWR_contact.level 				= RWR_DEFAULT_LEVEL
			RWR_contact.controllers     	= {{"rotate_using_parameter",0,1},
											   {"move_up_down_using_parameter",1,contact_scale},
											   {"parameter_in_range",1,0.01,1.09},
											  } 
			RWR_contact.element_params  	= {"RWR_CONTACT_"..i.."_AZIMUTH", "RWR_CONTACT_"..i.."_POWER_ADJUSTED"}
			RWR_contact.parent_element 		= "RWR_base"
			Add(RWR_contact)
			

	x_size 	= 0.095	* RWR_SCALE	
	thickness = 0.85 
				
	local 	RWR_lock	 				= CreateElement "ceStringPoly"
			RWR_lock.name			  	= "RWR_"..i.."_lock"
			RWR_lock.material    		= green_color	
			RWR_lock.vertices	   		= {	{-x_size 			, x_size			},
											{-x_size 			,-x_size 			},
											{-x_size*thickness	, x_size*thickness	},
											{-x_size*thickness	,-x_size*thickness	},	
											
											{-x_size 			, x_size 			},
											{ x_size  			, x_size 			},
											{-x_size*thickness	, x_size*thickness	},
											{ x_size*thickness	, x_size*thickness	},	
													
											{ x_size 			, x_size			},
											{ x_size 			,-x_size 			},
											{ x_size*thickness	, x_size*thickness  },
											{ x_size*thickness	,-x_size*thickness	},	
												
											{ x_size 			,-x_size			},
											{-x_size 			,-x_size 			},
											{ x_size*thickness	,-x_size*thickness  },
											{-x_size*thickness	,-x_size*thickness	},	}		
			RWR_lock.indices	   		= 	{ 	0,1,2,		1,2,3,
												4,5,6,		5,6,7,
												8,9,10,		9,10,11,
												12,13,14,	13,14,15,}
			RWR_lock.element_params  	= {	"RWR_CONTACT_" .. i .. "_LOCK_SYM",
											"RWR_CONTACT_" .. i .. "_AZIMUTH",}
			RWR_lock.controllers     	= {	{"parameter_in_range",0,0.9,1.1},
											{"rotate_using_parameter",1,-1},} 
			RWR_lock.parent_element 	= "RWR_contact_" .. i .. "_name"
			RWR_lock.h_clip_relation 	= h_clip_relations.COMPARE
			RWR_lock.level 				= RWR_DEFAULT_LEVEL
			Add(RWR_lock)				
	

	x_size 	= 0.11	* RWR_SCALE	
	wmul = 0.76 
				
	local 	RWR_plane	 				= CreateElement "ceStringPoly"
			RWR_plane.name			  	= "RWR_"..i.."_plane"
			RWR_plane.material    		= green_color	
			RWR_plane.vertices	   		= {	{-x_size/2 		, x_size/2		},
											{0 				, x_size 		},
											{0				, x_size * wmul	},
											{-(x_size*wmul)/2, x_size/2		},	
												
											{ x_size/2 		, x_size/2		},
											{0 				, x_size 		},
											{0				, x_size * wmul	},
											{ (x_size*wmul)/2, x_size/2		},	
										 }		
			RWR_plane.indices	   		= { 0,1,2,	0,2,3,
											4,5,6,	4,6,7,}
			RWR_plane.element_params  	= {"RWR_CONTACT_" .. i .. "_GENERAL_TYPE", "RWR_CONTACT_" .. i .. "_AZIMUTH"}
			RWR_plane.controllers     	= {{"parameter_in_range",0,1}, {"rotate_using_parameter",1,-1}} 
			RWR_plane.parent_element 	= "RWR_contact_" .. i .. "_name"
			RWR_plane.h_clip_relation 	= h_clip_relations.COMPARE
			RWR_plane.level 			= RWR_DEFAULT_LEVEL
			Add(RWR_plane)				
			
			
	local RWR_contact_symbol           = CreateElement "ceStringPoly" -- if contact is not airplane use custom symbol
	RWR_contact_symbol.name            = "RWR_contact_"..i.."_symbol"
	RWR_contact_symbol.material        = MakeFont({used_DXUnicodeFontData = "font_cockpit_RWR"},{0,255,0,215})
	RWR_contact_symbol.alignment       = "CenterCenter"
	RWR_contact_symbol.stringdefs      = {0.006,0.5 * 0.006, 0, 0}  -- {size vertical, horizontal, 0, 0}
	RWR_contact_symbol.formats         = {"%s"} 
	RWR_contact_symbol.element_params  = {"RWR_CONTACT_" .. i .. "_GENERAL_TYPE", "RWR_CONTACT_" .. i .. "_AZIMUTH", "RWR_CONTACT_" .. i .. "_UNIT_TYPE_SYM"}
	RWR_contact_symbol.controllers     = {{"parameter_in_range",0,1.5,3.1}, {"rotate_using_parameter",1,-1},{"text_using_parameter",2,0}} 
	RWR_contact_symbol.parent_element  = "RWR_contact_" .. i .. "_name"	
	RWR_contact_symbol.h_clip_relation = h_clip_relations.COMPARE
	RWR_contact_symbol.level 		   = RWR_DEFAULT_LEVEL
	Add(RWR_contact_symbol)
end


radius = 0.025 * RWR_SCALE	
local   RWR_grid_i 				= CreateElement "ceMeshPoly"
	RWR_grid_i.name 			= "RWR_grid_i"
	set_circle	(RWR_grid_i, radius + 0.0020, radius - 0.0020, 360, 32)
	RWR_grid_i.material 		= GREEN_SOFT
	RWR_grid_i.h_clip_relation	= h_clip_relations.COMPARE
	RWR_grid_i.level			= RWR_DEFAULT_LEVEL
	RWR_grid_i.parent_element 	= "RWR_base"
	RWR_grid_i.element_params	= {"RWR_BRIGHTNESS"}
	RWR_grid_i.controllers 		= {{"opacity_using_parameter",0}}
	Add(RWR_grid_i)


radius = 0.35 * RWR_SCALE	
local   RWR_grid_m 				= CreateElement "ceMeshPoly"
	RWR_grid_m.name 			= "RWR_grid_m"
	set_circle	(RWR_grid_m, radius + 0.0020, radius - 0.0020, 360, 32)
	RWR_grid_m.material 		= GREEN_SOFT
	RWR_grid_m.h_clip_relation	= h_clip_relations.COMPARE
	RWR_grid_m.level			= RWR_DEFAULT_LEVEL
	RWR_grid_m.parent_element 	= "RWR_base"
	RWR_grid_m.element_params	= {"RWR_BRIGHTNESS"}
	RWR_grid_m.controllers 		= {{"opacity_using_parameter",0}}
	Add(RWR_grid_m)	
		