#include "hook.h"

LRESULT CALLBACK wc3::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* details = (KBDLLHOOKSTRUCT*)lParam;
	INPUT ip;

	if (nCode == HC_ACTION)
	{
		ip.type = INPUT_KEYBOARD;
		if (details->vkCode == invCodes[0])
			ip.ki.wVk = 0x67;
		else if (details->vkCode == invCodes[1])
			ip.ki.wVk = 0x68;
		else if (details->vkCode == invCodes[2])
			ip.ki.wVk = 0x64;
		else if (details->vkCode == invCodes[3])
			ip.ki.wVk = 0x65;
		else if (details->vkCode == invCodes[4])
			ip.ki.wVk = 0x61;
		else if (details->vkCode == invCodes[5])
			ip.ki.wVk = 0x62;
		else
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		ip.ki.time = 0;
		ip.ki.dwFlags = 0;
		if (wParam == WM_KEYUP)
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
		return 1;
	}

}

int wc3::invCodes[6];

HHOOK wc3::hhkLowLevelKybd;
DWORD wc3::hookEnabled = 0;
