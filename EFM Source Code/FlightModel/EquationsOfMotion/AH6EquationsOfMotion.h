#ifndef _AH6EQUATIONSOFMOTION_H_
#define _AH6EQUATIONSOFMOTION_H_

#include "../stdafx.h"
#include <math.h>

#include "include/ED_FM_Utility.h"		// Provided utility functions that were in the initial EFM example
#include "include/AH6Constants.h"		// Common constants used throughout this DLL

namespace Helicopter
{
	class AH6Motion
	{
	protected:
		//-----------------------------------------------------------------
		// This variable is very important.  Make sure you set this
		// to 0 at the beginning of each frame time or else the moments
		// will accumulate.  For each frame time, add up all the moments
		// acting on the air vehicle with this variable using th
		//
		// Units = Newton * meter
		//-----------------------------------------------------------------
		Vec3	common_moment;							
		//-----------------------------------------------------------------
		// This variable is also very important.  This defines all the forces
		// acting on the air vehicle.  This also needs to be reset to 0 at the
		// beginning of each frame time.  
		//
		// Units = Newton
		//-----------------------------------------------------------------
		Vec3	common_force;
		//-----------------------------------------------------------------
		// Center of gravity of the air vehicle as calculated from the 
		// DCS simulation, I don't believe this is utilized within this 
		// EFM.
		//
		// Units = meter
		//-----------------------------------------------------------------
		Vec3    center_of_gravity;
		//-----------------------------------------------------------------
		// The moments of inertia for the air vehicle as calculated from the
		// DCS Simulation.  This is not used within this EFM as there is a bug
		// when trying to manipulate weight or moment of inertia from within
		// the EFM.  The inertia is currently set from entry.lua
		//
		// Units: Newton * meter^2
		//-----------------------------------------------------------------
		Vec3	inertia;

		//-----------------------------------------------------------------
		// The local winds acting on the air vehicle as calculated by the
		// DCS Simulation
		//
		// Units: Meters/(Second^2)
		//-----------------------------------------------------------------
		Vec3	wind;
		//-----------------------------------------------------------------
		// Absolute velocities of the air vehicle as calculated by DCS World
		//
		// Units: Meters/(Second^2)
		//-----------------------------------------------------------------
		Vec3	velocity_world_cs;
		//-----------------------------------------------------------------


		// current total mass (including fuel)
		//double mass;

		// mass times speed
		//double kinetic_energy;

		double rho; // atmospheric density
		double fuel_mass_delta; // change in fuel mass since last frame
		double weight_N; // Weight force of aircraft (N)
		double mass_kg;

	public:
		// Get the total absolute velocity acting on the aircraft with wind included
		// using english units so airspeed is in feet/second here
		Vec3	airspeed;
		double		ambientTemperature_DegK;	// Ambient temperature (kelvin)
		double		ambientDensity_KgPerM3;		// Ambient density (kg/m^3)
		double		dynamicPressure_LBFT2;		// Dynamic pressure (lb/ft^2)
		double		dynamicPressure_x;		// Dynamic pressure x (lb/ft^2)
		double		dynamicPressure_y;		// Dynamic pressure y (lb/ft^2)
		double		dynamicPressure_z;		// Dynamic pressure z (lb/ft^2)
		double		dynamicPressure_Rotor;		// Dynamic pressure for the rotor (lb/ft^2)
		double		speed_of_sound;				// (meters/sec)
		double		mach; // Well..Mach, yeah

		double		altitude_FT;		// Absolute altitude MSL (ft)
		double		ps_LBFT2;			// Ambient calculated pressure (lb/ft^2)
		double		totalVelocity_MPS;	// Total velocity (always positive) (m/s)
		double		airspeed_KTS;		// total airspeed (always positive) (knots)

		double		surfaceAlt;  // height of surface under aircraft
		double		altitudeAS;	 // above sea level
		double		altitudeAGL; // above ground level, includes buildings and objects

		AH6Motion() 
			: common_moment()
			, common_force()
			, center_of_gravity()
			, inertia()
			, fuel_mass_delta(0)
			, weight_N(0)
			, mass_kg(0)
			, rho(0)
			, wind()
			, velocity_world_cs()
			, airspeed()
			, ambientTemperature_DegK(0)
			, ambientDensity_KgPerM3(0)
			, dynamicPressure_LBFT2(0)
			, dynamicPressure_x(0)
			, dynamicPressure_y(0)
			, dynamicPressure_z(0)
			, dynamicPressure_Rotor(0)
			, speed_of_sound(0)
			, mach(0)
			, altitude_FT(0)
			, ps_LBFT2(0)
			, totalVelocity_MPS(0)
			, airspeed_KTS(0)
			, surfaceAlt(0)
			, altitudeAS(0)
			, altitudeAGL(0)
		{}
		~AH6Motion() {}

		// Very important! This function sum up all the forces acting on the aircraft for this run frame.  It currently assume the force is acting at the CG
		void add_local_force(const Vec3 & Force, const Vec3 & Force_pos)
		{
			common_force.x += Force.x;
			common_force.y += Force.y;
			common_force.z += Force.z;

			Vec3 delta_pos(Force_pos.x - center_of_gravity.x,
						   Force_pos.y - center_of_gravity.y,
						   Force_pos.z - center_of_gravity.z);

			Vec3 delta_moment = cross(delta_pos, Force);

			common_moment.x += delta_moment.x;
			common_moment.y += delta_moment.y;
			common_moment.z += delta_moment.z;
		}

		// Very important! This function sums up all the moments acting on the aircraft for this run frame.  It currently assumes the moment is acting at the CG
		void add_local_moment(const Vec3 & Moment)
		{
			sum_vec3(common_moment, Moment);
		}

		void clear()
		{
			// Very important! clear out the forces and moments before you start calculated a new set for this run frame
			clear_vec3(common_force);
			clear_vec3(common_moment);
			fuel_mass_delta = 0;
		}

		/*
		void setMassChange(double delta_mass)
		{
			mass += delta_mass;
		}
		*/

		void setMassState(double mass,
						double center_of_mass_x,
						double center_of_mass_y,
						double center_of_mass_z,
						double moment_of_inertia_x,
						double moment_of_inertia_y,
						double moment_of_inertia_z)
		{
			mass_kg = mass;
			weight_N = mass * 9.80665002864;

			center_of_gravity.x  = center_of_mass_x;
			center_of_gravity.y  = center_of_mass_y;
			center_of_gravity.z  = center_of_mass_z;

			inertia.x = moment_of_inertia_x;
			inertia.y = moment_of_inertia_y;
			inertia.z = moment_of_inertia_z;
		}

		void getLocalForce(double &x,double &y,double &z,double &pos_x,double &pos_y,double &pos_z)
		{
			x = common_force.x;
			y = common_force.y;
			z = common_force.z;
			pos_x = center_of_gravity.x;
			pos_y = center_of_gravity.y;
			pos_z = center_of_gravity.z;
		}

		void getLocalMoment(double &x,double &y,double &z)
		{
			x = common_moment.x;
			y = common_moment.y;
			z = common_moment.z;
		}

		bool isMassChanged() const
		{
			if (fuel_mass_delta != 0)
			{
				return true;
			}
			if (inertia.x != Helicopter::inertia_Ix_KGM2
				|| inertia.y != Helicopter::inertia_Iz_KGM2
				|| inertia.z != Helicopter::inertia_Iy_KGM2)
			{
				return true;
			}
			return false;
		}

		void getMassMomentInertiaChange(double & delta_mass,
										double & delta_mass_pos_x,
										double & delta_mass_pos_y,
										double & delta_mass_pos_z,
										double & delta_mass_moment_of_inertia_x,
										double & delta_mass_moment_of_inertia_y,
										double & delta_mass_moment_of_inertia_z)
		{
			// TODO: change in amount of fuel -> change in mass -> set here

			delta_mass -= fuel_mass_delta;
			//delta_mass = 0;
			delta_mass_pos_x = 0.0;
			delta_mass_pos_y = 0.0;
			delta_mass_pos_z = 0.0;

			//delta_mass_pos_x = -1.0;
			//delta_mass_pos_y =  1.0;
			//delta_mass_pos_z =  0;

			delta_mass_moment_of_inertia_x = Helicopter::inertia_Ix_KGM2 - inertia.x;
			delta_mass_moment_of_inertia_y = Helicopter::inertia_Iy_KGM2 - inertia.y;
			delta_mass_moment_of_inertia_z = Helicopter::inertia_Iz_KGM2 - inertia.z;

			// TODO: decrement this delta from inertia now?
			fuel_mass_delta = 0;
		}

		void setAtmosphere(const double temperature, const double density, const double soundspeed, const double altitude, const double pressure)
		{
			ambientTemperature_DegK = temperature;
			ambientDensity_KgPerM3 = density;
			altitudeAS = altitude;
			altitude_FT = altitude * Helicopter::meterToFoot; // meters to feet
			ps_LBFT2 = pressure * 0.020885434273; // (N/m^2) to (lb/ft^2)
			speed_of_sound = soundspeed;

			// calculate some helpers already
			rho = ambientDensity_KgPerM3;
		}
		void setSurface(const double surfaceHeight)
		{
			surfaceAlt = surfaceHeight;
		}

		void setAirspeed(const double vx, const double vy, const double vz, const double wind_vx, const double wind_vy, const double wind_vz)
		{
			velocity_world_cs.x = vx;
			velocity_world_cs.y = vy;
			velocity_world_cs.z = vz;

			wind.x = wind_vx;
			wind.y = wind_vy;
			wind.z = wind_vz;

			// Get the total absolute velocity acting on the aircraft with wind included m/s
			// y and z acis corrected for normal aerodynamic usage
			airspeed.x = velocity_world_cs.x - wind.x;
			airspeed.y = velocity_world_cs.z - wind.z;
			airspeed.z = -(velocity_world_cs.y - wind.y);
		}

		void updateFrame(double frameTime)
		{
			totalVelocity_MPS = sqrt(airspeed.x * airspeed.x + airspeed.y * airspeed.y + airspeed.z * airspeed.z);
			if (totalVelocity_MPS < 0.01)
			{
				totalVelocity_MPS = 0.01;
			}
			airspeed_KTS = totalVelocity_MPS * 1.943844;
			// Call the atmosphere model to get mach and dynamic pressure
			mach = totalVelocity_MPS / speed_of_sound;
			dynamicPressure_LBFT2 = .5 * rho * pow(totalVelocity_MPS, 2);  // q = .5*rho*V^2
			dynamicPressure_x = .5 * rho * pow(airspeed.x, 2);  // q = .5*rho*V^2
			dynamicPressure_z = .5 * rho * pow(airspeed.z, 2);  // q = .5*rho*V^2
			dynamicPressure_y = .5 * rho * pow(airspeed.y, 2);  // q = .5*rho*V^2

			altitudeAGL = altitudeAS - surfaceAlt;
			
		}
		//----------------------------------------------------------------
		// All forces calculated in newtons.  All  moments calculated in N*m
		//----------------------------------------------------------------
		void updateAeroForces(const double Cx_total, const double Cz_total, const double Cm_total, const double Cy_total, const double Cl_total, const double Cn_total)
		{
			// Longitudinal forces
			// Cx force out the nose in Newtons
			Vec3 cx_force(Cx_total * mass_kg, 0, 0 );		
			Vec3 cx_force_pos(0,0,0);
			add_local_force(cx_force ,cx_force_pos);
			
			// Cz force down the bottom of the aircraft in Newtons
			Vec3 cz_force(0.0,  -Cz_total * mass_kg, 0.0 );	
			Vec3 cz_force_pos(0,0,0);
			add_local_force(cz_force ,cz_force_pos);

			// Cm	pitching moment in N*m
			Vec3 cm_moment(0.0, 0.0, Cm_total * inertia.y );
			add_local_moment(cm_moment);

			// Lateral forces
			// Cy	force out the right wing in Newtons 
			Vec3 cy_force(0.0, 0.0, Cy_total * mass_kg);		 
			Vec3 cy_force_pos(0,0,0); 
			add_local_force(cy_force ,cy_force_pos);

			// Cl	rolling moment in N*m
			Vec3 cl_moment(Cl_total * inertia.x, 0.0,  0.0);
			add_local_moment(cl_moment);

			// Cn	yawing moment in N*m
			Vec3 cn_moment(0.0, -Cn_total * inertia.z, 0.0);
			add_local_moment(cn_moment);

		}


		void updateFuelUsageMass(double mass_delta, double posX, double posY, double posZ)
		{
			fuel_mass_delta = mass_delta;
		}

		double getWeightN() const
		{
			return weight_N;
		}
	};
}

#endif // ifndef _AH6EQUATIONSOFMOTION_H_
