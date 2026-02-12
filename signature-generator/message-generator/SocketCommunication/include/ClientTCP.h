#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <unordered_map>

class ClientTCP
{
protected: /* socket */
	SOCKET client;

protected: /* config */
	std::string ip = "127.0.0.1";
	uint16_t port = 8080;

protected: /* flag */
	bool flag = false;

public: /* connection */
	bool connect();
	bool disconnect();

public: /* send */
	bool sendSimpleMessage(std::string _msg);

private: /* thread */
	bool startReceiveThread();
	bool stopThread();

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg);
};