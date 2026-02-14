#include "ClientTCP.h"
#include "Predefined.h"

bool ClientTCP::connect()
{
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (ret != 0)
	{
		std::cerr << "[Client] Startup Failed : " << ret << std::endl;
		return false;
	}

	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (client == INVALID_SOCKET)
	{
		std::cerr << "[Client] Socket Creation Failed : " << WSAGetLastError() << std::endl;
		WSACleanup();

		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	u_long mode = 0;
	ioctlsocket(client, FIONBIO, &mode);

	if (::connect(client, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cerr << "[Client] Connection Failed : " << WSAGetLastError() << std::endl;
		closesocket(client);
		WSACleanup();

		return false;
	}

	else
	{
		std::cerr << "[Server] Connected : " << ip << ":" << port << std::endl;
		return startReceiveThread();
	}
}

bool ClientTCP::disconnect()
{
	stopThread();

	closesocket(client);
	WSACleanup();

	return true;
}

bool ClientTCP::sendSimpleMessage(std::string _msg)
{
	send(client, _msg.c_str(), _msg.length(), 0);
	return true;
}

bool ClientTCP::startReceiveThread()
{
	if (flag) return false;
	flag = true;

	auto func = [&]()
		{
			std::cerr << "[Server] Receive Thread Started" << std::endl;

			while (flag)
			{
				char msg[BUFFER_SIZE] = { 0, };
				int ret = recv(client, msg, BUFFER_SIZE, 0);

				if (ret == 0)
				{
					std::cerr << "[Server] Disconnected" << std::endl;
					break;
				}

				else if (ret < 0)
				{
					std::cerr << "[Server] Receive Failed : " << WSAGetLastError() << std::endl;
				}

				else
				{
					processReceivedMessage(std::string(msg, ret));
				}
			}

			std::cerr << "[Server] Receive Thread Terminated" << std::endl;
		};

	std::thread trd = std::thread(func);
	trd.detach();

	return true;
}

bool ClientTCP::stopThread()
{
	flag = false;
	return true;
}

void ClientTCP::processReceivedMessage(std::string _msg)
{
	std::cerr << "[Server] " << _msg << std::endl;
}
