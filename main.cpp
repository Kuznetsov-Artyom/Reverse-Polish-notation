#include <iostream>
#include <map>
#include <vector>
#include <cctype>
#include <string>
#include <stack>




// Возможные варианты лексем
enum class TypeToken
{
	DEFAULT = 0,
	CONST = 1,
	VARIABLE = 2,
	OPERATION = 3,
	OPEN_BRACKET = 4,
	CLOSE_BRACKET = 5
};

// Арность оператора
enum class Arity
{
	DEFAULT = 0,
	UNARY = 1,
	BINARY = 2
};


struct Token
{
	TypeToken type = TypeToken::DEFAULT;
	std::string name{};
	int value{};

	// Поле для лексемы типа OPERATION
	Arity arity = Arity::DEFAULT;

	Token() {}
	Token(TypeToken t, std::string n, int v = 0, Arity a = Arity::DEFAULT) : type{ t }, name{ n }, value{ v }, arity{ a } {}
};

std::vector<std::string> Separation(std::string str)
{
	std::vector<std::string> tokens;
	std::string current;

	bool flagVar = false;

	for (size_t i = 0; i < str.size(); ++i)
	{
		char sym = str[i];

		if (sym == ' ')
		{
			if (flagVar)
			{
				tokens.emplace_back(current);
				flagVar = false;
				current.clear();
			}
			continue;
		}

		if (isdigit(sym) || isalpha(sym) || sym == '_')
		{
			current += sym;
			flagVar = true;
		}
		else
		{
			if (flagVar)
			{
				tokens.emplace_back(current);
				flagVar = false;
				current.clear();
			}

			current += sym;
			tokens.emplace_back(current);
			current.clear();
		}
	}

	if (current.size()) tokens.emplace_back(current);

	return tokens;
}

bool isConst(std::string token)
{
	if (token.size() == 1 && token[0] == '0') return true;
	if (token.size() > 1 && token[0] == '0') return false;

	for (const auto& sym : token)
		if (!isdigit(sym)) return false;

	return true;
}
bool isVariable(std::string token)
{
	if (isdigit(token[0])) return false;

	for (const auto& sym : token)
		if (!isalpha(sym) && !isdigit(sym) && sym != '_') return false;

	return true;
}
bool isOperation(std::string token)
{
	if (token.size() > 1) return false;

	char sym = token[0];

	if (sym == '-' || sym == '+' || sym == '*' || sym == '/' || sym == '^') return true;

	return false;
}
bool isOpenBracket(std::string token)
{
	if (token.size() > 1) return false;

	return token[0] == '(';
}
bool isCloseBracket(std::string token)
{
	if (token.size() > 1) return false;

	return token[0] == ')';
}




class Record
{
private:
	std::string srcStr{};
	std::vector<Token> tokens{};

public:
	Record() {}
	Record(const std::string str) : srcStr{ str }
	{
		std::vector<std::string> strTokens = Separation(str);

		for (auto& token : strTokens)
		{
			if (isConst(token))
			{
				tokens.emplace_back(Token(TypeToken::CONST, token, std::atoi(token.c_str())));
			}

			else if (isVariable(token))
			{
				tokens.emplace_back(Token(TypeToken::VARIABLE, token));
			}

			else if (isOperation(token))
			{
				int priority = (token == "+" || token == "-") ? 1 : ((token == "^") ? 3 : 2);
				tokens.emplace_back(Token(TypeToken::VARIABLE, token, priority));
			}

			else if (isOpenBracket(token))
			{
				tokens.emplace_back(Token(TypeToken::OPEN_BRACKET, token));
			}

			else if (isCloseBracket(token))
			{
				tokens.emplace_back(Token(TypeToken::CLOSE_BRACKET, token));
			}
			else throw - 1;
		}
	}
	Record(const Record& other) : srcStr(other.srcStr), tokens{ other.tokens } {}


	size_t GetCountTokens() const noexcept { return tokens.size(); }
	std::string  GetSrcString() const noexcept { return srcStr; }

	Record& operator = (const Record& other)
	{
		if (this == &other) return *this;

		srcStr = other.srcStr;
		tokens = other.tokens;

		return *this;
	}
	Record& operator = (const std::string str)
	{
		if (str == srcStr) return *this;

		Record tmp(str);
		*this = tmp;

		return *this;
	}
	Token& operator [] (const size_t& pos)
	{
		return tokens.at(pos);
	}
};





class Poliz
{
private:
	Record recordTokens;
	std::map<std::string, int> tableVariable;
	std::vector<Token> postForm;
	std::stack<Token> stOperation;

	std::string polStr;

public:




		
};













int main()
{
	std::vector<std::string> vec = Separation("1 +5 *(6*    g56)-      3-uUuj+l");

	std::cout << isConst("05445") << '\n';
	std::cout << isVariable("gvf_123") << '\n';
	std::cout << isOperation("+") << '\n';
	std::cout << isOpenBracket(")") << '\n';
	std::cout << isCloseBracket("5") << '\n';


	Token a;

	Token b;

	b.name = "123";

	a = b;

	std::cout << a.name << '\n';


	for (const auto& elem : vec)
		std::cout << elem << ' ';
	std::cout << '\n';



	return 0;
}