#pragma once
#include <cstdint>
#include <string>
#include <array>
#include "../../Parser.h"
struct BaseIndex
{
	BaseIndex(Parser::LexemIterator b, Parser::LexemIterator end)
	{
		auto base_it = std::find_if(b, end, [](auto t) {return t.Type == LexemType::Register32Bit; });
		auto index_it = std::find_if(base_it + 1, end, [](auto t) {return t.Type == LexemType::Register32Bit; });
		auto seg_it = std::find_if(b, end, [](auto t) {return t.Type == LexemType::SegmentRegister16Bit; });
		if (seg_it != end)
			Segment = seg_it->Name;
		if (base_it != end)
			Base = base_it->Name;
		if (index_it != end)
			Index = index_it->Name;
		if (Base == "ESP" && Index == "ESP") throw RawParserException("not a valid base/index expression");
	}
	std::string Base;
	std::string Index;
	uint8_t Scaler = 1;
	std::string Segment;
};
class IntelCodeGenerator
{	
public:
	static void GenerateSib(std::vector<uint8_t>& code, uint8_t modrm, uint8_t opcode, BaseIndex& baseindex);
};