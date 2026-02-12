#include "UDP.h"
#include "Predefined.h"

UDP::UDP(uint16_t _port)
{
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (ret != 0)
	{
		std::cerr << "[Host] WSAStartup Failed : " << ret << std::endl;
		return;
	}

	host = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (host == INVALID_SOCKET)
	{
		std::cerr << "[Host] Socket Creation Failed : " << WSAGetLastError() << std::endl;
		WSACleanup();

		return;
	}

	ports.insert(_port);

	auto func = [&](uint16_t _port)
		{
			sockaddr_in addr;
			int size = sizeof(addr);

			addr.sin_family = AF_INET;
			addr.sin_port = htons(_port);
			addr.sin_addr.s_addr = INADDR_ANY;

			if (bind(host, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			{
				std::cerr << "[Host] Bind Failed : " << WSAGetLastError() << std::endl;

				closesocket(host);
				WSACleanup();

				return false;
			}

			if (getsockname(host, (sockaddr*)&addr, &size) == SOCKET_ERROR)
			{
				std::cerr << "[Host] Name Failed : " << WSAGetLastError() << std::endl;

				closesocket(host);
				WSACleanup();

				return false;
			}

			uint64_t port = ntohs(addr.sin_port);

			while (ports.find(_port) != ports.end())
			{
				char msg[BUFFER_SIZE] = { 0, };
				int ret = recvfrom(host, msg, BUFFER_SIZE, 0, (sockaddr*)&addr, &size);

				if (ret == SOCKET_ERROR)
				{
					std::cerr << "[Host " << port << "] " << "Receive Failed : " << WSAGetLastError() << std::endl;
				}

				else
				{
					processReceivedMessage(std::string(msg, ret), port);
				}
			}

			closesocket(host);
			WSACleanup();

			return true;
		};

	std::thread trd = std::thread(func, _port);
	trd.detach();
}

UDP::~UDP()
{
	stopThread();

	closesocket(host);
	WSACleanup();

	return;
}

bool UDP::sendSimpleMessage(std::string _msg, std::string _ip, uint16_t _port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr);

	int ret = sendto(host, _msg.c_str(), _msg.size(), 0, (sockaddr*)&addr, sizeof(addr));

	if (ret == SOCKET_ERROR)
	{
		std::cerr << "[Host] Send Failed : " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

bool UDP::startReceiveThread(uint16_t _port)
{
	if (ports.find(_port) != ports.end()) return false;
	ports.insert(_port);

	auto func = [&](uint16_t _port)
		{
			SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			if (sock == INVALID_SOCKET)
			{
				std::cerr << "[Host " << _port << "] " << "Socket Creation Failed : " << WSAGetLastError() << std::endl;
				WSACleanup();

				return false;
			}

			sockaddr_in addr;
			int size = sizeof(addr);

			addr.sin_family = AF_INET;
			addr.sin_port = htons(_port);
			addr.sin_addr.s_addr = INADDR_ANY;

			if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			{
				std::cerr << "[Host " << _port << "] " << "Bind Failed : " << WSAGetLastError() << std::endl;

				closesocket(sock);
				WSACleanup();

				return false;
			}

			while (ports.find(_port) != ports.end())
			{
				char msg[BUFFER_SIZE] = { 0, };
				int ret = recvfrom(sock, msg, BUFFER_SIZE, 0, (sockaddr*)&addr, &size);

				if (ret == SOCKET_ERROR)
				{
					std::cerr << "[Host " << _port << "] " << "Receive Failed : " << WSAGetLastError() << std::endl;
				}

				else
				{
					processReceivedMessage(std::string(msg, ret), _port);
				}
			}

			closesocket(sock);
			WSACleanup();

			return true;
		};

	std::thread trd = std::thread(func, _port);
	trd.detach();

	return true;
}

bool UDP::stopThread(uint16_t _port)
{
	ports.erase(_port);
	return true;
}

bool UDP::stopThread()
{
	ports.clear();
	return true;
}

void UDP::processReceivedMessage(std::string _msg, uint16_t _port)
{
	std::cerr << "[Host " << _port << "] " << _msg << std::endl;
}