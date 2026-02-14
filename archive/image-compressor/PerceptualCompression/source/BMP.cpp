#include "BMP.h"
#include "HeaderBMP.h"

std::string BMP::path(const std::string& _fname)
{
	return _fname.substr(0, _fname.find_last_of('/'));
}

std::string BMP::name(const std::string& _fname)
{
	return _fname.substr(_fname.find_last_of('/') + 1);
}

std::string BMP::extension(const std::string& _fname)
{
	return _fname.substr(_fname.find_last_of('.') + 1);
}

bool BMP::render(HINSTANCE _winst, int _ncmds)
{
	if (raw.empty()) return false;

	RECT rect = { 0, 0, width, height };
	HWND hwnd;
	BITMAPINFO bmi = { 0 };
	PAINTSTRUCT ps;
	MSG msg;

	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, FALSE);

	hwnd = CreateWindow
	(
		L"DefaultWindow",
		L"Image Compressor",
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		_winst,
		NULL
	);

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	ShowWindow(hwnd, _ncmds);
	UpdateWindow(hwnd);
	InvalidateRect(hwnd, NULL, TRUE);

	SetDIBitsToDevice(BeginPaint(hwnd, &ps), 0, 0, width, height, 0, 0, 0, height, raw.data(), &bmi, DIB_RGB_COLORS);
	EndPaint(hwnd, &ps);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

bool BMP::load(const std::string& _fname)
{
	HeaderBMP header;
	std::ifstream ifs(_fname, std::ios::binary);

	if (!ifs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	ifs.read(reinterpret_cast<char*>(&header), sizeof(HeaderBMP));

	if (header.type != 0x4D42)
	{
		std::cerr << "Header Error : " << header.type << std::endl;
		return false;
	}

	if (header.depth != 0x0018)
	{
		std::cerr << "Header Error : " << header.depth << std::endl;
		return false;
	}

	width = header.width;
	height = header.height;

	raw.resize(header.dsi);

	ifs.read(reinterpret_cast<char*>(raw.data()), header.dsi);
	ifs.close();

	return true;
}

bool BMP::save(const std::string& _fname)
{
	HeaderBMP header;
	std::ofstream ofs(_fname, std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Open Error : " << _fname << std::endl;
		return false;
	}

	header.width = width;
	header.height = height;

	header.fsi = raw.size() + header.ofs;
	header.dsi = raw.size();

	ofs.write(reinterpret_cast<const char*>(&header), header.ofs);
	ofs.write(reinterpret_cast<const char*>(raw.data()), header.dsi);
	ofs.close();

	return true;
}

bool BMP::decode(const std::string& _fname)
{
	return false;
}

bool BMP::encode(const std::string& _fname)
{
	return false;
}
