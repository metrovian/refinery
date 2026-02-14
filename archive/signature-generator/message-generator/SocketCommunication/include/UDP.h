#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <set>
#include <map>

class UDP
{
protected: /* socket */
	SOCKET host;

protected: /* ports */
	std::set<uint16_t> ports;

public: /* constructor */
	UDP(uint16_t _port);
	~UDP();

public: /* send */
	bool sendSimpleMessage(std::string _msg, std::string _ip, uint16_t _port);

public: /* receive */
	bool startReceiveThread(uint16_t _port);
	bool stopThread(uint16_t _port);
	bool stopThread();

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg, uint16_t _port);
};