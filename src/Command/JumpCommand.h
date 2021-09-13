#pragma once
#include "Mnemonic.h"
class JumpCommand : public Mnemonic
{
public:
	JumpCommand(const std::string& name, Parser::LexemIterator b, Parser::LexemIterator e,int op_type)
		:m_Code({}),m_Name(name), m_LexTable(b, e), m_OpBegin(m_LexTable.begin()), m_OpEnd(m_LexTable.end()),m_OpType(op_type) {};
	virtual uint8_t GetSize() const override { return m_Code.size(); };
	virtual const std::vector<uint8_t>& GetByteCode() override { return m_Code; };
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) override;
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) override;
private:
	void p_Process(const Parser::SegmentData& data,const Parser::LabelTable& offset_table, uint8_t command_size = 0);
private:
	std::vector<uint8_t> m_Code;
	std::string m_Name;
	Lexer::LexemTable m_LexTable;
	Parser::LexemIterator m_OpBegin;
	Parser::LexemIterator m_OpEnd;
	int m_OpType = -1;
	bool m_DefaultOffsetMode = false;
};