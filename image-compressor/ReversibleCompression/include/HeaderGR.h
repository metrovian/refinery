#pragma once
#pragma pack(push, 1)
struct HeaderGR
{
	uint16_t type = 0x5247;
	uint32_t fsi;
	uint32_t ofs = 0x001A;
	int32_t width;
	int32_t height;
	uint16_t depth = 0x0018;
	uint32_t dsi;
	uint8_t grk = 0x0005;
	uint8_t grm = 0x0020;
};
#pragma pack(pop)