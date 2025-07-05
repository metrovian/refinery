#pragma once
#include "Sieve.h"

class PritchardSieve : public Sieve
{
protected: /* range */
	static bool less_pair(const std::pair<std::string, std::string>& _num, const std::pair<std::string, std::string>& _min);
	static bool greater_pair(const std::pair<std::string, std::string>& _num, const std::pair<std::string, std::string>& _min);

protected: /* data */
	std::priority_queue
		<
		std::pair<std::string, std::string>,
		std::vector<std::pair<std::string, std::string>>,
		decltype(&PritchardSieve::greater_pair)
		> cps;

protected: /* virtual */
	bool process_pre(std::string _min, std::string _max) override;
	bool process_post(std::string _min, std::string _max) override;
};