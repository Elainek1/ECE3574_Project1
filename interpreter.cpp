#include "interpreter.hpp"
#include "tokenize.hpp"
#include "interpreter_semantic_error.hpp"
//#include "environment.hpp"
#include <string>


Interpreter::Interpreter()
{
	rootAST = nullptr;
}

Interpreter::~Interpreter()
{
	//makes sure to delete the tree
	deleteAST(rootAST);
	rootAST = nullptr;

}

void Interpreter::reset()
{
	//this resets the the interpreter by deleting AST and reseting environment
	deleteAST(rootAST);
	rootAST = nullptr;
	environment.reset();
}

// Given a vtscript program as a std::istream, attempt to parse into an internal AST
// return true on success, false on failure
bool Interpreter::parse(std::istream & expression) noexcept
{
	std::vector<std::string> tokens;
	std::string inputAll = "";
	std::string inputPart = "";
	while (!expression.eof()) //while there is still input
	{
		getline(expression, inputPart);  //get the line
		for (int i = 0; i < inputPart.size(); i++)  //this checks to see if there is a ;
		{
			if (inputPart[i] == ';') //if there is one then the rest following it is a comment
			{
				break;
			}
			else //otherwise you add it to the input
			{
				inputAll += inputPart[i];
			}
		}
		inputAll += '\n';

	}
	std::istringstream inputStr(inputAll); //make input a stream
	tokens = tokenizeInput(inputStr); //feed it to the tokenizeInput to get the tokens

	if (tokens.size()>1) //if there is tokens 
	{
		
		if (!buildAST(tokens)) //try to build the AST
		{
			return false; //if that fails then parsing failed
		}

		return true; //otherwise return true parsing suceeded
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
		Expression answer = evalExp(rootAST); //try to evaluate the tree
		deleteAST(rootAST); //delete the tree
		rootAST = nullptr; //reset pointer
		return answer; //return the result of the evaluation
	}
	catch (InterpreterSemanticError error)  //catch any error that occurs
	{
		//std::cout << error.what() << "\n";
		deleteAST(rootAST); //delete the tree
		rootAST = nullptr; //reset pointer
		throw; //throw the error
	}
	
}

//this is a recursive function that evaluates the current expression node
Expression Interpreter::evalExp(Expression * curLevel)
{
	Expression answer = Expression(); //creates answer expression
	//answer.doubleAtom = 0;
	try {
		if (curLevel == nullptr)  //if there is nothing then can't evaluate it
		{
			std::string error = "Error: Nothing to evaluate";
			throw InterpreterSemanticError(error); //throw an error
			return Expression();
		}
		else //otherwise check which operation it is
		{
			//check to see if any of the string is an operation we must perform
			//if it is then call the corresponding function
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
			//otherwise if it is none then there is an error
			else if (curLevel->atomType == Expression::noneType)
			{
				std::string error = "Error: Input is not valid";
				throw InterpreterSemanticError(error); //throw the error
				return Expression();
			}
			else  //if neither of those then check to see if it is a variable defined in the environment
			{
				if (curLevel->atomType == 3) //if it is a string
				{
					int a = environment.symbolExist(curLevel->stringAtom); //check to see if symbol exists
					if (a == 1) //if a one is returned then it was a bool answer
					{
						if (curLevel->children.size() == 0)
						{
							return Expression(environment.getBoolSymbol(curLevel->stringAtom)); //return this bool answer
						}
					}
					else if (a == 2) //if it was a two then it was a double answer
					{
						if (curLevel->children.size() == 0)
						{
							return Expression(environment.getDoubleSymbol(curLevel->stringAtom)); //return this double answer
						}
					}
					else //otherwise it means the variable isn't defined in the environment
					{
						std::string error = "Error: Variable not defined\n";
						throw InterpreterSemanticError(error); //throw an error then
						return Expression(); //return none type
					}
				}
				else //otherwise just return the current expression
				{
					return *curLevel;
				}
				return *curLevel;
			}
		}
	}
	catch (InterpreterSemanticError error) //catch any errors thrown
	{
		throw; //rethrow the error
		return Expression();
	}

	return answer; //return the answer in the end
}

//this builds the AST based on the vector of tokens that is given as input
//the function returns a boolean, true meaning the tree building was successful
//false indicates that the tree didn't build successfully
bool Interpreter::buildAST(std::vector<std::string> tokens)
{
	Expression *currLevel;
	Expression *previousLevel;
	//if the first and last do not have () it automatically isn't a good tree
	if ((tokens[0] == "(") && (tokens[tokens.size() - 1] == ")"))
	{
		//loop through the token vector
		for (unsigned int i = 0; i < tokens.size(); i++)
		{
			//if it is a ( then you start a new node in the tree
			if (tokens[i] == "(")
			{
				//make sure the next token is a valid atom value
				if ((tokens[i + 1] == "(") || (tokens[i + 1] == ")"))
				{
					return false;//otherwise it isn't so return false
				}
				else
				{
					if (i == 0)//if it is the first ( then you know that it is the root
					{
						rootAST = checkToken(tokens[i + 1], rootAST); //make the root node
						if (rootAST->atomType == 0) //if it is a none type then it failed
						{
							return false; //return false
						}
						currLevel = rootAST; //otherwise you can update the pointers
						previousLevel = currLevel;
						i++; //increment to next token
					}
					else //otherwise it isn't the first node so use the currLevel pointer
					{

						previousLevel = currLevel;
						currLevel = checkToken(tokens[i + 1], currLevel); //create the next node in the tree
						if (currLevel->atomType == 0)
						{
							return false;
						}
						//make sure to connect the node to the rest of the tree
						currLevel->parent = previousLevel; //do this by connect parent and children association
						currLevel->parent->children.push_back(currLevel);
						i++; //move to next token
					}
				}
			}
			else if (tokens[i] == ")") //if it is ) then you end the current level and move up in the tree
			{
				if (previousLevel->parent != nullptr) //if it is not null then you are not at the root so you can move up
				{
					previousLevel = previousLevel->parent;  //move up
				}
				currLevel = currLevel->parent; //move up
			}
			else //otherwise it is an atom value and needs to be a child
			{
				previousLevel = currLevel;
				currLevel = checkToken(tokens[i], currLevel); //make the new node
				if (currLevel->atomType == 0)
				{
					return false;
				}
				currLevel->parent = previousLevel;
				currLevel->parent->children.push_back(currLevel); //make it a children
				currLevel = previousLevel; //update the pointer
			}
		}
	}
	else //this return false is for the bad tree () beginning
	{
		return false;
	}
	return true; //if everything else executes correctly then the tree was successfully built
}

//this traverses the tree recursively and prints the expression at each node in the pree
//this function was used for debugging purposes
void Interpreter::traversePost(Expression * curLevel)
{
	if (curLevel == nullptr) //if the pointer is null then there is nothing to print
	{
		return;
	}
	else //otherwise go through each children
	{

		for (unsigned int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			traversePost(curLevel->children[childIndex]); //call traverse on each children
		}
		
		printExpression(*curLevel); //print the expressioin
	}
}

//this deletes the AST to make sure that there is not memory leak
//the pointer that is fed in should be the root node pointer
void Interpreter::deleteAST(Expression * curLevel)
{
	if (curLevel == nullptr) //if does not point to anything then you don't have to delete anything
	{
		return;
	}
	else //otherwise loop through each children and call deleteAST on it to delete them first
	{

		for (unsigned int childIndex = 0; childIndex < curLevel->children.size(); childIndex++)
		{
			deleteAST(curLevel->children[childIndex]);
		}
		delete curLevel; //then delete yourself
	}
}

//this prints the expression out
//the pointer that is fed in is the pointer to the expression that we want to print
void Interpreter::printExpression(Expression curLevel)
{
	if (curLevel.atomType == 0) //if it is a none type then you print ()
	{
		std::cout << "()\n";
	}
	else if (curLevel.atomType == 1) //if it is a boolean type
	{
		if (curLevel.boolAtom) //then read the value
		{
			std::cout << "(True)\n"; //if it is true then print true
		}
		else //else print false
		{
			std::cout << "(False)\n";
		}
	}
	else if (curLevel.atomType == 2) //if it is a double type
	{
		std::cout << "(" << curLevel.doubleAtom << ")\n"; //then print the value of the double
	}
	else if (curLevel.atomType == 3) //if it is a string type
	{
		std::cout << "(" << curLevel.stringAtom << ")\n"; //then print the value of the string
	}
	return;
}

//this epxression takes in a token and the pointer that is suppose to pointer to the expression created
//and it returns the pointer of the expression, this is used to assign value to the pointer in the calling function
Expression * Interpreter::checkToken(std::string token, Expression* curLevel)
{
	if ((token == "True")) //if it is true then create the Expression with value true
	{
		curLevel = new Expression(true);
	}
	else if (token == "False") //if it is false create expression with value false
	{
		curLevel = new Expression(false);
	}
	else //otherwise need to determine what type of token it is
	{
		double tokenVal; //this is to hold the token
		std::stringstream parser; //create this so we can read in the token
		parser << token; //push token into the stream
		parser >> tokenVal; //try to assign token value to the double holder
		if (parser.fail()) //if it did fail
		{
			parser.clear(); //then that means that there was no digits at all
			curLevel = new Expression(token); //create a Expression that is purely string
		}
		else
		{
			if (parser.eof()) //if all of it was read into the double holder
			{
				curLevel = new Expression(tokenVal); //then create an expression that is a double
			}
			else //otherwise it is a string
			{
				if (!isdigit(token[0])) //if the first character in the string is not a digit
				{
					curLevel = new Expression(token); //then it is a valid string expressioin
				}
				else //otherwise it isn't valid so return the none type expression
				{
					curLevel = new Expression();
				}
			}
		}
	}
	return curLevel;
}


//the rest of these functions are for the evaluation of the expression
//each function takes in a pointer to the expression that needs to be valuated
//the function returns an expression that contains the value of the evaluation

//for example the add function sees the expression that contains +
//that means we need to recursively evaluate the children and then add them all together
//and that result is what it returns
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