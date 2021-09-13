#pragma once
#include "Lexer.h"
#include "DataManager.h"
#include <map>
#include <string>


using Layout = std::vector<std::vector<LexemType>>;


struct DataDerectivesData
{
	uint8_t Size;
	std::string LabelType;
};

class Supported
{
public:
	static const std::map<std::string, LexemType> TOKENS;
	static const std::map<std::string, uint8_t> REGISTER_NUMS;
	static const std::map<std::string, uint8_t> SEGMENT_REGISTER_CODES;
	static const std::map<std::string, LexemType> KEYWORDS;
	static const std::map<std::string, DataDerectivesData> DATA_DERECTIVES;
	static const std::map<std::string, LexemType> PRIVATE_TOKENS;
	static const Layout ASM_GRAMMATICS;
};
