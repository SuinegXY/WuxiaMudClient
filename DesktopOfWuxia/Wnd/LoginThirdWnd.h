#ifndef _LOGINTHIRDWND_H_
#define _LOGINTHIRDWND_H_

#include "BaseWnd.h"

class CLoginWnd;
class CLoginThirdWnd : public CBaseWnd
{
public:
	CLoginThirdWnd(CLoginWnd* pLoginWnd);
	~CLoginThirdWnd();

	void Show(const WCHAR* pwszUrl);

protected:
	void NaviUrl(const WCHAR* pwszUrl);
	void OnClick(TNotifyUI& msg);

	virtual LPCTSTR		GetWindowClassName() const; 
	virtual CDuiString	GetSkinFile(void);
	virtual LPCTSTR		GetClass() const;
	virtual void		InitWindow(void);
	virtual LRESULT		HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT		OnCEFCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static bool OnJSFunc(const string& name, vector<VARIANT>& argv, VARIANT*);
	static void OnCreateJSFunc(OLECHAR*** pFuncName, int& nLen);

	static CLoginThirdWnd* s_pThis;
private:
	CLoginWnd*  m_pLoginWnd;
	CChromeUI*	m_pChrome;
	wstring		m_wstrUrl;
	BOOL		m_bChrome;
};

#endif
