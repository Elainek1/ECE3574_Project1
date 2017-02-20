#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "tokenize.hpp"
//#include "expression.hpp"
//#include "interpreter.hpp"
////////////////////////////////////
///SEE test_interpreter.cpp for added test cases
////////////////////////////////////
//Additional Test Cases
//More test cases were added to test_interpreter.cpp

TEST_CASE("Test Tokenize", "[tokenize]") {

	std::string tokenInput = "( + 1 2 )";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	tokens.push_back("(");
	tokens.push_back("+");
	tokens.push_back("1");
	tokens.push_back("2");
	tokens.push_back(")");
	REQUIRE(output == tokens);
}

TEST_CASE("Test Tokenize with newline", "[tokenize]") {

	std::string tokenInput = "(\n+\n1 2\n)";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	tokens.push_back("(");
	tokens.push_back("+");
	tokens.push_back("1");
	tokens.push_back("2");
	tokens.push_back(")");
	REQUIRE(output == tokens);
}

TEST_CASE("Test Tokenize with tab", "[tokenize]") {

	std::string tokenInput = "(\t+\t1\t2 )";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	tokens.push_back("(");
	tokens.push_back("+");
	tokens.push_back("1");
	tokens.push_back("2");
	tokens.push_back(")");
	REQUIRE(output == tokens);
}

TEST_CASE("Test Tokenize with newline and tab", "[tokenize]") {

	std::string tokenInput = "(\n+\t1 2\n)\t";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	tokens.push_back("(");
	tokens.push_back("+");
	tokens.push_back("1");
	tokens.push_back("2");
	tokens.push_back(")");
	REQUIRE(output == tokens);
}


TEST_CASE("Test Tokenize missing end )", "[tokenize]") {

	std::string tokenInput = "( + 1 2 ";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	REQUIRE(output == tokens);
}
TEST_CASE("Test Tokenize missing beginning (", "[tokenize]") {

	std::string tokenInput = " +\t1\t2)";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	REQUIRE(output == tokens);
}

TEST_CASE("Test Tokenize symbol in front (", "[tokenize]") {

	std::string tokenInput = "+ ( + 1 2)";
	std::istringstream iss(tokenInput);

	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	tokens.push_back("+");
	tokens.push_back("(");
	tokens.push_back("+");
	tokens.push_back("1");
	tokens.push_back("2");
	tokens.push_back(")");
	REQUIRE(output == tokens);
}
TEST_CASE("Test Tokenize symbol behind )", "[tokenize]") {

	std::string tokenInput = " (+ 1 2)+";
	std::istringstream iss(tokenInput);

	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	tokens.push_back("(");
	tokens.push_back("+");
	tokens.push_back("1");
	tokens.push_back("2");
	tokens.push_back(")");
	tokens.push_back("+");
	REQUIRE(output == tokens);
}
TEST_CASE("Test Tokenize extra (", "[tokenize]") {

	std::string tokenInput = "( + 1 (2 )";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	REQUIRE(output == tokens);
}
TEST_CASE("Test Tokenize extra )", "[tokenize]") {

	std::string tokenInput = "( + 1 2) )";
	std::istringstream iss(tokenInput);



	std::vector<std::string> output = tokenizeInput(iss);
	std::vector<std::string> tokens;
	REQUIRE(output == tokens);
}
/*
//added test case
TEST_CASE("Test Interpreter parser with second half input", "[interpreter]") {


{
std::string program = ")";
std::istringstream iss(program);

Interpreter interp;
bool ok = interp.parse(iss);
REQUIRE(ok == false);
}

{
std::string program = "f)";
std::istringstream iss(program);

Interpreter interp;
bool ok = interp.parse(iss);
REQUIRE(ok == false);
}

{
std::string program = "gin (define r 10) (* pi (* r r)))";
std::istringstream iss(program);

Interpreter interp;
bool ok = interp.parse(iss);
REQUIRE(ok == false);
}
}

//added cases
TEST_CASE("Test Interpreter special forms: begin, define, and if", "[interpreter]") {

{
std::string program = "(begin (define a True) (if a 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}

{
std::string program = "(begin (define a False) (if a 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(100.));
}
}
//added cases
TEST_CASE("Test Interpreter special forms: begin, define, if, and <", "[interpreter]") {

{
std::string program = "(begin (define a 1) (if (< a 2) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}

{
std::string program = "(begin (define a 1) (if (< a 0) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(100.));
}
}

//added cases
TEST_CASE("Test Interpreter special forms: begin, define, if, and <=", "[interpreter]") {

{
std::string program = "(begin (define a 1) (if (<= a 2) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}

{
std::string program = "(begin (define a 1) (if (<= a 0) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(100.));
}

{
std::string program = "(begin (define a 1) (if (<= a 1) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}
}
//added cases
TEST_CASE("Test Interpreter special forms: begin, define, if, and >", "[interpreter]") {

{
std::string program = "(begin (define a 3) (if (> a 2) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}

{
std::string program = "(begin (define a 1) (if (> a 2) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(100.));
}
}
//added cases
TEST_CASE("Test Interpreter special forms: begin, define, if, and >=", "[interpreter]") {

{
std::string program = "(begin (define a 1) (if (>= a 2) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(100.));
}

{
std::string program = "(begin (define a 1) (if (>= a 0) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}

{
std::string program = "(begin (define a 1) (if (>= a 1) 200 100))";
Expression result = run(program);
REQUIRE(result == Expression(200.));
}
}
//added cases
*/
