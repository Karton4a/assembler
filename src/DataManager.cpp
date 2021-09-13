#include "DataManager.h"
#include "SupportedTokens.h"
#include <fstream>
void DataManager::GenerateLayouts(const std::string& path)
{
	Lexer lex;
	std::ifstream infile(path);
	std::string line;
	while (std::getline(infile,line))
	{
		lex.PushLine(line, Supported::PRIVATE_TOKENS);
		const auto& table = lex.GetCurrentTable();
		if (table.size() == 2 && table[0].Type == LexemType::Unknown)
		{
			if (table[1].Name == "BEGIN")
			{
				m_Ranges.try_emplace(table[0].Name, m_Layouts.size(), m_Layouts.size() + 1);
			}
			else if(table[1].Name == "END")
			{
				auto it = m_Ranges.find(table[0].Name);
				if (it != m_Ranges.end())
				{
					it->second.m_To = m_Layouts.size() - 1;
				}
				else
				{
					static_assert(true,"LayoutManager Error Missing Begin");
				}
			}
		}
		else if(table.size()!= 0)
		{
			m_Layouts.push_back({});
			for (const auto& row : table)
			{
				m_Layouts.back().push_back(row.Type);
			}
		}
	}
	infile.close();
}

void DataManager::GenerateCommadsData(const std::string& path)
{
	Lexer lex;
	std::ifstream infile(path);
	std::string line;
	std::map<std::string, LexemType> lexems = { {"|",LexemType::Separator},{",",LexemType::Coma} };
	while (std::getline(infile,line))
	{
		lex.PushLine(line,lexems);
		auto table_it = lex.GetCurrentTable().begin();
		auto table_end = lex.GetCurrentTable().end();
		if (table_it == table_end) continue;
		if (table_it->Type != LexemType::Unknown) throw std::runtime_error("GenerateCommadsData Syntax Error");
		auto [element,status] = m_Comands.emplace(table_it->Name, std::vector<CommandData>{});
		using withoutconst = std::remove_const <decltype(Supported::TOKENS)>::type;
		const_cast<withoutconst*>(&Supported::TOKENS)->emplace(table_it->Name,LexemType::Command);
		table_it++;
		CommandData data;
		while (table_it->Type != LexemType::Separator)
		{
			if (table_it->Type == LexemType::Coma)
			{
				table_it++;
				continue;
			}
			auto range_it = m_Ranges.find(table_it->Name);
			if (range_it == m_Ranges.end()) throw std::runtime_error("GenerateCommadsData No Such Range Error");
			data.Operands.push_back(&range_it->second);
			table_it++;
		}
		if (data.Operands.size() > 2) throw std::runtime_error("GenerateCommadsData More than 2 operands doesn't supported");
		table_it++;
		if (!IsHex(table_it->Type)) throw std::runtime_error("GenerateCommadsData Syntax Error");
		data.Opcode = std::stoi(table_it->Name,0,16);
		element->second.push_back(data);
		table_it++;
		if (table_it == table_end) continue;
		if (table_it->Type != LexemType::Separator) throw std::runtime_error("GenerateCommadsData Syntax Error");
		table_it++;
		while (table_it != table_end)
		{
			if (table_it->Size == 2 && table_it->Name[0] == '/')
			{
				element->second.back().ModRM = std::stoi(table_it->Name.data() + 1) << 3;
			}
			else if (table_it->Size == 1 && table_it->Name[0] == 'P')
			{
				element->second.back().Prefix = true;
			}
			else if (table_it->Size == 1 && table_it->Name[0] == 'S')
			{
				element->second.back().Short = true;
			}
			table_it++;
		}
	}
	infile.close();
}

std::optional<DataManager::CommandData> DataManager::FindCommand(const std::string& name) const
{
	auto it = m_Comands.find(name);
	if (it == m_Comands.end()) return std::nullopt;
	auto vec_begin = it->second.begin();
	auto vec_end = it->second.end();
	auto res_it = std::find_if(vec_begin, vec_end, [](const CommandData& d) { return d.Operands.size() == 0; });
	if(res_it == vec_end) return std::nullopt;
	return *res_it;
}

std::optional<DataManager::CommandData> DataManager::FindCommand(const std::string& name, uint16_t first_op) const
{
	auto it = m_Comands.find(name);
	if (it == m_Comands.end()) return std::nullopt;
	auto vec_begin = it->second.begin();
	auto vec_end = it->second.end();
	auto res_it = std::find_if(vec_begin, vec_end, 
		[first_op](const CommandData& d) { return d.Operands.size() == 1 && d.Operands.front()->Includes(first_op); });
	if (res_it == vec_end) return std::nullopt;
	return *res_it;
}

std::optional<DataManager::CommandData> DataManager::FindCommand(const std::string& name, uint16_t first_op, uint16_t second_op) const
{
	auto it = m_Comands.find(name);
	if (it == m_Comands.end()) return std::nullopt;
	auto vec_begin = it->second.begin();
	auto vec_end = it->second.end();
	auto res_it = std::find_if(vec_begin, vec_end,
		[first_op, second_op](const CommandData& d) { 
			return d.Operands.size() == 2 && d.Operands.front()->Includes(first_op) && d.Operands[1]->Includes(second_op);
		});
	if (res_it == vec_end) return std::nullopt;
	return *res_it;
}

const std::vector<DataManager::CommandData>* DataManager::GetAllCommandData(const std::string& name)
{
	auto it = m_Comands.find(name);
	if (it != m_Comands.end())
		return &it->second;
	return nullptr;
}

std::optional<DataManager::LayoutRange> DataManager::FindRange(const std::string& name)
{
	auto it = m_Ranges.find(name);
	if (it != m_Ranges.end())
	{
		return it->second;
	}
	return std::nullopt;
}

bool DataManager::CompareRange(const std::string& name, LayoutRange* op_range)
{
	auto it = m_Ranges.find(name);
	if (it != m_Ranges.end())
	{
		return *op_range == it->second;
	}
	return false;
}

bool DataManager::SubRange(const std::string& name, LayoutRange* op_range)
{
	auto it = m_Ranges.find(name);
	if (it != m_Ranges.end())
	{
		return it->second.IsSubRange(*op_range);
	}
	return false;
}

