#pragma once
#include "Mnemonic.h"
#include "Generators/IntelCodeGenerator.h"
#include "../SupportedTokens.h"
class OneOperandCommand : public Mnemonic
{
public:
	OneOperandCommand() = delete;
	OneOperandCommand(DataManager::CommandData data, Parser::LexemIterator b, Parser::LexemIterator e)
		:m_Code({}), m_Data(data), m_Begin(b), m_End(e) {};
	virtual uint8_t GetSize() const override { return m_Code.size(); };
	virtual const std::vector<uint8_t>& GetByteCode() override { return m_Code; };
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) override 
	{ m_DefaultOffsetMode = true; p_Process(); m_DefaultOffsetMode = false;};
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) override {  p_Process(); };
private:
	void p_Process();
private:
	std::vector<uint8_t> m_Code;
	bool m_DefaultOffsetMode = false;
	DataManager::CommandData m_Data;
	Parser::LexemIterator m_Begin;
	Parser::LexemIterator m_End;
};