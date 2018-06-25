#ifndef _ROLEINFOWND_H_
#define _ROLEINFOWND_H_

#include "Wnd/BaseWnd.h"

class CRoleInfoWnd : public CBaseWnd
{
public:
	CRoleInfoWnd();

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual void InitWindow(void);


protected:
	virtual void OnClick(TNotifyUI& msg);

	void Add();
	void New();
	void Copy(const WCHAR* pszGameID);
	void Del(const WCHAR* pszGameName);

	void Show();

	static void CallbackPlayer(const char* pszContant);

private:
	static CRoleInfoWnd* s_pThis;
	CVerticalLayoutUI* m_pGamesLayout;
	CVerticalLayoutUI* m_pAddLayout;

	CVerticalLayoutUI* m_pNew;
	CEditUI* m_pEditName;
	CEditUI* m_pEditID;
	CButtonUI* m_pEditBtn;

	int m_nHeight;
	BOOL m_bSelf;
	map<string, CVerticalLayoutUI*> m_mapGames;
};

#endif