#include "interpreter.hpp"
#include "tokenize.hpp"
#include "interpreter_semantic_error.hpp"
//#include "environment.hpp"
#include <string>


Interpreter::Interpreter()
{
	//environment = Environment();
	rootAST = nullptr;
}

Interpreter::~Interpreter()
{

	deleteAST(rootAST);
	rootAST = nullptr;

}

void Interpreter::reset()
{
	deleteAST(rootAST);
	rootAST = nullptr;
	environment.reset();
}

// Given a vtscript program as a std::istream, attempt to parse into an internal AST
// return true on success, false on failure
bool Interpreter::parse(std::istream & expression) noexcept
{
	std::vector<std::string> tokens;
	//std::istringstream inputString(input);
	std::string inputAll = "";
	std::string inputPart = "";
	while (!expression.eof())
	{
		getline(expression, inputPart);
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
	std::istringstream inputStr(inputAll);
	tokens = tokenizeInput(inputStr);
	/*
	for (int i = 0; i < tokens.size(); i++) //print the token
	{
		std::cout << tokens[i] << '\n';
	}
	*/
	if (tokens.size()>1)
	{
		
		if (!buildAST(tokens))
		{
			return false;
		}
/*
		try
		{
			Expression output = eval();		
		
			if (output.atomType == 0)
			{
				//deleteAST(rootAST);
				environment = Environment();
				//return false;
			}
			else
			{
				printExpression(output);
			}
		}
		catch (InterpreterSemanticError error)
		{
			environment = Environment();
		}
		deleteAST(rootAST);
		rootAST = nullptr;
		*/
		/*
		if (output.atomType == 0)
		{
			environment = Environment();
			//return false;
		}
		else
		{
			printExpression(output);
		}
		*/
		return true;
	}
	return false;
}

// Evaluate the current AST and return the resulting Expression
// throws InterpreterSemanticError if a semantic error is encountered
// the exception message string should document the nature of the semantic error 
Expression Interpreter::eval()
{
	try
	{
		Expression answer = evalExp(rootAST);
		deleteAST(rootAST);
		rootAST = nullptr;
		return answer;
	}
	catch (InterpreterSemanticError error)
	{
		//std::cout << error.what() << "\n";
		deleteAST(rootAST);
		rootAST = nullptr;
		throw;
	}
	
}
Expression Interpreter::evalExp(Expression * curLevel)
{
	Expression answer = Expression();
	//answer.doubleAtom = 0;
	try {
		if (curLevel == nullptr)
		{
			//Expression * answer = new Expression(0.0);
			//return *answer;
			//throw "Error: Nothing to evaluate";
			std::string error = "Error: Nothing to evaluate";
			throw InterpreterSemanticError(error);
			return Expression();
		}
		else
		{
			/*
			switch (curLevel->stringAtom)
			{
			case "+":
				return add(curLevel);
			case "/":
				return divide(curLevel);
			}*/
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
			else if (curLevel->atomType == Expression::noneType)
			{
				std::string error = "Error: Input is not valid";
				throw InterpreterSemanticError(error);
				//throw error;
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
					}/*
					else if (a == 3)
					{
						environment.getFunctPtr(curLevel->stringAtom)(curLevel);
					}*/
					else
					{
						std::string error = "Error: Variable not defined\n";
						throw InterpreterSemanticError(error);
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
	catch (InterpreterSemanticError error)
	{
		//std::cout << error.what() << "\n";
		throw;
		return Expression();
	}

	return answer;
}
bool Interpreter::buildAST(std::vector<std::string> tokens)
{
	Expression *currLevel;
	Expression *previousLevel;
	if ((tokens[0] == "(") && (tokens[tokens.size() - 1] == ")"))
	{
		for (unsigned int i = 0; i < tokens.size(); i++)
		{
			if (tokens[i] == "(")
			{
				if ((tokens[i + 1] == "(") || (tokens[i + 1] == ")"))
				{
					return false;
				}
				else
				{
					if (i == 0)
					{
						rootAST = checkToken(tokens[i + 1], rootAST);
						if (rootAST->atomType == 0)
						{
							return false;
						}
						currLevel = rootAST;
						previousLevel = currLevel;
						i++;
					}
					else
					{

						previousLevel = currLevel;
						currLevel = checkToken(tokens[i + 1], currLevel);
						if (currLevel->atomType == 0)
						{
							return false;
						}
						//currLevel = new Expression(tokens[i + 1]);
						currLevel->parent = previousLevel;
						currLevel->parent->children.push_back(currLevel);
						i++;
					}
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
				currLevel = checkToken(tokens[i], currLevel);
				if (currLevel->atomType == 0)
				{
					return false;
				}
				currLevel->parent = previousLevel;
				currLevel->parent->children.push_back(currLevel);
				currLevel = previousLevel;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
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

void Interpreter::deleteAST(Expression * curLevel)
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
		delete curLevel;
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

Expression * Interpreter::checkToken(std::string token, Expression* curLevel)
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
			if (parser.eof())
			{
				curLevel = new Expression(tokenVal);
			}
			else
			{
				if (!isdigit(token[0]))
				{
					curLevel = new Expression(token);
				}
				else
				{
					curLevel = new Expression();
				}
				
			}
			//std::cout << tokenVal << "\n";
			
		}
	}
	return curLevel;
}

Expression Interpreter::add(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() > 0)
		{

			Expression answer = Expression(0.0);
			for (unsigned int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
			{
				Expression child = evalExp(curLevel->children[childIndex]);
				if (child.atomType == 2)
				{
					answer.doubleAtom += (child.doubleAtom);
				}
				else
				{
					std::string error = "Error: Incorrrect argument type for +";
					throw InterpreterSemanticError(error);
					return Expression();
				}
			}
			return answer;
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for +";
			throw InterpreterSemanticError(error);
			//throw error;
			return Expression();
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::divide(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() == 2)
		{
			Expression answer = Expression(0.0);
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);

			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.doubleAtom = (child1.doubleAtom) / (child2.doubleAtom);
				return answer;
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for /";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for /";
			throw InterpreterSemanticError(error);
			return Expression();
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::multiply(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() > 0)
		{
			Expression answer = Expression(1.0);
			for (unsigned int i = 0; i < curLevel->children.size(); i++)
			{
				Expression child = evalExp(curLevel->children[i]);
				if (child.atomType == 2)
				{
					answer.doubleAtom *= child.doubleAtom;
				}
				else
				{
					std::string error = "Error: Incorrrect argument type for *";
					throw InterpreterSemanticError(error);
					return Expression();
				}
				
			}
			return answer;
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for *";
			throw InterpreterSemanticError(error);
			return Expression();
		}
	}
	catch(InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::subtract(Expression * curLevel)
{
	try
	{
		Expression answer = Expression(0.0);
		if (curLevel->children.size() == 1)
		{
			Expression child = evalExp(curLevel->children[0]);
			if (child.atomType == 2)
			{
				answer.doubleAtom -= (child.doubleAtom);
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for -";
				throw InterpreterSemanticError(error);
				return Expression();
			}
			
		}
		else if (curLevel->children.size() == 2)
		{
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);
			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.doubleAtom = (child1.doubleAtom) - (child2.doubleAtom);

			}
			else
			{
				std::string error = "Error: Incorrrect argument type for -";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error =  "Error: too many arguments for -";
			throw InterpreterSemanticError(error);
			return Expression();
		}
		return answer;
	}
	catch(InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::equal(Expression * curLevel)
{
	try
	{
		Expression answer = Expression(false);
		if (curLevel->children.size() == 2)
		{
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);
			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.boolAtom = (child1.doubleAtom) == (child2.doubleAtom);
				return answer;
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for =";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for =";
			throw InterpreterSemanticError(error);
			return Expression();
		}
		
	}
	catch(InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::greaterEqual(Expression * curLevel)
{
	try
	{
		Expression answer = Expression(false);
		if (curLevel->children.size() == 2)
		{
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);
			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.boolAtom = (child1.doubleAtom) >= (child2.doubleAtom);
				return answer;
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for >=";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for >=";
			throw InterpreterSemanticError(error);
			return Expression();
		}
		
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::greater(Expression * curLevel)
{
	try
	{
		Expression answer = Expression(false);
		if (curLevel->children.size() == 2)
		{
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);
			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.boolAtom = (child1.doubleAtom) > (child2.doubleAtom);
				return answer;
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for >";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for >";
			throw InterpreterSemanticError(error);
			return Expression();
		}

	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::lessEqual(Expression * curLevel)
{
	try
	{
		Expression answer = Expression(false);
		if (curLevel->children.size() == 2)
		{
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);
			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.boolAtom = (child1.doubleAtom) <= (child2.doubleAtom);
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for <=";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for <=";
			throw InterpreterSemanticError(error);
			return Expression();
		}
		return answer;
	}
	catch(InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::less(Expression * curLevel)
{
	try
	{
		Expression answer = Expression(false);
		if (curLevel->children.size() == 2)
		{
			Expression child1 = evalExp(curLevel->children[0]);
			Expression child2 = evalExp(curLevel->children[1]);
			if ((child1.atomType == 2) && (child2.atomType == 2))
			{
				answer.boolAtom = (child1.doubleAtom) < (child2.doubleAtom);
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for <";
				throw InterpreterSemanticError(error);
				return Expression();
			}
		}
		else
		{
			std::string error = "Error: Incorrrect number of arguments for <";
			throw InterpreterSemanticError(error);
			return Expression();
		}
		return answer;
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::orFunct(Expression * curLevel)
{
	try
	{
		
		if (curLevel->children.size() > 0)
		{
			Expression answer = Expression(false);
			for (unsigned int i = 0; i < curLevel->children.size(); i++)
			{
				Expression child = evalExp(curLevel->children[i]);
				if (child.atomType == 1)
				{
					answer.boolAtom = answer.boolAtom || child.boolAtom;
				}
				else
				{
					std::string error = "Error: Incorrrect argument type for or";
					throw InterpreterSemanticError(error);
					return Expression();
				}

			}
			return answer;
		}
		else
		{
			std::string error = "Error: Not enough arguments for or";
			throw InterpreterSemanticError(error);
			return Expression();
		}

	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::andFunct(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() > 0)
		{
			Expression answer = Expression(true);
			for (unsigned int i = 0; i < curLevel->children.size(); i++)
			{
				Expression child = evalExp(curLevel->children[i]);
				if (child.atomType == 1)
				{
					answer.boolAtom = answer.boolAtom && child.boolAtom;
				}
				else
				{
					std::string error = "Error: Incorrrect argument type for and";
					throw InterpreterSemanticError(error);
					return Expression();
				}
			}
			return answer;
		}
		else
		{
			Expression answer = Expression();
			std::string error = "Error: Not enough arguments for and";
			throw InterpreterSemanticError(error);
			return answer;
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
	
}

Expression Interpreter::notFunct(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() == 1)
		{
			Expression answer = Expression(true);
			Expression child = evalExp(curLevel->children[0]);
			if (child.atomType == 1)
			{
				answer.boolAtom = !child.boolAtom;
			}
			else
			{
				std::string error = "Error: Incorrrect argument type for not";
				throw InterpreterSemanticError(error);
				return Expression();
			}
			
			return answer;
		}
		else
		{
			Expression answer = Expression();
			std::string error = "Error: Not enough arguments for and";
			throw InterpreterSemanticError(error);
			return answer;
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::ifExpression(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() == 3)
		{
			Expression answer = evalExp(curLevel->children[0]);
			if (answer.atomType == 1)
			{
				if (answer.boolAtom)
				{
					return evalExp(curLevel->children[1]);
				}
				else
				{
					return evalExp(curLevel->children[2]);
				}
			}
			else
			{
				Expression answer = Expression();
				std::string error = "Error: Incorrect type for condition expression";
				throw InterpreterSemanticError(error);
				return answer;
			}
		}
		else
		{
			Expression answer = Expression();
			std::string error = "Error: Incorrect number of arguments for if";
			throw InterpreterSemanticError(error);
			return answer;
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::begin(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() > 0)
		{
			for (unsigned int i = 0; i < curLevel->children.size() - 1; i++)
			{
				evalExp(curLevel->children[i]);
			}
			return evalExp(curLevel->children[curLevel->children.size() - 1]);
		}
		else
		{
			Expression answer = Expression();
			std::string error = "Error: no expressions to evaluate";
			throw InterpreterSemanticError(error);
			return answer;
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}

Expression Interpreter::define(Expression * curLevel)
{
	try
	{
		if (curLevel->children.size() == 2)
		{
			if (curLevel->children[0]->atomType == 3)
			{
				if (environment.symbolExist(curLevel->children[0]->stringAtom))
				{
					Expression answer = Expression();
					std::string error = "Error: Can not overwrite variable";
					throw InterpreterSemanticError(error);
					return answer;
				}
				else
				{
					Expression answer = evalExp(curLevel->children[1]);
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
						Expression answer = Expression();
						std::string error = "Error: Can not define variable";
						throw InterpreterSemanticError(error);
						return answer;
					}


				}
			}
			else
			{
				Expression answer = Expression();
				std::string error = "Error: Invalid variable name";
				throw InterpreterSemanticError(error);
				return answer;
			}
		}
		else
		{
			Expression answer = Expression();
			std::string error = "Error: Invalid number of arguments for define";
			throw InterpreterSemanticError(error);
			return answer;
		}
	}
	catch (InterpreterSemanticError error)
	{
		throw;
	}
}