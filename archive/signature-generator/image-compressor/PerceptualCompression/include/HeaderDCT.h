#pragma once
#pragma pack(push, 1)
struct HeaderDCT
{
	uint16_t type = 0x4344;
	uint32_t fsi;
	uint32_t ofs = 0x0019;
	int32_t width;
	int32_t height;
	uint16_t depth = 0x0018;
	uint32_t dsi;
	uint8_t qual = 0x005A;
};
#pragma pack(pop)