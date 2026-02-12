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

class ServerTCP
{
protected: /* socket */
	SOCKET server;
	std::vector<SOCKET> client;
	
protected: /* config */
	uint32_t max = 10;
	uint16_t port = 8080;

protected: /* flag */
	bool flag = false;

public: /* connection */
	bool open();
	bool close();

public: /* send */
	bool sendSimpleMessage(std::string _msg, uint64_t _idx);
	bool sendSimpleMessage(std::string _msg);

private: /* thread */
	bool startAcceptThread();
	bool startReceiveThread(uint64_t _idx);
	bool stopThread();

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg, uint64_t _idx);
};