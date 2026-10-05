#pragma once

#include "stdafx.h"

// ED's API
#include "Include/Cockpit/CockpitAPI.h" // Provides param handle interfacing for use in lua
#include "include/FM/API_Declare.h"	// Provides all DCS related functions in this cpp file

// Utility headers
#include "include/ED_FM_Utility.h"		// Provided utility functions that were in the initial EFM example
#include "include/UtilityFunctions.h"

#include "include/GlobalData.h"		// Constants used throughout this DLL
#include "include/GlobalParams.h"

// Model headers
#include "EFMData.h"				// store common info from DCS in one spot
#include "Systems/Engine.h"			// Engine model functions
#include "Systems/FuelSystem.h"		// Fuel and tank usage functions
#include "Systems/Damage.h"			// damages model
#include "Systems/ElectricSystem.h"	// Generators, battery etc.
#include "Systems/FlightControls.h"
#include "Systems/CautionLights.h"
#include "FlightModel/Aero.h"		// Aerodynamic model functions


EFM_Globals G_Params;
EDPARAM cockpitAPI;

static EFMData* EFMdata = NULL;
static FuelSystem* Fuel = NULL;
static AH6JDamage* damageModel = NULL;
static ElectricSystem* Electrics = NULL;
static FlightControls* flightControls = NULL;
static LightSystem* Lighting = NULL;
static TurboshaftEngine* Engine = NULL;
static AH6Aero* Aero = NULL;

void createHeli()
{
	EFMdata = new EFMData;
	Fuel = new FuelSystem(*EFMdata);
	damageModel = new AH6JDamage;
	Electrics = new ElectricSystem;
	flightControls = new FlightControls;
	Lighting = new LightSystem;
	Engine = new TurboshaftEngine(*EFMdata, *flightControls, *Electrics);
	Aero = new AH6Aero(*EFMdata, *damageModel, *flightControls);
}
void releaseHeli()
{
	delete EFMdata;
	delete Fuel;
	delete damageModel;
	delete Electrics;
	delete flightControls;
	delete Lighting;
	delete Engine;
	delete Aero;
	EFMdata = NULL;
	Fuel = NULL;
	damageModel = NULL;
	Electrics = NULL;
	flightControls = NULL;
	Lighting = NULL;
	Engine = NULL;
	Aero = NULL;
}


//This will store a parameter that can be read in LUA. Useful for transferring data between the dll and lua code
//void* TEST_mass = cockpitAPI.getParamHandle("TEST_mass");

