#include "ServerSNMP.h"

ServerSNMP::ServerSNMP() : UDP(0)
{
	startReceiveThread(161);
	startReceiveThread(162);
}

ServerSNMP::~ServerSNMP()
{
	stopThread(161);
	stopThread(162);
}

std::string ServerSNMP::encodeNumberASN1(int32_t _num)
{
	std::string ret;

	ret.push_back(0x02);
	ret.push_back(0x04);

	ret.push_back(static_cast<char>(0xFF & (_num >> 24)));
	ret.push_back(static_cast<char>(0xFF & (_num >> 16)));
	ret.push_back(static_cast<char>(0xFF & (_num >> 8)));
	ret.push_back(static_cast<char>(0xFF & (_num >> 0)));

	return ret;
}

std::string ServerSNMP::encodeMessageASN1(std::string _str)
{
	std::string ret;

	ret.push_back(0x04);
	ret.push_back(_str.size());

	ret += _str;

	return ret;
}

std::string ServerSNMP::encodeFrameASN1(SNMP_SESSION _ses, SNMP_FRAME _msg)
{
	std::string ret;

	uint64_t sofs;
	uint64_t bofs;
	uint64_t cofs;

	ret += encodeNumberASN1(_msg.version);
	ret += encodeMessageASN1(_msg.community);

	ret.push_back(static_cast<char>(_ses));
	ret.push_back(0x00);

	sofs = ret.size();

	ret += encodeNumberASN1(_msg.id);
	ret += encodeNumberASN1(_msg.ers);
	ret += encodeNumberASN1(_msg.eri);

	ret.push_back(0x30);
	ret.push_back(0x00);

	bofs = ret.size();

	for (const auto& pair : _msg.binds) 
	{
		ret.push_back(0x30);
		ret.push_back(0x00);

		cofs = ret.size();

		ret.push_back(0x06);
		ret.push_back(pair.first.size());

		ret += pair.first;

		ret.push_back(0x05);
		ret.push_back(pair.second.size());

		ret += pair.second;

		ret[cofs - 1] = ret.size() - cofs;
	}

	ret[sofs - 1] = ret.size() - sofs;
	ret[bofs - 1] = ret.size() - bofs;

	ret.insert(ret.begin(), ret.size());
	ret.insert(ret.begin(), 0x30);

	return ret;
}

bool ServerSNMP::sendRequestMessage(std::string _ip, SNMP_SESSION _ses, SNMP_FRAME _msg)
{
	return sendSimpleMessage(encodeFrameASN1(_ses, _msg), _ip, 161);
}

void ServerSNMP::processReceivedMessage(std::string _msg, uint16_t _port)
{
	SNMP_FRAME ret;
	SNMP_SESSION ses;

	const uint8_t* msg = reinterpret_cast<const uint8_t*>(_msg.c_str());

	if (msg[0] == 0x30)
	{
		if (msg[1] == _msg.size() - 2)
		{
			if (msg[2] != 0x02) return;
			if (msg[4 + msg[3]] != 0x04) return;

			for (uint64_t i = 0; i < msg[3]; ++i)
			{
				ret.version = msg[i + 4] | (ret.version << 8);
			}

			for (uint64_t i = 0; i < msg[5 + msg[3]]; ++i)
			{
				ret.community += msg[i + 6 + msg[3]];
			}

			ses = static_cast<SNMP_SESSION>(msg[6 + msg[3] + msg[5 + msg[3]]]);

			uint64_t ptr = 8 + msg[3] + msg[5 + msg[3]];
			uint64_t add = 0;

			auto read = [&](int32_t& _val)
				{
					if (msg[ptr++] != 0x02) return;

					for (uint64_t i = 0; i < msg[ptr]; ++i)
					{
						_val = msg[i + 1 + ptr] | (_val << 8);
					}

					ptr += msg[ptr] + 1;
				};

			std::cerr << "[Host " << _port << "] ";

			switch (ses)
			{

			case SNMP_SESSION::GET_RESPONSE:
			{
				read(ret.id);
				read(ret.ers);
				read(ret.eri);

				std::cerr << "Response : " << ret.id << std::endl; 
				break;
			}

			case SNMP_SESSION::TRAP:
			{
				if (msg[ptr++] != 0x06) break;
				if (msg[ptr + msg[ptr] + 1] != 0x40) break;

				std::string ip;

				for (uint64_t i = 0; i < 3; ++i)
				{
					ip += std::to_string(static_cast<int>(msg[ptr + msg[ptr] + i + 3]));
					ip += '.';
				}

				ip += std::to_string(static_cast<int>(msg[ptr + msg[ptr] + 6]));

				std::cerr << "Trap : " << ip << std::endl; 
				break;
			}

			}
		}
	}
}