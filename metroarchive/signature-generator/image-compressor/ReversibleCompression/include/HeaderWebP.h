#pragma once
#pragma pack(push, 1)
struct HeaderWebP
{
	uint32_t rf = 0x46464952;
	uint32_t fsi;
	uint32_t type = 0x50424557;
	uint32_t cype;
	uint32_t csi;
};
#pragma pack(pop)