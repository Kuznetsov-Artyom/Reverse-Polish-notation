#include "Arithmetic.hpp"


// +-+-+-+-+-+-+-+-+ Приватные методы +-+-+-+-+-+-+-+-+

// Генерирует поствиксную запись лексем
void Arithmetic::GenerationPostForm()
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

// Копирует другой объект типа Arithmetic
void Arithmetic::CopyOther(const Arithmetic& other)
{
	tokens = other.tokens;
	tableVariable = other.tableVariable;
	postForm = other.postForm;
	polStr = other.polStr;
	result = other.result;
}




// +-+-+-+-+-+-+-+-+ Конструкторы +-+-+-+-+-+-+-+-+

Arithmetic::Arithmetic(const std::string& str) : tokens{ str }
{
	GenerationPostForm();
	Calculation();
}
Arithmetic::Arithmetic(const Arithmetic& other) { CopyOther(other); }




// +-+-+-+-+-+-+-+-+ Методы (геттеры) +-+-+-+-+-+-+-+-+

// Возвращает "обратную польскую запись"
std::string Arithmetic::GetPolStr() const noexcept { return polStr; }

// Возвращает исходную переданную строку
std::string Arithmetic::GetSrcStr() const noexcept { return tokens.GetSrcStr(); }

// Возвращает результат вычисления
double Arithmetic::GetResult() const noexcept { return result; }

// Возвращает значение определенной переменной
double Arithmetic::GetValVar(const std::string& name)
{
	if (tableVariable.find(name) == tableVariable.end())
		throw std::logic_error{ "Данный элемент отсутсвует" };

	return tableVariable[name];
}

// Возвращает константкую ссылку на таблицу с переменными и их значениями
const std::map<std::string, double>& Arithmetic::GetTableVar() const noexcept
{
	return tableVariable;
}




// +-+-+-+-+-+-+-+-+ Сеттер +-+-+-+-+-+-+-+-+

// Изменяет значение переменной из таблицы
void Arithmetic::SetValVar(std::string name, double value)
{
	if (tableVariable.find(name) == tableVariable.end())
		throw std::logic_error{ "Данный элемент отсутсвует" };

	tableVariable[name] = value;
}




// +-+-+-+-+-+-+-+-+ Прочие методы +-+-+-+-+-+-+-+-+

// Вычисляет результат выражения
void Arithmetic::Calculation()
{
	std::stack<double> stVariable;
	double left{};
	double right{};

	for (const auto& token : postForm)
	{
		// Если лексема переменная или константа
		if (token.type == TypeToken::CONST || token.type == TypeToken::VARIABLE)
		{
			stVariable.push(tableVariable[token.name]);
		}
		// Если лексема операция
		else if (token.type == TypeToken::OPERATION)
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

				if (token.name == "+") stVariable.push(left + right);
				else if (token.name == "-") stVariable.push(left - right);
				else if (token.name == "*") stVariable.push(left * right);
				else if (token.name == "/")
				{
					// Выбрасываем исключение, если происходит деление на ноль
					if (right == 0)
						throw ExceptionRecord{ CodeError::ZERO_DIVISION, "Деление на ноль" };

					stVariable.push(left / right);
				}
				else stVariable.push(pow(left, right));

				left = 0;
				right = 0;
			}
		}
	}

	result = stVariable.top();
}

// Выводит переменные и их значения в консоль (логи)
void Arithmetic::ShowTableVar() const noexcept
{
	std::cout << "size tableVariable = " << tableVariable.size() << '\n';
	for (const auto& elem : tableVariable)
	{
		std::cout << elem.first << ' ' << elem.second << '\n';
	}
}




// +-+-+-+-+-+-+-+-+ Операторы +-+-+-+-+-+-+-+-+

Arithmetic& Arithmetic::operator = (const Arithmetic& other)
{
	if (this == &other) return *this;

	CopyOther(other);

	return *this;
}
Arithmetic& Arithmetic::operator = (const std::string& str)
{
	if (tokens.GetSrcStr() == str) return *this;

	Arithmetic tmp(str);
	CopyOther(tmp);

	return *this;
}



