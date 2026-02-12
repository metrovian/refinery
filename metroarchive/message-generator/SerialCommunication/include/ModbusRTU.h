#pragma once
#include "UART.h"

class ModbusRTU : public UART
{
public: /* constructor */
	ModbusRTU();
	~ModbusRTU() = default;

protected: /* parts */
	WORD calcCRC(std::vector<BYTE> _frame);

public: /* send */
	bool sendRequest(std::vector<BYTE> _frame);

protected: /* virtual */
	virtual void processReceivedMessage(std::string _msg);
};