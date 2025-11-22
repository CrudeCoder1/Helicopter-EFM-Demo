#pragma once

//params handling
typedef void * (*PFN_ED_COCKPIT_GET_PARAMETER_HANDLE        )  (const char * name);
typedef void   (*PFN_ED_COCKPIT_UPDATE_PARAMETER_WITH_STRING)  (void * handle, const char * string_value);
typedef void   (*PFN_ED_COCKPIT_UPDATE_PARAMETER_WITH_NUMBER)  (void * handle, double number_value);
typedef bool   (*PFN_ED_COCKPIT_PARAMETER_VALUE_TO_NUMBER   )  (const void * handle, double & res, bool interpolated);
typedef bool   (*PFN_ED_COCKPIT_PARAMETER_VALUE_TO_STRING   )  (const void * handle, char * buffer, unsigned buffer_size);
typedef int    (*PFN_ED_COCKPIT_COMPARE_PARAMETERS          )  (void * handle_1, void * handle_2);

typedef void   (*PFN_ED_COCKPIT_DISPATCH_ACTION_TO_DEVICE   )  (int device, int command, double value);
typedef void   (*PFN_ED_COCKPIT_SET_DRAW_ARGUMENT)  (int arg, float value);
typedef float  (*PFN_ED_COCKPIT_GET_DRAW_ARGUMENT)  (int arg);
typedef void   (*PFN_ED_COCKPIT_SET_EXTERNAL_DRAW_ARGUMENT)  (int arg, float value);
typedef float  (*PFN_ED_COCKPIT_GET_EXTERNAL_DRAW_ARGUMENT)  (int arg);
 
struct cockpit_param_api
{
    PFN_ED_COCKPIT_GET_PARAMETER_HANDLE             get_parameter_handle;      
    PFN_ED_COCKPIT_UPDATE_PARAMETER_WITH_STRING     update_parameter_with_string;
    PFN_ED_COCKPIT_UPDATE_PARAMETER_WITH_NUMBER     update_parameter_with_number;
    PFN_ED_COCKPIT_PARAMETER_VALUE_TO_NUMBER        parameter_value_to_number;  
    PFN_ED_COCKPIT_PARAMETER_VALUE_TO_STRING        parameter_value_to_string;
    PFN_ED_COCKPIT_COMPARE_PARAMETERS               compare_parameters;

    PFN_ED_COCKPIT_DISPATCH_ACTION_TO_DEVICE        dispatch_action_to_device;
    PFN_ED_COCKPIT_SET_DRAW_ARGUMENT                set_draw_argument;
    PFN_ED_COCKPIT_GET_DRAW_ARGUMENT                get_draw_argument;
    PFN_ED_COCKPIT_SET_EXTERNAL_DRAW_ARGUMENT       set_external_draw_argument;
    PFN_ED_COCKPIT_GET_EXTERNAL_DRAW_ARGUMENT       get_external_draw_argument;
};
 
inline cockpit_param_api  ed_get_cockpit_param_api()
{
    HMODULE cockpit_dll                 = GetModuleHandleA("CockpitBase.dll"); 
 
    cockpit_param_api ret;
    ret.get_parameter_handle            = (PFN_ED_COCKPIT_GET_PARAMETER_HANDLE)        GetProcAddress(cockpit_dll,"ed_cockpit_get_parameter_handle");
    ret.update_parameter_with_number    = (PFN_ED_COCKPIT_UPDATE_PARAMETER_WITH_NUMBER)GetProcAddress(cockpit_dll,"ed_cockpit_update_parameter_with_number");
    ret.update_parameter_with_string    = (PFN_ED_COCKPIT_UPDATE_PARAMETER_WITH_STRING)GetProcAddress(cockpit_dll,"ed_cockpit_update_parameter_with_string");
    ret.parameter_value_to_number       = (PFN_ED_COCKPIT_PARAMETER_VALUE_TO_NUMBER)   GetProcAddress(cockpit_dll,"ed_cockpit_parameter_value_to_number");
    ret.parameter_value_to_string       = (PFN_ED_COCKPIT_PARAMETER_VALUE_TO_STRING)   GetProcAddress(cockpit_dll,"ed_cockpit_parameter_value_to_string");  
    ret.compare_parameters              = (PFN_ED_COCKPIT_COMPARE_PARAMETERS)          GetProcAddress(cockpit_dll,"ed_cockpit_compare_parameters");

    ret.dispatch_action_to_device       = (PFN_ED_COCKPIT_DISPATCH_ACTION_TO_DEVICE)   GetProcAddress(cockpit_dll,"ed_cockpit_dispatch_action_to_device");
    ret.set_draw_argument               = (PFN_ED_COCKPIT_SET_DRAW_ARGUMENT)           GetProcAddress(cockpit_dll,"ed_cockpit_set_draw_argument");
    ret.get_draw_argument               = (PFN_ED_COCKPIT_GET_DRAW_ARGUMENT)           GetProcAddress(cockpit_dll,"ed_cockpit_get_draw_argument");
    ret.set_external_draw_argument      = (PFN_ED_COCKPIT_SET_EXTERNAL_DRAW_ARGUMENT)  GetProcAddress(cockpit_dll,"ed_cockpit_set_external_draw_argument");
    ret.get_external_draw_argument      = (PFN_ED_COCKPIT_GET_EXTERNAL_DRAW_ARGUMENT)  GetProcAddress(cockpit_dll,"ed_cockpit_get_external_draw_argument");

    return ret;
}


/*
all functions from CockpitBase.dll:
///--not sure if useful
ed_cockpit_dispatch_action_analog
ed_cockpit_dispatch_action_digital
ed_cockpit_external_lights_get_child
ed_cockpit_external_lights_get_reference
ed_cockpit_internal_lights_get_reference
ed_cockpit_internal_lights_get_reference_get_color
ed_cockpit_internal_lights_get_reference_set_attenuation
ed_cockpit_internal_lights_get_reference_set_color
ed_cockpit_internal_lights_get_reference_set_cone
ed_cockpit_close_lua_state
ed_cockpit_open_lua_state
ed_cockpit_set_action_analog
ed_cockpit_set_action_digital
ed_cockpit_set_action_to_device
ed_cockpit_set_external_lights_power
ed_cockpit_get_external_lights_power
ed_cockpit_track_is_reading
ed_cockpit_track_is_writing
ed_module_initialize

///-- Could be useful
ed_cockpit_aircraft_get_property
ed_cockpit_get_base_sensor_output
ed_cockpit_get_self_airspeed
ed_cockpit_get_self_coordinates
ed_cockpit_get_self_velocity



///--New working functions
ed_cockpit_set_draw_argument          // figured out by Nibbylot (same format as lua function)
ed_cockpit_get_draw_argument
ed_cockpit_set_external_draw_argument // figured out by Nibbylot (same format as lua function)
ed_cockpit_get_external_draw_argument
ed_cockpit_dispatch_action_to_device // figured out by Freebird

////provided by default
ed_cockpit_get_parameter_handle         //provided by default
ed_cockpit_parameter_value_to_number    //default
ed_cockpit_parameter_value_to_string    //default
ed_cockpit_update_parameter_with_number //default
ed_cockpit_update_parameter_with_string //default
ed_cockpit_compare_parameters           //default
*/