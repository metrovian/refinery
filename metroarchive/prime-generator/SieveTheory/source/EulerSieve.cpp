#include "EulerSieve.h"

bool EulerSieve::process_pre(std::string _min, std::string _max)
{
	std::string num;
	std::string prm;
	std::string ptr;

	for (std::string num = "2"; less(num, _max); num = increase(num))
	{
		primes.clear();
		primes.seekg(0, primes.beg);

		while (primes >> prm)
		{
			ptr = cross(num, prm);

			if (less(ptr, _max))
			{
				if (least(ptr, _min)) conds[ntoi(ptr, _min)] = false;
				if (mod(num, prm) == "0") break;

				continue;
			}

			break;
		}
	}

	primes.clear();
	primes.seekg(0, primes.beg);

	return true;
}

bool EulerSieve::process_post(std::string _min, std::string _max)
{
	std::string num;
	std::string prm;
	std::string ptr;

	for (uint64_t i = 0; less(iton(i, _min), _max); ++i)
	{
		num = iton(i, _min);

		if (conds[i])
		{
			primes << num << " ";
			++count;
		}

		primes.clear();
		primes.seekg(0, primes.beg);

		while (primes >> prm)
		{
			ptr = cross(num, prm);

			if (less(ptr, _max))
			{
				if (least(ptr, _min)) conds[ntoi(ptr, _min)] = false;
				if (mod(num, prm) == "0") break;

				continue;
			}

			break;
		}
	}

	primes.clear();
	primes.seekg(0, primes.beg);

	return true;
}