#include "Token.hpp"



// Checks the token for the CONST type
bool IsConst(std::string token)
{
	if (token.front() == '.' || token.back() == '.') return false;
	if (token.size() > 1 && token[0] == '0' && token[1] != '.') return false;

	bool flagPoint = false;

	for (const auto& sym : token)
	{
		if (sym == '.')
		{
			if (!flagPoint) flagPoint = true;
			else return false;
		}
		if (!(isdigit(sym) || sym == '.')) return false;
	}

	return true;


}

// Checks the token for the VARIABLE type
bool IsVariable(std::string token)
{
	if (isdigit(token[0])) return false;

	for (const auto& sym : token)
		if (!(isalpha(sym) || isdigit(sym) || sym == '_')) return false;

	return true;
}

// Checks the token for the OPERATION type
bool IsOperation(std::string token)
{
	if (token.size() > 1) return false;

	char sym = token[0];

	if (sym == '-' || sym == '+' || sym == '*' || sym == '/' || sym == '^') return true;

	return false;
}

// Checks the token for the OPEN_BRACKET type
bool IsOpenBracket(std::string token)
{
	if (token.size() > 1) return false;

	return token[0] == '(';
}

// Checks the token for the CLOSE_BRACKET type
bool IsCloseBracket(std::string token)
{
	if (token.size() > 1) return false;

	return token[0] == ')';
}