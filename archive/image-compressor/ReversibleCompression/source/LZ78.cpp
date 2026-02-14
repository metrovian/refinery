#include "LZ78.h"
#include "HeaderLZ78.h"

bool LZ78::decode(const std::string& _fname)
{
	HeaderLZ78 header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderLZ78));

	if (header.type != 0x5A4C)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.ver != 0x3837)
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

	std::vector<std::string> dict(1, "");
	std::string ptrn = "";

	uint32_t ival = 0;
	uint8_t sym = 0;

	for (uint64_t i = 0; i + 4 < comp.size(); i += 5)
	{
		ival = comp[i];
		ival = comp[i + 1] | (ival << 8);
		ival = comp[i + 2] | (ival << 8);
		ival = comp[i + 3] | (ival << 8);

		sym = comp[i + 4];

		ptrn = dict[ival];
		ptrn += static_cast<char>(sym);

		dict.push_back(ptrn);

		raw.insert(raw.end(), ptrn.begin(), ptrn.end());
	}

	return true;
}

bool LZ78::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	comp.clear();

	uint32_t ival = 0;
	uint32_t dval = 0;

	std::unordered_map<std::string, uint32_t> dict;
	std::string ptrn = "";

	dict[ptrn] = dval++;

	for (uint64_t i = 0; i < raw.size(); ++i)
	{
		ptrn += static_cast<char>(raw[i]);

		if (dict.find(ptrn) == dict.end())
		{
			dict[ptrn] = dval++;

			ival = dict[ptrn.substr(0, ptrn.size() - 1)];

			comp.push_back(static_cast<uint8_t>((ival >> 24) & 0xFF));
			comp.push_back(static_cast<uint8_t>((ival >> 16) & 0xFF));
			comp.push_back(static_cast<uint8_t>((ival >> 8) & 0xFF));
			comp.push_back(static_cast<uint8_t>((ival) & 0xFF));

			comp.push_back(ptrn.back());

			ptrn.clear();
		}
	}

	if (!ptrn.empty())
	{
		ival = dict[ptrn.substr(0, ptrn.size() - 1)];

		comp.push_back(static_cast<uint8_t>((ival >> 24) & 0xFF));
		comp.push_back(static_cast<uint8_t>((ival >> 16) & 0xFF));
		comp.push_back(static_cast<uint8_t>((ival >> 8) & 0xFF));
		comp.push_back(static_cast<uint8_t>((ival) & 0xFF));

		comp.push_back(ptrn.back());
	}

	HeaderLZ78 header;
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
