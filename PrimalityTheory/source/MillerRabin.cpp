#include "MillerRabin.h"

std::pair<uint64_t, uint64_t> MillerRabin::factor(uint64_t _num)
{
    uint64_t res = 0;
    uint64_t rem = _num;

    while (rem % 2 == 0)
    {
		++res;
		rem >>= 1;
    }

    return std::make_pair(res, rem);
}

PRIMALITY_RESULT MillerRabin::calc(uint64_t _base, uint64_t _num)
{
    if (_num == 0 || _num == 1) return PRIMALITY_RESULT::FALSE;
    if (_num == 2 || _num == 3) return PRIMALITY_RESULT::TRUE;

    if (_num % 2 == 0) return PRIMALITY_RESULT::FALSE;

    std::pair<uint64_t, uint64_t> factors = factor(_num - 1);

    uint64_t ret = 1;
    uint64_t base = _base;

	uint64_t rep = factors.first - 1;
    uint64_t exp = factors.second;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            ret = (ret * base) % _num;
        }

        base = (base * base) % _num;
        exp /= 2;
    }

	if (ret == 1 || ret == _num - 1)
	{
		return PRIMALITY_RESULT::PROBABLY_TRUE;
	}

    for (uint64_t i = 0; i < rep; ++i) 
    {
		ret = (ret * ret) % _num;

		if (ret == _num - 1)
		{
			return PRIMALITY_RESULT::PROBABLY_TRUE;
		}
    }

    return PRIMALITY_RESULT::FALSE;
}