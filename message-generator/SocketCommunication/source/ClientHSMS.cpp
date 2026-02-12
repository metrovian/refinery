#include "ClientHSMS.h"

bool ClientHSMS::connect(std::string _ip, uint16_t _port)
{
	if (state != HSMS_STATE::NONE) return false;

	ip = _ip;
	port = _port;

	bool ret = ClientTCP::connect();

	if (ret) state = HSMS_STATE::CONNECTED;
	return ret;
}

bool ClientHSMS::disconnect()
{
	if (state == HSMS_STATE::NONE) return false;

	bool ret = ClientTCP::disconnect();

	if (ret) state = HSMS_STATE::NONE;
	return ret;
}

bool ClientHSMS::sendRequest(HSMS_SESSION _ses)
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
	if (!sendSimpleMessage(msg)) return false;

	auto time = std::chrono::steady_clock::now();
	while (std::chrono::steady_clock::now() < time + std::chrono::milliseconds(HSMS_TIMEOUT))
	{
		if (pends.find(pend) == pends.end()) return true;
	}

	std::cerr << "[Server] Reply Timeout > " << HSMS_TIMEOUT << " ms" << std::endl;
	return false;
}

bool ClientHSMS::sendResponse(HSMS_SESSION _ses, uint32_t _sbyte)
{
	if (state == HSMS_STATE::NONE) return false;

	std::string msg;
	msg.resize(HSMS_SIZE);

	msg[5] = static_cast<char>(_ses);
	msg[6] = static_cast<char>(_sbyte >> 24);
	msg[7] = static_cast<char>((_sbyte >> 16) & 0xFF);
	msg[8] = static_cast<char>((_sbyte >> 8) & 0xFF);
	msg[9] = static_cast<char>(_sbyte & 0xFF);

	return sendSimpleMessage(msg);
}

bool ClientHSMS::sendData(std::string _msg)
{
	if (state != HSMS_STATE::SELECTED) return false;

	std::string msg;
	msg.resize(HSMS_SIZE);

	msg[5] = static_cast<char>(HSMS_SESSION::DATA);
	msg[6] = static_cast<char>(sbyte >> 24);
	msg[7] = static_cast<char>((sbyte >> 16) & 0xFF);
	msg[8] = static_cast<char>((sbyte >> 8) & 0xFF);
	msg[9] = static_cast<char>(sbyte & 0xFF);

	if (!sendSimpleMessage(msg + _msg)) return false;
	return true;
}

void ClientHSMS::processReceivedMessage(std::string _msg)
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
			if (state == HSMS_STATE::SELECTED) std::cerr << "[Server] " << _msg.substr(10, _msg.size() - 10) << std::endl;
			else sendResponse(HSMS_SESSION::REJECT_REQ, rans);
			break;
		}

		case HSMS_SESSION::SELECT_REQ:
		{
			state = HSMS_STATE::SELECTED;
			sendResponse(HSMS_SESSION::SELECT_RSP, rans);
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
			sendResponse(HSMS_SESSION::DESELECT_RSP, rans);
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
			sendResponse(HSMS_SESSION::LINKTEST_RSP, rans);
			break;
		}

		case HSMS_SESSION::LINKTEST_RSP:
		{
			pends.erase(rans);
			break;
		}

		case HSMS_SESSION::REJECT_REQ:
		{
			std::cerr << "[Server] Rejected" << std::endl;
			break;
		}

		default:
		{
			sendResponse(HSMS_SESSION::REJECT_REQ, rans);
			break;
		}

		}

		return;
	}
}
