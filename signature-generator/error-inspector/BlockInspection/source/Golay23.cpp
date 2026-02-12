#include "Golay23.h"

bool Golay23::encode()
{
	std::vector<std::bitset<23>> ret;
	std::vector<std::bitset<12>> baw = transform<12>(raw);

	for (uint64_t i = 0; i < baw.size(); ++i)
	{
		std::bitset<23> res(0);

		uint64_t par = 0;
		uint64_t prod = 0;

		static const uint64_t gen[12] =
		{
			0x7FF800, 0x3FFC00, 0x1FFE00, 0x0FFF00,
			0x087F80, 0x043FC0, 0x021FE0, 0x010FF0,
			0x008878, 0x00443C, 0x00221E, 0x00110F,
		};

		for (uint64_t j = 0; j < 12; ++j) 
		{
			if (baw[i][j])
			{
				par ^= gen[j];
			}

			res[j] = baw[i][j];
		}

		for (uint64_t j = 0; j < 11; ++j)
		{
			res[j + 12] = par & (static_cast<uint64_t>(1) << j);
		}

		ret.push_back(res);
	}

	redc = inverse<23>(ret);
	return true;
}

bool Golay23::decode()
{
	std::vector<std::bitset<12>> ret;
	std::vector<std::bitset<23>> bedc = transform<23>(redc);

	for (uint64_t i = 0; i < bedc.size(); i++)
	{
		std::bitset<12> match(0);

		uint64_t mscore = 0;
		uint64_t mrange = static_cast<uint64_t>(1) << 23;

		static const uint64_t gen[12] =
		{
			0x7FF800, 0x3FFC00, 0x1FFE00, 0x0FFF00,
			0x087F80, 0x043FC0, 0x021FE0, 0x010FF0,
			0x008878, 0x00443C, 0x00221E, 0x00110F,
		};

		for (uint64_t j = 0; j < mrange; ++j)
		{
			if (std::bitset<23>(j).count() < 4)
			{
				std::bitset<23> cand = bedc[i] ^ std::bitset<23>(j);

				std::bitset<23> res(0);
				std::bitset<12> code(0);

				for (uint64_t k = 0; k < 12; ++k)
				{
					code[k] = cand[k];
				}

				uint64_t par = 0;
				uint64_t prod = 0;

				for (uint64_t k = 0; k < 12; ++k)
				{
					if (code[k])
					{
						par ^= gen[k];
					}

					res[k] = code[k];
				}

				for (uint64_t k = 0; k < 11; ++k)
				{
					res[k + 12] = par & (static_cast<uint64_t>(1) << k);
				}

				if (res == cand)
				{
					ret.push_back(code);
					break;
				}
			}
		}
	}

	raw = inverse<12>(ret);
	return true;
}
