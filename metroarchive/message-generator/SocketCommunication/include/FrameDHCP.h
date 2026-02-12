#pragma once

enum class DHCP_SESSION
{
	NONE,
	DISCOVER,
	OFFER,
	REQUEST,
	DECLINE,
	ACK,
	NACK,
	RELEASE
};

struct DHCP_FRAME
{
	DHCP_SESSION ses = DHCP_SESSION::NONE;

	uint8_t op = 0x00;
	uint8_t htype = 0x00;
	uint8_t hlen = 0x00;
	uint8_t hops = 0x00;
	uint32_t xid = 0x00;
	uint16_t secs = 0x00;
	uint16_t flags = 0x00;

	uint32_t ip1 = 0x00;
	uint32_t ip2 = 0x00;
	uint32_t ip3 = 0x00;
	uint32_t ip4 = 0x00;

	uint64_t mac1 = 0x00;
	uint64_t mac2 = 0x00;
};