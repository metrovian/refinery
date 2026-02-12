#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>

class Sieve 
{
protected: /* data */
	std::stringstream data;
	std::stringstream primes;
	
protected: /* condition */
	std::vector<bool> conds;

protected: /* number */
	uint64_t count = 0;
	uint64_t unit = UINT16_MAX;
	
protected: /* format */
	static std::string addcomma(const std::string& _num);

protected: /* modular */
	static std::string mod(const std::string& _num, const std::string& _mod);
	static std::string modplus(const std::string& _num, const std::string& _mum, const std::string& _mod);
	static std::string modsub(const std::string& _num, const std::string& _mum, const std::string& _mod);
	static std::string modcross(const std::string& _num, const std::string& _mum, const std::string& _mod);

protected: /* operator */
	static std::string plus(const std::string& _num, const std::string& _mum);
	static std::string sub(const std::string& _num, const std::string& _mum);
	static std::string cross(const std::string& _num, const std::string& _mum);
	static std::string div(const std::string& _num, const std::string& _mum);
	static std::string increase(const std::string& _num);
	static std::string square(const std::string& _num);

protected: /* range */
	static bool upto(const std::string& _num, const std::string& _max);
	static bool less(const std::string& _num, const std::string& _max);
	static bool least(const std::string& _num, const std::string& _min);
	static bool greater(const std::string& _num, const std::string& _min);
	static bool between(const std::string& _num, const std::string& _min, const std::string& _max);

protected: /* transform */
	static uint64_t ntoi(const std::string& _num, const std::string& _min);
	static std::string iton(const std::string& _idx, const std::string& _min);
	static std::string iton(const uint64_t _idx, const std::string& _min);
		
protected: /* control */
	bool set(std::string _min, std::string _max);
	bool select(std::string _min, std::string _max);
	bool save(std::string _fname);

public: /* public */
	bool run(std::string _max, std::string _fname);

protected: /* virtual */
	virtual bool process_pre(std::string _min, std::string _max) = 0;
	virtual bool process_post(std::string _min, std::string _max) = 0;
};