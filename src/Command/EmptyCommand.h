#pragma once
#include "Mnemonic.h"
class EmptyCommand : public Mnemonic
{
	virtual uint8_t GetSize() const override { return 0; };
	virtual const std::vector<uint8_t>& GetByteCode() { return {}; };
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) {};
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) {};
};