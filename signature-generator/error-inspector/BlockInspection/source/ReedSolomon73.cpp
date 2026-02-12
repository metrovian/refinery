#include "ReedSolomon73.h"

ReedSolomon73::symbol ReedSolomon73::symbol::operator+(const symbol& _sym) const
{
	return symbol(data ^ _sym.data);
}

ReedSolomon73::symbol ReedSolomon73::symbol::operator-(const symbol& _sym) const
{
	return symbol(data ^ _sym.data);
}

ReedSolomon73::symbol ReedSolomon73::symbol::operator*(const symbol& _sym) const
{
	std::bitset<3> ret(0);

	for (uint8_t i = 0; i < 3; ++i)
	{
		for (uint8_t j = 0; j < 3; ++j)
		{
			uint8_t rdeg1 = (i + j) % 3;
			uint8_t rdeg2 = (i + j) % 3 + 1;

			ret[rdeg1] = ret[rdeg1] ^ (data[i] & _sym.data[j]);

			if ((i + j) / 3)
			{
				ret[rdeg2] = ret[rdeg2] ^ (data[i] & _sym.data[j]);
			}
		}
	}

	return symbol(ret);
}

ReedSolomon73::symbol ReedSolomon73::symbol::operator/(const symbol& _sym) const
{
	return (*this) * _sym.inverse();
}

ReedSolomon73::symbol ReedSolomon73::symbol::inverse() const
{
	uint8_t dat = (*this).data.to_ulong();
	uint8_t pow = 0;

	for (uint8_t i = 0; i < 7; ++i)
	{
		pow = symbol(2).power(i).data.to_ulong();

		if (dat == pow)
		{
			return symbol(2).power(7 - i);
		}
	}

	return symbol(1);
}

ReedSolomon73::symbol ReedSolomon73::symbol::power(uint8_t _num) const
{
	symbol ret(1);

	for (uint8_t i = 0; i < _num; ++i)
	{
		ret = ret * (*this);
	}

	return ret;
}

bool ReedSolomon73::encode()
{
	std::vector<uint8_t> ret;
	std::vector<std::bitset<3>> baw = transform<3>(raw);

	while (baw.size() % 3 != 0)
	{
		baw.push_back(std::bitset<3>(0));
	}

	for (uint64_t i = 0; i < baw.size() / 3; ++i)
	{
		std::vector<uint8_t> poly_dat(7, 0);
		std::vector<uint8_t> poly_gen(5, 0);

		poly_dat[4] = baw[i * 3 + 0].to_ulong();
		poly_dat[5] = baw[i * 3 + 1].to_ulong();
		poly_dat[6] = baw[i * 3 + 2].to_ulong();

		poly_gen[0] = symbol(2).power(3).data.to_ulong();
		poly_gen[1] = symbol(2).power(1).data.to_ulong();
		poly_gen[2] = 1;
		poly_gen[3] = symbol(2).power(3).data.to_ulong();
		poly_gen[4] = 1;

		std::vector<uint8_t> poly_rem = poly_dat;
		std::vector<uint8_t> poly_send = poly_dat;

		for (uint64_t j = 0; j < 3; ++j)
		{
			symbol squo = symbol(std::bitset<3>(poly_rem[6 - j]));

			if (squo.data != 0)
			{
				for (uint64_t k = 0; k < 5; ++k)
				{
					symbol srem = symbol(std::bitset<3>(poly_rem[k + (2 - j)]));
					symbol sgen = symbol(std::bitset<3>(poly_gen[k]));

					poly_rem[k + (2 - j)] = (srem + squo * sgen).data.to_ulong();
				}
			}
		}

		for (uint64_t j = 0; j < 7; ++j)
		{
			symbol send = symbol(std::bitset<3>(poly_send[j]));
			symbol rem = symbol(std::bitset<3>(poly_rem[j]));

			poly_send[j] = (send + rem).data.to_ulong();
		}

		ret.insert(ret.end(), poly_send.begin(), poly_send.end());
	}

	redc = ret;
	return true;
}

bool ReedSolomon73::decode()
{
	std::vector<std::bitset<3>> ret;
	
	for (uint64_t i = 0; i < redc.size() / 7; ++i)
	{
		std::vector<symbol> syns(4, symbol(0));
		std::vector<symbol> bedc =
		{
			symbol(std::bitset<3>(redc[i * 7 + 0])),
			symbol(std::bitset<3>(redc[i * 7 + 1])),
			symbol(std::bitset<3>(redc[i * 7 + 2])),
			symbol(std::bitset<3>(redc[i * 7 + 3])),
			symbol(std::bitset<3>(redc[i * 7 + 4])),
			symbol(std::bitset<3>(redc[i * 7 + 5])),
			symbol(std::bitset<3>(redc[i * 7 + 6])),
		};

		for (uint64_t j = 0; j < 7; ++j)
		{
			for (uint64_t k = 0; k < 4; ++k)
			{
				syns[k] = syns[k] + bedc[j] * symbol(2).power(j * (k + 1));
			}
		}

		uint8_t jzero = 0;
		uint8_t jdeg = 0;
		uint8_t fdeg = 0;

		for (uint64_t j = 0; j < 4; ++j)
		{
			if (syns[j].data.to_ulong())
			{
				break;
			}

			++jzero;
		}

		if (jzero == 4)
		{
			ret.push_back(bedc[4].data);
			ret.push_back(bedc[5].data);
			ret.push_back(bedc[6].data);

			continue;
		}

		jdeg = jzero;
		fdeg = jdeg;

		symbol derr = syns[jdeg];
		symbol dorr = derr;

		std::vector<symbol> lamb(1, symbol(1));
		std::vector<symbol> lomb(1, symbol(1));

		lamb.insert(lamb.end(), static_cast<uint64_t>(jdeg + 1), symbol(0));

		for (uint64_t j = static_cast<uint64_t>(jdeg + 1); j < 4; ++j)
		{
			std::vector<symbol> lonb = lamb;
			std::vector<symbol> lomc = lomb;

			uint64_t sdeg = static_cast<uint64_t>(j - fdeg);
			uint64_t ndeg = static_cast<uint64_t>(j - fdeg) + lomb.size();

			lomc.insert(lomc.begin(), sdeg, symbol(0));

			if (ndeg > lonb.size())
			{
				lonb.resize(ndeg, symbol(0));
			}

			derr = syns[j];

			for (uint64_t k = 1; k < lamb.size(); ++k)
			{
				derr = derr + lamb[k] * syns[j - k];
			}

			if (derr.data.to_ulong() == 0)
			{
				continue;
			}

			for (uint64_t k = 0; k < lomc.size(); ++k)
			{
				lonb[k] = lonb[k] + (derr / dorr) * lomc[k];
			}

			int64_t detj = static_cast<int64_t>(j);
			int64_t detf = static_cast<int64_t>(fdeg);

			detj -= static_cast<int64_t>(lamb.size());
			detf -= static_cast<int64_t>(lomb.size());

			if (detj > detf)
			{
				lomb = lamb;
				dorr = derr;

				fdeg = j;
			}

			lamb = lonb;
		}

		std::vector<uint64_t> invs;

		for (uint64_t j = 0; j < 7; ++j)
		{
			symbol sum = symbol(0);
			symbol sinv = (symbol(2).inverse()).power(j);

			for (uint64_t k = 0; k < lamb.size(); ++k)
			{
				sum = sum + lamb[k] * sinv.power(k);
			}

			if (sum.data.to_ulong() == 0)
			{
				invs.push_back(j);
			}
		}
	
		std::vector<symbol> crr(7, symbol(0));
		std::vector<symbol> arr;

		if (invs.size() == 1)
		{
			arr.push_back(syns[0] / symbol(2).power(invs[0]));
		}

		if (invs.size() == 2)
		{
			symbol ans1 = symbol(2).power(invs[0]);
			symbol ans2 = symbol(2).power(invs[1]);

			symbol ansq1 = ans1.power(2);
			symbol ansq2 = ans2.power(2);

			symbol det = ans1 * ansq2 - ans2 * ansq1;

			symbol wans1 = (syns[0] * ansq1 + syns[1] * ans1) / det;
			symbol wans2 = (syns[0] * ansq2 + syns[1] * ans2) / det;

			arr.push_back(wans2);
			arr.push_back(wans1);
		}
		
		for (uint64_t j = 0; j < invs.size(); ++j)
		{
			crr[invs[j]] = crr[invs[j]] + arr[j];
		}

		for (uint64_t j = 0; j < bedc.size(); ++j)
		{
			bedc[j] = bedc[j] + crr[j];
		}

		ret.push_back(bedc[4].data);
		ret.push_back(bedc[5].data);
		ret.push_back(bedc[6].data);
	}

	raw = inverse<3>(ret);
	return true;
}