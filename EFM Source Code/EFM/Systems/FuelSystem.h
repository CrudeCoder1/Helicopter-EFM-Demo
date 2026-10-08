#pragma once
 
 // using JP-8 fuel @6.7lb/gallon
 // 
// TODO: add 2nd aux tank logic

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
	EDPARAM cockpitAPI;

	bool isUnlimitedFuel = false;
	bool isIdleCutoff = false; // true means no fuel flow
	float auxValvePos = 0;
	bool shutoffValveOpen = true;

	FuelTank MainTank{ 401 / Convert::kg_to_lb };
	FuelTank AftTank{ 412 / Convert::kg_to_lb };// aux tank #1

	const double fuelCautionAmt = 80.0 * Convert::lb_to_kg;// caution light threshold
	const double Fuel_transferRate_kgs = 197.0/3600.0;//manual says 65gal/hr == 435lb/hr == 197kg/hr

	EFMData* p_EFMdata;

	void* MainFuelTank_lb = cockpitAPI.getParamHandle("MainFuelTank_lb");
	void* AftFuelTank_lb = cockpitAPI.getParamHandle("AftFuelTank_lb");

public:
	bool isFuelFlow = false;
	std::vector<double> fuelMassDelta{};

	FuelSystem(EFMData& ptr_EFMdata)
		: p_EFMdata(&ptr_EFMdata) 
		{}
	~FuelSystem() {}

	void initCold()
	{
		isIdleCutoff = true;
		isFuelFlow = false;
		fuelMassDelta.push_back(MainTank.currentFuel);
		fuelMassDelta.push_back(AftTank.currentFuel);
	}
	void initHot()
	{
		isIdleCutoff = false;
		isFuelFlow = true;
		fuelMassDelta.push_back(MainTank.currentFuel);
		fuelMassDelta.push_back(AftTank.currentFuel);
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
		AftTank.currentFuel = 0;
		refuelAdd(fuel_kg);
	}

	void setExternalFuel(int station, double fuel, double x, double y, double z)
	{
		if (station == 2)
		{
			if (fuel < AftTank.currentFuel)
			{
				fuelMassDelta.push_back(-(AftTank.currentFuel - fuel));
			}
			AftTank.currentFuel = fuel;
		}
	}

	// total internal fuel in kg
	double getInternalFuel() const
	{
		return MainTank.currentFuel;
	}

	// total external fuel in kg
	double getExternalFuel() const
	{
		return AftTank.currentFuel;
	}

	void refuelAdd(const double fuel_kg) // <- in kg
	{	// distribute fuel to each tank
		double addition = fuel_kg;
		addition = MainTank.addFuel(addition);
		//addition = AftTank.addFuel(addition);
	}

	void setThrottle(float value)
	{		
		isIdleCutoff = value < -0.2;
	}

	void setUnlimitedFuel(bool status)
	{
		isUnlimitedFuel = status;
	}

	void transferFuel()
	{
		double transferAmt = Fuel_transferRate_kgs * p_EFMdata->deltaTime;
		if (AftTank.currentFuel > transferAmt)
		{
			double excessFuel = MainTank.addFuel(transferAmt);
			AftTank.decFuel(transferAmt);// - excessFuel); in the AH6, if the main tank if full, extra fuel from the aux transfer is vented overboard
		}		
	}

	void setCommand(int command, const float value)
	{
		switch (command)
		{
			case (int)device_commands::AuxHandle:
				auxValvePos = value;
				break;
			case (int)device_commands::FuelShutoffSw:
				shutoffValveOpen = value == 0;
				break;
		}
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

		if (auxValvePos > 0)
		{
			transferFuel();
		}
		//fuelBurnPerFrame_kg = AuxTank.decFuel(fuelBurnPerFrame_kg);
		fuelBurnPerFrame_kg = MainTank.decFuel(fuelBurnPerFrame_kg);

		if (getInternalFuel() > 0 && !isIdleCutoff && shutoffValveOpen)
		{
			isFuelFlow = true;
		}
		else
		{
			isFuelFlow = false;
		}

		G_Params.cautionLight[CL_FuelLow] = isLowFuel();

		cockpitAPI.setParamNumber(AftFuelTank_lb, AftTank.currentFuel * Convert::kg_to_lb);
		cockpitAPI.setParamNumber(MainFuelTank_lb, MainTank.currentFuel * Convert::kg_to_lb);
	}

};