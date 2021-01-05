#include "WndProc.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR lpCmdLine, int nCmdShow)
{
	HWND hMainWindow = gui::mainHwnd = initWindow(hInstance, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}