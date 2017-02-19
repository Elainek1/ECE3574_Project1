#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <string>
#include "expression.hpp"
//#include "environment.hpp"

class Interpreter
{
public:
	// Default construct an Interpreter with the default environment and an empty AST
	Interpreter();

	// Given a vtscript program as a std::istream, attempt to parse into an internal AST
	// return true on success, false on failure
	bool parse(std::istream & expression) noexcept;

	// Evaluate the current AST and return the resulting Expression
	// throws InterpreterSemanticError if a semantic error is encountered
	// the exception message string should document the nature of the semantic error 
	Expression eval();
	Expression eval(Expression *& curLevel);
private:
	Expression * rootAST;
	void buildAST(std::vector<std::string> tokens);
	void deleteAST(Expression *& curLevel);
	void checkToken(std::string token, Expression *& curLevel);
	void printAtom(Expression curLevel);
	void traversePost(Expression* curLevel);
};



#endif