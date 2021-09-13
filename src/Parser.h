#pragma once
#include "Lexer.h"
#include "DataManager.h"
#include <vector>
#include <map>
#include <stack>

class ParserException : public std::exception
{
public:
	ParserException(std::string s)
		:m_Error(s) {};
	const char* what() const throw () {
		return m_Error.c_str();
	}
protected:
	std::string m_Error;
};
class CriticalParserException : public ParserException
{
public:
	CriticalParserException(std::string s)
		:ParserException(s) {};
};
class RawParserException : public ParserException
{
public:
	RawParserException(std::string s)
		:ParserException(s) {};
};

class Parser
{
public:
	struct SyntaxTableColumn
	{
		uint8_t LexemPosition;
		uint8_t LexemCount;
		int OperandType = -1;
	};
	using SyntaxTable = std::vector<SyntaxTableColumn>;
	struct SegmentData
	{
		std::string Name;
		uint8_t BitDepth = 32;
		uint32_t CurrentOffsert = 0;
		uint32_t Size = 0;
	};
	using LexemIterator = Lexer::LexemTable::const_iterator;
	struct LabelParameters
	{
		std::string Type;
		std::string SegmentName;
		uint32_t Offset;
	};
	using LabelTable = std::map<std::string, LabelParameters>;
	using SegmentTable = std::array<SegmentData, 2>;
public:
	Parser();
	void ParseLexemTable(const Lexer::LexemTable& lex_table);
	uint8_t FirstPass(Lexer::LexemTable lex_table);
	std::vector<uint8_t> SecondPass(Lexer::LexemTable lex_table, uint32_t op_size);
	const SyntaxTable& GetSyntaxTable() const { return m_Table; }
	uint32_t GetCurrentOffset() const;
	void ClearSegmentTable();
	void ClearEndStatus() { m_ProgramEnd = false; }
	bool GetEndStatus() const { return m_ProgramEnd; }
	const LabelTable& GetLabelTable() const { return m_LabelTable; };
	const SegmentTable& GetSegmentTable() const { return m_SegmentTable; }
	const std::map<std::string, std::string>& GetSegmentDestinationTable() const { return m_SegmentDestinationTable; }
	uint8_t GetSegmentCount() const { return m_CreatedSegments; }
	static long long ToNum(const Lexer::LexemTableRow& row);
	static long long CalculateEquasion(LexemIterator begin, LexemIterator end);
private:
	static long long p_CalculateOperation(std::stack<long long>& data_stack, std::string& op);
	static int p_OperatorPriority(const std::string& op);
	void p_AddLabel(const Lexer::LexemTable& lex_table);
	void p_ProccesAsmCommands(const Lexer::LexemTable& lex_table);
	void p_SetOperandsType(Lexer::LexemTable& lex_table);
private:
	SyntaxTable m_Table;
	LabelTable m_LabelTable;
	SegmentTable m_SegmentTable;
	uint8_t m_CreatedSegments = 0;
	bool m_ProgramEnd = false;
	std::map<std::string, std::string> m_SegmentDestinationTable;
	decltype(m_SegmentTable)::iterator m_ActiveSegment = m_SegmentTable.end();
};