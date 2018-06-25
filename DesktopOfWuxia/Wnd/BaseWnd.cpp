#include "StdAfx.h"
#include "BaseWnd.h"
#include "resource1.h"

DUI_BEGIN_MESSAGE_MAP(CBaseWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSEENTER,OnClick)
DUI_END_MESSAGE_MAP()

LPCTSTR CBaseWnd::GetWindowClassName() const 
{ 
	return _T("CBaseWnd");
}

UILIB_RESOURCETYPE CBaseWnd::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CDuiString CBaseWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CBaseWnd::GetZIPFileName() const
{
	return _T("MarcoInstall.zip");
}

LPCTSTR CBaseWnd::GetClass() const
{
	return _T("CBaseWnd");
}

LPCTSTR CBaseWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

CBaseWnd::~CBaseWnd(void) 
{
}

CBaseWnd::CBaseWnd(void)
	: m_nBaseCX(400)
	, m_nBaseCY(200)
	, m_nSizeMinX(400)
	, m_nSizeMinY(200)
{
	m_hAttachWnd = NULL;
}

void CBaseWnd::InitWindow( void )
{
	m_PaintManager.SetMinInfo(m_nSizeMinX, m_nSizeMinY);
}

void CBaseWnd::OnClick( TNotifyUI& msg )
{
}

void CBaseWnd::OnSelectChanged( TNotifyUI& msg )
{
}

LRESULT CBaseWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnSetFocus( UINT , WPARAM , LPARAM , BOOL& bHandled )
{
	if ( m_hAttachWnd )
	{
		bHandled = TRUE;
		//::SetFocus(m_hAttachWnd);
		::PostMessage(m_hAttachWnd, WM_SETFOCUS, NULL, NULL);
	}
	return -1;
}
