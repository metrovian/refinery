#include "FibonacciHash.h"

std::vector<uint8_t> FibonacciHash::calc_hash()
{
	std::vector<uint8_t> ret;

	uint64_t res = multiply_decimal(sum_xor(raw), knuth);

	for (uint64_t i = 0; i < 10; ++i)
	{
		ret.push_back(static_cast<uint8_t>(res % 10));
		res /= 10;
	}

	return ret;
}
