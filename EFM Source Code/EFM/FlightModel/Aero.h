#pragma once

#include "../stdafx.h"
#include <vector>
#include "../include/UtilityFunctions.h"
#include "../include/MatrixFunction.h"
#include "../include/GlobalParams.h"
#include "../include/GlobalData.h"
#include "../Systems/Damage.h"
#include "../Systems/FlightControls.h"

// Data tables for this aircraft
#include "NACA0015_airfoil.h"
#include "aero_coefficients.h"

#include "../EFMData.h"
#include "../Include/Cockpit/CockpitAPI.h" // Provides param handle interfacing for use in lua


#define NUM_BLADE_SEGMENTS 8
#define NUM_BLADES 5
//#define USE_LAG_DOF
static const double OmegaT = 49.7419; //main rotor nominal rotational velocity, [rad/sec] (475 RPM = 49.741884 rad/s)

class AH6Aero
{
public:
	AH6Aero(EFMData &ptr_EFMdata, AH6JDamage& ptr_Damage, FlightControls& ptr_fltCntrl);
	~AH6Aero() {}

    EFMData& p_EFMdata;
    AH6JDamage& p_Damage;
    FlightControls& p_flightControl;

	std::vector<ForceComponent> aeroForces{};
    std::vector<Vec3> aeroMoments{};


    MatrixFunction fn_CL_NACA0015{ _NACA0015_CL, _NACA0015full_Points, _NACA0015full_AoA };
    MatrixFunction fn_CD_NACA0015{ _NACA0015_CD, _NACA0015_Points, _NACA0015_AoA };

    MatrixFunction fn_Cx_Fuselage{ _CxFuse_Cx, _CxFuse_Points, _CxFuse_Alpha };
    MatrixFunction fn_Cz_Fuselage{ _CzFuse_Cz, _CzFuse_Points, _CzFuse_Alpha };
    MatrixFunction fn_Cm_Fuselage{ _CmFuse_Cm, _CmFuse_Points, _CmFuse_Alpha };
    MatrixFunction fn_Cna_Fuselage{ _CnaFuse_Cna, _CnaFuse_Points, _CnaFuse_Beta };
    MatrixFunction fn_Cnb_Fuselage{ _CnbFuse_Cnb, _CnbFuse_Points, _CnbFuse_Beta };

    MatrixFunction fn_EKFX{ _EKFX_data, _EKFX_Points, _SkewAngle };
    MatrixFunction fn_EKFZ{ _EKFZ_data, _EKFX_Points, _SkewAngle };

    MatrixFunction fn_lagDamp{ _lagDamp_data, _LagDamp_Points, _DeltDot };

    EDPARAM cockpitAPI;
    void* EFM_N2_RPM = cockpitAPI.getParamHandle("EFM_N2_RPM"); // for use in digital gauge
    void* EFM_NR_RPM = cockpitAPI.getParamHandle("EFM_NR_RPM"); // for use in digital gauge


    void Initialize();
    void InitializeOff();
    void InitializeOn();
    //void Release();

    void update(double engtorque);

   
    void MainRotorModule();
    void FuselageModule();
    void EmpennageModule();
    void TailRotorModule();
    void RotorDegreeOfFreedom(double engtorque);


    void setBodyStates(double ax, double ay, double az,
        double vx, double vy, double vz,
        double wind_vx, double wind_vy, double wind_vz,
        double omegadotx, double omegadoty, double omegadotz,
        double omegax, double omegay, double omegaz,
        double yaw, double pitch, double roll )
    {
        // Converting body states from DCS coordinate system to typical coordinate system,
        // add wind velocity, and convert to imperial units
        r = -omegay;// yaw rate [rad/s]
        q = omegaz;// pitch rate [rad/s]
        p = omegax;// roll rate [rad/s]
        rDot = -omegadoty;// yaw acceleration [rad/s^2]
        qDot = omegadotz;// pitch acceleration [rad/s^2]
        pDot = omegadotx;// roll acceleration [rad/s^2]
        VXB = (vx - wind_vx) * Convert::meterToFoot; // X velocity [ft/s]
        VYB = (vz - wind_vz) * Convert::meterToFoot;
        VZB = -(vy - wind_vy) * Convert::meterToFoot;
        VXBDot = ax * Convert::meterToFoot;// X acceleration [ft/s^2]
        VYBDot = az * Convert::meterToFoot;
        VZBDot = -ay * Convert::meterToFoot;

        PhiB = roll;
        ThetaB = pitch;
    }

    double getMRomega()
    {
        return Omega;
    }
    double getTRomega()
    {
        return OmegaTR;
    }
    double getN2omega()
    {
        return OmegaE;
    }
    const double getN2PCT()
    {
        return OmegaE / OmegaT * 100.0;
    }

    void setRotorBrake(float value)
    {
        isRotorBrakeEngaged = value > 0.0;
    }

private:
    //============== Overall constants ============== 
    const double FSCG{ 100 };		//Center of Gravity Fuselage Station position(longitudinal;x), [inch]
    const double WLCG{ 49.6 };		//Center of Gravity Waterline position(vertical;z), [inch]
    const double BLCG{ 0.0 };		//Center of Gravity Buttline position(lateral;y), [inch]
    //============== Main rotor constants ============== 
    const double RMR{ 13.167 };				// main rotor radius, [ft]
    const double BLMR{ 0.0 };				// buttline pos(left/right) MR, [inch]
    const double FSMR{ 100.0 };				// fuselage station MR, [inch]
    const double WLMR{ 83.0 };				// waterline MR, [inch]
    const double BMR{ 0.97 };				// MR blade tip loss factor
    const double CR{ 0.5625 };				// blade root chord, [ft]
    const double e{ 0.46 };					// MR flap hinge offset, [ft]         this affects how strong the moments are on the hub
    const double ePrime{ 1.125 };			// distance from hinge to blade start, [ft]   this affects amount of blade area  
    const double Ib{ 85.0 };				// main rotor blade inertia about hinge, slugs [ft^2]  46.8
    const double Mb{ 3.5 };				    // blade first mass moment about hinge, slugs [ft]   lower==more stable, higher==more control  
    const double Wb{ 40.0 };				// blade weight, [lb] 37.25
    const double Theta1{ -8.0 };				//main rotor blade twist, [deg/unit radius]
    const double iS{ -3.0 * Convert::degToRad }; // MR shaft tilt, rad (negative=forward)  
    const double Kbeta{ 0.0 };				    //flapping hinge spring const, [ft lb/rad]    // this is 0 for articulated rotors
    const double KbetaDot{ 0.0 };				//flapping hinge rate damp const, [ft lb sec/rad]
    const double betaUp{ 25.0 * Convert::degToRad };   // blade flapping upper limit, [rad]
    const double betaDown{ -25.0 * Convert::degToRad };// blade flapping lower limit, [rad]
    const double KlambdaPrime{ 0.3 };			       //downwash loop filter time const, [Seconds]
    const double KfPrime{ 0.15 };			            //MR force filter time const, [Seconds]
    const double DeltaSP{ -12.0 * Convert::degToRad };	//swashplate rotation, [rad]
    //const double DeltaCD{ 0.005 };			            //blade drag coef   
    const double delt3{ 0.0 };		                // pitch-flap coupling(AKA Delta3), [rad] 
    //const double KDeltDot{ 1550.0 };	        // lag damper, [ft lb sec/rad]   this effects retreating blade stall onset
    //const double Fdelt{ 0.0 };                  // lagging hinge spring const, [ft lb/rad]
    const double deltAFT{ -0.3 };		        // blade lagging angle aft limit, [rad]
    const double deltFWD{ 0.25 };		        // blade lagging angle forward limit, [rad]


    //============== Fuselage constants =================
    const double SPF{ 26.0 };       // Plan (front) area of fuselage, [ft^2]
    const double SSF{ 60.0 };       // Side area of fuselage, [ft^2]

    //============== Tail constants =====================
    const double EKTR{ 0.0 };		// tail rotor wash factor on vert tail
    const double FSHT{ 290.0 };	    // FS horizontal tail, [inch]
    const double WLHT{ 90.0 };		// WL horizontal tail, [inch]
    const double iHT{ 9.0 };		// horz tail incidence, [deg]
    const double SAHT{ 8.5 };       // Horizontal tail surface area, [ft^2] (7.5)
    const double FSVT{ 285.0 };		// FS vertical tail, [inch]
    const double WLVT{ 56.0 };		// WL vertical tail, [inch]
    const double SAVT{ 9.6 };         // Vertical tail surface area, [ft^2]

    //============== Tail Rotor Constants ===============
    const double BLTR{ -11.0 };			//buttline TR(y pos), [in]
    const double FSTR{ 292.0 };			// fuselage station(x pos) TR, [in]
    const double WLTR{ 54.1 };			//waterline(height) pos, [in]
    const double OmegaTR_T{ 297.2 };	//TR nominal rotational speed, [rad/sec] (297.2 = 2838rpm)
    const int bNTR{ 2 };				//number of TR blades
    const double cTR{ 0.4 };			//TR blade chord, [ft]
    const double RTR{ 2.375 };			//TR radius, [ft]
    const double aTR{ 5.6 };			//tail rotor blade lift curve slope, [1/rad]
    const double BTR{ 0.94 };			//TR blade tip loss factor
    const double KTRBLK{ 0.94 };			//fin-tail blockage factor ***estimate
    const double GammaTR{ 90.0 * Convert::degToRad };	//tail rotor cant(tilt) angle from up, [rad]
    const double Delt3TR{ 43.0 * Convert::degToRad };	//TR blade hinge skew angle, [rad]
    const double Theta1TR{ -8.0 };		//TR blade twist, [deg]
    const double DragTR{ 1.3 };         // Tail Rotor Drag coefficient
    const double da0overdT{ 0.00129 };	// rate of change of cone angle, [deg/lb]
    const double KlambdaPrimeTR{ 0.1 };			//downwash loop filter const 

    //============== Gearbox Constants ==================
    const double JMR = 600.0;// inertia of MR, [slug-ft2] can be estimated J=N*I_B
    const double JE = 150.0;//engine (N2 turbine) inertia, [slug-ft^2]
    const double KFRQ = 25.0;// MR torque filter constant




    //MR module variable setup
    double Omega = 0.0;			         //current MR speed [rad/sec].  to disable rotor DOF set to OmegaT
    double OmegaDot = 0.0;		         //MR acceleration [rad/s^2].   to disable rotor DOF set to 0
    double XI = 0.0;                     //hinge offset normalized
    double XIPrime = 0.0;                //spar length
    double PsiMR = 0.0;                 //whole rotor rotation, -pi to pi [rad]
    double LambdaMR = 0.0;               //main rotor inflow
    double CTA = 0.0;                   //calculated at end of force calcs
    double DWMR = 0.0;                  // main rotor downwash
    double Y2[NUM_BLADE_SEGMENTS] = { 0.0 };      //segment distance to center of lift, per unit radius(0-1)
    double DeltaY[NUM_BLADE_SEGMENTS] = { 0.0 };  //segment width in unit radius(0-1)
    double Psi[NUM_BLADES] = { 0.0 };             //rad, individual blade rotation
    double SinPsi[NUM_BLADES] = { 0.0 };              // sin of rotor position
    double CosPsi[NUM_BLADES] = { 0.0 };              // cos of rotor position
    double SinBeta[NUM_BLADES] = { 0.0 };             // sin of flap angle
    double CosBeta[NUM_BLADES] = { 0.0 };             // cos of flap angle
    double SinDelt[NUM_BLADES] = { 0.0 };             // sin of lag angle
    double CosDelt[NUM_BLADES] = { 0.0 };             // cos of lag angle
    double BetaDotDot[NUM_BLADES] = { 0.0 };            //blade flapping acceleration, [rad/sec^2]
    double BetaDot[NUM_BLADES] = { 0.0 };               //blade flapping velocity, [rad/sec]
    double Beta[NUM_BLADES] = { 0.0 };                  //blade flapping angle, [rad]
    double DeltDotDot[NUM_BLADES] = { 0.0 };            //blade lag acceleration, [rad/sec^2]
    double DeltDot[NUM_BLADES] = { 0.0 };               //blade lag velocity, [rad/sec]
    double Delt[NUM_BLADES] = { 0.0 };                  //blade lagging angle, [rad]
    double UP[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };          //blade segment perpendicular velocity, non dimen
    double UT[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };          //blade segment tangential velocity, non dimen
    double UR[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };          //blade segment radial velocity, non dimen
    double Theta[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };       //blade segment pitch angle, [rad]
    double alphaTrans[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };  //blade function angle of attack map entry, [deg]
    double CLY[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };         //blade segment lift coef
    double CDY[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };         //blade segment drag coef
    double FP[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };          //blade segment force perpendicular, [lb]
    double FT[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };          //blade segment force tangential, [lb]
    double FR[NUM_BLADES][NUM_BLADE_SEGMENTS] = { 0.0 };          //blade segment force radial, [lb]
    double FXA[NUM_BLADES] = { 0.0 };//blade aerodynamic shear forces, rotating shaft axis, [lb]
    double FYA[NUM_BLADES] = { 0.0 };//blade aerodynamic shear forces, rotating shaft axis, [lb]
    double FZA[NUM_BLADES] = { 0.0 };//blade aerodynamic shear forces, rotating shaft axis, [lb]
    double MFA[NUM_BLADES] = { 0.0 };//aerodynamic moment about flapping hinge, [ft-lb]
    double MLA[NUM_BLADES] = { 0.0 };//aerodynamic moment about lagging hinge, [ft-lb]
    double MFD[NUM_BLADES] = { 0.0 };//flap damper moment, [ft-lb]
    double MLD[NUM_BLADES] = { 0.0 };//lag damper moment, [ft-lb]
    double FXT[NUM_BLADES] = { 0.0 };//blade total shear forces, RSA, [lb]
    double FYT[NUM_BLADES] = { 0.0 };//blade total shear forces, RSA, [lb]
    double FZT[NUM_BLADES] = { 0.0 };//blade total shear forces, RSA, [lb]
    double skewAngleMR = 0.0;//MR wake skew angle, [deg]
    double XMR = 0.0;// final MR force X-axis, [lb]
    double YMR = 0.0;// final MR force Y-axis, [lb]
    double ZMR = 0.0;// final MR force Z-axis, [lb]
    double LMR = 0.0;// final MR moment roll, [ft-lb]
    double MMR = 0.0;// final MR moment pitch, [ft-lb]
    double NMR = 0.0;// final MR moment yaw, [ft-lb]

    // Fuselage module variable setup
    double alphaF_deg = 0.0;    //fuselage angle of attack, [deg]
    double PsiFdeg = 0.0;      //fuselage angle of sideslip, [deg]

    // tail module variable setup
    double alphaVT = 0.0;   //Angle of Attack of VT, [rad]
    double EKTXU = 0.0;     //rotor wash factor on upper horizontal tail
    double EKTZU = 0.0;     //rotor wash factor on upper horizontal tail
    double KQVT = 0.0;      //dynamic pressure loss factor VT
    double TauTU = 0.0;     //time lag in wing flow reaching upper HT, [sec]
    double VZIWU = 0.0;     //wing downwash velocity at upper HT, z axis, [ft/s]
    double VYIW = 0.0;      //wing sidewash velocity at VT, y axis, [ft/s]

    // tail rotor variable setup
    double OmegaTR = 0.0;   // tail rotor rotional veloctiy [rad/s]
    double PsiTR = 0.0;     // whole rotor rotation, -pi to pi [rad]
    double TTR = 0.0;       // tail rotor thrust, [lb]
    double DWTR = 0.0;      // downwash Tail rotor, normalized 0-1 (ie 1==OmegaT * RMR)
    double LambdaTR = 0.0;  //tail rotor inflow, normalized 0-1 (ie 1==OmegaT * RMR)
    double TRSolidity = 0.0; // solidity of TR, ie ratio of blade area to disk size


    // Body states
    double r = 0.0;// yaw rate [rad/s]
    double q = 0.0;// pitch rate [rad/s]
    double p = 0.0;// roll rate [rad/s]
    double rDot = 0.0;// yaw acceleration [rad/s^2]
    double qDot = 0.0;// pitch acceleration [rad/s^2]
    double pDot = 0.0;// roll acceleration [rad/s^2]
    double VXB = 0.0;// X velocity body axis [ft/s]
    double VYB = 0.0;// Y velocity body axis [ft/s]
    double VZB = 0.0;// Z velocity body axis [ft/s]
    double VXBDot = 0.0;// X acceleration body axis [ft/s^2]
    double VYBDot = 0.0;// Y acceleration body axis [ft/s^2]
    double VZBDot = 0.0;// Z acceleration body axis [ft/s^2]
    double PhiB = 0.0;// roll, [rad]
    double ThetaB = 0.0; // pitch, [rad]

    // Rotor Degree Of Freedom (DOF)
    double QMR = 0.0;// MR torque shaft axis, [ft-lb]
    double QMRfiltered = 0.0;// filtered MR torque, [ft-lb]
    bool isClutchEngaged = true;
    double OmegaE = 0.0;//engine shaft speed (N2), [rad/sec]
    bool isRotorBrakeEngaged = false;

};


