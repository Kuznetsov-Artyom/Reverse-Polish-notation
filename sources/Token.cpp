#include "Token.hpp"



// ��������� ������� �� ��� CONST
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

// ��������� ������� �� ��� VARIABLE
bool IsVariable(std::string token)
{
	if (isdigit(token[0])) return false;

	for (const auto& sym : token)
		if (!(isalpha(sym) || isdigit(sym) || sym == '_')) return false;

	return true;
}

// ��������� ������� �� ��� OPERATION
bool IsOperation(std::string token)
{
	if (token.size() > 1) return false;

	char sym = token[0];

	if (sym == '-' || sym == '+' || sym == '*' || sym == '/' || sym == '^') return true;

	return false;
}

// ��������� ������� �� ��� OPEN_BRACKET
bool IsOpenBracket(std::string token)
{
	if (token.size() > 1) return false;

	return token[0] == '(';
}

// ��������� ������� �� ��� CLOSE_BRACKET
bool IsCloseBracket(std::string token)
{
	if (token.size() > 1) return false;

	return token[0] == ')';
}