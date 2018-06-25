#ifndef _BASE_H_
#define _BASE_H_

#include "..\DuiLib\Utils\WinImplBase.h"

class CBaseWnd : public WindowImplBase
{
public:
	CBaseWnd(void);
	~CBaseWnd(void);

	virtual void InitWindow(void);
	virtual LPCTSTR GetWindowClassName() const; 
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetSkinFolder(void);
	virtual CDuiString GetSkinFile(void) = 0;
	virtual CDuiString GetZIPFileName() const;
	LPCTSTR GetResourceID() const;
	virtual LPCTSTR GetClass() const;
	virtual LRESULT HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	virtual LRESULT OnSetFocus(UINT , WPARAM , LPARAM , BOOL& bHandled);

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
		virtual void OnSelectChanged(TNotifyUI& msg);

protected:
	HWND m_hAttachWnd;
	int m_nBaseCX;
	int m_nBaseCY;
	int m_nSizeMinX;
	int m_nSizeMinY;
};

#endif