#include "stdafx.h"
#include "PopupWnd.h"
#include "Define.h"
#include "MainFrame.h"
#include "Util\ATW.h"
#include "Object\Router.h"
#include "GameInfo\Object.h"
#include "GameInfo\Item.h"
#include "GameInfo\Npc.h"

#define		WND_WIDTH		80

InstanceRealizeNoCreate(CPopupWnd);

void CPopupWnd::NewInstance( HWND hWnd )
{
	if ( s_pInstance == NULL )
	{
		s_pInstance = new CPopupWnd();
		if ( s_pInstance )
		{
			s_pInstance->Create(hWnd, NULL, WS_POPUP, 0, 0, 0, 0, 0);
			SetWindowLong(s_pInstance->GetHWND(), GWL_EXSTYLE, GetWindowLong(s_pInstance->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
			s_pInstance->m_hAttachWnd = hWnd;
		}
	}
}

LPCTSTR CPopupWnd::GetWindowClassName() const
{
	return _T("PopupFrame");
}

CDuiString CPopupWnd::GetSkinFile( void )
{
	return _T("Popup.xml");
}

LPCTSTR CPopupWnd::GetClass() const
{
	return _T("CListFrame");
}

CPopupWnd::CPopupWnd()
{
}

void CPopupWnd::InitWindow( void )
{
	m_pLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("Layout")));
}

void CPopupWnd::Clear()
{
	m_pLayout->RemoveAll();
	m_nHeight = 2;
}

void CPopupWnd::AddMenu( const char* pszEvent, const WCHAR* pwszTitle )
{
	CButtonUI* pBtn = new CButtonUI;
	if ( pBtn )
	{
		m_pLayout->Add(pBtn);
		pBtn->SetUserData(AtoW(pszEvent).c_str());
		pBtn->SetFixedHeight(20);
		pBtn->SetFixedWidth(WND_WIDTH);
		RECT rc = {0,-2,0,0};
		pBtn->SetTextPadding(rc);
		pBtn->SetBorderSize(1);
		pBtn->SetBorderColor(0xff000000);
		pBtn->SetFont(0);
		pBtn->SetTextColor(0xff111111);
		pBtn->SetText(pwszTitle);
		pBtn->SetAttribute(_T("align"), _T("vcenter"));
	}
	m_nHeight += 20;
}

void CPopupWnd::Move( int x, int y )
{
	MoveWindow(m_hWnd, x - 10, y + 5, WND_WIDTH + 2, m_nHeight, TRUE);
	ShowWindow();
}

LRESULT CPopupWnd::OnSetFocus( UINT , WPARAM , LPARAM , BOOL& bHandled )
{
	return 0;
}

LRESULT CPopupWnd::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//ShowWindow(false);
	return 0;
}

void CPopupWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == L"click" )
	{
		wstring wstrUserData = msg.pSender->GetUserData();
		if ( wstrUserData.length() )
		{
			if ( m_cType == 'E' )
			{
				::PostMessage(m_hAttachWnd, WM_USER_EVENT_TYPE, atoi(WtoA(wstrUserData).c_str()), NULL);
			}
			else if ( wstrUserData.find(L"wisper") == 0 )
			{
				CMainFrameWnd::EditType(wstrUserData.c_str(), L"密语");
			}
			else
			{
				CRouter::GetInstance()->Send(wstrUserData.c_str());
			}
			ShowWindow(false);
		}
	}
}

void CPopupWnd::PopupCreate( int x, int y, const char* pszID, BOOL bGet )
{
	Clear();
	m_cType = pszID[0];
	switch ( m_cType )
	{
	case '2':
		{
			AddMenu((string("look ") + pszID).c_str(), L"观察");
			if ( bGet )
			{
				AddMenu((string("get ") + pszID).c_str(), L"捡起");
			}
			else
			{
				AddMenu((string("drink ") + pszID).c_str(), L"丢弃");
			}
			AddMapEvent(CItemInfo::GetInstance()->GetEvent(pszID));
		}
		break;
	//case '1':
	case '3':
		{
			AddMenu((string("look ") + pszID).c_str(), L"观察");
			//AddMenu((string("ask ") + pszID + " all").c_str(), L"咨询");
			//AddMenu((string("fight ") + pszID).c_str(), L"比武");
			//AddMenu((string("kill ") + pszID).c_str(), L"杀死");
			AddMapEvent(CNpcInfo::GetInstance()->GetEvent(pszID));
		}
		break;
	case '4':
		break;
	case '5':
		{
			AddMenu((string("look ") + pszID).c_str(), L"观察");
			AddMapEvent(CObjectInfo::GetInstance()->GetEvent(pszID));
		}
		break;
	case 'E':
		{
			AddMenu("1", L"世界");
			AddMenu("2", L"本地");
			//AddMenu("3", L"密语");
			AddMenu("4", L"指令");
		}
		break;
	case 'p':
		{
			string strID = pszID + 1;
			//AddMenu((string("look ") + strID).c_str(), L"查看");
			AddMenu((string("playerGame ser ") + strID).c_str(), L"资料");
			AddMenu((string("wisper ") + strID.c_str() + " ").c_str(), L"密语");
		}
		break;
	case 'P':
		{
			string strID = pszID + 1;
			AddMenu((string("playerGame ser ") + strID).c_str(), L"资料");
			AddMenu((string("wisper ") + strID.c_str() + " ").c_str(), L"密语");
		}
		break;
	default:
		break;
	}
	Move(x, y);
}

void CPopupWnd::AddMapEvent( vecEvent* pMapEvent )
{
	vecEvent::iterator pEvent = pMapEvent->begin();
	while ( pEvent != pMapEvent->end() )
	{
		string strEvent = pEvent->first;
		wstring wstrTitle = pEvent->second;
		do
		{
			string strEventSingle = GetStringHead(strEvent, "_");
			wstring wstrTitleSingle = GetStringHeadW(wstrTitle, L"_");
			AddMenu(strEventSingle.c_str(), wstrTitleSingle.c_str());
		} while ( strEvent.length() );
		++ pEvent;
	}
}
