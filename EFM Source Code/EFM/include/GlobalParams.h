#pragma once

#include "../stdafx.h"
#include "UtilityFunctions.h"
#include "Cockpit/CockpitAPI.h"



// Easily share a variable for more than 2 systems, but for best practice use sparingly
struct EFM_Globals
{

	//caution lights
	bool cautionLight[16]{ false };

	

};
extern EFM_Globals G_Params;
