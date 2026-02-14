#include "Hash.h"
#include "MultiplicativeHash.h"
#include "FibonacciHash.h"
#include "MD5.h"
#include "SHA256.h"
#include "SHA512.h"

int main()
{
	Hash* engine = new SHA512();

	std::vector<uint8_t> hash1 = engine->str_hash("x64/HashInspection.exe");
	std::vector<uint8_t> hash2 = engine->file_hash("x64/HashInspection.exe");

	for (uint64_t i = 0; i < hash1.size(); ++i)
	{
		std::cout 
			<< std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<uint64_t>(hash1[i]);
	}
	std::cout << std::endl;

	for (uint64_t i = 0; i < hash2.size(); ++i)
	{
		std::cout
			<< std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<uint64_t>(hash2[i]);
	}
	std::cout << std::endl;

	return 0;
}