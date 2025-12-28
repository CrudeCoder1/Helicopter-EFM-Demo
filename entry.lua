local self_ID = "AH-6J"
declare_plugin(self_ID,
{
dirName			= current_mod_path,
displayName		= _("AH-6J Little Bird"),
fileMenuName	= _("AH-6J"),
state			= "installed",
--developerName	= _(" "),
info			= _("The AH-6J Little Bird is a light attack helicopter used by the 160th Special Operations Aviation Regiment (SOAR) of the US Army."),
encyclopedia_path = current_mod_path..'/Encyclopedia',

binaries =
{ 
'AH6J',
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
		name	= _("AH-6J"),
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
	["AH-6J"] = current_mod_path .. '/Input',	
},
Options =
{
    {
        name		= _("AH-6J"),
        nameId		= "AH-6J",
        dir			= "Options",
        CLSID		= "{AH-6J options}",
		--allow_in_simulation = true
    },
},
})
-------------------------------------------------------------------------------
mount_vfs_model_path(current_mod_path.."/Shapes")
mount_vfs_model_path(current_mod_path.."/Cockpit/Shapes")
--mount_vfs_liveries_path (current_mod_path.."/Liveries")
mount_vfs_texture_path(current_mod_path.."/Textures")
mount_vfs_texture_path(current_mod_path.."/Textures/AH6J_ExternalTextures")
mount_vfs_texture_path(current_mod_path.."/Cockpit/Textures/AH6J_CockpitTextures")
mount_vfs_texture_path(current_mod_path.."/Cockpit/Textures/Avionics")--for textures used in cockpit systems i.e. digital fonts
mount_vfs_texture_path(current_mod_path.."/Skins/1/ME")

dofile(current_mod_path..'/AH-6J.lua')
dofile(current_mod_path..'/Weapons/AH6_Weapons.lua')
dofile(current_mod_path..'/UnitPayloads/AH-6.lua')
dofile(current_mod_path.."/Views.lua")
make_view_settings('AH-6J', ViewSettings, SnapViews)

local cfg_path = current_mod_path.."/FM_Config.lua"
dofile(cfg_path)
EFM[1] 				= self_ID
EFM[2] 				= 'AH6J'
EFM.config_path 	= cfg_path

make_flyable('AH-6J',current_mod_path..'/Cockpit/Scripts/', EFM, current_mod_path..'/comm.lua')

plugin_done()