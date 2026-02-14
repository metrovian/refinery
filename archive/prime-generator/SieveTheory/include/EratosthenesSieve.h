#pragma once
#include "Sieve.h"

class EratosthenesSieve : public Sieve
{
protected: /* virtual */
	bool process_pre(std::string _min, std::string _max) override;
	bool process_post(std::string _min, std::string _max) override;
};