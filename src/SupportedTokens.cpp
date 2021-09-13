#include "SupportedTokens.h"

const std::map<std::string, LexemType> Supported::TOKENS ={
	{"EAX",LexemType::Register32Bit },
	{"EBX",LexemType::Register32Bit },
	{"ECX",LexemType::Register32Bit },
	{"EDX",LexemType::Register32Bit },
	{"ESP",LexemType::Register32Bit },
	{"ESI",LexemType::Register32Bit },
	{"EDI",LexemType::Register32Bit },
	{"EBP",LexemType::Register32Bit },
	{"ES", LexemType::SegmentRegister16Bit },
	{"CS", LexemType::SegmentRegister16Bit },
	{"SS", LexemType::SegmentRegister16Bit },
	{"DS", LexemType::SegmentRegister16Bit },
	{"FS", LexemType::SegmentRegister16Bit },
	{"GS", LexemType::SegmentRegister16Bit },
	{"AH", LexemType::Register8Bit },
	{"AL", LexemType::Register8Bit },
	{"BH", LexemType::Register8Bit },
	{"BL", LexemType::Register8Bit },
	{"CH", LexemType::Register8Bit },
	{"CL", LexemType::Register8Bit },
	{"DH", LexemType::Register8Bit },
	{"DL", LexemType::Register8Bit },
	{"+",LexemType::Plus},
	{"-",LexemType::Minus},
	{"*",LexemType::Multi},
	{"/",LexemType::Division},
	{":",LexemType::Ñolon},
	{",",LexemType::Coma},
	{"[",LexemType::OpenSquareBracket},
	{"]",LexemType::CloseSquareBracket},
	{"(",LexemType::OpenBracket},
	{")",LexemType::CloseBracket},
	{"DB", LexemType::DataDerective},
	{"DW", LexemType::DataDerective},
	{ "DD",LexemType::DataDerective},
	{"SEGMENT",LexemType::SegmentStart},
	{"ENDS",LexemType::SegmentEnd},
	{"DWORD",LexemType::DwordType},
	{"BYTE", LexemType::ByteType},
	{"PTR", LexemType::Ptr},
	{"END", LexemType::ProgramEnd}
};
const std::map<std::string, LexemType> Supported::PRIVATE_TOKENS = {
	{"REG32",LexemType::Register32Bit},
	{"SEG",LexemType::SegmentRegister16Bit},
	{":",LexemType::Ñolon},
	{"REG8",LexemType::Register8Bit},
	{"OFFSET",LexemType::UserIdentifier},
	{"BIN8",LexemType::BinaryConstant8Bit},
	{"BIN16",LexemType::BinaryConstant16Bit},
	{"BIN32",LexemType::BinaryConstant32Bit},
	{"HEX8",LexemType::HexConstant8Bit},
	{"HEX16",LexemType::HexConstant16Bit},
	{"HEX32",LexemType::HexConstant32Bit},
	{"DEC8",LexemType::DecimalConstant8Bit},
	{"DEC16",LexemType::DecimalConstant16Bit},
	{"DEC32",LexemType::DecimalConstant32Bit},
	{"DWORD",LexemType::DwordType},
	{"BYTE", LexemType::ByteType},
	{"PTR", LexemType::Ptr},
	{"[",LexemType::OpenSquareBracket},
	{"]",LexemType::CloseSquareBracket},
	{"+",LexemType::Plus},
	{"-",LexemType::Minus},
	{"*",LexemType::Multi},
};

const std::map<std::string, uint8_t> Supported::REGISTER_NUMS = {
	{"EAX",0b000 },
	{"EBX",0b011 },
	{"ECX",0b001 },
	{"EDX",0b010 },
	{"EBP",0b101 },
	{"ESP",0b100 },
	{"ESI",0b110 },
	{"EDI",0b111 },
	{"ES", 0b000 },
	{"CS", 0b001 },
	{"SS", 0b010 },
	{"DS", 0b011 },
	{"FS", 0b100 },
	{"GS", 0b101 },
	{"AH", 0b100 },
	{"AL", 0b000 },
	{"BH", 0b111 },
	{"BL", 0b011 },
	{"CH", 0b101 },
	{"CL", 0b001 },
	{"DH", 0b110 },
	{"DL", 0b010 },
};
/*	 0x02E     CS:
     0x036     SS:
     0x03E     DS:
     0x026     ES:
     0x064     FS:
     0x065     Gs:*/
const std::map<std::string, uint8_t> Supported::SEGMENT_REGISTER_CODES = {
	{"ES", 0x026 },
	{"CS", 0x02E },
	{"SS", 0x036 },
	{"DS", 0x03E },
	{"FS", 0x064 },
	{"GS", 0x065 },
};
const std::map<std::string, DataDerectivesData> Supported::DATA_DERECTIVES = {
	{"DB", {1 ,"Byte"}},
	{"DW", {2 ,"Word"} },
	{ "DD",{4 ,"Dword"}},
};
const std::map <std::string, LexemType> Supported::KEYWORDS = {
	{"SEGMENT",LexemType::SegmentStart},
	{"END", LexemType::Unknown},
	{"ENDS",LexemType::SegmentEnd},
	{"DWORD",LexemType::DwordType},
	{"BYTE", LexemType::ByteType},
	{"PTR", LexemType::Ptr}
};
const Layout Supported::ASM_GRAMMATICS = {
	{LexemType::UserIdentifier,LexemType::SegmentStart},
	{LexemType::UserIdentifier,LexemType::SegmentEnd},
	{LexemType::UserIdentifier,LexemType::Ñolon},
	{LexemType::ProgramEnd},
};