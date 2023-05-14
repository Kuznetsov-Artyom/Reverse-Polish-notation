#include "Record.hpp"



// Checks for the correctness of parentheses
bool CheckingParentheses(const std::string& str)
{
	int result = 0;

	for (int i = 0; i < str.size() && result >= 0; ++i)
	{
		if (str[i] == '(') ++result;
		else if (str[i] == ')') --result;
	}

	return result == 0;
}





// Splits the source string into tokens
void Record::SplitOnTokens(const std::string& str)
{
	std::string current;

	bool flagVar = false;

	for (size_t i = 0; i < str.size(); ++i)
	{
		char sym = str[i];

		if (sym == ' ')
		{
			if (flagVar)
			{
				strTokens.emplace_back(current);
				flagVar = false;
				current.clear();
			}
			continue;
		}

		if (isdigit(sym) || isalpha(sym) || sym == '_' || sym == '.')
		{
			current += sym;
			flagVar = true;
		}
		else
		{
			if (flagVar)
			{
				strTokens.emplace_back(current);
				flagVar = false;
				current.clear();
			}

			current += sym;
			strTokens.emplace_back(current);
			current.clear();
		}
	}

	if (current.size()) strTokens.emplace_back(current);
}

// Defines the types of tokens
void Record::DefiningTypes()
{
	for (auto& token : strTokens)
	{
		// Checking for the CONST type
		if (IsConst(token))
		{
			tokens.emplace_back(Token(TypeToken::CONST, token, std::atof(token.c_str())));
		}
		// Checking for the VARIABLE type
		else if (IsVariable(token))
		{
			tokens.emplace_back(Token(TypeToken::VARIABLE, token));
		}
		// OPERATION type check
		else if (IsOperation(token))
		{
			int priority = (token == "+" || token == "-") ? 1 : ((token == "^") ? 3 : 2);
			tokens.emplace_back(Token(TypeToken::OPERATION, token, static_cast<double>(priority)));
		}
		// Checking for the OPEN_BRACKET type
		else if (IsOpenBracket(token))
		{
			tokens.emplace_back(Token(TypeToken::OPEN_BRACKET, token));
		}
		// Checking for CLOSE_BRACKET type
		else if (IsCloseBracket(token))
		{
			tokens.emplace_back(Token(TypeToken::CLOSE_BRACKET, token));
		}
		// If the type is not defined (invalid value)
		else throw ExceptionRecord{ CodeError::TYPE_UNDEFINED, "Invalid token entered" };
	}
}

// Checks the correctness of the entered expression
void Record::CheckingCorrect()
{
	// Checking for the correctness of parentheses
	if (!CheckingParentheses(GetSrcStr()))
		throw ExceptionRecord{ CodeError::INVALID_PARENTHESES, "Parentheses are entered incorrectly" };

	bool flagVarConst = false;
	bool flagOperation = false;

	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i].type == TypeToken::CONST || tokens[i].type == TypeToken::VARIABLE)
		{
			// We throw an exception if there are 2 operands in a row
			if (flagVarConst)
				throw ExceptionRecord{ CodeError::NO_OPERATOR, "Absent operator" };

			flagVarConst = true;
			flagOperation = false;

		}
		else if (tokens[i].type == TypeToken::OPERATION)
		{
			// We throw an exception if the last token is an operation
			if (flagOperation || i == tokens.size() - 1)
				throw ExceptionRecord{ CodeError::NO_OPERAND, "Absent operand" };

			// We throw an exception if there is no operand in the beginning. expressions or after the opening parenthesis 
			if (tokens[i].name == "*" || tokens[i].name == "/" || tokens[i].name == "^")
				if (i == 0 || tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					throw ExceptionRecord{ CodeError::NO_OPERAND, "Absent operand" };

			flagOperation = true;
			flagVarConst = false;

		}
		else
		{
			// We throw an exception if there is no operator between the operands and the opening parenthesis
			if (tokens[i].type == TypeToken::OPEN_BRACKET && flagVarConst)
				throw ExceptionRecord{ CodeError::NO_OPERATOR, "Absent operator" };

			if (tokens[i].type == TypeToken::CLOSE_BRACKET)
			{
				// We throw an exception if empty parentheses were entered
				if (tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					throw ExceptionRecord{ CodeError::EMPTY_PARENTHESES, "Empty parentheses are introduced" };

				// We throw an exception if there is no operand before the closing parenthesis
				else if (flagOperation)
					throw ExceptionRecord{ CodeError::NO_OPERAND, "Absent operand" };
			}

			flagVarConst = false;
			flagOperation = false;
		}
	}
}




// +-+-+-+-+-+-+-+-+ Constructors +-+-+-+-+-+-+-+-+

Record::Record(const std::string& str) : srcStr{ str }
{
	// Throwing an exception if the expression was not entered
	if (srcStr.size() == 0)
		throw ExceptionRecord{ CodeError::EMPTY_EXPRESSION, "Expression is missing" };

	SplitOnTokens(str);
	DefiningTypes();
	CheckingCorrect();

}
Record::Record(const Record& other) : srcStr(other.srcStr), tokens{ other.tokens } {}




// +-+-+-+-+-+-+-+-+ Methods (getters) +-+-+-+-+-+-+-+-+

// Returns the number of tokens in the record
size_t Record::GetCount() const noexcept { return tokens.size(); }

// Returns the original passed string
std::string  Record::GetSrcStr() const noexcept { return srcStr; }




// +-+-+-+-+-+-+-+-+ Operators +-+-+-+-+-+-+-+-+

Record& Record::operator = (const Record& other)
{
	if (this == &other) return *this;

	srcStr = other.srcStr;
	tokens = other.tokens;

	return *this;
}
Record& Record::operator = (const std::string& str)
{
	if (str == srcStr) return *this;

	Record tmp(str);
	*this = tmp;

	return *this;
}
Token& Record::operator [] (const size_t& pos) { return tokens.at(pos); }