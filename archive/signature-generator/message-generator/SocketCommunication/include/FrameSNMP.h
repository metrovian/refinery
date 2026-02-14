#pragma once

enum class SNMP_SESSION
{
	GET_REQUEST		= 0xA0,
	GET_NEXT		= 0xA1,
	GET_RESPONSE	= 0xA2,
	SET_REQUEST		= 0xA3,
	TRAP			= 0xA4
};

struct SNMP_FRAME
{
	int32_t version = 0x00;
	int32_t id = 0x00;
	int32_t ers = 0x00;
	int32_t eri = 0x00;

	std::string community;
	std::vector<std::pair<std::string, std::string>> binds;
};