//-----------------------------------------------------------------------------------------------
// These are utility functions defined by the University of Minnesota Flight Model Code
//-----------------------------------------------------------------------------------------------
//
// Original code: mexndinterp.c
// From non-linear F-16 simulation at: http://www.aem.umn.edu/people/faculty/balas/darpa_sec/SEC.Software.html
//
//-----------------------------------------------------------------------------------------------
// Modified to reduce re-allocations of buffers, added helpers to keep track of buffer data
// and moved most of allocations outside of primary loops (caller must allocate with helpers).
// This should improve performance (by reducing allocations) 
// and should reduce memory fragmentation (also improves performance).
// As a bonus, code should simpler to follow.
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>
//


#pragma once

#include <cmath>

#include <malloc.h>
#include <memory.h>

// simple buffer helper
// reduce repeating malloc()/free()
template<typename T> class UtilBuffer
{
private:
	// disallow copies
	UtilBuffer(const UtilBuffer &other) {}
	UtilBuffer& operator=(const UtilBuffer &other) { return *this; }

public:
	T *m_vec; // array
	size_t capacity; // amount of elements in array
	//int used; // amount of elements in use

	UtilBuffer()
		: m_vec(nullptr)
		, capacity(0)
	{}
	UtilBuffer(const size_t elements)
		: m_vec(nullptr)
		, capacity(elements)
	{
		m_vec = (T*)malloc(elements*sizeof(T));
	}
	~UtilBuffer() 
	{
		// cleanup on destroy (exiting scope)
		release();
	}
	void release()
	{
		if (m_vec != nullptr)
		{
			free(m_vec);
			m_vec = nullptr;
		}
	}

	// support growing as needed
	T *getVec(const size_t elements)
	{
		if (elements > capacity)
		{
			free(m_vec);
			capacity = elements;
			m_vec = (T*)malloc(elements*sizeof(T));
		}
		return m_vec;
	}

	// copy from source, grow if needed
	void copyVec(const size_t elements, const T *src)
	{
		// Note: T should be same type

		size_t toCopy = elements*sizeof(T);
		T *dest = getVec(elements); // check for size
		memcpy(dest, src, toCopy);
	}
};

template<typename T> class UtilMatrix
{
private:
	// disallow copies
	UtilMatrix(const UtilMatrix &other) {}
	UtilMatrix& operator=(const UtilMatrix &other) { return *this; }

public:
	T **m_mat;
	size_t m_n;
	size_t m_m;

	UtilMatrix()
		: m_mat(nullptr), m_n(0), m_m(0)
	{}
	~UtilMatrix() 
	{
		release();
	}

	void allocate(const size_t n, const size_t m)
	{
		// in case need to allocate different size
		if (n != m_n || m != m_m)
		{
			release();
		}

		m_n = n; m_m = m;
		m_mat = (T**)malloc(m_n*sizeof(T*));
		for (size_t i = 0; i < m_n; i++)
		{
			m_mat[i] = (T*)malloc(m_m*sizeof(T));
		}
	}
	void release()
	{
		if (m_mat == nullptr)
		{
			return;
		}

		for (size_t i = 0; i < m_n; i++)
		{
			free(m_mat[i]);
		}
		free(m_mat);
		m_mat = nullptr;
	}
};

// Start of Utility Functions
namespace ndinterp
{

// Struct to define a set of data with a given number of dimenions and points
typedef struct {
		int nDimension;
		int *nPoints;   
		} ND_INFO;



/************************************************/
/*    Get the indices of the hyper cube in the  */
/*    grid in which the point lies              */
/************************************************/
inline bool getHyperCube(double **Xmat, UtilMatrix<int> &indexMatrix, const double *V, const ND_INFO &ndinfo)
{
	for(int i=0; i<ndinfo.nDimension; i++)
	{
		int indexMax = ndinfo.nPoints[i]; /* Get the total # of points in this dimension */
		double xmax = Xmat[i][indexMax-1];	 /* Get the upper bound along this axis */
		double xmin = Xmat[i][0];			/* Get the lower bound along this axis */

		/**************************************************************************** 
			It has been assumed that the gridpoints are monotonically increasing
			the zero index is the minimum and the max-1 is the maximum.
		*****************************************************************************/

		/****************************************************************************
        		Get the ith component in the vector V, the point at which we want to 
        		interpolate
		****************************************************************************/
		double x = V[i];

		/* Check to see if this point is within the bound */
		if(x<xmin || x>xmax)
		{
			//ErrMsg("Point lies out data grid (in getHyperCube)");

			// can this happen in normal case or should we just bail out now?
			//return false;
		}
		else
		{
			for(int j=0; j<indexMax-1; j++)
			{
				if(x==Xmat[i][j])
				{
					indexMatrix.m_mat[i][0] = indexMatrix.m_mat[i][1] = j;
					break;
				}
				if(x==Xmat[i][j+1])
				{
					indexMatrix.m_mat[i][0] = indexMatrix.m_mat[i][1] = j + 1;
					break;
				}
				if(x > Xmat[i][j] && x < Xmat[i][j+1] )
				{
					indexMatrix.m_mat[i][0] = j;
					indexMatrix.m_mat[i][1] = j + 1;
					break;
				}
			}/*End of for(j=...) */
		}/*End of if-else */
	}/* End of for(i= ...) */

	return true;
} // getHyperCube()

/*********************************************************************************
 indexVector contains the co-ordinate of a point in the ndimensional grid
 the indices along each axis are assumed to begin from zero
 *********************************************************************************/
inline int getLinIndex(const UtilBuffer<int> &indexVector, const ND_INFO &ndinfo)
{
	int linIndex=0;
	for(int i=0; i<ndinfo.nDimension; i++)
	{
		int P=1;
		for(int j=0; j<i; j++)
		{
			P = P*ndinfo.nPoints[j];
		}
		linIndex = linIndex + P*indexVector.m_vec[i];
	}
	return(linIndex);
}

// Linearly interpolate between two data values
inline double linearInterpolate(const UtilBuffer<double> &Tbuf, const double *V, UtilMatrix<double> &Xmat, UtilBuffer<int> &indexVector, const ND_INFO &ndinfo)
{
	int nVertices = 1<<(ndinfo.nDimension);

	UtilBuffer<double> oldTbuf(nVertices);
	oldTbuf.copyVec(nVertices, Tbuf.m_vec);

	// reuse buffer until done here
	UtilBuffer<double> newTbuf;

	int n = ndinfo.nDimension;
	int dimNum = 0;
	while(n>0)
	{
		int m = n-1;
		nVertices = (1<<m);

		newTbuf.getVec(nVertices); // prepare for next loop
		for(int i=0; i<nVertices; i++)
		{
			for(int j=0; j<m; j++)
			{
				int mask = (1<<j);
				indexVector.m_vec[j] =  (mask & i) >> j;
			}/*End of for j*/

			int index1 = 0;
			int index2 = 0;
			for(int j=0; j<m; j++)
			{
				index1 = index1 + (1<<(j+1))*indexVector.m_vec[j];
				index2 = index2 + (1<<j)*indexVector.m_vec[j];
			}/*End of for j*/

			double f1 = oldTbuf.m_vec[index1];
			double f2 = oldTbuf.m_vec[index1+1];
			if (Xmat.m_mat[dimNum][0] != Xmat.m_mat[dimNum][1])
			{
				double lambda = (V[dimNum] - Xmat.m_mat[dimNum][0]) / (Xmat.m_mat[dimNum][1] - Xmat.m_mat[dimNum][0]);

				newTbuf.m_vec[index2] = lambda*f2 + (1-lambda)*f1;
			}
			else
			{
				newTbuf.m_vec[index2] = f1;
			}
		}/*End of for i*/

		oldTbuf.copyVec(nVertices, newTbuf.m_vec);
		n=m;
		dimNum++;
	}/* End of while*/

	return oldTbuf.m_vec[0];
} // linearInterpolate()

/*indexMatrix[i][0] => Lower, ...[1]=>Higher*/
inline double interpn(UtilBuffer<int> &indexVector, double **Xmat, const double *Y, const double *xPar, UtilMatrix<double> &xPoint, UtilMatrix<int> &indexMatrix, const ND_INFO &ndinfo, UtilBuffer<double> &Tbuf)
{
	const int nVertices = (1<<ndinfo.nDimension);

	/* Get the indices of the hypercube containing the point in argument */
	if (getHyperCube(Xmat, indexMatrix, xPar, ndinfo) == false)
	{
		//ErrMsg("Point lies out data grid (in getHyperCube)");
	}

	/* Get the co-ordinates of the hyper cube */
	for(int i=0; i<ndinfo.nDimension; i++)
	{
		int low  = indexMatrix.m_mat[i][0];
		int high = indexMatrix.m_mat[i][1];
		xPoint.m_mat[i][0] = Xmat[i][low];
		xPoint.m_mat[i][1] = Xmat[i][high];
	}

	for(int i=0; i<nVertices; i++)
	{
		for(int j=0; j<ndinfo.nDimension; j++)
		{
			int mask = 1<<j;
			int val = (mask & i) >> j;
			indexVector.m_vec[j] = indexMatrix.m_mat[j][val];
		}

		int index = getLinIndex(indexVector, ndinfo);
		Tbuf.m_vec[i] = Y[index];
	}

	double result = linearInterpolate(Tbuf, xPar, xPoint, indexVector, ndinfo);

	return(result);
}
} // namespace
// End of Utility Functions
