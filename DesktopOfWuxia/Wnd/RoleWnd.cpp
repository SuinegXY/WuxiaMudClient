#include "stdafx.h"
#include "RoleWnd.h"
#include "Define.h"
#include "MainFrame.h"
#include "Object\Parser.h"
#include "Util\ATW.h"
#include "GameInfo\Item.h"
#include "GameInfo\Skill.h"
#include "PopupWnd.h"

CRoleWnd* CRoleWnd::s_pThis = NULL;

LPCTSTR CRoleWnd::GetWindowClassName() const
{
	return _T("RoleFrame");
}

CDuiString CRoleWnd::GetSkinFile( void )
{
	return _T("Role.xml");
}

LPCTSTR CRoleWnd::GetClass() const
{
	return _T("CRoleFrame");
}

CRoleWnd::CRoleWnd()
{
	m_nBaseCX = 200;
	m_nBaseCY = VALUE_MAIN_FRAME_CY+VALUE_LIST_WND_CY;
	m_nLevel = 0;
	ZeroMemory(m_nGift, sizeof(m_nGift));
	ZeroMemory(m_nGiftSkill, sizeof(m_nGiftSkill));
}

void CRoleWnd::InitWindow( void )
{
	s_pThis = this;
	
	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	MoveWindow(m_hWnd, rcDesktop.right-VALUE_MAIN_FRAME_CX-m_nBaseCX, VALUE_HEIGHT_DIST, m_nBaseCX, m_nBaseCY, TRUE);

	m_pItemsLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("LayoutItems")));

	CEventParser::GetInstance()->EventReg("name", CallbackName);
	CEventParser::GetInstance()->EventReg("gift", CallbackGift);
	CEventParser::GetInstance()->EventReg("eandp", CallbackExpAndPot);
	CEventParser::GetInstance()->EventReg("money", CallbackMoney);
	CEventParser::GetInstance()->EventReg("items", CallbackItems);
	CEventParser::GetInstance()->EventReg("skills", CallbackSkills);
}

void CRoleWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == _T("click") )
	{
		if ( msg.pSender->GetUserData().Find(L"Items") == 0  )
		{
			wstring wstrUserData = msg.pSender->GetUserData();
			POINT pt;
			GetCursorPos(&pt);
			CPopupWnd::GetInstance()->PopupCreate(pt.x, pt.y, WtoA(wstrUserData).c_str()+5, FALSE);
		}
	}
}

void CRoleWnd::CallbackName( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	CControlUI* pName = s_pThis->m_PaintManager.FindControl(_T("Name_Suf"));
	ControlAction(pName, SetText, AtoW(strContant).c_str());
}

void CRoleWnd::CallbackGift( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	string strLevel = GetStringHead(strContant, EVENT_SEPERATE);
	string strLevelUp = GetStringHead(strContant, EVENT_SEPERATE);
	strLevel.append(" / ");
	strLevel.append(strLevelUp);
	CControlUI* pLevel = s_pThis->m_PaintManager.FindControl(_T("Level"));
	ControlAction(pLevel, SetText, AtoW(strLevel).c_str());
	s_pThis->m_nLevel = atoi(strLevel.c_str());
	for ( int i = 0; i < 4; ++i )
	{
		string strGift = GetStringHead(strContant, EVENT_SEPERATE);
		s_pThis->m_nGift[i] = atoi(strGift.c_str());
		WCHAR wszGift[10] = {0};
		wsprintf(wszGift, L"Gift_%d", i);
		CControlUI* pGift = s_pThis->m_PaintManager.FindControl(wszGift);
		ControlAction(pGift, SetText, AtoW(strGift).c_str());
	}
}

void CRoleWnd::CallbackExpAndPot( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strPot = pszContant;
	string strExp = GetStringHead(strPot, EVENT_SEPERATE);
	static CControlUI* pExp = s_pThis->m_PaintManager.FindControl(L"Exp");
	static CControlUI* pPot = s_pThis->m_PaintManager.FindControl(L"Pot");
	ControlAction(pExp, SetText, AtoW(strExp).c_str());
	ControlAction(pPot, SetText, AtoW(strPot).c_str());
}

void CRoleWnd::CallbackMoney( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	static CControlUI* pMoney = s_pThis->m_PaintManager.FindControl(L"Money");
	ULONG lMoney = 0;
	sscanf(pszContant, "%d", &lMoney);
	wstring wstrMoney = L"";
	do 
	{
		CItemInfo* pInfo = CItemInfo::GetInstance();
		int nCash = lMoney / 1000000;
		if ( nCash )
		{
			wstrMoney = wstring(L"存款：") + pInfo->GetInfo(string("20003"), nCash);
			break;
		}
		int nGold = lMoney / 10000;
		if ( nGold )
		{
			wstrMoney = wstring(L"存款：") + pInfo->GetInfo(string("20004"), nGold);
			break;
		}
		int nSilver = lMoney /100;
		if ( nSilver )
		{
			wstrMoney = wstring(L"存款：") + pInfo->GetInfo(string("20005"), nSilver);
			break;
		}
		int nCoin = lMoney;
		if ( nCoin )
		{
			wstrMoney = wstring(L"存款：") + pInfo->GetInfo(string("20006"), nCoin);
			break;
		}
		wstrMoney = wstring(L"存款：无");
	} while (FALSE);
	ControlAction(pMoney, SetText, wstrMoney.c_str());
}

void CRoleWnd::CallbackItems( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	string strType = GetStringHead(strContant, EVENT_SEPERATE);

	if ( strcmp(strType.c_str(), "add") == 0 )
	{
		s_pThis->FixItem(strContant, TRUE);
	}
	else if ( strcmp(strType.c_str(), "dec") == 0 )
	{
		s_pThis->FixItem(strContant, FALSE);
	}
	else if ( strcmp(strType.c_str(), "all") == 0 )
	{
		s_pThis->SetItems(strContant);
	}
}

void CRoleWnd::SetItems( string& strItemsList )
{
	if ( m_pItemsLayout == NULL ) return;
	m_pItemsLayout->RemoveAll();
	m_mapItems.clear();
	m_mapItemNum.clear();

	while( strItemsList.length() )
	{
		string strID = GetStringHead(strItemsList, EVENT_SEPERATE);
		AddItem(strID);
	}
}

void CRoleWnd::AddItem( string& strID )
{
	if ( m_pItemsLayout == NULL ) return;
	if ( strID.length() == 0 ) return;
	if ( strID.c_str()[0] != 2 ) return;

	string strNum = strID;
	strID = GetStringHead(strNum, "_");
	int nNum = atoi(strNum.c_str());
	wstring wstrContant = CItemInfo::GetInstance()->GetInfo(strID, nNum);

	if ( wstrContant.length() == 0 ) return;
	CButtonUI* pBtn = new CButtonUI;
	if ( pBtn )
	{
		wstring strUserData = L"Items ";
		strUserData.append(AtoW(strID));
		m_pItemsLayout->Add(pBtn);
		pBtn->SetUserData(strUserData.c_str());
		pBtn->SetFixedHeight(16);
		pBtn->SetFixedWidth(GetStringLengthW(wstrContant.c_str()));
		RECT rc = {3,0,0,0};
		pBtn->SetPadding(rc);
		pBtn->SetFont(0);
		pBtn->SetTextColor(CItemInfo::GetInstance()->GetRGBA(strID));
		pBtn->SetText(wstrContant.c_str());
		pBtn->SetAttribute(_T("align"), _T("left"));
		m_mapItems.insert(make_pair(strID, pBtn));
		m_mapItemNum.insert(make_pair(strID, nNum));
	}
}

void CRoleWnd::FixItem( string& strID, BOOL bAdd )
{
	if ( m_pItemsLayout == NULL ) return;
	if ( strID.length() == 0 ) return;
	if ( strID.c_str()[0] != 2 ) return;

	string strBuffer = strID;
	string strNum = strID;
	strID = GetStringHead(strNum, "_");

	CButtonUI* pBtn = m_mapItems[strID];
	if ( pBtn )
	{
		int nNumChange = atoi(strNum.c_str());
		int nNumSource = m_mapItemNum[strNum];
		if ( nNumSource )
		{
			nNumSource = bAdd ? (nNumSource + nNumChange) : (nNumSource - nNumChange);
			if ( nNumSource < 1 )
			{
				m_pItemsLayout->Remove(pBtn);
				m_mapItems.erase(m_mapItems.find(strID));
			}
			else
			{
				wstring wstrContant = CItemInfo::GetInstance()->GetInfo(strID, nNumSource);
				pBtn->SetText(wstrContant.c_str());
			}
		}
	}
	else if ( bAdd )
	{
		AddItem(strBuffer);
	}
}

void CRoleWnd::CallbackSkills( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;

	while( strContant.length() )
	{
		string strSkill = GetStringHead(strContant, EVENT_SEPERATE);
		s_pThis->SetSkill(strSkill);
	}
}

void CRoleWnd::SetSkill( string& strSkill )
{
	static CVerticalLayoutUI* pLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("LayoutSkills")));
	if ( pLayout == NULL ) return;

	string strID = GetStringHead(strSkill, "_");
	string strLevel = GetStringHead(strSkill, "_");

	CLabelUI* pLevel = m_mapSkillsLevel[strID];
	CLabelUI* pExp = m_mapSkillsExp[strID];
	if ( pLevel == NULL && pExp == NULL )
	{
		CHorizontalLayoutUI* pHor = new CHorizontalLayoutUI;
		if ( pHor == NULL ) return;
		pLayout->Add(pHor);
		pHor->SetFixedHeight(20);
		RECT rc = {3,0,0,0};
		pHor->SetPadding(rc);
		CLabelUI* pName = new CLabelUI;
		if ( pName )
		{
			pHor->Add(pName);
			pName->SetFixedHeight(20);
			pName->SetFixedWidth(80);
			pName->SetFont(0);
			pName->SetTextColor(0xffeeeeee);
			pName->SetText(CSkillInfo::GetInstance()->GetInfo(strID).c_str());
			pName->SetAttribute(_T("align"), _T("center"));
		}
		pLevel = new CLabelUI;
		if ( pLevel )
		{
			pHor->Add(pLevel);
			pLevel->SetFixedHeight(20);
			pLevel->SetFixedWidth(50);
			pLevel->SetFont(0);
			pLevel->SetTextColor(0xffeeeeee);
			pLevel->SetAttribute(_T("align"), _T("right"));
			m_mapSkillsLevel.insert(make_pair(strID, pLevel));
			m_mapSkillsLevel[strID] = pLevel;
		}
		pExp = new CLabelUI;
		if ( pExp )
		{
			pHor->Add(pExp);
			pExp->SetFixedHeight(20);
			pExp->SetFixedWidth(60);
			pExp->SetFont(0);
			pExp->SetTextColor(0xffeeeeee);
			pExp->SetAttribute(_T("align"), _T("right"));
			m_mapSkillsExp.insert(make_pair(strID, pExp));
			m_mapSkillsExp[strID] = pExp;
		}
	}
	if ( pLevel )
	{
		pLevel->SetText(AtoW(strLevel).c_str());
	}
	if ( pExp )
	{
		pExp->SetText(AtoW(strSkill).c_str());
	}
}
