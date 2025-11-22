#pragma once

#include "ccParametersAPI.h"


class EDPARAM
{
public:
	EDPARAM()
	{
		ed_param_api = ed_get_cockpit_param_api();
	}
	cockpit_param_api ed_param_api;

	void* getParamHandle(const char* name)
	{
		return ed_param_api.get_parameter_handle(name);
	}

	void setParamNumber(void* handle, double value)
	{
		ed_param_api.update_parameter_with_number(handle, value);
	}

	void setParamString(void* handle, const char* string)
	{
		ed_param_api.update_parameter_with_string(handle, string);
	}

	double getParamNumber(void* handle)
	{
		double res{ 0 };
		ed_param_api.parameter_value_to_number(handle, res, false);
		return res;
	}

	void getParamString(void* handle, char* buffer, unsigned int buffer_size)
	{
		ed_param_api.parameter_value_to_string(handle, buffer, buffer_size);
	}

	int compareParams(void* handle1, void* handle2)
	{
		return ed_param_api.compare_parameters(handle1, handle2);
	}

	void dispatchAction(int device, int command, double value)
	{
		ed_param_api.dispatch_action_to_device(device, command, value);
	}

	void setCockpitDrawArg(int arg, float value)
	{
		ed_param_api.set_draw_argument(arg, value);
	}
	float getCockpitDrawArg(int arg)
	{
		return ed_param_api.get_draw_argument(arg);
	}
	void setExternalDrawArg(int arg, float value)
	{
		ed_param_api.set_external_draw_argument(arg, value);
	}
	float getExternalDrawArg(int arg)
	{
		return ed_param_api.get_external_draw_argument(arg);
	}

};
