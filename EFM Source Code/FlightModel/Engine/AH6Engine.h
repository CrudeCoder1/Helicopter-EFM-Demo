#pragma once

/* Start sequence based on OH-6 manual
OH-6 start sequence:
batt on (or ext for ground elec power start)
press and hold starter button
wait until N1 reaches 15%
open throttle to idle pos
release starter at 58% N1
final N1 62%

engine run up:
throttle to full open, N1 101%
*/


class AH6Engine
{
protected:

public:
		
	double throttleInput;	// Throttle input command 

	double N1_RPM; // gas producer/core
	double N2_RPM; // going to use this for NR(rotor rpm) for now bc they are very similar
	
	bool engineRunning;
	bool isStarterPressed;

	// oil pressure: 0-200 psi
	double oilPressure;
	// cockpit gauge: 300..900 range
	double turbineTemperature;
	bool hotStart;
	bool engineFire;

	double torque;

	double rotorPosition; // animation position

	EDPARAM cockpitAPI;

	AH6Engine() 
		: throttleInput(0)
		, N1_RPM(0)
		, N2_RPM(0)
		, engineRunning(false)
		, isStarterPressed(false)
		, oilPressure(0)
		, turbineTemperature(20)
		, hotStart(false)
		, engineFire(false)
		, torque(0)
		, rotorPosition(0)
			
	{}
	~AH6Engine() {}

	void* N2RPM = cockpitAPI.getParamHandle("N2_RPM"); // for use in digital gauge
	void* TOT = cockpitAPI.getParamHandle("TOT"); // for use in digital gauge
	void* TRQ = cockpitAPI.getParamHandle("TRQ"); // for use in digital gauge

	void initCold()
	{
		throttleInput = 0;
		N1_RPM = 0;
		N2_RPM = 0;
		isStarterPressed = false;
		engineRunning = false;
		rotorPosition = 0;
		turbineTemperature = 15;
		hotStart = false;
		engineFire = false;
	}
	void initHot()
	{
		throttleInput = 1;
		N1_RPM = 100;
		N2_RPM = 100;
		isStarterPressed = false;
		engineRunning = true;
		rotorPosition = 0;
		turbineTemperature = 600;
		hotStart = false;
		engineFire = false;
	}

	void setThrottleInput(const double value)
	{
		throttleInput = value;
		throttleInput = limit(throttleInput, 0, 1);
	}
	void setStarterButton(const float value)
	{
		if (value == 1)
		{
			isStarterPressed = true;
			return;
		}
		isStarterPressed = false;
	}

	double getCoreRelatedRPM()
	{
		return N1_RPM / 100;
	}

	void spinRotor(const double frameTime)
	{
		rotorPosition += (N2_RPM /100) * frameTime * 7.8; //470rpm=~ 7.83 rev per sec
		if (rotorPosition > 1)
		{
			rotorPosition = 0;
		}
	}
	void turnStarter(double frameTime)
	{
		N1_RPM += frameTime * 3; // will get to 15% in 5 seconds
		N1_RPM = limit(N1_RPM, 0, 20); // starter wont turn engine past 20% or so
	}
	void spoolUp(double frameTime)
	{
		N1_RPM += frameTime * 3.4; 
		N1_RPM = limit(N1_RPM, 0, 62);
	}
	void spoolDown(double frameTime)
	{
		N1_RPM -= frameTime * 5;
		N1_RPM = limit(N1_RPM, 0, 101);
	}
	void updateTOT(double frameTime, bool isFuelFlow, double OutsideAirTemp)
	{
		double TOTtarget;
		if (turbineTemperature > OutsideAirTemp && isFuelFlow == false)
		{
			if (N1_RPM > 1) // engine isn't ignited but still turning, causing air to move and cool engine faster
			{
				if (turbineTemperature > 260)
				{
					turbineTemperature -= (turbineTemperature - OutsideAirTemp) * frameTime * 0.08;
				}
				else
				{
					turbineTemperature -= frameTime * 5.2;
				}
			}
			else // engine cooling off due to lower ambient air
			{
				turbineTemperature -= frameTime * 1.7;
			}
		}
		else if (isFuelFlow == true)
		{
			if (N1_RPM < 58) // starting
			{
				if (N1_RPM < 32)
				{
					TOTtarget = 730;
					if (hotStart == true)
					{
						TOTtarget = 1200;
					}
					turbineTemperature += (TOTtarget - turbineTemperature) * frameTime;
				}
				else if (N1_RPM > 37) // temp will stay high for a short period of time before decending
				{
					turbineTemperature -= frameTime * 4;
				}
			}
			else // running
			{
				TOTtarget = (N1_RPM - 58) / 0.42 + 550; // linear 550-650 for now
				if (turbineTemperature > TOTtarget)
				{
					turbineTemperature -= frameTime * 4;
				}
				else 
				{
					turbineTemperature += frameTime * 4;
				}
			}
		}
		turbineTemperature = limit(turbineTemperature, 0, 1200);
		cockpitAPI.setParamNumber(TOT, turbineTemperature);

		if (turbineTemperature > 1000)
		{
			engineFire = true;
		}
	}
	void updateTorque(double frameTime, double collective)
	{
		torque = ((N2_RPM-70)/.39) * 0.68; // just linear based on N2
		cockpitAPI.setParamNumber(TRQ, torque);
	}

	void update(const double frameTime, const bool isFuelFlow, const bool isDCpower, const double OAT, const double collective)
	{

		if (N1_RPM >= 58 && isFuelFlow)
		{
			engineRunning = true;
		}
		else
		{
			engineRunning = false;
		}

		if (isStarterPressed && isDCpower)
		{
			if (isFuelFlow == true && N1_RPM > 13)
			{
				spoolUp(frameTime);
			}
			else
			{
				turnStarter(frameTime);
			}
		}
		else if (N1_RPM > 58 && N1_RPM < 62 && engineRunning)
		{
			spoolUp(frameTime);
		}
		else if (engineRunning == false)
		{
			spoolDown(frameTime);
		}

		if (engineRunning && N1_RPM >= 62)
		{
			N1_RPM = (39 * throttleInput) + 62;
			N1_RPM = limit(N1_RPM, 0, 101);
		}


		// N2 will try to reach the rpm of 15-90%N1 for 0-100%N2 
		// this will allow the rotor to start moving at 15%N1
		double targetRPM = 0;
		targetRPM = (N1_RPM - 15) / 0.75;
		targetRPM = limit(targetRPM, 0, 109);
		double growthRate = 0;
		if ((targetRPM - N2_RPM) > 40.0)
		{
			growthRate = 0.4 * 40;
		}
		else
		{
			growthRate = 0.4 * (targetRPM - N2_RPM);
		}
		N2_RPM += (growthRate * frameTime);
		N2_RPM = limit(N2_RPM, 0.0, 109.0);
		
		cockpitAPI.setParamNumber(N2RPM, N2_RPM);

		spinRotor(frameTime);
		updateTOT(frameTime, isFuelFlow, OAT);
		updateTorque(frameTime, collective);
	}
};
