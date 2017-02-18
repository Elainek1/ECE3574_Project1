#include "interpreter.hpp"
#include "tokenize.hpp"

Interpreter::Interpreter()
{
	
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
	}*/
	if (tokens.size()>1)
	{
		if (!(tokens[0] == "(") || !(tokens[tokens.size() - 1] == ")"))
		{
			return false;
		}
		buildAST(tokens);
		eval();
		deleteAST(root);
		return true;
	}
	return false;
}

// Evaluate the current AST and return the resulting Expression
// throws InterpreterSemanticError if a semantic error is encountered
// the exception message string should document the nature of the semantic error 
Expression Interpreter::eval()
{
	Expression a = eval(root);
	
	return a;
}
Expression Interpreter::eval(Expression * curLevel)
{
	Expression a;
	if (curLevel == nullptr)
	{
		return a;
	}
	else
	{
		for (int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			eval(curLevel->children[childIndex]);
		}
		std::cout << curLevel->stringAtom << std::endl;
	}

	return a;
}
void Interpreter::buildAST(std::vector<std::string> tokens)
{
	//Expression *root;
	Expression *currLevel;
	Expression *previousLevel;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "(")
		{
			if (i == 0)
			{
				root = new Expression(tokens[i + 1]);
				currLevel = root;
				i++;
			}
			else
			{
				previousLevel = currLevel;
				currLevel = new Expression(tokens[i + 1]);
				currLevel->parent = previousLevel;
				currLevel->parent->children.push_back(currLevel);
				i++;

			}
		}
		else if (tokens[i] == ")")
		{
			previousLevel = previousLevel->parent;
			currLevel = currLevel->parent;
		}
		else
		{
			previousLevel = currLevel;
			currLevel = new Expression(tokens[i]);
			currLevel->parent = previousLevel;
			currLevel->parent->children.push_back(currLevel);
			currLevel = previousLevel;
		}
	}
}

void traversePost(Expression * curLevel)
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
		std::cout << curLevel->stringAtom << std::endl;
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

		for (int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			deleteAST(curLevel->children[childIndex]);
		}
		//delete curLevel;
		curLevel == nullptr;
	}
}