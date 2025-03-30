#include "PrimalityTest.h"
#include "Fermat.h"
#include "Lucas.h"
#include "MillerRabin.h"
#include "SolovayStrassen.h"

bool PrimalityTest::push_back(std::string _name, uint64_t _base)
{
    if (_name == "Fermat")
    {
        data.push_back(std::bind(Fermat::calc, _base, std::placeholders::_1));
    }

	else if (_name == "Lucas")
	{
		data.push_back(std::bind(Lucas::calc, _base, std::placeholders::_1));
	}

	else if (_name == "Miller-Rabin")
	{
		data.push_back(std::bind(MillerRabin::calc, _base, std::placeholders::_1));
	}

	else if (_name == "Solovay-Strassen")
	{
		data.push_back(std::bind(SolovayStrassen::calc, _base, std::placeholders::_1));
	}

	else
	{
		return false;
	}

    return true;
}

bool PrimalityTest::pop_back()
{
    data.pop_back();
    return true;
}

bool PrimalityTest::clear()
{
    data.clear();
    return true;
}

bool PrimalityTest::is_prime(uint64_t _num)
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