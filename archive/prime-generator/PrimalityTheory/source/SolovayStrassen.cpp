#include "SolovayStrassen.h"

uint64_t SolovayStrassen::jacobi(uint64_t _base, uint64_t _num)
{
	if (_num % 2 == 0) return 0;
	if (_num == 1) return 0;

	uint64_t ret = 1;
	uint64_t base = _base;

	while (base != 0)
	{
		while (base % 2 == 0)
		{
			base /= 2;

			if (_num % 8 == 3 || _num % 8 == 5)
			{
				ret = _num - ret;
			}
		}

		std::swap(base, _num);

		if (base % 4 == 3 && _num % 4 == 3)
		{
			ret = _num - ret;
		}

		base %= _num;
	}

	return ret;
}

PRIMALITY_RESULT SolovayStrassen::calc(uint64_t _base, uint64_t _num)
{
	if (_num == 0 || _num == 1) return PRIMALITY_RESULT::FALSE;
	if (_num == 2 || _num == 3) return PRIMALITY_RESULT::TRUE;

	if (_num % 2 == 0) return PRIMALITY_RESULT::FALSE;

	uint64_t jac = jacobi(_base, _num) % _num;

	uint64_t ret = 1;
	uint64_t base = _base;

	uint64_t exp = (_num - 1) / 2;

	while (exp > 0)
	{
		if (exp % 2 == 1)
		{
			ret = (ret * base) % _num;
		}

		base = (base * base) % _num;
		exp /= 2;
	}

	if (ret != jac)
	{
		return PRIMALITY_RESULT::FALSE;
	}

	return PRIMALITY_RESULT::PROBABLY_TRUE;
}