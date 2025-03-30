#pragma once
#include "Primality.h"

class Lucas
{
protected: /* factor */
	static std::vector<uint64_t> find(uint64_t _num);
	static std::set<uint64_t> factor(uint64_t _num);

protected: /* parts */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);

public: /* public */
	static PRIMALITY_TEST test(uint64_t _base);
};