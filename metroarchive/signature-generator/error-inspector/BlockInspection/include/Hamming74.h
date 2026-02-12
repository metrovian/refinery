#pragma once
#include "Block.h"

class Hamming74 : public Block
{
protected: /* block */
	virtual uint8_t input_block() const { return 4; };
	virtual uint8_t encode_block() const { return 7; };

public: /* virtual */
	virtual bool encode();
	virtual bool decode();
};