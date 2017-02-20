#include "interpreter.hpp"
#include "tokenize.hpp"

//#include "environment.hpp"
#include <string>


Interpreter::Interpreter()
{
	environment = Environment();
	rootAST = nullptr;
}


// Given a vtscript program as a std::istream, attempt to parse into an internal AST
// return true on success, false on failure
bool Interpreter::parse(std::istream & expression) noexcept
{
	std::vector<std::string> tokens;
	
	tokens = tokenizeInput(expression);
	/*
	for (int i = 0; i < tokens.size(); i++) //print the token
	{
		std::cout << tokens[i] << '\n';
	}
	*/
	if (tokens.size()>1)
	{
		if (!(tokens[0] == "(") || !(tokens[tokens.size() - 1] == ")"))
		{
			return false;
		}
		buildAST(tokens);
		Expression output = eval();

		
		if (output.atomType == 0)
		{
			return false;
		}
		printExpression(output);
		//std::cout << printAtom(&answer) << std::endl;


		//std::cout << answer.stringAtom;
		//deleteAST(rootAST);
		return true;
	}
	return false;
}

// Evaluate the current AST and return the resulting Expression
// throws InterpreterSemanticError if a semantic error is encountered
// the exception message string should document the nature of the semantic error 
Expression Interpreter::eval()
{
	//traversePost(rootAST);
	return eval(rootAST);
}
Expression Interpreter::eval(Expression *& curLevel)
{
	Expression * answer = new Expression();
	//answer.doubleAtom = 0;
	try {
		if (curLevel == nullptr)
		{
			//Expression * answer = new Expression(0.0);
			//return *answer;
			throw "Error: Nothing to evaluate";
			return Expression();
		}
		else
		{
			if (curLevel->stringAtom == "+")
			{
				return add(curLevel);
			}
			else if (curLevel->stringAtom == "/")
			{
				return divide(curLevel);
			}
			else if (curLevel->stringAtom == "*")
			{
				return multiply(curLevel);
			}
			else if (curLevel->stringAtom == "-")
			{
				return subtract(curLevel);
			}
			else if (curLevel->stringAtom == "=")
			{
				return equal(curLevel);
			}
			else if (curLevel->stringAtom == ">=")
			{
				return greaterEqual(curLevel);
			}
			else if (curLevel->stringAtom == ">")
			{
				return greater(curLevel);
			}
			else if (curLevel->stringAtom == "<=")
			{
				return lessEqual(curLevel);
			}
			else if (curLevel->stringAtom == "<")
			{
				return less(curLevel);
			}
			else if (curLevel->stringAtom == "or")
			{
				return orFunct(curLevel);
			}
			else if (curLevel->stringAtom == "and")
			{
				return andFunct(curLevel);
			}
			else if (curLevel->stringAtom == "not")
			{
				return notFunct(curLevel);
			}
			else if (curLevel->stringAtom == "if")
			{
				return ifExpression(curLevel);
			}
			else if (curLevel->stringAtom == "begin")
			{
				return begin(curLevel);
			}
			else if (curLevel->stringAtom == "define")
			{
				return define(curLevel);
			}
			else if (curLevel->atomType == 0)
			{
				throw "Error: Input is not valid";
				return Expression();
			}
			else
			{
				if (curLevel->atomType == 3) //if it is a string
				{
					int a = environment.symbolExist(curLevel->stringAtom);
					if (a == 1)
					{
						if (curLevel->children.size() == 0)
						{
							return Expression(environment.getBoolSymbol(curLevel->stringAtom));
						}
					}
					else if (a == 2)
					{
						if (curLevel->children.size() == 0)
						{
							return Expression(environment.getDoubleSymbol(curLevel->stringAtom));
						}
					}
					else
					{
						std::cout << "Error: Variable not defined\n";
						return Expression();
					}
				}
				else
				{
					return *curLevel;
				}
				return *curLevel;
			}
		}
	}
	catch (std::string e)
	{
		std::cout << e << "\n";
		return Expression();
	}

	return *answer;
}
void Interpreter::buildAST(std::vector<std::string> tokens)
{
	Expression *currLevel;
	Expression *previousLevel;

	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "(")
		{
			if (i == 0)
			{
				//if ((tokens[i + 1]))
				checkToken(tokens[i + 1],rootAST);
				currLevel = rootAST;
				previousLevel = currLevel;
				i++;
			}
			else
			{
				previousLevel = currLevel;
				checkToken(tokens[i + 1], currLevel);
				//currLevel = new Expression(tokens[i + 1]);
				currLevel->parent = previousLevel;
				currLevel->parent->children.push_back(currLevel);
				i++;

			}
		}
		else if (tokens[i] == ")")
		{
			if (previousLevel->parent != nullptr)
			{
				previousLevel = previousLevel->parent;
			}
			currLevel = currLevel->parent;
		}
		else
		{
			previousLevel = currLevel;
			checkToken(tokens[i], currLevel);
			//currLevel = new Expression(tokens[i]);
			currLevel->parent = previousLevel;
			currLevel->parent->children.push_back(currLevel);
			currLevel = previousLevel;
		}
	}
	return;
}

void Interpreter::traversePost(Expression * curLevel)
{
	if (curLevel == nullptr)
	{
		return;
	}
	else
	{

		for (unsigned int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			traversePost(curLevel->children[childIndex]);
		}
		
		printExpression(*curLevel);
		//std::cout << curLevel->boolAtom << "," << curLevel->doubleAtom << "," << curLevel->stringAtom << std::endl;
	}
}

void Interpreter::deleteAST(Expression *& curLevel)
{
	if (curLevel == nullptr)
	{
		return;
	}
	else
	{

		for (unsigned int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			deleteAST(curLevel->children[childIndex]);
		}
		//delete curLevel;
		curLevel == nullptr;
	}
}

void Interpreter::printExpression(Expression curLevel)
{
	if (curLevel.atomType == 0)
	{
		std::cout << "()\n";
	}
	else if (curLevel.atomType == 1)
	{
		if (curLevel.boolAtom)
		{
			std::cout << "(True)\n";
		}
		else
		{
			std::cout << "(False)\n";
		}
	}
	else if (curLevel.atomType == 2)
	{
		std::cout << "(" << curLevel.doubleAtom << ")\n";
	}
	else if (curLevel.atomType == 3)
	{
		std::cout << "(" << curLevel.stringAtom << ")\n";
	}
	return;
}

Expression Interpreter::checkToken(std::string token, Expression *& curLevel)
{
	/*if (isdigit(token[0]))
	{
		double tokenVal;
		std::stringstream(token) >> tokenVal;
		std::cout << tokenVal;// std::stoi(token) << (double)std::stoi(token);
		std::cout << std::stod(token);
		curLevel = new Expression(std::stod(token));
	}*/
	if ((token == "True"))
	{
		curLevel = new Expression(true);
	}
	else if (token == "False")
	{
		curLevel = new Expression(false);
	}
	else
	{
		double tokenVal;
		std::stringstream parser;
		parser << token;
		parser >> tokenVal;
		if (parser.fail())
		{
			parser.clear();
			curLevel = new Expression(token);
		}
		else
		{
			//std::cout << tokenVal << "\n";
			curLevel = new Expression(tokenVal);
		}
	}
	return *curLevel;
}

Expression Interpreter::add(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() > 1)
		{

			Expression * answer = new Expression(0.0);
			for (unsigned int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
			{
				answer->doubleAtom += (eval(curLevel->children[childIndex]).doubleAtom);
				//std::cout << answer->doubleAtom << " ";
			}
			return *answer;
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Incorrrect number of arguments for +";
			throw error;
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::divide(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() == 2)
		{
			Expression * answer = new Expression(0.0);
			answer->doubleAtom = (eval(curLevel->children[0]).doubleAtom) / (eval(curLevel->children[1]).doubleAtom);
			return *answer;
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Incorrrect number of arguments for /";
			throw error;
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::multiply(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() > 1)
		{
			Expression * answer = new Expression(1.0);
			for (unsigned int i = 0; i < curLevel->children.size(); i++)
			{
				answer->doubleAtom *= eval(curLevel->children[i]).doubleAtom;
			}
			return *answer;
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Incorrrect number of arguments for *";
			throw error;
			return *answer;
		}
	}
	catch(std::string error)
	{
		throw;
	}
}

Expression Interpreter::subtract(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(0.0);
		if (curLevel->children.size() == 1)
		{
			answer->doubleAtom -= (eval(curLevel->children[0]).doubleAtom);
		}
		else if (curLevel->children.size() == 2)
		{
			answer->doubleAtom = (eval(curLevel->children[0]).doubleAtom) - (eval(curLevel->children[1]).doubleAtom);
		}
		else
		{
			std::string error =  "Error: too many arguments for -";
			throw error;
			return Expression();
		}
		return *answer;
	}
	catch(std::string error)
	{
		throw;
	}
}

Expression Interpreter::equal(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(false);
		if (curLevel->children.size() == 2)
		{
			answer->boolAtom = (eval(curLevel->children[0]).doubleAtom) == (eval(curLevel->children[1]).doubleAtom);
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for =";
			throw error;
		}
		return *answer;
	}
	catch(std::string error)
	{
		throw;
	}
}

Expression Interpreter::greaterEqual(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(false);
		if (curLevel->children.size() == 2)
		{
			answer->boolAtom = (eval(curLevel->children[0]).doubleAtom) >= (eval(curLevel->children[1]).doubleAtom);
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for >=";
			throw error;
		}
		return *answer;
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::greater(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(false);
		if (curLevel->children.size() == 2)
		{
			answer->boolAtom = (eval(curLevel->children[0]).doubleAtom) > (eval(curLevel->children[1]).doubleAtom);
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for >";
			throw error;
		}
		return *answer;
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::lessEqual(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(false);
		if (curLevel->children.size() == 2)
		{
			answer->boolAtom = (eval(curLevel->children[0]).doubleAtom) <= (eval(curLevel->children[1]).doubleAtom);
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for <=";
			throw error;
		}
		return *answer;
	}
	catch(std::string error)
	{
		throw;
	}
}

Expression Interpreter::less(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(false);
		if (curLevel->children.size() == 2)
		{
			answer->boolAtom = (eval(curLevel->children[0]).doubleAtom) < (eval(curLevel->children[1]).doubleAtom);
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for <";
			throw error;
		}
		return *answer;
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::orFunct(Expression *& curLevel)
{
	try
	{
		Expression * answer = new Expression(false);
		if (curLevel->children.size() > 1)
		{
			for (unsigned int i = 0; i < curLevel->children.size(); i++)
			{
				answer->boolAtom = answer->boolAtom || eval(curLevel->children[i]).boolAtom;
			}
			return *answer;
		}
		else
		{
			std::string error = "Error: Not enough arguments for or";
			throw error;
		}
		return *answer;
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::andFunct(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() > 1)
		{
			Expression * answer = new Expression(true);
			for (unsigned int i = 0; i < curLevel->children.size(); i++)
			{
				answer->boolAtom = answer->boolAtom && eval(curLevel->children[i]).boolAtom;
			}
			return *answer;
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Not enough arguments for and";
			throw error;
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
	
}

Expression Interpreter::notFunct(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() == 1)
		{
			Expression * answer = new Expression(true);
			answer->boolAtom = !eval(curLevel->children[0]).boolAtom;
			return *answer;
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Not enough arguments for and";
			throw error;
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::ifExpression(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() == 3)
		{
			Expression answer = eval(curLevel->children[0]);
			if (answer.atomType == 1)
			{
				if (answer.boolAtom)
				{
					return eval(curLevel->children[1]);
				}
				else
				{
					return eval(curLevel->children[2]);
				}
			}
			else
			{
				Expression * answer = new Expression();
				std::string error = "Error: Incorrect type for condition expression";
				throw error;
				return *answer;
			}
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Incorrect number of arguments for if";
			throw error;
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::begin(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() > 0)
		{
			for (unsigned int i = 0; i < curLevel->children.size() - 1; i++)
			{
				eval(curLevel->children[i]);
			}
			return eval(curLevel->children[curLevel->children.size() - 1]);
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: no expressions to evaluate";
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
}

Expression Interpreter::define(Expression *& curLevel)
{
	try
	{
		if (curLevel->children.size() == 2)
		{
			if (curLevel->children[0]->atomType == 3)
			{
				if (environment.symbolExist(curLevel->children[0]->stringAtom))
				{
					Expression * answer = new Expression();
					std::string error = "Error: Can not overwrite variable";
					throw error;
					return *answer;
				}
				else
				{
					Expression answer = eval(curLevel->children[1]);
					if (answer.atomType == 1)
					{
						environment.addBoolSymbol(curLevel->children[0]->stringAtom, answer.boolAtom);
						return answer;
					}
					else if (answer.atomType == 2)
					{
						environment.addDoubleSymbol(curLevel->children[0]->stringAtom, answer.doubleAtom);
						return answer;
					}
					else
					{
						Expression * answer = new Expression();
						std::string error = "Error: Can not define variable";
						throw error;
						return *answer;
					}


				}
			}
			else
			{
				Expression * answer = new Expression();
				std::string error = "Error: Invalid variable name";
				throw error;
				return *answer;
			}
		}
		else
		{
			Expression * answer = new Expression();
			std::string error = "Error: Invalid number of arguments for define";
			throw error;
			return *answer;
		}
	}
	catch (std::string error)
	{
		throw;
	}
}