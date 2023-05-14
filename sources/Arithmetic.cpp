#include "Arithmetic.hpp"


// Generates a postfix entry of tokens
void Arithmetic::GenerationPostForm()
{
	std::stack<Token> stOperation;

	for (size_t i = 0; i < tokens.GetCount(); ++i)
	{
		// If the token is a variable or constant
		if (tokens[i].type == TypeToken::VARIABLE || tokens[i].type == TypeToken::CONST)
		{
			postForm.emplace_back(tokens[i]);
			tableVariable[tokens[i].name] = tokens[i].value;
		}
		// If the token is an opening bracket
		else if (tokens[i].type == TypeToken::OPEN_BRACKET)
		{
			stOperation.push(tokens[i]);
		}
		// If the token is a closing bracket
		else if (tokens[i].type == TypeToken::CLOSE_BRACKET)
		{
			while (!(stOperation.top().type == TypeToken::OPEN_BRACKET))
			{
				postForm.emplace_back(stOperation.top());
				stOperation.pop();
			}

			stOperation.pop();
		}
		// If the token is an operation
		else if (tokens[i].type == TypeToken::OPERATION)
		{
			// Checking for unary
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

// Copies another object of the Arithmetic type
void Arithmetic::CopyOther(const Arithmetic& other)
{
	tokens = other.tokens;
	tableVariable = other.tableVariable;
	postForm = other.postForm;
	polStr = other.polStr;
	result = other.result;
}




// +-+-+-+-+-+-+-+-+ Constructors +-+-+-+-+-+-+-+-+

Arithmetic::Arithmetic(const std::string& str) : tokens{ str }
{
	GenerationPostForm();
	Calculation();
}
Arithmetic::Arithmetic(const Arithmetic& other) { CopyOther(other); }




// +-+-+-+-+-+-+-+-+ Methods (getters) +-+-+-+-+-+-+-+-+

// Returns "reverse Polish entry"
std::string Arithmetic::GetPolStr() const noexcept { return polStr; }

// Returns the original passed string
std::string Arithmetic::GetSrcStr() const noexcept { return tokens.GetSrcStr(); }

// Returns the result of the calculation
double Arithmetic::GetResult() const noexcept { return result; }

// Returns the value of a specific variable
double Arithmetic::GetValVar(const std::string& name)
{
	if (tableVariable.find(name) == tableVariable.end())
		throw std::logic_error{ "This element is missing" };

	return tableVariable[name];
}

// Returns a constant reference to a table with variables and their values
const std::map<std::string, double>& Arithmetic::GetTableVar() const noexcept
{
	return tableVariable;
}




// +-+-+-+-+-+-+-+-+ Setter +-+-+-+-+-+-+-+-+

// Changes the value of a variable from the table
void Arithmetic::SetValVar(std::string name, double value)
{
	if (tableVariable.find(name) == tableVariable.end())
		throw std::logic_error{ "This element is missing" };

	tableVariable[name] = value;
}




// +-+-+-+-+-+-+-+-+ Other methods +-+-+-+-+-+-+-+-+

// Calculates the result of the expression
void Arithmetic::Calculation()
{
	std::stack<double> stVariable;
	double left{};
	double right{};

	for (const auto& token : postForm)
	{
		// If the token is a variable or constant
		if (token.type == TypeToken::CONST || token.type == TypeToken::VARIABLE)
		{
			stVariable.push(tableVariable[token.name]);
		}
		// If the token is an operation
		else if (token.type == TypeToken::OPERATION)
		{
			// If the operation is unary
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
			// Otherwise, the operation is binary
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
					// We throw an exception if division by zero occurs
					if (right == 0)
						throw ExceptionRecord{ CodeError::ZERO_DIVISION, "Division by zero" };

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

// Outputs variables and their values to the console (logs)
void Arithmetic::ShowTableVar() const noexcept
{
	std::cout << "size tableVariable = " << tableVariable.size() << '\n';
	for (const auto& elem : tableVariable)
	{
		std::cout << elem.first << ' ' << elem.second << '\n';
	}
}




// +-+-+-+-+-+-+-+-+ Operators +-+-+-+-+-+-+-+-+

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



