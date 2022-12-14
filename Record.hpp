//#ifndef RECORD_HPP
//#define RECORD_HPP
//
//#include "Token.hpp"
//
//
//
//class Record
//{
//private:
//	std::string srcStr{};
//	std::vector<Token> tokens{};
//
//public:
//	Record() {}
//	Record(const std::string& str);
//	Record(const Record& other) : srcStr(other.srcStr), tokens{ other.tokens } {}
//
//
//	size_t GetCount() const noexcept;
//	std::string  GetSrcStr() const noexcept;
//
//
//	Record& operator = (const Record& other);
//	Record& operator = (const std::string& str);
//	Token& operator [] (const size_t& pos);
//};
//
//
//
//
//#endif // RECORD_HPP