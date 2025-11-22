#pragma once

#include <malloc.h>
#include <memory.h>

#include "ED_FM_Utility.h"

// Used to store force direction and position
struct ForceComponent
{
	Vec3 dir;
	Vec3 pos;
};

// Simple upper and lower limiter
inline double limit(double input, double lower_limit, double upper_limit)
{
	if (input > upper_limit)
	{
		return upper_limit;
	}
	else if (input < lower_limit)
	{
		return lower_limit;
	}
	else
	{
		return input;
	}
}

// limits the max change rate of input value
//0.006 is frame time
inline double deltaLimit(double input, double targetValue, double deltaLimit)
{
	double output;
	double difference = targetValue - input;

	if (difference > 0.0)
	{
		output = limit(input + deltaLimit * 0.006, input, targetValue);
	}
	else
	{
		output = limit(input - deltaLimit * 0.006, targetValue, input);
	}

	return output;
}

// linear interpolate function that will output a value between lowerB and upperB that varies linearly based on lowerA->upperA
inline double LinInterp(double inputA, double lowerA, double upperA, double lowerB, double upperB)
{
	double t = (inputA - lowerA) / (upperA - lowerA);//gives linear value 0-1
	double output = (upperB - lowerB) * t + lowerB;

	if (upperB < lowerB)
	{
		output = limit(output, upperB, lowerB);
	}
	else
	{
		output = limit(output, lowerB, upperB);
	}

	return output;
}
