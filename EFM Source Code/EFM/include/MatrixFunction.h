// These are utility functions defined by the University of Minnesota Flight Model Code
// They are mostly there to help look up tables.

#pragma once


#include "ndinterp.h"

// mostly just wrapper to assist with interpolation 
// and related buffers and configuration for each calculation
class MatrixFunction
{
private:
	// disallow copies
	MatrixFunction(const MatrixFunction&other) {}
	MatrixFunction& operator=(const MatrixFunction&other) { return *this; }

protected:

	// stuff only for temporary use,
	// avoid reallocations
	UtilBuffer<int> indexVector;
	UtilBuffer<double> m_Tbuf; // reusable buffer to reduce malloc()/free()

	UtilMatrix<double> m_xPointMat; // used in interpolation, reduce reallocation
	UtilMatrix<int> m_indexMat; // used in interpolation, reduce reallocation

public:
	ndinterp::ND_INFO ndinfo; // dimensions descriptor

	double** m_Xmat{}; // pointers to static arrays of data (X matrix)
	double* m_Ydata{}; // pointer to static array of related data (Y)

	//double *m_xPar; // parameters for interpolation (1-3 pars)
	double m_xPar1Limit = 0.0; // upper limit for X-parameter 1 in functions (only upper and only for this)

	MatrixFunction(double* Ydata, int X1size, double *X1)
		: indexVector()
		, m_Tbuf()
		, m_xPointMat()
		, m_indexMat()
		, ndinfo()
		, m_Xmat(nullptr)
		, m_Ydata(Ydata)
	{
		ndinfo.nPoints = nullptr;
		ndinfo.nDimension = 1;
		init();
		ndinfo.nPoints[0] = X1size;
		m_Xmat[0] = X1;
	}

	MatrixFunction(double* Ydata, int X1size, double* X1, int X2size, double* X2)
		: indexVector()
		, m_Tbuf()
		, m_xPointMat()
		, m_indexMat()
		, ndinfo()
		, m_Xmat(nullptr)
		, m_Ydata(Ydata)
	{
		ndinfo.nPoints = nullptr;
		ndinfo.nDimension = 2;
		init();
		
		m_Xmat[0] = X1;
		ndinfo.nPoints[0] = X1size;		
		m_Xmat[1] = X2;
		ndinfo.nPoints[1] = X2size;
	}

	~MatrixFunction()
	{
		if (ndinfo.nPoints != nullptr)
		{
			free(ndinfo.nPoints);
			ndinfo.nPoints = nullptr;
		}
		if (m_Xmat != nullptr)
		{
			free(m_Xmat);
			m_Xmat = nullptr;
		}
		m_indexMat.release();
		m_xPointMat.release();
		m_Tbuf.release();
		indexVector.release();
	}

	void init()
	{
		ndinfo.nPoints = (int*)malloc(ndinfo.nDimension*sizeof(int));

		// just array of pointers to static data
		m_Xmat = (double **)malloc(ndinfo.nDimension*sizeof(double*));

		// 1-3 parameters
		//m_xPar = (double*)malloc(ndinfo.nDimension*sizeof(double*));

		int nVertices = (1 << ndinfo.nDimension);
		m_Tbuf.getVec(nVertices); // preallocate

		// preallocate
		m_xPointMat.allocate(ndinfo.nDimension, 2);
		m_indexMat.allocate(ndinfo.nDimension, 2);

		// preallocate another temporary buffer to reuse
		indexVector.getVec(ndinfo.nDimension);
	}

	double interpnf1Lim(const double xPar1)
	{
		if (xPar1 > m_xPar1Limit) // par 1 is normally alpha
		{
			// use limit
			return interpnf1(m_xPar1Limit);
		}
		return interpnf1(xPar1);
	}
	double interpnf2Lim(const double xPar1, const double xPar2)
	{
		if (xPar1 > m_xPar1Limit) // par 1 is normally alpha
		{
			// use limit
			return interpnf2(m_xPar1Limit, xPar2);
		}
		return interpnf2(xPar1, xPar2);
	}
	double interpnf3Lim(const double xPar1, const double xPar2, const double xPar3)
	{
		if (xPar1 > m_xPar1Limit) // par 1 is normally alpha
		{
			// use limit
			return interpnf3(m_xPar1Limit, xPar2, xPar3);
		}
		return interpnf3(xPar1, xPar2, xPar3);
	}

	double interpnf1(const double xPar1)
	{
		double Xpars[1];
		Xpars[0] = xPar1;

		return ndinterp::interpn(indexVector, m_Xmat, m_Ydata, Xpars, m_xPointMat, m_indexMat, ndinfo, m_Tbuf);
	}

	double interpnf2(const double xPar1, const double xPar2)
	{
		double Xpars[2];
		Xpars[0] = xPar1;
		Xpars[1] = xPar2;

		return ndinterp::interpn(indexVector, m_Xmat, m_Ydata, Xpars, m_xPointMat, m_indexMat, ndinfo, m_Tbuf);
	}

	double interpnf3(const double xPar1, const double xPar2, const double xPar3)
	{
		double Xpars[3];
		Xpars[0] = xPar1;
		Xpars[1] = xPar2;
		Xpars[2] = xPar3;

		return ndinterp::interpn(indexVector, m_Xmat, m_Ydata, Xpars, m_xPointMat, m_indexMat, ndinfo, m_Tbuf);
	}
};

