#pragma once
#include "Hash.h"

class SHA512 : public Hash
{
protected: /* virtual */
	std::vector<uint8_t> calc_hash() override;
};