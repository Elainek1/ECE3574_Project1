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
	string filename;  //for storing the filename grabbed from commandline
	string expression;
	std::vector<std::string>  arguments;
    for(int i = 0; i < argc; ++i)
	{
		arguments.push_back(argv[i]);
	}
    // code can use arguments as a C++ vector of C++ strings 
	
	//if there are more than one arguments grab the last one and use that for the filename
	if(argc == 1)
	{
		cout << "Interperter" << endl;
	}
	else if(argc == 2)
	{
		filename = arguments[argc-1];
		cout << filename << endl;
	}
	else if(argc == 3)
	{
		if(arguments[argc-2]=="-e")
		{
			expression = arguments[argc-1];
			cout << expression << endl;
		}
	}
	else
	{
		cout << "Error: Unexpected arguments" << endl; //no filename specified so return with exit failure
		return EXIT_FAILURE;
	}
	//opens file
	/*
	ifstream ins(filename, std::ios::binary);
	if(!ins)
	{
		cout << "Error: Could not open file" << endl;
		return EXIT_FAILURE; //if file doesnt open then return
	}
	uint8_t value;  //used to read in each character
	uint8_t lines[16];  //used to hold each line of characters to print out at the end
	ins.read(reinterpret_cast<std::fstream::char_type*>(&value), sizeof value);  //gets character

	while(!ins.fail())  //keep looping till end of file
	{
	}
	*/
    return EXIT_SUCCESS;  //return success
}
