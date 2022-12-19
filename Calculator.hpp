#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP


#include <iostream>
#include "Record.hpp"
#include <map>
#include <stack>
#include <cmath>



class Calculator
{
private:
	Record tokens;
	std::map<std::string, double> tableVariable;
	std::vector<Token> postForm;
	std::string polStr;
	double result;

private:
	// +-+-+-+-+-+-+-+-+ Приватные методы +-+-+-+-+-+-+-+-+
	 
	// Генерирует поствиксную запись лексем
	void GenerationPostForm();

	// Копирует другой объект типа Calculator
	void CopyOther(const Calculator& other);

public:
	// +-+-+-+-+-+-+-+-+ Конструкторы +-+-+-+-+-+-+-+-+

	Calculator(const std::string& str);
	Calculator(const Calculator& other);

	~Calculator() {}



	// +-+-+-+-+-+-+-+-+ Методы (геттеры) +-+-+-+-+-+-+-+-+
	
	// Возвращает "обратную польскую запись"
	std::string GetPolStr() const noexcept;

	// Возвращает исходную переданную строку
	std::string GetSrcStr() const noexcept;

	// Возвращает результат вычисления
	double GetResult() const noexcept;

	// Возвращает значение определенной переменной
	double GetValVar(const std::string& name);

	// Возвращает константкую ссылку на таблицу с переменными и их значениями
	const std::map<std::string, double>& GetTableVar() const noexcept;




	// +-+-+-+-+-+-+-+-+ Сеттер +-+-+-+-+-+-+-+-+
	
	// Изменяет значение переменной из таблицы
	void SetValVar(std::string name, double value);




	// +-+-+-+-+-+-+-+-+ Прочие методы +-+-+-+-+-+-+-+-+

	// Вычисляет результат выражения
	void Calculation();

	// Выводит переменные и их значения в консоль (логи)
	void ShowTableVar() const noexcept;
	



	// +-+-+-+-+-+-+-+-+ Операторы +-+-+-+-+-+-+-+-+

	Calculator& operator = (const Calculator& other);
	Calculator& operator = (const std::string& str);
};



#endif // !CALCULATOR_HPP
