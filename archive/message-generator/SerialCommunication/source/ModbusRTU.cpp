#include "ModbusRTU.h"

ModbusRTU::ModbusRTU()
{
	baud = 9600;
	par = 0;
	stop = 0;
}

WORD ModbusRTU::calcCRC(std::vector<BYTE> _frame)
{
	WORD ret = 0xFFFF;

	for (BYTE byte : _frame)
	{
		ret ^= byte;

		for (BYTE i = 0; i < 8; ++i)
		{
			if (ret & 0x0001)
			{
				ret >>= 1;
				ret ^= 0xA001;
			}

			else
			{
				ret >>= 1;
			}
		}
	}

	return ret;
}

bool ModbusRTU::sendRequest(std::vector<BYTE> _frame)
{
	WORD crc = calcCRC(_frame);

	_frame.push_back(crc & 0xFF);
	_frame.push_back(crc >> 8);

	std::string msg;

	for (BYTE byte : _frame)
	{
		msg.push_back(static_cast<char>(byte));
	}

	return sendSimpleMessage(msg);
}

void ModbusRTU::processReceivedMessage(std::string _msg)
{
	std::vector<BYTE> frame;

	for (char data : _msg)
	{
		frame.push_back(static_cast<BYTE>(data));
	}

	WORD crc = (frame.back() << 8);
	frame.pop_back();

	crc |= frame.back();
	frame.pop_back();

	if (crc == calcCRC(frame))
	{
		std::cerr << "[" << port << "] " << std::uppercase << std::hex;

		for (BYTE byte : frame)
		{
			if (byte < 16) std::cerr << "0x0" << static_cast<uint32_t>(byte) << " ";
			else std::cerr << "0x" << static_cast<uint32_t>(byte) << " ";
		}

		std::cerr << std::dec << std::endl;
	}

	else
	{
		std::cerr << "[" << port << "] " << "CRC Failed" << std::endl;
	}
}