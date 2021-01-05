#pragma once
#ifndef _GUI_H_
#define _GUI_H_

#define SELECTED_COLOR RGB(94, 94, 94)

#pragma comment(lib, "comctl32.lib")

#define ID_APPLY_PRESSED 0x10
#define ID_CLEAR_PRESSED 0x11
#define ID_INVENTORY_PRESSED 0x2
#define KEY_RESERVED "#"

#include "WndProc.h"

#include <Windows.h>
#include <string>
#include <CommCtrl.h>

const int keysCount = 136; 

namespace gui
{
	void initGui(HWND hwnd);
	void initKeysNames();
	std::string getKeyName(int vk_code);

	extern BOOL flags[6];
	extern HWND hEdit[6];
	extern HWND hApplyButton;
	extern HWND hClearButton;
	extern HWND mainHwnd;
	extern std::string keysNames[keysCount];
}

#endif