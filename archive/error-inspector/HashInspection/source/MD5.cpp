#include "MD5.h"

std::vector<uint8_t> MD5::calc_hash()
{
	HCRYPTPROV prov = 0;
	HCRYPTHASH hash = 0;

	std::vector<uint8_t> ret;

	if (!CryptAcquireContext(&prov, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		return std::vector<uint8_t>();
	}
		
	if (!CryptCreateHash(prov, CALG_MD5, 0, 0, &hash)) 
	{
		CryptReleaseContext(prov, 0);

		return std::vector<uint8_t>();
	}

	if (!CryptHashData(hash, raw.data(), raw.size(), 0)) 
	{
		CryptDestroyHash(hash);
		CryptReleaseContext(prov, 0);

		return std::vector<uint8_t>();
	}

	DWORD len = 0;
	DWORD size = sizeof(DWORD);

	if (!CryptGetHashParam(hash, HP_HASHSIZE, (BYTE*)&len, &size, 0)) 
	{
		CryptDestroyHash(hash);
		CryptReleaseContext(prov, 0);

		return std::vector<uint8_t>();
	}

	ret.resize(len);

	if (!CryptGetHashParam(hash, HP_HASHVAL, ret.data(), &len, 0)) 
	{
		CryptDestroyHash(hash);
		CryptReleaseContext(prov, 0);

		return std::vector<uint8_t>();
	}

	CryptDestroyHash(hash);
	CryptReleaseContext(prov, 0);

	return ret;
}
