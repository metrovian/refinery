#pragma once
#include "BMP.h"

class PNG : public BMP
{
protected: /* struct */
	struct chunk
	{
	public: /* number */
		uint32_t len;
		uint32_t crc;

	public: /* container */
		std::string type;
		std::vector<uint8_t> data;

	public: /* file */
		bool read(std::ifstream& _ifs);
		bool write(std::ofstream& _ofs, const std::string& _type, const std::vector<uint8_t>& _data);
	};

protected: /* transform */
	std::vector<uint8_t> transform(const std::vector<uint8_t>& _bmp);
	std::vector<uint8_t> inverse(const std::vector<uint8_t>& _png);

public: /* compression */
	virtual bool decode(const std::string& _fname);
	virtual bool encode(const std::string& _fname);
};