#include "Record.hpp"



// ��������� �� ������������ ������� ������
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




// +-+-+-+-+-+-+-+-+ ��������� ������ +-+-+-+-+-+-+-+-+

// ��������� �������� ������ �� �������
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

// ���������� ���� ������
void Record::DefiningTypes()
{
	for (auto& token : strTokens)
	{
		// �������� �� ��� CONST
		if (IsConst(token))
		{
			tokens.emplace_back(Token(TypeToken::CONST, token, std::atof(token.c_str())));
		}
		// �������� �� ��� VARIABLE
		else if (IsVariable(token))
		{
			tokens.emplace_back(Token(TypeToken::VARIABLE, token));
		}
		// �������� �� ��� OPERATION
		else if (IsOperation(token))
		{
			int priority = (token == "+" || token == "-") ? 1 : ((token == "^") ? 3 : 2);
			tokens.emplace_back(Token(TypeToken::OPERATION, token, static_cast<double>(priority)));
		}
		// �������� �� ��� OPEN_BRACKET
		else if (IsOpenBracket(token))
		{
			tokens.emplace_back(Token(TypeToken::OPEN_BRACKET, token));
		}
		// �������� �� ��� CLOSE_BRACKET
		else if (IsCloseBracket(token))
		{
			tokens.emplace_back(Token(TypeToken::CLOSE_BRACKET, token));
		}
		// ���� ��� �� ��������� (������������ ��������)
		else throw ExceptionRecord{ CodeError::TYPE_UNDEFINED, "������� ������������ �������" };
	}
}

// ��������� ������������ ���������� ���������
void Record::CheckingCorrect()
{
	// �������� �� ������������ ������� ������
	if (!CheckingParentheses(GetSrcStr()))
		throw ExceptionRecord{ CodeError::INVALID_PARENTHESES, "������� ������ ������� �����������" };

	bool flagVarConst = false;
	bool flagOperation = false;

	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i].type == TypeToken::CONST || tokens[i].type == TypeToken::VARIABLE)
		{
			// ����������� ����������, ���� ���� 2 �������� ������
			if (flagVarConst)
				throw ExceptionRecord{ CodeError::NO_OPERATOR, "����������� ��������" };

			flagVarConst = true;
			flagOperation = false;

		}
		else if (tokens[i].type == TypeToken::OPERATION)
		{
			// ����������� ����������, ���� ��������� ������� ��������
			if (flagOperation || i == tokens.size() - 1)
				throw ExceptionRecord{ CodeError::NO_OPERAND, "����������� �������" };

			// ����������� ����������, ���� ���������� ������� � ���. ��������� ��� ����� ����. ������ 
			if (tokens[i].name == "*" || tokens[i].name == "/" || tokens[i].name == "^")
				if (i == 0 || tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					throw ExceptionRecord{ CodeError::NO_OPERAND, "����������� �������" };

			flagOperation = true;
			flagVarConst = false;

		}
		else
		{
			// ����������� ����������, ���� ���������� �������� ����� ��������� � ����. �������
			if (tokens[i].type == TypeToken::OPEN_BRACKET && flagVarConst)
				throw ExceptionRecord{ CodeError::NO_OPERATOR, "����������� ��������" };

			if (tokens[i].type == TypeToken::CLOSE_BRACKET)
			{
				// ����������� ����������, ���� ���� ������� ������ ������� ������
				if (tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					throw ExceptionRecord{ CodeError::EMPTY_PARENTHESES, "������� ������ ������� ������" };

				// ����������� ����������, ���� ���������� ������� ����� ����. �������
				else if (flagOperation)
					throw ExceptionRecord{ CodeError::NO_OPERAND, "����������� �������" };
			}

			flagVarConst = false;
			flagOperation = false;
		}
	}
}




// +-+-+-+-+-+-+-+-+ ������������ +-+-+-+-+-+-+-+-+

Record::Record(const std::string& str) : srcStr{ str }
{
	// ����������� ����������, ���� ��������� �� ���� �������
	if (srcStr.size() == 0)
		throw ExceptionRecord{ CodeError::EMPTY_EXPRESSION, "��������� ����������" };

	SplitOnTokens(str);
	DefiningTypes();
	CheckingCorrect();

}
Record::Record(const Record& other) : srcStr(other.srcStr), tokens{ other.tokens } {}




// +-+-+-+-+-+-+-+-+ ������ (�������) +-+-+-+-+-+-+-+-+

// ���������� ���������� ������ � ������
size_t Record::GetCount() const noexcept { return tokens.size(); }

// ���������� �������� ���������� ������
std::string  Record::GetSrcStr() const noexcept { return srcStr; }




// +-+-+-+-+-+-+-+-+ ��������� +-+-+-+-+-+-+-+-+

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