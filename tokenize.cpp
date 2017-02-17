#include "tokenize.hpp"

void tokenizeInput(std::string input)
{
		/*// inputs
		std::string str("abc:def");
		char split_char1 = ':';

		// work
		std::istringstream split(str);
		std::vector<std::string> tokens;
		for (std::string each; std::getline(split, each, split_char); tokens.push_back(each));

		// now use `tokens`*/
	std::istringstream inputString(input);

	std::string token;
	std::vector<std::string> tokens;
	inputString >> token;
	while (!inputString.fail())
	{
		//std::getline(inputString, token, " ");
		std::string variable;
		for (int i = 0; i < token.size(); i++)
		{
			if (token[i] == '(')
			{
				if (variable.size())
				{
					tokens.push_back(variable);
					variable.clear();
				}
				tokens.push_back("(");
			}
			else if (token[i] == ')')
			{
				if (variable.size())
				{
					tokens.push_back(variable);
					variable.clear();
				}
				tokens.push_back(")");
			}
			else
			{
				variable.push_back(token[i]);
				if (i == (token.size() - 1))
				{
					tokens.push_back(variable);
				}
			}
			
		}
		//tokens.push_back(token);
		//tokens.push_back("(");
		inputString >> token;
	}
	for (int i = 0; i < tokens.size(); i++)
	{
		std::cout << tokens[i] << "\n";
	}
}