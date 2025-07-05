#include "EratosthenesSieve.h"

bool EratosthenesSieve::process_pre(std::string _min, std::string _max)
{
	std::string prm;

	while (primes >> prm)
	{
		if (!less(square(prm), _max)) break;

		for (std::string j = sub(_min, mod(_min, prm)); less(j, _max); j = plus(j, prm))
		{
			if (least(j, _min)) conds[ntoi(j, _min)] = false;
		}
	}

	primes.clear();
	primes.seekg(0, primes.beg);

	return true;
}

bool EratosthenesSieve::process_post(std::string _min, std::string _max)
{
	std::string num;

	for (uint64_t i = 0; less(iton(i, _min), _max); ++i)
	{
		if (!conds[i]) continue;

		num = iton(i, _min);

		primes << num << " ";
		++count;

		for (std::string j = num; less(j, _max); j = plus(j, num))
		{
			conds[ntoi(j, _min)] = false;
		}
	}

	return true;
}