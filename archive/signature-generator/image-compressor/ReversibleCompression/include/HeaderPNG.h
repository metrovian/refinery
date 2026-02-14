#pragma once
#pragma pack(push, 1)
struct HeaderPNG
{
	uint64_t type = 0x0A1A0A0D474E5089;
	uint32_t width;
	uint32_t height;
	uint8_t depth = 0x0008;
	uint8_t colp = 0x0002;
	uint8_t cmeth = 0x0000;
	uint8_t fmeth = 0x0000;
	uint8_t imeth = 0x0000;
};
#pragma pack(pop)