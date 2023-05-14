#ifndef TOKEN_HPP
#define TOKEN_HPP


#include <string>
#include <vector>
#include <cctype>




// Possible variants of tokens
enum class TypeToken
{
	DEFAULT = 0,
	CONST = 1,
	VARIABLE = 2,
	OPERATION = 3,
	OPEN_BRACKET = 4,
	CLOSE_BRACKET = 5
};




// Operator arity
enum class Arity
{
	DEFAULT = 0,
	UNARY = 1,
	BINARY = 2
};




// The structure describing the token
struct Token
{
	TypeToken type = TypeToken::DEFAULT;
	std::string name{};
	double value{};

	// Field for the OPERATION type token
	Arity arity = Arity::DEFAULT;

	Token() {}
	Token(TypeToken t, std::string n, double v = 0.0, Arity a = Arity::DEFAULT) : type{ t }, name{ n }, value{ v }, arity{ a } {}
};



// Checks the token for the CONST type
bool IsConst(std::string token);

// Checks the token for the VARIABLE type
bool IsVariable(std::string token);

// Checks the token for the OPERATION type
bool IsOperation(std::string token);

// Checks the token for the OPEN_BRACKET type
bool IsOpenBracket(std::string token);

// Checks the token for the CLOSE_BRACKET type
bool IsCloseBracket(std::string token);



#endif // TOKEN_HPP