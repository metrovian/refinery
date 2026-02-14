#pragma once
#include "UDP.h"
#include "FrameSNMP.h"

class ServerSNMP : public UDP
{
public: /* constructor */
	ServerSNMP();
	~ServerSNMP();

protected: /* parts */
	std::string encodeNumberASN1(int32_t _num);
	std::string encodeMessageASN1(std::string _str);

protected: /* encode */
	std::string encodeFrameASN1(SNMP_SESSION _ses, SNMP_FRAME _msg);

public: /* send */
	bool sendRequestMessage(std::string _ip, SNMP_SESSION _ses, SNMP_FRAME _msg);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg, uint16_t _port);
};