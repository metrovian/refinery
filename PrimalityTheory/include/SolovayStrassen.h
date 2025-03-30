#pragma once
#include "PrimalityTest.h"

class SolovayStrassen
{
protected: /* parts */
	static uint64_t jacobi(uint64_t _base, uint64_t _num);

public: /* public */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);
};