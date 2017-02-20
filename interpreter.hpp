#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <string>
#include "environment.hpp"
#include "expression.hpp"


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
	Environment environment;

	Expression * rootAST;
	void buildAST(std::vector<std::string> tokens);
	void deleteAST(Expression *& curLevel);
	Expression checkToken(std::string token, Expression *& curLevel);
	void printExpression(Expression curLevel);
	void traversePost(Expression* curLevel);

	Expression add(Expression *& curLevel);
	Expression divide(Expression *& curLevel);
	Expression multiply(Expression *& curLevel);
	Expression subtract(Expression *& curLevel);
	Expression equal(Expression *& curLevel);
	Expression greaterEqual(Expression *& curLevel);
	Expression greater(Expression *& curLevel);
	Expression lessEqual(Expression *& curLevel);
	Expression less(Expression *& curLevel);
	Expression or (Expression *& curLevel);
	Expression and (Expression *& curLevel);
	Expression not(Expression *& curLevel);
	Expression ifExpression(Expression *& curLevel);
	Expression begin(Expression *& curLevel);
	Expression define(Expression *& curLevel);
};



#endif