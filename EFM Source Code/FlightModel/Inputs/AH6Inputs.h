#pragma once

// Used in ed_fm_set_command() 
enum AH6InputCommands
{
	// commands from command_defs.lua
	starterButton		= 3010,
	throttleIdleCutoff  = 3011,
	throttle			= 3012,
	batterySwitch		= 3013,
	generatorSwitch		= 3014,
	inverterSwitch		= 3015,
	throttleAxis		= 3016,
	trimUp				= 3017,
	trimDown			= 3018,
	trimLeft			= 3019,
	trimRight			= 3020,



	// joystick axis commands
	JoystickPitch		= 2001,	
	JoystickRoll		= 2002,
	JoystickYaw			= 2003,
	JoystickThrottle	= 2004,


	Reserved // placeholder
};