#pragma once
#include "ClientTCP.h"
#include "FrameHSMS.h"

class ClientHSMS : public ClientTCP
{
protected: /* states */
	HSMS_STATE state = HSMS_STATE::NONE;
	uint32_t sbyte = static_cast<uint32_t>(rand());

protected: /* pends */
	std::set<uint32_t> pends;

public: /* connection */
	bool connect(std::string _ip, uint16_t _port);
	bool disconnect();

public: /* send */
	bool sendRequest(HSMS_SESSION _ses);
	bool sendResponse(HSMS_SESSION _ses, uint32_t _sbyte);
	bool sendData(std::string _msg);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg);
};