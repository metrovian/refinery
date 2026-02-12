#include "ServerTCP.h"
#include "Predefined.h"

bool ServerTCP::open()
{
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (ret != 0)
	{
		std::cerr << "[Server] Startup Failed : " << ret << std::endl;
		return false;
	}

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server == INVALID_SOCKET)
	{
		std::cerr << "[Server] Socket Creation Failed : " << WSAGetLastError() << std::endl;
		WSACleanup();

		return false;
	}

	sockaddr_in addr;
	sockaddr_in cddr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	int clen = sizeof(cddr);

	if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cerr << "[Server] Bind Failed : " << WSAGetLastError() << std::endl;
		closesocket(server);
		WSACleanup();

		return false;
	}

	if (getsockname(server, (sockaddr*)&cddr, &clen) == SOCKET_ERROR)
	{
		std::cerr << "[Server] Bind Failed : " << WSAGetLastError() << std::endl;
		closesocket(server);
		WSACleanup();

		return false;
	}

	if (listen(server, max) == SOCKET_ERROR)
	{
		std::cerr << "[Server] Listen Failed : " << WSAGetLastError() << std::endl;
		closesocket(server);
		WSACleanup();

		return false;
	}

	char ip[32];
	inet_ntop(AF_INET, &cddr.sin_addr, ip, INET_ADDRSTRLEN);

	std::cerr << "[Server] Listen Started : " << ip << ":" << ntohs(cddr.sin_port) << std::endl;

	u_long mode = 0;
	ioctlsocket(server, FIONBIO, &mode);
	startAcceptThread();

	return true;
}

bool ServerTCP::close()
{
	stopThread();

	for (uint64_t i = client.size(); i > 0; --i)
	{
		closesocket(client[i - 1]);
		client.pop_back();
	}

	closesocket(server);
	WSACleanup();

	return true;
}

bool ServerTCP::sendSimpleMessage(std::string _msg, uint64_t _idx)
{
	if (_idx > client.size()) return false;

	send(client[_idx], _msg.c_str(), _msg.length(), 0);
	return true;
}

bool ServerTCP::sendSimpleMessage(std::string _msg)
{
	for (uint64_t i = 0; i < client.size(); ++i)
	{
		if (!sendSimpleMessage(_msg, i)) return false;
	}

	return true;
}

bool ServerTCP::startAcceptThread()
{
	if (flag) return false;
	flag = true;

	sockaddr_in addr;
	int size = sizeof(addr);

	auto func = [&]()
		{
			std::cerr << "[Server] Accept Thread Started" << std::endl;

			while (flag)
			{
				client.push_back(accept(server, (sockaddr*)&addr, &size));

				if (client[client.size() - 1] == INVALID_SOCKET)
				{
					std::cerr << "[Server] Accept Failed : " << WSAGetLastError() << std::endl;

					closesocket(client[client.size() - 1]);
					client.pop_back();
				}

				else
				{
					char ip[32];
					inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

					std::cerr << "[Client " << client.size() - 1 << "] Connected : " << ip << ":" << ntohs(addr.sin_port) << std::endl;
					startReceiveThread(client.size() - 1);
				}
			}

			std::cerr << "[Server] Accept Thread Terminated" << std::endl;
		};

	std::thread trd = std::thread(func);
	trd.detach();

	return true;
}

bool ServerTCP::startReceiveThread(uint64_t _idx)
{
	if (_idx > client.size() - 1) return false;

	auto func = [&]()
		{
			std::cerr << "[Client " << _idx << "] Receive Thread Started" << std::endl;

			while (flag)
			{
				char msg[BUFFER_SIZE] = { 0, };
				int ret = recv(client[_idx], msg, BUFFER_SIZE, 0);

				if (ret == 0)
				{
					std::cerr << "[Client " << _idx << "] Disconnected" << std::endl;
					break;
				}

				else if (ret < 0)
				{
					std::cerr << "[Client " << _idx << "] Receive Failed : " << WSAGetLastError() << std::endl;
					break;
				}

				else
				{
					processReceivedMessage(std::string(msg, ret), _idx);
				}
			}

			std::cerr << "[Client " << _idx << "] Receive Thread Terminated" << std::endl;
		};

	std::thread trd = std::thread(func);
	trd.detach();

	return true;
}

bool ServerTCP::stopThread()
{
	flag = false;
	return true;
}

void ServerTCP::processReceivedMessage(std::string _msg, uint64_t _idx)
{
	std::cerr << "[Client " << _idx << "] " << _msg << std::endl;
}
