#pragma once
#include "ServerTCP.h"
#include "FrameHTTP.h"

class ServerHTTP : public ServerTCP
{
protected: /* parse */
	HTTP_REQUEST parseRequestMessage(std::string _msg);

public: /* connection */
	bool open();
	bool close();

protected: /* send */
	bool sendResponseMessage(HTTP_RESPONSE _msg, uint64_t _idx);

protected: /* receive */
	bool processReceivedGet(HTTP_REQUEST _msg, uint64_t _idx);
	bool processReceivedPost(HTTP_REQUEST _msg, uint64_t _idx);
	bool processReceivedPut(HTTP_REQUEST _msg, uint64_t _idx);
	bool processReceivedDelete(HTTP_REQUEST _msg, uint64_t _idx);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg, uint64_t _idx);
};