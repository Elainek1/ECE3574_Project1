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
	for (int i = 0; i < tokens.size(); i++) //print the token
	{
	std::cout << tokens[i] << "\n";
	}
	if (tokens.size()>1)
	{
		if (!(tokens[0] == "(") || !(tokens[tokens.size() - 1] == ")"))
		{
			return false;
		}
		buildAST(tokens);
		return true;
	}
	return false;
}

// Evaluate the current AST and return the resulting Expression
// throws InterpreterSemanticError if a semantic error is encountered
// the exception message string should document the nature of the semantic error 
Expression eval()
{
	return "(a)";
}

void Interpreter::buildAST(std::vector<std::string> tokens)
{
	Expression *root = new Expression;
	Expression *currLevel;
	Expression *previousLevel;

	//if()

}