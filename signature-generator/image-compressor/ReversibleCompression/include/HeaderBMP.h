#pragma once
#pragma pack(push, 1)
struct HeaderBMP
{
	uint16_t type = 0x4D42;
	uint32_t fsi;
	uint16_t res1 = 0x0000;
	uint16_t res2 = 0x0000;
	uint32_t ofs = 0x0036;
	uint32_t hsi = 0x0028;
	int32_t width;
	int32_t height;
	uint16_t colp = 0x0001;
	uint16_t depth = 0x0018;
	uint32_t comp = 0x0000;
	uint32_t dsi;
	int32_t resx = 0x0000;
	int32_t resy = 0x0000;
	uint32_t colu = 0x0000;
	uint32_t coli = 0x0000;
};
#pragma pack(pop)