#include "MultiplicativeHash.h"

uint64_t MultiplicativeHash::sum_xor(std::vector<uint8_t> _data)
{
	uint64_t ret = 0;

	for (uint64_t i = 0; i < _data.size(); ++i)
	{
		ret ^= _data[i];
	}

	return ret;
}

uint64_t MultiplicativeHash::multiply_decimal(uint64_t _data, double _real)
{
	double res = static_cast<double>(_data) * _real;
	double deci = static_cast<double>(res - static_cast<uint64_t>(res));

	return static_cast<uint64_t>(deci * std::pow(10.0, hsize));
}

std::vector<uint8_t> MultiplicativeHash::calc_hash()
{
	std::vector<uint8_t> ret;

	uint64_t res = multiply_decimal(sum_xor(raw), 0.123456789);

	for (uint64_t i = 0; i < 10; ++i)
	{
		ret.push_back(static_cast<uint8_t>(res % 10));
		res /= 10;
	}

	return ret;
}
