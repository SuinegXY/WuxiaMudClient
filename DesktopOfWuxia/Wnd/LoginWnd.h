#ifndef _LOGINWND_H_
#define _LOGINWND_H_

#include "BaseWnd.h"

class CLoginThirdWnd;
class CLoginWnd : public CBaseWnd
{
public:
	CLoginWnd();
	~CLoginWnd();

	void Show(int nType = 0);
	void SetError(const TCHAR* ptszError);

	void Login(const char* pszJasn);
	virtual void CenterWindow();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnReturn(TNotifyUI& msg);

protected:
	void OnClick(TNotifyUI& msg);

	virtual LPCTSTR		GetWindowClassName() const; 
	virtual CDuiString	GetSkinFile(void);
	virtual LPCTSTR		GetClass() const;
	virtual void		InitWindow(void);
	virtual LRESULT		HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnGetIdentCode();
	void GetIdentCodeTimeLimit();
	void GetIdentCodeShow();

	void OnLogin();

	static DWORD WINAPI OnGetIdentCodeThread(LPVOID lp);
	static DWORD WINAPI OnLoginUserPassThread(LPVOID lp);
	static DWORD WINAPI OnLoginUserIdentThread(LPVOID lp);

private:
	CLoginThirdWnd* m_pThirdWnd;
	CEditUI*	m_pUsername;
	CButtonUI*	m_pGetIdentCode;
	CButtonUI*	m_pLogin;
	int m_nGetIdentCodeTime;
	int m_nType;
	int m_nLoginType;
};

#endif
