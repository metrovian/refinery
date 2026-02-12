#include "Block.h"

bool Block::encode()
{
	redc = raw;
	return true;
}

bool Block::decode()
{
	raw = redc;
	return true;
}
