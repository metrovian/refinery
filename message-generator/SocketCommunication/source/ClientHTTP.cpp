#include "ClientHTTP.h"

HTTP_RESPONSE ClientHTTP::parseResponseMessage(std::string _msg)
{
	HTTP_RESPONSE ret;

	std::istringstream ss(_msg);
	std::string line;

	if (!std::getline(ss, line)) return ret;

	std::istringstream ls(line);
	ls >> ret.version >> ret.code;

	size_t pos = line.find("\r");
	size_t cos = line.find(ret.code);

	ret.status = line.substr(1 + cos + ret.code.length(), pos);

	while (std::getline(ss, line) && (line != "\r"))
	{
		pos = line.find(": ");

		if (pos != std::string::npos)
		{
			std::string prop = line.substr(0, pos);
			std::string val = line.substr(2 + pos);

			if (!val.empty() && (val.back() == '\r'))
			{
				val.pop_back();
			}

			ret.header[prop] = val;
		}
	}

	if (ret.header.find("Content-Length") != ret.header.end())
	{
		int len = std::stoi(ret.header["Content-Length"]);
		ret.body.resize(len);
		ss.read(&ret.body[0], len);
	}

	return ret;
}

bool ClientHTTP::connect(std::string _ip)
{
	ip = _ip;
	port = 80;

	return ClientTCP::connect();
}

bool ClientHTTP::disconnect()
{
	return ClientTCP::disconnect();
}

bool ClientHTTP::sendRequestMessage(HTTP_REQUEST _msg)
{
	std::string msg = _msg.method + " " + _msg.url + " " + _msg.version + "\r\n";

	for (const auto& head : _msg.header)
	{
		msg += head.first + ": " + head.second + "\r\n";
	}

	msg += "\r\n";
	msg += _msg.body;

	return sendSimpleMessage(msg);
}

void ClientHTTP::processReceivedMessage(std::string _msg)
{
	HTTP_RESPONSE rsp = parseResponseMessage(_msg);
	std::cerr << "[Server] Response : " << rsp.version << " " << rsp.code << " " << rsp.status << std::endl;
}
