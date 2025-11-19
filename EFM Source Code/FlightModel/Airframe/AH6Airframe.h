#pragma once

// - nav lights
// - formation lights
// - landing lights
// - damage in sections 

class AH6Airframe
{
protected:
		
	// TODO: support for each lamp in lights?
	//bool navigationLight[10];
	//bool formationLight[8];
	//bool landingLamp[5];

	// current 3D model has three lights:
	// left, right and back (tail)
	// 0=off, 1=on
	float leftWingLamp;
	float rightWingLamp;
	float backTailLamp;

	bool navigationLights;
	bool formationLights;
	bool landingLights;
	bool strobeLights;

	// damage status of section in 0.01 increments
	double elementIntegrity[111]; // store each damage element 
	bool isImmortal; // <- ignore damage

public:

	double rotorIntegrityFactor; // basic damage model to decrease lift if blades are missing
	double tailRotorIntegrityFactor;

	AH6Airframe() 
		: leftWingLamp(0)
		, rightWingLamp(0)
		, backTailLamp(0)
		, navigationLights(false)
		, formationLights(false)
		, landingLights(false)
		, strobeLights(false)
		, isImmortal(false)
		, rotorIntegrityFactor(1)
		, tailRotorIntegrityFactor(1)
	{

	}
	~AH6Airframe() {}

	void init()
	{
		rotorIntegrityFactor = 1;
		tailRotorIntegrityFactor = 1;
		for (int i = 0; i < 111; i++)
		{
			elementIntegrity[i] = 1.0;
		}
	}

	float isNavigationLight() const
	{
		return (navigationLights == true) ? 1.0f : 0.0f;
	}
	float isFormationLight() const
	{
		return (formationLights == true) ? 1.0f : 0.0f;
	}
	float isLandingLight() const
	{
		return (landingLights == true) ? 1.0f : 0.0f;
	}

	// Scripts\Aircrafts\_Common\Damage.lua	 for element numbers
	void onAirframeDamage(int Element, double element_integrity_factor)
	{
		if (Element >= 0 && Element < 111)
		{
			elementIntegrity[Element] = element_integrity_factor;
		}

		if (isImmortal == false)
		{
			rotorIntegrityFactor = (elementIntegrity[65] + elementIntegrity[68] + elementIntegrity[71] + elementIntegrity[74] + elementIntegrity[77]) / 5;
			tailRotorIntegrityFactor = elementIntegrity[81];
		}
	}

	void onRepair()
	{
		for (int i = 0; i < 111; i++)
		{
			elementIntegrity[i] = 1.0;
		}
	}

	bool isRepairNeeded() const
	{
		// zero is fully broken
		for (int i = 0; i < 111; i++)
		{
			if (elementIntegrity[i] < 1)
			{
				return true;
			}
		}
		return false;
	}

	void setImmortal(bool value)
	{
		isImmortal = value;
	}


	void updateFrame(const double frameTime)
	{
		
	}
};
