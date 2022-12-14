#include "Calculator.hpp"

// +-+-+-+-+-+-+-+-+ ��������� ������ +-+-+-+-+-+-+-+-+

// ���������� ����������� ������ ������
void Calculator::GenerationPostForm()
{
	std::stack<Token> stOperation;

	for (size_t i = 0; i < tokens.GetCount(); ++i)
	{
		// ���� ������� ���������� ��� ���������
		if (tokens[i].type == TypeToken::VARIABLE || tokens[i].type == TypeToken::CONST)
		{
			postForm.emplace_back(tokens[i]);
			tableVariable[tokens[i].name] = tokens[i].value;
		}
		// ���� ������� ����������� ������
		else if (tokens[i].type == TypeToken::OPEN_BRACKET)
		{
			stOperation.push(tokens[i]);
		}
		// ���� ������� ����������� ������
		else if (tokens[i].type == TypeToken::CLOSE_BRACKET)
		{
			while (!(stOperation.top().type == TypeToken::OPEN_BRACKET))
			{
				postForm.emplace_back(stOperation.top());
				stOperation.pop();
			}

			stOperation.pop();
		}
		// ���� ������� ��������
		else if (tokens[i].type == TypeToken::OPERATION)
		{
			// �������� �� ���������
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

// �������� ������ ������ ���� Calculator
void Calculator::CopyOther(const Calculator& other)
{
	tokens = other.tokens;
	tableVariable = other.tableVariable;
	postForm = other.postForm;
	polStr = other.polStr;
	result = other.result;
}




// +-+-+-+-+-+-+-+-+ ������������ +-+-+-+-+-+-+-+-+

Calculator::Calculator(const std::string& str) : tokens{ str }
{
	GenerationPostForm();
	Calculation();
}
Calculator::Calculator(const Calculator& other) { CopyOther(other); }




// +-+-+-+-+-+-+-+-+ ������ (�������) +-+-+-+-+-+-+-+-+

// ���������� "�������� �������� ������"
std::string Calculator::GetPolStr() const noexcept { return polStr; }

// ���������� �������� ���������� ������
std::string Calculator::GetSrcStr() const noexcept { return tokens.GetSrcStr(); }

// ���������� ��������� ����������
double Calculator::GetResult() const noexcept { return result; }

// ���������� �������� ������������ ����������
double Calculator::GetValVar(const std::string& name)
{
	if (tableVariable.find(name) == tableVariable.end())
		throw - 2;

	return tableVariable[name];
}

// ���������� ����������� ������ �� ������� � ����������� � �� ����������
const std::map<std::string, double>& Calculator::GetTableVar() const noexcept
{
	return tableVariable;
}




// +-+-+-+-+-+-+-+-+ ������ +-+-+-+-+-+-+-+-+
 
// �������� �������� ���������� �� �������
void Calculator::SetValVar(std::string name, double value)
{
	if (tableVariable.find(name) == tableVariable.end())
		throw - 2;

	tableVariable[name] = value;
}




// +-+-+-+-+-+-+-+-+ ������ ������ +-+-+-+-+-+-+-+-+

// ��������� ��������� ���������
void Calculator::Calculation()
{
	std::stack<double> stVariable;
	double left{};
	double right{};

	for (const auto& token : postForm)
	{
		// ���� ������� ���������� ��� ���������
		if (token.type == TypeToken::CONST || token.type == TypeToken::VARIABLE)
		{
			stVariable.push(tableVariable[token.name]);
		}
		// ���� ������� ��������
		if (token.type == TypeToken::OPERATION)
		{
			// ���� �������� �������
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
			// ����� �������� ��������
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

// ������� ���������� � �� �������� � ������� (����)
void Calculator::ShowTableVar() const noexcept
{
	std::cout << "size tableVariable = " << tableVariable.size() << '\n';
	for (const auto& elem : tableVariable)
	{
		std::cout << elem.first << ' ' << elem.second << '\n';
	}
}




// +-+-+-+-+-+-+-+-+ ��������� +-+-+-+-+-+-+-+-+

Calculator& Calculator::operator = (const Calculator& other)
{
	if (this == &other) return *this;

	CopyOther(other);

	return *this;
}
Calculator& Calculator::operator = (const std::string& str)
{
	if (tokens.GetSrcStr() == str) return *this;

	Calculator tmp(str);
	CopyOther(tmp);

	return *this;
}


