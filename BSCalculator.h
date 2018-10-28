#pragma once
/*
Author : Rachit Maheshwari
Date: Oct 6, 2017
It is the header file BSCalculator class.
BSCalculator  provides with functions to calculate the call & put option price based on Black and Scholes formula
*/

#ifndef BSCALCULATOR_H
#define BSCALCULATOR_H

class BSCalculator
{
public:
	BSCalculator(double Spot, double strike, double intrate, double volatility, double timematurity, double divyield,char option_flag );
	BSCalculator(double Spot, double strike, double intrate, double volatility, double timematurity, char option_flag);
	virtual ~BSCalculator() {};
	double option_vega();
	double option_price();
	double option_delta();
	double getSpot();
	double getStrike();
	double getMaturity();
	double getVolatility();
	double getIntrate();
	double getDivyield();
	char getOptionType();
	void setOptionType(char flag);
	void setSpot(double);
	void setStrike(double);
	void setVolatility(double);
	void setMaturity(double);
	void setIntrate(double);
	void setDivyield(double);
	void calc_d1_d2();

protected:

	double d1;
	double d2;
private:
	double S0;
	double K;
	double r;
	double q;
	double sigma;
	double T;
	char option_flag;

};

#endif // STDNORMALCDF_H
