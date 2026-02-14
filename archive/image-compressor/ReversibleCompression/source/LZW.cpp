#include "LZW.h"
#include "HeaderLZW.h"

bool LZW::decode(const std::string& _fname)
{
	HeaderLZW header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderLZW));

	if (header.type != 0x5A4C)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.ver != 0x0057)
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

	std::unordered_map<uint32_t, std::string> dict;

	for (uint64_t i = 0; i < 256; i++)
	{
		dict[i] = std::string(1, static_cast<char>(i));
	}

	uint32_t ival = 0;
	uint32_t pval = 0;
	uint32_t dval = 256;

	std::string cur = dict[pval];
	std::string entry = dict[pval];

	raw.insert(raw.end(), cur.begin(), cur.end());

	pval = comp[0];
	pval = comp[1] | (pval << 8);
	pval = comp[2] | (pval << 8);
	pval = comp[3] | (pval << 8);

	for (uint64_t i = 4; i < comp.size(); i += 4)
	{
		ival = comp[i];
		ival = comp[i + 1] | (ival << 8);
		ival = comp[i + 2] | (ival << 8);
		ival = comp[i + 3] | (ival << 8);

		if (dict.find(ival) != dict.end())
		{
			entry = dict[ival];
		}

		else if (ival == dval)
		{
			entry = dict[pval] + dict[pval].front();
		}

		dict[dval++] = dict[pval] + entry.front();
		pval = ival;

		raw.insert(raw.end(), entry.begin(), entry.end());
	}

	return true;
}

bool LZW::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	comp.clear();

	std::unordered_map<std::string, uint32_t> dict;
	std::string ptrn = "";

	uint32_t ival = 0;
	uint32_t dval = 256;

	for (uint64_t i = 0; i < 256; i++)
	{
		dict[std::string(1, static_cast<char>(i))] = i;
	}

	for (uint64_t i = 0; i < raw.size(); ++i)
	{
		ptrn += static_cast<char>(raw[i]);

		if (dict.find(ptrn) == dict.end())
		{
			dict[ptrn] = dval++;

			ival = dict[ptrn.substr(0, ptrn.size() - 1)];
			ptrn = ptrn.back();

			comp.push_back(static_cast<uint8_t>((ival >> 24) & 0xFF));
			comp.push_back(static_cast<uint8_t>((ival >> 16) & 0xFF));
			comp.push_back(static_cast<uint8_t>((ival >> 8) & 0xFF));
			comp.push_back(static_cast<uint8_t>((ival) & 0xFF));
		}
	}

	if (!ptrn.empty())
	{
		ival = dict[ptrn];

		comp.push_back(static_cast<uint8_t>((ival >> 24) & 0xFF));
		comp.push_back(static_cast<uint8_t>((ival >> 16) & 0xFF));
		comp.push_back(static_cast<uint8_t>((ival >> 8) & 0xFF));
		comp.push_back(static_cast<uint8_t>((ival) & 0xFF));
	}

	HeaderLZW header;
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
