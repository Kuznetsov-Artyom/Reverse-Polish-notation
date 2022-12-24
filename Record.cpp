#include "Record.hpp"



// Проверяет на корректность круглые скобки
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




// +-+-+-+-+-+-+-+-+ Приватные методы +-+-+-+-+-+-+-+-+

// Разделяет исходную строку на лексемы
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

// Определяет типы лексем
void Record::DefiningTypes()
{
	for (auto& token : strTokens)
	{
		// Проверка на тип CONST
		if (IsConst(token))
		{
			tokens.emplace_back(Token(TypeToken::CONST, token, std::atof(token.c_str())));
		}
		// Проверка на тип VARIABLE
		else if (IsVariable(token))
		{
			tokens.emplace_back(Token(TypeToken::VARIABLE, token));
		}
		// Проверка на тип OPERATION
		else if (IsOperation(token))
		{
			int priority = (token == "+" || token == "-") ? 1 : ((token == "^") ? 3 : 2);
			tokens.emplace_back(Token(TypeToken::OPERATION, token, static_cast<double>(priority)));
		}
		// Проверка на тип OPEN_BRACKET
		else if (IsOpenBracket(token))
		{
			tokens.emplace_back(Token(TypeToken::OPEN_BRACKET, token));
		}
		// Проверка на тип CLOSE_BRACKET
		else if (IsCloseBracket(token))
		{
			tokens.emplace_back(Token(TypeToken::CLOSE_BRACKET, token));
		}
		// Если тип не определен (некорректное значение)
		else throw ExceptionRecord{ CodeError::TYPE_UNDEFINED, "Введена недопустимая лексема" };
	}
}

// Проверяет корректность введенного выражения
void Record::CheckingCorrect()
{
	// Проверка на корректность круглых скобок
	if (!CheckingParentheses(GetSrcStr()))
		throw ExceptionRecord{ CodeError::INVALID_PARENTHESES, "Круглые скобки введены некорректно" };

	bool flagVarConst = false;
	bool flagOperation = false;

	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i].type == TypeToken::CONST || tokens[i].type == TypeToken::VARIABLE)
		{
			// Выбрасываем исключение, если идут 2 операнда подряд
			if (flagVarConst)
				throw ExceptionRecord{ CodeError::NO_OPERATOR, "Отсутствует оператор" };

			flagVarConst = true;
			flagOperation = false;

		}
		else if (tokens[i].type == TypeToken::OPERATION)
		{
			// Выбрасываем исключение, если последняя лексема операция
			if (flagOperation || i == tokens.size() - 1)
				throw ExceptionRecord{ CodeError::NO_OPERAND, "Отсутствует операнд" };

			// Выбрасываем исключение, если отсутсвует операнд в нач. выражения или после откр. скобки 
			if (tokens[i].name == "*" || tokens[i].name == "/" || tokens[i].name == "^")
				if (i == 0 || tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					throw ExceptionRecord{ CodeError::NO_OPERAND, "Отсутствует операнд" };

			flagOperation = true;
			flagVarConst = false;

		}
		else
		{
			// Выбрасываем исключение, если отсутсвует оператор между операндом и откр. скобкой
			if (tokens[i].type == TypeToken::OPEN_BRACKET && flagVarConst)
				throw ExceptionRecord{ CodeError::NO_OPERATOR, "Отсутствует оператор" };

			if (tokens[i].type == TypeToken::CLOSE_BRACKET)
			{
				// Выбрасываем исключение, если были введены пустые круглые скобки
				if (tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					throw ExceptionRecord{ CodeError::EMPTY_PARENTHESES, "Введены пустые круглые скобки" };

				// Выбрасываем исключение, если отсутсвует операнд перед закр. скобкой
				else if (flagOperation)
					throw ExceptionRecord{ CodeError::NO_OPERAND, "Отсутствует операнд" };
			}

			flagVarConst = false;
			flagOperation = false;
		}
	}
}




// +-+-+-+-+-+-+-+-+ Конструкторы +-+-+-+-+-+-+-+-+

Record::Record(const std::string& str) : srcStr{ str }
{
	// Выбрасываем исключение, если выражение не было введено
	if (srcStr.size() == 0)
		throw ExceptionRecord{ CodeError::EMPTY_EXPRESSION, "Выражение отсутсвует" };

	SplitOnTokens(str);
	DefiningTypes();
	CheckingCorrect();

}
Record::Record(const Record& other) : srcStr(other.srcStr), tokens{ other.tokens } {}




// +-+-+-+-+-+-+-+-+ Методы (геттеры) +-+-+-+-+-+-+-+-+

// Возвращает количество лексем в записи
size_t Record::GetCount() const noexcept { return tokens.size(); }

// Возвращает исходную переданную строку
std::string  Record::GetSrcStr() const noexcept { return srcStr; }




// +-+-+-+-+-+-+-+-+ Операторы +-+-+-+-+-+-+-+-+

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