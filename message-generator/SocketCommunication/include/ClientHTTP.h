#pragma once
#include "ClientTCP.h"
#include "FrameHTTP.h"

class ClientHTTP : public ClientTCP
{
protected: /* parse */
	HTTP_RESPONSE parseResponseMessage(std::string _msg);

public: /* connection */
	bool connect(std::string _ip);
	bool disconnect();

public: /* send */
	bool sendRequestMessage(HTTP_REQUEST _msg);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg);
};