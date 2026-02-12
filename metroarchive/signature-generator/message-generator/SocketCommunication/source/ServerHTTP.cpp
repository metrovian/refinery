#include "ServerHTTP.h"

HTTP_REQUEST ServerHTTP::parseRequestMessage(std::string _msg)
{
	HTTP_REQUEST ret;

	std::istringstream ss(_msg);
	std::string line;

	if (!std::getline(ss, line)) return ret;

	std::istringstream ls(line);
	ls >> ret.method >> ret.url >> ret.version;

	while (std::getline(ss, line) && (line != "\r"))
	{
		size_t pos = line.find(": ");

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

bool ServerHTTP::open()
{
	port = 80;
	return ServerTCP::open();
}

bool ServerHTTP::close()
{
	return ServerTCP::close();
}

bool ServerHTTP::sendResponseMessage(HTTP_RESPONSE _msg, uint64_t _idx)
{
	std::string rsp = _msg.version + " " + _msg.code + " " + _msg.status + "\r\n";

	for (const auto& head : _msg.header)
	{
		rsp += head.first + ": " + head.second + "\r\n";
	}

	rsp += "\r\n";
	rsp += _msg.body;
	
	return sendSimpleMessage(rsp, _idx);
}

bool ServerHTTP::processReceivedGet(HTTP_REQUEST _msg, uint64_t _idx)
{
	HTTP_RESPONSE rsp;

	if (_msg.url == "/")
	{
		std::ifstream file("index.html", std::ios::binary);

		if (file.is_open())
		{
			rsp.body = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
			
			rsp.version = "HTTP/1.1";
			rsp.code = "200";
			rsp.status = "OK";
			rsp.header["Content-Type"] = "text/html; charset=UTF-8";
			rsp.header["Content-Length"] = std::to_string(rsp.body.length());

			return sendResponseMessage(rsp, _idx);
		}
	}

	else
	{
		std::ifstream file(_msg.url.substr(1), std::ios::binary);

		if (file.is_open())
		{
			rsp.body = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

			rsp.version = "HTTP/1.1";
			rsp.code = "200";
			rsp.status = "OK";
			rsp.header["Content-Type"] = "text/html; charset=UTF-8";
			rsp.header["Content-Length"] = std::to_string(rsp.body.length());

			return sendResponseMessage(rsp, _idx);
		}
	}

	rsp.version = "HTTP/1.1";
	rsp.code = "404";
	rsp.status = "Not Found";
	rsp.header["Content-Type"] = "text/html; charset=UTF-8";
	rsp.header["Content-Length"] = "0";

	return sendResponseMessage(rsp, _idx);
}

bool ServerHTTP::processReceivedPost(HTTP_REQUEST _msg, uint64_t _idx)
{
	static uint64_t num = 0;

	std::string url = _msg.url.substr(1) + "/" + std::to_string(num++) + ".html";
	std::ofstream ofs(url, std::ios::binary);

	if (ofs.is_open())
	{
		ofs.write(&_msg.body[0], _msg.body.length());
		ofs.close();

		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "200";
		rsp.status = "OK";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		return sendResponseMessage(rsp, _idx);
	}

	else
	{
		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "403";
		rsp.status = "Forbidden";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		return sendResponseMessage(rsp, _idx);
	}
}

bool ServerHTTP::processReceivedPut(HTTP_REQUEST _msg, uint64_t _idx)
{
	std::ofstream ofs(_msg.url.substr(1), std::ios::binary);

	if (ofs.is_open())
	{
		ofs.write(&_msg.body[0], _msg.body.length());
		ofs.close();

		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "200";
		rsp.status = "OK";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		return sendResponseMessage(rsp, _idx);
	}

	else
	{
		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "403";
		rsp.status = "Forbidden";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		return sendResponseMessage(rsp, _idx);
	}
}

bool ServerHTTP::processReceivedDelete(HTTP_REQUEST _msg, uint64_t _idx)
{
	if (std::remove(_msg.url.substr(1).c_str()) == 0)
	{
		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "200";
		rsp.status = "OK";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		return sendResponseMessage(rsp, _idx);
	}

	else
	{
		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "403";
		rsp.status = "Forbidden";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		return sendResponseMessage(rsp, _idx);
	}
}

void ServerHTTP::processReceivedMessage(std::string _msg, uint64_t _idx)
{
	HTTP_REQUEST req = parseRequestMessage(_msg);

	if		(req.method == "GET")		processReceivedGet(req, _idx);
	else if (req.method == "POST")		processReceivedPost(req, _idx);
	else if (req.method == "PUT")		processReceivedPut(req, _idx);
	else if (req.method == "DELETE")	processReceivedDelete(req, _idx);

	else
	{
		HTTP_RESPONSE rsp;

		rsp.version = "HTTP/1.1";
		rsp.code = "400";
		rsp.status = "Bad Request";
		rsp.header["Content-Type"] = "text/html; charset=UTF-8";
		rsp.header["Content-Length"] = "0";

		sendResponseMessage(rsp, _idx);
	}

	std::cerr << "[Client " << _idx << "] " << "Request : " << req.method << " " << req.url << " " << req.version << std::endl;
}
