#pragma once
#include "Block.h"

class ReedSolomon73 : public Block
{
protected: /* symbol */
	class symbol
	{
	public: /* data */
		std::bitset<3> data;

	public: /* constructor */
		symbol(std::bitset<3>_num = 0) : data(_num) {}
		~symbol() = default;

	public: /* operator */
		symbol operator+(const symbol& _sym) const;
		symbol operator-(const symbol& _sym) const;
		symbol operator*(const symbol& _sym) const;
		symbol operator/(const symbol& _sym) const;

	public: /* operation */
		symbol inverse() const;
		symbol power(uint8_t _num) const;
	};

protected: /* block */
	virtual uint8_t input_block() const { return 3; };
	virtual uint8_t encode_block() const { return 7; };

public: /* virtual */
	virtual bool encode();
	virtual bool decode();
}; 