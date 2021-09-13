#include "DataDerectiveCommand.h"
void DataDerectiveCommand::CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table)
{
	m_Size = (m_STable.size() - 1) * m_CommandSize;
}

void DataDerectiveCommand::GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size)
{
	//DATA_DERECTIVES
	for (auto it = m_STable.begin() + 1; it != m_STable.end(); it++)
	{
		auto lexem_begin = m_LTable.begin() + it->LexemPosition;
		auto lexem_end = lexem_begin + it->LexemCount;

		auto num = Parser::ToNum(*lexem_begin);
		/*for (size_t i = 0; i < m_CommandSize; i++)
		{
			m_Code.push_back((uint8_t)num);
			num >>= 8;
		}*/
		for (int i = m_CommandSize - 1; i >= 0; i--)
		{
			m_Code.push_back((uint8_t)(num >> 8 * i));
			//num >>= 8;
		}
	}
	m_Size = m_Code.size();
}
