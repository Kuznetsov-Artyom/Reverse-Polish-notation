#ifndef RECORD_HPP
#define RECORD_HPP


#include "Token.hpp"
#include "ExceptionRecord.hpp"




// Проверяет на корректность круглые скобки
bool CheckingParentheses(const std::string& str);




class Record
{
private:
	std::string srcStr{};
	std::vector<std::string> strTokens;
	std::vector<Token> tokens{};

private:
	// +-+-+-+-+-+-+-+-+ Приватные методы +-+-+-+-+-+-+-+-+
	
	// Разделяет исходную строку на лексемы
	void SplitOnTokens(const std::string& str);

	// Определяет типы лексем
	void DefiningTypes();

	// Проверяет корректность введенного выражения
	void CheckingCorrect();

public:
	// +-+-+-+-+-+-+-+-+ Конструкторы +-+-+-+-+-+-+-+-+

	Record() {}
	Record(const std::string& str);
	Record(const Record& other);



	// +-+-+-+-+-+-+-+-+ Методы (геттеры) +-+-+-+-+-+-+-+-+
	 
	// Возвращает количество лексем в записи
	size_t GetCount() const noexcept;

	// Возвращает исходную переданную строку
	std::string  GetSrcStr() const noexcept;




	// +-+-+-+-+-+-+-+-+ Операторы +-+-+-+-+-+-+-+-+

	Record& operator = (const Record& other);
	Record& operator = (const std::string& str);
	Token& operator [] (const size_t& pos);
};




#endif // RECORD_HPP