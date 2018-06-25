#ifndef _POPUPWND_H_
#define _POPUPWND_H_

#include "Wnd/BaseWnd.h"

class CPopupWnd : public CBaseWnd
{
	InstanceDefineNoCreate(CPopupWnd);
public:
	CPopupWnd();
	static void NewInstance(HWND hWnd);

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual void InitWindow(void);
	virtual LRESULT OnSetFocus(UINT , WPARAM , LPARAM , BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnClick(TNotifyUI& msg);

	void PopupCreate(int x, int y, const char* pszID, BOOL bGet = TRUE);
	void AddMapEvent(vector<pair<string, wstring>>* pMapEvent);

protected:
	void Clear();
	void AddMenu(const char* pszEvent, const WCHAR* pwszTitle);
	void Move(int x, int y);

private:
	HWND m_hAttachWnd;
	CVerticalLayoutUI* m_pLayout;
	int m_nHeight;
	char m_cType;
};

#endif