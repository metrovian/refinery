#pragma once
#include "Sieve.h"

class SundaramSieve : public Sieve
{
protected: /* condition */
	std::string max_cond1(std::string _num);
	std::string max_cond2(std::string _num, std::string _mum);

protected: /* virtual */
	bool process_pre(std::string _min, std::string _max) override;
	bool process_post(std::string _min, std::string _max) override;
};