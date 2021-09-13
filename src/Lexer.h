#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <map>
#include <functional>

enum class LexemType
{
	Unknown,
	Register32Bit,
	Register8Bit,
	SegmentRegister16Bit,
	UserIdentifier,
	BinaryConstant8Bit,
	BinaryConstant16Bit,
	BinaryConstant32Bit,
	DecimalConstant8Bit,
	DecimalConstant16Bit,
	DecimalConstant32Bit,
	HexConstant8Bit,
	HexConstant16Bit,
	HexConstant32Bit,
	Command,
	Separator,
	OpenSquareBracket,
	CloseSquareBracket,
	OpenBracket,
	CloseBracket,
	Coma,
	Ñolon,
	Ptr,
	ByteType,
	DwordType,
	DataDerective,
	SegmentStart,
	SegmentEnd,
	ProgramEnd,
	Plus,
	Minus,
	Multi,
	Division,
	Range,
};
std::string LexemTypeToString(LexemType type);
static bool IsHex(LexemType t)
{
	return t == LexemType::HexConstant32Bit || t == LexemType::HexConstant8Bit || t == LexemType::HexConstant16Bit;
}
static bool IsBin(LexemType t)
{
	return t == LexemType::BinaryConstant32Bit || t == LexemType::BinaryConstant8Bit || t == LexemType::BinaryConstant16Bit;
}
static bool IsDec(LexemType t)
{
	return t == LexemType::DecimalConstant32Bit || t == LexemType::DecimalConstant8Bit || t == LexemType::DecimalConstant16Bit;
}
static bool IsImm(LexemType t)
{
	return IsHex(t) || IsDec(t) || IsBin(t);
}
class LexerException : public std::exception
{
public:
	LexerException(std::string s)
		:m_Error(s) {};
	const char* what() const throw () {
		return m_Error.c_str();
	}
private:
	std::string m_Error;
};
class Lexer
{
public:
	struct LexemTableRow
	{
		LexemTableRow(const std::string& name, const std::string& desc = "")
			:Name(name), Size(name.size()), Description(desc)
		{
			std::transform(Name.begin(), Name.end(), Name.begin(), ::toupper);
		};
		LexemTableRow() = default;
		LexemTableRow(std::string&, uint8_t, std::string&) = delete;
		std::string Name;
		LexemType Type = LexemType::Unknown;
		uint8_t Size = 0;
		std::string Description;
	};
	using LexemTable = std::vector<LexemTableRow>;
public:
	Lexer() = default;
	void PushLine(const std::string& line,const std::map<std::string, LexemType>& tokens);
	void SetUserIdentifierRule(std::function<bool(const std::string&)> p) { m_UserIdentifierRule = p; };
	void SetDefaultUserIdentifierRule() { m_UserIdentifierRule = [](const std::string&) { return false; }; };
	void SetException(bool b) { m_Exception = b; };
	std::function<bool(const std::string&)> GetDefaultUserIdentifierRule() const { return m_UserIdentifierRule; }
	const LexemTable& GetCurrentTable() const { return m_Table; };
	static LexemType TypeOfConstant(std::string& constant);
private:
	std::string::const_iterator p_SkipSpaces(const std::string::const_iterator it ,const std::string::const_iterator end);
private:
	std::function<bool(const std::string&)> m_UserIdentifierRule = [](const std::string&) { return false; };
	bool m_Exception = false;
	char m_EndSymbol = ';';
	//std::array<char, 9> m_SpecialSymbols{'-','+','*',':' , ',' , '[' , ']', '(' ,')'};
	LexemTable m_Table;
};