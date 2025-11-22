#include "Aero.h"

//!!!!!!!!!!! TODO bugs/missing stuff: !!!!!!!!!!!!
//rotor wash factors

/// Helicopter flight model using blade element analysis
/// Source Data: 
/// 1) NASA TM-78629 (RSRA) -for math model equations
/// 2) NASA TM-88360 for clutch model
/// 3) NASA CR-166309 (GenHel) -similar to RSRA with small variations
/// 4) Helicopter Flight Dynamics by Gareth Padfield 3rd Edition -for generalized fuselage data
/// 5) Aerodynamic Tests of an Operational OH-6A Helicopter in the Ames Wind Tunnel -for specific Convert to OH-6
/// 6) NASA CR-3144 for specific constants to OH-6/AH-6
/// 
/*
Force and velocity units are imperial, angle units are in both radians and degrees.
All variables should be labeled with units denoted by brackets [unit].

Shaft axis is body axis with rotor shaft tilt (typically ~3deg forward).
Blade span axis is the local axis to each blade and
includes tangential, radial, and perpendicular components.
*/

AH6Aero::AH6Aero(EFMData& ptr_EFMdata, AH6JDamage& ptr_Damage, FlightControls& ptr_fltCntrl)
	: p_EFMdata(ptr_EFMdata)
	, p_Damage(ptr_Damage)
	, p_flightControl(ptr_fltCntrl)
{
	Initialize();
}


void AH6Aero::Initialize()
{
	//blade segment measurements
	XI = e / RMR;//hinge offset normalized to unit radius
	XIPrime = ePrime / RMR;//spar length, unit radius
	Y2[0] = sqrt(((1.0 - pow(XI + XIPrime, 2)) / (2.0 * NUM_BLADE_SEGMENTS)) + pow(XI + XIPrime, 2)) - XI;//1st segment distance
	double YINB[NUM_BLADE_SEGMENTS] = { 0.0 };//distance to inboard end of segment from centerline, unit radius
	double YOUTB[NUM_BLADE_SEGMENTS] = { 0.0 };//distance to outboard end of segment from centerline, unit radius
	YINB[0] = sqrt(pow(XI + Y2[0], 2) - (1.0 - pow(XI + XIPrime, 2)) / (2.0 * NUM_BLADE_SEGMENTS));
	YOUTB[0] = sqrt(pow(XI + Y2[0], 2) + (1.0 - pow(XI + XIPrime, 2)) / (2.0 * NUM_BLADE_SEGMENTS));
	DeltaY[0] = YOUTB[0] - YINB[0];
	
	for (int s = 1; s < NUM_BLADE_SEGMENTS; s++)
	{
		Y2[s] = sqrt(((1.0 - pow(XI + XIPrime, 2)) / NUM_BLADE_SEGMENTS) + pow(XI + Y2[s - 1], 2)) - XI;//2-nS segment distance
		YINB[s] = sqrt(pow(XI + Y2[s], 2) - (1.0 - pow(XI + XIPrime, 2)) / (2.0 * NUM_BLADE_SEGMENTS));
		YOUTB[s] = sqrt(pow(XI + Y2[s], 2) + (1.0 - pow(XI + XIPrime, 2)) / (2.0 * NUM_BLADE_SEGMENTS));
		DeltaY[s] = YOUTB[s] - YINB[s];
	}
	//cy mean chord of segment 

	TRSolidity = bNTR * cTR / (M_PI * RTR);
}
	
void AH6Aero::InitializeOff()
{
	Omega = 0.0;
	OmegaE = 0.0;
	LambdaMR = 0.0;
	CTA = 0.01;
	DWMR = 0.01;
}

void AH6Aero::InitializeOn()
{
	Omega = OmegaT;
	OmegaE = OmegaT;
	LambdaMR = -0.022;
	CTA = 0.014;
	DWMR = 0.022;
}

void AH6Aero::update(double engtorque)
{
	MainRotorModule();
	FuselageModule();
	EmpennageModule();
	TailRotorModule();
	RotorDegreeOfFreedom(engtorque);
}



// TODO:  
// check control input pitch amounts
void AH6Aero::MainRotorModule()
{
	double ThetaCUFF = p_flightControl.CollectiveInput * 19.0 + 2.0;//impressed MR collective pitch, [deg]
	double A1S = p_flightControl.rollOutput * 8.0;//total lat cyclic input, -8 to 8 [deg]
	double B1S = p_flightControl.pitchOutput * 12.0;//total long cyclic input, -11 to 15 [deg]


	double Weight = p_EFMdata.mass_kg * Convert::kg_to_lb;
	double Wbd = Weight - NUM_BLADES * Wb;//aircraft weight without rotor blades, [lb]
	double FSCGB = (FSCG * Weight - NUM_BLADES * FSMR * Wb) / Wbd;//COG FS without blades, [in]
	double WLCGB = (WLCG * Weight - NUM_BLADES * WLMR * Wb) / Wbd;//COG WL without blades, [in]
	double BLCGB = (BLCG * Weight - NUM_BLADES * BLMR * Wb) / Wbd;//COG BL without blades, [in]
	double lMR = (FSCGB - FSMR) / 12.0;//MR moment arm, x axis, [ft]
	double bMR = (BLCGB - BLMR) / 12.0;//MR moment arm, y axis, [ft]
	double hMR = (WLCGB - WLMR) / 12.0;//MR moment arm, z axis, [ft]

	// body translational accelerations at the rotor hub
	double VXHDot = VXBDot - r * VYB + q * VZB - lMR * (q * q + r * r) + bMR * (p * q - rDot) + hMR * (p * r + qDot) + Convert::gravity_fts2 * sin(ThetaB);
	double VYHDot = VYBDot - p * VZB + r * VXB + lMR * (p * q + rDot) - bMR * (p * p + r * r) + hMR * (q * r - pDot) - Convert::gravity_fts2 * sin(PhiB) * cos(ThetaB);
	double VZHDot = VZBDot - q * VXB + p * VYB + lMR * (p * r - qDot) + bMR * (q * r + pDot) - hMR * (p * p + q * q) - Convert::gravity_fts2 * cos(PhiB) * cos(ThetaB);

	// body translational velocities at the rotor hub
	double MuXH = (VXB + q * hMR - r * bMR) / (OmegaT * RMR);//x body axis velocity at MR hub, non dimensionalized
	double MuYH = (VYB - p * hMR + r * lMR) / (OmegaT * RMR);//y body axis velocity at MR hub, non dimensionalized
	double MuZH = (VZB - q * lMR + p * bMR) / (OmegaT * RMR);//z body axis velocity at MR hub, non dimensionalized

	//body axis to shaft axis transformation
	double VXSDot = VXHDot * cos(iS) - VZHDot * sin(iS);	//MR x shaft axis acceleration, [ft/sec^2]
	double VYSDot = VYHDot;									//MR y shaft axis acceleration, [ft/sec^2]
	double VZSDot = VXHDot * sin(iS) + VZHDot * cos(iS);	//MR z shaft axis acceleration, [ft/sec^2]
	double pDotS = pDot * cos(iS) - rDot * sin(iS);			//shaft axis roll acceleration, [rad/sec^2]
	double qDotS = qDot;									//shaft axis pitch acceleration, [rad/sec^2]
	double rDotS = pDot * sin(iS) + rDot * cos(iS);			//shaft axis yaw acceleration, [rad/sec^2]
	double MuXS = MuXH * cos(iS) - MuZH * sin(iS);			//x shaft axis velocity at MR hub
	double MuYS = MuYH;										//y shaft axis velocity at MR hub
	double MuZS = MuXH * sin(iS) + MuZH * cos(iS);			//z shaft axis velocity at MR hub
	double pS = p * cos(iS) - r * sin(iS);					//shaft axis roll rate, [rad/sec]
	double qS = q;											//shaft axis pitch rate, [rad/sec]
	double rS = p * sin(iS) + r * cos(iS);					//shaft axis yaw rate, [rad/sec]

	double MuTOT = sqrt(MuXS * MuXS + MuYS * MuYS + LambdaMR * LambdaMR);// total velocity at hub wth downwash, non dimensionalized
	double KGL = sqrt(pow(MuXS, 2) + pow(MuYS, 2)) / MuTOT;//1st harmonic inflow coef
	double K1X = KGL * (MuXS / MuTOT);//cos component of 1st harmonic 
	double K1Y = KGL * (MuYS / MuTOT);//sin component of 1st harmonic 

	// Downwash and inflow calculation
	double Klambda = KlambdaPrime / p_EFMdata.deltaTime;//downwash filter constant
	DWMR = ((Klambda - 1.0) / Klambda) * DWMR + (1.0 / Klambda) * (CTA / MuTOT);// downwash normalized (ie 1==OmegaT * RMR)
	LambdaMR = MuZS - DWMR;//inflow

	// Main Rotor rotational position
	//Omega = OmegaT;// uncomment to disable rotor degree of freedom (ie make rotor speed constant)
	double DeltaPsi = Omega * p_EFMdata.deltaTime;//MR advance angle, [rad]
	PsiMR += DeltaPsi;//MR rotational position
	if (PsiMR > M_PI)
	{
		PsiMR -= 2.0 * M_PI;
	}

	// Individual blade rotations
	// zero azimuth is aligned over tail (i.e. negative X axis)
	Psi[0] = PsiMR;
	for (int b = 1; b < NUM_BLADES; b++)
	{
		Psi[b] = Psi[b - 1] + 2.0 * M_PI / NUM_BLADES;
		if (Psi[b] > M_PI)
		{
			Psi[b] -= 2.0 * M_PI;
		}
	}

	double Z_rotor = limit(p_EFMdata.altitudeAGL_ft  + (WLMR - WLCG) / 12.0, 1.0, 1000.0);//rotor height above ground, [ft] 	(limit to avoid divide by zero)
	double Kge = limit(pow(1.0 + 0.13 * pow(RMR / Z_rotor, 2) * LambdaMR / sqrt(pow(MuXS, 2) + pow(MuYS, 2) + pow(LambdaMR, 2)), -2.0 / 3.0), 1.0, 1.5);//ground effect gain factor

	//---------------------------
	//----Calculations for each blade-------
	for (int b = 0; b < NUM_BLADES; b++)
	{
		// pre-calculate commonly used functions
		SinPsi[b] = sin(Psi[b]);
		CosPsi[b] = cos(Psi[b]);
		//-------- Blade flapping angle and rates----------------------
		double OmegaB = limit(Omega, OmegaT / 10.0, OmegaT * 5.0);//this variable is only for beta and betaDot equations to avoid divide by 0
		Beta[b] = Beta[b] + BetaDot[b] * sin(DeltaPsi) / OmegaB + BetaDotDot[b] * (1.0 - cos(DeltaPsi)) / pow(OmegaB, 2);
		Beta[b] = limit(Beta[b], betaDown, betaUp);//limit to avoid divide by zero. flapping angle shouldn't get past ~30deg anyways
		

		BetaDot[b] = BetaDot[b] * cos(DeltaPsi) + BetaDotDot[b] * sin(DeltaPsi) / OmegaB;
		BetaDot[b] = limit(BetaDot[b], -50.0, 50.0);

		BetaDotDot[b] = (Mb / Ib) * (cos(Beta[b]) * (VZSDot + e * (2.0 * Omega * (pS * CosPsi[b] - qS * SinPsi[b]) + pDotS * SinPsi[b] + qDotS * CosPsi[b])) + sin(Beta[b]) * cos(Delt[b]) * (VYSDot * SinPsi[b] - VXSDot * CosPsi[b] - e * pow(Omega - rS, 2)) + sin(Delt[b]) * sin(Beta[b]) * (VXSDot * SinPsi[b] + VYSDot * CosPsi[b] - e * (rDotS - OmegaDot)))
			+ pow(cos(Beta[b]), 2) * (cos(Delt[b]) * (pDotS * SinPsi[b] + qDotS * CosPsi[b] - 2.0 * (DeltDot[b] + Omega) * (qS * SinPsi[b] - pS * CosPsi[b])) - sin(Delt[b]) * (2.0 * (Omega + DeltDot[b]) * (pS * SinPsi[b] + qS * CosPsi[b]) + qDotS * SinPsi[b] - pDotS * CosPsi[b]))
			+ cos(Beta[b]) * sin(Beta[b]) * (2.0 * DeltDot[b] * (rS - Omega) - pow(Omega - rS, 2) - pow(DeltDot[b], 2))
			+ (MFA[b] / Ib) + (MFD[b] / Ib);
		BetaDotDot[b] = limit(BetaDotDot[b], -2000.0, 2000.0);

		// pre-calculate commonly used functions
		SinBeta[b] = sin(Beta[b]);
		CosBeta[b] = cos(Beta[b]);

#ifdef USE_LAG_DOF
		//---------- Blade lagging angle and rates----------------------------
		Delt[b] = Delt[b] + DeltDot[b] * sin(DeltaPsi) / OmegaB + DeltDotDot[b] * (1.0 - cos(DeltaPsi)) / pow(OmegaB, 2);
		Delt[b] = limit(Delt[b], deltAFT, deltFWD);

		DeltDot[b] = DeltDot[b] * cos(DeltaPsi) + DeltDotDot[b] * sin(DeltaPsi) / OmegaB;
		DeltDot[b] = limit(DeltDot[b], -500.0, 500.0);

		DeltDotDot[b] = (Mb / (Ib * cos(Beta[b]))) * (sin(Delt[b]) * (VYSDot * SinPsi[b] - VXSDot * CosPsi[b] - e * pow(Omega - rS, 2)) - cos(Delt[b]) * (VXSDot * SinPsi[b] + VYSDot * CosPsi[b] + e * (OmegaDot - rDotS)))
			+ (sin(Beta[b]) / cos(Beta[b])) * (2.0 * BetaDot[b] * (Omega + DeltDot[b] - rS) + qDotS * sin(Psi[b] + Delt[b]) - pDotS * cos(Psi[b] + Delt[b])) + (rDotS - OmegaDot)
			+ 2.0 * BetaDot[b] * (cos(Delt[b]) * (qS * SinPsi[b] - pS * CosPsi[b]) + sin(Delt[b]) * (pS * SinPsi[b] + qS * CosPsi[b]))
			- MLA[b] / (Ib * cos(Beta[b])) - MLD[b] / (Ib * cos(Beta[b]));
		DeltDotDot[b] = limit(DeltDotDot[b], -2000.0, 2000.0);		
#endif
		// pre-calculate commonly used functions
		SinDelt[b] = sin(Delt[b]);
		CosDelt[b] = cos(Delt[b]);

		//-----------------------------
		//----Calculations for each segment--------
		for (int s = 0; s < NUM_BLADE_SEGMENTS; s++) //calculate for each segment
		{
			//Blade segment velocities (blade span axis)
			UP[b][s] = LambdaMR * CosBeta[b] + MuYS * SinBeta[b] * sin(Psi[b] + Delt[b]) - MuXS * SinBeta[b] * cos(Psi[b] + Delt[b])
				+ XI * CosBeta[b] * ((qS / OmegaT - K1X * DWMR) * CosPsi[b] + (pS / OmegaT + K1Y * DWMR) * SinPsi[b])
				+ XI * SinBeta[b] * SinDelt[b] * ((Omega - rS) / OmegaT)
				+ Y2[s] * (-(BetaDot[b] / OmegaT) + (qS / OmegaT - K1X * DWMR * CosBeta[b]) * cos(Psi[b] + Delt[b]) + (pS / OmegaT + K1Y * DWMR * CosBeta[b]) * sin(Psi[b] + Delt[b]));

			UT[b][s] = MuXS * sin(Psi[b] + Delt[b]) + MuYS * cos(Psi[b] + Delt[b])
				+ XI * CosDelt[b] * ((Omega - rS) / OmegaT)
				+ Y2[s] * ((DeltDot[b] / OmegaT) + (pS / OmegaT * cos(Psi[b] + Delt[b]) - qS / OmegaT * sin(Psi[b] + Delt[b])) * SinBeta[b] + CosBeta[b] * ((Omega - rS) / OmegaT));
			if (UT[b][s] == 0.0) { UT[b][s] = 0.00001; }

			UR[b][s] = LambdaMR * SinBeta[b] + MuXS * CosBeta[b] * cos(Psi[b] + Delt[b]) - MuYS * CosBeta[b] * sin(Psi[b] + Delt[b])
				+ XI * SinBeta[b] * ((qS / OmegaT - K1X * DWMR) * CosPsi[b] + (pS / OmegaT + K1Y * DWMR) * SinPsi[b])
				- XI * CosBeta[b] * SinDelt[b] * ((Omega - rS) / OmegaT)
				+ Y2[s] * SinBeta[b] * (-K1X * DWMR * cos(Psi[b] + Delt[b]) + K1Y * DWMR * sin(Psi[b] + Delt[b]));

			// Resultant velocity at blade segment
			double UYAW = sqrt(pow(UT[b][s], 2) + pow(UP[b][s], 2) + pow(UR[b][s], 2));//blade segment resultant velocity, non dimen

			//blade segment flow skew angle, non dimen
			double cosGamma = abs(UT[b][s]) / sqrt(pow(UT[b][s], 2) + pow(UR[b][s], 2));
			if (cosGamma == 0.0) { cosGamma = 0.00001; }//to avoid divide by zero later

			// Blade segment geometric pitch angle
			Theta[b][s] = (ThetaCUFF - A1S * cos(Psi[b] + DeltaSP) - B1S * sin(Psi[b] + DeltaSP) + Theta1 * (Y2[s] - XIPrime) - 57.3 * Beta[b] * tan(delt3)) * Convert::degToRad;// + Kalpha1 * (57.3 * Delt[b]) + Kalpha2 * pow(57.3 * Delt[b], 2);//rad


			//ThetaA[b][n] + pow(ThetaA[b][s], 3) / 3.0 + (2.0 / 15.0) * pow(ThetaA[b][s], 5) = tan(ThetaA[b][s])
			//blade segment angle of attack, [deg]
			double alphaY = atan2((UT[b][s] * tan(Theta[b][s]) + UP[b][s]) * cosGamma, UT[b][s] - UP[b][s] * tan(Theta[b][s]) * pow(cosGamma, 2)) * Convert::radToDeg;
			alphaY = limit(alphaY, -180.0, 180.0);


			//yawed alpha is modified here for input to CL lookup table (sweep theory-> unyawed alpha)
			if (abs(alphaY) >= 0 && abs(alphaY) <= 13.5 / cosGamma)
			{
				if (abs(alphaY) > 90.0)
				{
					alphaTrans[b][s] = abs(alphaY * cosGamma + alphaY / abs(alphaY) * 180.0 * (1.0 - cosGamma));
				}
				else
				{
					alphaTrans[b][s] = abs(alphaY * cosGamma);
				}
			}
			else if (abs(alphaY) >= (180.0 - 8.0 / cosGamma) && abs(alphaY) <= 180.0)
			{
				if (abs(alphaY) > 90.0)
				{
					alphaTrans[b][s] = abs(alphaY * cosGamma + alphaY / abs(alphaY) * 180.0 * (1.0 - cosGamma));
				}
				else
				{
					alphaTrans[b][s] = abs(alphaY * cosGamma);
				}
			}
			else if (abs(alphaY) >= 13.5 / cosGamma && abs(alphaY) <= (180.0 - 8.0 / cosGamma))
			{
				alphaTrans[b][s] = abs(alphaY);
			}
			alphaTrans[b][s] = limit(alphaTrans[b][s], 0.0, 180.0);
			if (alphaY < 0.0)
			{
				//alphaTrans[b][s] = -alphaTrans[b][s];
			}

			//double MachS = pow(UT[b][s] * UT[b][s] + UP[b][s] * UP[b][s], 0.5) * OmegaT * RMR / p_EFMdata.speedOfSound_fts;// blade segment mach, for Cd&Cl table lookup

			// Blade segment lift and drag coefficients
			double CLfinal = 0.0;
			double CDfinal = 0.0;
			//if (alphaTrans[b][s] <= 30.0)
			//{
			//	CLfinal = _CL_NACA0015_30(alphaTrans[b][s], MachS);
			//	CDfinal = _CD_NACA0015_30(abs(alphaY), MachS);
			//}
			//else if (alphaTrans[b][s] > 30.0)
			//{
			CLfinal = fn_CL_NACA0015.interpnf1(alphaTrans[b][s]);// _CL_NACA0015(alphaTrans[b][s]);
			CDfinal = fn_CD_NACA0015.interpnf1(abs(alphaY));//_CD_NACA0015(abs(alphaY));
			//}
			
			if (s < NUM_BLADE_SEGMENTS - 1)
			{
				CLY[b][s] = CLfinal * 1.0;// _CL_NACA0015(alphaTrans[b][n]);
			}
			if (s == NUM_BLADE_SEGMENTS - 1)//last segment only
			{
				CLY[b][s] = (1.0 - ((1.0 - BMR) / DeltaY[NUM_BLADE_SEGMENTS - 1])) * CLfinal;// _CL_NACA0015(alphaTrans[b][n]);
			}
			if (alphaY < 0.0)
			{
				CLY[b][s] = -CLY[b][s];
			}

			CDY[b][s] = CDfinal;// _CD_NACA0015(limit(abs(alphaY), 0.0, 180.0)) + DeltaCD;

			// Blade segment forces (blade span axis)
			FP[b][s] = 0.5 * p_EFMdata.rho_SlgFt3 * pow(OmegaT, 2) * pow(RMR, 3) * (CR * DeltaY[s]) * UYAW * (CLY[b][s] * (UT[b][s] / cosGamma) + CDY[b][s] * UP[b][s]);// main source of lift
			FT[b][s] = 0.5 * p_EFMdata.rho_SlgFt3 * pow(OmegaT, 2) * pow(RMR, 3) * (CR * DeltaY[s]) * UYAW * (CDY[b][s] * UT[b][s] - CLY[b][s] * UP[b][s] * cosGamma);
			FR[b][s] = 0.5 * p_EFMdata.rho_SlgFt3 * pow(OmegaT, 2) * pow(RMR, 3) * (CR * DeltaY[s]) * UYAW * (CDY[b][s] - CLY[b][s] * (UP[b][s] / UT[b][s]) * cosGamma) * UR[b][s];

		}// end of blade segment calculations

		//Blade aerodynamic shears (blade span axis)
		double FPB = 0.0;//blade shear force perp., [lb]
		double FTB = 0.0;//blade shear force tangential, [lb]
		double FRB = 0.0;//blade shear force radial, [lb]
		for (int s = 0; s < NUM_BLADE_SEGMENTS; s++)
		{
			FPB += FP[b][s];
			FTB += FT[b][s];
			FRB += FR[b][s];
		}


		// Blade aerodynamic shears (shaft axis)
		FXA[b] = FRB * CosBeta[b] * SinDelt[b] - FTB * CosDelt[b] - FPB * SinBeta[b] * SinDelt[b];
		FYA[b] = FRB * CosBeta[b] * CosDelt[b] + FTB * SinDelt[b] - FPB * SinBeta[b] * CosDelt[b];
		FZA[b] = -(FRB * SinBeta[b] + FPB * CosBeta[b]);

		// Moments about blade flap+lag hinges
		MFA[b] = 0.0;
		for (int s = 0; s < NUM_BLADE_SEGMENTS; s++)
		{
			MFA[b] += Y2[s] * FP[b][s];
		}
		MFA[b] = MFA[b] * RMR;

		MLA[b] = 0.0;
		for (int s = 0; s < NUM_BLADE_SEGMENTS; s++)
		{
			MLA[b] += Y2[s] * FT[b][s];
		}
		MLA[b] = MLA[b] * RMR;

		// Blade hinge contraint moments
		MFD[b] = -(Kbeta * Beta[b] + KbetaDot * BetaDot[b]);// this is 0 for articulated rotors, non-zero for hingeless rotors
		
#ifdef USE_LAG_DOF
		//double FdeltDot = fn_lagDamp.interpnf1(limit(abs(DeltDot[b]), 0.0, 0.82));// lagging hinge damper const, [ft-lb-sec/rad]
		//if (DeltDot[b] < 0.0)
		//{
		//	FdeltDot = -FdeltDot;
		//}
		//MLD[b] = 1450.0 * DeltDot[b];// LagDampArm * FdeltDot;
#endif

		// Blade inertia shear forces at hinge (rotating shaft axis), [lb]
		double FXI = Mb * (CosBeta[b] * CosDelt[b] * (rDotS - OmegaDot - DeltDotDot[b])
			+ 2.0 * SinBeta[b] * CosDelt[b] * (DeltDot[b] * BetaDot[b] + (Omega - rS) * BetaDot[b])
			+ CosBeta[b] * SinDelt[b] * (pow(DeltDot[b], 2) + pow(BetaDot[b], 2) + 2.0 * (Omega - rS) * DeltDot[b] + pow(Omega - rS, 2))
			+ 2.0 * BetaDot[b] * CosBeta[b] * (pS * CosPsi[b] - qS * SinPsi[b])
			+ BetaDotDot[b] * SinBeta[b] * SinDelt[b])
			- (Wb / Convert::gravity_fts2) * (VXSDot * SinPsi[b] + VYSDot * CosPsi[b]);
		double FYI = Mb * (CosBeta[b] * CosDelt[b] * (pow(DeltDot[b], 2) + pow(BetaDot[b], 2) + 2.0 * (Omega - rS) * DeltDot[b] + pow(Omega - rS, 2))
			+ BetaDotDot[b] * SinBeta[b] * CosDelt[b] + DeltDotDot[b] * CosBeta[b] * SinDelt[b] - 2.0 * BetaDot[b] * CosBeta[b] * (pS * SinPsi[b] + qS * CosPsi[b])
			+ (Wb * e / (Convert::gravity_fts2 * Mb)) * pow(Omega - rS, 2))
			+ (Wb / Convert::gravity_fts2) * (VXSDot * CosPsi[b] - VYSDot * SinPsi[b]);
		double FZI = Mb * (BetaDotDot[b] * CosBeta[b] - pow(BetaDot[b], 2) * SinBeta[b] + 2.0 * BetaDot[b] * SinBeta[b] * CosDelt[b] * (pS * SinPsi[b] + qS * CosPsi[b])
			+ CosBeta[b] * SinDelt[b] * (2.0 * (Omega + DeltDot[b]) * (pS * SinPsi[b] + qS * CosPsi[b]) + qDotS * SinPsi[b] - pDotS * CosPsi[b])
			- CosBeta[b] * CosDelt[b] * (2.0 * (Omega + DeltDot[b]) * (pS * CosPsi[b] - qS * SinPsi[b]) + pDotS * SinPsi[b] + qDotS * CosPsi[b])
			- (Wb * e / (Convert::gravity_fts2 * Mb)) * (2.0 * Omega * (pS * CosPsi[b] - qS * SinPsi[b]) + pDotS * SinPsi[b] + qDotS * CosPsi[b]))
			- (Wb / Convert::gravity_fts2) * VZSDot;

		
		// Total blade shear forces at hinge (shaft axis)
		FXT[b] = FXA[b] + FXI;
		FYT[b] = FYA[b] + FYI;
		FZT[b] = (FZA[b] + FZI) * Kge;

		double bladeHealth = p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
		FXT[b] *= bladeHealth;
		FYT[b] *= bladeHealth;
		FZT[b] *= bladeHealth;

		// convert rotating shaft axis to rotor hub axis
		double XB = -(FYT[b] * CosPsi[b] - FXT[b] * SinPsi[b]);
		double YB = (FXT[b] * CosPsi[b] + FYT[b] * SinPsi[b]);
		double ZB = FZT[b];

		ForceComponent bladeForce;
		bladeForce.dir.x = limit(XB * cos(iS) + ZB * sin(iS), -10000.0, 10000.0) * Convert::lbf_to_N;
		bladeForce.dir.y = limit(-(-XB * sin(iS) + ZB * cos(iS)), -10000.0, 10000.0) * Convert::lbf_to_N;
		bladeForce.dir.z = limit(YB, -10000.0, 10000.0) * Convert::lbf_to_N;
		bladeForce.pos.x = (lMR - e * CosPsi[b] )* Convert::feetToMeter;
		bladeForce.pos.y = -hMR * Convert::feetToMeter;
		bladeForce.pos.z = (bMR + e * SinPsi[b]) * Convert::feetToMeter;
		aeroForces.push_back(bladeForce);



	}// end of individual blade calculations


	/*
	//double T = 0.0;//Total thrust force shaft axis, [lb]
	for (int b = 0; b < NUM_BLADES; b++)
	{
	//	T += FZT[b]* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	}
	//T *= KGE;

	//double H = 0.0;//Total drag force shaft axis, [lb]
	for (int b = 0; b < NUM_BLADES; b++)
	{
		//H += (FYT[b] * CosPsi[b] - FXT[b] * SinPsi[b])* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	}
	//H = -H;

	//double J = 0.0;//Total side force shaft axis, [lb]
	for (int b = 0; b < NUM_BLADES; b++)
	{
		//J += (FXT[b] * CosPsi[b] + FYT[b] * SinPsi[b])* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	}
	*/
	double TA = 0.0;//aerodynamic thrust for downwash calc, [lb]
	for (int b = 0; b < NUM_BLADES; b++)
	{
		TA += FZA[b] * p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	}
	TA = -TA;

	CTA = TA / (2.0 * p_EFMdata.rho_SlgFt3 * pow(OmegaT, 2) * pow(RMR, 4) * M_PI);// MR thrust coef used for downwash calc


	double LH = 0.0;//MR rolling moment shaft axis, [ft-lb]
	//for (int b = 0; b < NUM_BLADES; b++)
	//{
		//LH += ((MFD[b] * CosDelt[b] - MLD[b] * SinBeta[b] * SinDelt[b]) * SinPsi[b] + (MFD[b] * SinDelt[b] + MLD[b] * SinBeta[b] * CosDelt[b]) * CosPsi[b])* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	//}

	double MH = 0.0;//MR pitching moment shaft axis, [ft-lb]
	//for (int b = 0; b < NUM_BLADES; b++)
	//{
		//MH += ((MFD[b] * CosDelt[b] - MLD[b] * SinBeta[b] * SinDelt[b]) * CosPsi[b] + (MFD[b] * SinDelt[b] + MLD[b] * SinBeta[b] * CosDelt[b]) * SinPsi[b])* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	//}

	double Q = 0.0;//MR torque shaft axis, [ft-lb]
	for (int b = 0; b < NUM_BLADES; b++)
	{
		//Q += (e* FXT[b] + MLD[b] * CosBeta[b]);//note: if lag DOF enabled
		Q += (e * FXT[b] - MLA[b] * CosBeta[b])* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];// if no lag DOF
	}
	Q = -Q;
	QMR = Q;

	double Q2 = 0.0;//MR torque shaft axis, [ft-lb]
	for (int b = 0; b < NUM_BLADES; b++)
	{
		//Q += (e* FXT[b] + MLD[b] * CosBeta[b]);//note: if lag DOF enabled
		Q2 += (- MLA[b] * CosBeta[b])* p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];// if no lag DOF
	}
	Q2 = -Q2*0.5;

	//shaft axis to body axis transformation & filter
	double Kforce = KfPrime / p_EFMdata.deltaTime;// filter constant
	//XMR = ((Kforce - 1.0) / Kforce) * XMR + (1.0 / Kforce) * (H * cos(iS));
	//YMR = ((Kforce - 1.0) / Kforce) * YMR + (1.0 / Kforce) * (J);
	//ZMR = ((Kforce - 1.0) / Kforce) * ZMR + (1.0 / Kforce) * (-H * sin(iS));
	LMR = ((Kforce - 1.0) / Kforce) * LMR + (1.0 / Kforce) * (LH * cos(iS) + Q2 * sin(iS));
	MMR = ((Kforce - 1.0) / Kforce) * MMR + (1.0 / Kforce) * (MH);
	NMR = ((Kforce - 1.0) / Kforce) * NMR + (1.0 / Kforce) * (-LH * sin(iS) + Q2 * cos(iS));



	double a1SF = 0.0;//fourier series coef, flapping, [deg]
	for (int b = 0; b < NUM_BLADES; b++)
	{
		a1SF += Beta[b] * CosPsi[b] * p_Damage.elementIntegrity[BLADE_1_CENTER + b * 3];
	}
	a1SF *= -2.0 / NUM_BLADES * Convert::radToDeg;

	skewAngleMR = atan2(MuXS, abs(LambdaMR)) * Convert::radToDeg + a1SF;//[deg], angle between rotor wake and MR shaft (i.e. is rotor wake straight down or washed rearward due to forward velocity)

	double animPos = PsiMR / (2.0*M_PI);//animation is 0-1, but PsiMR is -Pi to Pi
	if (animPos < 0)
	{
		animPos += 1.0;
	}
	cockpitAPI.setExternalDrawArg(EXT_RotorSpin, (float)LinInterp(animPos, 0, 1, 1, 0));// linInterp required due to external model error of incorrect rotor spin direction


	if (p_EFMdata.time > p_EFMdata.deltaTime * 2)//delay adding forces if there is an initialization issue
	{
		//ForceComponent MainRotorForce;
		//MainRotorForce.dir.x = limit(XMR, -10000.0, 10000.0) * Convert::lbf_to_N;
		//MainRotorForce.dir.y = 0;// limit(-ZMR, -10000.0, 10000.0)* Convert::lbf_to_N;
		//MainRotorForce.dir.z = limit(YMR, -10000.0, 10000.0) * Convert::lbf_to_N;
		//MainRotorForce.pos.x = lMR * Convert::feetToMeter;
		//MainRotorForce.pos.y = -hMR * Convert::feetToMeter;
		//MainRotorForce.pos.z = bMR * Convert::feetToMeter;
		//aeroForces.push_back(MainRotorForce);

		Vec3 mrMoment;
		mrMoment.x = limit(LMR, -5000.0, 5000.0) * Convert::lbft_to_Nm;
		mrMoment.y = limit(-NMR, -5000.0, 5000.0) * Convert::lbft_to_Nm;
		mrMoment.z = limit(MMR, -5000.0, 5000.0) * Convert::lbft_to_Nm;
		aeroMoments.push_back(mrMoment);
	}
}

//todo add rearward n coef table
//todo validate wash factor tables
void AH6Aero::FuselageModule()
{
	double EKFX = fn_EKFX.interpnf1(limit(skewAngleMR, -20.0, 90.0));//rotor wash factor on fuselage
	double EKFZ = fn_EKFZ.interpnf1(limit(skewAngleMR, -20.0, 90.0));//rotor wash factor on fuselage

	double VXF = VXB + EKFX * (DWMR * OmegaT * RMR);//fuselage x body axis velocity, [ft/s]
	double VYF = VYB;								//fuselage y body axis velocity, [ft/s]
	double VZF = VZB - EKFZ * (DWMR * OmegaT * RMR);//fuselage z body axis velocity, [ft/s]
	double qF = 0.5 * p_EFMdata.rho_SlgFt3 * (VXF * VXF + VYF * VYF + VZF * VZF);//dynamic pressure at fuselage, [lb/ft^2]

	double alphaF = atan2(VZF, VXF);//AoA of fuselage, [rad]
	alphaF_deg = alphaF * Convert::radToDeg;
	double BetaF = asin(VYF / sqrt(VXF * VXF + VYF * VYF + VZF * VZF));//angle of sideslip fuselage, [rad]
	double BetaF_deg = BetaF * Convert::radToDeg;
	PsiFdeg = -BetaF * Convert::radToDeg;

	double Xf = qF * SPF * fn_Cx_Fuselage.interpnf1(alphaF_deg);
	double Yf = qF * SSF * fn_Cz_Fuselage.interpnf1(BetaF_deg);
	double Zf = qF * SPF * fn_Cz_Fuselage.interpnf1(alphaF_deg);

	double Mf = qF * SPF * fn_Cm_Fuselage.interpnf1(alphaF_deg);
	double Nf = qF * SSF * fn_Cna_Fuselage.interpnf1(BetaF_deg);


	ForceComponent FuseForce;
	FuseForce.dir.x = Xf * Convert::lbf_to_N;
	FuseForce.dir.y = -Zf * Convert::lbf_to_N;
	FuseForce.dir.z = Yf * Convert::lbf_to_N;
	FuseForce.pos.x = 0;
	FuseForce.pos.y = 0;
	FuseForce.pos.z = 0;
	aeroForces.push_back(FuseForce);

	Vec3 fuseMoment;
	fuseMoment.x = 0;
	fuseMoment.y = -Nf * Convert::lbft_to_Nm;
	fuseMoment.z = Mf * Convert::lbft_to_Nm;
	aeroMoments.push_back(fuseMoment);

}//end fuselage module

void AH6Aero::EmpennageModule()
{
	/*
	todo: need to change wash factor to just be effective at high wake angles
	*/


	//EKTXU = _EKTXU(limit(skewAngleMR, -20, 100));
	//EKTZU = _EKTZU(limit(skewAngleMR, -20, 100));

	//double epsilonWT;//data tables***
	//double epsilonWTU = _EpsilonWTU(limit(alphaFdeg, -8, 28));//wing downwash angle at upper HT, [deg]
	//double VZIW = (epsilonWT / 57.3) * VXB;
	//VZIWU = (epsilonWTU / 57.3) * VXB;

	// Sidewash
	//double sigmaWT = _SigmaWT(limit(abs(-PsiFdeg), 0, 60));//wing sidewash angle at VT, [deg]
	//VYIW = (sigmaWT / 57.3) * VXB;

	//double TauT = lHT / VXB;
	//TauTU = lHTU / VXB;

	//-----------------------------------------
	//		 Horizontal tail
	//-----------------------------------------
	double lHT = (FSHT - FSCG) / 12.0;//horizontal tail moment arm x axis, [ft]
	double hHT = (WLHT - WLCG) / 12.0;//horizontal tail moment arm z axis, [ft]

	double KQHT;//dynamic pressure loss factor HT
	if (alphaF_deg >= 10.0 && alphaF_deg <= 46.0)
	{
		KQHT = sqrt((0.4 + 0.0333 * abs(alphaF_deg - 28.0)));
	}
	else
	{
		KQHT = 1.0;
	}

	double VXHT = VXB * KQHT - q * hHT;// +EKTXU * (DWMR * OmegaT * RMR);//HT x body axis velocity, [ft/s]
	double VYHT = VYB - r * lHT + p * hHT;// - VYIW* TauTU;//HT y body axis velocity, [ft/s]
	double VZHT = VZB + q * lHT;// -VZIWU * TauTU - EKTZU * (DWMR * OmegaT * RMR);//HT z body axis velocity, [ft/s]
	double VHT2 = VXHT * VXHT + VYHT * VYHT + VZHT * VZHT;
	double qHT = 0.5 * p_EFMdata.rho_SlgFt3 * VHT2;//dynamic pressure at HT, [lb/ft^2]

	double alphaHT = atan2(VZHT, VXHT);//Horiz Tail AoA, [rad]
	double alphaHTT = alphaHT * Convert::radToDeg + iHT;//total AoA of HT, [deg]

	double CL_HT = 0.0;// fn_CL_NACA0015.interpnf1(abs(alphaHTT));
	double CD_HT = 0.0;// fn_CD_NACA0015.interpnf1(abs(alphaHTT));
	double machHT = sqrt(VHT2) / p_EFMdata.speedOfSound_fts;
	//if (abs(alphaHTT) <= 30.0)
	//{
	//	CL_HT = _CL_NACA0015_30(abs(alphaHTT),machHT);
	//	CD_HT = _CD_NACA0015_30(abs(alphaHTT), machHT);
	//}
	//else
	//{
	CL_HT = fn_CL_NACA0015.interpnf1(abs(alphaHTT));
	CD_HT = fn_CD_NACA0015.interpnf1(abs(alphaHTT));
	//}
	if (alphaHTT < 0)
	{
		CL_HT = -CL_HT;
	}
	//double CD_HT = fn_CD_NACA0015.interpnf1(abs(alphaHTT));

	double XHTU = -(CD_HT * cos(alphaHT) * cos(alphaVT) - CL_HT * sin(alphaHT)) * SAHT * qHT;//HT longitudinal force, [lb]
	double YHTU = -(CD_HT * sin(alphaVT)) * qHT;//HT lateral force, lb
	double ZHTU = -(CD_HT * sin(alphaHT) * cos(alphaVT) + CL_HT * cos(alphaHT)) * SAHT * qHT;//HT vertical force, [lb]

	ForceComponent HTForce;
	HTForce.dir.x = XHTU * Convert::lbf_to_N;
	HTForce.dir.y = -ZHTU * Convert::lbf_to_N;
	HTForce.dir.z = YHTU * Convert::lbf_to_N;
	HTForce.pos.x = -lHT * Convert::feetToMeter;
	HTForce.pos.y = hHT * Convert::feetToMeter;
	HTForce.pos.z = 0.0;
	aeroForces.push_back(HTForce);

	//--------------------------------------
	//		 vertical tail 
	//--------------------------------------
	double lVT = (FSVT - FSCG) / 12.0;//vertical tail moment arm x axis, [ft]
	double hVT = (WLVT - WLCG) / 12.0;//vertical tail moment arm z axis, [ft]

	double qVTqBar;
	if (PsiFdeg >= -17.5 && PsiFdeg <= 17.5)
	{
		qVTqBar = 0.68 + 0.01314 * abs(PsiFdeg);
	}
	else if (PsiFdeg < -17.5)
	{
		qVTqBar = 0.91 - 0.00124 * (PsiFdeg + 17.5);
	}
	else if (PsiFdeg > 17.5)
	{
		qVTqBar = 0.91 + 0.00124 * (PsiFdeg - 17.5);
	}
	//double qVTqBarAS = _qVTqBar(limit(PsiFdeg, 0, 30), limit(abs(alphaFdeg), -20, 20));
	KQVT = 1.0;// pow(qVTqBar + qVTqBarAS, 0.5);


	double VXVT = VXB * KQVT - q * hVT;// +EKTXU * (DWMR * OmegaT * RMR);//VT x axis velocity, [ft/sec]
	double VYVT = VYB - r * lVT + p * hVT;// -VYIW * TauTU + EKTR * (DWMR * OmegaT * RMR);//VT y axis velocity, [ft/sec]
	double VZVT = VZB + q * lVT;// -VZIWU * TauTU - EKTZU * (DWMR * OmegaT * RMR);//VT z axis velocity, [ft/sec]
	double VVT2 = VXVT * VXVT + VYVT * VYVT + VZVT * VZVT;
	double qVT = 0.5 * p_EFMdata.rho_SlgFt3 * VVT2;// dynamic presure at VT

	alphaVT = asin(VYVT / sqrt(VVT2));//[rad]
	double alphaVTT = alphaVT * Convert::radToDeg;//AoA VT, [deg]

	double CL_VT = 0.0;// fn_CL_NACA0015.interpnf1(abs(alphaVTT));
	double CD_VT = 0.0;// fn_CD_NACA0015.interpnf1(abs(alphaVTT));
	double machVT = sqrt(VVT2) / p_EFMdata.speedOfSound_fts;
	//if (abs(alphaVTT) <= 30.0)
	//{
	//	CL_VT = _CL_NACA0015_30(abs(alphaVTT), machVT);
	//	CD_VT = _CD_NACA0015_30(abs(alphaVTT), machVT);
	//}
	//else
	//{
	CL_VT = fn_CL_NACA0015.interpnf1(abs(alphaVTT));
	CD_VT = fn_CD_NACA0015.interpnf1(abs(alphaVTT));
	//}
	if (alphaVTT < 0)
	{
		CL_VT = -CL_VT;
	}


	double XVT = -(CD_VT * cos(alphaVT) - CL_VT * sin(alphaVT)) * SAVT * qVT;//VT longitudinal force, [lb]
	double YVT = -(CD_VT * sin(alphaVT) + CL_VT * cos(alphaVT)) * SAVT * qVT;//VT lateral force, [lb]
	double ZVT = 0;//VT vertical force, [lb]

	ForceComponent VTForce;
	VTForce.dir.x = XVT * Convert::lbf_to_N;
	VTForce.dir.y = -ZVT * Convert::lbf_to_N;
	VTForce.dir.z = YVT * Convert::lbf_to_N;
	VTForce.pos.x = -lVT * Convert::feetToMeter;
	VTForce.pos.y = hVT * Convert::feetToMeter;
	VTForce.pos.z = 0;
	aeroForces.push_back(VTForce);
}

void AH6Aero::TailRotorModule()
{
	double ThetaCTR = p_flightControl.PedalInput * 16.0 + 9.0;//TR collective pitch, 14.0 + 4.0

	double lTR = (FSTR - FSCG) / 12.0;//TR moment arm x axis, [ft]
	double hTR = (WLTR - WLCG) / 12.0;//TR moment arm z axis, [ft]
	double bTR = BLTR / 12.0;		  //TR moment arm y axis, [ft]

	double VXTRB = VXB * KQVT - q * hTR + r * bTR;	// +EKTXU * (DWMR * OmegaT * RMR);//TR x body axis velocity, [ft/sec]
	double VYTRB = VYB - r * lTR + p * hTR;			// -VYIW * TauTU;//TR y body axis velocity, [ft/sec]
	double VZTRB = VZB + q * lTR - p * bTR;			// -VZIWU * TauTU - EKTZU * (DWMR * OmegaT * RMR);//TR z body axis velocity, [ft/sec]

	double VXTR = VXTRB;										//TR x shaft axis velocity, [ft/sec]
	double VYTR = VYTRB * cos(GammaTR) + VZTRB * sin(GammaTR);	//TR y shaft axis velocity, [ft/sec]
	double VZTR = -VYTRB * sin(GammaTR) + VZTRB * cos(GammaTR);	//TR z shaft axis velocity, [ft/sec]

	double MuXTR = VXTR / (OmegaTR * RTR);//x shaft axis velocity at TR hub
	double MuYTR = VYTR / (OmegaTR * RTR);//y shaft axis velocity at TR hub
	double MuZTR = VZTR / (OmegaTR * RTR);//z shaft axis velocity at TR hub
	double MuTR2 = pow(MuXTR, 2) + pow(MuYTR, 2);

	double ThetaTR = (ThetaCTR - TTR * da0overdT * tan(Delt3TR)) / 57.3;//actual TR collective pitch angle, [rad]

	double t31 = pow(BTR, 2) / 2.0 + MuTR2 / 4.0;//bailey coef
	double t32 = pow(BTR, 3) / 3.0 + (BTR / 2.0) * MuTR2;//bailey coef
	double t33 = pow(BTR, 4) / 4.0 + (pow(BTR, 2) / 4.0) * MuTR2;//bailey coef

	double GTR = (aTR / 2.0) * TRSolidity;
	double Klambda = KlambdaPrimeTR / p_EFMdata.deltaTime;//downwash filter constant
	DWTR = ((Klambda - 1.0) / Klambda) * DWTR + (1.0 / Klambda) * (GTR * (MuZTR * t31 + ThetaTR * t32 + t33 * Theta1TR / 57.3) / (2.0 * sqrt(MuTR2 + pow(LambdaTR, 2)) + GTR * t31));// TR downwash
	LambdaTR = MuZTR - DWTR;

	double CTHTR = 2.0 * DWTR * sqrt(MuTR2 + pow(LambdaTR, 2));//TR thrust coef
	TTR = CTHTR * pow(Omega / OmegaT, 2) * p_EFMdata.rho_SlgFt3 * M_PI * pow(RTR, 4) * pow(OmegaTR, 2) * KTRBLK;//Thrust TR, [lb]

	double XTR = -DragTR * 0.5 * p_EFMdata.rho_SlgFt3 * pow(VXTR, 2);
	double YTR = TTR * sin(GammaTR);
	double ZTR = -TTR * cos(GammaTR);

	ForceComponent TRForce;
	TRForce.dir.x = XTR * Convert::lbf_to_N;
	TRForce.dir.y = -ZTR * Convert::lbf_to_N;
	TRForce.dir.z = YTR * Convert::lbf_to_N;
	TRForce.pos.x = -lTR * Convert::feetToMeter;
	TRForce.pos.y = hTR * Convert::feetToMeter;
	TRForce.pos.z = bTR * Convert::feetToMeter;
	aeroForces.push_back(TRForce);
}

// todo: add TR torque?, tune inertias/ratios
void AH6Aero::RotorDegreeOfFreedom(double engtorque)
{
	/* Summary of operation
	* Change in collective causes change in MR torque which changes the speed of the rotor, if engine torque is constant.
	* Engine governor tries to keep constant N2 speed, therefore with a change in N2 speed,
	* the governor will change fuel flow, which changes N1, which changes N2 speed and engine torque output.
	* this change in engine torque will correct rotor speed then match required torque of MR
	* // OmegaDot (angular accel)=torque/inertia
	*/


	// torque "required/used" by main rotor	
	QMRfiltered = QMRfiltered * ((KFRQ - 1.0) / KFRQ) + (1.0 / KFRQ) * QMR;//filtered torque (required to maintain constant rpm) from main rotor
	double rotorBrakeTorque = 0.0;
	//if (isRotorBrakeEngaged)// additional torque from rotor brake to help MR stop sooner
	//{
	//	rotorBrakeTorque = LinInterp(Omega, OmegaT * 0.4, 0.0, 200, 500);
	//}
	double netMRTorque = -QMRfiltered - rotorBrakeTorque;

	// torque produced by engine pre-clutch
	double QEng = engtorque; // torque produced by N1 gas producer
	double N2torque = LinInterp(OmegaE, 1.0, OmegaT, 25.0, 600.0);// torque needed to drive N2 turbine, [ft-lb], 600 ft-lb ~=100%rpm torque when clutch disengaged
	double netEngTorque = QEng - N2torque;// remaining torque available to turn MR

	//--- Overrunning Clutch ----------------------------------------------------------------

	if (OmegaE > Omega)
	{
		isClutchEngaged = true;
	}
	else
	{
		isClutchEngaged = false;
	}


	if (isClutchEngaged == true)
	{
		netMRTorque += abs(netEngTorque);
		OmegaE = Omega;
	}

	double OmegaEdot = netEngTorque / JE;//engine shaft acceleration, [rad/sec^2]
	OmegaE += OmegaEdot * p_EFMdata.deltaTime;
	OmegaE = limit(OmegaE, 0.0, OmegaT * 1.2);


	double OmegaMRDot = netMRTorque / JMR;//MR acceleration, [rad/s^2].   to disable rotor DOF set to 0
	Omega += OmegaMRDot * p_EFMdata.deltaTime;
	Omega = limit(Omega, 0.0, OmegaT * 1.2);//limit rotor speed to 120% to avoid FM anomalies, it should probably break at that point anyways


	//----- Outputs for lua indicator script -----
	cockpitAPI.setParamNumber(N2_RPM, OmegaE / OmegaT * 100.0);
	cockpitAPI.setParamNumber(NR_RPM, Omega / OmegaT * 100.0);	
}