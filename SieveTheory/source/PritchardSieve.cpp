#include "PritchardSieve.h"

bool PritchardSieve::less_pair(const std::pair<std::string, std::string>& _num, const std::pair<std::string, std::string>& _min)
{
	return less(_num.first, _min.first);
}

bool PritchardSieve::greater_pair(const std::pair<std::string, std::string>& _num, const std::pair<std::string, std::string>& _min)
{
	return greater(_num.first, _min.first);
}

bool PritchardSieve::process_pre(std::string _min, std::string _max)
{
	static std::once_flag once;

	std::call_once
	(
		once,
		[this]()
		{
			cps = std::priority_queue
				<
				std::pair<std::string, std::string>,
				std::vector<std::pair<std::string, std::string>>,
				decltype(&PritchardSieve::greater_pair)
				>(&PritchardSieve::greater_pair);
		}
	);

	std::string prm;
	std::string psqr;

	while (primes >> prm)
	{
		psqr = square(prm);

		if (less(psqr, _max))
		{
			if (less(psqr, _min))
			{
				psqr = sub(_min, mod(_min, prm));

				if (less(psqr, _min))
				{
					psqr = plus(psqr, prm);
				}
			}

			conds[ntoi(psqr, _min)] = false;
			cps.push(std::make_pair(psqr, prm));
		}
	}

	primes.clear();
	primes.seekg(0, primes.beg);

	return true;
}

bool PritchardSieve::process_post(std::string _min, std::string _max)
{
	for (std::string i = _min; less(i, _max); i = increase(i))
	{
		if (conds[ntoi(i, _min)])
		{
			primes << i << " ";
			++count;

			if (less(square(i), _max))
			{
				conds[ntoi(square(i), _min)] = false;
				cps.push(std::make_pair(square(i), i));
			}
		}

		else
		{
			while (!cps.empty())
			{
				std::string cmp = cps.top().first;
				std::string prm = cps.top().second;

				if (cmp != i) break;

				if (less(plus(cmp, prm), _max))
				{
					conds[ntoi(plus(cmp, prm), _min)] = false;
					cps.push(std::make_pair(plus(cmp, prm), prm));
				}

				cps.pop();
			}
		}
	}

	return true;
}
