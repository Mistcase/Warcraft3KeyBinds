#pragma once
#ifndef _HOOK_H_
#define _HOOK_H_

#include <Windows.h>

namespace wc3
{
	LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);

	extern HHOOK hhkLowLevelKybd;
	extern DWORD hookEnabled;

	extern int invCodes[6];
}

#endif