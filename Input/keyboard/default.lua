local cockpit = folder.."../../Cockpit/Scripts/"
dofile(cockpit.."devices.lua")
dofile(cockpit.."command_defs.lua")
local res = external_profile("Config/Input/Aircrafts/common_keyboard_binding.lua")

-- down = single instance,  pressed = continuous input

join(res.keyCommands,{

-- Systems
	{combos = {{key = 'Home'}}, down = EFM_commands.starterButton, up = EFM_commands.starterButton, value_down = 1.0, value_up = 0.0, name = _('Starter Button'), category = _('Systems')},
    {combos = {{key = 'End'}}, down = Keys.ThrottleCutoff, name = _('Throttle Idle Cutoff'), category = _('Systems')},
	{combos = {{key = 'PageUp'}}, pressed = Keys.ThrottleIncrease, up = Keys.ThrottleStop,  name = _('Throttle Up'), category = _('Systems')},
    {combos = {{key = 'PageDown'}}, pressed = Keys.ThrottleDecrease, up = Keys.ThrottleStop,  name = _('Throttle Down'), category = _('Systems')},
    {combos = {{key = 'B'}}, down = Keys.BattSwitch, name = _('Battery Switch'), category = _('Systems')}, 
	{combos = {{key = 'B', reformers = {'LCtrl'}}}, down = Keys.ExtPwrSwitch, name = _('External Power Switch'), category = _('Systems')}, 
	


-- Weapons                                                                        
    {combos = {{key = 'Space'}}, down = device_commands.GunTrigger, up = device_commands.GunTrigger,cockpit_device_id = devices.WEAPON_SYSTEM,value_down = 1.0, value_up = 0.0, name = _('Gun Fire Trigger'), category = _('Weapons')},
    {combos = {{key = 'Space', reformers = {'LAlt'}}}, down = device_commands.RocketFireButton, up = device_commands.RocketFireButton,cockpit_device_id = devices.WEAPON_SYSTEM,value_down = 1.0, value_up = 0.0, name = _('Rocket Fire Button'), category = 'Weapons'},
	{combos = {{key = 'M'}}, down = Keys.MasterArmToggle, name = _('Master Arm Toggle'), category = _('Weapons')},
	
-- Lighting
    {combos = {{key = 'L', reformers = {'RShift'}}},	down = Keys.LandingLight, 							name = _('Landing Light'),	category = _('Lighting')},
	
-- Others
	{combos = {{key = 'E', reformers = {'LCtrl'}}}, down 	= iCommandPlaneEject, 				 name = _('Eject (3 times)'), category = _('General')},
	
-- Night Vision Goggles
	{combos = {{key = 'N'}}	, down = iCommandViewNightVisionGogglesOn,	 name = _('Toggle Night Vision Goggles'), 	category = _('NVG')},
	{combos = {{key = 'N', reformers = {'RCtrl'}}}, pressed = iCommandPlane_Helmet_Brightess_Up  , name = _('Gain NVG up')  , category = _('NVG')},
	{combos = {{key = 'N', reformers = {'RAlt'}}} , pressed = iCommandPlane_Helmet_Brightess_Down, name = _('Gain NVG down'), category = _('NVG')},

-- Multicrew
	{combos = {{key = '1'}},	down = iCommandViewCockpitChangeSeat, value_down = 1, name = _('Occupy Pilot Seat'),	category = _('Crew Control')},
	{combos = {{key = '2'}},	down = iCommandViewCockpitChangeSeat, value_down = 2, name = _('Occupy Copilot Seat'),	category = _('Crew Control')},	
	{combos = {{key = 'C'}},	down = iCommandNetCrewRequestControl,				name = _('Request Aircraft Control'),category = _('Crew Control')},
	
	--{combos = {{key = 'P', reformers = {'RShift'}}}, down = iCommandCockpitShowPilotOnOff, name = _('Show Pilot Body'), category = _('General')},

-- Flight Controls
	{combos = {{key = 'Z'}}, pressed = EFM_commands.KeyRudderLeft,	up = EFM_commands.KeyRudderStop, name = _('Aircraft Yaw Left'),	category = _('Flight Control')},
	{combos = {{key = 'X'}}, pressed = EFM_commands.KeyRudderRight,	up = EFM_commands.KeyRudderStop, name = _('Aircraft Yaw Right'),category = _('Flight Control')},
    {combos = defaultDeviceAssignmentFor("thrust_up"), pressed = EFM_commands.KeyCollectiveUp, name = _('Collective up'), category = _('Flight Control')},
	{combos = defaultDeviceAssignmentFor("thrust_down"), pressed = EFM_commands.KeyCollectiveDown, name = _('Collective down'), category = _('Flight Control')},

	{combos = {{key = 'Down'}}, pressed = EFM_commands.KeyCyclicBack,	name = _('Aircraft Pitch Up'),category = _('Flight Control')},
	{combos = {{key = 'Up'}}, pressed = EFM_commands.KeyCyclicForward,	name = _('Aircraft Pitch Down'),category = _('Flight Control')},
	{combos = {{key = 'Left'}}, pressed = EFM_commands.KeyCyclicLeft,	name = _('Aircraft Roll Left'),category = _('Flight Control')},
	{combos = {{key = 'Right'}}, pressed = EFM_commands.KeyCyclicRight,	name = _('Aircraft Roll Right'),category = _('Flight Control')},   

	{combos = {{key = ';', reformers = {'RCtrl'}}}, pressed = EFM_commands.trimUp, name = _('Cyclic Trim Up'), category = _('Flight Control')},
	{combos = {{key = '.', reformers = {'RCtrl'}}}, pressed = EFM_commands.trimDown, name = _('Cyclic Trim Down'), category = _('Flight Control')},
	{combos = {{key = ',', reformers = {'RCtrl'}}}, pressed = EFM_commands.trimLeft, name = _('Cyclic Trim Left'), category = _('Flight Control')},
	{combos = {{key = '/', reformers = {'RCtrl'}}}, pressed = EFM_commands.trimRight, name = _('Cyclic Trim Right'), category = _('Flight Control')},

})
return res
