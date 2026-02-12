#include "HAAR.h"
#include "HeaderHAAR.h"

std::vector<uint8_t> HAAR::transform(const std::vector<uint8_t>& _bmp)
{
	std::vector<uint8_t> ret;
	std::vector<uint8_t> pmp = _bmp;

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pheight = (height + 1) & ~1;

	if (height % 2)
	{
		pmp.insert(pmp.end(), pwidth, 0);
	}

	std::vector<int16_t> fret(pwidth * pheight, 0);
	std::vector<int16_t> sret(pwidth * pheight, 0);

	for (uint64_t i = 0; i < pwidth; i += 1)
	{
		for (uint64_t j = 0; j < pheight; j += 2)
		{
			int16_t pval1 = static_cast<int16_t>(pmp[j * pwidth + i]);
			int16_t pval2 = static_cast<int16_t>(pmp[(j + 1) * pwidth + i]);

			uint64_t idx1 = (j / 2) * pwidth + i;
			uint64_t idx2 = (j / 2 + pheight / 2) * pwidth + i;

			fret[idx1] = (pval1 + pval2) / 4;
			fret[idx2] = (pval1 - pval2) / 4;
		}
	}

	for (uint64_t i = 0; i < pwidth; i += 2)
	{
		for (uint64_t j = 0; j < pheight; j += 1)
		{
			int16_t pval1 = static_cast<int16_t>(fret[j * pwidth + i]);
			int16_t pval2 = static_cast<int16_t>(fret[j * pwidth + i + 1]);

			uint64_t idx1 = j * pwidth + (i / 2);
			uint64_t idx2 = j * pwidth + (i / 2 + pwidth / 2);

			sret[idx1] = (pval1 + pval2) / 2;
			sret[idx2] = (pval1 - pval2) / 2;
		}
	}

	for (uint64_t i = 0; i < sret.size(); ++i)
	{
		ret.push_back(sret[i] + 64);
	}

	return ret;
}

std::vector<uint8_t> HAAR::inverse(const std::vector<uint8_t>& _dwt)
{
	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pheight = (height + 1) & ~1;

	std::vector<uint8_t> ret(pwidth * pheight, 0);
	std::vector<int16_t> sret;

	auto clamp = [](int16_t _val)
		{
			if (_val > 255) return static_cast<uint8_t>(255);
			if (_val < 0) return static_cast<uint8_t>(0);

			return static_cast<uint8_t>(_val);
		};

	for (uint64_t i = 0; i < _dwt.size(); ++i)
	{
		sret.push_back(static_cast<int16_t>(_dwt[i] - 64) << 1);
	}

	for (uint64_t i = 0; i + 1 < pwidth; i += 2)
	{
		for (uint64_t j = 0; j + 1 < pheight; j += 2)
		{
			uint64_t idx1 = (j / 2) * pwidth + (i / 2);
			uint64_t idx2 = (j / 2) * pwidth + (i / 2 + pwidth / 2);
			uint64_t idx3 = (j / 2 + pheight / 2) * pwidth + (i / 2);
			uint64_t idx4 = (j / 2 + pheight / 2) * pwidth + (i / 2 + pwidth / 2);

			ret[j * pwidth + i] = clamp(sret[idx1] + sret[idx2] + sret[idx3] + sret[idx4]);
			ret[j * pwidth + (i + 1)] = clamp(sret[idx1] - sret[idx2] + sret[idx3] - sret[idx4]);
			ret[(j + 1) * pwidth + i] = clamp(sret[idx1] + sret[idx2] - sret[idx3] - sret[idx4]);
			ret[(j + 1) * pwidth + (i + 1)] = clamp(sret[idx1] - sret[idx2] - sret[idx3] + sret[idx4]);
		}
	}

	if (height % 2)
	{
		ret.erase(ret.end() - pwidth, ret.end());
	}

	return ret;
}

bool HAAR::decode(const std::string& _fname)
{
	HeaderHAAR header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderHAAR));

	if (header.type != 0x4148)
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

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pheight = (height + 1) & ~1;

	std::vector<uint8_t> dwt(pwidth * pheight, 64);

	for (uint64_t i = 0; i < pwidth; ++i)
	{
		for (uint64_t j = 0; j < pheight / 2; ++j)
		{
			dwt[j * pwidth + i] = comp[j * pwidth + i];
		}

		for (uint64_t j = pheight / 2; j < pheight / 2 + pheight / 4 + pheight % 4; ++j)
		{
			uint64_t edx = (j * 2 - pheight / 2) * pwidth + i;
			uint64_t odx = (j * 2 - pheight / 2 + 1) * pwidth + (i - pwidth / 2);

			if (j * pwidth + i < comp.size())
			{
				if (i < pwidth / 2)
				{
					dwt[edx] = comp[j * pwidth + i];
				}

				else
				{
					dwt[odx] = comp[j * pwidth + i];
				}
			}
		}
	}

	raw.clear();
	raw = inverse(dwt);

	return true;
}

bool HAAR::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	std::vector<uint8_t> dwt = transform(raw);

	HeaderHAAR header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pheight = (height + 1) & ~1;

	comp.clear();

	for (uint64_t i = 0; i < dwt.size(); ++i)
	{
		uint64_t wdx = i % pwidth;
		uint64_t hdx = i / pwidth;

		if (wdx < pwidth / 2 || hdx < pheight / 2)
		{
			comp.push_back(dwt[i]);
		}
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
