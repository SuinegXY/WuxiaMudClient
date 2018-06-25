#include "stdafx.h"
#include "Define.h"
#include "LoginThirdWnd.h"
#include "LoginWnd.h"

#define WIDTH	250
#define HEIGHT	725
#define HOMEPAGE	_T("http://wuxia.suileyoo.com/api/account/authUrl")

CLoginThirdWnd* CLoginThirdWnd::s_pThis = NULL;

CLoginThirdWnd::CLoginThirdWnd(CLoginWnd* pLoginWnd)
{
	m_pLoginWnd = pLoginWnd;
	m_pChrome	= NULL;
	m_wstrUrl	= _T("");
	m_bChrome	= FALSE;
}

CLoginThirdWnd::~CLoginThirdWnd()
{
}

LPCTSTR CLoginThirdWnd::GetWindowClassName() const
{
	return _T("LoginThirdWnd");
}

DuiLib::CDuiString CLoginThirdWnd::GetSkinFile( void )
{
	return _T("LoginThirdWnd.xml");
}

LPCTSTR CLoginThirdWnd::GetClass() const
{
	return _T("CLoginThirdWnd");
}

void CLoginThirdWnd::InitWindow( void )
{
	s_pThis = this;
	CChromeUI::InitWebkit(true);
	m_pChrome = static_cast<CChromeUI*>(m_PaintManager.FindControl(_T("browser")));
}

void CLoginThirdWnd::Show(const WCHAR* pwszUrl)
{
	ShowWindow();
	CenterWindow();
	m_wstrUrl = pwszUrl;
	if ( m_bChrome )
	{
		NaviUrl(pwszUrl);
	}
}

LRESULT CLoginThirdWnd::OnCEFCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( m_wstrUrl.length() )
	{
		NaviUrl(m_wstrUrl.c_str());
	}
	if ( m_pChrome )
	{
		m_pChrome->SetJSCallBack(OnJSFunc);
		m_pChrome->SetCreateJSFunction(OnCreateJSFunc);
	}
	m_bChrome = TRUE;
	return 0;
}

void CLoginThirdWnd::NaviUrl( const WCHAR* pwszUrl )
{
	if ( m_pChrome && ::IsWindowVisible(m_hWnd) )
	{
		m_pChrome->NavigateUrl(pwszUrl);
	}
}

void CLoginThirdWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == L"click" )
	{
		if ( msg.pSender == m_PaintManager.FindControl(_T("btn_home")) )
		{
			NaviUrl(HOMEPAGE);
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_close")) )
		{
			ShowWindow(false);
			m_pLoginWnd->Show();
		}
	}
}

bool CLoginThirdWnd::OnJSFunc(const string& strFuncName, vector<VARIANT>& argv, VARIANT* pRetValue)
{
	if ( strFuncName == "receiveSuccess" )
	{
		do 
		{
			if ( argv.size() < 1 ) break;
			if (argv[0].vt != VT_BSTR) break;
			int nLen = WideCharToMultiByte(CP_ACP, 0, argv[0].bstrVal, _tcslen(argv[0].bstrVal)+1, NULL, 0, NULL, NULL);
			if ( nLen <= 0 ) break;
			char* pszJson = (char*)malloc(nLen);
			if ( pszJson == NULL ) break;
			WideCharToMultiByte(CP_ACP, 0, argv[0].bstrVal, _tcslen(argv[0].bstrVal)+1, pszJson, nLen, NULL, NULL);

			s_pThis->m_pLoginWnd->Login(pszJson);
			s_pThis->ShowWindow(false);

			free(pszJson);
		} while (FALSE);
	}
	else if ( strFuncName == "receiveError" )
	{
		do 
		{
			if ( argv.size() < 1 ) break;
			if (argv[0].vt != VT_BSTR) break;
			int nLen = WideCharToMultiByte(CP_ACP, 0, argv[0].bstrVal, _tcslen(argv[0].bstrVal)+1, NULL, 0, NULL, NULL);
			if ( nLen <= 0 ) break;
			char* pszJson = (char*)malloc(nLen);
			if ( pszJson == NULL ) break;
			WideCharToMultiByte(CP_ACP, 0, argv[0].bstrVal, _tcslen(argv[0].bstrVal)+1, pszJson, nLen, NULL, NULL);

			s_pThis->m_pLoginWnd->SetError(L"µÚÈý·½µÇÂ¼Ê§°Ü");
			s_pThis->ShowWindow(false);

			free(pszJson);
		} while (FALSE);
	}
	return false;
}

void CLoginThirdWnd::OnCreateJSFunc(OLECHAR*** pFuncName, int& nLen)
{
	nLen = 2;
	(*pFuncName) = (OLECHAR**)HeapAlloc(GetProcessHeap(),0,sizeof(OLECHAR*)*nLen);
	(*pFuncName)[0] = (OLECHAR*)HeapAlloc(GetProcessHeap(), 0, sizeof(OLECHAR) * (32 + 1));
	_tcscpy((*pFuncName)[0], _T("receiveSuccess"));
	(*pFuncName)[1] = (OLECHAR*)HeapAlloc(GetProcessHeap(), 0, sizeof(OLECHAR) * (32 + 1));
	_tcscpy((*pFuncName)[1], _T("receiveError"));
}

LRESULT CLoginThirdWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if ( uMsg == WM_CEF_CREATE )
	{
		OnCEFCreate(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}
