#pragma once
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

class Hash
{
protected: /* data */
	std::vector<uint8_t> raw;

protected: /* raw */
	bool read_file(std::string _fname);
	bool read_str(std::string _str);

public: /* hash */
	std::vector<uint8_t> file_hash(std::string _fname);
	std::vector<uint8_t> str_hash(std::string _str);

protected: /* virtual */
	virtual std::vector<uint8_t> calc_hash() = 0;
};