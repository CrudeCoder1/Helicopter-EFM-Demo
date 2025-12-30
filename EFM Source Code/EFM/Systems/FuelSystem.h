#pragma once
 

// TODO: transfer of fuel between tanks (need handle added in 3d model)

class FuelTank
{
private:
	double capacity = 0.0; // max capacity of tank [kg]
	Vec3 position;// for weight and balance
public:	
	double currentFuel = 0.0; // amount of fuel currently in tank [kg]

	FuelTank(double maxCapacity = 0, double posX = 0, double posY = 0, double posZ = 0)
		: capacity(maxCapacity)
		, position(posX, posY, posZ)
	{}
	~FuelTank() {}

	// add what is possible, return remaining if full
	double addFuel(const double addition)
	{
		double space = capacity - currentFuel;
		if (space < addition)
		{
			currentFuel = capacity; // set to max
			return (addition - space); // overflow
		}
		currentFuel += addition;
		return 0.0;
	}

	double decFuel(const double decrement)
	{
		if (currentFuel < decrement)
		{
			currentFuel = 0.0; // set to min
			return decrement - currentFuel; // remaining
		}
		currentFuel -= decrement;
		return 0.0;
	}

};

class FuelSystem
{
private:
	bool isUnlimitedFuel = false;
	bool isIdleCutoff = false; // true means no fuel flow

	FuelTank MainTank{ 401 / Convert::kg_to_lb };
	FuelTank AuxTank{ 412 / Convert::kg_to_lb };

	const double fuelCautionAmt = 80.0 * Convert::lb_to_kg;// caution light threshold
	//const double Fuel_transferRate_kgh = 195.0;//manual says 55 minutes or about 430lb/hr=195kg/h

public:
	bool isFuelFlow = false;
	std::vector<double> fuelMassDelta{};

	FuelSystem() {}
	~FuelSystem() {}

	void initCold()
	{
		isIdleCutoff = true;
		isFuelFlow = false;
		fuelMassDelta.push_back(MainTank.currentFuel);
		fuelMassDelta.push_back(AuxTank.currentFuel);
	}
	void initHot()
	{
		isIdleCutoff = false;
		isFuelFlow = true;
		fuelMassDelta.push_back(MainTank.currentFuel);
		fuelMassDelta.push_back(AuxTank.currentFuel);
	}


	// is low fuel indication
	bool isLowFuel() const
	{
		return getInternalFuel() <= fuelCautionAmt;		
	}

	// called on initialization and on refueling
	void setInternalFuel(const double fuel_kg) // <- in kg
	{
		MainTank.currentFuel = 0;
		AuxTank.currentFuel = 0;
		refuelAdd(fuel_kg);
	}

	// total internal fuel in kg
	double getInternalFuel() const
	{
		return MainTank.currentFuel + AuxTank.currentFuel;
	}

	void refuelAdd(const double fuel_kg) // <- in kg
	{	// distribute fuel to each tank
		double addition = fuel_kg;
		addition = MainTank.addFuel(addition);
		addition = AuxTank.addFuel(addition);
	}

	void setThrottle(float value)
	{		
		isIdleCutoff = value < -0.2;
	}

	void setUnlimitedFuel(bool status)
	{
		isUnlimitedFuel = status;
	}

	void update(const double FF_kgHr, const double dt)
	{
		double fuelFlow_KgS = FF_kgHr / 3600.0;//fuel flow [Kg/s]
		double fuelBurnPerFrame_kg = fuelFlow_KgS * dt;
		if (isUnlimitedFuel == true)
		{
			fuelBurnPerFrame_kg = 0.0;
		}

		fuelMassDelta.push_back(-fuelBurnPerFrame_kg);

		fuelBurnPerFrame_kg = AuxTank.decFuel(fuelBurnPerFrame_kg);
		fuelBurnPerFrame_kg = MainTank.decFuel(fuelBurnPerFrame_kg);

		if (getInternalFuel() > 0 && !isIdleCutoff)
		{
			isFuelFlow = true;
		}
		else
		{
			isFuelFlow = false;
		}

		G_Params.cautionLight[CL_FuelLow] = isLowFuel();
	}

};