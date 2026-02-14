#pragma once
#pragma pack(push, 1)
struct HeaderJPEG
{
	uint16_t type = 0xFFD8;
	int32_t width;
	int32_t height;
	uint8_t colc = 0x0003;
	uint8_t cols = 0x0002;
};
#pragma pack(pop)