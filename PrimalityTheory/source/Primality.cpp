#include "Primality.h"

bool Primality::push_back(PRIMALITY_TEST _test)
{
    data.push_back(_test);
    return true;
}

bool Primality::pop_back()
{
    data.pop_back();
    return true;
}

bool Primality::clear()
{
    data.clear();
    return true;
}

bool Primality::is_prime(uint64_t _num)
{
    bool ret = false;

    for (uint64_t i = 0; i < data.size(); ++i)
    {
        PRIMALITY_RESULT res = data[i](_num);
        
        switch (res)
        {
        case PRIMALITY_RESULT::FALSE: return false;
        case PRIMALITY_RESULT::TRUE: return true;
        case PRIMALITY_RESULT::PROBABLY_FALSE: ret |= false; break;
        case PRIMALITY_RESULT::PROBABLY_TRUE: ret |= true; break;
        }
    }

    return ret;
}
