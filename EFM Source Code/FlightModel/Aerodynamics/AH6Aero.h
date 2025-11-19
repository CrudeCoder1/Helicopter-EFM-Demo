#pragma once

#include "../stdafx.h"
#include "AH6AeroData.h"
#include "../include/UtilityFunctions.h"

namespace Helicopter
{
	class AERO_Function
	{
	public:
		ND_INFO ndinfo; // dimensions descriptor

		double **X; // pointers to static arrays of data (X matrix)

		double *data; // pointer to static array of related data (Y)

		UtilBuffer buf; // reusable buffer to reduce malloc()/free()

		AERO_Function()
			: ndinfo()
			, X(NULL)
			, data(NULL)
			, buf()
		{
			ndinfo.nDimension = 0;
		}

		~AERO_Function()
		{
			if (ndinfo.nPoints != NULL)
			{
				free(ndinfo.nPoints);
				ndinfo.nPoints = NULL;
			}
			if (X != NULL)
			{
				free(X);
				X = NULL;
			}
		}

		void init(const int nDimension)
		{
			ndinfo.nDimension = nDimension;
			ndinfo.nPoints = (int*)malloc(ndinfo.nDimension*sizeof(int));
			X = (double **) malloc(ndinfo.nDimension*sizeof(double*));

			int nVertices = (1<<nDimension);
			buf.getVec(nVertices); // preallocate
		}

		double interpnf(const double *xPar)
		{
			return interpn(X, data, xPar, ndinfo, buf);
		}
	};


	class AH6Aero
	{
	protected:
		double		Xu;
		double		Xw;
		double		Xq;
		double		Xv;
		double		Xp;
		double		Xr;
		double		Xdc;
		double		Xdb;
		double		Xda;
		double		Xdp;

		double		Zu;
		double		Zw;
		double		Zq;
		double		Zv;
		double		Zp;
		double		Zr;
		double		Zdc;
		double		Zdb;
		double		Zda;
		double		Zdp;

		double		Mu;
		double		Mw;
		double		Mq;
		double		Mv;
		double		Mp;
		double		Mr;
		double		Mdc;
		double		Mdb;
		double		Mda;
		double		Mdp;

		double		Yu;
		double		Yw;
		double		Yq;
		double		Yv;
		double		Yp;
		double		Yr;
		double		Ydc;
		double		Ydb;
		double		Yda;
		double		Ydp;

		double		Lu;
		double		Lw;
		double		Lq;
		double		Lv;
		double		Lp;
		double		Lr;
		double		Ldc;
		double		Ldb;
		double		Lda;
		double		Ldp;

		double		Nu;
		double		Nw;
		double		Nq;
		double		Nv;
		double		Np;
		double		Nr;
		double		Ndc;
		double		Ndb;
		double		Nda;
		double		Ndp;

		double		Cx_total;
		double		Cz_total;
		double		Cm_total;
		double		Cy_total;
		double		Cl_total;
		double		Cn_total;
		

		AERO_Function fn_Xu;
		AERO_Function fn_Xw;
		AERO_Function fn_Xq;
		AERO_Function fn_Xv;
		AERO_Function fn_Xp;
		AERO_Function fn_Xr;
		AERO_Function fn_Xdc;
		AERO_Function fn_Xdb;
		AERO_Function fn_Xda;
		AERO_Function fn_Xdp;

		AERO_Function fn_Zu;
		AERO_Function fn_Zw;
		AERO_Function fn_Zq;
		AERO_Function fn_Zv;
		AERO_Function fn_Zp;
		AERO_Function fn_Zr;
		AERO_Function fn_Zdc;
		AERO_Function fn_Zdb;
		AERO_Function fn_Zda;
		AERO_Function fn_Zdp;

		AERO_Function fn_Mu;
		AERO_Function fn_Mw;
		AERO_Function fn_Mq;
		AERO_Function fn_Mv;
		AERO_Function fn_Mp;
		AERO_Function fn_Mr;
		AERO_Function fn_Mdc;
		AERO_Function fn_Mdb;
		AERO_Function fn_Mda;
		AERO_Function fn_Mdp;

		AERO_Function fn_Yu;
		AERO_Function fn_Yw;
		AERO_Function fn_Yq;
		AERO_Function fn_Yv;
		AERO_Function fn_Yp;
		AERO_Function fn_Yr;
		AERO_Function fn_Ydc;
		AERO_Function fn_Ydb;
		AERO_Function fn_Yda;
		AERO_Function fn_Ydp;

		AERO_Function fn_Lu;
		AERO_Function fn_Lw;
		AERO_Function fn_Lq;
		AERO_Function fn_Lv;
		AERO_Function fn_Lp;
		AERO_Function fn_Lr;
		AERO_Function fn_Ldc;
		AERO_Function fn_Ldb;
		AERO_Function fn_Lda;
		AERO_Function fn_Ldp;

		AERO_Function fn_Nu;
		AERO_Function fn_Nw;
		AERO_Function fn_Nq;
		AERO_Function fn_Nv;
		AERO_Function fn_Np;
		AERO_Function fn_Nr;
		AERO_Function fn_Ndc;
		AERO_Function fn_Ndb;
		AERO_Function fn_Nda;
		AERO_Function fn_Ndp;


		double _Xu(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xu.interpnf(x);
		}
		double _Xw(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xw.interpnf(x);
		}
		double _Xq(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xq.interpnf(x);
		}
		double _Xv(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xv.interpnf(x);
		}
		double _Xp(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xp.interpnf(x);
		}
		double _Xr(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xr.interpnf(x);
		}
		double _Xdc(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xdc.interpnf(x);
		}
		double _Xdb(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xdb.interpnf(x);
		}
		double _Xda(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xda.interpnf(x);
		}
		double _Xdp(double airspeed)
		{
			double x[1];	
			x[0] = airspeed;
			return fn_Xdp.interpnf(x);
		}

		double _Zu(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zu.interpnf(x);
		}
		double _Zw(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zw.interpnf(x);
		}
		double _Zq(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zq.interpnf(x);
		}
		double _Zv(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zv.interpnf(x);
		}
		double _Zp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zp.interpnf(x);
		}
		double _Zr(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zr.interpnf(x);
		}
		double _Zdc(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zdc.interpnf(x);
		}
		double _Zdb(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zdb.interpnf(x);
		}
		double _Zda(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zda.interpnf(x);
		}
		double _Zdp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Zdp.interpnf(x);
		}
		
		double _Mu(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mu.interpnf(x);
		}
		double _Mw(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mw.interpnf(x);
		}
		double _Mq(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mq.interpnf(x);
		}
		double _Mv(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mv.interpnf(x);
		}
		double _Mp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mp.interpnf(x);
		}
		double _Mr(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mr.interpnf(x);
		}
		double _Mdc(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mdc.interpnf(x);
		}
		double _Mdb(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mdb.interpnf(x);
		}
		double _Mda(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mda.interpnf(x);
		}
		double _Mdp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Mdp.interpnf(x);
		}

		double _Yu(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yu.interpnf(x);
		}
		double _Yw(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yw.interpnf(x);
		}
		double _Yq(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yq.interpnf(x);
		}
		double _Yv(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yv.interpnf(x);
		}
		double _Yp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yp.interpnf(x);
		}
		double _Yr(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yr.interpnf(x);
		}
		double _Ydc(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ydc.interpnf(x);
		}
		double _Ydb(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ydb.interpnf(x);
		}
		double _Yda(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Yda.interpnf(x);
		}
		double _Ydp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ydp.interpnf(x);
		}

		double _Lu(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lu.interpnf(x);
		}
		double _Lw(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lw.interpnf(x);
		}
		double _Lq(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lq.interpnf(x);
		}
		double _Lv(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lv.interpnf(x);
		}
		double _Lp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lp.interpnf(x);
		}
		double _Lr(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lr.interpnf(x);
		}
		double _Ldc(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ldc.interpnf(x);
		}
		double _Ldb(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ldb.interpnf(x);
		}
		double _Lda(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Lda.interpnf(x);
		}
		double _Ldp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ldp.interpnf(x);
		}

		double _Nu(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Nu.interpnf(x);
		}

		double _Nw(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Nw.interpnf(x);
		}

		double _Nq(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Nq.interpnf(x);
		}

		double _Nv(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Nv.interpnf(x);
		}

		double _Np(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Np.interpnf(x);
		}

		double _Nr(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Nr.interpnf(x);
		}

		double _Ndc(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ndc.interpnf(x);
		}

		double _Ndb(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ndb.interpnf(x);
		}

		double _Nda(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Nda.interpnf(x);
		}

		double _Ndp(double airspeed)
		{
			double x[1];
			x[0] = airspeed;
			return fn_Ndp.interpnf(x);
		}


		void stabilityDerivatives(double airspeed)
		{
			Xu = _Xu(airspeed);
			Xw = _Xw(airspeed);
			Xq = _Xq(airspeed);
			Xv = _Xv(airspeed);
			Xp = _Xp(airspeed);
			Xr = _Xr(airspeed);
			Xdc = _Xdc(airspeed);
			Xdb = _Xdb(airspeed);
			Xda = _Xda(airspeed);
			Xdp = _Xdp(airspeed);

			Zu = _Zu(airspeed);
			Zw = _Zw(airspeed);
			Zq = _Zq(airspeed);
			Zv = _Zv(airspeed);
			Zp = _Zp(airspeed);
			Zr = _Zr(airspeed);
			Zdc = _Zdc(airspeed);
			Zdb = _Zdb(airspeed);
			Zda = _Zda(airspeed);
			Zdp = _Zdp(airspeed);

			Mu = _Mu(airspeed);
			Mw = _Mw(airspeed);
			Mq = _Mq(airspeed);
			Mv = _Mv(airspeed);
			Mp = _Mp(airspeed);
			Mr = _Mr(airspeed);
			Mdc = _Mdc(airspeed);
			Mdb = _Mdb(airspeed);
			Mda = _Mda(airspeed);
			Mdp = _Mdp(airspeed);

			Yu = _Yu(airspeed);
			Yw = _Yw(airspeed);
			Yq = _Yq(airspeed);
			Yv = _Yv(airspeed);
			Yp = _Yp(airspeed);
			Yr = _Yr(airspeed);
			Ydc = _Ydc(airspeed);
			Ydb = _Ydb(airspeed);
			Yda = _Yda(airspeed);
			Ydp = _Ydp(airspeed);

			Lu = _Lu(airspeed);
			Lw = _Lw(airspeed);
			Lq = _Lq(airspeed);
			Lv = _Lv(airspeed);
			Lp = _Lp(airspeed);
			Lr = _Lr(airspeed);
			Ldc = _Ldc(airspeed);
			Ldb = _Ldb(airspeed);
			Lda = _Lda(airspeed);
			Ldp = _Ldp(airspeed);

			Nu = _Nu(airspeed);
			Nw = _Nw(airspeed);
			Nq = _Nq(airspeed);
			Nv = _Nv(airspeed);
			Np = _Np(airspeed);
			Nr = _Nr(airspeed);
			Ndc = _Ndc(airspeed);
			Ndb = _Ndb(airspeed);
			Nda = _Nda(airspeed);
			Ndp = _Ndp(airspeed);
		}
	

		
	public:
		double		groundEffectFactor; // ranges 1-1.3, 1.3 being max ground effect

		AH6Aero();
		~AH6Aero() {};

		/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		compute Cx_tot, Cz_tot, Cm_tot, Cy_tot, Cn_tot, and Cl_total; these are essentially accelerations of each degree of motion
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
		void computeTotals(const double airspeedx, const double airspeedy, const double airspeedz, 
						const double pitchRate, const double rollRate, const double yawRate, 
						const double CollectiveInput, const double PitchInput, const double RollInput, const double PedalInput, 
						const double airspeed_KTS, const double rpm, const double rotorIntegrity, const double tailRotorIntegrity)
		{
			const double airspeed_Limited = limit(airspeedx, -40.0, 130.0);// data tables are based on airspeed between -40 to 130
			stabilityDerivatives(airspeed_Limited); // load stability/control derivatives from table data based on airspeed (in knots)

			/*  Cx_tot		force out nose (-drag)*/
			Cx_total = Xu * airspeedx + Xw * airspeedz + Xq * pitchRate + Xv * airspeedy + Xp * rollRate + Xr * yawRate + (rotorIntegrity * (Xdc * CollectiveInput + Xdb * PitchInput + Xda * RollInput) + Xdp * PedalInput * tailRotorIntegrity) * rpm;

			/*  Cz_tot		force out bottom (lift)*/ 
			Cz_total = (Zu * airspeedx + Zw * airspeedz + Zq * pitchRate + Zv * airspeedy + Zp * rollRate + Zr * yawRate + (rotorIntegrity * (Zdc * CollectiveInput + Zdb * PitchInput + Zda * RollInput) + Zdp * PedalInput * tailRotorIntegrity) * rpm) * groundEffectFactor;
			
			/*  Cm_tot		pitch moment */ 
			Cm_total = Mu * airspeedx + Mw * airspeedz + Mq * pitchRate + Mv * airspeedy + Mp * rollRate + Mr * yawRate + (rotorIntegrity * (Mdc * CollectiveInput + Mdb * PitchInput + Mda * RollInput) + Mdp * PedalInput * tailRotorIntegrity) * rpm;


			/*  Cy_tot		force out right wing */
			Cy_total = Yu * airspeedx + Yw * airspeedz + Yq * pitchRate + Yv * airspeedy + Yp * rollRate + Yr * yawRate + (rotorIntegrity * (Ydc * CollectiveInput + Ydb * PitchInput + Yda * RollInput) + Ydp * PedalInput * tailRotorIntegrity) * rpm;
	
			/*  Cl_total	roll moment */
			Cl_total = Lu * airspeedx + Lw * airspeedz + Lq * pitchRate + Lv * airspeedy + Lp * rollRate + Lr * yawRate + (rotorIntegrity * (Ldc * CollectiveInput + Ldb * PitchInput + Lda * RollInput) + Ldp * PedalInput * tailRotorIntegrity) * rpm;
			
			/*  Cn_tot		 yaw moment */ 
			Cn_total = Nu * airspeedx + Nw * airspeedz + Nq * pitchRate + Nv * airspeedy + Np * rollRate + Nr * yawRate + (rotorIntegrity * (Ndc * CollectiveInput + Ndb * PitchInput + Nda * RollInput) + Ndp * PedalInput * tailRotorIntegrity) * rpm;

		}

		double getCxTotal() const { return Cx_total; }
		double getCzTotal() const { return Cz_total; }
		double getCmTotal() const { return Cm_total; }
		double getCyTotal() const { return Cy_total; }
		double getCnTotal() const { return Cn_total; }
		double getClTotal() const { return Cl_total; }

		// basic hackish ground effect. simply increases overall lift force by up to 30%
		void setGroundEffectFactor(const double factor)
		{
			groundEffectFactor = 1 + factor * 0.3;
		}

	}; // class AH6Aero

	// constructor
	AH6Aero::AH6Aero() :
		groundEffectFactor(0),

		Cx_total(0),
		Cz_total(0),
		Cm_total(0),
		Cy_total(0),
		Cn_total(0),
		Cl_total(0),
		
		Xu(0),
		Xw(0),
		Xq(0),
		Xv(0),
		Xp(0),
		Xr(0),
		Xdc(0),
		Xdb(0),
		Xda(0),
		Xdp(0),

		Zu(0),
		Zw(0),
		Zq(0),
		Zv(0),
		Zp(0),
		Zr(0),
		Zdc(0),
		Zdb(0),
		Zda(0),
		Zdp(0),

		Mu(0),
		Mw(0),
		Mq(0),
		Mv(0),
		Mp(0),
		Mr(0),
		Mdc(0),
		Mdb(0),
		Mda(0),
		Mdp(0),

		Yu(0),
		Yw(0),
		Yq(0),
		Yv(0),
		Yp(0),
		Yr(0),
		Ydc(0),
		Ydb(0),
		Yda(0),
		Ydp(0),

		Lu(0),
		Lw(0),
		Lq(0),
		Lv(0),
		Lp(0),
		Lr(0),
		Ldc(0),
		Ldb(0),
		Lda(0),
		Ldp(0),

		Nu(0),
		Nw(0),
		Nq(0),
		Nv(0),
		Np(0),
		Nr(0),
		Ndc(0),
		Ndb(0),
		Nda(0),
		Ndp(0),


		fn_Xu(),
		fn_Xw(),
		fn_Xq(),
		fn_Xv(),
		fn_Xp(),
		fn_Xr(),
		fn_Xdc(),
		fn_Xdb(),
		fn_Xda(),
		fn_Xdp(),

		fn_Zu(),
		fn_Zw(),
		fn_Zq(),
		fn_Zv(),
		fn_Zp(),
		fn_Zr(),
		fn_Zdc(),
		fn_Zdb(),
		fn_Zda(),
		fn_Zdp(),

		fn_Mu(),
		fn_Mw(),
		fn_Mq(),
		fn_Mv(),
		fn_Mp(),
		fn_Mr(),
		fn_Mdc(),
		fn_Mdb(),
		fn_Mda(),
		fn_Mdp(),

		fn_Yu(),
		fn_Yw(),
		fn_Yq(),
		fn_Yv(),
		fn_Yp(),
		fn_Yr(),
		fn_Ydc(),
		fn_Ydb(),
		fn_Yda(),
		fn_Ydp(),

		fn_Lu(),
		fn_Lw(),
		fn_Lq(),
		fn_Lv(),
		fn_Lp(),
		fn_Lr(),
		fn_Ldc(),
		fn_Ldb(),
		fn_Lda(),
		fn_Ldp(),

		fn_Nu(),
		fn_Nw(),
		fn_Nq(),
		fn_Nv(),
		fn_Np(),
		fn_Nr(),
		fn_Ndc(),
		fn_Ndb(),
		fn_Nda(),
		fn_Ndp()
			
	{
		fn_Xu.init(1);
		fn_Xu.data = _XuData;
		fn_Xu.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xu.X[0] = airspeed1;

		fn_Xw.init(1);
		fn_Xw.data = _XwData;
		fn_Xw.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xw.X[0] = airspeed1;
		
		fn_Xq.init(1);
		fn_Xq.data = _XqData;
		fn_Xq.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xq.X[0] = airspeed1;
		
		fn_Xv.init(1);
		fn_Xv.data = _XvData;
		fn_Xv.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xv.X[0] = airspeed1;
		
		fn_Xp.init(1);
		fn_Xp.data = _XpData;
		fn_Xp.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xp.X[0] = airspeed1;
		
		fn_Xr.init(1);
		fn_Xr.data = _XrData;
		fn_Xr.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xr.X[0] = airspeed1;
		
		fn_Xdc.init(1);
		fn_Xdc.data = _XdcData;
		fn_Xdc.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xdc.X[0] = airspeed1;
		
		fn_Xdb.init(1);
		fn_Xdb.data = _XdbData;
		fn_Xdb.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xdb.X[0] = airspeed1;
		
		fn_Xda.init(1);
		fn_Xda.data = _XdaData;
		fn_Xda.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xda.X[0] = airspeed1;
		
		fn_Xdp.init(1);
		fn_Xdp.data = _XdpData;
		fn_Xdp.ndinfo.nPoints[0] = airspeed1_size;	
		fn_Xdp.X[0] = airspeed1;
/////////////////////////////////////////////////////////////////
		fn_Zu.init(1);
		fn_Zu.data = _ZuData;
		fn_Zu.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zu.X[0] = airspeed1;

		fn_Zw.init(1);
		fn_Zw.data = _ZwData;
		fn_Zw.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zw.X[0] = airspeed1;

		fn_Zq.init(1);
		fn_Zq.data = _ZqData;
		fn_Zq.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zq.X[0] = airspeed1;

		fn_Zv.init(1);
		fn_Zv.data = _ZvData;
		fn_Zv.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zv.X[0] = airspeed1;

		fn_Zp.init(1);
		fn_Zp.data = _ZpData;
		fn_Zp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zp.X[0] = airspeed1;

		fn_Zr.init(1);
		fn_Zr.data = _ZrData;
		fn_Zr.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zr.X[0] = airspeed1;

		fn_Zdc.init(1);
		fn_Zdc.data = _ZdcData;
		fn_Zdc.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zdc.X[0] = airspeed1;

		fn_Zdb.init(1);
		fn_Zdb.data = _ZdbData;
		fn_Zdb.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zdb.X[0] = airspeed1;

		fn_Zda.init(1);
		fn_Zda.data = _ZdaData;
		fn_Zda.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zda.X[0] = airspeed1;

		fn_Zdp.init(1);
		fn_Zdp.data = _ZdpData;
		fn_Zdp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Zdp.X[0] = airspeed1;
/////////////////////////////////////////////////////////////////
		fn_Mu.init(1);
		fn_Mu.data = _MuData;
		fn_Mu.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mu.X[0] = airspeed1;

		fn_Mw.init(1);
		fn_Mw.data = _MwData;
		fn_Mw.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mw.X[0] = airspeed1;

		fn_Mq.init(1);
		fn_Mq.data = _MqData;
		fn_Mq.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mq.X[0] = airspeed1;

		fn_Mv.init(1);
		fn_Mv.data = _MvData;
		fn_Mv.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mv.X[0] = airspeed1;

		fn_Mp.init(1);
		fn_Mp.data = _MpData;
		fn_Mp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mp.X[0] = airspeed1;

		fn_Mr.init(1);
		fn_Mr.data = _MrData;
		fn_Mr.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mr.X[0] = airspeed1;

		fn_Mdc.init(1);
		fn_Mdc.data = _MdcData;
		fn_Mdc.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mdc.X[0] = airspeed1;

		fn_Mdb.init(1);
		fn_Mdb.data = _MdbData;
		fn_Mdb.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mdb.X[0] = airspeed1;

		fn_Mda.init(1);
		fn_Mda.data = _MdaData;
		fn_Mda.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mda.X[0] = airspeed1;

		fn_Mdp.init(1);
		fn_Mdp.data = _MdpData;
		fn_Mdp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Mdp.X[0] = airspeed1;
/////////////////////////////////////////////////////////////////
		fn_Yu.init(1);
		fn_Yu.data = _YuData;
		fn_Yu.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yu.X[0] = airspeed1;

		fn_Yw.init(1);
		fn_Yw.data = _YwData;
		fn_Yw.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yw.X[0] = airspeed1;

		fn_Yq.init(1);
		fn_Yq.data = _YqData;
		fn_Yq.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yq.X[0] = airspeed1;

		fn_Yv.init(1);
		fn_Yv.data = _YvData;
		fn_Yv.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yv.X[0] = airspeed1;

		fn_Yp.init(1);
		fn_Yp.data = _YpData;
		fn_Yp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yp.X[0] = airspeed1;

		fn_Yr.init(1);
		fn_Yr.data = _YrData;
		fn_Yr.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yr.X[0] = airspeed1;

		fn_Ydc.init(1);
		fn_Ydc.data = _YdcData;
		fn_Ydc.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ydc.X[0] = airspeed1;

		fn_Ydb.init(1);
		fn_Ydb.data = _YdbData;
		fn_Ydb.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ydb.X[0] = airspeed1;

		fn_Yda.init(1);
		fn_Yda.data = _YdaData;
		fn_Yda.ndinfo.nPoints[0] = airspeed1_size;
		fn_Yda.X[0] = airspeed1;

		fn_Ydp.init(1);
		fn_Ydp.data = _YdpData;
		fn_Ydp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ydp.X[0] = airspeed1;
/////////////////////////////////////////////////////////////////
		fn_Lu.init(1);
		fn_Lu.data = _LuData;
		fn_Lu.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lu.X[0] = airspeed1;

		fn_Lw.init(1);
		fn_Lw.data = _LwData;
		fn_Lw.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lw.X[0] = airspeed1;

		fn_Lq.init(1);
		fn_Lq.data = _LqData;
		fn_Lq.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lq.X[0] = airspeed1;

		fn_Lv.init(1);
		fn_Lv.data = _LvData;
		fn_Lv.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lv.X[0] = airspeed1;

		fn_Lp.init(1);
		fn_Lp.data = _LpData;
		fn_Lp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lp.X[0] = airspeed1;

		fn_Lr.init(1);
		fn_Lr.data = _LrData;
		fn_Lr.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lr.X[0] = airspeed1;

		fn_Ldc.init(1);
		fn_Ldc.data = _LdcData;
		fn_Ldc.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ldc.X[0] = airspeed1;

		fn_Ldb.init(1);
		fn_Ldb.data = _LdbData;
		fn_Ldb.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ldb.X[0] = airspeed1;

		fn_Lda.init(1);
		fn_Lda.data = _LdaData;
		fn_Lda.ndinfo.nPoints[0] = airspeed1_size;
		fn_Lda.X[0] = airspeed1;

		fn_Ldp.init(1);
		fn_Ldp.data = _LdpData;
		fn_Ldp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ldp.X[0] = airspeed1;
/////////////////////////////////////////////////////////////////
		fn_Nu.init(1);
		fn_Nu.data = _NuData;
		fn_Nu.ndinfo.nPoints[0] = airspeed1_size;
		fn_Nu.X[0] = airspeed1;

		fn_Nw.init(1);
		fn_Nw.data = _NwData;
		fn_Nw.ndinfo.nPoints[0] = airspeed1_size;
		fn_Nw.X[0] = airspeed1;

		fn_Nq.init(1);
		fn_Nq.data = _NqData;
		fn_Nq.ndinfo.nPoints[0] = airspeed1_size;
		fn_Nq.X[0] = airspeed1;

		fn_Nv.init(1);
		fn_Nv.data = _NvData;
		fn_Nv.ndinfo.nPoints[0] = airspeed1_size;
		fn_Nv.X[0] = airspeed1;

		fn_Np.init(1);
		fn_Np.data = _NpData;
		fn_Np.ndinfo.nPoints[0] = airspeed1_size;
		fn_Np.X[0] = airspeed1;

		fn_Nr.init(1);
		fn_Nr.data = _NrData;
		fn_Nr.ndinfo.nPoints[0] = airspeed1_size;
		fn_Nr.X[0] = airspeed1;

		fn_Ndc.init(1);
		fn_Ndc.data = _NdcData;
		fn_Ndc.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ndc.X[0] = airspeed1;

		fn_Ndb.init(1);
		fn_Ndb.data = _NdbData;
		fn_Ndb.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ndb.X[0] = airspeed1;

		fn_Nda.init(1);
		fn_Nda.data = _NdaData;
		fn_Nda.ndinfo.nPoints[0] = airspeed1_size;
		fn_Nda.X[0] = airspeed1;

		fn_Ndp.init(1);
		fn_Ndp.data = _NdpData;
		fn_Ndp.ndinfo.nPoints[0] = airspeed1_size;
		fn_Ndp.X[0] = airspeed1;
	} // AH6Aero::AH6Aero()
}