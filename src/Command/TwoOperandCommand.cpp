#include "TwoOperandCommand.h"
#include "../SupportedTokens.h"


void TwoOperandCommand::p_Process()
{
	if (m_Data.Prefix)
	{
		m_Code.push_back(0x0F);
	}
	if (DataManager::Get().CompareRange("R32", m_Data.Operands[0]) && DataManager::Get().CompareRange("R32", m_Data.Operands[1]))
	{
		m_Code.push_back(m_Data.Opcode);
		uint8_t modrm = 0;
		modrm |= 0b11 << 6;
		modrm |= Supported::REGISTER_NUMS.find(m_First_Op_Begin->Name)->second << 3;
		modrm |= Supported::REGISTER_NUMS.find(m_Second_Op_Begin->Name)->second;
		m_Code.push_back(modrm);
	}
	else if (DataManager::Get().CompareRange("R8", m_Data.Operands[0]) &&
		(DataManager::Get().CompareRange("MEM32", m_Data.Operands[1]) || DataManager::Get().CompareRange("MEM32_8", m_Data.Operands[1]))) //TODO merge this branch
	{
		uint8_t modrm = 0;
		modrm |= Supported::REGISTER_NUMS.find(m_First_Op_Begin->Name)->second << 3;
		BaseIndex bi(m_Second_Op_Begin, m_End);
		IntelCodeGenerator::GenerateSib(m_Code, modrm, m_Data.Opcode, bi);
	}
	else if (DataManager::Get().CompareRange("R32", m_Data.Operands[0])
		&& (DataManager::Get().CompareRange("MEM32", m_Data.Operands[1]) || DataManager::Get().CompareRange("MEM32_32", m_Data.Operands[1])))
	{
		uint8_t modrm = 0;
		modrm |= Supported::REGISTER_NUMS.find(m_First_Op_Begin->Name)->second << 3;
		BaseIndex bi(m_Second_Op_Begin, m_End);
		IntelCodeGenerator::GenerateSib(m_Code, modrm, m_Data.Opcode, bi);
	}
	else if (DataManager::Get().CompareRange("R32", m_Data.Operands[0]) && DataManager::Get().CompareRange("MEM32_8", m_Data.Operands[1]))
	{
		auto reg_num = Supported::REGISTER_NUMS.find(m_First_Op_Begin->Name)->second;
		if (reg_num < 4)
		{
			uint8_t modrm = 0;
			modrm |= reg_num << 3;
			BaseIndex bi(m_Second_Op_Begin, m_End);
			IntelCodeGenerator::GenerateSib(m_Code, modrm, m_Data.Opcode, bi);
		}
		else
		{
			throw RawParserException("size of operands not match"); //TODO error
		}
	}
	else if (DataManager::Get().CompareRange("R8", m_Data.Operands[1]) &&
		(DataManager::Get().CompareRange("MEM32", m_Data.Operands[0]) || DataManager::Get().CompareRange("MEM32_8", m_Data.Operands[0]))) //TODO merge this branch
	{
		uint8_t modrm = 0;

		modrm |= Supported::REGISTER_NUMS.find(m_Second_Op_Begin->Name)->second << 3;
		BaseIndex bi(m_First_Op_Begin, m_Second_Op_Begin);
		IntelCodeGenerator::GenerateSib(m_Code, modrm, m_Data.Opcode, bi);
	}
	else if (DataManager::Get().CompareRange("R32", m_Data.Operands[1])
		&& (DataManager::Get().CompareRange("MEM32", m_Data.Operands[0]) || DataManager::Get().CompareRange("MEM32_32", m_Data.Operands[0])))
	{
		uint8_t modrm = 0;
		modrm |= Supported::REGISTER_NUMS.find(m_Second_Op_Begin->Name)->second << 3;
		BaseIndex bi(m_First_Op_Begin, m_Second_Op_Begin);
		IntelCodeGenerator::GenerateSib(m_Code, modrm, m_Data.Opcode, bi);
	}
	else if (DataManager::Get().CompareRange("R32", m_Data.Operands[1]) && DataManager::Get().CompareRange("MEM32_8", m_Data.Operands[0]))
	{
		auto reg_num = Supported::REGISTER_NUMS.find(m_Second_Op_Begin->Name)->second;
		if (reg_num < 4)
		{
			uint8_t modrm = 0;
			modrm |= reg_num << 3;
			BaseIndex bi(m_First_Op_Begin, m_Second_Op_Begin);
			IntelCodeGenerator::GenerateSib(m_Code, modrm, m_Data.Opcode, bi);
		}
		else
		{
			throw RawParserException("size of operands not match"); //TODO error
		}
	}
	else if (DataManager::Get().CompareRange("MEM32_32", m_Data.Operands[0]) && DataManager::Get().SubRange("IMM", m_Data.Operands[1]))
	{
		BaseIndex bi(m_First_Op_Begin, m_End);
		IntelCodeGenerator::GenerateSib(m_Code, 0, m_Data.Opcode, bi);

		long long num = Parser::ToNum(*m_Second_Op_Begin);
		for (int i = 3; i >= 0; i--)
		{
			m_Code.push_back((uint8_t)(num>>8*i));
			//num >>= 8;
		}
	}
	else if (DataManager::Get().CompareRange("MEM32_8", m_Data.Operands[0]) && DataManager::Get().SubRange("IMM", m_Data.Operands[1]))
	{
		BaseIndex bi(m_First_Op_Begin, m_End);
		IntelCodeGenerator::GenerateSib(m_Code, 0, m_Data.Opcode, bi);
		int num = Parser::ToNum(*m_Second_Op_Begin);
		m_Code.push_back((uint8_t)num);
	}
}
