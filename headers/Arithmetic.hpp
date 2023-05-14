#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP


#include <iostream>
#include "Record.hpp"
#include <map>
#include <stack>
#include <cmath>



class Arithmetic
{
private:
	Record tokens;
	std::map<std::string, double> tableVariable;
	std::vector<Token> postForm;
	std::string polStr{};
	double result{};

private:

	// Generates a postfix entry of tokens
	void GenerationPostForm();

	// Copies another object of the Arithmetic type
	void CopyOther(const Arithmetic& other);

public:
	// +-+-+-+-+-+-+-+-+ Constructors +-+-+-+-+-+-+-+-+

	Arithmetic() {}
	Arithmetic(const std::string& str);
	Arithmetic(const Arithmetic& other);

	~Arithmetic() {}



	// +-+-+-+-+-+-+-+-+ Methods (getters) +-+-+-+-+-+-+-+-+

	// Returns "reverse Polish entry"
	std::string GetPolStr() const noexcept;

	// Returns the original passed string
	std::string GetSrcStr() const noexcept;

	// Returns the result of the calculation
	double GetResult() const noexcept;

	// Returns the value of a specific variable
	double GetValVar(const std::string& name);

	// Returns a constant reference to a table with variables and their values
	const std::map<std::string, double>& GetTableVar() const noexcept;




	// +-+-+-+-+-+-+-+-+ Setter +-+-+-+-+-+-+-+-+

	// Changes the value of a variable from the table
	void SetValVar(std::string name, double value);




	// +-+-+-+-+-+-+-+-+ Other methods +-+-+-+-+-+-+-+-+

	// Calculates the result of the expression
	void Calculation();

	// Outputs variables and their values to the console (logs)
	void ShowTableVar() const noexcept;




	// +-+-+-+-+-+-+-+-+ Operators +-+-+-+-+-+-+-+-+

	Arithmetic& operator = (const Arithmetic& other);
	Arithmetic& operator = (const std::string& str);
};


#endif // CALCULATOR_HPP
