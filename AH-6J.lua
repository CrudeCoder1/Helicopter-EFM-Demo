local lbToKg = 0.453592
local ftToM = 0.3048
local knotToMpS = 0.514444
local knotToKMpH = 1.852

AH6J = {
	Name				=	'AH-6J', -- unique name internal to DCS
	Picture				=	'AH-6.png',	-- Rearming loadout window picture
	DisplayName			=	_('AH-6J'),-- actual name displayed in menus and in-game
	
	shape_table_data 	= {
		{
			file  	    = 'AH-6J';
			username    = 'AH-6J';
			desrt		= 'AH-6_destr';-- name of destroyed shape table, see below
			index       =  WSTYPE_PLACEHOLDER;
			life  	    = 16; --   The health of the object (ie. lifebar)
			vis   	    = 3; -- Visibility factor (For a small objects is better to put lower nr).
			fire  	    = { 300, 2}; -- Fire on the ground after destoyed: 300sec 4m
		},
		{name="AH-6_destr"; file="AH-6_destr"; fire={ 240, 2};},
	},

	mapclasskey 		= "P0091000021", -- found in MissionEditor/data/NewMap/images/nato/P91000021.png   gives map symbol "A"
	attribute  			= {wsType_Air, wsType_Helicopter, wsType_Battleplane, WSTYPE_PLACEHOLDER ,"Attack helicopters",},
	Categories 			= {},
	Rate 				= 30,  -- RewardPoint in Multiplayer
	Countries 			= {"USA"},
	date_of_introduction =   1989.0,--unsure exact date for AH-6J
	country_of_origin 	=   "USA",
	-------------- Aircraft Physical properties -----------
	length				= 32.06*ftToM, -- [meters]
	height				= 8.9*ftToM, -- [meters]
	rotor_RPM			= 475, -- This is used in the EFM for rotor sound
	tail_rotor_RPM		= 2838,
	M_empty				= 2550*lbToKg, -- [kg] empty weight (2150lb) + pilots (400lb)
	M_nominal			= 2900*lbToKg, -- [kg]
	M_max				= 3950*lbToKg, -- Max gross weight [kg]
	M_fuel_max			= 401*lbToKg, -- max fuel weight [kg]	401 lb usable main tank, aux tank will be added as external
	RCS					= 3, -- Radar Cross Section m^2
	IR_emission_coeff	= 0.2, -- 1 is IR emission of Su-27	
	--MOI 				= {12000, 35000, 40000},
	nose_gear_pos 		= { 1.416,	-1.729,	0}, -- used for starting placement on ground {forward/back,up/down,left/right}
	main_gear_pos 		= { -0.719,	-1.62,	0.889},
	lead_stock_main		= -0.1,--something to do with the gear
	lead_stock_support	= -0.1,--something to do with the gear	
	--sound_name 		= "Rotor", -- rotor sound from Sounds/sdef
	engines_count		= 1, -- number of engines
	engines_nozzles 	= 
	{
		{
		engine_number   = 1, -- this nozzle is for engine #1
		pos     	    = {-1.8,-0.565, 0}, -- important for heatblur effect
		diameter        = 0.13, -- [meters]
		smokiness_level = 0.1,
		}
	},
	
	------------- AI definitions (also affects Mission Editor i.e. max speed to set at waypoint) ------------------------------
	V_max				=	152*knotToKMpH, -- [km/h] max speed for AI
	V_max_cruise		=	130*knotToKMpH,	-- cruise speed [km/h]
	Vy_max				=	10.5, --Max climb speed in [m/s]
	H_stat_max_L		=	15000*ftToM,-- max height hover [m]
	H_stat_max			=	15000*ftToM,-- max height hover [m]
	H_din_two_eng		=	18000*ftToM,-- max height forward flight [m]
	H_din_one_eng		=	18000*ftToM,-- max height forward flight [m]
	range				=	430, -- max range [km], for AI
	flight_time_typical	=	90,-- minutes
	flight_time_maximum	=	180,-- minutes
	Vy_land_max			=	20*knotToMpS, -- landing speed [m/s]
	Ny_max				=	3.5, --max G for AI
	scheme				=	0,-- 0 for normal helis, 1 for ka-50 (might be rotor configuration)
	----- AI flight model probably (doesnt affect human FM)
	rotor_height		=	7.67*ftToM,-- height of rotor from ground [meters]
	rotor_diameter		=	27.35*ftToM,-- [meters]
	blade_chord			=	0.171,-- [meters]
	blades_number		=	5,
	blade_area			=	0.712, -- [m^2]
	fuselage_Cxa0		=	0.4,-- drag coefficient
	fuselage_Cxa90		=	3,-- side drag coefficient?
	fuselage_area		=	1.4, -- [m^2]
	centering			=	0,--???
	tail_pos 			= 	{-4.564,	0.163,	0},
	tail_fin_area		=	0.467,-- vertical fin area [m^2]
	tail_stab_area		=	0.66,-- horizontal tail area [m^2]
	thrust_correction	=	0.55,
	rotor_MOI			=	1500,
	rotor_pos 			= 	{0,	0.86, 0},

	engine_data = 
	{  -- these are only for AI (except the sound_name)
		power_take_off = 480,-- [kW]
		power_max	   = 480,-- [kW]
		power_WEP	   = 480,-- [kW]
		power_TH_k     = { -- power change from altitude
			[1] = {0,	-230.8,	2245.6},
			[2] = {0,	-230.8,	2245.6},
			[3] = {0,	-325.4,	2628.9},
			[4] = {0,	-235.6,	1931.9},
		},
		SFC_k = {2.045e-007, -0.0006328, 0.803},-- Specific fuel consumption (probably polynomial coefs.)
		power_RPM_k = 	{-0.08639,	0.24277,	0.84175},-- power vs RPM (probably polynomial coefs.)
		power_RPM_min	=	9.1384,
		--sound_name	= "EngineTV3117", -- engine sound from Sounds/sdef
	},
	
	Sensors = {	-- defines what the AI can use in terms of sensors
        OPTIC = {"TADS DVO"}, -- AI can engage enemy at night
        RWR = "Abstract RWR"
	},	
	CanopyGeometry = {
        azimuth   = {-100.0, 120.0}, -- pilot view horizontal (AI)
        elevation = {-50.0, 110.0} -- pilot view vertical (AI)
    },
	--CanopyGeometry = makeHelicopterCanopyGeometry(LOOK_AVERAGE, LOOK_AVERAGE, LOOK_AVERAGE),
		
	------------- Misc Properties ------------------------------
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
			--canopy_arg   = 38, -- important for noise effects of open/closed
		},
		[2] =
		{	ejection_seat_name	=	0,
			drop_canopy_name	=	0,
			pos = 	{0.725, 0.14, -0.7},
			ejection_order    = 2,
			--pilot_body_arg  = 501,
			can_be_playable  = true,	
			role = "instructor",
			role_display_name = _("Copilot"),
			--canopy_arg   = 38,
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
		
	Tasks = { 	-- Mission Editor Tasks. Defined in db_units_planes.lua
        aircraft_task(CAS), 	
        aircraft_task(GroundAttack),
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
		["ROTOR"]			= {critical_damage = 3, args = {160}, deps_cells = {"BLADE_1_IN", "BLADE_2_IN", "BLADE_3_IN", "BLADE_4_IN", "BLADE_5_IN"}},
		["BLADE_1_IN"]		= {critical_damage = 1, args = {161}, deps_cells = {"BLADE_1_CENTER"}},
		["BLADE_1_CENTER"]	= {critical_damage = 1, args = {161}, deps_cells = {"BLADE_1_OUT"}},
		["BLADE_1_OUT"]		= {critical_damage = 1, args = {161}},
		["BLADE_2_IN"]		= {critical_damage = 1, args = {162}, deps_cells = {"BLADE_2_CENTER"}},
		["BLADE_2_CENTER"]	= {critical_damage = 1, args = {162}, deps_cells = {"BLADE_2_OUT"}},
		["BLADE_2_OUT"]		= {critical_damage = 1, args = {162}},
		["BLADE_3_IN"]		= {critical_damage = 1, args = {163}, deps_cells = {"BLADE_3_CENTER"}},
		["BLADE_3_CENTER"]	= {critical_damage = 1, args = {163}, deps_cells = {"BLADE_3_OUT"}},
		["BLADE_3_OUT"]		= {critical_damage = 1, args = {163}},
		["BLADE_4_IN"]		= {critical_damage = 1, args = {164}, deps_cells = {"BLADE_4_CENTER"}},
		["BLADE_4_CENTER"]	= {critical_damage = 1, args = {164}, deps_cells = {"BLADE_4_OUT"}},
		["BLADE_4_OUT"]		= {critical_damage = 1, args = {164}},
		["BLADE_5_IN"]		= {critical_damage = 1, args = {165}, deps_cells = {"BLADE_5_CENTER"}},
		["BLADE_5_CENTER"]	= {critical_damage = 1, args = {165}, deps_cells = {"BLADE_5_OUT"}},
		["BLADE_5_OUT"]		= {critical_damage = 1, args = {165}},
		
		-- Fuselage
		["MAIN"]  			= {critical_damage = 10, args = {151}},
		["KEEL_OUT"]  		= {critical_damage = 8, args = {151}, deps_cells = {"TAIL"}}, -- aft upper fuselage
		["FRONT"]			= {critical_damage = 1, args = {170}}, -- glass canopy
		["COCKPIT"]			= {critical_damage = 1, args = {149}}, -- internals
		["FUEL_TANK_B"]		= {critical_damage = 1, args = {150}}, -- backseat tank
		["FUEL_TANK_F"]		= {critical_damage = 1, args = {152}}, -- main tank (inside bottom of fuselage)
		["KEEL_CENTER"]		= {critical_damage = 1, args = {152}}, -- battery
		["ENGINE"]			= {critical_damage = 3, args = {152}}, -- Engine
		["ENGINE_R"]		= {critical_damage = 3, args = {152}}, -- Engine compartment
		
		["CREW_1"]			= {critical_damage = 2, args = {205}}, -- pilot
		["CREW_2"]			= {critical_damage = 2, args = {204}}, -- copilot
		
		-- Skids
		["WING_L"]			= {critical_damage = 5, args = {156}}, -- left skid shell
		["ELEVATOR_L_OUT"]	= {critical_damage = 2, args = {156}}, -- left skid line F
		["Line_STABIL_L"]	= {critical_damage = 2, args = {156}}, -- left skid line R
		["WING_R"]			= {critical_damage = 5, args = {157}}, -- right skid shell
		["ELEVATOR_R_OUT"]	= {critical_damage = 2, args = {157}}, -- right skid line F
		["Line_STABIL_R"]	= {critical_damage = 2, args = {157}}, -- right skid line R
		
		-- Tail
		["TAIL"]			= {critical_damage = 4, args = {159}, deps_cells = {"TAIL_RIGHT_SIDE","TAIL_LEFT_SIDE"}},
		["TAIL_RIGHT_SIDE"]	= {critical_damage = 3, args = {159}, deps_cells = {"TAIL_TOP","TAIL_BOTTOM"}}, -- vertical tail
		["TAIL_TOP"]		= {critical_damage = 3, args = {166}}, -- horizontal tail
		["TAIL_BOTTOM"]		= {critical_damage = 1, args = {166}}, -- tail skid shell
		["WING_L_00"]		= {critical_damage = 1, args = {166}}, -- tail skid line
		["TAIL_LEFT_SIDE"]	= {critical_damage = 3, args = {159}, deps_cells = {"BLADE_6_OUT","BLADE_6_IN"}}, -- TR transmission
		["BLADE_6_OUT"]		= {critical_damage = 1, args = {166}}, -- tail rotor blade 1
		["BLADE_6_IN"]		= {critical_damage = 1, args = {166}}, -- tail rotor blade 2
		
		["PYLON1"]			= {critical_damage = 2, args = {205}}, -- weapon plank
		
	}),
	
	Failures = { -- not working yet
		{ id = 'engfail',	label = _('ENGINE'),	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
		{ id = 'rotor',		label = _('ROTOR'),		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
	},

	-- to drop custom parts,  use 1000+damage cell#   ex: to drop elevator use [1000+51]="_elevator"
	DamageParts = {	-- parts that fall off when aircraft is hit or crashes
		[1] = "AH-6_Tail",  --wing right
		[2] = "AH-6_Skid",  --wing left
		--[3] = "",    --nose part
		[4] = "AH-6_Skid",  -- tail part
		[5] = "AH-6_Rotor",	--blade
	},
	
	--[[
		Light Categories
		WOLALIGHT_STROBES          = 1		--strobe anti-collision lights
		WOLALIGHT_LANDING_LIGHTS   = 2		--Take-off and landing high power headlights
		WOLALIGHT_NAVLIGHTS        = 3		--colored navigation (position) wingtip lights
		WOLALIGHT_FORMATION_LIGHTS = 4		--Formation / Logo lights
		WOLALIGHT_TIPS_LIGHTS      = 5		--Helicopter-specific: rotor anti-collision tips lights
		WOLALIGHT_TAXI_LIGHTS      = 6		--Taxi headlights
		WOLALIGHT_BEACONS          = 7		--Rotary anti-collision lights
		WOLALIGHT_CABIN_BOARDING   = 8		--Cabin incandescence illumination
		WOLALIGHT_CABIN_NIGHT      = 9		--Cabin night time (UV) illumination
		WOLALIGHT_REFUEL_LIGHTS    = 10		--Refuel apparatus illumination
		WOLALIGHT_PROJECTORS       = 11		--Search lights, direction-controlled
		WOLALIGHT_AUX_LIGHTS       = 12		--Signal lights, also all aux. lights not fitting into other categories
		WOLALIGHT_IR_FORMATION     = 13		--IR formation strips. Currently not implemented due to engine NVG limitations
	]]--
	lights_data = {
		typename = "collection",
		lights = {
		[WOLALIGHT_STROBES] = { -- strobe/anti-collision lights
			typename = "collection",
			lights = {{typename = "argnatostrobelight", argument = 191, period = 0.75, flash_time = 0.5},}
		},
		[WOLALIGHT_LANDING_LIGHTS] = { -- landing lights
			typename = "collection",
			lights = {{typename  = "argumentlight",	argument  = 208},}
		},
		[WOLALIGHT_NAVLIGHTS]	= {	-- position/nav lights
			typename = "collection",
			lights = {{typename = "argumentlight", argument  = 190},}
		},
		[WOLALIGHT_FORMATION_LIGHTS] = {	-- formation lights
			typename = "collection",
			lights = {{typename  = "argumentlight", argument = 200},}
		},
		[WOLALIGHT_IR_FORMATION] = {	-- IR formation lights
			typename = "collection",
			lights = {{typename  = "argumentlight", argument = 201},}
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


	HumanRadio = {
		frequency     = 305.0,
        editable     = true,
        minFrequency     = 30.000,
        maxFrequency     = 399.975,
		rangeFrequency = {
			{min = 30.0,  max = 87.975, modulation	= MODULATION_FM},
			{min = 108.0, max = 173.975, modulation	= MODULATION_AM_AND_FM, modulationDef = MODULATION_FM},
			{min = 225.0, max = 399.975, modulation	= MODULATION_AM_AND_FM, modulationDef = MODULATION_AM},
		},
        modulation     = MODULATION_AM
	},

	panelRadio = {		
		[1] = {  
				name = _("VHF/UHF AN/ARC-182"),
				range = {{min = 30.0, max = 87.975, modulation	= MODULATION_FM},
					 {min = 108.0, max = 173.975, modulation	= MODULATION_AM_AND_FM, modulationDef = MODULATION_FM},
					 {min = 225.0, max = 399.975, modulation	= MODULATION_AM_AND_FM, modulationDef = MODULATION_AM}},
				channels = {
				[1] =  { name = _("Channel 1"),	 default = 305.0, connect = true}, -- default
				[2] =  { name = _("Channel 2"),	 default = 264.0},	-- min. water : 135.0, 264.0
				[3] =  { name = _("Channel 3"),	 default = 265.0},	-- nalchik : 136.0, 265.0
				[4] =  { name = _("Channel 4"),	 default = 256.0},	-- sochi : 127.0, 256.0
				[5] =  { name = _("Channel 5"),	 default = 254.0},	-- maykop : 125.0, 254.0
				[6] =  { name = _("Channel 6"),	 default = 250.0},	-- anapa : 121.0, 250.0
				[7] =  { name = _("Channel 7"),	 default = 270.0},	-- beslan : 141.0, 270.0
				[8] =  { name = _("Channel 8"),	 default = 257.0},	-- krasnodar-pashk. : 128.0, 257.0
				[9] =  { name = _("Channel 9"),	 default = 255.0},	-- gelenjik : 126.0, 255.0
				[10] = { name = _("Channel 10"), default = 262.0},	-- kabuleti : 133.0, 262.0
				[11] = { name = _("Channel 11"), default = 259.0},	-- gudauta : 130.0, 259.0
				[12] = { name = _("Channel 12"), default = 268.0},	-- soginlug : 139.0, 268.0
				[13] = { name = _("Channel 13"), default = 269.0},	-- vaziani : 140.0, 269.0
				[14] = { name = _("Channel 14"), default = 260.0},	-- batumi : 131.0, 260.0
				[15] = { name = _("Channel 15"), default = 263.0},	-- kutaisi : 134.0, 263.0
				[16] = { name = _("Channel 16"), default = 261.0},	-- senaki : 132.0, 261.0
				[17] = { name = _("Channel 17"), default = 267.0},	-- lochini : 138.0, 267.0
				[18] = { name = _("Channel 18"), default = 251.0},	-- krasnodar-center : 122.0, 251.0
				[19] = { name = _("Channel 19"), default = 253.0},	-- krymsk : 124.0, 253.0
				[20] = { name = _("Channel 20"), default = 266.0},	-- mozdok : 137.0, 266.0
                [21] = { name = _("Channel 21"), default = 225.0},
                [22] = { name = _("Channel 22"), default = 258.0},			
                [23] = { name = _("Channel 23"), default = 131.0},
                [24] = { name = _("Channel 24"), default = 134.0},
                [25] = { name = _("Channel 25"), default = 132.0},
                [26] = { name = _("Channel 26"), default = 127.0},
                [27] = { name = _("Channel 27"), default = 136.0},
                [28] = { name = _("Channel 28"), default = 128.0},
                [29] = { name = _("Channel 29"), default = 122.0},
                [30] = { name = _("Channel 30"), default = 137.0},				
				}
			},
	},	
	
	rotor_animation =
	{
		-- arg 448 for tail rotor vis
		-- arg 41 for TR spin animation, control using arg 42
		-- also requires all 4 ED propeller params be set in the EFM 
		-- (ED_FM_PROPELLER_1_RPM...ED_FM_PROPELLER_1_INTEGRITY_FACTOR)
		tail_rotor = 
		{
			blades_count	= 2,
			modelBlade_FBX	= "/models/AH-6J_tailrotor_blade.fbx",
			rotor_direction	= 1,
			rotor_locations = {{POSITION_WILL_BE_TAKEN_FROM_CONNECTOR}},
			--rotor_locations = {
			--	{	pos = {-4.8623, 0.0998,0.2795},yaw = math.rad(90)},
				--{	pos = {-4.8623, 0.0998,0.2795},yaw = math.rad(90)},
			--},
		},
		-- main rotor
		-- arg 447 for main rotor vis	
		-- arg 40 for MR spin animation, control using arg 37
		rotor_locations = {{POSITION_WILL_BE_TAKEN_FROM_CONNECTOR}},
		rotor_models =
		{{
			modelRotorHub_EDM  		= "AH-6J_Rotor_Hub",
			modelRotorHubLod_FBX 	= "/models/AH-6J_rotor_hub_LOD.fbx",
			boundRotorHub_FBX		= "/models/AH-6J_rotor_hub_bounds.fbx",
			boundBlade_FBX			= "/models/AH-6J_rotor_blade_bounds.fbx",
			modelBlade_FBX 			=
			{
				"/models/AH-6J_rotor_blade.fbx", 
			}
		}},
	},
	
	-- Aircraft Additional Properties in mission editor
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
		{id = "DoorsOn",
		  control = "checkbox",
		  label = _("Doors Installed"),
		  defValue = false,
		  weightWhenOn = 8.9,
		  arg = 1003,
		},
		{id = "IRexhaustOn",
		  control = "checkbox",
		  label = _("IR Suppressor Installed"),
		  defValue = false,
		  weightWhenOn = 5,
		  arg = 1004,
		},
	},
}

add_aircraft(AH6J)
