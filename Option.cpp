#include "stdafx.h"
#include "Option.h"
#include "BSCalculator.h"
#include <ql/quantlib.hpp> 

/*Option constructor with Strike, Spot, Volatility, timetoMaturity,Divyiled, option type as inputs. 
Also computes and stores the values for d1 & d2*/

Option::Option(char option_flag, double spot, double strike, double TimeToMaturity, double Vol, double intrate, double divyield) {

	Spot = spot;
	Strike = strike;
	cp_flag = option_flag;
	Maturity = TimeToMaturity;
	Volatility = Vol;
	Riskfreerate = intrate;
	DivYield = divyield;
	ImpliedVolatility = Vol;
}

/*Option constructor with Strike, Spot, Volatility, timetoMaturity, option type as inputs.
Divyield is taken as 0.0 if not defined.
Also computes and stores the values for d1 & d2*/

Option::Option(char option_flag, double spot, double strike, double TimeToMaturity, double Vol,double intrate) {


	 Spot = spot;
	 Strike = strike;
	 cp_flag = option_flag;
	 Maturity= TimeToMaturity;
	 Volatility=Vol;
	 Riskfreerate=intrate;
	 DivYield = 0.0;
	 ImpliedVolatility = Vol;
}


Option::~Option()
{
	//dtor
}

/*getImplied Volatility uses bisection method to calculate the implied volatility given the options real market Price
You can decrease and increase accurray based on upper & lower bound inputs, tolerance and maxiterations.
*/

double Option::getImpliedVolatility( double RealMarketPrice,double lb, double ub, double tolerance, int maxiterations)
{
	

	double mid,diff_lb, diff_ub, diff_mid;
	
	BSCalculator bs(Spot, Strike, Riskfreerate,0.5, Maturity, DivYield, cp_flag);
	RealPrice = RealMarketPrice;
	double impvol = Volatility;
	for (int i = 0; i < maxiterations; i++)
	{
		bs.setVolatility(lb);
		diff_lb = bs.option_price() - RealPrice;


		bs.setVolatility(ub);
		diff_ub = bs.option_price() - RealPrice;


		mid = ((lb + ub) / 2.0);
		bs.setVolatility(mid);
		diff_mid = bs.option_price() - RealPrice;
		if (fabs(diff_mid) <= tolerance) {
			impvol = mid;
			break;
		}

		impvol = mid;
		if ((diff_mid*diff_lb) < 0) {
			ub = mid;
		}
		else {
			lb = mid;
		}
	}
	ImpliedVolatility = impvol;
	return impvol;
}


//BS Option Price 
double Option::BS_option_price()
{
	BSCalculator bs(Spot, Strike, Riskfreerate, ImpliedVolatility, Maturity, DivYield, cp_flag);
	return bs.option_price();
}



/* For now only greeks using Black Scholes are calculated. In Future this can be updated to include greeks using other formulae.
*/


/*Outputs Option Vega based on Black Scholes Mertorn formula*/
double Option::BS_option_vega()
{
	BSCalculator bs(Spot, Strike, Riskfreerate, Volatility, Maturity, DivYield, cp_flag);
	return bs.option_vega();

}
/*Outputs Option Delta based on Black Scholes Mertorn formula*/
double Option::BS_option_delta()
{
	BSCalculator bs(Spot, Strike, Riskfreerate, Volatility, Maturity, DivYield, cp_flag);
	return bs.option_delta();
}



