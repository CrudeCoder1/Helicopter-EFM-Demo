dofile("Scripts/Database/Weapons/warheads.lua")

local GALLON_TO_KG = 3.785 * 0.8
local INCHES_TO_M = 0.0254
local POUNDS_TO_KG = 0.453592

local tracer_on_time = 0.01
local barrel_smoke_level = 1.0
local barrel_smoke_opacity = 0.1

    -- 7.62x51mm NATO M80 Ball
M80 = {
        category = CAT_SHELLS, name = "7.62x51mm M80", user_name = _("7.62x51mm M80"),
        model_name      = "tracer_bullet_white",
        mass            = 0.00952, --kg of each bullet
        round_mass      = 0.0254, --kg of entire round
        cartridge_mass  = 0.01588, --kg of shell
        explosive       = 0.000,
        v0              = 856.2,   -- 2809 fps = 856.2 m/s
        Dv0             = 0.0060,
        Da0             = 0.0022, -- dispersion amount
        Da1             = 0.0,
        life_time       = 30,
        caliber         = 7.62, --mm
        s               = 0.0,
        j               = 0.0,
        l               = 0.0,
        charTime        = 0,
        cx              = {0.5,1.27,0.70,0.200,2.30},
        k1              = 2.0e-08,
        tracer_off      = -1,
        tracer_on       = tracer_on_time,
        smoke_tail_life_time = 0,
        scale_tracer    = 0,
        cartridge       = 0,
        scale_smoke     = barrel_smoke_level,
        smoke_opacity   = barrel_smoke_opacity
}
declare_weapon(M80)

    -- 7.62x51mm NATO M61 Armor Piercing
M61 = {
        category = CAT_SHELLS, name = "7.62x51mm M61", user_name = _("7.62x51mm M61"),
        model_name      = "tracer_bullet_white",
        mass            = 0.00975, --kg of each bullet
        round_mass      = 0.0256, --kg of entire round
        cartridge_mass  = 0.01588, --kg of shell
        explosive       = 0.000,
        v0              = 854.6,   -- 2804 fps = 854.6 m/s
        Dv0             = 0.0060,
        Da0             = 0.0022, -- dispersion amount
        Da1             = 0.0,
        life_time       = 30,
        caliber         = 7.62, --mm
        s               = 0.0,
        j               = 0.0,
        l               = 0.0,
        charTime        = 0,
        cx              = {0.5,1.27,0.70,0.200,2.30},
        k1              = 2.0e-08,
        tracer_off      = -1,
        tracer_on       = tracer_on_time,
        smoke_tail_life_time = 0,
        scale_tracer    = 0,
        cartridge       = 0,
        scale_smoke     = barrel_smoke_level,
        smoke_opacity   = barrel_smoke_opacity
}
declare_weapon(M61)

    -- 7.62x51mm NATO M62 Tracer
M62 = {
        category = CAT_SHELLS, name = "7.62x51mm M62", user_name = _("7.62x51mm M62"),
        model_name      = "tracer_bullet_red",
        mass            = 0.0092, --kg of each bullet
        round_mass      = 0.02508, --kg of entire round
        cartridge_mass  = 0.01588, --kg of shell
        explosive       = 0.000,
        v0              = 856.2,   -- 2809 fps = 856.2 m/s
        Dv0             = 0.0060,
        Da0             = 0.0022, -- dispersion amount
        Da1             = 0.0,
        life_time       = 30,
        caliber         = 7.62, --mm
        s               = 0.0,
        j               = 0.0,
        l               = 0.0,
        charTime        = 0,
        cx              = {0.5,1.27,0.70,0.200,2.30},
        k1              = 2.0e-08,
        tracer_off      = 1.7,
        tracer_on       = tracer_on_time,
        smoke_tail_life_time = 1.7,
        scale_tracer    = 1,
        cartridge       = 0,
        scale_smoke     = barrel_smoke_level,
        smoke_opacity   = barrel_smoke_opacity
}
declare_weapon(M62)


function M134(tbl)
    tbl.category = CAT_GUN_MOUNT
    tbl.name      = "M134"
    tbl.supply      =
    {
        shells = {"7.62x51mm M80", "7.62x51mm M61", "7.62x51mm M62"},
        mixes  = {{1,1,2,2,2,3}},   -- for every 6 shots there will be one M62 (tracer)
        count  = 2000,		-- 2000 rounds
    }
    if tbl.mixes then
       tbl.supply.mixes = tbl.mixes
       tbl.mixes        = nil
    end
    tbl.gun =
    {
        max_burst_length    = 6, -- minimum number of rounds fired per burst
        rates               = {4000},
        recoil_coeff        = 0.07*1.3,
        barrels_count       = 6,
    }
    if tbl.rates then
       tbl.gun.rates        =  tbl.rates
       tbl.rates            = nil
    end
    tbl.ejector_pos             = {-0.818,-0.144,0.022} -- position from muzzle connector pos
    tbl.ejector_dir             = {-1, -6, 0} -- left/right; back/front;?/?
    tbl.supply_position         = tbl.supply_position   or {-0.155, 0.382, 0.944}
    tbl.aft_gun_mount           = false
    tbl.effective_fire_distance = 1000
    tbl.drop_cartridge          = 205	--drop shell shape/size from  aircraft_gun_mounts.lua
    tbl.muzzle_pos              = tbl.muzzle_pos            or  {0.886,-0.259,0}     -- all position from connector
    tbl.muzzle_pos_connector    = tbl.muzzle_pos_connector  or  "Gun_point" -- all position from connector
    tbl.azimuth_initial         = tbl.azimuth_initial       or  0
    tbl.elevation_initial       = tbl.elevation_initial     or  0
    if  tbl.effects == nil then
        tbl.effects = {{ name = "FireEffect"     , arg = tbl.effect_arg_number or 436 },
                       { name = "HeatEffectExt"  , shot_heat = 7.823, barrel_k = 0.462 * 16.0},
                       { name = "SmokeEffect"},
					   { name = "GatlingEffect", arg = 800, barrels_n = 6, spin_up_t = 1, spin_down_t = 0.01}
					   }
    end
    return declare_weapon(tbl)
end

minigun = {
    category        = CAT_PODS,
    CLSID           = "{M134 Minigun}",
    attribute       = {wsType_Weapon,wsType_GContainer,wsType_Cannon_Cont,WSTYPE_PLACEHOLDER},
    wsTypeOfWeapon  = {wsType_Weapon,wsType_Shell,wsType_Shell,WSTYPE_PLACEHOLDER},
    Picture         = "M134.png",
    displayName     = _("M134 Minigun"),
    Weight          = 39,      --loaded	39kg empty gun + 50.72kg ammo
    Cx_pil          = 0.00015,
    Elements        = {{ShapeName = "ah6_m134"}},
    kind_of_shipping = 2,   -- SOLID_MUNITION
    gun_mounts      = {
        M134({
			
			})
    },
    shape_table_data = {{file = 'ah6_m134'; username = 'M134 Minigun'; index = WSTYPE_PLACEHOLDER;}}
}
declare_loadout(minigun)



function AH6_GAU19(tbl)

	tbl.category = CAT_GUN_MOUNT 
	tbl.name 	 = "AH-6 GAU-19"
	tbl.supply 	 = 
	{
		shells = {"M2_50_aero_AP","M20_50_aero_APIT"},
		mixes  = {{2,1,1,1}},
		count  = 1000,
	}
	if tbl.mixes then 
	   tbl.supply.mixes =  tbl.mixes
	   tbl.mixes	    = nil
	end
	tbl.gun = 
	{
		max_burst_length = 3,
		rates 			 = {1300},
		recoil_coeff 	 = 0.25*1.3,
		barrels_count 	 = 3,
	}
	if tbl.rates then 
	   tbl.gun.rates    =  tbl.rates
	   tbl.rates	    = nil
	end	
	tbl.ejector_pos 			= {-1.072, -0.05, -0.039} -- position from muzzle connector pos
	tbl.ejector_dir 			= tbl.ejector_dir
	tbl.supply_position  		= tbl.supply_position or {0,  0.3, -0.3}
	tbl.aft_gun_mount 			= false
	tbl.effective_fire_distance = 1800
	tbl.drop_cartridge 			= 204 -- drop shell shape/size from  aircraft_gun_mounts.lua
	tbl.muzzle_pos				= tbl.muzzle_pos or {0,0,0} -- all position from connector
	tbl.muzzle_pos_connector	= tbl.muzzle_pos_connector 	or  "Gun_point" -- all position from connector
	tbl.azimuth_initial 		= tbl.azimuth_initial    or 0   
	tbl.elevation_initial 		= tbl.elevation_initial  or 0   
	if  tbl.effects == nil then
		tbl.effects = {{ name = "FireEffect"     , arg 		 = tbl.effect_arg_number or 436 },
					   { name = "HeatEffectExt"  , shot_heat = 7.823, barrel_k = 0.462 * 2.7, body_k = 0.462 * 14.3 },
					   { name = "SmokeEffect"},
					   { name = "GatlingEffect", arg = 44, barrels_n = 3, spin_up_t = 0.4, spin_down_t = 0.1}
					   }
	end
	return declare_weapon(tbl)
end

declare_loadout({
	category 		=   CAT_PODS,
	CLSID	 		=  "{AH6_GAU-19}",
	attribute		=   {wsType_Weapon,wsType_GContainer,wsType_Cannon_Cont,WSTYPE_PLACEHOLDER},
	wsTypeOfWeapon	= 	{wsType_Weapon,wsType_Shell,wsType_Shell,WSTYPE_PLACEHOLDER},
	Picture			=	"gau19.png",
	displayName		=	_("GAU-19 Gattling Gun"),-- loadout editor name
	Weight			=	63, --loaded	63kg empty gun + ???50kg ammo???  ammo is stored over center of mass. adding ammo weight here will cause bigger imbalance. need to find way to add ammo weight at center of gravity
	Cx_pil			=	0.00024,
	Elements  		= {{ShapeName = "AH6_GAU19"}},
	kind_of_shipping = 2,--SOLID_MUNITION
	gun_mounts		= {
			AH6_GAU19({
				ejector_dir = {0,-1,-0.5},
				supply_position = {2, -0.3, -0.4}
			})			
	},	
	shape_table_data = {{file  	 = 'AH6_GAU19';	username = 'GAU-19_gun'; index = WSTYPE_PLACEHOLDER;}}
})