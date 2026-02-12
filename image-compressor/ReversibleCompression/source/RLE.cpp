#include "RLE.h"
#include "HeaderRLE.h"

bool RLE::decode(const std::string& _fname)
{
	HeaderRLE header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderRLE));

	if (header.type != 0x4C52)
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

	for (uint64_t i = 0; i + 3 < comp.size(); i += 4)
	{
		uint64_t runc = comp[i];

		for (uint64_t j = 0; j < runc; j++)
		{
			raw.push_back(comp[i + 1]);
			raw.push_back(comp[i + 2]);
			raw.push_back(comp[i + 3]);
		}
	}

	return true;
}

bool RLE::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	uint64_t max = 765;
	uint64_t runc = 0;
	uint64_t now = 0;
	uint64_t next = 0;

	for (uint64_t i = 0; i + 2 < raw.size(); i += 3)
	{
		now = raw[i];
		now = raw[i + 1] + (now << 8);
		now = raw[i + 2] + (now << 8);

		for (runc = 3; i + runc + 2 < raw.size(); runc += 3)
		{
			next = raw[i + runc];
			next = raw[i + runc + 1] + (next << 8);
			next = raw[i + runc + 2] + (next << 8);

			if (next != now) break;
			if (runc >= max) break;
		}

		comp.push_back(static_cast<uint8_t>(runc / 3));
		comp.push_back(raw[i]);
		comp.push_back(raw[i + 1]);
		comp.push_back(raw[i + 2]);

		i += runc - 3;
	}

	HeaderRLE header;
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