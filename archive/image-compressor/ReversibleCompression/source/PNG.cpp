#include "PNG.h"
#include "HeaderPNG.h"

#include "zlib.h"

bool PNG::chunk::read(std::ifstream& _ifs)
{
	if (!_ifs.is_open()) return false;

	uint32_t vrc = 0;

	_ifs.read(reinterpret_cast<char*>(&len), 4);

	len = ntohl(len);

	type.resize(4);
	data.resize(len);

	_ifs.read(&type.front(), 4);

	_ifs.read(reinterpret_cast<char*>(data.data()), len);
	_ifs.read(reinterpret_cast<char*>(&crc), 4);

	crc = ntohl(crc);

	vrc = crc32(0, reinterpret_cast<const unsigned char*>(type.c_str()), 4);

	if (data.size())
	{
		vrc = crc32(vrc, data.data(), data.size());
	}

	if (crc != vrc) return false;
	return true;
}

bool PNG::chunk::write(std::ofstream& _ofs, const std::string& _type, const std::vector<uint8_t>& _data)
{
	if (!_ofs.is_open()) return false;

	type = _type;
	data = _data;
	len = _data.size();

	uint32_t blen = htonl(len);
	uint32_t bcrc = 0;

	_ofs.write(reinterpret_cast<const char*>(&blen), 4);
	_ofs.write(type.c_str(), 4);

	crc = crc32(0, reinterpret_cast<const unsigned char*>(type.c_str()), 4);

	if (data.size())
	{
		_ofs.write(reinterpret_cast<const char*>(data.data()), len);
		crc = crc32(crc, data.data(), len);
	}

	bcrc = htonl(crc);

	_ofs.write(reinterpret_cast<const char*>(&bcrc), 4);

	return true;
}

std::vector<uint8_t> PNG::transform(const std::vector<uint8_t>& _bmp)
{
	std::vector<uint8_t> ret;
	std::vector<uint8_t> swap = _bmp;

	uint64_t pwidth = (width * 3 + 3) & ~3;

	for (uint64_t i = 0; i < width; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			uint64_t bmpi = (height - j - 1) * pwidth + i * 3;
			uint64_t pngi = (j * width + i) * 3;

			swap[pngi + 0] = _bmp[bmpi + 2];
			swap[pngi + 1] = _bmp[bmpi + 1];
			swap[pngi + 2] = _bmp[bmpi + 0];
		}
	}

	for (uint64_t i = 0; i < height; ++i)
	{
		ret.push_back(0);
		ret.insert(ret.end(), swap.begin() + (i * width * 3), swap.begin() + ((i + 1) * width * 3));
	}

	return ret;
}

std::vector<uint8_t> PNG::inverse(const std::vector<uint8_t>& _png)
{
	std::vector<uint8_t> ret;
	std::vector<uint8_t> fpng;

	uint64_t pwidth = (width * 3 + 3) & ~3;
	uint64_t pad = pwidth - width * 3;

	for (uint64_t i = 0; i < height; ++i)
	{
		fpng.insert(fpng.end(), _png.begin() + (i * (width * 3 + 1) + 1), _png.begin() + ((i + 1) * (width * 3 + 1)));
		fpng.insert(fpng.end(), pad, 0);
	}

	ret.resize(fpng.size());

	for (uint64_t i = 0; i < width; ++i)
	{
		for (uint64_t j = 0; j < height; ++j)
		{
			uint64_t bmpi = (height - j - 1) * pwidth + i * 3;
			uint64_t pngi = (j * pwidth) + i * 3;

			ret[bmpi + 0] = fpng[pngi + 2];
			ret[bmpi + 1] = fpng[pngi + 1];
			ret[bmpi + 2] = fpng[pngi + 0];
		}
	}

	return ret;
}

bool PNG::decode(const std::string& _fname)
{
	HeaderPNG header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	uint64_t type = 0;

	chunk ihdr;
	chunk idat;
	chunk iend;

	ifs.read(reinterpret_cast<char*>(&type), 8);

	if (type != 0x0A1A0A0D474E5089)
	{
		std::cerr << "Header Error : " << type << std::endl;
		return false;
	}

	if (!ihdr.read(ifs) || !idat.read(ifs) || !iend.read(ifs))
	{
		std::cerr << "Read Error : " << _fname << std::endl;
		return false;
	}

	if (ihdr.type != "IHDR")
	{
		std::cerr << "Parse Error : IHDR" << std::endl;
		return false;
	}

	if (idat.type != "IDAT")
	{
		std::cerr << "Parse Error : IDAT" << std::endl;
		return false;
	}

	if (iend.type != "IEND")
	{
		std::cerr << "Parse Error : IEND" << std::endl;
		return false;
	}

	std::memcpy(&header.width, ihdr.data.data(), ihdr.len);

	width = ntohl(header.width);
	height = ntohl(header.height);

	if (header.depth != 8)
	{
		std::cerr << "Header Error : " << header.depth << std::endl;
		return false;
	}

	if (header.colp != 2)
	{
		std::cerr << "Header Error : " << header.colp << std::endl;
		return false;
	}

	if (header.cmeth != 0)
	{
		std::cerr << "Header Error : " << header.cmeth << std::endl;
		return false;
	}

	if (header.fmeth != 0)
	{
		std::cerr << "Header Error : " << header.fmeth << std::endl;
		return false;
	}

	if (header.imeth != 0)
	{
		std::cerr << "Header Error : " << header.imeth << std::endl;
		return false;
	}

	comp = idat.data;

	raw.clear();
	raw.resize(width * height * 4);

	unsigned long rsi = raw.size();
	int cret = uncompress
	(
		&raw.front(),
		&rsi,
		reinterpret_cast<const unsigned char*>(comp.data()),
		comp.size()
	);

	if (cret != Z_OK) return false;

	raw.resize(rsi);
	raw = inverse(raw);

	return true;
}

bool PNG::encode(const std::string& _fname)
{
	if (raw.empty()) return false;

	raw = transform(raw);

	comp.clear();
	comp.resize(compressBound(raw.size()));

	unsigned long csi = comp.size();
	int cret = compress
	(
		&comp.front(),
		&csi,
		reinterpret_cast<const unsigned char*>(raw.data()),
		raw.size()
	);

	if (cret != Z_OK) return false;

	comp.resize(csi);

	HeaderPNG header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ofs.write(reinterpret_cast<const char*>(&header.type), sizeof(header.type));

	header.width = htonl(width);
	header.height = htonl(height);

	std::vector<uint8_t> hdr
	(
		reinterpret_cast<unsigned char*>(&header) + sizeof(header.type),
		reinterpret_cast<unsigned char*>(&header) + sizeof(header)
	);

	chunk ihdr;
	chunk idat;
	chunk iend;

	ihdr.write(ofs, "IHDR", hdr);
	idat.write(ofs, "IDAT", comp);
	iend.write(ofs, "IEND", std::vector<uint8_t>());

	ofs.close();

	return true;
}