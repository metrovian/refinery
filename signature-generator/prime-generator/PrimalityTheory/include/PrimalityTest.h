#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <functional>

enum class PRIMALITY_RESULT
{
	FALSE,
	TRUE,
	PROBABLY_FALSE,
	PROBABLY_TRUE,
};

enum class PRIMALITY_TYPE
{
	FERMAT,
	LUCAS,
	MILLER_RABIN,
	SOLOVAY_STRASSEN,
};

typedef std::function<PRIMALITY_RESULT(uint64_t)> PRIMALITY_TEST;
typedef std::vector<PRIMALITY_TEST> PRIMALITY_SET;

class PrimalityTest
{
protected: /* data */
	PRIMALITY_SET data;

public: /* operation */
	bool push_back(PRIMALITY_TYPE _name, uint64_t _base);
	bool pop_back();
	bool clear();

public: /* public */
	bool is_prime(uint64_t _num);
};