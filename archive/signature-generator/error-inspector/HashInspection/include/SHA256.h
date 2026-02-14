#pragma once
#include "Hash.h"

class SHA256 : public Hash
{
protected: /* virtual */
	std::vector<uint8_t> calc_hash() override;
};