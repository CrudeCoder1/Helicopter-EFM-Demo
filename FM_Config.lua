function skidSuspension(collisionLine, arg)
	return{
			self_attitude					= false,
			yaw_limit						= math.rad(0.0),

			amortizer_min_length					= 0.0,
			amortizer_max_length					= 0.115,
			amortizer_basic_length					= 0.115,
			amortizer_spring_force_factor			= 80000.0, 
			amortizer_spring_force_factor_rate		= 1,
			amortizer_static_force					= 7500.0,
			amortizer_reduce_length					= 0.115,
			amortizer_direct_damper_force_factor	= 10000,
			amortizer_back_damper_force_factor		= 2000,

			wheel_radius					= 1.3,
			wheel_static_friction_factor	= 0.7,
			wheel_side_friction_factor		= 0.5,
			wheel_roll_friction_factor		= 0.3,
			wheel_glide_friction_factor		= 0.80,
			wheel_damage_force_factor		= 350.0,
			wheel_damage_speedX				= 110,
			wheel_damage_delta_speedX		= 10.0,

			arg_post			= -1,
			arg_amortizer		= arg,
			arg_wheel_yaw		= -1,
			arg_wheel_rotation	= -1,
			damage_element		= 83,
			collision_shell_name = collisionLine,
	}
end


EFM = {   
	center_of_mass    = {0,0,0},--{-0.125, 0.15, 0.0}, -- center of mass position relative to object 3d model center for empty aircraft (m)     -- {forward/back,up/down,left/right}	
    moment_of_inertia = {458, 1008, 1242, 129},--{446, 979, 1219, 128},  -- moment of inertia of empty aircraft (Ixx,Iyy,Izz,Ixz DCS axis)/(Ix,Iz,Iy,Ixy normal axis) [kg*m^2]  
	
	suspension = { 
		skidSuspension("ELEVATOR_L_OUT", 1),
		skidSuspension("ELEVATOR_R_OUT", 343),
		skidSuspension("Line_STABIL_L", 6),
		skidSuspension("Line_STABIL_R", 4),		
	}, 
    disable_built_in_oxygen_system  = false, -- set this to false to enable hypoxia effects, etc
}