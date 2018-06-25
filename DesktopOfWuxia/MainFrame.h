#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#include "Wnd/BaseWnd.h"

class CRouter;
class CChatWnd;
class CListWnd;
class CMapWnd;
class CRoleWnd;
class CRoleInfoWnd;
class CLoginWnd;
class CMainFrameWnd : public CBaseWnd
{
public:
	CMainFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual HRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow(void);
	virtual LRESULT HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI& msg);
	virtual LRESULT OnSetFocus(UINT , WPARAM , LPARAM , BOOL& bHandled);
	virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void Move();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnReturn(TNotifyUI& msg);

public:
	static void EditType(const WCHAR* pwszEvent, const WCHAR* pwszTitle, BOOL bImportant = FALSE);

protected:
	static void CallbackInfo(const char* pszContant);
	static void CallbackRoom(const char* pszContant);
	static void CallbackStatus(const char* pszContant);
	static void CallbackFoodAndWater(const char* pszContant);
	static void CallbackFirstInit(const char* pszContant);

	void SetRoomID(string& strRoomID);
	void SetRoomList(string& strRoomList);
	void AddRoomList(string& strID);
	void FixRoomList(string& strID, BOOL bAdd);
	void EditSend();
	void Info(DWORD dwRGB, const WCHAR* pwszContant);

	void FoodOrWater(BOOL bFood, int nNow, int nMax = 0);

private:
	static CMainFrameWnd* s_pThis;
	CRouter* m_pRouter;
	CChatWnd* m_pChatWnd;
	CListWnd* m_pListWnd;
	CMapWnd* m_pMapWnd;
	CRoleWnd* m_pRoleWnd;
	CRoleInfoWnd* m_pRoleInfoWnd;
	CLoginWnd* m_pLoginWnd;
	CControlUI* m_pBtnEditType;

	CEditUI* m_pEdit;
	CVerticalLayoutUI* m_pRoomListLayout;

	map<wstring, CBaseWnd*> m_mapWindow;
	map<string, CButtonUI*> m_mapRoomList;
	map<string, int> m_mapItemNum;
	string m_strWisperID;
	int m_nInfoWidth;
};

#endif