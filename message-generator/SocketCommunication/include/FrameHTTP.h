#pragma once

struct HTTP_REQUEST
{
	std::string method;
	std::string url;
	std::string version;
	std::string body;
	std::unordered_map<std::string, std::string> header;
};

struct HTTP_RESPONSE
{
	std::string version;
	std::string code;
	std::string status;
	std::string body;
	std::unordered_map<std::string, std::string> header;
};