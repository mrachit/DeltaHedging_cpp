#pragma once
#ifndef MYCUMHEDGEERROR_H
#define MYCUMHEDGEERROR_H
#include <math.h>

double getCumHedgeError(double Spot, double Delta_prev, double B_prev, double intrate_prev, double deltaT, double Value) {
	return (Delta_prev*Spot + B_prev* exp(intrate_prev*deltaT) - Value);
}

#endif