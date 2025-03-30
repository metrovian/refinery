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
    return PRIMALITY_RESULT();
}

PRIMALITY_TEST MillerRabin::test(uint64_t _base)
{
    return PRIMALITY_TEST();
}
