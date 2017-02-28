#include "expression.hpp"
#include <iostream>

Expression::Expression()
{
	parent = nullptr;
	stringAtom = "";
	atomType = noneType;
}

Expression::~Expression()
{
	/*
	for (int i = 0; i < children.size(); i++)
	{
		(*children[i]).~Expression();
	}
	delete this;
	*/
	//delete parent;
}


Expression::Expression(bool value)
{
	parent = nullptr;
	atomType = boolType;
	stringAtom = "";
	boolAtom = value;
}

// Construct an Expression with a single Number atom with value
Expression::Expression(double value)
{
	parent = nullptr;
	atomType = doubleType;
	stringAtom = "";
	doubleAtom = value;
}

// Construct an Expression with a single Symbol atom with value
Expression::Expression(const std::string & value)
{
	parent = nullptr;
	atomType = stringType;
	stringAtom = value;
}

// Equality operator for two Expressions, two expressions are equal if the have the same 
// type, atom value, and number of arguments
bool Expression::operator==(const Expression & exp) const noexcept
{
	return true;
}
