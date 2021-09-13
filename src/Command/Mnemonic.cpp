#include "Mnemonic.h"
#include "../SupportedTokens.h"
#include "JumpCommand.h"
#include "OneOperandCommand.h"
#include "TwoOperandCommand.h"
#include "NoOperandCommand.h"
#include "ShortTwoOpCommand.h"
#include "DataDerectiveCommand.h"
#include "ErrorComand.h"
std::shared_ptr<Mnemonic> Mnemonic::Create(LexemType type, const Lexer::LexemTable& l,const Parser::SyntaxTable& s, uint32_t offset)
{
	auto op_count = s.size() - 1;
	auto& mnemonic_name = l[s.front().LexemPosition].Name;
	if (type == LexemType::Command)
	{
		if (op_count == 0)
		{
			if (auto res = DataManager::Get().FindCommand(mnemonic_name))
			{
				return std::make_shared<NoOperandCommand>(*res);
			}
		}
		else if (op_count == 1)
		{
			auto op_it = s.begin() + 1;
			if (mnemonic_name.front() == 'J')
			{
				return std::make_shared<JumpCommand>(mnemonic_name, l.begin() + s[1].LexemPosition, l.end(), op_it->OperandType);
			}
			if (auto res = DataManager::Get().FindCommand(mnemonic_name, op_it->OperandType))
			{
				return std::make_shared<OneOperandCommand>(*res, l.begin() + s[1].LexemPosition, l.end());
			}
		}
		else if (op_count == 2)
		{
			auto op_it = s.begin() + 1;
			if (auto res = DataManager::Get().FindCommand(mnemonic_name, op_it->OperandType, (op_it+1)->OperandType))
			{
				if (res->Short)
				{
					return std::make_shared<ShortTwoOpCommand>(*res, l.begin() + s[1].LexemPosition, l.begin() + s[2].LexemPosition, l.end());
				}

				return std::make_shared<TwoOperandCommand>(*res, l.begin() + s[1].LexemPosition,l.begin() + s[2].LexemPosition, l.end());
			}
		}
	}
	else if(type == LexemType::DataDerective)
	{
		auto& data = Supported::DATA_DERECTIVES.find(mnemonic_name)->second;
		std::string type;
		if (op_count != 0)
		{

			bool isValid = true;
			std::string type;
			if (data.Size == 1)
				type = "IMM8";
			else if (data.Size == 2)
				type = "IMM16";
			else if (data.Size == 4)
				type = "IMM32";
			if (auto range = DataManager::Get().FindRange(type))
			{
				for (auto i = 1; i < s.size(); i++)
				{
					if (!range->Includes(s[i].OperandType))
					{
						isValid = false;
						break;
					}
				}
			}
			if(isValid)
				return std::make_shared<DataDerectiveCommand>(data.Size, s, l);
			//else throw Raw
		}
	}
	return std::make_shared<ErrorComand>(mnemonic_name);
};