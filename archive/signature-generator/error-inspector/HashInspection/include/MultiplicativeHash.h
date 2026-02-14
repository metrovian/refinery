#pragma once
#include "Hash.h"

class MultiplicativeHash : public Hash
{
protected: /* config */
	const uint64_t hsize = 10;

protected: /* constants */
	static constexpr double knuth = 0.6180339887;
	static constexpr double phi = 0.3819660113;

protected: /* operation */
	uint64_t sum_xor(std::vector<uint8_t> _data);
	uint64_t multiply_decimal(uint64_t _data, double _real);

protected: /* virtual */
	std::vector<uint8_t> calc_hash() override;
};