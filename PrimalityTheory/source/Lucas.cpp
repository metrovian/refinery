#include "Lucas.h"

std::vector<uint64_t> Lucas::find(uint64_t _num)
{
    static std::vector<uint64_t> primes = { 2, 3, 5, 7, 11 };

    for (uint64_t i = primes.back() + 1; i <= _num; i++) 
    {
        bool cond1 = false;
        bool cond2 = true;

        for (uint64_t j = 0; j < primes.size(); ++j)
        {
            uint64_t prm = primes[j];

            cond1 = (i < prm * prm);
            cond2 = (i % prm);

            if (cond1 || !cond2) break;
        }

        if (cond2)
        {
            primes.push_back(i);
        }
    }

	return primes;
}

std::set<uint64_t> Lucas::factor(uint64_t _num)
{
    static std::vector<uint64_t> primes = { 2, 3, 5, 7, 11 };

    uint64_t num = _num;
    uint64_t prm = 0;

    if (primes.back() * primes.back() <= num)
    {
        primes = find(num);
    }

    std::set<uint64_t> ret;

    for (uint64_t i = 0; i < primes.size(); ++i)
    {
        prm = primes[i];

        if (prm * prm > num) break;

        while (num % prm == 0)
        {
            ret.insert(prm);
            num /= prm;
        }
    }

    if (num > 1)
    {
        ret.insert(num);
    }

    return ret;
}

PRIMALITY_RESULT Lucas::calc(uint64_t _base, uint64_t _num)
{
    if (_num == 0 || _num == 1) return PRIMALITY_RESULT::FALSE;
    if (_num == 2 || _num == 3) return PRIMALITY_RESULT::TRUE;

    if (_num % 2 == 0) return PRIMALITY_RESULT::FALSE;

	std::set<uint64_t> factors = factor(_num - 1);

    auto func = [&](uint64_t _div)
        {
            uint64_t ret = 1;

            uint64_t base = _base;
            uint64_t exp = (_num - 1) / _div;

            while (exp > 0)
            {
                if (exp % 2 == 1)
                {
                    ret = (ret * base) % _num;
                }

                base = (base * base) % _num;
                exp /= 2;
            }

            return ret;
        };

    if (func(1) != 1)
    {
		return PRIMALITY_RESULT::FALSE;
    }

	for (auto div : factors)
	{
        if (func(div) == 1)
        {
            return PRIMALITY_RESULT::PROBABLY_FALSE;
        }
	}

    return PRIMALITY_RESULT::TRUE;
}

PRIMALITY_TEST Lucas::test(uint64_t _base)
{
    return std::bind(calc, _base, std::placeholders::_1);
}
