#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <vector>

class Expression
{
	//enum expressionType { noneType, boolType, doubleType, stringType };
public:
	enum expressionType { noneType = 0, boolType, doubleType, stringType };
	expressionType atomType;
	bool boolAtom;
	double doubleAtom;
	std::string stringAtom;
	Expression * parent;
	//auto atom;
	std::vector<Expression *> children;

	// Default construct an Expression of type None
	Expression();

	//Default deconstructor
	~Expression();

	// Construct an Expression with a single Boolean atom with value
	Expression(bool value);

	// Construct an Expression with a single Number atom with value
	Expression(double value);

	// Construct an Expression with a single Number atom with value
	Expression(int value);

	// Construct an Expression with a single Symbol atom with value
	Expression(const std::string & value);

	// Equality operator for two Expressions, two expressions are equal if the have the same 
	// type, atom value, and number of arguments
	bool operator==(const Expression & exp) const noexcept;
};

#endif