#pragma once


//Electrical System is based on the OH - 6A Cayuse operation manual
class AH6ElectricSystem
{
protected:
	bool isBatteryOn;
	bool isExternalPwrOn;
	bool isGeneratorOn;
	bool isInverterOn;

	double batteryVoltage;

	EDPARAM cockpitAPI;

public:
	bool isDC_busPowered;
	bool isAC_busPowered;

	AH6ElectricSystem() 
		: isBatteryOn(false)
		, isExternalPwrOn(false)
		, isGeneratorOn(false)
		, isInverterOn(false)
		, batteryVoltage(24)
		, isDC_busPowered(false)
		, isAC_busPowered(false)
	{}
	~AH6ElectricSystem() {}
		
	void* external_power = cockpitAPI.getParamHandle("EXTERNAL_POWER"); // to get ground power status from lua functions
	void* dc_power_avail = cockpitAPI.getParamHandle("DC_POWER_AVAIL"); // for use in lua scripts, i.e. digital displays
	void* ac_power_avail = cockpitAPI.getParamHandle("AC_POWER_AVAIL");

	void initCold()
	{
		isBatteryOn = false;
		isExternalPwrOn = false;
		isGeneratorOn = false;
		isInverterOn = false;
		batteryVoltage = 24;
		isDC_busPowered = false;
		isAC_busPowered = false;
	}
	void initHot()
	{
		isBatteryOn = true;
		isExternalPwrOn = false;
		isGeneratorOn = true;
		isInverterOn = true;
		batteryVoltage = 24;
		isDC_busPowered = true;
		isAC_busPowered = true;
	}

	void setBatteryOn(const float value)
	{
		if (value == 1)
		{
			isBatteryOn = true;
			isExternalPwrOn = false;
			return;
		}
		if (value == -1)
		{
			isExternalPwrOn = true;
			isBatteryOn = false;
			return;
		}
		isBatteryOn = false;
		isExternalPwrOn = false;
	}
	void setGeneratorOn(const float value)
	{
		if (value == 1)
		{
			isGeneratorOn = true;
			return;
		}
		isGeneratorOn = false;
	}
	void setInverterOn(const float value)
	{
		if (value == 1)
		{
			isInverterOn = true;
			return;
		}
		isInverterOn = false;
	}
	

	void update(const double frameTime, const double rpm)
	{
		if (rpm >= 62 && isGeneratorOn)
		{
			isDC_busPowered = true;
			cockpitAPI.setParamNumber(dc_power_avail, 1);
			if (batteryVoltage < 24)
			{
				batteryVoltage += frameTime / 60;
			}
		}
		else if (isBatteryOn && batteryVoltage > 22)
		{
			isDC_busPowered = true;
			cockpitAPI.setParamNumber(dc_power_avail, 1);
			batteryVoltage -= frameTime / 150;
		}
		else if (isExternalPwrOn && cockpitAPI.getParamNumber(external_power) == 1)
		{
			isDC_busPowered = true;
			cockpitAPI.setParamNumber(dc_power_avail, 1);
		}
		else
		{
			isDC_busPowered = false;
			cockpitAPI.setParamNumber(dc_power_avail, 0);
		}


		if (isDC_busPowered == true && isInverterOn)
		{
			isAC_busPowered = true;
			cockpitAPI.setParamNumber(ac_power_avail, 1);
		}
		else
		{
			isAC_busPowered = false;
			cockpitAPI.setParamNumber(ac_power_avail, 0);
		}

	}

};