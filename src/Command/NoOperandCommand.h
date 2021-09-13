#pragma once
#include <array>
#include "Mnemonic.h"
#include "../SupportedTokens.h"

class NoOperandCommand : public Mnemonic
{
public:
	NoOperandCommand() = delete;
	NoOperandCommand(const DataManager::CommandData& data);
	virtual uint8_t GetSize() const override { return m_Code.size(); };
	virtual const std::vector<uint8_t>& GetByteCode() override { return m_Code; };
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) override {};
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) override {};
private:
	std::vector<uint8_t> m_Code;
};