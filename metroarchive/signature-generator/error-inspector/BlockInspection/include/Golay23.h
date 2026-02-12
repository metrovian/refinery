#pragma once
#include "Block.h"

class Golay23 : public Block
{
protected: /* block */
	virtual uint8_t input_block() const { return 12; };
	virtual uint8_t encode_block() const { return 23; };

public: /* virtual */
	virtual bool encode();
	virtual bool decode();
};