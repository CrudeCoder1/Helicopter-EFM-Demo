
AH6 = {
	Name										=	'AH-6',
	Picture										=	'AH-6.png',	-- Mission editor loadout picture
	DisplayName									=	_('AH-6J'),
	
	shape_table_data 	= {
		{
			file  	    = 'AH-6';
			username    = 'AH-6';
			desrt		= 'AH-6_destr';
			index       =  WSTYPE_PLACEHOLDER;
			life  	    = 16; --   The strength of the object (ie. lifebar *)
			vis   	    = 3; -- Visibility factor (For a small objects is better to put lower nr).
			fire  	    = { 300, 2}; -- Fire on the ground after destoyed: 300sec 4m
			classname   = "lLandPlane";
			positioning = "BYNORMAL";
		},
		{
			name  ="AH-6_destr";
			file  ="AH-6_destr";
			fire  = { 240, 2};
		},
	},

	mapclasskey 		= "P0091000021", -- found in MissionEditor/data/NewMap/images/themes/nato/P91000021.png   gives map symbol A
	attribute  			= {wsType_Air, wsType_Helicopter, wsType_Battleplane, WSTYPE_PLACEHOLDER ,"Attack helicopters",},
	Categories 			= {},
	Rate 				= 30,  -- RewardPoint in Multiplayer
	Countries 			= {"USA"},
	-------------- Aircraft Physical properties -----------
	length				=	9.8, -- meters
	height				=	3.0,-- meters
	rotor_RPM			=	470,
	M_empty				=	722, --kg
	M_nominal			=	1200, --kg
	M_max				=	1610, --kg		
	M_fuel_max			=	362, -- kg	1 litre = 0.79 kg	62 gal (~185kg) main tank, + 62 gal internal backseat tank (60 usable)
	RCS					=	3, -- Radar Cross Section m^2
	IR_emission_coeff	=	0.3, -- 1 is IR emission of Su-27

	------------------ AI defs ------------------------------
	V_max				=	78.3, -- m/s max speed for AI
	V_max_cruise		=	70,	-- cruise speed
	Vy_max				=	10.5, --Max climb speed in m/s 
	H_stat_max_L		=	5070,
	H_max 				=	5500, --km, max operation height
	H_stat_max			=	3040,
	H_din_two_eng		=	5500,
	H_din_one_eng		=	5500,
	range				=	430, --km, for AI
	flight_time_typical	=	140,
	flight_time_maximum	=	189,
	Vy_land_max			=	12.8, -- landing speed
	Ny_max				=	3.5, --max G for AI
	Sensors = {	-- defines what the AI can use in terms of sensors
           OPTIC = {"TADS DVO"}, -- AI can engage enemy at night
           RWR = "Abstract RWR"
	},
	--CanopyGeometry = makeHelicopterCanopyGeometry(LOOK_AVERAGE, LOOK_AVERAGE, LOOK_AVERAGE),
	 CanopyGeometry = {
           azimuth   = {-100.0, 120.0}, -- pilot view horizontal (AI)
           elevation = {-50.0, 110.0} -- pilot view vertical (AI)
       },
	-------------------------------------------------------------------		
	nose_gear_pos 		= { 1.490,	-1.822,	0}, -- used for starting placement on ground {forward/back,up/down,left/right}
	main_gear_pos 		= { -0.612,	-1.76,	0.964},
	---------------------- unknown ---------------------------------------------
	lead_stock_main		=	-0.1,
	lead_stock_support	=	-0.1,
	scheme				=	0,
	fire_rate			=	625,
	cannon_sight_type	=	0,
	------------ AI flight model probably (doesnt affect human FM)---------------
	rotor_height		=	2.7,-- meters
	rotor_diameter		=	19,--8.33, --m
	blade_chord			=	0.171,
	blades_number		=	5,
	blade_area			=	0.712, --m^2
	fuselage_Cxa0		=	0.4,
	fuselage_Cxa90		=	3,
	fuselage_area		=	1.4, 
	centering			=	0,
	tail_pos 			= 	{-4.564,	0.163,	0},
	tail_fin_area		=	0.467,
	tail_stab_area		=	0.66,
	thrust_correction	=	0.55,
	rotor_MOI			=	1500,
	rotor_pos 			= 	{0,	0.86, 0},


	engines_count		=	1,
	engines_nozzles = 
	{
		{
		pos     		= {-1.8,-0.565, 0}, -- important for heatblur effect
		diameter      = 0.13, 
		engine_number = 1,
		--exhaust_length_ab	=	1,
		--exhaust_length_ab_K	=	0.76,
		smokiness_level = 0.1,
		}
	},
		
	crew_size = 2,
	crew_members = 
	{
			[1] =
			{	ejection_seat_name	=	0, -- name of object file used for pilot ejection
				drop_canopy_name	=	0, -- name of object file used for canopy jettison
				pos = 	{0.725, 0.14, 0.7}, -- used for ejection location	
				ejection_order    = 1,
				can_be_playable  = true,	
				role = "pilot",
				role_display_name = _("Pilot"), 
			},
			[2] =
			{	ejection_seat_name	=	0,
				drop_canopy_name	=	0,
				pos = 	{0.725, 0.14, -0.7},
				ejection_order    = 2,
				--pilot_body_arg  = 501,
				can_be_playable  = true,	
				role = "instructor",
				role_display_name = _("Instructor pilot"), 
			},
	},

	fires_pos =
		{ 
			[1] = 	{-1.8,	-0.55,	0}, -- turbine exit
		},		

	Pylons = {
        pylon(1, 0, 0.241, -0.926, -1.55,	-- (Pylon #, ext wing=0(no ejection)/ext fuselage=1/internal bay=2, forward/back, up/down, left/right)
            {use_full_connector_position = true, connector = "Pylon1",},
            {
				{CLSID = "{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}", arg_value = 0.0},	-- LAU-68-M151 High Explosive *7
				{CLSID = "{4F977A2A-CD25-44df-90EF-164BFA2AE72F}", arg_value = 0.0},	-- LAU-68-MK156 White Phosphorus *7
				{CLSID = "{FD90A1DC-9147-49FA-BF56-CB83EF0BD32B}", arg_value = 0.0},	-- LAU-61 2.75" rockets MK151 HE *19
				{CLSID = "{AH6_GAU-19}", arg_value = 0.0,								-- .50 cal gun
					forbidden = {{station = 2, loadout = {"{M134 Minigun}"}},}}, -- prohibits m134 from being mounted when gau-19 is mounted
            }
        ),
        pylon(2, 0, 0.241, -0.926, -0.945,
            {use_full_connector_position = true, connector = "Pylon2",},
            {
                {CLSID = "{M134 Minigun}", arg_value = 0.0}, -- .308 cal gun
            }
        ),
		pylon(3, 0, 0.0, 0.0, 0.0,
            {
			arg				= 310,
			arg_value		= 0,
			DisplayName = "Plank",
			},
            {
				{CLSID = "<CLEAN>", arg_value = 1, forbidden = {
				{station = 1, loadout = {"{AH6_GAU-19}"}},
				{station = 1, loadout = {"{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}"}},
				{station = 1, loadout = {"{4F977A2A-CD25-44df-90EF-164BFA2AE72F}"}},
				{station = 1, loadout = {"{FD90A1DC-9147-49FA-BF56-CB83EF0BD32B}"}},
				{station = 2, loadout = {"{M134 Minigun}"}},
				{station = 4, loadout = {"{M134 Minigun}"}},
				{station = 5, loadout = {"{AH6_GAU-19}"}},
				{station = 5, loadout = {"{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}"}},
				{station = 5, loadout = {"{4F977A2A-CD25-44df-90EF-164BFA2AE72F}"}},
				{station = 5, loadout = {"{FD90A1DC-9147-49FA-BF56-CB83EF0BD32B}"}},
				}},
            }
        ),
        pylon(4, 0, 0.241, -0.926, 0.945,
            {use_full_connector_position = true, connector = "Pylon3",DisplayName = "3",},
            {
				{CLSID = "{M134 Minigun}", arg_value = 0.0}, -- .308 cal gun
            }
        ),
        pylon(5, 0, 0.241, -0.926, 1.55,
            {use_full_connector_position = true, connector = "Pylon4",DisplayName = "4",},
            {
                {CLSID = "{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}", arg_value = 0.0},	-- LAU-68-M151 High Explosive *7
				{CLSID = "{4F977A2A-CD25-44df-90EF-164BFA2AE72F}", arg_value = 0.0},	-- LAU-68-MK156 White Phosphorus *7
				{CLSID = "{FD90A1DC-9147-49FA-BF56-CB83EF0BD32B}", arg_value = 0.0},	-- LAU-61 2.75" rockets MK151 HE *19
				{CLSID = "{AH6_GAU-19}", arg_value = 0.0,								-- .50 cal gun
					forbidden = {{station = 4, loadout = {"{M134 Minigun}"}},}}, 
            }
        ),
    },
		
	Tasks = { 	-- defined in db_units_planes.lua
        aircraft_task(CAS), 	--31
        aircraft_task(GroundAttack), --32
        aircraft_task(AFAC),
        aircraft_task(Reconnaissance),
    },	
	DefaultTask = aircraft_task(CAS),
	
	LandRWCategories = 	-- adds these takeoff and landing options avaliable in mission editor
    {	[1] = 
        {
           Name = "HelicopterCarrier",
        },
        [2] = 
        {
           Name = "AircraftCarrier",
        },
    },
	TakeOffRWCategories = 
    {	[1] = 
        {
            Name = "HelicopterCarrier",
        },
        [2] = 
        {
           Name = "AircraftCarrier",
        },
    },
	
	Damage = verbose_to_dmg_properties( --index meaning see in Scripts\Aircrafts\_Common\Damage.lua
	{		-- deps_cells defines what other parts get destroyed along with it
		["ROTOR"]			= {critical_damage = 3, args = {160}, deps_cells = {"BLADE_1_IN", "BLADE_2_IN", "BLADE_3_IN"}},
		["BLADE_1_IN"]		= {critical_damage = 1, args = {161}}, -- shell
		["BLADE_1_CENTER"]	= {critical_damage = 1, args = {161}}, -- line
		["BLADE_2_IN"]		= {critical_damage = 1, args = {162}},
		["BLADE_2_CENTER"]	= {critical_damage = 1, args = {162}},
		["BLADE_3_IN"]		= {critical_damage = 1, args = {163}},
		["BLADE_3_CENTER"]	= {critical_damage = 1, args = {163}},
		["BLADE_4_IN"]		= {critical_damage = 1, args = {164}},
		["BLADE_4_CENTER"]	= {critical_damage = 1, args = {164}},
		["BLADE_5_IN"]		= {critical_damage = 1, args = {165}},
		["BLADE_5_CENTER"]	= {critical_damage = 1, args = {165}},
		
		["MAIN"]  			= {critical_damage = 10, args = {151}}, 
		
		["WING_L"]			= {critical_damage = 5, args = {156}}, -- left skid shell
		["ELEVATOR_L_OUT"]	= {critical_damage = 1, args = {156}}, -- left skid line F
		["Line_STABIL_L"]	= {critical_damage = 1, args = {156}}, -- left skid line R
		["WING_R"]			= {critical_damage = 5, args = {157}}, -- right skid shell
		["ELEVATOR_R_OUT"]	= {critical_damage = 1, args = {157}}, -- right skid line F
		["Line_STABIL_R"]	= {critical_damage = 1, args = {157}}, -- right skid line R
		
		["TAIL"]			= {critical_damage = 4, args = {159}, deps_cells = {"BLADE_6_OUT"}}, 
		["BLADE_6_OUT"]		= {critical_damage = 1, args = {166}}, -- tail rotor blades
		
		["CREW_1"]			= {critical_damage = 2, args = {205}}, -- pilot
		["CREW_2"]			= {critical_damage = 2, args = {204}}, -- copilot
		
		["FRONT"]			= {critical_damage = 1, args = {170}}, -- glass canopy
		["COCKPIT"]			= {critical_damage = 1, args = {149}},
		["FUEL_TANK_B"]		= {critical_damage = 1, args = {150}}, -- backseat tank
		["FUEL_TANK_F"]		= {critical_damage = 1, args = {152}}, -- main tank (inside bottom of fuselage)

	}),
	
	Failures = { -- not working yet
		{ id = 'engfail',	label = _('ENGINE'),	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
		{ id = 'rotor',		label = _('ROTOR'),		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
	},

	DamageParts = {	-- parts that fall off when aircraft is hit or crashes
		[1] = "AH-6_Tail",  --wing right
		[2] = "AH-6_Skid",  --wing left
		--[3] = "",    --nose part
		[4] = "AH-6_Skid",  -- tail part
		[5] = "AH-6_Rotor",	--blade
	},
	
	lights_data = {
	typename = "collection",
	lights = {
    [1] = { typename = "collection",
				lights = {
						--[1] = {typename = "omnilight", connector = "RED_BEACON", color = {0.99, 0.11, 0.3}, pos_correction  = {0, 0, 0}, intensity_max = 6.0},
				}
			},
	[2] = { typename = "collection",
				lights = {
						{typename  = "argumentlight",	argument  = 208},--landing light
				}
			},
    [3]	= {	typename = "collection",
				lights = {
						{typename = "argumentlight", argument  = 190},-- navlights
				}
			},
    [4] = {	typename = "collection",
				lights = {
						--[1] = {typename  = "argumentlight", argument = 200},
				}
			},
		}
	},
		
	net_animation = {   --transmits draw arguments over multiplyer for others to see
					9, --collective
					11, -- stick roll
					15, --stick pitch
					37, -- main rotor spin
					40,-- tail rotor spin
					156, --skid visibility
					157, --skid
					159,--tail visibility
					160, --rotor vis
					190, --nav lights
					208, --landing light
					500, --pedals
					501,--ammo box vis
					502,--ammo box vis
					--1000,--red cover test
					},
		
	--sound_name = "Rotor", -- rotor sound from Sounds/sdef

	engine_data = 
	{  -- most of these are unknown right now, but they are only for AI
			power_take_off	=	473,
			power_max	=	473,
			power_WEP	=	473,
			power_TH_k = 
			{
				[1] = 	{0,	-230.8,	2245.6},
				[2] = 	{0,	-230.8,	2245.6},
				[3] = 	{0,	-325.4,	2628.9},
				[4] = 	{0,	-235.6,	1931.9},
			},
			SFC_k = 	{2.045e-007,	-0.0006328,	0.803},
			power_RPM_k = 	{-0.08639,	0.24277,	0.84175},
			power_RPM_min	=	9.1384,
			--sound_name	= "EngineTV3117", -- engine sound from Sounds/sdef
	},
					
	HumanRadio = {
			frequency = 124.0,
			editable = true,
			minFrequency = 118,
			maxFrequency = 143.975,
			modulation = MODULATION_AM
	},
		
	panelRadio = {		
			[1] = {
					name = _("FM Radio"),		   
					range = {{min = 30, max = 87.975}},
					channels = {
								[1] = { name = _("Channel 1"),		default = 30.0, modulation = _("FM")}, --, connect = true}, -- default
								[2] = { name = _("Channel 2"),		default = 31.0, modulation = _("FM")},
								[3] = { name = _("Channel 3"),		default = 32.0, modulation = _("FM")},
								[4] = { name = _("Channel 4"),		default = 33.0, modulation = _("FM")},
								[5] = { name = _("Channel 5"),		default = 40.0, modulation = _("FM")},
								[6] = { name = _("Channel 6"),		default = 41.0, modulation = _("FM")},
								[7] = { name = _("Channel 0"),		default = 42.0, modulation = _("FM")},
								[8] = { name = _("Channel RG"),		default = 50.0, modulation = _("FM")},
								}
					},
	},	
	
	AddPropAircraft = {
		{id = "SoloFlight", control = 'checkbox' , label = _('Solo Flight'), defValue = false, weightWhenOn = -80},
		{id = "NetCrewControlPriority" , control = 'comboList', label = _('Aircraft Control Priority'), playerOnly = true,
		  values = {{id =  0, dispName = _("Pilot")},
					{id =  1, dispName = _("Instructor")},
					{id = -1, dispName = _("Ask Always")},
					{id = -2, dispName = _("Equally Responsible")}},
		  defValue  = 1,
		  wCtrl     = 150
		},
	},
}

add_aircraft(AH6)
