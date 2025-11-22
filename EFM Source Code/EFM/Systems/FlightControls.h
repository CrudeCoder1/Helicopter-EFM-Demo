#pragma once

// Flight controls in the AH-6J are very simple. Other aircraft will require hydraulics and flight control systems (FCS, SAS etc.)

class FlightControls
{
private:
	
public:
	FlightControls() {}
	~FlightControls() {}

	EDPARAM cockpitAPI;

	double CollectiveInput = 0.0;// Raw collective input
	double PedalInput = 0.0;	// Raw pedal input	
	double PitchInput = 0.0;	// Raw pitch input
	double RollInput = 0.0;		// Raw roll input

	double rollTrim = 0.0;
	double pitchTrim = 0.0;

	double pitchOutput = 0.0;
	double rollOutput = 0.0;

	// These are used for the controls indicator
	void* PITCH_INPUT = cockpitAPI.getParamHandle("PITCH_INPUT");
	void* ROLL_INPUT = cockpitAPI.getParamHandle("ROLL_INPUT");
	void* PEDAL_INPUT = cockpitAPI.getParamHandle("PEDAL_INPUT");
	void* COLLECTIVE_INPUT = cockpitAPI.getParamHandle("COLLECTIVE_INPUT");

	void init()
	{
		CollectiveInput = 0.0;
		PedalInput = 0.0;
		PitchInput = 0.0;
		RollInput = 0.0;
		rollTrim = 0.0;
		pitchTrim = 0.0;
	}

	void release()
	{
		CollectiveInput = 0.0;
		PedalInput = 0.0;
		PitchInput = 0.0;
		RollInput = 0.0;
		rollTrim = 0.0;
		pitchTrim = 0.0;
	}
	/*
	void setCommand(int command, const float value)
	{
		if (command == JoystickRoll)
		{
			RollInput = limit(value, -1.0, 1.0);
		}
		else if (command == JoystickPitch)
		{
			PitchInput = limit(-value, -1.0, 1.0);
		}
		else if (command == JoystickYaw)
		{
			PedalInput = limit(-value, -1.0, 1.0);
		}
		else if (command == JoystickThrottle)
		{
			CollectiveInput = limit(((-value + 1.0) / 2.0), 0.0, 1.0);
		}

		else if (command == trimUp)
		{
			pitchTrim = limit(flightControls.pitchTrim - 0.0015, -1, 1);
		}
		else if (command == trimDown)
		{
			pitchTrim = limit(flightControls.pitchTrim + 0.0015, -1, 1);
		}
		else if (command == trimLeft)
		{
			rollTrim = limit(flightControls.rollTrim - 0.0015, -1, 1);
		}
		else if (command == trimRight)
		{
			rollTrim = limit(flightControls.rollTrim + 0.0015, -1, 1);
		}
	}*/

	void update()
	{
		pitchOutput = limit(PitchInput + pitchTrim, -1, 1);
		rollOutput = limit(RollInput + rollTrim, -1, 1);

		//----- for lua Control Indicator ------------------------
		cockpitAPI.setParamNumber(PITCH_INPUT, pitchOutput);
		cockpitAPI.setParamNumber(ROLL_INPUT, rollOutput);
		cockpitAPI.setParamNumber(PEDAL_INPUT, PedalInput);
		cockpitAPI.setParamNumber(COLLECTIVE_INPUT, CollectiveInput);
	}

};