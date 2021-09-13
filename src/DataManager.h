#pragma once
#include "Lexer.h"
#include <vector>
#include <map>
#include <optional>
#define T_TO_STRING(x) #x
class DataManager
{
public:
	using Layout = std::vector<std::vector<LexemType>>;
	template<typename T>
	class Range
	{
		friend DataManager;
	public:
		Range(T from, T to) : m_From(from), m_To(to)
		{
			using namespace std::string_literals;
			if (from > to) throw "RangeClass<"s + T_TO_STRING(T) +"> from > to" ;
		}
		Range() = delete;
		Range& operator=(const Range& r)
		{
			if (&r != this)
			{
				m_From = r.m_From;
				m_To = r.m_To;
			}
			return *this;
		};
		Range(const Range& r) { *this = r; }
		bool operator==(const Range& r) { return m_To == r.m_To && m_From == r.m_From; };
		bool Includes(T num) { return num <= m_To && num >= m_From; };
		bool IsSubRange(const Range& r) { return m_From <= r.m_From && m_To >= r.m_To; }
	private:
		T m_From;
		T m_To;
	};
	using LayoutRange = Range<uint8_t>;
	struct CommandData
	{
		std::vector<LayoutRange*> Operands;
		uint8_t Opcode = 0;
		uint8_t ModRM = 0;
		bool Prefix = false;
		bool Short = false;
	};
public:
	void GenerateLayouts(const std::string& path);
	void GenerateCommadsData(const std::string& path);

	std::optional<CommandData> FindCommand(const std::string& name) const;
	std::optional<CommandData> FindCommand(const std::string& name,uint16_t first_op) const;
	std::optional<CommandData> FindCommand(const std::string& name, uint16_t first_op, uint16_t second_op) const;
	const std::vector<CommandData>* GetAllCommandData(const std::string& name);
	std::optional<LayoutRange> FindRange(const std::string& name);
	bool CompareRange(const std::string& name, LayoutRange* op_range);
	bool SubRange(const std::string& name, LayoutRange* op_range);
	//uint8_t GetOperandType() const;
	const Layout& GetLayouts() const { return m_Layouts; }
	static DataManager& Get() 
	{
		static DataManager instance;
		return instance;
	};
private:
	DataManager() = default;
private:
	
	Layout m_Layouts;
	std::map<std::string, LayoutRange> m_Ranges;
	std::map<std::string, std::vector<CommandData>> m_Comands;
};