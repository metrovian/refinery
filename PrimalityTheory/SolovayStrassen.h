#pragma once
#include "Primality.h"

class SolovayStrassen
{
protected: /* factor */
	static uint64_t jacobi(uint64_t _base, uint64_t _num);

protected: /* parts */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);

public: /* public */
	static PRIMALITY_TEST test(uint64_t _base);
};