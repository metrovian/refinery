#include "BMP.h"
#include "YUV.h"
#include "DCT.h"
#include "HAAR.h"
#include "JPEG.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default: return DefWindowProc(hwnd, msg, wParam, lParam);

	}

	return 0;
}

int WinSimpleErrorMessage(const wchar_t* msg)
{
	MessageBox(NULL, msg, L"SYSTEM", FALSE);
	exit(-1);

	return -1;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	MessageBox(NULL, L"Program Start", L"SYSTEM", FALSE);

	WNDCLASS wndc = { 0 };
	wndc.lpfnWndProc = WndProc;
	wndc.hInstance = hInstance;
	wndc.lpszClassName = L"DefaultWindow";
	RegisterClass(&wndc);

	std::string name = "x64/test1";
	std::string ext1 = ".bmp";
	std::string ext2 = ".jpeg";

	BMP* engine = new JPEG;

	engine->load(name + ext1);

	if (!engine->encode(name + ext2)) WinSimpleErrorMessage(L"Encode Error");
	if (!engine->decode(name + ext2)) WinSimpleErrorMessage(L"Decode Error");

	engine->save(name + "_decode" + ext1);
	engine->render(hInstance, nShowCmd);

	delete engine;

	MessageBox(NULL, L"Program End", L"SYSTEM", FALSE);

	return 0;
}