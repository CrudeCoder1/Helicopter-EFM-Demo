#pragma once
#include "include/ED_FM_Utility.h"

// Class to store data from DCS to be used across the EFM

class EFMData
{
public:

	EFMData()
		: centerOfGravity()
		, inertia()
		, airspeed()
	{}
	~EFMData() {}

	Vec3 centerOfGravity;
	Vec3 inertia;
	Vec3 airspeed;// airspeed with wind, [m/s]

	double weight_N = 0.0; // Weight force of aircraft (N)
	double mass_kg = 0.0;

	// set surface
	double surfaceAlt = 0.0;  // [meters] height of surface under aircraft
	
	// set Atmo
	double altitudeMSL = 0.0;// [meters]
	double altitudeMSL_FT = 0.0;// [feet]
	double ambientTemp_C = 0.0;// [deg C]
	double speedOfSound = 0.0;// [meters/sec]
	double speedOfSound_fts = 0.0;// [ft/sec]
	double rho = 0.0;// air density [kg/m^3]
	double rho_SlgFt3 = 0.0;// air density [slug/ft^3]
	double airPressure = 0.0;// [Pascal]


	//double elementIntegrity[137]{ 0.0 };

	// calculated
	double totalVelocity_MPS = 0.0;	// Total velocity (always positive) [m/s]
	double mach = 0.0;
	double altitudeAGL = 0.0; // [meters] above ground level, includes buildings and objects
	double altitudeAGL_ft = 0.0; // [feet] above ground level, includes buildings and objects

	double time = 0.0;//time since simulation start
	double deltaTime = 0.0;// Frame Time, dt


	void init()
	{
		
	}

	void setSurface(double h_obj, unsigned surface_type)
	{
		surfaceAlt = h_obj;
	}

	void setAtmosphere(double altMSL_m, double temp_k, double speedSound_mps, double airDensity_kgm3, double airPressure_Nm2)
	{
		altitudeMSL = altMSL_m;
		altitudeMSL_FT = altMSL_m * Convert::meterToFoot;
		ambientTemp_C = temp_k - 273.15;
		speedOfSound = speedSound_mps;
		speedOfSound_fts = speedSound_mps * Convert::meterToFoot;
		rho = airDensity_kgm3;
		rho_SlgFt3 = Convert::kgm3ToSlugFt3;
		airPressure = airPressure_Nm2;
	}

	void setMassState(double mass,double COM_x,double COM_y,double COM_z,double MOI_x,double MOI_y,double MOI_z)
	{
		mass_kg = mass;
		weight_N = mass * Convert::standard_gravity;

		centerOfGravity.x = COM_x;
		centerOfGravity.y = COM_y;
		centerOfGravity.z = COM_z;

		inertia.x = MOI_x;
		inertia.y = MOI_y;
		inertia.z = MOI_z;
	}

	void setCurrentBodyState(
		double ax, double ay, double az, 
		double vx, double vy, double vz, 
		double wind_vx, double wind_vy, double wind_vz, 
		double omegadotx, double omegadoty, double omegadotz, 
		double omegax, double omegay, double omegaz, 
		double yaw, double pitch, double roll)
	{
		// y and z axis corrected for normal aerodynamic usage
		airspeed.x = vx - wind_vx;
		airspeed.y = vy - wind_vz;
		airspeed.z = -(vz - wind_vy);
	}

	void update(const double dt)
	{
		totalVelocity_MPS = sqrt(airspeed.x * airspeed.x + airspeed.y * airspeed.y + airspeed.z * airspeed.z);
		mach = totalVelocity_MPS / speedOfSound;
		altitudeAGL = altitudeMSL - surfaceAlt;
		altitudeAGL_ft = altitudeAGL * Convert::meterToFoot;

		time += dt;
		deltaTime = dt;
	}
};
