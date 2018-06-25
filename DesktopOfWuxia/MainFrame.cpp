#include "stdafx.h"
#include "MainFrame.h"
#include "Define.h"
#include "Wnd/ChatWnd.h"
#include "Wnd/ListWnd.h"
#include "Wnd/MapWnd.h"
#include "Wnd/PopupWnd.h"
#include "Wnd/LoginWnd.h"
#include "Wnd/RoleWnd.h"
#include "Wnd/RoleInfoWnd.h"
#include "Object/Router.h"
#include "Logger.h"
#include "Util/ATW.h"
#include "Object/Parser.h"
#include "GameInfo/Room.h"
#include "GameInfo/Item.h"
#include "GameInfo/Npc.h"
#include "GameInfo/Object.h"
#include "GameInfo/User.h"

#define MAX_LINE	100

DUI_BEGIN_MESSAGE_MAP(CMainFrameWnd, CBaseWnd)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RETURN, OnReturn)
DUI_END_MESSAGE_MAP()

CMainFrameWnd* CMainFrameWnd::s_pThis = NULL;

LPCTSTR CMainFrameWnd::GetWindowClassName() const
{
	return _T("MainFrame");
}

CDuiString CMainFrameWnd::GetSkinFile( void )
{
	return _T("MainFrame.xml");
}

LPCTSTR CMainFrameWnd::GetClass() const
{
	return _T("CMainFrame");
}

CMainFrameWnd::CMainFrameWnd( void )
{
	m_nBaseCX	= VALUE_MAIN_FRAME_CX;
	m_nBaseCY	= VALUE_MAIN_FRAME_CY;
	m_pChatWnd	= NULL;
	m_pListWnd	= NULL;
	m_pMapWnd	= NULL;
	m_pRoleWnd	= NULL;
	m_pLoginWnd = NULL;
	m_pEdit		= NULL;
	m_pBtnEditType = NULL;
	m_nInfoWidth = 280;
}

void CMainFrameWnd::InitWindow( void )
{
	s_pThis = this;
	__super::InitWindow();

	CEventParser::GetInstance()->EventReg("info", CallbackInfo);
	CEventParser::GetInstance()->EventReg("room", CallbackRoom);
	CEventParser::GetInstance()->EventReg("status", CallbackStatus);
	CEventParser::GetInstance()->EventReg("fandw", CallbackFoodAndWater);
	CEventParser::GetInstance()->EventReg("firstinit", CallbackFirstInit);

	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	MoveWindow(m_hWnd, rcDesktop.right-m_nBaseCX, VALUE_HEIGHT_DIST, m_nBaseCX, m_nBaseCY, TRUE);
	CUser::GetInstance()->SetWnd(m_hWnd);

	m_pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("EditSend")));
	m_pBtnEditType = m_PaintManager.FindControl(_T("EditType"));
	m_pRoomListLayout = static_cast<CVerticalLayoutUI*>(s_pThis->m_PaintManager.FindControl(_T("LayoutRoomList")));

	m_pChatWnd = new CChatWnd(m_hWnd);
	if ( m_pChatWnd )
	{
		m_pChatWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 0, 0);
		SetWindowLong(m_pChatWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pChatWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
		m_pChatWnd->ShowWindow(true);
		m_mapWindow.insert(make_pair(L"chat", m_pChatWnd));
	}
	m_pListWnd = new CListWnd(m_hWnd);
	if ( m_pListWnd )
	{
		m_pListWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 0, 0);
		SetWindowLong(m_pListWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pListWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
		m_pListWnd->ShowWindow(true);
		m_mapWindow.insert(make_pair(L"list", m_pListWnd));
	}
	m_pMapWnd = new CMapWnd;
	if ( m_pMapWnd )
	{
		m_pMapWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 0, 0);
		SetWindowLong(m_pMapWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pMapWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
		m_mapWindow.insert(make_pair(L"map", m_pMapWnd));
	}
	m_pRoleWnd = new CRoleWnd;
	if ( m_pRoleWnd )
	{
		m_pRoleWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 0, 0);
		SetWindowLong(m_pRoleWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pRoleWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
		m_mapWindow.insert(make_pair(L"role", m_pRoleWnd));
	}
	m_pRoleInfoWnd = new CRoleInfoWnd;
	if ( m_pRoleInfoWnd )
	{
		m_pRoleInfoWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 0, 0);
		SetWindowLong(m_pRoleInfoWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pRoleInfoWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
	}
	m_pLoginWnd = new CLoginWnd;
	if ( m_pLoginWnd )
	{
		m_pLoginWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 600, 430);
		SetWindowLong(m_pLoginWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pLoginWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
	}
	CPopupWnd::NewInstance(m_hWnd);

	m_pRouter = CRouter::GetInstance();
	m_pRouter->Start();
	//SetRoomID(string("10101"));
	//SetRoomList(string("50101 30101"));
	//CallbackStatus("100 100 100 0 0");
	//FoodOrWater(TRUE, 50, 300);
	//FoodOrWater(FALSE, 10, 300);
	//FoodOrWater(TRUE, -10);
	//FoodOrWater(TRUE, -10);
	//CEventParser::GetInstance()->EventDo(string("money 210541205"));
	//CEventParser::GetInstance()->EventDo(string("playerGame  suineg 57642 game1 id1 game2 id2"));
	//CEventParser::GetInstance()->EventDo(string("skills 40011_1_0"));
	
}

HRESULT CMainFrameWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( wParam == SC_CLOSE )
	{
		bHandled = TRUE;
		PostQuitMessage(0L);
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrameWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch (uMsg)
	{
	case WM_USER_EVENT_TYPE:
		{
			if ( m_pBtnEditType == NULL ) break;
			switch ( wParam )
			{
			case 1:
				m_pBtnEditType->SetText(_T("世界"));
				m_pBtnEditType->SetUserData(_T("chat "));
				break;
			case 2:
				m_pBtnEditType->SetText(_T("本地"));
				m_pBtnEditType->SetUserData(_T("say "));
				break;
			case 3:
				m_pBtnEditType->SetText(_T("悄悄"));
				m_pBtnEditType->SetUserData(AtoW(string("wisper ") + m_strWisperID + " ").c_str());
				break;
			case 4:
				m_pBtnEditType->SetText(_T("指令"));
				m_pBtnEditType->SetUserData(_T(""));
				break;
			}
		}
		break;
	case WM_USER_LOGIN:
		{
			string strUserID = CUser::GetInstance()->GetUser();
			if ( strUserID.length() )
			{
				CControlUI* pBtn = m_PaintManager.FindControl(_T("UserName"));
				ControlAction(pBtn, SetEnabled, false);
				ControlAction(pBtn, SetText, _T("游戏(已登录)"));
			}
		}
		break;
	//case WM_USER_SCROLL_END:
	//	{	
	//		static CVerticalLayoutUI* pLayout = static_cast<CVerticalLayoutUI*>(s_pThis->m_PaintManager.FindControl(_T("LayoutInfoList")));
	//		ControlAction(pLayout, EndDown);
	//	}
	//	break;
	}
	return 0;
}

void CMainFrameWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == _T("click") )
	{
		BOOL bNeedSetEdit = TRUE;
		if( msg.pSender->GetName() == _T("closebtn") )
		{
			PostQuitMessage(0L);
		}
		else if( msg.pSender->GetName() == _T("minbtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if ( msg.pSender->GetName() == _T("EditClear") )
		{
			//CallbackStatus("100 100 100 0 0");
			ControlAction(m_pEdit, SetText ,_T(""));
		}
		else if ( msg.pSender->GetName() == _T("EditType") )
		{
			RECT rc;
			GetWindowRect(m_hWnd, &rc);
			CPopupWnd::GetInstance()->PopupCreate(rc.left+10, rc.bottom-82, "E");
			bNeedSetEdit = FALSE;
		}
		else if ( msg.pSender->GetName() == _T("UserName") )
		{
			ControlAction(m_pLoginWnd, Show, FALSE);
			ControlAction(m_pLoginWnd, CenterWindow);
			bNeedSetEdit = FALSE;
		}
		else if ( msg.pSender->GetName().Find(L"Room") == 0 )
		{
			wstring wstrToward = msg.pSender->GetUserData();
			if ( wstrToward.length() )
			{
				CRouter::GetInstance()->Send("walk", wstrToward.c_str());
			}
//#ifdef _DEBUG
//			Info(RGB_RED, wstrToward.c_str());
//#endif
		}
		//else if ( msg.pSender->GetUserData().Find(L"SHOW") == 0 )
		//{
		//	wstring wstrUserData = msg.pSender->GetUserData();
		//	m_pEdit->SetText(wstrUserData.c_str()+5);
		//}
		else if ( msg.pSender->GetUserData().Find(L"List") == 0  )
		{
			wstring wstrUserData = msg.pSender->GetUserData();
			POINT pt;
			GetCursorPos(&pt);
			CPopupWnd::GetInstance()->PopupCreate(pt.x, pt.y, WtoA(wstrUserData).c_str()+5);
			bNeedSetEdit = FALSE;
		}
		if ( bNeedSetEdit )
		{
			ControlAction(m_pEdit, SetFocus);
			ControlAction(m_pEdit, SetSel, m_pEdit->GetText().GetLength(), -1);
		}
	}
}

void CMainFrameWnd::Move()
{
}

LRESULT CMainFrameWnd::OnSetFocus( UINT , WPARAM , LPARAM , BOOL& bHandled )
{
	if (m_pEdit)
	{
		m_pEdit->SetFocus();
		m_pEdit->SetSel(m_pEdit->GetText().GetLength(), -1);
	}
	CPopupWnd::GetInstance()->ShowWindow(false);
	return 0;
}

void CMainFrameWnd::OnSelectChanged( TNotifyUI& msg )
{
	COptionUI* pUI = static_cast<COptionUI*>(msg.pSender);
	if ( pUI )
	{
		wstring strName = pUI->GetUserData();
		if ( strName == L"role" && pUI->IsSelected() )
		{
			if ( CUser::GetInstance()->GetUser().length() == 0 )
			{
				ControlAction(m_pLoginWnd, Show, FALSE);
				ControlAction(m_pLoginWnd, CenterWindow);
				pUI->Selected(false);
				return;
			}
			else
			{
				string strSend = "playerGame ser ";
				strSend.append(CUser::GetInstance()->GetUser());
				CRouter::GetInstance()->Send(strSend.c_str());
			}
		}
		CBaseWnd* pWnd = m_mapWindow[strName];
		if ( pWnd )
		{
			pWnd->ShowWindow((static_cast<COptionUI*>(msg.pSender))->IsSelected());
		}
	}
}

void CMainFrameWnd::OnReturn( TNotifyUI& msg )
{
	EditSend();
}

LRESULT CMainFrameWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( wParam == 0x0d )
	{
		bHandled = TRUE;
		EditSend();
	}
	return 0;
}

void CMainFrameWnd::EditSend()
{
	if ( m_pEdit == NULL ) return;
	wstring wstrEvent = m_pBtnEditType->GetUserData();
	wstring wstrContant = m_pEdit->GetText();
	if ( wstrContant.length() )
	{
		CRouter::GetInstance()->Send((wstrEvent + wstrContant).c_str());
	}
	if ( !m_pBtnEditType->IsEnabled() )
	{
		m_pBtnEditType->SetText(_T("世界"));
		m_pBtnEditType->SetUserData(_T("chat "));
		m_pBtnEditType->SetEnabled(true);
	}
	m_pEdit->SetFocus();
	//m_pEdit->SetSel(m_pEdit->GetText().GetLength(), -1);
	m_pEdit->SetSel(0, -1);
}

void CMainFrameWnd::EditType( const WCHAR* pwszEvent, const WCHAR* pwszTitle, BOOL bImportant )
{
	if ( !s_pThis->m_pBtnEditType->IsEnabled() ) return;
	s_pThis->m_pBtnEditType->SetText(pwszTitle);
	s_pThis->m_pBtnEditType->SetUserData(pwszEvent);
	if ( bImportant )
	{
		s_pThis->m_pBtnEditType->SetEnabled(false);
	}
}

void CMainFrameWnd::CallbackInfo( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	string strRGB = GetStringHead(strContant, EVENT_SEPERATE);
	if ( strContant.length() == 0 )
	{
		s_pThis->Info(RGB_WHITE, AtoW(strRGB).c_str());
	}
	else
	{
		DWORD dwRGB = 0;
		sscanf(strRGB.c_str(), "%d", &dwRGB);
		if  ( dwRGB == 0 )
		{
			s_pThis->Info(RGB_WHITE, AtoW(strRGB + " " + strContant).c_str());
		}
		else
		{
			s_pThis->Info(dwRGB, AtoW(strContant).c_str());
		}
	}
}

void CMainFrameWnd::Info( DWORD dwRGB, const WCHAR* pwszContant )
{
	static CVerticalLayoutUI* pLayout = static_cast<CVerticalLayoutUI*>(s_pThis->m_PaintManager.FindControl(_T("LayoutInfoList")));
	if ( pLayout == NULL ) return;
	static int nDigit = 0;

	CTextUI* pUI = new CTextUI;
	if ( pUI )
	{
		pLayout->Add(pUI);
		RECT rc = {3,0,0,0};
		pUI->SetPadding(rc);
		pUI->SetFont(0);
		pUI->SetTextColor(dwRGB);
		pUI->SetText(pwszContant);
		pUI->SetAttribute(_T("align"), _T("left"));
	}

	if ( nDigit == MAX_LINE )
	{
		pLayout->RemoveAt(0);
	}
	else
	{
		++ nDigit;
	}
	//pLayout->NeedUpdate();
	//pLayout->Invalidate();
	SendMessage(WM_PAINT, NULL, NULL);
	//PostMessage(WM_USER_SCROLL_END, NULL, NULL);

	pLayout->EndDown();
}

void CMainFrameWnd::CallbackRoom( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	string strRoom = GetStringHead(strContant, EVENT_SEPERATE);
	if ( strcmp(strRoom.c_str(), "add") == 0 )
	{
		s_pThis->FixRoomList(strContant, TRUE);
	}
	else if ( strcmp(strRoom.c_str(), "dec") == 0 )
	{
		s_pThis->FixRoomList(strContant, FALSE);
	}
	else
	{
		s_pThis->SetRoomID(strRoom);
		s_pThis->SetRoomList(strContant);
	}
}

void CMainFrameWnd::SetRoomID( string& strRoomID )
{
	wstring strContant = CRoomInfo::GetInstance()->GetInfo(strRoomID);
	if ( strContant.length() )
	{
//#ifdef _DEBUG
//		Info(RGB_RED, AtoW(strRoomID).c_str());
//#endif
		ControlAction(m_pMapWnd, SetMapID, strRoomID.c_str());
		int nDigit = 1;
		WCHAR wszControlName[20] = {0};
		do
		{
			wstring strRoomInfo = GetStringSepa(strContant, L"~");
			wstring strUserData = L"";
			if ( strRoomInfo.length() )
			{
				strUserData = GetStringSepa(strRoomInfo, L"_");
				//wstring strRoomID = GetStringSepa(strRoomInfo, L"_");
			}
			wsprintf(wszControlName, L"Room_%d", nDigit);
			CControlUI* pUI = m_PaintManager.FindControl(wszControlName);
			if ( pUI )
			{
				pUI->SetText(strRoomInfo.c_str());
				if ( strUserData.length() )
				{
					pUI->SetUserData(strUserData.c_str());
					pUI->SetEnabled();
				}
				else
				{
					pUI->SetEnabled(false);
				}
			}
			++ nDigit;
		}
		while ( strContant.length() );
	}
	else
	{
		CLogger::GetInstance()->Log(__FUNCTION__, strRoomID.c_str(), "房间信息不存在");
	}
}

void CMainFrameWnd::SetRoomList( string& strRoomList )
{
	if ( m_pRoomListLayout == NULL ) return;
	m_pRoomListLayout->RemoveAll();
	m_mapRoomList.clear();
	m_mapItemNum.clear();

	while( strRoomList.length() )
	{
		string strID = GetStringHead(strRoomList, EVENT_SEPERATE);
		AddRoomList(strID);
	}
}

void CMainFrameWnd::AddRoomList( string& strID )
{
	if ( m_pRoomListLayout == NULL ) return;
	if ( strID.length() == 0 ) return;
	wstring wstrContant;
	string strKey;
	DWORD dwRGB;
	switch ( strID[0] )
	{
	case '2':
		{
			string strNum = strID;
			strID = GetStringHead(strNum, "_");
			strKey = strID;
			int nNum = atoi(strNum.c_str());
			wstrContant = CItemInfo::GetInstance()->GetInfo(strID, nNum);
			dwRGB = CItemInfo::GetInstance()->GetRGBA(strID);
			m_mapItemNum.insert(make_pair(strID, nNum));
		}
		break;
	case '3':
		strKey = strID;
		wstrContant  = CNpcInfo::GetInstance()->GetInfo(strID);
		dwRGB = CItemInfo::GetInstance()->GetRGBA(strID);
		break;
	case '5':
		strKey = strID;
		wstrContant  = CObjectInfo::GetInstance()->GetInfo(strID);
		dwRGB = CItemInfo::GetInstance()->GetRGBA(strID);
		break;
	case 'p':
		{
			string strID1 = GetStringHead(strID, "_");
			strID1.append(GetStringHead(strID, "_"));
			wstrContant = AtoW(strID);
			strID = strID1;
			strKey = strID.c_str() + 1;
			dwRGB = RGB_WHITE;
		}
		break;
	}
	if ( wstrContant.length() == 0 ) return;
	CButtonUI* pBtn = new CButtonUI;
	if ( pBtn )
	{
		wstring strUserData = L"List ";
		strUserData.append(AtoW(strID));
		m_pRoomListLayout->Add(pBtn);
		pBtn->SetUserData(strUserData.c_str());
		pBtn->SetFixedHeight(16);
		pBtn->SetFixedWidth(GetStringLengthW(wstrContant.c_str()));
		RECT rc = {3,0,0,0};
		pBtn->SetPadding(rc);
		pBtn->SetFont(0);
		pBtn->SetTextColor(dwRGB);
		pBtn->SetText(wstrContant.c_str());
		pBtn->SetAttribute(_T("align"), _T("left"));
		m_mapRoomList.insert(make_pair(strKey, pBtn));
		m_mapRoomList[strKey] = pBtn;
	}
}

void CMainFrameWnd::FixRoomList( string& strID, BOOL bAdd )
{
	string strBuffer = strID.c_str();
	string strNum;
	if ( strID.c_str()[0] == 2 )
	{
		strNum = strID;
		strID = GetStringHead(strNum, "_");
	}
	else if ( strID.c_str()[0] == 'p' )
	{
		string strID1 = GetStringHead(strID, "_");
		strID = GetStringHead(strID, "_");
	}
	CButtonUI* pBtn = m_mapRoomList[strID];
	if ( pBtn )
	{
		if ( strID.c_str()[0] == '2' )
		{
			int nNumChange = atoi(strNum.c_str());
			int nNumSource = m_mapItemNum[strNum];
			if ( nNumSource )
			{
				nNumSource = bAdd ? (nNumSource + nNumChange) : (nNumSource - nNumChange);
				if ( nNumSource < 1 )
				{
					m_pRoomListLayout->Remove(pBtn);
					m_mapRoomList.erase(m_mapRoomList.find(strID));
				}
				else
				{
					wstring wstrContant = CItemInfo::GetInstance()->GetInfo(strID, nNumSource);
					pBtn->SetText(wstrContant.c_str());
				}
			}
		}
		else if ( !bAdd )
		{
			m_pRoomListLayout->Remove(pBtn);
			m_mapRoomList.erase(m_mapRoomList.find(strID));
		}
	}
	else if ( bAdd )
	{
		AddRoomList(strBuffer);
	}
}

void CMainFrameWnd::CallbackStatus( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
#define MAX	140
#define RC_HP	{27,13,167,23}
#define RC_MP	{27,25,167,35}
	static CControlUI* pHpUp = s_pThis->m_PaintManager.FindControl(_T("HP_UP"));
	static CControlUI* pHpNow = s_pThis->m_PaintManager.FindControl(_T("HP_NOW"));
	static CControlUI* pHpText = s_pThis->m_PaintManager.FindControl(_T("HP_TEXT"));
	static CControlUI* pMpUp = s_pThis->m_PaintManager.FindControl(_T("MP_UP"));
	static CControlUI* pMpNow = s_pThis->m_PaintManager.FindControl(_T("MP_NOW"));
	static CControlUI* pMpText = s_pThis->m_PaintManager.FindControl(_T("MP_TEXT"));
	if ( pHpUp == NULL || pHpNow == NULL || pHpText == NULL || pMpUp == NULL || pMpNow == NULL || pMpText == NULL ) return;
	string strContant = pszContant;
	string strHPMax = GetStringHead(strContant, EVENT_SEPERATE);
	string strHPUp = GetStringHead(strContant, EVENT_SEPERATE);
	string strHPNow = GetStringHead(strContant, EVENT_SEPERATE);
	string strMPMax = GetStringHead(strContant, EVENT_SEPERATE);
	string strMPNow = GetStringHead(strContant, EVENT_SEPERATE);
	if ( strContant.length() == 0 )
	{
		int nHpMax = atoi(strHPMax.c_str());
		int nHpUp = atoi(strHPUp.c_str());
		int nHpNow = atoi(strHPNow.c_str());
		int nMpMax = atoi(strMPMax.c_str());
		int nMpNow = atoi(strMPNow.c_str());
		if ( nMpMax == 0 ) nMpMax = 1;

		RECT rcHpUp = RC_HP;
		RECT rcHpNow = RC_HP;
		RECT rcMpUp = RC_MP;
		RECT rcMpNow = RC_MP;
		rcHpUp.right = rcHpUp.left + nHpUp*MAX/nHpMax;
		rcHpNow.right = rcHpNow.left + nHpNow*MAX/nHpMax;
		rcMpUp.right = rcMpUp.left + (nMpNow > nMpMax ? (nMpNow-nMpMax)*MAX/nMpMax : 0);
		rcMpNow.right = rcMpNow.left + (nMpNow > nMpMax ? MAX : nMpNow*MAX/nMpMax);
		pHpUp->SetFloatPos(rcHpUp);
		pHpNow->SetFloatPos(rcHpNow);
		pMpUp->SetFloatPos(rcMpUp);
		pMpNow->SetFloatPos(rcMpNow);

		pHpText->SetText(AtoW(strHPNow + "/" + strHPUp).c_str());
		pMpText->SetText(AtoW(strMPNow + "/" + strMPMax).c_str());
	}
}

void CMainFrameWnd::CallbackFoodAndWater( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strWater = pszContant;
	string strMax = GetStringHead(strWater, EVENT_SEPERATE);
	string strFood = GetStringHead(strWater, EVENT_SEPERATE);
	s_pThis->FoodOrWater(TRUE, atoi(strFood.c_str()), atoi(strMax.c_str()));
	s_pThis->FoodOrWater(FALSE, atoi(strWater.c_str()), atoi(strMax.c_str()));
}

void CMainFrameWnd::FoodOrWater( BOOL bFood, int nNow, int nMax /*= 0*/ )
{
	if ( nNow == 0 && nMax == 0 ) return;
	static CProgressUI* pFood = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("FOOD")));
	static CProgressUI* pWater = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("WATER")));
	CProgressUI* pProgress = bFood ? pFood : pWater;
	if ( pProgress == NULL ) return;
	if ( nMax )
	{
		pProgress->SetMaxValue(nMax);
	}
	if ( nNow > -1 )
	{
		pProgress->SetValue(nNow);
	}
	WCHAR szText[20] = {0};
	wsprintf(szText, L"%d/%d", pProgress->GetValue(), pProgress->GetMaxValue());
	pProgress->SetText(szText);
}

void CMainFrameWnd::CallbackFirstInit( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	string strEvent = GetStringHead(strContant, EVENT_SEPERATE);
	if ( strEvent == "start" )
	{
		s_pThis->Info(RGB_YELLOW, L"这位壮士,请取个响亮的江湖名号吧！");
		s_pThis->Info(RGB_WHITE, L"(直接在聊天框输入名称并发送)");
		s_pThis->EditType(L"firstinit name ", L"命名", TRUE);
	}
	else if ( strEvent == "gift" )
	{
		s_pThis->Info(RGB_YELLOW, L"这位壮士,确认下你自己的能力！");
		s_pThis->Info(RGB_WHITE, L"(力量:影响伤害、招架率、食量上限)");
		s_pThis->Info(RGB_WHITE, L"(敏捷:影响命中率、闪避率)");
		s_pThis->Info(RGB_WHITE, L"(根骨:影响血量、内力、减伤率)");
		s_pThis->Info(RGB_WHITE, L"(悟性:影响武功等级、学习效率)");
		s_pThis->Info(RGB_YELLOW, L"(本次为内测阶段,以后可洗点)");
		s_pThis->Info(RGB_RED, L"(单项数值为[10-30],四项总和80)");
		s_pThis->Info(RGB_YELLOW, L"(例如:20 20 20 20)");
		s_pThis->EditType(L"firstinit gift ", L"天赋", TRUE);
	}
}
