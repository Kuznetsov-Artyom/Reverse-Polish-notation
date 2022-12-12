#include <iostream>
#include <map>
#include <vector>
#include <cctype>
#include <string>
#include <stack>
#include <cmath>



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


// Структура описывающая лексему
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
	Record(const std::string& str) : srcStr{ str }
	{
		std::vector<std::string> strTokens = Separation(str);

		for (auto& token : strTokens)
		{
			// Проверка на тип CONST
			if (isConst(token))
			{
				tokens.emplace_back(Token(TypeToken::CONST, token, std::atoi(token.c_str())));
			}
			// Проверка на тип VARIABLE
			else if (isVariable(token))
			{
				tokens.emplace_back(Token(TypeToken::VARIABLE, token));
			}
			// Проверка на тип OPERATION
			else if (isOperation(token))
			{
				int priority = (token == "+" || token == "-") ? 1 : ((token == "^") ? 3 : 2);
				tokens.emplace_back(Token(TypeToken::OPERATION, token, priority));
			}
			// Проверка на тип OPEN_BRACKET
			else if (isOpenBracket(token))
			{
				tokens.emplace_back(Token(TypeToken::OPEN_BRACKET, token));
			}
			// Проверка на тип CLOSE_BRACKET
			else if (isCloseBracket(token))
			{
				tokens.emplace_back(Token(TypeToken::CLOSE_BRACKET, token));
			}
			// Если тип не определен (некорректное значение)
			else throw - 1;
		}
	}
	Record(const Record& other) : srcStr(other.srcStr), tokens{ other.tokens } {}


	size_t GetCount() const noexcept { return tokens.size(); }
	std::string  GetSrcString() const noexcept { return srcStr; }

	Record& operator = (const Record& other)
	{
		if (this == &other) return *this;

		srcStr = other.srcStr;
		tokens = other.tokens;

		return *this;
	}
	Record& operator = (const std::string& str)
	{
		if (str == srcStr) return *this;

		Record tmp(str);
		*this = tmp;

		return *this;
	}
	Token& operator [] (const size_t& pos) { return tokens.at(pos); }
};





class Calculator
{
private:
	Record tokens;
	std::map<std::string, int> tableVariable;
	std::vector<Token> postForm;
	std::string polStr;
	int result;

private:
	void GenerationPostForm()
	{
		std::stack<Token> stOperation;

		for (size_t i = 0; i < tokens.GetCount(); ++i)
		{
			// Если лексема переменная или константа
			if (tokens[i].type == TypeToken::VARIABLE || tokens[i].type == TypeToken::CONST)
			{
				postForm.emplace_back(tokens[i]);
				tableVariable[tokens[i].name] = tokens[i].value;
			}
			// Если лексема открывающая скобка
			else if (tokens[i].type == TypeToken::OPEN_BRACKET)
			{
				stOperation.push(tokens[i]);
			}
			// Если лексема закрывающая скобка
			else if (tokens[i].type == TypeToken::CLOSE_BRACKET)
			{
				while (!(stOperation.top().type == TypeToken::OPEN_BRACKET))
				{
					postForm.emplace_back(stOperation.top());
					stOperation.pop();
				}

				stOperation.pop();
			}
			// Если лексема операция
			else if (tokens[i].type == TypeToken::OPERATION)
			{
				// Проверка на унарность
				if (tokens[i].name == "+" || tokens[i].name == "-")
				{
					if (i == 0 || tokens[i - 1].type == TypeToken::OPEN_BRACKET)
					{
						tokens[i].arity = Arity::UNARY;
					}
				}

				while (!stOperation.empty() && tokens[i].value <= stOperation.top().value)
				{
					postForm.emplace_back(stOperation.top());
					stOperation.pop();
				}

				stOperation.push(tokens[i]);
			}
		}

		while (!stOperation.empty())
		{
			postForm.emplace_back(stOperation.top());
			stOperation.pop();
		}

		for (const auto& token : postForm)
		{
			polStr += token.name;
			polStr += ' ';
		}
		polStr = polStr.substr(0, polStr.size() - 1);
	}
	void CopyOther(const Calculator& other)
	{
		tokens = other.tokens;
		tableVariable = other.tableVariable;
		postForm = other.postForm;
		polStr = other.polStr;
		result = other.result;
	}

public:
	Calculator(const std::string& str) : tokens{ str }
	{
		GenerationPostForm();
		Calculation();		
	}
	Calculator(const Calculator& other) { CopyOther(other); }


	std::string GetPolStr() const noexcept { return polStr; }
	int GetResult() const noexcept { return result; }
	int GetValVar(const std::string& name)
	{
		if (tableVariable.find(name) == tableVariable.end())
			throw - 2;

		return tableVariable[name];
	}
	const std::map<std::string, int>& GetTableVar() const noexcept
	{
		return tableVariable;
	}


	void SetValVar(std::string name, int value)
	{
		if (tableVariable.find(name) == tableVariable.end())
			throw - 2;

		tableVariable[name] = value;
	}	


	void Calculation()
	{
		std::stack<int> stVariable;
		int left{};
		int right{};

		for (const auto& token : postForm)
		{
			// Если лексема переменная или константа
			if (token.type == TypeToken::CONST || token.type == TypeToken::VARIABLE)
			{
				stVariable.push(tableVariable[token.name]);
			}
			// Если лексема операция
			if (token.type == TypeToken::OPERATION)
			{
				// Если операция унарная
				if (token.arity == Arity::UNARY)
				{
					right = stVariable.top();
					stVariable.pop();

					if (token.name == "+")
						stVariable.push(right);
					if (token.name == "-")
						stVariable.push(-right);

					right = 0;
				}
				// Иначе операция бинарная
				else
				{
					right = stVariable.top();
					stVariable.pop();

					left = stVariable.top();
					stVariable.pop();

					if (token.name == "+")
						stVariable.push(left + right);
					if (token.name == "-")
						stVariable.push(left - right);
					if (token.name == "*")
						stVariable.push(left * right);
					if (token.name == "/")
						stVariable.push(left / right);
					if (token.name == "^")
						stVariable.push(pow(left, right));

					left = 0;
					right = 0;
				}
			}
		}

		result = stVariable.top();
	}
	void ShowTableVar() const noexcept
	{
		std::cout << "size tableVariable = " << tableVariable.size() << '\n';
		for (const auto& elem : tableVariable)
		{
			std::cout << elem.first << ' ' << elem.second << '\n';
		}
	}


	Calculator& operator = (const Calculator& other)
	{
		if (this == &other) return *this;

		CopyOther(other);

		return *this;
	}
	Calculator& operator = (const std::string& str)
	{
		if (tokens.GetSrcString() == str) return *this;

		Calculator tmp(str);
		CopyOther(tmp);

		return *this;
	}
};













int main()
{
	Calculator test("-1 * (-1) + 2 ^ (9) / 3");

	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	test = "5 + a + 5 -3*(-11) + aaa_1";

	std::cout << '\n';

	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	
	test.SetValVar("a", -23);
	test.Calculation();

	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	std::cout << '\n';

	test = "2+(-3) + abc ^ a";
	std::cout << test.GetResult() << "\n\n";
	test.SetValVar("abc", -5);
	test.SetValVar("a", 2);
	test.Calculation();
	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';






	for (const auto& elem : test.GetTableVar())
	{
		std::cout << elem.first << ' ' << elem.second << '\n';
	}







	return 0;
}