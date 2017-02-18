#include "expression.hpp"

Expression::Expression()
{
	
}
/*
Expression::~Expression()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
	//delete parent;
}
*/

Expression::Expression(bool value)
{
	boolAtom = value;
}

// Construct an Expression with a single Number atom with value
Expression::Expression(double value)
{
	doubleAtom = value;
}

// Construct an Expression with a single Symbol atom with value
Expression::Expression(const std::string & value)
{
	stringAtom = value;
}

Expression::Expression(int value)
{
	doubleAtom = (double) value;
}

// Equality operator for two Expressions, two expressions are equal if the have the same 
// type, atom value, and number of arguments
bool Expression::operator==(const Expression & exp) const noexcept
{
	return true;
}
