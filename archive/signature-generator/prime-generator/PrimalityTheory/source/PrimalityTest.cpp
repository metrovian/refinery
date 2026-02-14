#include "PrimalityTest.h"
#include "Fermat.h"
#include "Lucas.h"
#include "MillerRabin.h"
#include "SolovayStrassen.h"

bool PrimalityTest::push_back(PRIMALITY_TYPE _name, uint64_t _base)
{
	switch (_name)
	{

	case PRIMALITY_TYPE::FERMAT:
	{
		data.push_back(std::bind(Fermat::calc, _base, std::placeholders::_1));
		break;
	}

	case PRIMALITY_TYPE::LUCAS:
	{
		data.push_back(std::bind(Lucas::calc, _base, std::placeholders::_1));
		break;
	}

	case PRIMALITY_TYPE::MILLER_RABIN:
	{
		data.push_back(std::bind(MillerRabin::calc, _base, std::placeholders::_1));
		break;
	}

	case PRIMALITY_TYPE::SOLOVAY_STRASSEN:
	{
		data.push_back(std::bind(SolovayStrassen::calc, _base, std::placeholders::_1));
		break;
	}

	default: return false;

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