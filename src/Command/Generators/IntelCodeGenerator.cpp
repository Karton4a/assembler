#include "IntelCodeGenerator.h"
#include "../../SupportedTokens.h"

void IntelCodeGenerator::GenerateSib(std::vector<uint8_t>& code, uint8_t modrm, uint8_t opcode, BaseIndex& base_index)
{
	uint8_t sib = 0;
	int seg_num = -1;
	auto default_seg = "DS";
	if (!base_index.Segment.empty())
	{
		seg_num = Supported::SEGMENT_REGISTER_CODES.find(base_index.Segment)->second;
	}
	if (base_index.Index == "ESP")//ss
	{
		std::swap(base_index.Base, base_index.Index);
		default_seg = "SS";
	}
	if (base_index.Base == "EBP") //ss
	{
		modrm |= 0b01 << 6;
		default_seg = "SS";
	}
	modrm |= 0b100;

	sib |= 0 << 6;
	sib |= Supported::REGISTER_NUMS.find(base_index.Base)->second;
	sib |= Supported::REGISTER_NUMS.find(base_index.Index)->second << 3;
	if (seg_num != -1 && default_seg != base_index.Segment)
		code.push_back(seg_num);
	code.push_back(opcode);
	code.push_back(modrm);
	code.push_back(sib);
	if (base_index.Base == "EBP")
	{
		code.push_back(0);
	}
}
