#ifndef RECORD_HPP
#define RECORD_HPP


#include "Token.hpp"
#include "ExceptionRecord.hpp"




// Checks for the correctness of parentheses
bool CheckingParentheses(const std::string& str);




class Record
{
private:
	std::string srcStr{};
	std::vector<std::string> strTokens;
	std::vector<Token> tokens{};

private:	
	// Splits the source string into tokens
	void SplitOnTokens(const std::string& str);

	// Defines the types of tokens
	void DefiningTypes();

	// Checks the correctness of the entered expression
	void CheckingCorrect();

public:
	// +-+-+-+-+-+-+-+-+ Constructors +-+-+-+-+-+-+-+-+

	Record() {}
	Record(const std::string& str);
	Record(const Record& other);



	// +-+-+-+-+-+-+-+-+ Methods (getters) +-+-+-+-+-+-+-+-+
	 
	// Returns the number of tokens in the record
	size_t GetCount() const noexcept;

	// Returns the original passed string
	std::string  GetSrcStr() const noexcept;




	// +-+-+-+-+-+-+-+-+ Operators +-+-+-+-+-+-+-+-+

	Record& operator = (const Record& other);
	Record& operator = (const std::string& str);
	Token& operator [] (const size_t& pos);
};

#endif // RECORD_HPP