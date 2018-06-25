#ifndef _MAPWND_H_
#define _MAPWND_H_

#include "Wnd/BaseWnd.h"

class CMapWnd : public CBaseWnd
{
public:
	CMapWnd();

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual void InitWindow(void);

	void SetMapID(const char* pszRoomID);

private:
	CVerticalLayoutUI* m_pLayout;
	char szMapIDBuffer1;
	char szMapIDBuffer2;
	map<string, CLabelUI*> m_mapRoom;
};

#endif