#pragma once
#include "ServerTCP.h"
#include "FrameHSMS.h"

class ServerHSMS : public ServerTCP
{
protected: /* states */
	HSMS_STATE state = HSMS_STATE::NONE;
	uint32_t sbyte = static_cast<uint32_t>(rand());

protected: /* pends */
	std::set<uint32_t> pends;

public: /* connection */
	bool open(uint16_t _port);
	bool close();

public: /* send */
	bool sendRequest(HSMS_SESSION _ses, uint64_t _idx);
	bool sendResponse(HSMS_SESSION _ses, uint32_t _sbyte, uint64_t _idx);
	bool sendData(std::string _msg, uint64_t _idx);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg, uint64_t _idx);
};