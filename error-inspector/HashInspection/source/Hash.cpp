#include "Hash.h"

bool Hash::read_file(std::string _fname)
{
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.seekg(0, std::ios::end);

	raw.clear();
	raw.resize(ifs.tellg(), 0);

	ifs.seekg(0, std::ios::beg);
	ifs.read(reinterpret_cast<char*>(raw.data()), raw.size());

	return true;
}

bool Hash::read_str(std::string _str)
{
	raw.clear();
	raw.resize(_str.size(), 0);

	std::copy(_str.begin(), _str.end(), raw.begin());

	return true;
}

std::vector<uint8_t> Hash::file_hash(std::string _fname)
{
	if (!read_file(_fname)) return std::vector<uint8_t>();
	return calc_hash();
}

std::vector<uint8_t> Hash::str_hash(std::string _str)
{
	if (!read_str(_str)) return std::vector<uint8_t>();
	return calc_hash();
}
