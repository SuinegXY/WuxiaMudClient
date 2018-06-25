#include "stdafx.h"
#include "ListWnd.h"
#include "Define.h"
#include "MainFrame.h"
#include "Object\Parser.h"
#include "Util\ATW.h"
#include "PopupWnd.h"
#include "GameInfo\User.h"

CListWnd* CListWnd::s_pThis = NULL;

LPCTSTR CListWnd::GetWindowClassName() const
{
	return _T("ListFrame");
}

CDuiString CListWnd::GetSkinFile( void )
{
	return _T("List.xml");
}

LPCTSTR CListWnd::GetClass() const
{
	return _T("CListFrame");
}

CListWnd::CListWnd( HWND hWnd )
{
	m_hAttachWnd = hWnd;
	m_nBaseCX = VALUE_LIST_WND_CX;
	m_nBaseCY = VALUE_LIST_WND_CY;
	m_nSizeMinX = 50;
	m_nSizeMinY = 200;
}

void CListWnd::InitWindow( void )
{
	s_pThis = this;
	__super::InitWindow();
	m_PaintManager.SetMaxInfo(200, 1200);
	
	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	MoveWindow(m_hWnd, rcDesktop.right-m_nBaseCX, VALUE_MAIN_FRAME_CY+VALUE_HEIGHT_DIST, m_nBaseCX, m_nBaseCY, TRUE);

	CEventParser::GetInstance()->EventReg("players", CallbackList);

	m_pLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("list")));
}

void CListWnd::CallbackList( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	SafeCheck(s_pThis);

	string strContant = pszContant;
	string strMid = GetStringHead(strContant, EVENT_SEPERATE);
	if ( strContant.length() == 0 ) return;

	if ( strMid == "all" )
	{
		while( strContant.length() )
		{
			string strName = GetStringHead(strContant, EVENT_SEPERATE);
			string strID = GetStringHead(strName, "_");
			s_pThis->AddContant(strID.c_str(), strName.c_str());
		}
	}
	else if ( strMid == "add" )
	{
		string strName = GetStringHead(strContant, EVENT_SEPERATE);
		string strID = GetStringHead(strName, "_");
		s_pThis->AddContant(strID.c_str(), strName.c_str());
	}
	else if ( strMid == "dec" )
	{
		s_pThis->DecContant(strContant.c_str());
	}
}

void CListWnd::AddContant( const char* pszID, const char* pszName )
{
	if ( strcmp(pszID, CUser::GetInstance()->GetUser().c_str()) == 0 )
	{
		return;
	}
	CButtonUI* pBtn = m_mapPlayer[pszID];
	if ( pBtn ) return;
	pBtn = new CButtonUI;
	if ( pBtn )
	{
		wstring strUserData = L"P";
		strUserData.append(AtoW(pszID));
		//strUserData.append(L" ");
		//strUserData.append(AtoW(pszName));
		m_pLayout->Add(pBtn);
		pBtn->SetUserData(strUserData.c_str());
		pBtn->SetFixedHeight(16);
		int nWidth = GetStringLength(pszName);
		pBtn->SetFixedWidth(GetStringLength(pszName));
		RECT rc = {3,0,0,0};
		pBtn->SetPadding(rc);
		pBtn->SetFont(0);
		pBtn->SetTextColor(0xffffffff);
		pBtn->SetText(AtoW(pszName).c_str());
		pBtn->SetAttribute(_T("align"), _T("left"));
		m_mapPlayer.insert(make_pair(pszID, pBtn));
		m_mapPlayer[pszID] = pBtn;
	}
}

void CListWnd::DecContant( const char* pszID )
{
	CButtonUI* pBtn = m_mapPlayer[pszID];
	if ( pBtn )
	{
		m_pLayout->Remove(pBtn);
	}
}

void CListWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == _T("click") )
	{
		wstring wstrUserData = msg.pSender->GetUserData();
		POINT pt;
		GetCursorPos(&pt);
		CPopupWnd::GetInstance()->PopupCreate(pt.x, pt.y, WtoA(wstrUserData).c_str());
	}
}
