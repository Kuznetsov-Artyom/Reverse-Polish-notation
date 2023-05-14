#ifndef EXCEPTION_RECORD_HPP
#define EXCEPTION_RECORD_HPP


#include <stdexcept>


// Enumeration with possible error codes
enum class CodeError
{
	DEFAULT = 0,
	EMPTY_EXPRESSION = -1,
	TYPE_UNDEFINED = -2,
	INVALID_PARENTHESES = -3,
	NO_OPERATOR = -4,
	NO_OPERAND = -5,
	EMPTY_PARENTHESES = -6,
	ZERO_DIVISION = -7,
};

// Exception class for the Record class
class ExceptionRecord : public std::exception
{
private:
	CodeError code = CodeError::DEFAULT;

public:
	ExceptionRecord(CodeError codeError, const char* msgError) : exception{ msgError }
	{
		code = codeError;
	}

	int GetCodeError() const noexcept { return static_cast<int>(code); };
};


#endif // EXCEPTION_RECORD_HPP
