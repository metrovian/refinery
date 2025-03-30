#include "Fermat.h"

PRIMALITY_RESULT Fermat::calc(uint64_t _base, uint64_t _num)
{
    if (_num == 0 || _num == 1) return PRIMALITY_RESULT::FALSE;
    if (_num == 2 || _num == 3) return PRIMALITY_RESULT::TRUE;

    if (_num % 2 == 0) return PRIMALITY_RESULT::FALSE;

    uint64_t ret = 1;
   
    uint64_t base = _base;
    uint64_t exp = _num - 1;

    while (exp > 0) 
    {
        if (exp % 2 == 1)
        {
            ret = (ret * base) % _num;
        }
            
        base = (base * base) % _num;
        exp /= 2;
    }

    if (ret == 1)
    {
        return PRIMALITY_RESULT::PROBABLY_TRUE;
    }

    return PRIMALITY_RESULT::FALSE;
}

PRIMALITY_TEST Fermat::test(uint64_t _base)
{
	return std::bind(calc, _base, std::placeholders::_1);
}
