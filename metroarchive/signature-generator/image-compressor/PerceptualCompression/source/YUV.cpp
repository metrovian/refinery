#include "YUV.h"
#include "HeaderYUV.h"

std::vector<uint8_t> YUV::transform(const std::vector<uint8_t>& _bmp)
{
	std::vector<uint8_t> ret = _bmp;

	uint64_t pwidth = (width * 3 + 3) & ~3;

	for (uint64_t i = 0; i < width; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			uint64_t bmpi = (height - j - 1) * pwidth + i * 3;
			uint64_t yuvi = (j * width + i) * 3;

			uint8_t scb = _bmp[bmpi + 0];
			uint8_t scg = _bmp[bmpi + 1];
			uint8_t scr = _bmp[bmpi + 2];

			ret[yuvi + 0] = static_cast<uint8_t>(16 + (0.257 * scr) + (0.504 * scg) + (0.098 * scb));
			ret[yuvi + 1] = static_cast<uint8_t>(128 - (0.148 * scr) - (0.291 * scg) + (0.439 * scb));
			ret[yuvi + 2] = static_cast<uint8_t>(128 + (0.439 * scr) - (0.368 * scg) - (0.071 * scb));
		}
	}

	return ret;
}

std::vector<uint8_t> YUV::inverse(const std::vector<uint8_t>& _yuv)
{
	std::vector<uint8_t> ret;
	std::vector<uint8_t> fyuv;

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pad = pwidth - width * 3;

	for (uint64_t i = 0; i < height; ++i)
	{
		fyuv.insert(fyuv.end(), _yuv.begin() + i * (width * 3), _yuv.begin() + (i + 1) * (width * 3));
		fyuv.insert(fyuv.end(), pad, 0);
	}

	ret.resize(fyuv.size());

	for (uint64_t i = 0; i < width; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			uint64_t bmpi = (height - j - 1) * pwidth + i * 3;
			uint64_t yuvi = (j * pwidth) + i * 3;

			int64_t scy = static_cast<int64_t>(fyuv[yuvi + 0]) - 16;
			int64_t scb = static_cast<int64_t>(fyuv[yuvi + 1]) - 128;
			int64_t scr = static_cast<int64_t>(fyuv[yuvi + 2]) - 128;

			auto clamp = [](int64_t _val)
				{
					if (_val > 255) return static_cast <uint8_t>(255);
					if (_val < 0) return static_cast <uint8_t>(0);

					return static_cast<uint8_t>(_val);
				};

			ret[bmpi + 0] = clamp(static_cast<int64_t>(1.164 * scy + 2.017 * scb));
			ret[bmpi + 1] = clamp(static_cast<int64_t>(1.164 * scy - 0.813 * scr - 0.392 * scb));
			ret[bmpi + 2] = clamp(static_cast<int64_t>(1.164 * scy + 1.596 * scr));
		}
	}

	return ret;
}

bool YUV::decode(const std::string& _fname)
{
	HeaderYUV header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderYUV));

	if (header.type != 0x5559)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.depth != 0x0018)
	{
		std::cerr << "Header Error : " << header.depth << std::endl;
		return false;
	}

	if (header.qdy != 0x0004)
	{
		std::cerr << "Header Error : " << header.qdy << std::endl;
		return false;
	}

	if (header.qdb != 0x0002)
	{
		std::cerr << "Header Error : " << header.qdb << std::endl;
		return false;
	}

	if (header.qdr != 0x0002)
	{
		std::cerr << "Header Error : " << header.qdr << std::endl;
		return false;
	}

	width = header.width;
	height = header.height;

	comp.resize(header.dsi);

	ifs.read(reinterpret_cast<char*>(comp.data()), header.dsi);
	ifs.close();

	raw.clear();

	uint64_t ptr = width * height;

	std::vector<uint8_t> scy(comp.begin(), comp.begin() + ptr);
	std::vector<uint8_t> scb;
	std::vector<uint8_t> scr;

	for (uint64_t i = 0; i < height; ++i)
	{
		for (uint64_t j = 0; j * 2 < width; ++j)
		{
			if (j * 2 + 1 < width)
			{
				scb.push_back(comp[ptr + j]);
				scb.push_back(comp[ptr + j]);
			}

			else
			{
				scb.push_back(comp[ptr + j]);
			}
		}

		ptr += width / 2 + width % 2;
	}

	for (uint64_t i = 0; i < height; ++i)
	{
		for (uint64_t j = 0; j * 2 < width; ++j)
		{
			if (j * 2 + 1 < width)
			{
				scr.push_back(comp[ptr + j]);
				scr.push_back(comp[ptr + j]);
			}

			else
			{
				scr.push_back(comp[ptr + j]);
			}
		}

		ptr += width / 2 + width % 2;
	}

	std::vector<uint8_t> yuv;

	for (uint64_t i = 0; i < scy.size(); ++i)
	{
		yuv.push_back(scy[i]);
		yuv.push_back(scb[i]);
		yuv.push_back(scr[i]);
	}

	raw = inverse(yuv);

	return true;
}

bool YUV::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	std::vector<uint8_t> yuv = transform(raw);

	std::vector<uint8_t> scy;
	std::vector<uint8_t> scb;
	std::vector<uint8_t> scr;

	for (uint64_t i = 0; i < width * height; ++i)
	{
		scy.push_back(yuv[i * 3]);
		scb.push_back(yuv[i * 3 + 1]);
		scr.push_back(yuv[i * 3 + 2]);
	}

	comp.clear();
	comp.insert(comp.end(), scy.begin(), scy.end());

	uint64_t ptr = 0;
	uint16_t mean = 0;

	for (uint64_t i = 0; i < height; i += 1)
	{
		for (uint64_t j = 0; j < width; j += 2)
		{
			ptr = i * width + j;

			if (j + 1 < width)
			{
				mean = static_cast<uint16_t>(scb[ptr]) + static_cast<uint16_t>(scb[ptr + 1]);
				mean /= 2;

				comp.push_back(mean);
			}

			else
			{
				comp.push_back(scb[ptr]);
			}
		}
	}

	for (uint64_t i = 0; i < height; i += 1)
	{
		for (uint64_t j = 0; j < width; j += 2)
		{
			ptr = i * width + j;

			if (j + 1 < width)
			{
				mean = static_cast<uint16_t>(scr[ptr]) + static_cast<uint16_t>(scr[ptr + 1]);
				mean /= 2;

				comp.push_back(mean);
			}

			else
			{
				comp.push_back(scr[ptr]);
			}
		}
	}

	HeaderYUV header;
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
