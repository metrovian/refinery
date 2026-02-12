#include "GR.h"
#include "HeaderGR.h"

bool GR::decode(const std::string& _fname)
{
	HeaderGR header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderGR));

	if (header.type != 0x5247)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.depth != 0x0018)
	{
		std::cerr << "Header Error : " << header.depth << std::endl;
		return false;
	}

	width = header.width;
	height = header.height;

	comp.resize(header.dsi);

	ifs.read(reinterpret_cast<char*>(comp.data()), header.dsi);
	ifs.close();

	raw.clear();

	uint64_t curi = 0;
	uint64_t curp = 0;

	auto read_unary = [&]()
		{
			if (curi < comp.size())
			{
				int64_t ret = static_cast<int64_t>((comp[curi] >> (7 - curp)) & 1);

				if (++curp == 8)
				{
					curp = 0;
					++curi;
				}

				return ret;
			}

			return static_cast<int64_t>(-1);
		};

	auto read_binary = [&](uint64_t _bits)
		{
			int64_t ret = 0;

			for (uint64_t i = 0; i < _bits; ++i)
			{
				int64_t rval = read_unary();

				if (rval >= 0)
				{
					ret = (ret << 1) | rval;
				}

				else return static_cast<int64_t>(-1);
			}

			return ret;
		};

	while (1)
	{
		int64_t quo = 0;
		int64_t rem = 0;

		while (1)
		{
			int64_t rval = read_unary();

			if (rval < 0) return true;
			if (rval == 0) break;

			++quo;
		}

		rem = read_binary(header.grk);

		if (rem < 0)
		{
			return true;
		}

		raw.push_back(quo * header.grm + rem);
	}
}

bool GR::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	comp.clear();

	uint8_t curb = 0;
	uint64_t curp = 0;

	auto write_unary = [&](bool _bit)
		{
			if (_bit)
			{
				curb |= (1 << (7 - curp));
			}

			if (++curp == 8)
			{
				comp.push_back(curb);

				curb = 0;
				curp = 0;
			}
		};

	auto write_binary = [&](uint64_t _val, uint64_t _bits)
		{
			for (int64_t i = _bits - 1; i >= 0; --i)
			{
				write_unary((_val >> i) & 1);
			}
		};

	HeaderGR header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	for (uint8_t num : raw)
	{
		int64_t quo = num / header.grm;
		int64_t rem = num % header.grm;

		for (uint64_t i = 0; i < quo; ++i)
		{
			write_unary(1);
		}

		write_unary(0);
		write_binary(rem, header.grk);
	}

	if (curp > 0)
	{
		comp.push_back(curb);
	}

	header.width = width;
	header.height = height;

	header.fsi = comp.size() + header.ofs;
	header.dsi = comp.size();

	ofs.write(reinterpret_cast<const char*>(&header), header.ofs);
	ofs.write(reinterpret_cast<const char*>(comp.data()), header.dsi);
	ofs.close();

	return true;
}
