#pragma once
#include "UDP.h"
#include "FrameDHCP.h"

class ServerDHCP : public UDP
{
protected: /* data */
	std::map<uint32_t, uint64_t> band;
	std::string ip;
	std::string snet;

public: /* constructor */
	ServerDHCP(std::string _ip, std::string _snet);
	~ServerDHCP();

protected: /* parse */
	DHCP_FRAME parseReceivedMessage(const uint8_t* _msg);

protected: /* message */
	void printHardwareAddress(uint64_t _mac);
	void printHostAddress(uint32_t _ip);

protected: /* address */
	uint32_t leaseHostAddress(uint64_t _mac);
	uint32_t releaseHostAddress(uint64_t _mac);

public: /* band */
	bool insert(std::string _ip);
	bool erase(std::string _ip);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg, uint16_t _port);
};