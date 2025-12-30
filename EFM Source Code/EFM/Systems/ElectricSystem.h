#pragma once

//TODO: ammeter current

// Note: Battery drain is only linear and not based on current draw.

// Note: in the A/MH-6J, the NiCad battery was initially installed, but eventually replaced with a Sealed Lead Acid Battery (SLAB).
// The only discernible difference is the temp warning lights are disabled for the SLAB
class SLAB_Battery // Sealed Lead Acid Battery
{
private:
	double chargePercent = 100.0;
	double batteryVoltage = 25.8; // 23.2-25.8 voltage range
	const double chargeTime = 8.0 * 60.0;// charge time in seconds  8min=480s charge
	const double dischargeTime = 15.0 * 60.0;// discharge time in seconds  15min=720s discharge

public:
	SLAB_Battery() {}
	~SLAB_Battery() {}

	void dischargeBattery(const double dt)
	{
		chargePercent -= dt * (100.0 / dischargeTime);
		chargePercent = limit(chargePercent, 0, 100);
		batteryVoltage = LinInterp(chargePercent, 0.0, 100.0, 23.2, 25.8);
	}
	void chargeBattery(const double dt)
	{
		chargePercent += dt * (100.0 / chargeTime);
		chargePercent = limit(chargePercent, 0, 100);
		batteryVoltage = LinInterp(chargePercent, 0.0, 100.0, 23.2, 25.8);
	}
	double getBatteryVoltage() const
	{
		return batteryVoltage;
	}
	void init()
	{
		chargePercent = 100.0;
		batteryVoltage = 25.8;
	}
};

class ElectricSystem
{
private:
	bool generatorSwOn = false;
	bool inverterSwOn = false;
	float powerSelSw = 0.0;
	double DCbusVoltage = 28.0;// 28 volts nominal
	double ACbus115Voltage = 115.0;// 115 volts nominal
	double ACbus26Voltage = 26.0;// 26 volts nominal
	//double currentDraw = 0.0;

	SLAB_Battery battery;

	EDPARAM cockpitAPI;


public:
	ElectricSystem() {}
	~ElectricSystem() {}

	void* external_power = cockpitAPI.getParamHandle("EXTERNAL_POWER"); // to get ground power status from lua functions
	void* DC_Bus_Voltage = cockpitAPI.getParamHandle("DC_Bus_Voltage");// for use in lua scripts
	void* AC_115_Bus_Voltage = cockpitAPI.getParamHandle("AC_115_Bus_Voltage");// for use in lua scripts
	void* AC_26_Bus_Voltage = cockpitAPI.getParamHandle("AC_26_Bus_Voltage");// for use in lua scripts
		
	void initCold()
	{
		generatorSwOn = false;
		inverterSwOn = false;
		powerSelSw = 0.0;
		DCbusVoltage = 0.0;
		ACbus115Voltage = 0.0;
		ACbus26Voltage = 0.0;
		battery.init();
	}
	void initHot()
	{
		generatorSwOn = true;
		inverterSwOn = true;
		powerSelSw = 1.0;
		DCbusVoltage = 28.0;
		ACbus115Voltage = 115.0;
		ACbus26Voltage = 26.0;
		battery.init();
	}
/*
   void setCommand(int command, const float value)
   {
	   if (command == batterySwitch)
	   {
		   powerSelSw = value;
	   }
	   else if (command == generatorSwitch)
	   {
		   generatorSwOn = value == 1.0f;
	   }
	   else if (command == inverterSwitch)
	   {
		   inverterSwOn = value == 1.0f;
	   }
   }*/

	void setPowerSw(const float value)
	{
		powerSelSw = value;
	}
	void setGeneratorSw(const float value)
	{
		generatorSwOn = value == 1.0f;
	}
	void setInverterSw(const float value)
	{
		inverterSwOn = value == 1.0f;
	}
	
	void update(const double dt, const double rpm)
	{
		bool extPwrOn = cockpitAPI.getParamNumber(external_power) == 1;
		double generatorVoltage = generatorSwOn ? LinInterp(rpm, 0.55, 0.62, 0.0, 28.0) : 0.0;

		if (powerSelSw < 0 && extPwrOn)
		{
			DCbusVoltage = 28.0;
			battery.chargeBattery(dt);
		}
		else if (powerSelSw > 0)
		{
			DCbusVoltage = std::max(battery.getBatteryVoltage(), generatorVoltage);
			if (generatorVoltage > battery.getBatteryVoltage())
			{
				battery.chargeBattery(dt);
			}
			else
			{
				battery.dischargeBattery(dt);
			}
		}
		else
		{
			DCbusVoltage = 0.0;
		}
		

		if (inverterSwOn)
		{
			ACbus115Voltage = LinInterp(DCbusVoltage, 10.0, 22.0, 0.0, 115.0);
			ACbus26Voltage = LinInterp(DCbusVoltage, 10.0, 22.0, 0.0, 26.0);
		}
		else
		{
			ACbus115Voltage = 0.0;
			ACbus26Voltage = 0.0;
		}

		cockpitAPI.setParamNumber(DC_Bus_Voltage, DCbusVoltage);
		cockpitAPI.setParamNumber(AC_115_Bus_Voltage, ACbus115Voltage);
		cockpitAPI.setParamNumber(AC_26_Bus_Voltage, ACbus26Voltage);

		G_Params.cautionLight[CL_GenOut] = generatorVoltage < battery.getBatteryVoltage();
		
	}

};