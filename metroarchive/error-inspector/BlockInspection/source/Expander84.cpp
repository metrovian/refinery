#include "Expander84.h"

bool Expander84::encode()
{
	std::vector<std::bitset<8>> ret;
	std::vector<std::bitset<4>> baw = transform<4>(raw);

	for (uint64_t i = 0; i < baw.size(); ++i)
	{
		std::bitset<8> res(0);

		res[0] = baw[i][0];
		res[1] = baw[i][1];
		res[2] = baw[i][2];
		res[3] = baw[i][3];

		res[4] = res[0] ^ res[1];
		res[5] = res[1] ^ res[2];
		res[6] = res[2] ^ res[3];
		res[7] = res[3] ^ res[0];

		ret.push_back(res);
	}

	redc = inverse<8>(ret);
	return true;
}

bool Expander84::decode()
{
	std::vector<std::bitset<4>> ret;
	std::vector<std::bitset<8>> bedc = transform<8>(redc);

	for (uint64_t i = 0; i < bedc.size(); ++i)
	{
		std::bitset<4> res(0);
		std::vector<uint8_t> score(4, 0);

		uint8_t dat1 = bedc[i][0];
		uint8_t dat2 = bedc[i][1];
		uint8_t dat3 = bedc[i][2];
		uint8_t dat4 = bedc[i][3];

		uint8_t par1 = bedc[i][4];
		uint8_t par2 = bedc[i][5];
		uint8_t par3 = bedc[i][6];
		uint8_t par4 = bedc[i][7];

		if (par1 != (dat1 ^ dat2))
		{
			score[0]++;
			score[1]++;
		}

		if (par2 != (dat2 ^ dat3))
		{
			score[1]++;
			score[2]++;
		}

		if (par3 != (dat3 ^ dat4))
		{
			score[2]++;
			score[3]++;
		}

		if (par4 != (dat4 ^ dat1))
		{
			score[3]++;
			score[0]++;
		}

		uint8_t max = 0;
		uint8_t epos = 0;

		for (uint8_t j = 0; j < 4; ++j)
		{
			if (score[j] > max)
			{
				max = score[j];
				epos = j;
			}
		}

		if (max > 1)
		{
			bedc[i][epos] = !bedc[i][epos];
		}

		res[0] = bedc[i][0];
		res[1] = bedc[i][1];
		res[2] = bedc[i][2];
		res[3] = bedc[i][3];

		ret.push_back(res);
	}

	raw = inverse<4>(ret);
	return true;
}
