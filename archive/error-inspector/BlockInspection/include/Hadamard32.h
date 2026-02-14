#pragma once
#include "Block.h"

class Hadamard32 : public Block
{
protected: /* block */
	virtual uint8_t input_block() const { return 5; };
	virtual uint8_t encode_block() const { return 32; };

public: /* virtual */
	virtual bool encode();
	virtual bool decode();
};