/*
Author: Rachit Maheshwari
Date: Oct 6, 2017
Implements all the functions defined in the StdNormalCDF header file.
STDNORMALCDF implements normal cdf function. 
*/
#include "stdafx.h"
#include "StdNormalCDF.h"
#include <math.h>

StdNormalCDF::StdNormalCDF()
{
	//ctor
}

StdNormalCDF::~StdNormalCDF()
{
	//dtor
}

const double StdNormalCDF::A1 = 0.319381530;
const double StdNormalCDF::A2 = -0.356563782;
const double StdNormalCDF::A3 = 1.781477937;
const double StdNormalCDF::A4 = -1.821255978;
const double StdNormalCDF::A5 = 1.330274429;
const double StdNormalCDF::B = 0.2316419;
const double StdNormalCDF::NORMALIZER = 0.3989422804;


/*Returns CDF for any double value following Standard normal Normal Distribution */
double StdNormalCDF::operator()(double x) const {
	if (x<0) {
		return (1 - (*this)(-x));
	}
	double z = 1.0 / (1.0 + B*x);
	double Rz = z*(A1 + z*(A2 + z*(A3 + z*(A4 + z*A5))));
	double N = 1 - NORMALIZER*Rz*exp(-0.5*pow(x, 2));
	return N;
}
