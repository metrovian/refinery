#include "Hadamard32.h"

bool Hadamard32::encode()
{
	std::vector<std::bitset<32>> ret;
	std::vector<std::bitset<5>> baw = transform<5>(raw);

	for (uint64_t i = 0; i < baw.size(); ++i)
	{
		std::bitset<32> res(0);

		for (uint8_t j = 0; j < 32; ++j)
		{
			res[j] = (baw[i] & std::bitset<5>(j)).count() % 2;
		}

		ret.push_back(res);
	}

	redc = inverse<32>(ret);
	return true;
}

bool Hadamard32::decode()
{
	std::vector<std::bitset<5>> ret;
	std::vector<std::bitset<32>> bedc = transform<32>(redc);

	for (uint64_t i = 0; i < bedc.size(); i++)
	{
		std::bitset<5> match(0);

		uint64_t mscore = 0;
		uint64_t prod = 0;

		for (uint64_t j = 0; j < 32; ++j)
		{
			std::bitset<5> cand(j);
			std::bitset<32> code(0);

			uint64_t inner = 0;

			for (uint64_t k = 0; k < 32; ++k)
			{
				code[k] = (cand & std::bitset<5>(k)).count() % 2;

				if (bedc[i][k] == code[k])
				{
					++inner;
				}
			}

			if (inner > mscore)
			{
				mscore = inner;
				match = cand;
			}
		}

		ret.push_back(match);
	}

	raw = inverse<5>(ret);
	return true;
}
