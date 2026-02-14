#include "LZ77.h"
#include "HeaderLZ77.h"

bool LZ77::decode(const std::string& _fname)
{
	HeaderLZ77 header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderLZ77));

	if (header.type != 0x5A4C)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.ver != 0x3737)
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

	for (uint64_t i = 0; i + 3 < comp.size(); i += 4)
	{
		uint16_t mao = comp[i + 1] | (static_cast<uint16_t>(comp[i]) << 8);
		uint8_t mal = comp[i + 2];
		uint8_t sym = comp[i + 3];

		uint64_t start = raw.size() - mao;

		for (uint64_t j = start; j < start + mal; j++)
		{
			raw.push_back(raw[j]);
		}

		raw.push_back(sym);
	}

	return true;
}

bool LZ77::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	uint64_t start = 0;
	uint16_t mao = 0;
	uint8_t mal = 0;
	uint8_t sym = 0;

	comp.clear();

	for (uint64_t i = 0; i < raw.size(); ++i)
	{
		mao = 0;
		mal = 0;
		sym = 0;

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

		if (i + mal < raw.size())
		{
			sym = raw[i + mal];
		}

		comp.push_back(static_cast<uint8_t>((mao >> 8) & 0xFF));
		comp.push_back(static_cast<uint8_t>((mao) & 0xFF));
		comp.push_back(mal);
		comp.push_back(sym);

		i += mal;
	}

	HeaderLZ77 header;
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
