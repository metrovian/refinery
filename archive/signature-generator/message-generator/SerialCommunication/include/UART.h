#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <iomanip>

class UART
{
protected: /* handle */
	HANDLE com = NULL;

protected: /* config */
	std::string port = "COM7";
	DWORD baud = 115200;
	BYTE data = 8;
	BYTE par = 0;
	BYTE stop = 0;
	COMMTIMEOUTS timeouts = { MAXDWORD, 0, 0, 0, 0 };

protected: /* flag */
	bool flag = false;

public: /* connection */
	bool open();
	bool close();

public: /* send */
	bool sendSimpleMessage(std::string _msg);

private: /* thread */
	bool startReceiveThread();
	bool stopThread();

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg);

public: /* static */
	static std::vector<std::string> searchPorts();
};