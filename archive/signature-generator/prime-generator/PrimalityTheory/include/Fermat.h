#pragma once
#include "PrimalityTest.h"

class Fermat
{
public: /* public */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);
};