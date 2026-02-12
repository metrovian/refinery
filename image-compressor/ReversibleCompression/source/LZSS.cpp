#include "LZSS.h"
#include "HeaderLZSS.h"

bool LZSS::decode(const std::string& _fname)
{
	HeaderLZSS header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderLZSS));

	if (header.type != 0x5A4C)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.ver != 0x5353)
	{
		std::cerr << "Header Error : " << header.ver << std::endl;
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

	for (uint64_t i = 0; i < comp.size(); ++i)
	{
		uint8_t fcmp = comp[i++];

		if (fcmp == 1)
		{
			uint16_t mao = comp[i + 1] | (static_cast<uint16_t>(comp[i]) << 8);
			uint8_t mal = comp[i + 2];

			uint64_t start = raw.size() - mao;

			for (uint64_t j = 0; j < mal; j++)
			{
				raw.push_back(raw[start + j]);
			}

			i += 2;
		}

		else
		{
			raw.push_back(comp[i]);
		}
	}

	return true;
}

bool LZSS::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	uint64_t start = 0;
	uint16_t mao = 0;
	uint8_t mal = 0;
	uint8_t mul = 3;
	uint8_t fcmp = 0;

	comp.clear();

	for (uint64_t i = 0; i < raw.size(); ++i)
	{
		mao = 0;
		mal = 0;
		start = (i > wsi) ? (i - wsi) : 0;

		for (uint64_t j = start; j < i; ++j)
		{
			for (uint64_t k = 0; i + k < raw.size() && k < bsi; ++k)
			{
				if (raw[i + k] != raw[j + k]) break;

				if (k + 1 > mal)
				{
					mao = static_cast<uint16_t>(i - j);
					mal = static_cast<uint8_t>(k + 1);
				}
			}
		}

		fcmp = static_cast<uint8_t>(mal >= mul);

		if (fcmp)
		{
			comp.push_back(fcmp);
			comp.push_back(static_cast<uint8_t>((mao >> 8) & 0xFF));
			comp.push_back(static_cast<uint8_t>((mao) & 0xFF));
			comp.push_back(mal);

			i += static_cast<uint64_t>(mal - 1);
		}

		else
		{
			comp.push_back(fcmp);
			comp.push_back(raw[i]);
		}
	}

	HeaderLZSS header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
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