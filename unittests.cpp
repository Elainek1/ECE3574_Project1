#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "tokenize.hpp"


////////////////////////////////////
//Additional Test Cases

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

TEST_CASE("Test Tokenize with \n", "[tokenize]") {

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

TEST_CASE("Test Tokenize", "[tokenize]") {

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