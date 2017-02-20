#include "environment.hpp"
#include <math.h>  

Environment::Environment()
{
	environmentMap1.clear();
	environmentMap2.clear();
	environmentMap2["pi"] = atan2(0, -1);
	environmentMap2["not"] = 0;
	environmentMap2["and"] = 0;
	environmentMap2["or"] = 0;
	environmentMap2["<"] = 0;
	environmentMap2["<="] = 0;
	environmentMap2[">"] = 0;
	environmentMap2[">="] = 0;
	environmentMap2["="] = 0;
	environmentMap2["+"] = 0;
	environmentMap2["-"] = 0;
	environmentMap2["*"] = 0;
	environmentMap2["/"] = 0;
	environmentMap2["define"] = 0;
	environmentMap2["begin"] = 0;
	environmentMap2["if"] = 0;
}

double Environment::addDoubleSymbol(std::string symbol, double value)
{
	environmentMap2[symbol] = value;
	return value;
}
bool Environment::addBoolSymbol(std::string symbol, bool value)
{
	environmentMap1[symbol] = value;
	return value;
}
int Environment::symbolExist(std::string symbol)
{
	if (environmentMap1.find(symbol) != environmentMap1.end())
	{
		return 1;
	}
	else if (environmentMap2.find(symbol) != environmentMap2.end())
	{
		return 2;
	}
	else
	{
		return 0;
	}
}
double Environment::getDoubleSymbol(std::string symbol)
{
	return environmentMap2[symbol];
}
bool Environment::getBoolSymbol(std::string symbol)
{
	return environmentMap1[symbol];
}
