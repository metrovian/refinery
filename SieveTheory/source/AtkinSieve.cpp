#include "AtkinSieve.h"

bool AtkinSieve::reverse_cond1(std::string _nsqr, std::string _msqr, std::string _min, std::string _max)
{
	std::string num = plus(cross(_nsqr, "4"), _msqr);
	std::string res = mod(num, "12");

	if (between(num, _min, _max))
	{
		if (res == "1" || res == "5")
		{
			conds[ntoi(num, _min)] = !conds[ntoi(num, _min)];
			return true;
		}
	}

	return false;
}

bool AtkinSieve::reverse_cond2(std::string _nsqr, std::string _msqr, std::string _min, std::string _max)
{
	std::string num = plus(cross(_nsqr, "3"), _msqr);
	std::string res = mod(num, "12");

	if (between(num, _min, _max))
	{
		if (res == "7")
		{
			conds[ntoi(num, _min)] = !conds[ntoi(num, _min)];
			return true;
		}
	}

	return false;
}

bool AtkinSieve::reverse_cond3(std::string _nsqr, std::string _msqr, std::string _min, std::string _max)
{
	std::string num = sub(cross(_nsqr, "3"), _msqr);
	std::string res = mod(num, "12");

	if (num.empty()) return false;

	if (between(num, _min, _max))
	{
		if (less(_msqr, _nsqr))
		{
			if (res == "11")
			{
				conds[ntoi(num, _min)] = !conds[ntoi(num, _min)];
				return true;
			}
		}
	}

	return false;
}

bool AtkinSieve::process_pre(std::string _min, std::string _max)
{
	std::string nsqr;
	std::string msqr;

	if (_min == "2")
	{
		if (least(_max, "2")) conds[0] = false;
		if (least(_max, "3")) conds[1] = false;
	}

	for (std::string i = "1"; less(square(i), _max); i = increase(i))
	{
		nsqr = square(i);

		for (std::string j = "1"; less(square(j), _max); j = increase(j))
		{
			msqr = square(j);

			reverse_cond1(nsqr, msqr, _min, _max);
			reverse_cond2(nsqr, msqr, _min, _max);
			reverse_cond3(nsqr, msqr, _min, _max);
		}
	}

	for (std::string i = _min; less(i, _max); i = plus(i, "1"))
	{
		nsqr = square(i);

		if (!conds[ntoi(i, _min)])
		{
			for (std::string j = nsqr; less(j, _max); j = plus(j, nsqr))
			{
				conds[ntoi(j, _min)] = true;
			}
		}
	}

	return true;
}

bool AtkinSieve::process_post(std::string _min, std::string _max)
{
	std::string prm;
	std::string res;
	std::string sqr;

	while (primes >> prm)
	{
		res = square(prm);
		sqr = res;

		while (less(res, _max))
		{
			if (least(res, _min))
			{
				conds[ntoi(res, _min)] = true;
			}

			res = plus(res, sqr);
		}
	}

	primes.clear();
	primes.seekg(0, primes.beg);

	for (uint64_t i = 0; less(iton(i, _min), _max); ++i)
	{
		if (!conds[i])
		{
			primes << iton(i, _min) << " ";
			++count;
		}
	}

	return true;
}