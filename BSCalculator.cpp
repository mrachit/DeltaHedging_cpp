/*
Author : Rachit Maheshwari
Date: Oct 16, 2017
BSCalculator  provides with functions to calculate the call & put option price based on Black and Scholes formula
This file implements all the functions as defined in the BSCalculator header file.
*/
#include "stdafx.h"
#include "BSCalculator.h"
#include "StdNormalCDF.h"
#include <math.h>

/*BSCalculator constructor with Strike, Spot, Volatility, timetoMaturity,Divyiled, option type as inputs. Also computes and stores the values for d1 & d2*/
BSCalculator::BSCalculator(double Spot, double strike, double intrate, double volatility, double timematurity, double divyield, char call_put_flag)
{
	S0 = Spot;
	K = strike;
	r = intrate;
	q = divyield;
	sigma = volatility;
	T = timematurity;
	option_flag = call_put_flag;
	calc_d1_d2();
}
/*BSCalculator constructor with Strike, Spot, Volatility, timetoMaturity, option type as inputs. 
DivYield is taken as zero if not assigned
Also computes and stores the values for d1 & d2*/
BSCalculator::BSCalculator(double Spot, double strike, double intrate, double volatility, double timematurity, char call_put_flag)
{
	S0 = Spot;
	K = strike;
	r = intrate;
	q = 0.0;
	sigma = volatility;
	T = timematurity;
	option_flag = call_put_flag;
	calc_d1_d2();
}
/*Calculates d1 and d2 in Black Scholes Merton Option Pricing Formula*/
void BSCalculator::calc_d1_d2()
{
	d1 = ((log(S0 / (K*1.0)) + (r - q + 0.5*pow(sigma*1.0, 2))*T) / (sigma*sqrt(T)));
	d2 = d1 - sigma*sqrt(T);
}

/*Returns the Black Scholes implied option delta based on option Type. Default is Call Option*/
double BSCalculator::option_delta()
{
	calc_d1_d2();
	StdNormalCDF norm_CDF;

	if (option_flag == 'P') //put option
	{
		return (exp(-q*T)*(norm_CDF(d1) - 1));
	}
	else {//call option
		return (exp(-q*T)*norm_CDF(d1));
	}

	/*if else can be extended  to add any other exotic options in future.  */
}
/*Returns the Black Scholes implied option Vega*/
double BSCalculator::option_vega()
{
	calc_d1_d2();
	StdNormalCDF norm_CDF;
	return S0*sqrt(T)*norm_CDF(d1);

}

/*Returns the Black Scholes implied option Price based on option Type. Default is Call Option*/
double BSCalculator::option_price()
{
	calc_d1_d2();
	StdNormalCDF norm_CDF;
	if (option_flag == 'P') //put option
	{
		return (-S0*exp(-q*T)*norm_CDF(-d1) + K*exp(-r*T)*norm_CDF(-d2));
	}
	else {//call option

		return (S0*exp(-q*T)*norm_CDF(d1) - K*exp(-r*T)*norm_CDF(d2));
	}

	/*if else can be extended  to add any other exotic options in future.  */
}

/*Getter Setter Methods for Private & Protected variables of BS Calculator*/
double BSCalculator::getSpot() {
	return S0;
}
double BSCalculator::getStrike()
{
	return K;
}
double BSCalculator::getMaturity() {
	return T;
}
double BSCalculator::getVolatility() {
	return sigma;
}
double BSCalculator::getIntrate() {
	return r;
}
double BSCalculator::getDivyield() {
	return q;
}
char BSCalculator::getOptionType() {
	return option_flag;
}

void BSCalculator::setOptionType(char flag){
	option_flag = flag;
}

void BSCalculator::setSpot(double spot) {
	S0 = spot;
}
void BSCalculator::setStrike(double strike) {
	K = strike;
}
void BSCalculator::setVolatility(double volatility) {
	sigma = volatility;

}
void BSCalculator::setMaturity(double maturity) {
	T = maturity;

}
void BSCalculator::setIntrate(double intrate) {
	r = intrate;
}

void BSCalculator::setDivyield(double divyield) {
	q = divyield;
}