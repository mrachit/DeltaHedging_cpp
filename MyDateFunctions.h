#pragma once
#ifndef MYDATEFUNCTIONS_H
#define MYDATEFUNCTIONS_H

#include "stdafx.h"
#include<string.h>
#include <ql/quantlib.hpp>

bool is_date(std::string s) {
	// if any string is less than 10 characters, its not a date format of yyyy-mm-dd
	if (s.size()<10) 
	{
		return false;
	}
	int data_year;
	int data_mnth;
	std::string mnth_day_string = s.substr(s.find('-') + 1);
	try {
		sscanf(s.substr(0, s.find('-')).c_str(), "%d", &data_year);
		sscanf(mnth_day_string.substr(0, mnth_day_string.find('-')).c_str(), "%d", &data_mnth);
	}
	catch (std::exception e)
	{
		return false;
	}
	if ((data_year<2018) & (data_year>0) & (data_mnth<13) &(data_mnth>0)) {// Checks for valid month and year
		return true;
	}
	return false;
}


QuantLib::Date ConvertStrtoDate(std::string str)
{
	int date[3], j(0);
	size_t pos;
	std::string delim = "-";
	while ((pos = str.find(delim)) != std::string::npos) {
		date[j++] = atoi(str.substr(0, pos).c_str());
		str.erase(0, pos + delim.length());
	}
	date[j] = atoi(str.c_str());
	return QuantLib::Date(date[2], QuantLib::Month(date[1]), date[0]);
}

std::string ConvertDatetoStr(QuantLib::Date d)
{
	return  std::to_string(d.year()) + "-" + std::to_string(d.month()) + "-" + std::to_string(d.dayOfMonth());
}

#endif