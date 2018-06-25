#ifndef _WINDOWS_COMMON_H_
#define _WINDOWS_COMMON_H_

#include <Windows.h>

class CHWnd
{
public:
	static BOOL IsSameWindow(HWND hGet, HWND hRes);
};

#endif