#include "Sieve.h"

std::string Sieve::addcomma(const std::string& _num)
{
	if (_num.length() < 4) return _num;

	uint64_t ptr = _num.length();
	uint64_t str = 0;

	std::string ret = _num;

	while (ptr > 3)
	{
		ptr -= 3;
		ret.insert(ret.begin() + ptr, ',');
	}

	return ret;
}

std::string Sieve::mod(const std::string& _num, const std::string& _mod)
{
	if (_num.empty() || _mod.empty()) return "";

	if (_mod == "0") return "";
	if (_mod == "1") return "0";

	if (_mod == "-") return _num;

	try
	{
		uint64_t num = std::stoull(_num);
		uint64_t mod = std::stoull(_mod);

		return std::to_string(num % mod);
	}

	catch (const std::out_of_range& err)
	{
		std::string ret = "";

		uint64_t nlen = _num.length();
		uint64_t mlen = _mod.length();

		for (uint64_t i = 0; i < nlen; ++i)
		{
			ret.push_back(_num[i]);
			ret.erase(0, ret.find_first_not_of('0'));

			if (ret.empty()) ret = "0";

			if (modsub(ret, _mod, "-").empty()) continue;

			for (uint64_t j = 9; j > 0; --j)
			{
				std::string sub = modcross(_mod, std::to_string(j), "-");
				std::string res = modsub(ret, sub, "-");

				if (!res.empty())
				{
					ret = res;
					break;
				}
			}
		}

		return ret;
	}
}

std::string Sieve::modplus(const std::string& _num, const std::string& _mum, const std::string& _mod)
{
	uint64_t nlen = _num.length();
	uint64_t mlen = _mum.length();

	uint64_t plen = std::min(nlen, mlen);
	uint64_t flen = std::max(nlen, mlen) + 1;

	std::string ret(flen, '0');

	for (uint64_t i = 0; i < plen; ++i)
	{
		char num = _num[nlen - i - 1] - '0';
		char mum = _mum[mlen - i - 1] - '0';

		char res = num + mum;

		ret[flen - i - 1] += res % 10;
		ret[flen - i - 2] += res / 10;

		if (ret[flen - i - 1] > '9')
		{
			ret[flen - i - 1] -= 10;
			ret[flen - i - 2] += 1;
		}
	}

	if (mlen == plen)
	{
		for (uint64_t i = plen; i < flen - 1; ++i)
		{
			ret[flen - i - 1] += _num[flen - i - 2] - '0';

			if (ret[flen - i - 1] > '9')
			{
				ret[flen - i - 1] -= 10;
				ret[flen - i - 2] += 1;
			}
		}
	}

	else
	{
		for (uint64_t i = plen; i < flen - 1; ++i)
		{
			ret[flen - i - 1] += _mum[flen - i - 2] - '0';

			if (ret[flen - i - 1] > '9')
			{
				ret[flen - i - 1] -= 10;
				ret[flen - i - 2] += 1;
			}
		}
	}

	ret.erase(0, ret.find_first_not_of('0'));
	ret.shrink_to_fit();

	return mod(ret, _mod);
}

std::string Sieve::modsub(const std::string& _num, const std::string& _mum, const std::string& _mod)
{
	uint64_t nlen = _num.length();
	uint64_t mlen = _mum.length();

	if (nlen < mlen) return mod("", _mod);

	if (nlen == mlen)
	{
		if (_num == _mum) return mod("0", _mod);

		for (uint64_t i = 0; i < mlen; ++i)
		{
			if (_num[i] < _mum[i]) return mod("", _mod);
			if (_num[i] > _mum[i]) break;
		}
	}

	uint64_t plen = std::min(nlen, mlen);
	uint64_t flen = std::max(nlen, mlen) + 1;

	std::string ret(flen, '0');

	for (uint64_t i = 0; i < plen; ++i)
	{
		char num = _num[nlen - i - 1] - '0';
		char mum = _mum[mlen - i - 1] - '0';

		char res = num - mum;

		ret[flen - i - 1] += res;

		if (res < 0)
		{
			ret[flen - i - 1] += 10;
			ret[flen - i - 2] -= 1;
		}

		if (ret[flen - i - 1] < '0')
		{
			ret[flen - i - 1] += 10;
			ret[flen - i - 2] -= 1;
		}
	}

	for (uint64_t i = plen; i < flen - 1; ++i)
	{
		ret[flen - i - 1] += _num[flen - i - 2] - '0';

		if (ret[flen - i - 1] < '0')
		{
			ret[flen - i - 1] += 10;
			ret[flen - i - 2] -= 1;
		}
	}

	ret.erase(0, ret.find_first_not_of('0'));
	ret.shrink_to_fit();

	return mod(ret, _mod);
}

std::string Sieve::modcross(const std::string& _num, const std::string& _mum, const std::string& _mod)
{
	uint64_t nlen = _num.length();
	uint64_t mlen = _mum.length();

	try
	{
		uint64_t tnum = std::stoull(_num);
		uint64_t tmum = std::stoull(_mum);

		return mod(std::to_string(tnum * tmum), _mod);
	}

	catch (const std::out_of_range& err)
	{
		std::string ret = "";
		std::string rep = "";
		std::string req = "";

		for (uint64_t i = 0; i < nlen; ++i)
		{
			req.resize(mlen + 1, '0');

			for (uint64_t j = 0; j < mlen; ++j)
			{
				char num = _num[nlen - i - 1] - '0';
				char mum = _mum[mlen - j - 1] - '0';

				char res = num * mum;

				req[mlen - j] += res % 10;
				req[mlen - j - 1] += res / 10;

				if (req[mlen - j] > '9')
				{
					req[mlen - j] -= 10;
					req[mlen - j - 1] += 1;
				}
			}

			ret = modplus(ret, req + rep, _mod);

			rep.push_back('0');
			req.clear();
		}

		ret.erase(0, ret.find_first_not_of('0'));
		ret.shrink_to_fit();

		return mod(ret, _mod);
	}
}

std::string Sieve::plus(const std::string& _num, const std::string& _mum)
{
	return modplus(_num, _mum, "-");
}

std::string Sieve::sub(const std::string& _num, const std::string& _mum)
{
	return modsub(_num, _mum, "-");
}

std::string Sieve::cross(const std::string& _num, const std::string& _mum)
{
	return modcross(_num, _mum, "-");
}

std::string Sieve::div(const std::string& _num, const std::string& _mum)
{
	if (_num.empty() || _mum.empty()) return "";

	uint64_t nlen = _num.length();
	uint64_t mlen = _mum.length();

	if (nlen < mlen) return "0";

	if (nlen == mlen)
	{
		if (_num == _mum) return "1";

		for (uint64_t i = 0; i < mlen; ++i)
		{
			if (_num[i] < _mum[i]) return "0";
			if (_num[i] > _mum[i]) break;
		}
	}

	try
	{
		uint64_t num = std::stoull(_num);
		uint64_t mum = std::stoull(_mum);

		return std::to_string(num / mum);
	}

	catch (const std::out_of_range& err)
	{
		std::string ret = "";
		std::string rem = "";

		uint64_t nlen = _num.length();
		uint64_t mlen = _mum.length();

		for (uint64_t i = 0; i < nlen; ++i)
		{
			rem.push_back(_num[i]);

			rem.erase(0, rem.find_first_not_of('0'));
			ret.erase(0, ret.find_first_not_of('0'));

			if (rem.empty()) rem = "0";

			if (modsub(rem, _mum, "-").empty())
			{
				ret.push_back('0');
				continue;
			}

			for (uint64_t j = 9; j > 0; --j)
			{
				std::string sub = modcross(_mum, std::to_string(j), "-");
				std::string res = modsub(rem, sub, "-");

				if (!res.empty())
				{
					rem = res;
					ret.push_back(static_cast<char>(j) + '0');

					break;
				}
			}
		}

		return ret;
	}
}

std::string Sieve::increase(const std::string& _num)
{
	return plus(_num, "1");
}

std::string Sieve::square(const std::string& _num)
{
	return cross(_num, _num);
}

bool Sieve::upto(const std::string& _num, const std::string& _max)
{
	return !sub(_max, _num).empty();
}

bool Sieve::less(const std::string& _num, const std::string& _max)
{
	return sub(_num, _max).empty();
}

bool Sieve::least(const std::string& _num, const std::string& _min)
{
	return !sub(_num, _min).empty();
}

bool Sieve::greater(const std::string& _num, const std::string& _min)
{
	return sub(_min, _num).empty();
}

bool Sieve::between(const std::string& _num, const std::string& _min, const std::string& _max)
{
	return least(_num, _min) && less(_num, _max);
}

uint64_t Sieve::ntoi(const std::string& _num, const std::string& _min)
{
	return std::stoull(sub(_num, _min));
}

std::string Sieve::iton(const std::string& _idx, const std::string& _min)
{
	return plus(_idx, _min);
}

std::string Sieve::iton(const uint64_t _idx, const std::string& _min)
{
	return plus(std::to_string(_idx), _min);
}

bool Sieve::set(std::string _min, std::string _max)
{
	if (ntoi(_max, _min) > unit) return false;

	conds.clear();
	conds.resize(unit, true);

	return true;
}

bool Sieve::select(std::string _min, std::string _max)
{
	if (!process_pre(_min, _max)) return false;
	if (!process_post(_min, _max)) return false;

	return true;
}

bool Sieve::save(std::string _fname)
{
	std::ofstream ofs(_fname);

	if (ofs.is_open())
	{
		ofs << primes.rdbuf();
		ofs.close();

		return true;
	}

	return false;
}

bool Sieve::run(std::string _max, std::string _fname)
{
	std::string min = std::to_string(2);
	std::string max = std::to_string(2 + unit);

	std::string rep = plus(_max, "1");

	bool cond = true;

	while (cond)
	{
		if (less(rep, max))
		{
			cond = false;
			max = rep;
		}

		set(min, max);
		select(min, max);

		std::cout << "\u03C0(" << addcomma(sub(max, "1")) << ") = " << addcomma(std::to_string(count)) << std::endl;

		min = max;
		max = plus(max, std::to_string(unit));
	}

	return save(_fname);
}
