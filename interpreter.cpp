#include "interpreter.hpp"
#include "tokenize.hpp"
#include "environment.hpp"
#include <string>


Interpreter::Interpreter()
{
	Environment environment;
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
		Expression answer = eval();
		std::cout << answer.doubleAtom << std::endl;


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
	traversePost(rootAST);
	return eval(rootAST);
}
Expression Interpreter::eval(Expression *& curLevel)
{
	Expression * answer = new Expression(0.0);
	//answer.doubleAtom = 0;
	if (curLevel == nullptr)
	{
		return *answer;
	}
	else
	{
		if (curLevel->stringAtom == "+")
		{
			for (int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
			{
				answer->doubleAtom += (eval(curLevel->children[childIndex]).doubleAtom);
				std::cout << answer->doubleAtom << " ";
			}
			return *answer;
		}
		else if((curLevel->stringAtom == "/") && (curLevel->children.size() == 2))
		{
			answer->doubleAtom = (eval(curLevel->children[0]).doubleAtom) / (eval(curLevel->children[1]).doubleAtom);
			return *answer;
		}
		else if ((curLevel->stringAtom == "*") && (curLevel->children.size() > 1))
		{
			answer->doubleAtom = 1;
			for (int i = 0; i < curLevel->children.size(); i++)
			{
				answer->doubleAtom *= eval(curLevel->children[i]).doubleAtom;
			}
			return *answer;
		}
		if (curLevel->stringAtom == "-")
		{
			if (curLevel->children.size() == 1)
			{
				answer->doubleAtom -= (eval(curLevel->children[0]).doubleAtom);
			}
			else if (curLevel->children.size() == 2)
			{
				answer->doubleAtom = (eval(curLevel->children[0]).doubleAtom) - (eval(curLevel->children[1]).doubleAtom);
				std::cout << answer->doubleAtom << " ";
			}
			else
			{
				std::cout << "Error: too many arguments for -" << std::endl;
			}
			return *answer;
		}
		else
		{
			return *curLevel;
		}
		/*for (int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			eval(curLevel->children[childIndex]);
		}
		std::cout << curLevel->stringAtom << std::endl;*/
	}

	return *answer;
}
void Interpreter::buildAST(std::vector<std::string> tokens)
{
	//Expression *rootAST;
	Expression *currLevel;
	Expression *previousLevel;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "(")
		{
			if (i == 0)
			{
				//if ((tokens[i + 1]))
				checkToken(tokens[i + 1],rootAST);
				currLevel = rootAST;
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
	//traversePost(rootAST);
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

		for (int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			traversePost(curLevel->children[childIndex]);
		}
		
		printAtom(curLevel);
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

		for (int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			deleteAST(curLevel->children[childIndex]);
		}
		//delete curLevel;
		curLevel == nullptr;
	}
}

void Interpreter::printAtom(Expression *& curLevel)
{
	if (curLevel->atomType == 1)
	{
		std::cout << curLevel->boolAtom << '\n';
	}
	else if (curLevel->atomType == 2)
	{
		std::cout << curLevel->doubleAtom << '\n';
	}
	else if (curLevel->atomType == 3)
	{
		std::cout << curLevel->stringAtom << '\n';
	}
	return;
}

void Interpreter::checkToken(std::string token, Expression *& curLevel)
{
	if (isdigit((token[0])))
	{
		curLevel = new Expression(std::stod(token));
	}
	else if ((token == "True"))
	{
		curLevel = new Expression(true);
	}
	else if (token == "False")
	{
		curLevel = new Expression(false);
	}
	else
	{
		curLevel = new Expression(token);
	}
	return;
}