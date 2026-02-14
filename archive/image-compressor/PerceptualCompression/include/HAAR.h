#pragma once
#include "BMP.h"

class HAAR : public BMP
{
protected: /* transform */
	std::vector<uint8_t> transform(const std::vector<uint8_t>& _bmp);
	std::vector<uint8_t> inverse(const std::vector<uint8_t>& _dwt);

public: /* compression */
	virtual bool decode(const std::string& _fname);
	virtual bool encode(const std::string& _fname);
};