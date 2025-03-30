#pragma once
#include "PrimalityTest.h"

class Lucas
{
protected: /* parts */
	static std::vector<uint64_t> find(uint64_t _num);
	static std::set<uint64_t> factor(uint64_t _num);

public: /* public */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);
};