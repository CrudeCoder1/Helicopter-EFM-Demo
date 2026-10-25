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

	// 'Pilot Trimmer' like the UH-1H: hold the button, move the stick to where you want it to be centred (usually back to the
	// centre), then release. The cyclic position you held when you pressed the button stays, and the stick now works
	// relative to where it was when you released the button.
	double pitchRef = 0.0;		// stick position at the last trim release
	double rollRef = 0.0;
	bool trimHeld = false;
	double frozenPitch = 0.0;	// cyclic output held while the trimmer button is down
	double frozenRoll = 0.0;

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
		pitchRef = 0.0;
		rollRef = 0.0;
		trimHeld = false;
	}

	// 'Pilot Trimmer' button: value > 0.5 = pressed, otherwise released
	void setTrimmer(const float value)
	{
		if (value > 0.5f)
		{
			if (!trimHeld)
			{
				trimHeld = true;
				frozenPitch = pitchOutput;
				frozenRoll = rollOutput;
			}
		}
		else if (trimHeld)
		{
			trimHeld = false;
			pitchTrim = frozenPitch;	// keep the held cyclic position as the new neutral
			rollTrim = frozenRoll;
			pitchRef = PitchInput;		// the stick is relative to where it is now
			rollRef = RollInput;
		}
	}

	// 'Trimmer reset': cyclic neutral back to the stick centre
	void resetTrim()
	{
		pitchTrim = 0.0;
		rollTrim = 0.0;
		pitchRef = 0.0;
		rollRef = 0.0;
		trimHeld = false;
	}

	void release()
	{
		CollectiveInput = 0.0;
		PedalInput = 0.0;
		PitchInput = 0.0;
		RollInput = 0.0;
		rollTrim = 0.0;
		pitchTrim = 0.0;
		pitchRef = 0.0;
		rollRef = 0.0;
		trimHeld = false;
	}

	void setPedLeft()
	{
		PedalInput = limit(PedalInput + 0.008, -1.0, 1.0);
	}
	void setPedRight()
	{
		PedalInput = limit(PedalInput - 0.008, -1.0, 1.0);
	}
	void setPedStop()
	{
		PedalInput = limit(PedalInput, -0.5, 0.5);
	}
	void setCollUp()
	{
		CollectiveInput = limit(CollectiveInput + 0.007, 0, 1.0);
	}
	void setCollDown()
	{
		CollectiveInput = limit(CollectiveInput - 0.007, 0, 1.0);
	}
	void setCyclicForward()
	{
		PitchInput = limit(PitchInput + 0.007, -1.0, 1.0);
	}
	void setCyclicBack()
	{
		PitchInput = limit(PitchInput - 0.007, -1.0, 1.0);
	}
	void setCyclicLeft()
	{
		RollInput = limit(RollInput - 0.007, -1.0, 1.0);
	}
	void setCyclicRight()
	{
		RollInput = limit(RollInput + 0.007, -1.0, 1.0);
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

	// Stick to control mapping with trim. A real trimmed stick still reaches both mechanical stops: trim moves where the
	// stick rests, it does not take away travel. So the output is scaled on each side of the trimmed position so that full
	// forward stick always gives full forward control and full aft stick full aft control, whatever the trim.
	// (Before, output = stick + trim, so +72% forward trim left only -28% of aft control at full aft stick.)
	static double trimmedAxis(double stick, double ref, double trim)
	{
		double u = stick - ref;						// stick movement from the position it had when the trim was set
		if (u >= 0.0)
		{
			double room = 1.0 - ref;				// stick travel left towards the forward stop
			if (room < 0.05) room = 0.05;
			return trim + u * (1.0 - trim) / room;	// reaches +1.0 at full forward stick
		}
		double room = 1.0 + ref;					// stick travel left towards the aft stop
		if (room < 0.05) room = 0.05;
		return trim + u * (1.0 + trim) / room;		// reaches -1.0 at full aft stick
	}
	void update()
	{
		if (trimHeld)
		{
			pitchOutput = frozenPitch;
			rollOutput = frozenRoll;
		}
		else
		{
			pitchOutput = limit(trimmedAxis(PitchInput, pitchRef, pitchTrim), -1, 1);
			rollOutput = limit(trimmedAxis(RollInput, rollRef, rollTrim), -1, 1);
		}

		//----- for lua Control Indicator ------------------------
		cockpitAPI.setParamNumber(PITCH_INPUT, pitchOutput);
		cockpitAPI.setParamNumber(ROLL_INPUT, rollOutput);
		cockpitAPI.setParamNumber(PEDAL_INPUT, PedalInput);
		cockpitAPI.setParamNumber(COLLECTIVE_INPUT, CollectiveInput);
	}

};