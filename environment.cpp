#include "environment.hpp"
#include "interpreter.hpp"
#include <math.h>

typedef Expression(*functPtr)(Expression *);

//the environment constructor basically adds the default values
//like pi
Environment::Environment()
{
	environmentMap1.clear();
	environmentMap2.clear();
	
//	environmentMap0["+"] = add;

	environmentMap2["pi"] = atan2(0, -1);
	environmentMap2["True"] = 0;
	environmentMap2["False"] = 0;
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

//this function takes in the symbol and the double value
//then adds the mapping to the environmentMap2 (because this is the map that contains double values)
//it then returns the double value
double Environment::addDoubleSymbol(std::string symbol, double value)
{
	environmentMap2[symbol] = value;
	return value;
}

//this function takes in the symbol and the bool value
//then adds the mapping to the environmentMap1 (because this is the map that contains bool values)
//it then returns the bool value
bool Environment::addBoolSymbol(std::string symbol, bool value)
{
	environmentMap1[symbol] = value;
	return value;
}

//this function takes in the string of symbol
//and sees if it exists in the mapping
//the value return indicates which map is it located in or 0 means it is not in any mapping
int Environment::symbolExist(std::string symbol)
{
	if (environmentMap1.find(symbol) != environmentMap1.end()) //if it is in the first map then return 1
	{
		return 1;
	}
	else if (environmentMap2.find(symbol) != environmentMap2.end()) //if it is in the second map then return 2
	{
		return 2;
	}
	else if (environmentMap0.find(symbol) != environmentMap0.end()) //this is for later when I implement function pointers
	{
		return 3;
	}
	else
	{
		return 0; //otherwise is not in any maps
	}
}

//this resets the environment to the beginning
void Environment::reset()
{
	environmentMap1.clear();
	environmentMap2.clear();

	environmentMap2["pi"] = atan2(0, -1);
	environmentMap2["True"] = 0;
	environmentMap2["False"] = 0;
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

//get the value for function ptr
functPtr Environment::getFunctPtr(std::string symbol)
{
	return environmentMap0[symbol];
}

//this returns the double symbol value
double Environment::getDoubleSymbol(std::string symbol)
{
	return environmentMap2[symbol];
}

//this returns the bool symbol value
bool Environment::getBoolSymbol(std::string symbol)
{
	return environmentMap1[symbol];
}
