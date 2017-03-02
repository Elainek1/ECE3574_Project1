#include "tokenize.hpp"

//std::vector<std::string> tokenizeInput(std::string input)
std::vector<std::string> tokenizeInput(std::istream & inputString)
{
	//std::istringstream inputString(input);  //converts string into a string stream

	std::string token; //place holder for each token
	std::vector<std::string> tokens; //vector containing all tokens
	int parenthesisCounter= 0;
	inputString >> token; //read in first part seperated by white space
	while (!inputString.fail())
	{
		
		std::string variable; //this is for multicharacter tokens
		for (int i = 0; i < token.size(); i++) //iterate through each token to parse out the ( and )
		{
			if (token[i] == '(') //push ( into vector
			{
				parenthesisCounter++;
				if (variable.size() > 0)  //if variable before ( push that into vector first
				{
					tokens.push_back(variable);
					variable.clear();
				}
				tokens.push_back("(");
			}
			else if (token[i] == ')') //push ) into vector
			{
				parenthesisCounter--;
				if (variable.size() > 0) //if variable before ) push that into vector first
				{
					tokens.push_back(variable);
					variable.clear();
				}
				tokens.push_back(")");
			}
			else //otherwise
			{
				variable.push_back(token[i]); //append each character to variable
				if (i == (token.size() - 1))  //push variable at the end of token length
				{
					tokens.push_back(variable);
				}
			}
			
		}
		inputString >> token;  //read in next part
	}
	/*for (int i = 0; i < tokens.size(); i++) //print the token
	{
		std::cout << tokens[i] << "\n";
	}*/
	if (parenthesisCounter != 0)
	{
		tokens.clear();
		//return tokens;
	}
	return tokens;
}