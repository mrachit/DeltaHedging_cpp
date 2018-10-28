//Author: Rachit Maheshwari
//Date: Sep 20, 2017
//Payoff.h file for Payoff class which is a  generic power option class
#pragma once

#ifndef __hw2_main__PayOff__
#define __hw2_main__PayOff__

class PayOff {
public:
	PayOff();
	PayOff(double spot, double strike);
	PayOff(char option_flag, double spot, double strike);
	PayOff(char option_flag, double spot, double strike, int power );
	double getStrike();
	double getSpot();
	int getPower();
	char getOptionType();
	void setStrike(double X);
	void setSpot(double s);
	void setPower(int p);
	void setOptionType(char flag);
	double OptionPayoff();
	//double PutPayoff();
	~PayOff() {}
protected:
	double Spot;
	double Strike;
	int power;
	char cp_flag;

private:
};

#endif
