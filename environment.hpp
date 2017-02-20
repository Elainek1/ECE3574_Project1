#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <unordered_map>
#include <string>
//#include "expression.hpp"

class Environment
{
public:
	
	Environment();
	double addDoubleSymbol(std::string symbol, double value);
	bool addBoolSymbol(std::string symbol, bool value);
	int symbolExist(std::string symbol); //returns 0 if doesn't exist, 1 for symbol is bool, 2 for symbol is double
	double getDoubleSymbol(std::string symbol);
	bool getBoolSymbol(std::string symbol);
	//bool addBoolSymbol(std::string symbol, bool value);
private:
	std::unordered_map<std::string, double> environmentMap2;
	std::unordered_map<std::string, bool> environmentMap1;
};
#endif