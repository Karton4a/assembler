#include "OneOperandCommand.h"

void OneOperandCommand::p_Process()
{
	m_Code.clear();
	if (m_Data.Prefix)
		m_Code.push_back(0x0F);
	if (DataManager::Get().CompareRange("MEM32", m_Data.Operands.front()) || DataManager::Get().CompareRange("MEM32_32", m_Data.Operands.front()))
	{
		BaseIndex bi(m_Begin, m_End);
		IntelCodeGenerator::GenerateSib(m_Code, 0, m_Data.Opcode, bi);
	}
	else if (DataManager::Get().CompareRange("R32", m_Data.Operands.front()))
	{
		auto code = m_Data.Opcode;
		auto& [n, num] = *Supported::REGISTER_NUMS.find(m_Begin->Name);
		code += num;
		m_Code.push_back(code);
	}
	else if (DataManager::Get().CompareRange("R8", m_Data.Operands.front()))
	{
		m_Code.push_back(m_Data.Opcode);
		uint8_t modrm = m_Data.ModRM;
		modrm |= 0b11 << 6;
		auto r = Supported::REGISTER_NUMS.find(m_Begin->Name)->second;
		modrm |= r;
		m_Code.push_back(modrm);
	}
}
