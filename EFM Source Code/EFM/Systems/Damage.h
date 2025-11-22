#pragma once

class AH6JDamage
{
private:

	// damage status of section in 0.01 increments. 0 is fully broken, 1 is full health
	static const int maxElem = 137;// max number of damage elements from Scripts\Aircrafts\_Common\Damage.lua
	bool isImmortal = false; // <- ignore damage

public:

	AH6JDamage() {}
	~AH6JDamage() {}

	double elementIntegrity[maxElem]{ 0.0 }; // store each damage element 


	void init()
	{
		for (int i = 0; i < maxElem; i++)
		{
			elementIntegrity[i] = 1.0;
		}
	}


	// Scripts\Aircrafts\_Common\Damage.lua	 for element numbers
	void onAirframeDamage(int element, double elementIntegrityFactor)
	{
		if (element >= 0 && element < maxElem && !isImmortal)
		{
			elementIntegrity[element] = elementIntegrityFactor;
		}
	}

	void onRepair()
	{
		for (int i = 0; i < maxElem; i++)
		{
			elementIntegrity[i] = 1.0;
		}
	}

	bool isRepairNeeded() const
	{
		// zero is fully broken
		for (int i = 0; i < maxElem; i++)
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

};
