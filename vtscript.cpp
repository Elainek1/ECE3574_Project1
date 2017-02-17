#include "interpreter.hpp"
#include "environment.hpp"
#include "expression.hpp"
#include "tokenize.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
using namespace std;

int main(int argc, char*argv[])
{
	Interpreter interpreter;
	string filename;  //for storing the filename grabbed from commandline
	string input;
	std::vector<std::string>  arguments;
    for(int i = 0; i < argc; ++i)
	{
		arguments.push_back(argv[i]);
	}
    // code can use arguments as a C++ vector of C++ strings 
	
	//if there are more than one arguments grab the last one and use that for the filename
	if(argc == 1)
	{
		//std::vector<std::string> tokens;
		while (1)
		{
			cout << "vtscript>";
			//cout << "(begin\n\t(define b pi)\n\t(if (< a b) b a)\n\t)" << endl;
			//input = "(begin\n\t(define b pi)\n\t(if (< a b) b a)\n\t)";
			getline(cin, input);
			std::istringstream inputString(input);
			bool parsePass = interpreter.parse(inputString);
			if (!parsePass)
			{
				cout << "Error: Parsing error" << endl;
				return EXIT_FAILURE;
			}
		}
	}
	else if(argc == 2)
	{
		filename = arguments[argc-1];
		ifstream inputString(filename);
		if (!inputString)
		{
			cout << "Error: Could not open file" << endl;
			return EXIT_FAILURE; //if file doesnt open then return
		}
		bool parsePass = interpreter.parse(inputString);
		if (!parsePass)
		{
			cout << "Error: Parsing error" << endl;
			return EXIT_FAILURE;
		}
	}
	else if(argc == 3)
	{
		if(arguments[argc-2]=="-e")
		{
			input = arguments[argc-1];
			std::istringstream inputString(input);
			bool parsePass = interpreter.parse(inputString);
			if (!parsePass)
			{
				cout << "Error: Parsing error" << endl;
				return EXIT_FAILURE;
			}
		}
		else
		{
			cout << "Error: Unexpected arguments of " << arguments[argc-2] << endl; //not appropriate -e flag specified so return with exit failure
			return EXIT_FAILURE;
		}
	}
	else
	{
		cout << "Error: Unexpected number of arguments" << endl; //no filename specified so return with exit failure
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;  //return success
}
