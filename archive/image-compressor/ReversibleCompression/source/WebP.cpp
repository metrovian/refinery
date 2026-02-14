#include "WebP.h"
#include "HeaderWebP.h"

#include "webp/encode.h"
#include "webp/decode.h"

std::vector<uint8_t> WebP::transform(const std::vector<uint8_t>& _bmp)
{
	std::vector<uint8_t> ret = _bmp;

	uint64_t pwidth = (width * 3 + 3) & ~3;

	for (uint64_t i = 0; i < width; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			uint64_t bmpi = (height - j - 1) * pwidth + i * 3;
			uint64_t webpi = (j * width + i) * 3;

			ret[webpi + 0] = _bmp[bmpi + 0];
			ret[webpi + 1] = _bmp[bmpi + 1];
			ret[webpi + 2] = _bmp[bmpi + 2];
		}
	}

	return ret;
}

std::vector<uint8_t> WebP::inverse(const std::vector<uint8_t>& _webp)
{
	std::vector<uint8_t> ret;
	std::vector<uint8_t> fwebp;

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pad = pwidth - width * 3;

	for (uint64_t i = 0; i < height; ++i)
	{
		fwebp.insert(fwebp.end(), _webp.begin() + i * (width * 3), _webp.begin() + (i + 1) * (width * 3));
		fwebp.insert(fwebp.end(), pad, 0);
	}

	ret.resize(fwebp.size());

	for (uint64_t i = 0; i < width; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			uint64_t bmpi = (height - j - 1) * pwidth + i * 3;
			uint64_t webpi = (j * pwidth) + i * 3;

			ret[bmpi + 0] = fwebp[webpi + 0];
			ret[bmpi + 1] = fwebp[webpi + 1];
			ret[bmpi + 2] = fwebp[webpi + 2];
		}
	}

	return ret;
}

bool WebP::decode(const std::string& _fname)
{
	HeaderWebP header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderWebP));
	ifs.seekg(0, ifs.beg);

	if (header.rf != 0x46464952)
	{
		std::cerr << "Header Error : " << header.rf << std::endl;
		return false;
	}

	if (header.type != 0x50424557)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	comp.resize(static_cast<uint64_t>(header.fsi + 8));

	ifs.read(reinterpret_cast<char*>(comp.data()), static_cast<uint64_t>(header.fsi + 8));
	ifs.close();

	int dret = 0;
	int dwid = 0;
	int dhei = 0;

	uint8_t* input = nullptr;

	dret = WebPGetInfo(comp.data(), comp.size(), &dwid, &dhei);

	if (!dret)
	{
		std::cerr << "Decode Error : " << dret << std::endl;
		return false;
	}

	input = WebPDecodeBGR(comp.data(), comp.size(), &dwid, &dhei);

	if (!input)
	{
		std::cerr << "Decode Error : " << input << std::endl;
		return false;
	}

	width = dwid;
	height = dhei;

	raw.clear();
	raw.insert(raw.end(), input, input + (width * height * 3));

	raw = inverse(raw);

	WebPFree(input);

	return true;
}

bool WebP::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	raw = transform(raw);

	uint8_t* output = nullptr;
	uint64_t dsi = WebPEncodeLosslessBGR(raw.data(), width, height, width * 3, &output);

	if (dsi == 0)
	{
		std::cerr << "Encode Error : " << dsi << std::endl;
		return false;
	}

	HeaderWebP header;
	std::ofstream ofs(_fname, std::ios::binary);

	std::memcpy(&header, output, sizeof(HeaderWebP));

	comp.clear();
	comp.insert(comp.end(), output + sizeof(HeaderWebP), output + dsi);

	WebPFree(output);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));
	ofs.write(reinterpret_cast<const char*>(comp.data()), comp.size());
	ofs.close();

	return true;
}
