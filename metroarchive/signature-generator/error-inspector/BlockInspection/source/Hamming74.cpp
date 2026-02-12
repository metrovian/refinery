#include "Hamming74.h"

bool Hamming74::encode()
{
	std::vector<std::bitset<7>> ret;
	std::vector<std::bitset<4>> baw = transform<4>(raw);

	for (uint64_t i = 0; i < baw.size(); ++i)
	{
		std::bitset<7> res(0);

		res[2] = baw[i][0];
		res[4] = baw[i][1];
		res[5] = baw[i][2];
		res[6] = baw[i][3];

		res[0] = res[2] ^ res[4] ^ res[6];
		res[1] = res[2] ^ res[5] ^ res[6];
		res[3] = res[4] ^ res[5] ^ res[6];

		ret.push_back(res);
	}

	redc = inverse<7>(ret);
	return true;
}

bool Hamming74::decode()
{
	std::vector<std::bitset<4>> ret;
	std::vector<std::bitset<7>> bedc = transform<7>(redc);

	for (uint64_t i = 0; i < bedc.size(); ++i)
	{
		std::bitset<4> res(0);

		uint8_t dat1 = bedc[i][2];
		uint8_t dat2 = bedc[i][4];
		uint8_t dat3 = bedc[i][5];
		uint8_t dat4 = bedc[i][6];

		uint8_t par1 = bedc[i][0];
		uint8_t par2 = bedc[i][1];
		uint8_t par3 = bedc[i][3];

		uint8_t syn1 = par1 ^ dat1 ^ dat2 ^ dat4;
		uint8_t syn2 = par2 ^ dat1 ^ dat3 ^ dat4;
		uint8_t syn3 = par3 ^ dat2 ^ dat3 ^ dat4;

		uint8_t epos = (syn3 << 2) | (syn2 << 1) | (syn1 << 0);

		if (epos != 0) 
		{
			bedc[i] ^= static_cast<uint8_t>(1 << (epos - 1));
		}

		res[0] = bedc[i][2];
		res[1] = bedc[i][4];
		res[2] = bedc[i][5];
		res[3] = bedc[i][6];

		ret.push_back(res);
	}

	raw = inverse<4>(ret);
	return true;
}
