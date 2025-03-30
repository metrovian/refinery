#pragma once
#include "PrimalityTest.h"

class MillerRabin
{
protected: /* parts */
	static std::pair<uint64_t, uint64_t> factor(uint64_t _num);

public: /* public */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);
};