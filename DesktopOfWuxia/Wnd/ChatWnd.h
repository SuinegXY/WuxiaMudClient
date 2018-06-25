#ifndef _CHATFRAME_H_
#define _CHATFRAME_H_

#include "Wnd/BaseWnd.h"

enum
{
	AC_CHAT = 0,
	AC_SAY,
	AC_WISPER,
	AC_WISPERCALLBACK,
	AC_OVER,
	AC_DIGIT,
};

class CChatWnd : public CBaseWnd
{
public:
	CChatWnd(HWND hWnd);

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual void InitWindow(void);
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI& msg);
	//virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void Move();
private:
	static void CallbackChat(const char* pszContant);
	static void CallbackSay(const char* pszContant);
	static void CallbackWisper(const char* pszContant);
	static void CallbackWisperCallback(const char* pszContant);

	void AddContant(const char* pszContant, int nType);

private:
	static CChatWnd* s_pThis;

	CVerticalLayoutUI* m_pLayoutAll;
	CVerticalLayoutUI* m_pLayout[AC_OVER];
	wstring m_strLink[AC_OVER];
	DWORD m_dwTextColor[AC_OVER];
	int m_nDigit[AC_DIGIT];
};

#endif