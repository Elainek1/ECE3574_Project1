#include "interpreter.hpp"
#include "environment.hpp"
#include "expression.hpp"
#include "tokenize.hpp"
#include "interpreter_semantic_error.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
using namespace std;

int main(int argc, char*argv[])
{
	Interpreter interpreter;// = Interpreter();
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
		while (true)
		{
			cout << "vtscript>";
			getline(cin, input);
			if (input == "quit")
			{
				break;
			}
			else if (input != "")
			{
				std::istringstream inputString(input);
				/*std::string inputAll = "";
				std::string inputPart = "";
				while (!inputString.eof())
				{
					getline(inputString, inputPart);
					for (int i = 0; i < inputPart.size(); i++)
					{
						if (inputPart[i] == ';')
						{
							break;
						}
						else
						{
							inputAll += inputPart[i];
						}
					}
					inputAll += '\n';

				}
				std::istringstream inputStr(inputAll);*/
				bool parsePass = interpreter.parse(inputString);
				if (!parsePass)
				{
					cout << "Error: Parsing error" << endl;
					interpreter.reset();// = Interpreter(); //create reset method instead
					//return EXIT_FAILURE;
				}
				else
				{

					try
					{
						Expression result = interpreter.eval();
						if (result.atomType == 0)
						{
							//call interpreter reset method
							interpreter.reset();
						}
						else
						{
							interpreter.printExpression(result);
						}
					}
					catch (InterpreterSemanticError error)
					{
						//call interpreter reset method
						interpreter.reset();
						std::cout << error.what() << "\n";

					}
				}
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
		/*std::string inputAll = "";
		std::string inputPart = "";
		while (!inputString.eof())
		{
			getline(inputString, inputPart);
			for (int i = 0; i < inputPart.size(); i++)
			{
				if (inputPart[i] == ';')
				{
					break;
				}
				else
				{
					inputAll += inputPart[i];
				}
			}
			inputAll += '\n';
			
		}
		std::istringstream inputStr(inputAll);*/
		bool parsePass = interpreter.parse(inputString);
		if (!parsePass)
		{
			cout << "Error: Parsing error" << endl;
			return EXIT_FAILURE;
		}
		else
		{
			try
			{
				Expression result = interpreter.eval();
				if (result.atomType == 0)
				{
					std::cout << "Error when evaluating \n";
					return EXIT_FAILURE;
				}
				else
				{
					interpreter.printExpression(result);
				}
			}
			catch (InterpreterSemanticError error)
			{
				std::cout << error.what() << "\n";
				return EXIT_FAILURE;
			}
		}
	}
	else if(argc == 3)
	{
		if(arguments[argc-2]=="-e")
		{
			input = arguments[argc-1];
			std::istringstream inputString(input);
			/*std::string inputAll = "";
			std::string inputPart = "";
			while (!inputString.eof())
			{
				getline(inputString, inputPart);
				for (int i = 0; i < inputPart.size(); i++)
				{
					if (inputPart[i] == ';')
					{
						break;
					}
					else
					{
						inputAll += inputPart[i];
					}
				}
				inputAll += '\n';

			}
			std::istringstream inputStr(inputAll);*/
			bool parsePass = interpreter.parse(inputString);
			if (!parsePass)
			{
				cout << "Error: Parsing error" << endl;
				return EXIT_FAILURE;
			}
			else
			{
				try
				{
					Expression result = interpreter.eval();
					if (result.atomType == 0)
					{
						std::cout << "Error when evaluating \n";
						return EXIT_FAILURE;
					}
					else
					{
						interpreter.printExpression(result);
					}
				}
				catch (InterpreterSemanticError error)
				{
					std::cout << error.what() << "\n";
					return EXIT_FAILURE;
				}
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
