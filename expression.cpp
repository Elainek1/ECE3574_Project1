#include "expression.hpp"

Expression::Expression()
{

}

Expression::Expression(bool value)
{

}

// Construct an Expression with a single Number atom with value
Expression::Expression(double value)
{

}

// Construct an Expression with a single Symbol atom with value
Expression::Expression(const std::string & value)
{

}

// Equality operator for two Expressions, two expressions are equal if the have the same 
// type, atom value, and number of arguments
bool Expression::operator==(const Expression & exp) const noexcept
{
	return true;
}