#include "JumpCommand.h"



void JumpCommand::CalculateSize(const Parser::SegmentData& data, const Parser::LabelTable& offset_table)
{
	m_DefaultOffsetMode = true;
	p_Process(data, offset_table);
	m_DefaultOffsetMode = false;
}

void JumpCommand::GenerateCode(const Parser::SegmentData& data, const Parser::LabelTable& offset_table, uint8_t command_size)
{
	p_Process(data, offset_table, command_size);
}

void JumpCommand::p_Process(const Parser::SegmentData& data,const Parser::LabelTable& offset_table,uint8_t command_size)
{
	//Lexer::LexemTable local_table(op_begin, op_end);
	//auto user_identifier = std::find_if(local_table.begin(), local_table.end(),
	//	[](const auto& l) {return l.Type == LexemType::UserIdentifier; });
	//user_identifier->Name = "0";
	//user_identifier->Size = 1;
	//user_identifier->Type = LexemType::DecimalConstant8Bit;
	////TODO calculate
	if (auto command_data = DataManager::Get().GetAllCommandData(m_Name))
	{
		if (DataManager::Get().FindRange("PLAINLABEL")->Includes(m_OpType))
		{
			auto label = offset_table.find(m_OpBegin->Name);
			uint8_t offset_size = 1;
			long long offset_value = 0;
			if (label != offset_table.end())
			{
				if (label->second.SegmentName != data.Name) throw RawParserException("far jumps not allowed");
				if (label->second.Type != "Near") throw RawParserException("label type is not near");
				offset_value = (long long)label->second.Offset - (long long)(data.CurrentOffsert + 2);
				if (offset_value < -128 || offset_value > 127) //Todo check on more than 32 bit offset
				{
					offset_size = 4;
					offset_value -= offset_size;
				}
		
			}
			else if (m_DefaultOffsetMode)
			{
				offset_size = 4;
			}
			else
			{
				using namespace std::string_literals;
				throw RawParserException("undefined label "s + m_OpBegin->Name);
			}
			if (offset_size == 1)
			{
				if (command_data->at(0).Prefix)
					m_Code.push_back(0x0F);
				m_Code.push_back(command_data->at(0).Opcode);
			}
			else if (offset_size == 4)
			{
				if (command_data->at(1).Prefix)
					m_Code.push_back(0x0F);
				m_Code.push_back(command_data->at(1).Opcode);
			}
			auto delta = command_size - m_Code.size();
			if (command_size == 0) delta = offset_size;
			if (delta < offset_size) throw RawParserException("delta < offset_size");

			for (int i = offset_size-1; i >=0; i--)
			{
				m_Code.push_back((uint8_t)(offset_value >> 8*i));
			}
			for (size_t i = offset_size; i < delta; i++)
			{
				m_Code.push_back(0x90);
			}
		}
		else throw RawParserException(m_Name + " bad arguments");
	} else throw RawParserException(m_Name + " unknown command");
}


