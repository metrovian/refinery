#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

class BMP
{
protected: /* data */
	std::vector<uint8_t> raw;
	std::vector<uint8_t> comp;

public: /* dimension */
	uint64_t width = 0;
	uint64_t height = 0;

public: /* constructor */
	BMP() = default;
	~BMP() = default;

protected: /* extension */
	static std::string path(const std::string& _fname);
	static std::string name(const std::string& _fname);
	static std::string extension(const std::string& _fname);

public: /* display */
	bool render(HINSTANCE _winst, int _ncmds);

public: /* raw */
	bool load(const std::string& _fname);
	bool save(const std::string& _fname);

public: /* compression */
	virtual bool decode(const std::string& _fname);
	virtual bool encode(const std::string& _fname);
};