#include "SundaramSieve.h"

std::string SundaramSieve::max_cond1(std::string _num)
{
	return square(increase(cross(_num, "2")));
}

std::string SundaramSieve::max_cond2(std::string _num, std::string _mum)
{
	std::string ret = plus(_num, _mum);
	std::string crs = cross(_num, _mum);

	crs = cross(crs, "2");
	ret = increase(cross(plus(ret, crs), "2"));

	return ret;
}

bool SundaramSieve::process_pre(std::string _min, std::string _max)
{
	uint64_t par = mod(_min, "2") == "0" ? 0 : 1;

	for (uint64_t i = par; i < unit; i += 2)
	{
		conds[i] = false;
	}

	if (_min == "2")
	{
		if (least(_max, "2"))
		{
			conds[0] = true;
		}
	}

	return true;
}

bool SundaramSieve::process_post(std::string _min, std::string _max)
{
	std::string cur;
	std::string cus;

	for (std::string i = "1"; less(cur = max_cond1(i), _max); i = increase(i))
	{
		cus = i;

		if (less(increase(cross(i, "3")), _min))
		{
			std::string up = sub(_min, cross(i, "2"));
			std::string low = plus(cross(i, "4"), "2");

			if (less(cus = div(sub(up, "1"), low), i))
			{
				cus = i;
			}
		}

		for (std::string j = cus; less(cur = max_cond2(i, j), _max); j = increase(j))
		{
			if (least(cur, _min))
			{
				conds[ntoi(cur, _min)] = false;
			}
		}
	}

	for (uint64_t i = 0; less(iton(i, _min), _max); ++i)
	{
		if (conds[i])
		{
			primes << iton(i, _min) << " ";
			++count;
		}
	}

	return true;
}
