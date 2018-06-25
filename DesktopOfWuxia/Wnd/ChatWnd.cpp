#include "stdafx.h"
#include "ChatWnd.h"
#include "Define.h"
#include "MainFrame.h"
#include "Object\Parser.h"
#include "Util\ATW.h"
#include "PopupWnd.h"

#define USER_COLOR	0xffcccccc
#define MAX_LINE	500

CChatWnd* CChatWnd::s_pThis = NULL;

LPCTSTR CChatWnd::GetWindowClassName() const
{
	return _T("ChatFrame");
}

CDuiString CChatWnd::GetSkinFile( void )
{
	return _T("Chat.xml");
}

LPCTSTR CChatWnd::GetClass() const
{
	return _T("CChatFrame");
}

CChatWnd::CChatWnd( HWND hWnd )
{
	m_hAttachWnd = hWnd;
	m_nBaseCX = 300;
	m_nBaseCY = VALUE_LIST_WND_CY;
	m_nSizeMinX = 200;
	m_nSizeMinY = 200;
}

void CChatWnd::InitWindow( void )
{
	s_pThis = this;
	__super::InitWindow();
	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	MoveWindow(m_hWnd, rcDesktop.right-m_nBaseCX-VALUE_LIST_WND_CX, VALUE_MAIN_FRAME_CY+VALUE_HEIGHT_DIST, m_nBaseCX, m_nBaseCY, TRUE);

	CEventParser::GetInstance()->EventReg("chat", CallbackChat);
	CEventParser::GetInstance()->EventReg("say", CallbackSay);
	CEventParser::GetInstance()->EventReg("wisper", CallbackWisper);
	CEventParser::GetInstance()->EventReg("wisperCallback", CallbackWisperCallback);

	m_pLayoutAll = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("all")));
	m_pLayout[0] = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("world")));
	m_pLayout[1] = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("local")));
	m_pLayout[2] = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("wisper")));
	m_pLayout[3] = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("wisper")));
	m_strLink[0] = L"大声喊道：";
	m_strLink[1] = L"：";
	m_strLink[2] = L"悄悄对你说：";
	m_strLink[3] = L"听到你说：";
	m_dwTextColor[0] = 0xffffff22;
	m_dwTextColor[1] = 0xffccccff;
	m_dwTextColor[2] = 0xffff22ff;
	m_dwTextColor[3] = 0xffff22ff;
	ZeroMemory(m_nDigit, sizeof(m_nDigit));
}

void CChatWnd::OnSelectChanged( TNotifyUI& msg )
{
	static CControlUI* pBuffer = m_PaintManager.FindControl(_T("all"));
	COptionUI* pUI = static_cast<COptionUI*>(msg.pSender);
	if ( pUI )
	{
		CDuiString strName = pUI->GetUserData();
		CControlUI* pObecjt = m_PaintManager.FindControl(strName);
		if ( pObecjt )
		{
			pBuffer->SetVisible(false);
			pObecjt->SetVisible(true);
			pBuffer = pObecjt;
			////////////////////////////////测试用
			//static int nIndex = 0;
			//char szBuffer[5];
			//sprintf(szBuffer, "%d", nIndex);
			//++nIndex;
			//string str = "chat 11 熊宇aaa 向你问好你好吗asdfasfdasfd";
			//CEventParser::GetInstance()->EventDo(str+szBuffer);
			//string str2 = "say 12 熊宇你好的 向好你好吗";
			//CEventParser::GetInstance()->EventDo(str2+szBuffer);
			//string str3 = "info 0xffffffff asdfask 向你问好你好吗";
			//CEventParser::GetInstance()->EventDo(str3+szBuffer);
		}
	}
}

void CChatWnd::Move()
{
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	RECT rcAttach;
	GetWindowRect(m_hAttachWnd, &rcAttach);
	MoveWindow(m_hWnd, rcAttach.right+1, rcAttach.top, rcClient.right, rcClient.bottom, TRUE);
}

//LRESULT CChatWnd::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
//{
//	//int x = lParam&0xffff;
//	//ControlAction(m_pLayoutAll, SetFixedWidth, x);
//	//ControlAction(m_pLayout[0], SetFixedWidth, x);
//	//ControlAction(m_pLayout[1], SetFixedWidth, x);
//	//ControlAction(m_pLayout[2], SetFixedWidth, x);
//	return 0;
//}

void CChatWnd::CallbackChat( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	SafeCheck(s_pThis);
	s_pThis->AddContant(pszContant, AC_CHAT);
}

void CChatWnd::CallbackSay( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	SafeCheck(s_pThis);
	s_pThis->AddContant(pszContant, AC_SAY);
}

void CChatWnd::CallbackWisper( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	SafeCheck(s_pThis);
	s_pThis->AddContant(pszContant, AC_WISPER);
}

void CChatWnd::CallbackWisperCallback( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	SafeCheck(s_pThis);
	s_pThis->AddContant(pszContant, AC_WISPERCALLBACK);
}

#define SAME_NEW(name, type) C##type##UI * p##name##1 = new C##type##UI;C##type##UI * p##name##2 = new C##type##UI;
#define SAME_ADD(father, name) p##father##1->Add(p##name##1);p##father##2->Add(p##name##2);
#define SAME_ACT(name, action, ...) p##name##1->##action(__VA_ARGS__);p##name##2->##action(__VA_ARGS__);

void CChatWnd::AddContant( const char* pszContant, int nType )
{
	wstring wstrResource = AtoW(pszContant);
	wstring wstrID = GetStringHeadW(wstrResource, _T(EVENT_SEPERATE));
	wstring wstrName = GetStringHeadW(wstrResource, _T(EVENT_SEPERATE));
	wstring wstrContant = m_strLink[nType] + wstrResource;

	CVerticalLayoutUI* pLayout1 = m_pLayoutAll;
	CVerticalLayoutUI* pLayout2 = m_pLayout[nType];
	SAME_NEW(Bar, HorizontalLayout);
	SAME_ADD(Layout, Bar);
	SAME_ACT(Bar, SetFixedHeight, 18);

	SAME_NEW(Name, Button);
	SAME_ADD(Bar, Name);
	SAME_ACT(Name, SetFont, 0);
	SAME_ACT(Name, SetText, wstrName.c_str());
	SAME_ACT(Name, SetTextColor, USER_COLOR);
	SAME_ACT(Name, SetUserData, (wstring(L"p")+wstrID).c_str());
	SAME_ACT(Name, SetFixedWidth, GetStringLengthW(wstrName.c_str()));
	SAME_ACT(Name, SetAttribute, _T("align"), _T("center"));
	RECT rc = {1, 0, 0, 0};
	SAME_ACT(Name, SetTextPadding, rc);

	SAME_NEW(Contant, Label);
	SAME_ADD(Bar, Contant);
	SAME_ACT(Contant, SetFont, 0);
	SAME_ACT(Contant, SetText, wstrContant.c_str());
	SAME_ACT(Contant, SetTextColor, m_dwTextColor[nType]);
	SAME_ACT(Contant, SetAttribute, _T("align"), _T("left"));

	if ( m_nDigit[3] == MAX_LINE )
	{
		pLayout1->RemoveAt(0);
	}
	else
	{
		++ m_nDigit[3];
	}
	if ( nType == AC_WISPERCALLBACK ) nType == AC_WISPER;
	if ( m_nDigit[nType] == MAX_LINE )
	{
		pLayout2->RemoveAt(0);
	}
	else
	{
		++ m_nDigit[nType];
	}

	SendMessage(WM_PAINT, NULL, NULL);
	SAME_ACT(Layout, EndDown);
}

void CChatWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == _T("click") )
	{
		wstring wstrUserData = msg.pSender->GetUserData();
		POINT pt;
		GetCursorPos(&pt);
		CPopupWnd::GetInstance()->PopupCreate(pt.x, pt.y, WtoA(wstrUserData).c_str());
	}
}
