#include "GUI.h"

void gui::initGui(HWND hwnd)
{
	hApplyButton = CreateWindow("Button", "Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 320, 90, 30, hwnd, (HMENU)ID_APPLY_PRESSED, NULL, NULL);
	hClearButton = CreateWindow("Button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		360, 263, 117, 30, hwnd, (HMENU)ID_CLEAR_PRESSED, NULL, NULL);

	HFONT newFont = CreateFont(45, 30, 0, 0, FW_THIN, FALSE, FALSE, 0, JOHAB_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, ("Arial"));

	int boxSize = 56;
	int index = 0;
	int indent = 5;
	int startPosX = 360, startposY = 80;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			hEdit[index] = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | WS_BORDER | ES_READONLY,
				j * (indent + boxSize) + startPosX, i * (indent + boxSize) + startposY, boxSize, boxSize, hwnd, (HMENU)(index + ID_INVENTORY_PRESSED), NULL, 0);
			SetWindowSubclass(hEdit[index], &EditSubclassProc, index + 1, 0);
			SendMessage(hEdit[index++], WM_SETFONT, (WPARAM)newFont, TRUE);
			//SetWindowLong(hEdit[index], GWL_EXSTYLE, GetWindowLong(hEdit[index], GWL_EXSTYLE) | WS_EX_LAYERED);
			//::SetLayeredWindowAttributes(hEdit[index], 0, (255 * 80) / 100, LWA_ALPHA);

		}
	}
}

void gui::initKeysNames()
{
	for (int i = 0; i < 65; i++)
		keysNames[i] = KEY_RESERVED;
	for (int i = 0x6A; i <= 0x6F; i++)
		keysNames[i] = KEY_RESERVED;
	for (int i = 65; i <= 90; i++)
		keysNames[i] += (char)i;
	for (int i = 0x60; i <= 0x69; i++)
		keysNames[i] = "N" + std::to_string(i - 0x60);
	for (int i = 0x70; i <= keysCount - 1; i++)
		keysNames[i] = "F" + std::to_string(i - 0x70 + 1);
	keysNames[0x5B] = "LW";
	keysNames[0x5C] = "RW";
	keysNames[93] = KEY_RESERVED;
	keysNames[0x5B] = KEY_RESERVED;
	keysNames[0x5E] = KEY_RESERVED;
	keysNames[0x5F] = KEY_RESERVED;
	keysNames[0x9] = "Tab";
	keysNames[0x14] = "CS";
	keysNames[0x20] = "Sce"; //Space
}

std::string gui::getKeyName(int vk_code)
{
	return (vk_code < 1 || vk_code > keysCount - 1) ? KEY_RESERVED : gui::keysNames[vk_code];
}

HWND gui::mainHwnd;
HWND gui::hClearButton;
HWND gui::hApplyButton;
HWND gui::hEdit[6];
BOOL gui::flags[6];

std::string gui::keysNames[keysCount];