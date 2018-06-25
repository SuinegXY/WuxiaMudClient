#include "stdafx.h"
#include "Define.h"
#include "LoginWnd.h"
#include "LoginThirdWnd.h"
#include "statistic/ConfigManager.h"
#include "statistic/ClientStatistic.h"
#include "Object/Interface.h"
#include "GameInfo/User.h"
#include "json/JsonHelper.h"
#include "Util/ATW.h"
#include "Util/HttpClient.h"
#include "Util/MD5.h"

#define	GET_IDENT_CODE	"http://wuxia.suileyoo.com/api/account/sendCodeLogin"
#define	GET_LOGIN_IDENT	"http://wuxia.suileyoo.com/api/account/quickReg"
#define	GET_LOGIN_PASS	"http://wuxia.suileyoo.com/api/account/ucLogin"

DUI_BEGIN_MESSAGE_MAP(CLoginWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RETURN, OnReturn)
DUI_END_MESSAGE_MAP()

CLoginWnd::CLoginWnd()
{
	m_pUsername		= NULL;
	m_pGetIdentCode	= NULL;
	m_pLogin		= NULL;
	m_pThirdWnd		= NULL;
	m_nLoginType	= 0;
	m_nGetIdentCodeTime = 60;
}

CLoginWnd::~CLoginWnd()
{
	CChromeUI::ShutdownWebkit();
}

LPCTSTR CLoginWnd::GetWindowClassName() const
{
	return _T("LoginWnd");
}

DuiLib::CDuiString CLoginWnd::GetSkinFile( void )
{
	return _T("LoginWnd.xml");
}

LPCTSTR CLoginWnd::GetClass() const
{
	return _T("CLoginWnd");
}

void CLoginWnd::InitWindow( void )
{
	m_pUsername		= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_username")));
	m_pGetIdentCode = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_identcode")));
	m_pLogin		= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_login")));

	m_pThirdWnd = new CLoginThirdWnd(this);
	if ( m_pThirdWnd )
	{
		m_pThirdWnd->Create(m_hWnd, NULL, WS_POPUP, 0, 0, 0, 137, 203);
		SetWindowLong(m_pThirdWnd->GetHWND(), GWL_EXSTYLE, GetWindowLong(m_pThirdWnd->GetHWND(), GWL_EXSTYLE) & (~WS_EX_APPWINDOW));
	}
}

void CLoginWnd::Show(int nType)
{
	ShowWindow();

	m_nType = nType;
	SetError(NULL);
	ControlAction(m_pUsername, SetMaxChar, nType == 0 ? 255 : 11);
	static CControlUI* pLayoutIdentcode1= m_PaintManager.FindControl(_T("layout_identcode1"));
	static CControlUI* pLayoutIdentcode2= m_PaintManager.FindControl(_T("layout_identcode2"));
	static CControlUI* pLayoutIdentcode3= m_PaintManager.FindControl(_T("layout_identcode3"));
	static CControlUI* pLayoutDown		= m_PaintManager.FindControl(_T("layout_down"));
	static CControlUI* pEditPassword	= m_PaintManager.FindControl(_T("edit_password"));
	static CControlUI* pHome			= m_PaintManager.FindControl(_T("btn_home"));
	ControlAction(pLayoutIdentcode1, SetVisible, nType != 0);
	ControlAction(pLayoutIdentcode2, SetVisible, nType == 0);
	ControlAction(pLayoutIdentcode3, SetVisible, nType == 0);
	ControlAction(pHome, SetVisible, nType != 0);
	ControlAction(pLayoutDown, SetVisible, nType == 0);
	ControlAction(pEditPassword, SetVisible, nType == 0);

	CControlUI* pUsername = m_PaintManager.FindControl(_T("edit_username"));
	CControlUI* pPassword = m_PaintManager.FindControl(_T("edit_password"));
	ControlAction(pUsername, SetText, _T(""));
	ControlAction(pPassword, SetText, _T(""));
	ControlAction(m_pGetIdentCode, SetText, _T("获取验证码"));

	ControlAction(m_pLogin, SetText, nType != 2 ? _T("登录") : _T("注册"));
}

void CLoginWnd::OnClick( TNotifyUI& msg )
{
	if ( msg.sType == L"click" )
	{
		if ( msg.pSender == m_PaintManager.FindControl(_T("btn_home")) )
		{
			Show(0);
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_close")) )
		{
			ShowWindow(false);
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_loginident")) )
		{
			Show(1);
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_regedit")) )
		{
			Show(2);
		}
		else if ( msg.pSender == m_pGetIdentCode )
		{
			OnGetIdentCode();
		}
		else if ( msg.pSender == m_pLogin )
		{
			OnLogin();
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_forgetpassword")) )
		{
			::ShellExecuteA(NULL, "open", "explorer.exe", "https://register.stnts.com/new/v2/findpwd/step1.do", NULL, SW_SHOW);
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_QQ")) )
		{
			ControlAction(m_pThirdWnd, Show, L"http://simulator.suileyoo.com/api/auth/url?platform=qq");
			ShowWindow(false);
			m_nLoginType = EUA_LOGIN_QQ;
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_WX")) )
		{
			ControlAction(m_pThirdWnd, Show, L"http://simulator.suileyoo.com/api/auth/url?platform=wechat");
			ShowWindow(false);
			m_nLoginType = EUA_LOGIN_WX;
		}
		else if ( msg.pSender == m_PaintManager.FindControl(_T("btn_SN")) )
		{
			ControlAction(m_pThirdWnd, Show, L"http://simulator.suileyoo.com/api/auth/url?platform=weibo");
			ShowWindow(false);
			m_nLoginType = EUA_LOGIN_WB;
		}
	}
}

LRESULT CLoginWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if ( uMsg == WM_TIMER )
	{
		if ( wParam == WM_TIMER + 1 )
		{
			-- m_nGetIdentCodeTime;
			GetIdentCodeShow();
		}
	}
	return 0;
}

void CLoginWnd::SetError( const TCHAR* ptszError )
{
	static CControlUI* pIcon = m_PaintManager.FindControl(_T("warming_icon"));
	static CControlUI* pText = m_PaintManager.FindControl(_T("warming_text"));
	ControlAction(pIcon, SetVisible, ptszError != NULL);
	ControlAction(pText, SetVisible, ptszError != NULL);
	if ( ptszError && pText )
	{
		pText->SetText(ptszError);
	}
}

void CLoginWnd::OnGetIdentCode()
{
	m_pGetIdentCode->SetEnabled(false);
	CloseHandle(CreateThread(NULL, NULL, OnGetIdentCodeThread, this, NULL, NULL));
}

DWORD WINAPI CLoginWnd::OnGetIdentCodeThread( LPVOID lp )
{
	CLoginWnd* p = (CLoginWnd*)lp;
	if ( p->m_pUsername == NULL ) return -1;

	BOOL bRes = TRUE;
	do 
	{
		string strPhoneNumber = WtoA(p->m_pUsername->GetText().GetData());
		if ( strPhoneNumber.length() != 11 )
		{
			p->SetError(L"请输入正确的电话号码");
			bRes = FALSE;
			break;
		}
		for ( UINT i = 0; i < strPhoneNumber.length(); ++i )
		{
			if ( strPhoneNumber.c_str()[i] > '9' || strPhoneNumber.c_str()[i] < '0' )
			{
				bRes = FALSE;
				break;
			}
		}
		if (!bRes) break;
		string strCallback;
		HttpClient http;
		if (http.Post("www.baidu.com", "", NULL, strCallback) != CURLE_OK)
		if ( strcmp(SuinegJsonHelper::GetValueString(strCallback, "returnCode").c_str(), "200" ) != 0 ) 
		{
			string strErrorInfo = SuinegJsonHelper::GetValueString(strCallback, "returnMessage");
			if ( strErrorInfo.length() == 0 ) strErrorInfo = "获取验证码失败";
			else strErrorInfo = UTF8toA(strErrorInfo.c_str());
			p->SetError(AtoW(strErrorInfo.c_str()).c_str());
			bRes = FALSE;
			break;
		}
		else
		{
			p->SetError(NULL);
		}

		p->GetIdentCodeTimeLimit();
	} while (FALSE);

	if ( !bRes )
	{
		p->m_pGetIdentCode->SetEnabled(true);
	}

	return 0;
}

void CLoginWnd::GetIdentCodeTimeLimit()
{
	m_pGetIdentCode->SetEnabled(false);
	m_nGetIdentCodeTime = 60;
	GetIdentCodeShow();
	SetTimer(m_hWnd, WM_TIMER+1, 1000, NULL);
}

void CLoginWnd::GetIdentCodeShow()
{
	if ( m_nGetIdentCodeTime == 0 )
	{
		m_pGetIdentCode->SetEnabled(true);
		m_pGetIdentCode->SetText(_T("获取验证码"));
		KillTimer(m_hWnd, WM_TIMER+1);
	}
	else
	{
		char tszTimer[5] = {0};
		sprintf(tszTimer, "%d", m_nGetIdentCodeTime);
		m_pGetIdentCode->SetText(AtoW(tszTimer).c_str());
	}
}

void CLoginWnd::OnLogin()
{
	m_pLogin->SetEnabled(false);
	if ( m_nType == 0 )
	{
		CloseHandle(CreateThread(NULL, NULL, OnLoginUserPassThread, this, NULL, NULL));
	}
	else if ( m_nType == 1 || m_nType == 2 )
	{
		CloseHandle(CreateThread(NULL, NULL, OnLoginUserIdentThread, this, NULL, NULL));
	}
}

DWORD WINAPI CLoginWnd::OnLoginUserPassThread( LPVOID lp )
{
	CLoginWnd* p = (CLoginWnd*)lp;
	CControlUI* pUsername = p->m_PaintManager.FindControl(_T("edit_username"));
	CControlUI* pPassword = p->m_PaintManager.FindControl(_T("edit_password"));
	if ( pUsername == NULL || pPassword == NULL ) return -1;

	BOOL bRes = TRUE;
	do 
	{
		string strUsername = WtoA(pUsername->GetText());
		string strPassword = WtoA(pPassword->GetText());
		if ( strUsername.length() == 0 || strPassword.length() == 0 )
		{
			p->SetError(L"请输入正确的账号密码");
			bRes = FALSE;
			break;
		}

		string strCallback;
		HttpClient http;
		if ( http.Post("www.baidu.com", "", NULL, strCallback) != CURLE_OK )
		{
			strCallback = "";
		}

		if ( strcmp(SuinegJsonHelper::GetValueString(strCallback, "returnCode").c_str(), "200" ) != 0 ) 
		{
			string strErrorInfo = SuinegJsonHelper::GetValueString(strCallback, "returnMessage");
			if ( strErrorInfo.length() == 0 ) strErrorInfo = "登录失败";
			else strErrorInfo = UTF8toA(strErrorInfo.c_str());
			p->SetError(AtoW(strErrorInfo.c_str()).c_str());
			bRes = FALSE;
		}
		else
		{
			p->m_nLoginType = EUA_LOGIN_NORMAL;
			p->SetError(NULL);
			p->Login(strCallback.c_str());
		}
	} while (FALSE);

	if ( !bRes )
	{
		p->m_pLogin->SetEnabled(true);
	}
	else
	{
		p->ShowWindow(false);
	}

	p->m_pLogin->SetEnabled(true);

	return 0;
}

DWORD WINAPI CLoginWnd::OnLoginUserIdentThread( LPVOID lp )
{
	CLoginWnd* p = (CLoginWnd*)lp;
	CControlUI* pUsername = p->m_PaintManager.FindControl(_T("edit_username"));
	CControlUI* pIdencode = p->m_PaintManager.FindControl(_T("edit_identcode"));
	if ( pUsername == NULL || p->m_pGetIdentCode == NULL ) return -1;

	BOOL bRes = TRUE;
	do 
	{
		string strUsername = WtoA(pUsername->GetText());
		string strIdentCode= WtoA(pIdencode->GetText());
		if ( strUsername.length() == 0 || strIdentCode.length() == 0 )
		{
			p->SetError(L"请输入正确的手机及验证码");
			bRes = FALSE;
			break;
		}

		string strCallback;
		HttpClient http;
		if (http.Post("www.baidu.com", "", NULL, strCallback) != CURLE_OK)
		{
			strCallback = "";
		}

		if ( strcmp(SuinegJsonHelper::GetValueString(strCallback, "returnCode").c_str(), "200" ) != 0 ) 
		{
			string strErrorInfo = SuinegJsonHelper::GetValueString(strCallback, "returnMessage");
			if ( strErrorInfo.length() == 0 ) strErrorInfo = "登录失败";
			else strErrorInfo = UTF8toA(strErrorInfo.c_str());
			p->SetError(AtoW(strErrorInfo).c_str());
			bRes = FALSE;
		}
		else
		{
			p->m_nLoginType = EUA_LOGIN_PHONE;
			p->SetError(NULL);
			ControlAction(pIdencode, SetText, _T(""));
			p->m_nGetIdentCodeTime = 0;
			p->GetIdentCodeShow();
			p->Login(strCallback.c_str());
		}
	} while (FALSE);

	if ( !bRes )
	{
		p->m_pLogin->SetEnabled(true);
	}
	else
	{
		p->ShowWindow(false);
	}

	p->m_pLogin->SetEnabled(true);

	return 0;
}

void CLoginWnd::Login( const char* pszJasn )
{
	//{"id":30983783,"passport":"q_71829057242807","birthday":0,"gender":"M","point":0,"point_earned":0,"uid":30983783,
	//"mobile":null,"userName":"熊宇-湖北盛天网络技术股","logoPath":"https://register.stnts.com/storage/Security/309/783/30983783_92x92.jpg?1500004205412"}
	CUser::GetInstance()->UserLogin(pszJasn);
	CClientStatistic::GetInstance()->PackUserAction((EnumUserAction)m_nLoginType);
}

void CLoginWnd::OnReturn( TNotifyUI& msg )
{
	OnLogin();
}

void CLoginWnd::CenterWindow()
{
	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	MoveWindow(m_hWnd, rcDesktop.right-rcClient.right-VALUE_MAIN_FRAME_CX, VALUE_HEIGHT_DIST, rcClient.right, rcClient.bottom, TRUE);
}
