#include "SolovayStrassen.h"

uint64_t SolovayStrassen::jacobi(uint64_t _base, uint64_t _num)
{
    if (_num <= 0 || _num % 2 == 0) return 0;
    
    uint64_t ret = 1;

    while (_base != 0)
    {
        while (_base % 2 == 0)
        {
            _base /= 2;

            if (_num % 8 == 3 || _num % 8 == 5)
            {
                ret = _num - ret;
            }
        }

        std::swap(_base, _num);

        if (_base % 4 == 3 && _num % 4 == 3)
        {
            ret = _num - ret;
        }

        _base %= _num;
    }

    if (_num == 1)
    {
        return ret;
    }

    return 0;
}

PRIMALITY_RESULT SolovayStrassen::calc(uint64_t _base, uint64_t _num)
{
    return PRIMALITY_RESULT::PROBABLY_TRUE;
}

PRIMALITY_TEST SolovayStrassen::test(uint64_t _base)
{
    return std::bind(calc, _base, std::placeholders::_1);
}
