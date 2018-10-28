//Author: Rachit Maheshwari
//Date: Sep 20, 2017
//Payoff.cpp file for PayOff class implements all the functions defined for Payoff class.
#include "stdafx.h"
#include "PayOff.h"
#include<algorithm>// required for exponential & fmax

/*Constructor methods for PayOff with Strike, Spot , power and Option Type*/
PayOff::PayOff(double s, double X)
{
	Spot = s;
	Strike = X;
	power = 1;
	cp_flag = 'C';
}


PayOff::PayOff(char option_flag, double s, double X)
{
	Spot = s;
	Strike = X;
	power = 1;
	cp_flag = option_flag;
}


PayOff::PayOff(char option_flag, double s, double X, int p )
{
	Spot = s;
	Strike = X;
	power = p;
	cp_flag = option_flag;
}


PayOff::PayOff(){}

// Call & Put Power PayOff functions. Default is Call Payoff .
double PayOff::OptionPayoff() {
	// setPayoff( fmax((pow(spot,power)-strike),0.0));
	if (cp_flag == 'P') {
		return (fmax(Strike - (pow(Spot, power)), 0.0));
	}
	else {
		return (fmax((pow(Spot, power) - Strike), 0.0));
	}
}


/*Getter & Setter Methods for Private and Protected Members*/
double PayOff::getStrike()
{
	return Strike;
}


double PayOff::getSpot()
{
	return Spot;
}


int PayOff::getPower() {
	return power;
}

char PayOff::getOptionType() {
	return cp_flag;
}


//creating setter methods
void PayOff::setStrike(double X) {
	Strike = X;
}


void PayOff::setSpot(double s) {
	Spot = s;
}


void PayOff::setPower(int p) {
	power = p;
}


void PayOff::setOptionType(char flag) {
	cp_flag = flag;
}


