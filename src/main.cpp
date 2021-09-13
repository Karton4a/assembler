#include "Lexer.h"
#include "Parser.h"
#include "DataManager.h"
#include <fstream>
#include "SupportedTokens.h"
#include <regex>
#include <iostream>
#include <iomanip>
constexpr const char* target_path = "resources/test3.asm";
constexpr const char* result_path = "out/result_test2.lst";
void second_stage(const std::string& out_file);
void third_stage(const std::string& out_file);
int main()
{
	//third_stage("out/offsets_test2.txt");
	//second_stage("out/lexems_test2.txt");
	std::ifstream infile(target_path);
	std::string line;
	Lexer lex;
	DataManager::Get().GenerateLayouts("resources/test.rule");
	DataManager::Get().GenerateCommadsData("resources/commads.operands");
	Parser parser;
	auto lambda = [](const std::string& s)
	{
		if (s.size() <= 5 && std::isalpha(s[0]) && s.size() != 0) return true;
		return false;
	};
	lex.SetUserIdentifierRule(lambda);
	lex.SetException(true);
	size_t count = 1;
	std::vector<uint8_t> sizes;
	uint8_t max_command_size = 0;
	while (std::getline(infile, line))
	{
		try
		{
			lex.PushLine(line, Supported::TOKENS);
			parser.ParseLexemTable(lex.GetCurrentTable());
			sizes.push_back(parser.FirstPass(lex.GetCurrentTable()));
			if (max_command_size < sizes.back())
			{
				max_command_size = sizes.back();
			}
		}
		catch (LexerException& e)
		{
			std::cout << "[ " << count << " ] : " << "Lexer error " << e.what() << " " << line << std::endl;
			sizes.push_back(0);
		}
		catch (RawParserException& e)
		{
			std::cout << "[ " << count << " ] : " << "Parser error " << e.what() << " " << line << std::endl;
			sizes.push_back(0);
		}
		catch (CriticalParserException& e)
		{
			std::cout << "[ " << count << " ] : " << "Critical parser error " << e.what() << " " << line << std::endl;
			return 0;
		}
		count++;
	}
	infile.close();
	infile.open(target_path);
	count = 0;
	parser.ClearSegmentTable();
	parser.ClearEndStatus();
	std::ofstream result(result_path);
	constexpr uint8_t offset_size = 10;
	uint8_t source_code_offfset = offset_size + max_command_size * 3;
	uint8_t byte_code_offset = source_code_offfset - offset_size;
	std::ostream& out = result;
	size_t error_count = 0;
	while (std::getline(infile, line))
	{
		try
		{
			lex.PushLine(line, Supported::TOKENS);
			parser.ParseLexemTable(lex.GetCurrentTable());
			auto off = parser.GetCurrentOffset();
			auto code = parser.SecondPass(lex.GetCurrentTable(), sizes[count]);
			if (!line.empty())
			{
				out << std::internal << std::setfill('0') << std::setw(offset_size);
				out << std::hex << off << ' ';
			}
			for (auto n : code)
			{
				out << std::internal << std::setfill('0') << std::setw(2);
				out << std::hex << (unsigned short)n << ' ';
			}
			for (int i = 0; i < byte_code_offset - (int)code.size() * 3; i++)
			{
				out << ' ';
			}
			out << line << std::endl;
		}
		catch (LexerException& e)
		{
			out << "[ " <<std::dec<< count + 1 << " ] : " << "Lexer error " << e.what() << " " << line <<  std::endl;
			error_count++;
		}
		catch (RawParserException& e)
		{
			out << "[ " << std::dec << count + 1 << " ] : " << "Parser error " << e.what() << " " << line << std::endl;
			error_count++;
		}
		catch (CriticalParserException& e)
		{
			out << "[ " << std::dec << count + 1 << " ] : " << "Critical parser error " << e.what() << " " << line << std::endl;
			return 0;
		}

		count++;
	}
	if(!parser.GetEndStatus())
		out << "Parser error no end derective" << std::endl;
	out << std::endl;
	out << "Errors : " << error_count << std::endl;
	out << std::left << std::setfill(' ') << std::setw(15)<< "SymbolName" << std::setw(10) << "Type"<< "Value" <<std::endl;
	for (const auto& [name,params] : parser.GetLabelTable())
	{
		out << std::left << std::setfill(' ');
		out << std::setw(15) << name
			<< std::setw(10) <<  params.Type
			<<  params.SegmentName << ':' 
			<< std::setw(4)<<std::right<< std::setfill('0') << std::hex << params.Offset << std::endl;
	}
	const auto& seg_table = parser.GetSegmentTable();
	out << std::endl;
	out << std::left << std::setfill(' ') << std::setw(15) << "SEGMENT" << std::setw(10) << "SIZE" << "BIT" << std::endl;
	for (size_t i = 0; i < parser.GetSegmentCount(); i++)
	{
		out << std::setw(15) << seg_table[i].Name
			<< std::setw(10) << std::hex << seg_table[i].Size
			<< std::dec << (int)seg_table[i].BitDepth << std::endl;
	}
	out << std::endl;
	out << std::left << std::setfill(' ') << std::setw(15) << "SEGMENT" << std::setw(15) << "DESTINATION"<< std::endl;
	for (const auto&[seg,dest] : parser.GetSegmentDestinationTable())
	{
		out << std::setw(15) << seg << std::setw(15) << dest << std::endl;
	}
	result.close();
	return 0;
}
void third_stage(const std::string& out_file)
{
	std::ifstream infile(target_path);
	std::ofstream out(out_file);
	std::string line;
	Lexer lex;
	DataManager::Get().GenerateLayouts("resources/test.rule");
	DataManager::Get().GenerateCommadsData("resources/commads.operands");
	Parser parser;
	auto lambda = [](const std::string& s)
	{
		if (s.size() <= 5 && std::isalpha(s[0]) && s.size() != 0) return true;
		return false;
	};
	lex.SetUserIdentifierRule(lambda);
	lex.SetException(true);
	out << std::setfill('0');
	while (std::getline(infile, line))
	{
		try
		{
			lex.PushLine(line, Supported::TOKENS);
			parser.ParseLexemTable(lex.GetCurrentTable());
			auto t = parser.GetCurrentOffset();
			parser.FirstPass(lex.GetCurrentTable());
			out << std::right << std::hex << std::setw(10) << t << "    " << line << std::endl;
		}
		catch (const std::exception&)
		{

		}
	}
	out << std::left << std::setfill(' ') << std::setw(15) << "SymbolName" << std::setw(10) << "Type" << "Value" << std::endl;
	for (const auto& [name, params] : parser.GetLabelTable())
	{
		out << std::left << std::setfill(' ');
		out << std::setw(15) << name
			<< std::setw(10) << params.Type
			<< params.SegmentName << ':'
			<< std::setw(4) << std::right << std::setfill('0') << std::hex << params.Offset << std::endl;
	}
	const auto& seg_table = parser.GetSegmentTable();
	out << std::endl;
	out << std::left << std::setfill(' ') << std::setw(15) << "SEGMENT" << std::setw(10) << "SIZE" << "BIT" << std::endl;
	for (size_t i = 0; i < parser.GetSegmentCount(); i++)
	{
		out << std::setw(15) << seg_table[i].Name
			<< std::setw(10) << std::hex << seg_table[i].Size
			<< std::dec << (int)seg_table[i].BitDepth << std::endl;
	}
	out << std::endl;
	out << std::left << std::setfill(' ') << std::setw(15) << "SEGMENT" << std::setw(15) << "DESTINATION" << std::endl;
	for (const auto& [seg, dest] : parser.GetSegmentDestinationTable())
	{
		out << std::setw(15) << seg << std::setw(15) << dest << std::endl;
	}
}
void second_stage(const std::string& out_file)
{
	std::ifstream infile(target_path);
	std::string line;
	Lexer lex;
	Parser parser;
	std::ofstream out(out_file);//"out/lexems.lex");
	auto lambda = [](const std::string& s)
	{
		if (s.size() <= 5 && std::isalpha(s[0]) && s.size() != 0) return true;
		return false;
	};
	DataManager::Get().GenerateLayouts("resources/test.rule");
	DataManager::Get().GenerateCommadsData("resources/commads.operands");
	lex.SetUserIdentifierRule(lambda);
	while (std::getline(infile, line))
	{
		if (line.empty()) continue;
		lex.PushLine(line, Supported::TOKENS);
		parser.ParseLexemTable(lex.GetCurrentTable());
		out << line << std::endl;
		out << std::left <<std::setw(3)<< "N" <<std::setw(15) << std::left << "Name" << std::setw(15) << "Size" << std::setw(15) << "Type" << std::endl;
		const auto& l = lex.GetCurrentTable();
		for (size_t i = 0; i < l.size(); i++)
		{
			out << std::setw(3) <<i<<std::setw(15) << l[i].Name << std::setw(15) << (int)l[i].Size << std::setw(15) << LexemTypeToString(l[i].Type) << std::endl;
		}
		const auto& s = parser.GetSyntaxTable();
		out << std::setw(15) << std::left << "MnemoCode" << std::setw(15) << "Operand_1" << std::setw(15) << "Operand_2" << std::endl;
		for (size_t i = 0; i < s.size(); i++)
		{
			if (s[i].LexemCount == 0)
				out << std::setw(15) << "----------";
			else
				out << std::setw(15) << std::to_string((int)s[i].LexemPosition) + " " + std::to_string((int)s[i].LexemCount);
		}
		for (size_t i = 0; i < 3 - s.size(); i++)
		{
			out << std::setw(15) << "----------";
		}
		out << std::endl<<std::endl;
	}
}