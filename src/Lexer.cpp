#include "Lexer.h"
#include "SupportedTokens.h"
#include <regex>

void Lexer::PushLine(const std::string& line,const std::map<std::string, LexemType>& tokens)
{
	m_Table.clear();
	auto it = p_SkipSpaces(line.begin(),line.end());
	auto start = it;
	std::string token;
	while (it != line.end() && *it != m_EndSymbol)
	{	
		if(std::isspace(*it))
		{
			token.clear();
			token.append(start,it);
			if(!token.empty())
				m_Table.emplace_back(token);
			it = p_SkipSpaces(it,line.end());
			start = it;
			continue;
		}
		token = *it;
		if(tokens.find(token) != tokens.end())
		{
			token.clear();
			token.append(start,it);
			if(!token.empty())
				m_Table.emplace_back(token);
			token = *it;
			m_Table.emplace_back(token);
			start = it + 1;
		}
		it++;
	}
	token.clear();
	token.append(start,it);
	if(!token.empty())
		m_Table.emplace_back(token);
	for (auto& row : m_Table)
	{
		auto token_it = tokens.find(row.Name);
		if (token_it != tokens.end())
		{
			auto& [name, type] = *token_it;
			row.Type = type;
		}
		else
		{
			using namespace std::string_literals;
			row.Type = TypeOfConstant(row.Name);
			if (row.Type == LexemType::Unknown && m_UserIdentifierRule(row.Name))
			{
				row.Type = LexemType::UserIdentifier;
			}
			if(m_Exception && row.Type == LexemType::Unknown)  throw LexerException("unknown identifier "s + row.Name + " ");
		}
	}
}

std::string::const_iterator Lexer::p_SkipSpaces(std::string::const_iterator it, const std::string::const_iterator end)
{
	auto local = it;
	while (local != end && (std::isspace(*local) || *local == '\t')) { local++; }
	return local;
}

LexemType Lexer::TypeOfConstant(std::string& constant)
{
	if(constant.empty()) return LexemType::Unknown;
	if (constant.back() == 'H' && constant.size() == 1) return LexemType::Unknown;
	if (constant.back() == 'B' && constant.size() == 1) return LexemType::Unknown;
	if (constant.back() == 'D' && constant.size() == 1) return LexemType::Unknown;
	std::regex decimal("^[-+]?[0-9]+[D]?$");
	//std::regex decimal("^[0-9]+[D]?$");
	if(std::regex_match(constant,decimal))
	{
		auto num = std::stoll(constant);
		if(num >= std::numeric_limits<uint8_t>::min() && num <= std::numeric_limits<uint8_t>::max()
			|| num >= std::numeric_limits<int8_t>::min() && num <= std::numeric_limits<int8_t>::max())
			return LexemType::DecimalConstant8Bit;
		if (num >= std::numeric_limits<uint16_t>::min() && num <= std::numeric_limits<uint16_t>::max()
			|| num >= std::numeric_limits<int16_t>::min() && num <= std::numeric_limits<int16_t>::max())
			return LexemType::DecimalConstant16Bit;
		if (num >= std::numeric_limits<uint32_t>::min() && num <= std::numeric_limits<uint32_t>::max()
			|| num >= std::numeric_limits<int32_t>::min() && num <= std::numeric_limits<int32_t>::max())
			return LexemType::DecimalConstant32Bit;
	}
	if(constant.back() == 'B' && 
		std::all_of(constant.begin(),constant.end()-1,[](auto c){ return c == '1' || c == '0'; }))
	{
		auto num = std::stoll(constant,0,2);
		if (num >= std::numeric_limits<uint8_t>::min() && num <= std::numeric_limits<uint8_t>::max()
			|| num >= std::numeric_limits<int8_t>::min() && num <= std::numeric_limits<int8_t>::max())
			return LexemType::BinaryConstant8Bit;
		if (num >= std::numeric_limits<uint16_t>::min() && num <= std::numeric_limits<uint16_t>::max()
			|| num >= std::numeric_limits<int16_t>::min() && num <= std::numeric_limits<int16_t>::max())
			return LexemType::BinaryConstant16Bit;
		if (num >= std::numeric_limits<uint32_t>::min() && num <= std::numeric_limits<uint32_t>::max()
			|| num >= std::numeric_limits<int32_t>::min() && num <= std::numeric_limits<int32_t>::max())
			return LexemType::BinaryConstant32Bit;
	}
	if(constant.back() == 'H' 
		&& std::all_of(constant.begin(),constant.end()-1,[](auto c) { return ::isdigit(c) || (c >= 'A' && c <= 'F');}))
	{
		auto num = std::stoll(constant, 0, 16);
		if (num >= std::numeric_limits<uint8_t>::min() && num <= std::numeric_limits<uint8_t>::max()
			|| num >= std::numeric_limits<int8_t>::min() && num <= std::numeric_limits<int8_t>::max())
			return LexemType::HexConstant8Bit;
		if (num >= std::numeric_limits<uint16_t>::min() && num <= std::numeric_limits<uint16_t>::max()
			|| num >= std::numeric_limits<int16_t>::min() && num <= std::numeric_limits<int16_t>::max())
			return LexemType::HexConstant16Bit;
		if (num >= std::numeric_limits<uint32_t>::min() && num <= std::numeric_limits<uint32_t>::max()
			|| num >= std::numeric_limits<int32_t>::min() && num <= std::numeric_limits<int32_t>::max())
			return LexemType::HexConstant32Bit;
	}
	return LexemType::Unknown;

}

std::string LexemTypeToString(LexemType type)
{
	switch (type)
	{
	case LexemType::Unknown:
		return "Unknown";
	case LexemType::Register32Bit:
		return "Register32Bit";
	case LexemType::Register8Bit:
		return "Register8Bit";
	case LexemType::SegmentRegister16Bit:
		return "SegmentRegister16Bit";
	case LexemType::UserIdentifier:
		return "UserIdentifier";
	case LexemType::BinaryConstant8Bit:
		return "BinaryConstant8Bit";
	case LexemType::BinaryConstant16Bit:
		return "BinaryConstant16Bit";
	case LexemType::BinaryConstant32Bit:
		return "BinaryConstant32Bit";
	case LexemType::DecimalConstant8Bit:
		return "DecimalConstant8Bit";
	case LexemType::DecimalConstant16Bit:
		return "DecimalConstant16Bit";
	case LexemType::DecimalConstant32Bit:
		return "DecimalConstant32Bit";
	case LexemType::HexConstant8Bit:
		return "HexConstant8Bit";
	case LexemType::HexConstant16Bit:
		return "HexConstant16Bit";
	case LexemType::HexConstant32Bit:
		return "HexConstant32Bit";
	case LexemType::Command:
		return "Command";
	case LexemType::Separator:
		return "Separator";
	case LexemType::OpenSquareBracket:
		return "OpenSquareBracket";
	case LexemType::CloseSquareBracket:
		return "CloseSquareBracket";
	case LexemType::OpenBracket:
		return "OpenBracket";
	case LexemType::CloseBracket:
		return "CloseBracket";
	case LexemType::Coma:
		return "Coma";
	case LexemType::Ñolon:
		return "Ñolon";
	case LexemType::Ptr:
		return "Ptr";
	case LexemType::ByteType:
		return "ByteType";
	case LexemType::DwordType:
		return "DwordType";
	case LexemType::DataDerective:
		return "DataDerective";
	case LexemType::SegmentStart:
		return "SegmentStart";
	case LexemType::SegmentEnd:
		return "SegmentEnd";
	case LexemType::ProgramEnd:
		return "ProgramEnd";
	case LexemType::Plus:
		return "Plus";
	case LexemType::Minus:
		return "Minus";
	case LexemType::Multi:
		return "Multi";
	case LexemType::Division:
		return "Division";
	case LexemType::Range:
		return "Range";
	default:
		break;
	}
}
