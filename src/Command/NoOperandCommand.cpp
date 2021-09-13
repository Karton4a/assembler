#include "NoOperandCommand.h"

NoOperandCommand::NoOperandCommand(const DataManager::CommandData& data)
{
	if (data.Prefix)
		m_Code.push_back(0xF9);
	m_Code.push_back(data.Opcode);
}
