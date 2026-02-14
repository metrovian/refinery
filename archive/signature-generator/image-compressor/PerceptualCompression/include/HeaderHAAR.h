#pragma once
#pragma pack(push, 1)
struct HeaderHAAR
{
	uint16_t type = 0x4148;
	uint32_t fsi;
	uint32_t ofs = 0x0018;
	int32_t width;
	int32_t height;
	uint16_t depth = 0x0018;
	uint32_t dsi;
};
#pragma pack(pop)