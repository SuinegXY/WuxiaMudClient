#include "HWnd.h"

BOOL CHWnd::IsSameWindow( HWND hGet, HWND hRes )
{
	if ( hGet == hRes ) return TRUE;

	HWND hWnd = GetParent(hGet);
	while ( hWnd != NULL )
	{
		if ( hWnd == hRes ) return TRUE;
		hWnd = GetParent(hWnd);
	}

	return FALSE;
}

