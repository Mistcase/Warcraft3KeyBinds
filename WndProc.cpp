#include "WndProc.h"

HWND initWindow(HINSTANCE hInstance, int nCmdShow)
{
	static char szAppName[] = "wc3Hook";
	HWND hwnd;
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wndclass);
	hwnd = CreateWindow(
		szAppName, // window class name
		"Warcraft3 keybinds helper", // window caption
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // window style
		CW_USEDEFAULT, // initial x position
		CW_USEDEFAULT, // initial y position
		500, // initial x size
		400, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		hInstance, // program instance handle
		NULL
	); // creation parameters
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	gui::initGui(hwnd);
	gui::initKeysNames();
	return hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hmdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;
	static BITMAP bm;
	switch (iMsg)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadImage(NULL, "back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		return TRUE;
	case WM_CTLCOLORSTATIC:
		for (int i = ID_INVENTORY_PRESSED; i < 6 + ID_INVENTORY_PRESSED; i++)
		{
			hdc = (HDC)wParam;
			if ((HWND)lParam == GetDlgItem(hwnd, i))
			{
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(255, 0, 0));
				SetDCBrushColor(hdc, gui::flags[i - ID_INVENTORY_PRESSED] ? SELECTED_COLOR : RGB(0, 0, 0));
				return (LRESULT)GetStockObject(DC_BRUSH);
			}
		}
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//здесь начинаются строки, связанные с рисованием картинки
		hmdc = CreateCompatibleDC(hdc); //Создаёт совместимый с оконным контекст памяти
		SelectObject(hmdc, hBitmap); //Выбирает объект картинку
		GetObject(hBitmap, sizeof(bm), (LPSTR)&bm); //получаем высоту и ширину картинки
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hmdc, 0, 0, SRCCOPY); //Помещает картинку на экран в точку 10, 10
		//из памяти (hmdc)
		DeleteDC(hmdc); //удаляем из памяти контекст
		EndPaint(hwnd, &ps);
		return 0;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_APPLY_PRESSED)
		{
			if (!wc3::hookEnabled) //Activate
			{
				BOOL shouldActivate = FALSE;
				for (int i = 0; i < 6; i++)
				{
					char buffer[4];
					GetWindowText(gui::hEdit[i], buffer, 4);
					wc3::invCodes[i] = 0;
					for (int j = 0; j < keysCount; j++)
					{
						if (gui::keysNames[j] != KEY_RESERVED && gui::keysNames[j] == buffer) //93 decimal
						{
							wc3::invCodes[i] = j;
							shouldActivate = TRUE;
							break;
						}
					}
				}
				if (shouldActivate)
				{
					wc3::hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, wc3::HookProc, NULL, 0);
					wc3::hookEnabled = TRUE;
					SetWindowText(gui::hApplyButton, "Stop");
				}
				else
					MessageBox(hwnd, "No keys have been reassigned", "Message", MB_OK | MB_ICONASTERISK);
			}
			else
			{
				UnhookWindowsHookEx(wc3::hhkLowLevelKybd);
				wc3::hookEnabled = FALSE;
				SetWindowText(gui::hApplyButton, "Start");
			}
			SetFocus(gui::mainHwnd);
		}
		else if (LOWORD(wParam) == ID_CLEAR_PRESSED)
		{
			for (int i = 0; i < 6; i++)
			{
				gui::flags[i] = false;
				SetWindowText(gui::hEdit[i], "");
				SetFocus(gui::mainHwnd);
			}
		}
		return 0;
	case WM_KEYDOWN:
	{
		for (int j = 0; j < 6; j++)
		{
			if (gui::flags[j])
			{
				bool exists = false;
				for (int i = 0; i < 6; i++)
				{
					char buffer[4];
					GetWindowText(gui::hEdit[i], buffer, 4);
					if (buffer == gui::getKeyName(wParam) && buffer[0] != '#')
					{
						exists = true;
						break;
					}
				}
				if (!exists && gui::flags[j])
				{
					gui::flags[j] = false;
					SetWindowText(gui::hEdit[j], gui::getKeyName(wParam).c_str());
				}
				break;
			}
		}
		return TRUE;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (iMsg == WM_CHAR)
	{
		return TRUE;
	}
	else if (iMsg == WM_LBUTTONDOWN)
	{
		BOOL flag = false;
		auto hdc = GetDC(hwnd);
		int currentIndex = -1;
		for (int i = 0; i < 6; i++)
		{
			if (hwnd == gui::hEdit[i])
			{
				currentIndex = i;
				break;
			}
		}


		if (GetAsyncKeyState(VK_LMENU))
		{
			SetWindowText(hwnd, "");
		}
		else if (!gui::flags[currentIndex])
		{
			for (int i = 0; i < 6; i++)
			{
				if (gui::flags[i])
				{
					gui::flags[i] = false;
					break;
				}
			}
			flag = true;
		}
		for (int i = 0; i < 6; i++)
		{
			char buffer[4];
			if (gui::hEdit[i] == hwnd)
				gui::flags[i] = flag;
			GetWindowText(gui::hEdit[i], buffer, 4);
			SetWindowText(gui::hEdit[i], buffer);
		}
		DeleteDC(hdc);
		return TRUE;
	}
	return DefSubclassProc(hwnd, iMsg, wParam, lParam);
}
