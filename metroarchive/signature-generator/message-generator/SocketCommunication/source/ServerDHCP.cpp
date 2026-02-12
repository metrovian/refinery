#include "ServerDHCP.h"

ServerDHCP::ServerDHCP(std::string _ip, std::string _snet) : UDP(67)
{
	ip = _ip;
	snet = _snet;
}

ServerDHCP::~ServerDHCP()
{
}

DHCP_FRAME ServerDHCP::parseReceivedMessage(const uint8_t* _msg)
{
	DHCP_FRAME ret;

	ret.op = _msg[0];
	ret.htype = _msg[1];
	ret.hlen = _msg[2];
	ret.hops = _msg[3];

	ret.xid = _msg[4];
	ret.xid = _msg[5] | (ret.xid << 8);
	ret.xid = _msg[6] | (ret.xid << 8);
	ret.xid = _msg[7] | (ret.xid << 8);

	ret.secs = _msg[8];
	ret.secs = _msg[9] | (ret.secs << 8);

	ret.flags = _msg[10];
	ret.flags = _msg[11] | (ret.flags << 8);

	ret.ip1 = _msg[12];
	ret.ip1 = _msg[13] | (ret.ip1 << 8);
	ret.ip1 = _msg[14] | (ret.ip1 << 8);
	ret.ip1 = _msg[15] | (ret.ip1 << 8);

	ret.ip2 = _msg[16];
	ret.ip2 = _msg[17] | (ret.ip2 << 8);
	ret.ip2 = _msg[18] | (ret.ip2 << 8);
	ret.ip2 = _msg[19] | (ret.ip2 << 8);

	ret.ip3 = _msg[20];
	ret.ip3 = _msg[21] | (ret.ip3 << 8);
	ret.ip3 = _msg[22] | (ret.ip3 << 8);
	ret.ip3 = _msg[23] | (ret.ip3 << 8);

	ret.ip4 = _msg[24];
	ret.ip4 = _msg[25] | (ret.ip4 << 8);
	ret.ip4 = _msg[26] | (ret.ip4 << 8);
	ret.ip4 = _msg[27] | (ret.ip4 << 8);

	ret.mac1 = _msg[28];
	ret.mac1 = _msg[29] | (ret.mac1 << 8);
	ret.mac1 = _msg[30] | (ret.mac1 << 8);
	ret.mac1 = _msg[31] | (ret.mac1 << 8);
	ret.mac1 = _msg[32] | (ret.mac1 << 8);
	ret.mac1 = _msg[33] | (ret.mac1 << 8);
	ret.mac1 = _msg[34] | (ret.mac1 << 8);
	ret.mac1 = _msg[35] | (ret.mac1 << 8);

	ret.mac2 = _msg[36];
	ret.mac2 = _msg[37] | (ret.mac2 << 8);
	ret.mac2 = _msg[38] | (ret.mac2 << 8);
	ret.mac2 = _msg[39] | (ret.mac2 << 8);
	ret.mac2 = _msg[40] | (ret.mac2 << 8);
	ret.mac2 = _msg[41] | (ret.mac2 << 8);
	ret.mac2 = _msg[42] | (ret.mac2 << 8);
	ret.mac2 = _msg[43] | (ret.mac2 << 8);

	ret.ses = static_cast<DHCP_SESSION>(_msg[242]);

	return ret;
}

void ServerDHCP::printHardwareAddress(uint64_t _mac)
{
	std::cerr << std::uppercase << std::hex;

	std::vector<uint16_t> out(6);

	out[0] = uint16_t(0xFF & (_mac >> 56));
	out[1] = uint16_t(0xFF & (_mac >> 48));
	out[2] = uint16_t(0xFF & (_mac >> 40));
	out[3] = uint16_t(0xFF & (_mac >> 32));
	out[4] = uint16_t(0xFF & (_mac >> 24));
	out[5] = uint16_t(0xFF & (_mac >> 16));

	for (uint64_t i = 0; i < 5; ++i)
	{
		if (out[i] < 16) std::cerr << '0' << out[i] << ':';
		else std::cerr << out[i] << ":";
	}

	if (out[5] < 16) std::cerr << '0' << out[5];
	else std::cerr << out[5];

	std::cerr << std::dec << std::endl;
}

void ServerDHCP::printHostAddress(uint32_t _ip)
{
	std::cerr << uint16_t(0xFF & (_ip >> 24)) << '.';
	std::cerr << uint16_t(0xFF & (_ip >> 16)) << '.';
	std::cerr << uint16_t(0xFF & (_ip >> 8)) << '.';
	std::cerr << uint16_t(0xFF & (_ip >> 0)) << std::endl;
}

uint32_t ServerDHCP::leaseHostAddress(uint64_t _mac)
{
	auto it1 = std::find_if
	(
		band.begin(), 
		band.end(),
		[&](const auto& _pair) 
		{ 
			return _pair.second == _mac; 
		}
	);

	if (it1 != band.end()) return it1->first;

	else 
	{
		auto it2 = std::find_if
		(
			band.begin(),
			band.end(),
			[&](const auto& _pair)
			{
				return _pair.second == 0;
			}
		);

		if (it2 != band.end()) return it2->first;
	}

	return 0;
}

uint32_t ServerDHCP::releaseHostAddress(uint64_t _mac)
{
	uint32_t ret = 0;

	auto it = std::find_if
	(
		band.begin(),
		band.end(),
		[&](const auto& _pair)
		{
			return _pair.second == _mac;
		}
	);

	if (it != band.end())
	{
		band.erase(it);
		ret = it->first;
	}

	return ret;
}

bool ServerDHCP::insert(std::string _ip)
{
	band.insert(std::make_pair(ntohl(inet_addr(_ip.c_str())), 0));
}

bool ServerDHCP::erase(std::string _ip)
{
	band.erase(ntohl(inet_addr(_ip.c_str())));
}

void ServerDHCP::processReceivedMessage(std::string _msg, uint16_t _port)
{
	if (_msg.size() == 300)
	{
		DHCP_FRAME frame = parseReceivedMessage(reinterpret_cast<const uint8_t*>(_msg.c_str()));

		if (frame.ses != DHCP_SESSION::NONE)
		{
			std::string rsp(300, 0x00);

			rsp[0] = 0x02;
			rsp[1] = frame.htype;
			rsp[2] = frame.hlen;
			rsp[3] = frame.hops;
			rsp[4] = 0xFF & (frame.xid >> 24);
			rsp[5] = 0xFF & (frame.xid >> 16);
			rsp[6] = 0xFF & (frame.xid >> 8);
			rsp[7] = 0xFF & (frame.xid >> 0);
			rsp[8] = 0x00;
			rsp[9] = 0x00;
			rsp[10] = 0xFF & (frame.flags >> 8);
			rsp[11] = 0xFF & (frame.flags >> 0);

			rsp[236] = 0x63;
			rsp[237] = 0x82;
			rsp[238] = 0x53;
			rsp[239] = 0x63;

			switch (frame.ses)
			{

			case DHCP_SESSION::DISCOVER:
			{
				std::cerr << "[Host " << _port << "] Discover : ";

				uint32_t ipv4 = leaseHostAddress(frame.mac1);
				uint32_t ips = ntohl(inet_addr(ip.c_str()));

				rsp[16] = 0xFF & (ipv4 >> 24);
				rsp[17] = 0xFF & (ipv4 >> 16);
				rsp[18] = 0xFF & (ipv4 >> 8);
				rsp[19] = 0xFF & (ipv4 >> 0);

				rsp[28] = 0xFF & (frame.mac1 >> 56);
				rsp[29] = 0xFF & (frame.mac1 >> 48);
				rsp[30] = 0xFF & (frame.mac1 >> 40);
				rsp[31] = 0xFF & (frame.mac1 >> 32);
				rsp[32] = 0xFF & (frame.mac1 >>	24);
				rsp[33] = 0xFF & (frame.mac1 >> 16);
				rsp[34] = 0xFF & (frame.mac1 >> 8);
				rsp[35] = 0xFF & (frame.mac1 >> 0);

				rsp[36] = 0xFF & (frame.mac2 >> 56);
				rsp[37] = 0xFF & (frame.mac2 >> 48);
				rsp[38] = 0xFF & (frame.mac2 >> 40);
				rsp[39] = 0xFF & (frame.mac2 >> 32);
				rsp[40] = 0xFF & (frame.mac2 >> 24);
				rsp[41] = 0xFF & (frame.mac2 >> 16);
				rsp[42] = 0xFF & (frame.mac2 >> 8);
				rsp[43] = 0xFF & (frame.mac2 >> 0);

				rsp[240] = 0x35;
				rsp[241] = 0x01;

				if (ipv4 > 0) rsp[242] = static_cast<char>(DHCP_SESSION::OFFER);
				else rsp[242] = static_cast<char>(DHCP_SESSION::NACK);

				rsp[243] = 0x36;
				rsp[244] = 0x04;
				rsp[245] = 0xFF & (ips >> 24);
				rsp[246] = 0xFF & (ips >> 16);
				rsp[247] = 0xFF & (ips >> 8);
				rsp[248] = 0xFF & (ips >> 0);

				rsp[249] = 0xFF;

				static int opt = 1;
				setsockopt(host, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
				sendSimpleMessage(rsp, "255.255.255.255", 68);

				printHardwareAddress(frame.mac1);
				break;
			}

			case DHCP_SESSION::REQUEST:
			{
				std::cerr << "[Host " << _port << "] Request : ";

				uint32_t ipv4 = leaseHostAddress(frame.mac1);
				uint32_t ips = ntohl(inet_addr(ip.c_str()));
				uint32_t msks = ntohl(inet_addr(snet.c_str()));

				rsp[16] = 0xFF & (ipv4 >> 24);
				rsp[17] = 0xFF & (ipv4 >> 16);
				rsp[18] = 0xFF & (ipv4 >> 8);
				rsp[19] = 0xFF & (ipv4 >> 0);

				rsp[28] = 0xFF & (frame.mac1 >> 56);
				rsp[29] = 0xFF & (frame.mac1 >> 48);
				rsp[30] = 0xFF & (frame.mac1 >> 40);
				rsp[31] = 0xFF & (frame.mac1 >> 32);
				rsp[32] = 0xFF & (frame.mac1 >> 24);
				rsp[33] = 0xFF & (frame.mac1 >> 16);
				rsp[34] = 0xFF & (frame.mac1 >> 8);
				rsp[35] = 0xFF & (frame.mac1 >> 0);

				rsp[36] = 0xFF & (frame.mac2 >> 56);
				rsp[37] = 0xFF & (frame.mac2 >> 48);
				rsp[38] = 0xFF & (frame.mac2 >> 40);
				rsp[39] = 0xFF & (frame.mac2 >> 32);
				rsp[40] = 0xFF & (frame.mac2 >> 24);
				rsp[41] = 0xFF & (frame.mac2 >> 16);
				rsp[42] = 0xFF & (frame.mac2 >> 8);
				rsp[43] = 0xFF & (frame.mac2 >> 0);

				rsp[240] = 0x35;
				rsp[241] = 0x01;

				if (ipv4 > 0) rsp[242] = static_cast<char>(DHCP_SESSION::ACK);
				else rsp[242] = static_cast<char>(DHCP_SESSION::NACK);

				rsp[243] = 0x36;
				rsp[244] = 0x04;
				rsp[245] = 0xFF & (ips >> 24);
				rsp[246] = 0xFF & (ips >> 16);
				rsp[247] = 0xFF & (ips >> 8);
				rsp[248] = 0xFF & (ips >> 0);

				rsp[249] = 0x01;
				rsp[250] = 0x04;
				rsp[251] = 0xFF & (msks >> 24);
				rsp[252] = 0xFF & (msks >> 16);
				rsp[253] = 0xFF & (msks >> 8);
				rsp[254] = 0xFF & (msks >> 0);

				rsp[255] = 0xFF;

				static int opt = 1;
				setsockopt(host, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
				sendSimpleMessage(rsp, "255.255.255.255", 68);

				printHostAddress(ipv4);
				break;
			}

			case DHCP_SESSION::RELEASE:
			{
				std::cerr << "[Host " << _port << "] Release : ";

				printHostAddress(releaseHostAddress(frame.mac1));
				break;
			}

			}
		}
	}
}
