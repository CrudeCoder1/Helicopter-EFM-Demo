local cockpit = folder.."../../Cockpit/Scripts/"
dofile(cockpit.."devices.lua")
dofile(cockpit.."command_defs.lua")
local res = external_profile("Config/Input/Aircrafts/common_joystick_binding.lua")


join(res.keyCommands,{

-- Systems
	{down = EFM_commands.starterButton, up = EFM_commands.starterButton, value_down = 1.0, value_up = 0.0, name = _('Starter Button'), category = _('Systems')},
    {down = Keys.ThrottleCutoff, name = _('Throttle Idle Cutoff'), category = _('Systems')},
	{pressed = Keys.ThrottleIncrease, up = Keys.ThrottleStop,  name = _('Throttle Up'), category = _('Systems')},
    {pressed = Keys.ThrottleDecrease, up = Keys.ThrottleStop,  name = _('Throttle Down'), category = _('Systems')},
    {down = Keys.BattSwitch, name = _('Battery Switch'), category = _('Systems')}, 
	{down = Keys.ExtPwrSwitch, name = _('External Power Switch'), category = _('Systems')}, 
	
	{pressed = EFM_commands.trimUp, name = _('Cyclic Trim Up'), category = _('Systems')},
	{pressed = EFM_commands.trimDown, name = _('Cyclic Trim Down'), category = _('Systems')},
	{pressed = EFM_commands.trimLeft, name = _('Cyclic Trim Left'), category = _('Systems')},
	{pressed = EFM_commands.trimRight, name = _('Cyclic Trim Right'), category = _('Systems')},

-- Weapons                                                                        
    {pressed = Keys.TriggerFireOn, up = Keys.TriggerFireOff, name = _('Gun Fire Trigger'), category = _('Weapons')},
    {down = Keys.PickleOn, up = Keys.PickleOff, name = _('Rocket Fire Button'), category = 'Weapons'},
	{down = Keys.MasterArm, name = _('Master Arm'), category = _('Weapons')},
	
-- Lighting
    {down = Keys.LandingLight, 							name = _('Landing Light'),	category = _('Lighting')},
	--{down = 3256, cockpit_device_id  = 0,	value_down = 1.0,	name = _('Flashlight'),	category = _('Lighting')},
	
-- Others
	{down 	= iCommandPlaneEject, 				 name = _('Eject (3 times)'), category = _('General')},
	
-- Night Vision Goggles
	{down = iCommandViewNightVisionGogglesOn,	 name = _('Toggle Night Vision Goggles'), 	category = _('NVG')},
	{pressed = iCommandPlane_Helmet_Brightess_Up  , name = _('Gain NVG up')  , category = _('NVG')},
	{pressed = iCommandPlane_Helmet_Brightess_Down, name = _('Gain NVG down'), category = _('NVG')},

-- Multicrew
	{down = iCommandViewCockpitChangeSeat, value_down = 1, name = _('Occupy Pilot Seat'),	category = _('Crew Control')},
	{down = iCommandViewCockpitChangeSeat, value_down = 2, name = _('Occupy Copilot Seat'),	category = _('Crew Control')},	
	{down = iCommandNetCrewRequestControl,				name = _('Request Aircraft Control'),category = _('Crew Control')},
	
	--{combos = {{key = 'P', reformers = {'RShift'}}}, down = iCommandCockpitShowPilotOnOff, name = _('Show Pilot Body'), category = _('General')},

    --{down = Keys.showControlInd,				name = _('Show controls indicator') ,			 category = _('General')},

})

-- joystick axis 
join(res.axisCommands,{
    {action = iCommandViewHorizontalAbs			, name = _('Absolute Camera Horizontal View')},
    {action = iCommandViewVerticalAbs			, name = _('Absolute Camera Vertical View')},
    {action = iCommandViewZoomAbs				, name = _('Zoom View')},
    {action = iCommandViewRollAbs 				, name = _('Absolute Roll Shift Camera View')},
    {action = iCommandViewHorTransAbs 			, name = _('Absolute Horizontal Shift Camera View')},
    {action = iCommandViewVertTransAbs 			, name = _('Absolute Vertical Shift Camera View')},
    {action = iCommandViewLongitudeTransAbs 	, name = _('Absolute Longitude Shift Camera View')},
	
{combos = defaultDeviceAssignmentFor("roll")	, action = iCommandPlaneRoll,			name = _('Roll Cyclic')},
{combos = defaultDeviceAssignmentFor("pitch")	, action = iCommandPlanePitch,			name = _('Pitch Cyclic')},
{combos = defaultDeviceAssignmentFor("rudder")	, action = iCommandPlaneRudder, 		name = _('Pedals')},
{combos = defaultDeviceAssignmentFor("thrust")	, action = iCommandPlaneThrustCommon,	name = _('Collective')},
{action = EFM_commands.throttleAxis,	name = _('Throttle')},

})
return res
