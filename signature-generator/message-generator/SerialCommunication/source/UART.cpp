#include "UART.h"
#include "Predefined.h"

bool UART::open()
{
	if (com != NULL) return false;

	com = CreateFileA
	(
		port.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (com == INVALID_HANDLE_VALUE)
	{
		std::cerr << "[" << port << "] Open Failed" << std::endl;
		return false;
	}

	DCB dcbs = { 0 };
	dcbs.DCBlength = sizeof(dcbs);

	if (!GetCommState(com, &dcbs))
	{
		std::cerr << "[" << port << "] Status Check Failed" << std::endl;
		close();

		return false;
	}

	dcbs.BaudRate = baud;
	dcbs.ByteSize = data;
	dcbs.Parity = par;
	dcbs.StopBits = stop;

	if (!SetCommState(com, &dcbs))
	{
		std::cerr << "[" << port << "] Frame Configuration Failed" << std::endl;
		close();

		return false;
	}

	if (!SetCommTimeouts(com, &timeouts))
	{
		std::cerr << "[" << port << "] Timeout Configuration Failed" << std::endl;
		close();

		return false;
	}

	return startReceiveThread();
}

bool UART::close()
{
	if (com != NULL)
	{
		CloseHandle(com);
		com = NULL;

		return true;
	}

	return false;
}

bool UART::sendSimpleMessage(std::string _msg)
{
	DWORD bytes;

	if (!WriteFile(com, _msg.c_str(), _msg.length(), &bytes, NULL))
	{
		std::cerr << "[" << port << "] Send Failed" << std::endl;
		return false;
	}

	return (bytes == _msg.length());
}

bool UART::startReceiveThread()
{
	if (flag) return false;
	flag = true;

	auto func = [&]()
		{
			while (flag)
			{
				char msg[BUFFER_SIZE];
				DWORD bytes;

				if (ReadFile(com, msg, sizeof(msg), &bytes, NULL))
				{
					if (bytes > 0)
					{
						processReceivedMessage(std::string(msg, bytes));
					}
				}

				else
				{
					std::cerr << "[" << port << "] Receive Failed" << std::endl;
				}
			}
		};

	std::thread trd = std::thread(func);
	trd.detach();

	return true;
}

bool UART::stopThread()
{
	flag = false;
	return true;
}

void UART::processReceivedMessage(std::string _msg)
{
	std::cerr << "[" << port << "] " << _msg << std::endl;
}

std::vector<std::string> UART::searchPorts()
{
	std::vector<std::string> ret;

	for (unsigned char i = 0; i < 255; ++i)
	{
		std::string name = "\\\\.\\COM" + std::to_string(i + 1);
		HANDLE hport = CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

		if (hport != INVALID_HANDLE_VALUE)
		{
			ret.push_back("COM" + std::to_string(i + 1));
			CloseHandle(hport);
		}
	}

	return ret;
}
