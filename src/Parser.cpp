#include "Parser.h"
#include "SupportedTokens.h"
#include "Command/Mnemonic.h"
#include "Command/DataDerectiveCommand.h"
#include <iostream>
#include <iomanip>
#include <stack>
Parser::Parser()
{
	m_SegmentDestinationTable = {
		{"ES", "Nothing" },
		{"CS", "Nothing" },
		{"SS", "Nothing" },
		{"DS", "Nothing" },
		{"FS", "Nothing" },
		{"GS", "Nothing" },
	};
	//auto it = Supported::TOKENS.begin();
	//while ((it = std::find(it, Supported::TOKENS.end(), 
	//	[](auto t) { return t->second == LexemType::SegmentRegister16Bit; })) != Supported::TOKENS.end())
	//{
	//	//m_SegmentDestinationTable.emplace(it->first,"Nothing");
	//}
}
void Parser::ParseLexemTable(const Lexer::LexemTable& lex_table)
{
	m_Table.clear();
	auto lambda = 
		[](Lexer::LexemTableRow c) { return c.Type == LexemType::Command || c.Type == LexemType::DataDerective; };
	auto begin = lex_table.begin();
	auto it = std::find_if(begin, lex_table.end(), lambda);
	uint8_t count_of_lex = 0;
	uint8_t pos_of_first = std::distance(begin, it);
	while (it != lex_table.end() && lambda(*it))
	{
		count_of_lex++;
		it++;
	}
	if(count_of_lex == 0)
	{
		//no commands
	}
	m_Table.push_back({ pos_of_first, count_of_lex });
	auto separator_lambda = [](Lexer::LexemTableRow c) { return c.Name == ","; };
	auto op_it = std::find_if(it, lex_table.end(), separator_lambda);
	while (op_it != lex_table.end())
	{
		m_Table.push_back({ (uint8_t)std::distance(begin, it), (uint8_t)std::distance(it, op_it) });
		it = op_it + 1;
		op_it = std::find_if(it, lex_table.end(), separator_lambda);
	}
	uint8_t dist = std::distance(it, op_it);
	if(dist != 0)
		m_Table.push_back({ (uint8_t)std::distance(begin, it), dist });
}

uint8_t Parser::FirstPass(Lexer::LexemTable lex_table)
{
	uint8_t op_size = 0;
	if (lex_table.size() == 0 || m_Table.size() == 0) return op_size;
	auto compare_labda = [](auto& a, auto& b) {return a.Type == b; };
	if (m_Table.front().LexemCount == 0)
	{
		p_ProccesAsmCommands(lex_table);
	}
	else
	{
		if (m_ActiveSegment == m_SegmentTable.end()) throw RawParserException("no active segment");
		p_AddLabel(lex_table);
		const auto& first_lexem = lex_table[m_Table.front().LexemPosition];
		p_SetOperandsType(lex_table);
		auto ptr = Mnemonic::Create(first_lexem.Type,lex_table,m_Table, m_ActiveSegment->CurrentOffsert);
		ptr->CalculateSize(*m_ActiveSegment,m_LabelTable);
		m_ActiveSegment->CurrentOffsert += ptr->GetSize();
		op_size = ptr->GetSize();
	}
	return op_size;
}

std::vector<uint8_t> Parser::SecondPass(Lexer::LexemTable lex_table, uint32_t op_size)
{
	if (lex_table.size() == 0 || m_Table.size() == 0) return {};
	auto compare_labda = [](auto& a, auto& b) {return a.Type == b; };
	std::shared_ptr<Mnemonic> ptr;
	if (m_Table.front().LexemCount == 0)
	{
		p_ProccesAsmCommands(lex_table);
	}
	else
	{
		if (m_ActiveSegment == m_SegmentTable.end()) throw RawParserException("no active segment");
		//p_AddLabel(lex_table);
		const auto& first_lexem = lex_table[m_Table.front().LexemPosition];
		p_SetOperandsType(lex_table);
		ptr = Mnemonic::Create(first_lexem.Type, lex_table, m_Table, m_ActiveSegment->CurrentOffsert);
		ptr->GenerateCode(*m_ActiveSegment, m_LabelTable, op_size);
	}
	std::vector<uint8_t> result(op_size, 0);
	if (ptr)
	{
		result = ptr->GetByteCode();
		m_ActiveSegment->CurrentOffsert += ptr->GetSize();
	}
	else result = {};
	return result;
}

uint32_t Parser::GetCurrentOffset() const
{
	if (m_ActiveSegment != m_SegmentTable.end())
		return m_ActiveSegment->CurrentOffsert;
	return 0;
}

void Parser::ClearSegmentTable()
{
	for (auto& seg : m_SegmentTable)
	{
		seg = SegmentData();
	}
	m_CreatedSegments = 0;
	m_ActiveSegment = m_SegmentTable.end();
}


long long Parser::ToNum(const Lexer::LexemTableRow& row)
{
	if (IsDec(row.Type))	return std::stoll(row.Name);
	if (IsHex(row.Type))	return std::stoll(row.Name, 0, 16);
	if (IsBin(row.Type))	return std::stoll(row.Name, 0, 2);
}

long long Parser::CalculateEquasion(LexemIterator begin, LexemIterator end)
{
	//Lexer::LexemTable polish_notation;
	std::stack<long long> data_stack;
	std::stack<std::string> operator_stack;
	for (auto it = begin; it != end; it++)
	{
		if (IsDec(it->Type))
		{
			data_stack.push(std::stoi(it->Name));
		}
		else if (IsBin(it->Type))
		{
			data_stack.push(std::stoi(it->Name, 0, 2));
		}
		else if (IsHex(it->Type))
		{
			data_stack.push(std::stoi(it->Name, 0, 16));
		}
		else if (p_OperatorPriority(it->Name) != 5)
		{
			if (it->Name == "(")
			{
				operator_stack.push(it->Name);
			}
			else if (it->Name == ")")
			{
				while (operator_stack.top() != "(")
				{
					data_stack.push(p_CalculateOperation(data_stack, operator_stack.top()));
					operator_stack.pop();
				}
				operator_stack.pop();
			}
			else if (operator_stack.empty())
			{
				operator_stack.push(it->Name);
			}
			else if (p_OperatorPriority(operator_stack.top()) > p_OperatorPriority(it->Name))
			{
				operator_stack.push(it->Name);
			}
			else if (p_OperatorPriority(operator_stack.top()) <= p_OperatorPriority(it->Name))
			{
				data_stack.push(p_CalculateOperation(data_stack, operator_stack.top()));
				operator_stack.pop();
				operator_stack.push(it->Name);
			}
		}
	}
	while (!operator_stack.empty())
	{
		data_stack.push(p_CalculateOperation(data_stack, operator_stack.top()));
		operator_stack.pop();
	}
	return data_stack.top();
}

long long Parser::p_CalculateOperation(std::stack<long long>& data_stack, std::string& op)
{
	if (data_stack.empty()) throw RawParserException("equation parse error");
	long long result = 0;
	if (data_stack.size() == 1)
	{
		auto operand = data_stack.top();
		data_stack.pop();
		if (op == "-")
			return -1 * operand;
		else if (op == "+")
			return operand;
		else throw RawParserException("equation parse error");

	}
	auto first_operand = data_stack.top();
	data_stack.pop();
	auto second_operand = data_stack.top();
	data_stack.pop();
	if (op == "+")
	{
		result = first_operand + second_operand;
	}
	else if (op == "-")
	{
		result = second_operand - first_operand;
	}
	else if (op == "/")
	{
		result = second_operand / first_operand;
	}
	else if (op == "*")
	{
		result = first_operand * second_operand;
	}
	else throw RawParserException("equation parse error");

	return result;
}

int Parser::p_OperatorPriority(const std::string& op)
{
	if (op ==  "+") return 3;
	if (op =="-")   return 3;
	if (op == "*")  return 2;
	if (op == "/")  return 2;
	if (op == "(")  return 4;
	if (op == ")")  return 4;
	return 5;
}

void Parser::p_AddLabel(const Lexer::LexemTable& lex_table)
{
	auto compare_labda = [](auto& a, auto& b) {return a.Type == b; }; //TODO maybe make this variable a member
	std::vector<LexemType> grammatic_layout;
	std::string label_type;
	const auto& first_lexem = lex_table[m_Table.front().LexemPosition];
	if (first_lexem.Type == LexemType::DataDerective)
	{
		grammatic_layout = { LexemType::UserIdentifier };
		auto& [name, data] = *Supported::DATA_DERECTIVES.find(first_lexem.Name);
		label_type = data.LabelType;
	}
	else
	{
		grammatic_layout = Supported::ASM_GRAMMATICS[2]; //TODO remove magic number
		label_type = "Near";
	}

	if (m_Table.front().LexemPosition != 0)
	{
		auto begin = lex_table.begin();
		auto end = begin + m_Table.front().LexemPosition;
		bool status = std::equal(begin, end, grammatic_layout.begin(), grammatic_layout.end(), compare_labda);
		if (!status)
		{
			//TODO error unsupported label format
			throw RawParserException("unsupported label format");
		}
		if (m_ActiveSegment == m_SegmentTable.end()) throw RawParserException("no active segment");
		LabelParameters params = { label_type, m_ActiveSegment->Name, m_ActiveSegment->CurrentOffsert };
		//auto res = m_LabelTable.find(lex_table.front().Name);
		m_LabelTable.try_emplace(lex_table.front().Name, params);
		//if (res != m_LabelTable.end())
			//throw RawParserException(lex_table.front().Name + " already defined");
	}
}

void Parser::p_ProccesAsmCommands(const Lexer::LexemTable& lex_table)
{
	auto compare_labda = [](auto& a, auto& b) {return a.Type == b; };
	auto counter = 0;
	for (const auto& row : Supported::ASM_GRAMMATICS)
	{
		bool status = std::equal(lex_table.begin(), lex_table.end(), row.begin(), row.end(), compare_labda);
		if (status)
			break;
		counter++;
	}
	if (counter == Supported::ASM_GRAMMATICS.size())
	{
		throw RawParserException("usupported construction");
		//TODO unsupported construction
	}
	if (counter == 0)
	{
		if (m_ActiveSegment != m_SegmentTable.end()) 
			throw CriticalParserException(m_ActiveSegment->Name + " not closed");
		try
		{
			m_SegmentTable.at(m_CreatedSegments).Name = lex_table.front().Name;
			m_ActiveSegment = m_SegmentTable.begin() + m_CreatedSegments;
			m_CreatedSegments++;
		}
		catch (const std::exception&)
		{
			throw CriticalParserException("too many segments, supported only two");
		}
	}
	else if (counter == 2)
	{
		if (m_ActiveSegment == m_SegmentTable.end()) throw RawParserException("no active segment");
		LabelParameters params = { "Near", m_ActiveSegment->Name, m_ActiveSegment->CurrentOffsert };
		//auto res = m_LabelTable.find(lex_table.front().Name);
		m_LabelTable.try_emplace(lex_table.front().Name, params);
		//if (res != m_LabelTable.end())
			//throw RawParserException(lex_table.front().Name + " already defined");
	}
	else if (counter == 1) //segment end
	{
		if (m_ActiveSegment == m_SegmentTable.end()) throw CriticalParserException("no active segment");
		if (m_ActiveSegment->Name != lex_table.front().Name)
			throw CriticalParserException("error one segment opened but closed another");
		m_ActiveSegment->Size = m_ActiveSegment->CurrentOffsert;
		m_ActiveSegment = m_SegmentTable.end();
	}
	else if (counter == 3)
	{
		m_ProgramEnd = true;
		if (m_ActiveSegment != m_SegmentTable.end())
			throw CriticalParserException(m_ActiveSegment->Name + " not closed");
	}
}

void Parser::p_SetOperandsType(Lexer::LexemTable& lex_table)
{
	auto compare_labda = [](auto& a, auto& b) {return a.Type == b; };
	auto i = m_Table.begin() + 1;
	while (i != m_Table.end())
	{
		size_t counter = 0;

		auto op_begin = lex_table.begin() + i->LexemPosition;
		auto op_end = op_begin + i->LexemCount;
		//auto t = DataManager::Get().GetLayouts();
		for (const auto& layout : DataManager::Get().GetLayouts())
		{
			bool status = std::equal(op_begin, op_end, layout.begin(), layout.end(), compare_labda);
			if (status)
			{
				i->OperandType = counter;
				break;
			}
			counter++;
		}
		if (i->OperandType == -1)
		{
			auto count = std::count_if(op_begin, op_end,
				[](const Lexer::LexemTableRow& l) {return l.Type == LexemType::UserIdentifier; });
			if (count == 0)
			{
				auto res = CalculateEquasion(op_begin, op_end);
				Lexer::LexemTable local_table(lex_table.begin(), op_begin);
				local_table.emplace_back(std::to_string(res));
				local_table.back().Type = Lexer::TypeOfConstant(local_table.back().Name);
				for (auto it = op_end; it != lex_table.end(); it++)
				{
					local_table.push_back(*it);
				}
				for (auto it = i + 1; it != m_Table.end(); it++)
				{
					it->LexemPosition -= i->LexemCount + 1;
				}
				i->LexemCount = 1;
				//i->OperandType = 0; //TODO magic number
				auto op_begin = lex_table.begin() + i->LexemPosition;
				auto op_end = op_begin + i->LexemCount;
				lex_table = local_table;
				counter = 0;
				for (const auto& layout : DataManager::Get().GetLayouts())
				{
					bool status = std::equal(op_begin, op_end, layout.begin(), layout.end(), compare_labda);
					if (status)
					{
						i->OperandType = counter;
						break;
					}
					counter++;
				}
			}

		}
		i++;
	}
}
