#ifndef EXCEPTION_RECORD_HPP
#define EXCEPTION_RECORD_HPP


#include <stdexcept>


// Перечисление с кодами возможных ошибок
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

// Класс исключений для класса Record
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
