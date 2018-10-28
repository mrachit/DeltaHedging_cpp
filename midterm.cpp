//Author: Rachit maheshwari Date:Oct 17,2017 
//midterm.cpp Main File for midterm project. 
//
#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ql/quantlib.hpp>
#include "StockPriceSimulator.h"
#include "BSCalculator.h"
#include "Option.h"
#include "MyDateFunctions.h"
#include "MyCumHedgeError.h"
#include "test_all.h"

using namespace std;
map<QuantLib::Date, double> read_csv(string s);
map<QuantLib::Date, map<char, map<int, map<QuantLib::Date, double>>>> read_option_data(string s);

int main()
{
	test_all();// Can be used for testing the functionalities

	//    part 1
	double S0 = 100;
	double T = 0.4;
	int N = 100;
	double r = 0.025;
	double sigma = 0.24;
	double Mu = 0.05;
	double K = 105.0;
	double deltaT = (T*1.0) / N;
	double updated_maturity = T;
	double q = 0;
	//Storing all information in vectors (for analytical pursposes and testing)
	vector<double> option_prices;
	vector<double> option_delta;
	vector<double> option_cum_hedging_error;
	vector<double> B;

	BSCalculator bs(S0, K, r, sigma, T, q, 'C');

	StockPriceSimulator Sim(S0, T, N, Mu, sigma);

	vector<double> sim_prices = Sim.getSimulatedPrices();

	//checking if can create a output file to write the simulation
	ofstream outfile1("hedgingError_simulation_part1.csv");
	if (!outfile1.good()) {
		std::cout << "Failed to create Problem 1 file" << endl;
		return -1;
	}

	option_prices.push_back(bs.option_price());
	option_delta.push_back(bs.option_delta());
	option_cum_hedging_error.push_back(0);
	B.push_back(option_prices[0] - option_delta[0] * S0);
	outfile1 << "Period" << "," << "Spot" << "," << "Option Price" << "," << "Option delta" << "," << "Cumulative Hedging Error" << endl;
	outfile1 << "0" << "," << S0 << "," << option_prices[0] << "," << option_delta[0] << "," << "0.0" << endl;

	for (int i = 1; i <= N; i++)
	{
		updated_maturity = updated_maturity - deltaT;
		if (updated_maturity < 0) {
			updated_maturity = 0;
		}
		bs.setMaturity(updated_maturity);
		bs.setSpot(sim_prices[i]);

		option_prices.push_back(bs.option_price());
		option_delta.push_back(bs.option_delta());
		B.push_back((option_delta[i - 1] * sim_prices[i]) + (B[i - 1] * exp(r*deltaT)) - (option_delta[i] * sim_prices[i]));
		option_cum_hedging_error.push_back(option_delta[i - 1] * sim_prices[i] + (B[i - 1] * exp(r*deltaT)) - option_prices[i]);
		outfile1 << i << "," << sim_prices[i] << "," << option_prices[i] << "," << option_delta[i] << "," << option_cum_hedging_error[i] << endl;//
	}
	outfile1.close();

	//Q2

	//Variable initialization
	string input_ti, input_tn, input_T, input_K;
	double  Strike_2, timeToMaturity_2, impvol, B_2, B_prev, Delta_2, Delta_prev,
		Int_rate_2, Intrate_prev, initialVolassumption, Spot_2, V_2, V_2_initial, HE;
	double deltaT_2 = 1.0 / 252;
	char input_cp_flag, redo_flag;
	int int_input_K;
	double tolerance = 0.0000001, ub = 10.0, lb = 0.001;
	int maxIter = 1000;
	//Input Data Stored in Maps for easy access
	map<QuantLib::Date, double> InterestRate_map, SpotPrice_map;
	map<QuantLib::Date, map<char, map<int, map<QuantLib::Date, double>>>> option_data_map;
	QuantLib::Calendar usCal = QuantLib::UnitedStates();

	//	Read Interest rate & stock Price csv

	try {
		InterestRate_map = read_csv("./data_midterm/data/interest.csv");
	}
	catch (exception e) {
		std::cout << "Error Reading Interest rate file ";
		return -1;
	}

	try {
		SpotPrice_map = read_csv("./data_midterm/data/sec_GOOG.csv");
	}
	catch (exception e) {
		std::cout << "Error Reading Securities file ";
		return -1;
	}


	// Read Option Data
	try {
		option_data_map = read_option_data("./data_midterm/data/op_GOOG.csv");
	}
	catch (exception e) {
		std::cout << "Error Reading Option Data file ";
		return -1;
	}



	//take input
	do {


		std::cout << "Enter the Start Date (ti) in yyyy-mm-dd format" << std::endl;
		std::cin >> input_ti;
		while (!cin.eof() && cin.good()) {
			if (is_date(input_ti))// if input is ciorrect format do not loop else keep asking for correct input format.
			{
				break;
			}
			std::cout << "Incorrect date Format. Enter the Start Date (ti) in Valid yyyy-mm-dd format. You can enter ctrl +z/c for exit" << endl;
			std::cin >> input_ti;
		}
		if (std::cin.eof() && !std::cin.good()) {//exit if ctrl+C or ctrl +z
			return 0;
		}
		//storing the input date ti in Quantlid::Date format
		QuantLib::Date start_date(ConvertStrtoDate(input_ti));

		//taking tn input
		std::cout << "Enter the End Date (tn) in yyyy-mm-dd format" << endl;
		std::cin >> input_tn;
		// if input is correct format or system interrupt do not loop else keep asking for correct input format.
		while (!std::cin.eof() && std::cin.good()) {
			if (is_date(input_tn))
			{
				break;
			}
			std::cout << "Incorrect date Format. Enter the End Date (tn) in Valid yyyy-mm-dd format. You can enter ctrl +z/c for exit" << endl;
			std::cin >> input_tn;
		}
		if (std::cin.eof() && !std::cin.good()) {//exit if ctrl+C or ctrl +z
			return 0;
		}
		QuantLib::Date end_date(ConvertStrtoDate(input_tn));


		//taking Option expiration date
		std::cout << "Enter the Expiration Date (T) in yyyy-mm-dd format" << std::endl;
		std::cin >> input_T;
		// if input is correct format or system interrupt do not loop else keep asking for correct input format.
		while (!cin.eof() && cin.good()) {
			if (is_date(input_T))
			{
				break;
			}
			std::cout << "Incorrect date Format.Enter the Expiration Date (T) in yyyy-mm-dd format. You can enter ctrl +z/c for exit" << endl;
			std::cin >> input_T;
		}
		if (std::cin.eof() && !std::cin.good()) {//exit if ctrl+C or ctrl +z
			return 0;
		}

		QuantLib::Date expiration_date(ConvertStrtoDate(input_T));


		//Take Option_type
		std::cout << "Enter the Option Type. C for call P for put" << endl;
		cin >> input_cp_flag;
		// if input is correct format or system interrupt do not loop else keep asking for correct input format.
		while (!cin.eof() && cin.good()) {
			if ((input_cp_flag == 'C') || (input_cp_flag == 'P'))
			{
				break;
			}
			std::cout << "Incorrect Option Type format. Enter the Option Type.C for call P for put. You can enter ctrl +z/c for exit" << endl;
			cin >> input_cp_flag;
		}
		if (cin.eof() && !cin.good()) {//exit if ctrl+C or ctrl +z
			return 0;
		}


		/*STrike Price Input*/
		std::cout << "Enter the Strike Price(T) as double" << std::endl;
		std::cin >> input_K;
		// if input is correct format or system interrupt do not loop else keep asking for correct input format.
		while (!cin.eof() && cin.good()) {
			try {
				int_input_K = (int)((stod(input_K) + 0.00005) * 10000);// Note Multiplying by 10000 to store it as int i.e double with 4 decimal precision
				break;
			}
			catch (exception e)
			{
				std::cout << "Incorrect Strike Price (double) format. Enter the Strike Price(T) as double.You can enter ctrl +z/c for exit" << endl;
			}
		}
		if (cin.eof() && !cin.good()) {//exit if ctrl+C or ctrl +z
			return 0;
		}

		// Update start date to next business day if not a business day.
		while (!usCal.isBusinessDay(start_date)) {
			start_date = start_date + 1;
		}

		//check if data is available or not. It is available then start analysing else print data not available
		if (option_data_map[expiration_date][input_cp_flag][int_input_K].find(start_date) != option_data_map[expiration_date][input_cp_flag][int_input_K].end()) {
			QuantLib::Date updated_date = start_date;
			ofstream outfile2("results.csv");
			outfile2 << "Date" << "," << "Option Expiration date" << "," << "Option Type" << "," << "Security Spot Price" << "," << "Option Mkt Price" << "," << "Delta" << "," << "Implied Volatility" << "," << "Cum Hedging Error" << "," << "PNL" << std::endl;

			//do analysis only when updated_date less than expiration date and endDate
			while ((updated_date <= end_date) && (updated_date <= expiration_date))
			{
				//check availability of updated date
				if ((option_data_map[expiration_date][input_cp_flag][int_input_K].find(updated_date) != option_data_map[expiration_date][input_cp_flag][int_input_K].end())) {

					timeToMaturity_2 = usCal.businessDaysBetween(updated_date, expiration_date)*1.0 / 252.0;

					initialVolassumption = 0.1;
					Spot_2 = (double)SpotPrice_map[updated_date];
					Int_rate_2 = (double)InterestRate_map[updated_date] / 100.0;// Int Rate in %
					V_2 = option_data_map[expiration_date][input_cp_flag][int_input_K][updated_date];

					Strike_2 = double(int_input_K*1.0 / 10000);//Strike is multiplied by 10000 in int_input_K so diving it back

															   //calling an Option Object which will help in finding implied Vol and Delta. Initial Vol Assumption can be anything.
					Option option1(input_cp_flag, Spot_2, Strike_2, timeToMaturity_2, initialVolassumption, Int_rate_2);
					impvol = option1.getImpliedVolatility(V_2, lb, ub, tolerance, maxIter);
					option1.setVolatility(impvol);
					Delta_2 = option1.BS_option_delta();

					//Analysis for Calculating the Cumulative Hedging error HE				
					if (updated_date == start_date) {
						B_2 = V_2 - Delta_2*Spot_2;
						HE = 0.0;
						V_2_initial = V_2;
					}
					else {
						B_2 = Delta_prev*Spot_2 + B_prev* exp(Intrate_prev*deltaT_2) - Delta_2*Spot_2;
						HE = getCumHedgeError(Spot_2, Delta_prev, B_prev, Intrate_prev, deltaT_2, V_2);
					}


					B_prev = B_2;
					Intrate_prev = Int_rate_2;
					Delta_prev = Delta_2;
				}
				//Writing into a file
				outfile2 << ConvertDatetoStr(updated_date) << "," << ConvertDatetoStr(expiration_date) << "," << input_cp_flag << "," << Spot_2 << "," << V_2 << "," << Delta_2 << "," << impvol << "," << HE << "," << V_2 - V_2_initial << std::endl;
				updated_date = updated_date + 1;
			}
			outfile2.close();
		}
		else {
			std::cout << "Data not available for input parameters" << endl;
		}
		std::cout << "Output written to 'results.csv'.DO you want to reperform the analysis?Press y for yes, else code will exit here" << std::endl;
		std::cin >> redo_flag;
	} while (redo_flag == 'y');
	return 0;
}


//Reads the Option Data and stores the data in a one dimensional map for easy access. 
map<QuantLib::Date, double> read_csv(string s) {
	QuantLib::Date d;
	map<QuantLib::Date, double> data_map;
	string line;

	ifstream infile(s.c_str());
	//check file opened or not
	if (!infile.good()) {
		std::cout << "Failed to open" << s << " file" << endl;
	}

	// Read file and start storing data into vectors when we first see the data of format 'yyyy-mm-dd' format
	while (infile.good()) {
		getline(infile, line);
		if (is_date(line.substr(0, line.find(',')))) {
			if (line.length() == 0) {
				break;
			}
			else {
				d = ConvertStrtoDate(line.substr(0, line.find(',')));
				data_map[d] = stod(line.substr(line.find(',') + 1));
			}
		}
	}
	return data_map;
}


//Reads the Option Data and stores the data in a four dimensional map for easy access. 
map<QuantLib::Date, map<char, map<int, map<QuantLib::Date, double>>>> read_option_data(string s) {
	QuantLib::Date ti, T;
	double  OptionMktPrice, bid, offer;
	int K;
	char cp_flag;

	map<QuantLib::Date, map<char, map<int, map<QuantLib::Date, double>>>> data_map;
	string line;

	ifstream infile(s.c_str());
	//check file opened or not
	if (!infile.good()) {
		std::cout << "Failed to open" << s << " file" << endl;
	}

	// Read file and start storing data into vectors when we first see the data of format 'yyyy-mm-dd' format
	while (infile.good()) {
		getline(infile, line);
		if (is_date(line.substr(0, line.find(',')))) {
			if (line.length() == 0) {
				break;
			}
			else {
				ti = ConvertStrtoDate(line.substr(0, line.find(',')));
				line.erase(0, line.find(',') + 1);
				T = ConvertStrtoDate(line.substr(0, line.find(',')));
				line.erase(0, line.find(',') + 1);
				cp_flag = line[0];
				line.erase(0, line.find(',') + 1);
				K = (int)((stod(line.substr(0, line.find(',') + 1)) + 0.00005) * 10000);
				line.erase(0, line.find(',') + 1);
				bid = stod(line.substr(0, line.find(',') + 1));
				line.erase(0, line.find(',') + 1);
				offer = stod(line);
				OptionMktPrice = (bid + offer) / 2;
				data_map[T][cp_flag][K][ti] = OptionMktPrice;
			}
		}
	}
	return data_map;
}