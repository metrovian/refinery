#pragma once
#include "Primality.h"

class Fermat
{
protected: /* parts */
	static PRIMALITY_RESULT calc(uint64_t _base, uint64_t _num);

public: /* public */
	static PRIMALITY_TEST test(uint64_t _base);
};