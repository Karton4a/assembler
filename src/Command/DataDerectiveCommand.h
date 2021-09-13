#pragma once
#include "Mnemonic.h"
class DataDerectiveCommand : public Mnemonic 
{
public:
	DataDerectiveCommand(uint8_t size, const Parser::SyntaxTable& pars_table, const Lexer::LexemTable& lex_table)
		:m_CommandSize(size), m_STable(pars_table), m_LTable(lex_table) {};
	virtual uint8_t GetSize() const override { return m_Size; };
	virtual const std::vector<uint8_t>& GetByteCode() override { return m_Code; };
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) override;
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) override;
private:
	uint8_t m_Size = 0;
	std::vector<uint8_t> m_Code;
	const Parser::SyntaxTable& m_STable;
	const Lexer::LexemTable& m_LTable;
	uint8_t m_CommandSize = 0;
};