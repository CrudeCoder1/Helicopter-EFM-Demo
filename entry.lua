local self_ID = "AH-6"
declare_plugin(self_ID,
{
dirName			= current_mod_path,
displayName		= _("AH-6J Little Bird"),
fileMenuName	= _("AH-6J"),
--version 		= "0.4",
state			= "installed",
info			= _("The AH-6J Little Bird is a light attack helicopter used by the 160th Special Operations Aviation Regiment (SOAR) of the US Army."),

binaries =
{ 
'AH6',
},
Skins =
{
	{
		name	= "AH-6J",
		dir		= "Skins/1"
	},
},
Missions =
{
	{
		name	= _("AH-6"),
		dir		= "Missions",
	},
},
LogBook =
{
	{
		name	= _("AH-6J"),
		type	= "AH-6J",
	},
},
InputProfiles = 
{
	["AH-6"] = current_mod_path .. '/Input',	
},
Options =
{
    {
        name		= _("AH-6J"),
        nameId		= "AH-6J",
        dir			= "Options",
        CLSID		= "{AH-6J options}"
    },
},
})
-------------------------------------------------------------------------------
mount_vfs_model_path(current_mod_path.."/Shapes")
--mount_vfs_liveries_path (current_mod_path.."/Liveries")
mount_vfs_texture_path(current_mod_path.."/Textures")
mount_vfs_texture_path(current_mod_path.."/Textures/Avionics")--for textures used in cockpit systems i.e. digital fonts
mount_vfs_texture_path(current_mod_path.."/Skins/1/ME")

dofile(current_mod_path..'/AH-6.lua')
dofile(current_mod_path..'/Weapons/AH6_Weapons.lua')
dofile(current_mod_path..'/UnitPayloads/AH-6.lua')
dofile(current_mod_path.."/Views.lua")
make_view_settings('AH-6', ViewSettings, SnapViews)

local FM =
{
	[1] = self_ID,
	[2] = "AH6", -- name of dll
	center_of_mass = {0,0,0},  -- center of mass position relative to object 3d model center for empty aircraft
	moment_of_inertia = {446, 979, 1219, 128}, -- moment of inertia of empty (Ixx,Iyy,Izz,Ixz)	in kgm^2 
}

make_flyable('AH-6',current_mod_path..'/Cockpit/Scripts/', FM, current_mod_path..'/comm.lua')

plugin_done()