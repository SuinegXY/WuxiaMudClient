#include "stdafx.h"
#include "RoleInfoWnd.h"
#include "Define.h"
#include "GameInfo\Npc.h"
#include "GameInfo\User.h"
#include "Object\Parser.h"
#include "Object\Router.h"
#include "Util\ATW.h"

#define INIT_HEIGHT		79
#define OBJECT_HEIGHT	40
#define ADD_HEIGHT		23

CRoleInfoWnd* CRoleInfoWnd::s_pThis = NULL;

LPCTSTR CRoleInfoWnd::GetWindowClassName() const
{
	return _T("RoleInfoFrame");
}

CDuiString CRoleInfoWnd::GetSkinFile( void )
{
	return _T("RoleInfo.xml");
}

LPCTSTR CRoleInfoWnd::GetClass() const
{
	return _T("CRoleInfoFrame");
}

CRoleInfoWnd::CRoleInfoWnd()
{
	m_nBaseCX = 200;
	m_bSelf = TRUE;
	m_pAddLayout = NULL;
	m_pEditName = NULL;
	m_pEditID = NULL;
	m_pEditBtn = NULL;
}

void CRoleInfoWnd::InitWindow( void )
{
	s_pThis = this;

	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	MoveWindow(m_hWnd, rcDesktop.right-VALUE_MAIN_FRAME_CX-m_nBaseCX-200, VALUE_HEIGHT_DIST, m_nBaseCX, m_nBaseCY, TRUE);

	m_pGamesLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("LayoutGames")));
	m_pAddLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("LayoutAdd")));

	CEventParser::GetInstance()->EventReg("playerGame", CallbackPlayer);
}

void CRoleInfoWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == _T("click") )
	{
		if( msg.pSender->GetName() == _T("closebtn") )
		{
			ShowWindow(false);
		}
		else
		{
			wstring wstrUserName = msg.pSender->GetUserData();
			switch( wstrUserName.c_str()[0]  )
			{
			case L'A':
				Add();
				break;
			case L'N':
				New();
				break;
			case L'D':
				Del(wstrUserName.c_str()+1);
				break;
			case L'C':
				Copy(wstrUserName.c_str()+1);
				break;
			}
		}
	}
}

void CRoleInfoWnd::Add()
{
	m_pNew = new CVerticalLayoutUI;
	if ( m_pNew )
	{
		s_pThis->m_pGamesLayout->Add(m_pNew);
		m_pNew->SetFixedHeight(OBJECT_HEIGHT);
		m_pNew->SetFixedWidth(200);
		s_pThis->m_nHeight += OBJECT_HEIGHT;

		CHorizontalLayoutUI* pUp = new CHorizontalLayoutUI;
		if ( pUp )
		{
			m_pNew->Add(pUp);
			pUp->SetFixedHeight(OBJECT_HEIGHT/2);

			m_pEditName = new CEditUI;
			if ( m_pEditName )
			{
				pUp->Add(m_pEditName);
				m_pEditName->SetFixedHeight(OBJECT_HEIGHT/2);
				m_pEditName->SetFixedWidth(90);
				m_pEditName->SetFont(0);
				m_pEditName->SetAttribute(L"align", L"left");
				m_pEditName->SetTextColor(0xFFffffff);
				m_pEditName->SetDisabledTextColor(0xFFffffff);
				m_pEditName->SetBkColor(0xff0b055b);
				m_pEditName->SetNativeEditBkColor(0xff0b055b);
				m_pEditName->SetTextBackgroundColor(0xff0b055b);
				m_pEditName->SetTextBackground(L"游戏名");
				m_pEditName->SetTextBackgroundColor(0xffdddddd);
				RECT rc = {5,0,0,0};
				m_pEditName->SetPadding(rc);
				RECT rcText = {2,1,1,1};
				m_pEditName->SetTextPadding(rcText);
			}

			m_pEditID = new CEditUI;
			if ( m_pEditID )
			{
				pUp->Add(m_pEditID);
				m_pEditID->SetFixedHeight(OBJECT_HEIGHT/2);
				m_pEditID->SetFixedWidth(90);
				m_pEditID->SetFont(0);
				m_pEditID->SetAttribute(L"align", L"right");
				m_pEditID->SetTextColor(0xFFffffff);
				m_pEditID->SetDisabledTextColor(0xFFffffff);
				m_pEditID->SetBkColor(0xff0b055b);
				m_pEditID->SetNativeEditBkColor(0xff0b055b);
				m_pEditID->SetTextBackgroundColor(0xff0b055b);
				m_pEditID->SetTextBackground(L"游戏ID");
				m_pEditID->SetTextBackgroundColor(0xffdddddd);
				RECT rc = {5,0,0,0};
				m_pEditID->SetPadding(rc);
				RECT rcText = {2,1,1,1};
				m_pEditID->SetTextPadding(rcText);
			}
		}

		m_pEditBtn = new CButtonUI;
		if ( m_pEditBtn )
		{
			m_pNew->Add(m_pEditBtn);
			m_pEditBtn->SetFixedHeight(OBJECT_HEIGHT/2);
			m_pEditBtn->SetFixedWidth(50);
			m_pEditBtn->SetBkColor(0xff3D54CB);
			m_pEditBtn->SetFont(0);
			m_pEditBtn->SetAttribute(L"align", L"center");
			m_pEditBtn->SetTextColor(0xFFffffff);
			RECT rc = {140,0,0,0};
			m_pEditBtn->SetPadding(rc);
			m_pEditBtn->SetText(L"上传");
			m_pEditBtn->SetUserData(L"N");
		}
	}
	ControlAction(s_pThis->m_pAddLayout, SetVisible, false);
	m_nHeight -= ADD_HEIGHT;

	Show();
}

void CRoleInfoWnd::New()
{
	if ( m_pAddLayout == NULL || m_pEditName == NULL || m_pEditID == NULL || m_pEditBtn == NULL ) return;
	wstring wstrGameName = m_pEditName->GetText();
	wstring wstrGameID = m_pEditID->GetText();
	if ( wstrGameName.length() == 0 || wstrGameID.length() == 0 )
	{
		MessageBox(m_hWnd, L"请输入游戏名及游戏内解决名", NULL, NULL);
		return;
	}
	m_pEditName->SetEnabled(false);
	m_pEditName->SetBkColor(0xFF000000);
	m_pEditID->SetEnabled(false);
	m_pEditID->SetBkColor(0xFF000000);
	m_pEditBtn->SetText(L"删除");
	wstring wstrUserData = L"D";
	wstrUserData.append(wstrGameName);
	m_pEditBtn->SetUserData(wstrUserData.c_str());
	m_mapGames.insert(make_pair(WtoA(wstrGameName).c_str(), m_pAddLayout));
	m_pAddLayout->SetVisible(true);
	m_nHeight += ADD_HEIGHT;

	string strSend = "playerGame add ";
	strSend.append(WtoA(wstrGameName));
	strSend.append(" ");
	strSend.append(WtoA(wstrGameID));
	CRouter::GetInstance()->Send(strSend.c_str());

	m_pNew = NULL;

	Show();
}

void CRoleInfoWnd::Copy( const WCHAR* pszGameID )
{
	string strData = WtoA(pszGameID);
	if( ::OpenClipboard(NULL) )
	{
		::EmptyClipboard();
		HGLOBAL hClipBuffer;
		char* pszBuffer;
		hClipBuffer = ::GlobalAlloc(GMEM_DDESHARE, strData.length()+1);
		pszBuffer = (char*) ::GlobalLock(hClipBuffer);
		strcpy(pszBuffer, strData.c_str());
		::GlobalUnlock(hClipBuffer);
		::SetClipboardData(CF_TEXT, hClipBuffer);
		::CloseClipboard();
	}
}

void CRoleInfoWnd::Del( const WCHAR* pszGameName )
{
	string strGameName = WtoA(pszGameName);
	CVerticalLayoutUI* pLayout = m_mapGames[strGameName];
	if ( pLayout )
	{
		m_pGamesLayout->Remove(pLayout);
		m_nHeight -= OBJECT_HEIGHT;
		m_mapGames.erase(m_mapGames.find(strGameName));

		string strSend = "playerGame dec ";
		strSend.append(strGameName);
		CRouter::GetInstance()->Send(strSend.c_str());
	}
	Show();
}

void CRoleInfoWnd::Show()
{
	RECT rcDesktop;
	GetWindowRect(m_hWnd, &rcDesktop);
	MoveWindow(m_hWnd, rcDesktop.left, rcDesktop.top, m_nBaseCX, m_nHeight, TRUE);
	ShowWindow();
}

void CRoleInfoWnd::CallbackPlayer( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	s_pThis->m_nHeight = INIT_HEIGHT;
	s_pThis->m_mapGames.clear();
	s_pThis->m_pGamesLayout->RemoveAll();
	string strContant = pszContant;
	string strID = GetStringHead(strContant, EVENT_SEPERATE);
	s_pThis->m_bSelf = (strcmp(strID.c_str(), CUser::GetInstance()->GetUser().c_str()) == 0);

	string strName = GetStringHead(strContant, EVENT_SEPERATE);
	CControlUI* pName = s_pThis->m_PaintManager.FindControl(_T("Name_Suf"));
	ControlAction(pName, SetText, AtoW(strName).c_str());

	string strExp = GetStringHead(strContant, EVENT_SEPERATE);
	pName = s_pThis->m_PaintManager.FindControl(_T("Exp"));
	ControlAction(pName, SetText, CNpcInfo::GetInstance()->GetExpDescribe(strExp).c_str());

	while ( strContant.length() )
	{
		string strGameName	= GetStringHead(strContant, EVENT_SEPERATE);
		string strGameID	= GetStringHead(strContant, EVENT_SEPERATE);

		CVerticalLayoutUI* pLayout = new CVerticalLayoutUI;
		if ( pLayout )
		{
			s_pThis->m_pGamesLayout->Add(pLayout);
			pLayout->SetFixedHeight(OBJECT_HEIGHT);
			pLayout->SetFixedWidth(200);
			s_pThis->m_mapGames.insert(make_pair(strGameName, pLayout));
			s_pThis->m_nHeight += OBJECT_HEIGHT;

			CHorizontalLayoutUI* pUp = new CHorizontalLayoutUI;
			if ( pUp )
			{
				pLayout->Add(pUp);
				pUp->SetFixedHeight(OBJECT_HEIGHT/2);

				CLabelUI* pGameName = new CLabelUI;
				if ( pGameName )
				{
					pUp->Add(pGameName);
					pGameName->SetFixedHeight(OBJECT_HEIGHT/2);
					pGameName->SetFixedWidth(90);
					pGameName->SetFont(0);
					pGameName->SetText(AtoW(strGameName).c_str());
					pGameName->SetAttribute(L"align", L"left");
					pGameName->SetTextColor(0xFFffffff);
					pGameName->SetDisabledTextColor(0xFFffffff);
				}

				CLabelUI* pGameID = new CLabelUI;
				if ( pGameID )
				{
					pUp->Add(pGameID);
					pGameID->SetFixedHeight(OBJECT_HEIGHT/2);
					pGameID->SetFixedWidth(90);
					pGameID->SetFont(0);
					pGameID->SetText(AtoW(strGameID).c_str());
					pGameID->SetAttribute(L"align", L"right");
					pGameID->SetTextColor(0xFFffffff);
					pGameID->SetDisabledTextColor(0xFFffffff);
					pGameID->SetDisabledTextColor(0xFFffffff);
				}
			}

			CButtonUI* pBtn = new CButtonUI;
			if ( pBtn )
			{
				pLayout->Add(pBtn);
				pBtn->SetFixedHeight(OBJECT_HEIGHT/2);
				pBtn->SetFixedWidth(50);
				pBtn->SetBkColor(0xff3D54CB);
				pBtn->SetFont(0);
				pBtn->SetAttribute(L"align", L"center");
				pBtn->SetTextColor(0xFFffffff);
				RECT rc = {140,0,0,0};
				pBtn->SetPadding(rc);
				wstring wstrUserData;
				if ( s_pThis->m_bSelf )
				{
					pBtn->SetText(L"删除");
					wstrUserData = L"D";
					wstrUserData.append(AtoW(strGameName));
				}
				else
				{
					pBtn->SetText(L"复制");
					wstrUserData = L"C";
					wstrUserData.append(AtoW(strGameID));
				}
				pBtn->SetUserData(wstrUserData.c_str());
			}
		}
	}

	if ( s_pThis->m_bSelf )
	{
		s_pThis->m_nHeight += ADD_HEIGHT;
	}
	ControlAction(s_pThis->m_pAddLayout, SetVisible, s_pThis->m_bSelf);

	s_pThis->Show();
}

