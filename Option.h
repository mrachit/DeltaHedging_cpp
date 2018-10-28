#pragma once
/*
Author: Rachit maheshwari
Date: Oct 18,2017
Option Class is implemented in a way that it can call various financial pricing calculators to
solve for various option parameters. Currently it only provides methods for calculating Black Scholes implied volatility given
the MarketPrice of Option.
It is a derived Class from PayOff Class and inherits the PayOff function.
*/

#ifndef OPTION_H
#define OPTION_H
#include "PayOff.h"


class Option:public PayOff
{
public:
	Option(char option_flag, double Spot,double Strike, double TimeToMaturity,double Vol,double intrate);
	Option(char option_flag, double spot, double strike, double TimeToMaturity, double Vol, double intrate, double divyield);

	virtual ~Option();
	double BS_option_vega();
	double BS_option_delta();
	double BS_option_price();
	double getImpliedVolatility(double RealMarketPrice, double lb, double ub,double tolerance, int maxiterations);
	double getMaturity() { return Maturity; }
	void setMaturity(double val) { Maturity = val; }
	double getVolatility() { return Volatility; }
	void setVolatility(double val) { Volatility = val; }
	double getRiskfreerate() { return Riskfreerate; }
	void setRiskfreerate(double val) { Riskfreerate = val; }


protected:
	double Maturity;
	double Volatility;
	double Riskfreerate;
	double RealPrice;
	double DivYield;
	double ImpliedVolatility;

private:



};

#endif // OPTION_H

