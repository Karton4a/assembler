#pragma once
#include "Mnemonic.h"
class ErrorComand : public Mnemonic
{
public:
	ErrorComand(const std::string& name)
		:m_Name(name) {};

	virtual uint8_t GetSize() const override { return m_Code.size(); };
	virtual const std::vector<uint8_t>& GetByteCode() override { return m_Code; };
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) { throw RawParserException(m_Name + " Bad arguments");};
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) { throw RawParserException(m_Name + " Bad arguments"); };
private:
	std::string m_Name;
	std::vector<uint8_t> m_Code;
};