#include "DCT.h"
#include "HeaderDCT.h"

std::vector<uint8_t> DCT::transform(const std::vector<uint8_t>& _bmp)
{
	std::vector<uint8_t> ret(width * height * 3, 0.0);

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pmax = std::sqrt(width * height * 3.0) * 255.0;

	auto clamp = [](double _val)
		{
			if (_val > 255) return static_cast<uint8_t>(255);
			if (_val < 0) return static_cast<uint8_t>(0);

			return static_cast<uint8_t>(_val);
		};

	for (uint64_t u = 0; u < width * 3; ++u)
	{
		for (uint64_t v = 0; v < height; ++v)
		{
			static double pi = 3.1415926;
			double sum = 0.0;
			double norm1 = 0.0;
			double norm2 = 0.0;
			double phase1 = 0.0;
			double phase2 = 0.0;

			for (uint64_t i = 0; i < width * 3; ++i)
			{
				for (uint64_t j = 0; j < height; ++j)
				{
					norm1 = ((u == 0) ? std::sqrt(1.0 / 3.0 / width) : std::sqrt(2.0 / 3.0 / width));
					norm2 = ((v == 0) ? std::sqrt(1.0 / height) : std::sqrt(2.0 / height));

					phase1 = pi * static_cast<double>((2 * i + 1) * u) / static_cast<double>(width * 6);
					phase2 = pi * static_cast<double>((2 * j + 1) * v) / static_cast<double>(height * 2);

					sum += _bmp[j * pwidth + i] * norm1 * norm2 * std::cos(phase1) * std::cos(phase2);
				}
			}

			ret[v * width * 3 + u] = static_cast<uint8_t>(clamp(255.0 * std::log(sum) / std::log(pmax)));
		}
	}

	return ret;
}

std::vector<uint8_t> DCT::inverse(const std::vector<uint8_t>& _dct)
{
	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pmax = std::sqrt(width * height * 3.0) * 255.0;

	std::vector<uint8_t> ret(pwidth * height, 0.0);

	auto clamp = [](double _val)
		{
			if (_val > 255) return static_cast<uint8_t>(255);
			if (_val < 0) return static_cast<uint8_t>(0);

			return static_cast<uint8_t>(_val);
		};

	for (uint64_t i = 0; i < width * 3; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			static double pi = 3.1415926;
			double sup = 0.0;
			double sum = 0.0;
			double norm1 = 0.0;
			double norm2 = 0.0;
			double phase1 = 0.0;
			double phase2 = 0.0;

			for (uint64_t u = 0; u < width * 3; ++u)
			{
				for (uint64_t v = 0; v < height; ++v)
				{
					norm1 = ((u == 0) ? std::sqrt(1.0 / 3.0 / width) : std::sqrt(2.0 / 3.0 / width));
					norm2 = ((v == 0) ? std::sqrt(1.0 / height) : std::sqrt(2.0 / height));

					phase1 = pi * static_cast<double>((2 * i + 1) * u) / static_cast<double>(width * 6);
					phase2 = pi * static_cast<double>((2 * j + 1) * v) / static_cast<double>(height * 2);

					sup = std::pow(pmax, static_cast<double>(_dct[v * width * 3 + u] / 255.0));

					sum += sup * norm1 * norm2 * std::cos(phase1) * std::cos(phase2);
				}
			}

			ret[j * pwidth + i] = clamp(sum);
		}
	}

	return ret;
}

bool DCT::decode(const std::string& _fname)
{
	HeaderDCT header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderDCT));

	if (header.type != 0x4344)
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

	std::vector<uint8_t> dct(width * height * 3, 0);

	uint64_t cwidth = static_cast<double>(width * header.qual) * 3.0 / 100.0;
	uint64_t cheight = static_cast<double>(height * header.qual) / 100.0;

	for (uint64_t i = 0; i < cwidth; ++i)
	{
		for (uint64_t j = 0; j < cheight; ++j)
		{
			dct[j * width * 3 + i] = comp[j * cwidth + i];
		}
	}

	raw.clear();
	raw = inverse(dct);

	return true;
}

bool DCT::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	std::vector<uint8_t> dct = transform(raw);

	HeaderDCT header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	uint64_t cwidth = static_cast<double>(width * header.qual) * 3.0 / 100.0;
	uint64_t cheight = static_cast<double>(height * header.qual) / 100.0;

	comp.clear();
	comp.resize(cwidth * cheight, 0);

	for (uint64_t i = 0; i < cwidth; ++i)
	{
		for (uint64_t j = 0; j < cheight; ++j)
		{
			comp[j * cwidth + i] = dct[j * width * 3 + i];
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
