#include "ShortTwoOpCommand.h"
#include "../SupportedTokens.h"
#include "Generators/IntelCodeGenerator.h"
void ShortTwoOpCommand::p_Process()
{
	if (m_Data.Prefix)
	{
		m_Code.push_back(0x0F);
	}
	if ((DataManager::Get().CompareRange("R32", m_Data.Operands[0]) || (DataManager::Get().CompareRange("R8", m_Data.Operands[0])))
		&& DataManager::Get().SubRange("IMM", m_Data.Operands[1]))
	{
		auto opcode = m_Data.Opcode;
		opcode += Supported::REGISTER_NUMS.find(m_First_Op_Begin->Name)->second;
		m_Code.push_back(opcode);
		size_t byte_count = DataManager::Get().CompareRange("R32", m_Data.Operands[0]) ? 4 : 1;
		long long num = Parser::ToNum(*m_Second_Op_Begin);
		/*for (size_t i = 0; i < byte_count; i++)
		{
			m_Code.push_back((uint8_t)num);
			num >>= 8;
		}*/
		for (int i = byte_count - 1; i >= 0; i--)
		{
			m_Code.push_back((uint8_t)(num >> 8 * i));
			//num >>= 8;
		}
	}
	else if (DataManager::Get().CompareRange("MEM32_32", m_Data.Operands[0]) && DataManager::Get().CompareRange("IMM8", m_Data.Operands[1]))
	{
		BaseIndex bi(m_First_Op_Begin, m_Second_Op_Begin);
		IntelCodeGenerator::GenerateSib(m_Code, m_Data.ModRM, m_Data.Opcode, bi);
		auto num = Parser::ToNum(*m_Second_Op_Begin);
		m_Code.push_back(num);
	}
}
