#pragma once
#include "MultiplicativeHash.h"

class FibonacciHash : public MultiplicativeHash
{
protected: /* virtual */
	std::vector<uint8_t> calc_hash() override;
};