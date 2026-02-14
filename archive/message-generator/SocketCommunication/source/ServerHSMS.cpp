#include "ServerHSMS.h"

bool ServerHSMS::open(uint16_t _port)
{
	if (state != HSMS_STATE::NONE) return false;

	port = _port;

	bool ret = ServerTCP::open();

	if (ret) state = HSMS_STATE::CONNECTED;
	return ret;
}

bool ServerHSMS::close()
{
	if (state == HSMS_STATE::NONE) return false;

	bool ret = ServerTCP::close();

	if (ret) state = HSMS_STATE::NONE;
	return ret;
}

bool ServerHSMS::sendRequest(HSMS_SESSION _ses, uint64_t _idx)
{
	if (state == HSMS_STATE::NONE) return false;

	std::string msg;
	msg.resize(HSMS_SIZE);

	msg[5] = static_cast<char>(_ses);
	msg[6] = static_cast<char>(sbyte >> 24);
	msg[7] = static_cast<char>((sbyte >> 16) & 0xFF);
	msg[8] = static_cast<char>((sbyte >> 8) & 0xFF);
	msg[9] = static_cast<char>(sbyte & 0xFF);

	uint32_t pend = sbyte;

	pends.insert(sbyte++);
	if (!sendSimpleMessage(msg, _idx)) return false;

	auto time = std::chrono::steady_clock::now();
	while (std::chrono::steady_clock::now() < time + std::chrono::milliseconds(HSMS_TIMEOUT))
	{
		if (pends.find(pend) == pends.end()) return true;
	}

	std::cerr << "[Client " << _idx << "] Reply Timeout > " << HSMS_TIMEOUT << " ms" << std::endl;
	return false;
}

bool ServerHSMS::sendResponse(HSMS_SESSION _ses, uint32_t _sbyte, uint64_t _idx)
{
	if (state == HSMS_STATE::NONE) return false;

	std::string msg;
	msg.resize(HSMS_SIZE);

	msg[5] = static_cast<char>(_ses);
	msg[6] = static_cast<char>(_sbyte >> 24);
	msg[7] = static_cast<char>((_sbyte >> 16) & 0xFF);
	msg[8] = static_cast<char>((_sbyte >> 8) & 0xFF);
	msg[9] = static_cast<char>(_sbyte & 0xFF);

	return sendSimpleMessage(msg, _idx);
}

bool ServerHSMS::sendData(std::string _msg, uint64_t _idx)
{
	if (state != HSMS_STATE::SELECTED) return false;

	std::string msg;
	msg.resize(HSMS_SIZE);

	msg[5] = static_cast<char>(HSMS_SESSION::DATA);
	msg[6] = static_cast<char>(sbyte >> 24);
	msg[7] = static_cast<char>((sbyte >> 16) & 0xFF);
	msg[8] = static_cast<char>((sbyte >> 8) & 0xFF);
	msg[9] = static_cast<char>(sbyte & 0xFF);

	if (!sendSimpleMessage(msg + _msg, _idx)) return false;
	return true;
}

void ServerHSMS::processReceivedMessage(std::string _msg, uint64_t _idx)
{
	std::vector<BYTE> frame;

	for (char data : _msg)
	{
		frame.push_back(static_cast<BYTE>(data));
	}

	if (frame.size() < 9) return;

	uint32_t rans = static_cast<uint32_t>(frame[6] << 24);
	rans += static_cast<uint32_t>(frame[7] << 16);
	rans += static_cast<uint32_t>(frame[8] << 8);
	rans += static_cast<uint32_t>(frame[9]);

	if (frame[2] == 0x00)
	{
		switch (static_cast<HSMS_SESSION>(frame[5]))
		{

		case HSMS_SESSION::DATA:
		{
			if (state == HSMS_STATE::SELECTED) std::cerr << "[Client "<< _idx << "] " << _msg.substr(10, _msg.size() - 10) << std::endl;
			else sendResponse(HSMS_SESSION::REJECT_REQ, rans, _idx);
			break;
		}

		case HSMS_SESSION::SELECT_REQ:
		{
			state = HSMS_STATE::SELECTED;
			sendResponse(HSMS_SESSION::SELECT_RSP, rans, _idx);
			break;
		}

		case HSMS_SESSION::SELECT_RSP:
		{
			state = HSMS_STATE::SELECTED;
			pends.erase(rans);
			break;
		}

		case HSMS_SESSION::DESELECT_REQ:
		{
			state = HSMS_STATE::CONNECTED;
			sendResponse(HSMS_SESSION::DESELECT_RSP, rans, _idx);
			break;
		}

		case HSMS_SESSION::DESELECT_RSP:
		{
			state = HSMS_STATE::CONNECTED;
			pends.erase(rans);
			break;
		}

		case HSMS_SESSION::LINKTEST_REQ:
		{
			sendResponse(HSMS_SESSION::LINKTEST_RSP, rans, _idx);
			break;
		}

		case HSMS_SESSION::LINKTEST_RSP:
		{
			pends.erase(rans);
			break;
		}

		case HSMS_SESSION::REJECT_REQ:
		{
			std::cerr << "[Client " << _idx << "] Rejected" << std::endl;
			break;
		}

		default:
		{
			sendResponse(HSMS_SESSION::REJECT_REQ, rans, _idx);
			break;
		}

		}

		return;
	}
}
