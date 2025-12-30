#pragma once


#include "Systems/ElectricSystem.h"
#include "GlobalParams.h"



class LightSystem
{
private:
	EDPARAM cockpitAPI;

	bool testSwOn{ false };
	bool warningLightsDim{ false };
	//bool cautionLightPrevState[41]{ false };
	//bool masterCautionTrigger{ false };

	void* DC_Bus_Voltage = cockpitAPI.getParamHandle("DC_Bus_Voltage");
public:

	LightSystem() {}
	~LightSystem() {}

	void init()
	{	
		warningLightsDim = false;		
		testSwOn = false;		
	}

	void setCommand(int command, const float value)
	{
		if (command == cautionTest)
		{
			testSwOn = value > 0;
		}						
	}


	void updateFrame()
	{
		updateWarningLights();			
	}
	

	void updateWarningLights()
	{
		cockpitAPI.setCockpitDrawArg(INT_CautionBrightness, warningLightsDim ? 0.2f : 1.0f);

		if (cockpitAPI.getParamNumber(DC_Bus_Voltage)>12.0)
		{
			
			if (testSwOn)
			{
				for (int i = 0; i < CL_MaxNumber; i++)
				{
					cockpitAPI.setCockpitDrawArg(i + 411, 1.0f);
				}
				//cockpitAPI.setCockpitDrawArg(INT_MasterCautionLight, 1.0f);
			}
			else
			{
				for (int i = 0; i < CL_MaxNumber; i++)
				{
					cockpitAPI.setCockpitDrawArg(i + 411, G_Params.cautionLight[i] ? 1.0f : 0.0f);//turn on light if needed

					//if (cautionLightPrevState[i] != G_Params.cautionLight[i] && G_Params.cautionLight[i])
					//{
					//	masterCautionTrigger = true;
					//}
					//cautionLightPrevState[i] = G_Params.cautionLight[i];					
				}
				//cockpitAPI.setCockpitDrawArg(INT_MasterCautionLight, masterCautionTrigger ? 1.0f : 0.0f);				
			}
		}
		else
		{
			for (int i = 0; i < CL_MaxNumber; i++)
			{
				cockpitAPI.setCockpitDrawArg(i + 411, 0.0f);
			}
		}
	}

	

};
