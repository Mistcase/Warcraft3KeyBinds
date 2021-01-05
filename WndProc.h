#pragma once
#ifndef _WNDPROC_INCLUDED_
#define _WNDPROC_INCLUDED_

#include "GUI.h"
#include "hook.h"

#include <iostream>
#include <string>

HWND initWindow(HINSTANCE hInstance, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
#endif _WNDPROC_INCLUDED_