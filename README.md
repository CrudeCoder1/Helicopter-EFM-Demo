# Helicopter-EFM-Demo
Helicopter EFM Demo for DCS World using the AH-6J as the airframe.

Original Thread: https://forum.dcs.world/topic/228394-helicopter-efm-demo/


## Change log

0.4 13 October 2020
- Added option to remove aiming dot (under special options)
- External textures unwrapped and new basic textures thanks to Particleman
- Added cold start procedure to kneeboard
- Fixed bug where using the ']' key to cycle kneeboard pages moved idle cutoff and shut down engine
- Engine sound fixed (due to DCS update)
- RWR updated to more closely match real APR-39 display
- Added main menu logo
- Rockets able to be loaded when weapon plank removed - fixed
 
0.35 6 August 2020
- Added zoom axis
- Changed rockets in loadout from practice to HE and added White Phosphorus
- Added menu background by Silvrav
- Added segmented display to fuel gauge
- EFM:
- Lift now affected by missing rotor blades
- Further reduced pitch-back at high speed
- First implementation of ground effect and VRS introduced
- Added experimental N2 and TOT simulation

0.3 26 June 2020
- Added GAU-19 .50 cal gattling gun
- Added basic Force Feedback support
- Added option to remove weapon platform
- Added trim controls
- Added controls indicator
- M134 minigun now drops spent shells
- EFM:
- labeled data tables so they are easier to understand their affect on the FM
- reduced rotational instability during straight and level flight

0.21 20 June 2020
- Fixed throttle wrap-around when using keyboard
- Added throttle axis assignment
- Changed to have full rpm on hot start

0.2 16 June 2020
- Added M880 digital clock display
- Removed some unused key commands + fixed missing view commands for joystick
- Moved engine and electric system from lua to EFM to provide better working example
- Reduced pitch back at high speed
- Cleaned up source code and reorganized
- Updated DCS API (wHumanCustomPhysicsAPI) with newest version
 
0.11 23 March 2020
- Added more network animations for multiplayer
- Added option for copilot to take control
- Added Mainpanel layout diagram in Doc folder

0.1 22 March 2020
- Initial Release
