#pragma once
#pragma pack(push, 1)
struct HeaderLZ78
{
	uint16_t type = 0x5A4C;
	uint16_t ver = 0x3837;
	uint32_t fsi;
	uint32_t ofs = 0x001A;
	int32_t width;
	int32_t height;
	uint16_t depth = 0x0018;
	uint32_t dsi;
};
#pragma pack(pop)