#ifndef _LISTWND_H_
#define _LISTWND_H_

#include "Wnd/BaseWnd.h"

class CListWnd : public CBaseWnd
{
public:
	CListWnd(HWND hWnd);

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual void InitWindow(void);

protected:
	virtual void OnClick(TNotifyUI& msg);

	static void CallbackList(const char* pszContant);

	void AddContant(const char* pszID, const char* pszName);
	void DecContant(const char* pszID);

private:
	static CListWnd* s_pThis;

	CVerticalLayoutUI* m_pLayout;
	map<string, CButtonUI*> m_mapPlayer;
};

#endif