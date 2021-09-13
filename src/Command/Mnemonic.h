#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "../Lexer.h"
#include "../Parser.h"
#include "../DataManager.h"

class Mnemonic
{
public:
	virtual uint8_t GetSize() const = 0;
	virtual const std::vector<uint8_t>& GetByteCode() = 0;
	virtual void CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table) = 0;
	virtual void GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size) = 0;
	virtual ~Mnemonic() = default;
	static std::shared_ptr<Mnemonic> Create(LexemType type,const Lexer::LexemTable& t, const Parser::SyntaxTable& s, uint32_t offset);
};
