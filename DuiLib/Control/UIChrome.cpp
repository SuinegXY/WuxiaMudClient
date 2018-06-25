#include "stdafx.h"
#include "include/cef_runnable.h"

#define			FLASH_PLUGIN_FILENAME			_T("NPSWF32.dll")
#define			CEF_GETCOOKIE_EVENT				_T("STGETCOOKIE")
#define			CEF_SETCOOKIE_EVENT				_T("STGETCOOKIE")
#define			CEF_EXECJSFUNC_EVENT			_T("STEXECJSFUNC")

DuiLib::CChromeUI::CChromeUI()
{
	m_sHomePage.Empty();
	m_sUrlCache.Empty();
	m_sErrorPage.Empty();
	m_pHandler = NULL;
	m_fJScallback = NULL;
	m_fCreateJSFunc = NULL;
	m_fNavStateChanged = NULL;
	m_fBeforeBrowse = NULL;
	m_bPopMessage = FALSE;
	m_bAutoNavi = TRUE;
	m_bPopup	= FALSE;
	m_bStop		= FALSE;
	m_bLoadHide	= FALSE;
}

void DuiLib::CChromeUI::Init()
{
}

void DuiLib::CChromeUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		if (m_pHandler)
		{
			HWND hWnd = m_pHandler->GetBrowserWnd();
			if (hWnd)
			{
				HWND hFocusWnd = ::GetFocus();
				if (hWnd != hFocusWnd)
				{
					::SetFocus(hWnd);
				}
			}
		}
	}
	__super::DoEvent(event);
}

//设置主页
void DuiLib::CChromeUI::SetHomePage(LPCTSTR lpszUrl)
{
	m_sHomePage.Format(_T("%s"), lpszUrl);
}

//得到主页
LPCTSTR DuiLib::CChromeUI::GetHomePage()
{
	return m_sHomePage;
}

//设置出错页面
void DuiLib::CChromeUI::SetErrorPage(LPCTSTR lpszUrl)
{
	m_sErrorPage.Format(_T("%s"), lpszUrl);
}

//得到出错页面
LPCTSTR DuiLib::CChromeUI::GetErrorPage()
{
	return m_sErrorPage;
}

//设置是否提供加载页面消息处理
void DuiLib::CChromeUI::SetPopMessage(BOOL bPop)
{
	m_bPopMessage = bPop;
}

//得到是否提供加载页面消息处理
BOOL DuiLib::CChromeUI::GetPopMessage()
{
	return m_bPopMessage;
}

//设置是否启动时加载页面
void DuiLib::CChromeUI::SetAutoNavi(BOOL bAuto)
{
	m_bAutoNavi = bAuto;
}

//得到是否启动时加载页面
BOOL DuiLib::CChromeUI::GetAutoNavi()
{
	return m_bAutoNavi;
}

void DuiLib::CChromeUI::ShowDevTool(BOOL bShow)
{
	if ( m_fShowDevTool && m_fShowDevTool() && FALSE )
	{
		if (m_pHandler)
		{
			CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
			if (browser.get())
			{
				if (bShow)
				{
					browser->ShowDevTools();
				}
				else
				{
					browser->CloseDevTools();
				}
			}
		}
	}
}

BOOL DuiLib::CChromeUI::CreateChromeWnd(HWND hParent, RECT& rc)
{
	if (NULL == m_pHandler)
	{
		m_pHandler = new CChromeUIEventHandler();
		if (m_pHandler)
		{
			CefWindowInfo info;
			info.SetAsChild(hParent, rc);
			CefBrowserSettings settings;
			settings.javascript_access_clipboard_disallowed = 1;
			settings.dom_paste_disabled = 1;
			settings.caret_browsing_enabled = 0;
			bool bRes = false;
			if (m_bAutoNavi)
			{
				bRes = CefBrowser::CreateBrowser(info, static_cast<CefRefPtr<CefClient>>(m_pHandler), (LPCTSTR)(m_sUrlCache.GetLength() ? m_sUrlCache : m_sHomePage), settings);
				m_sUrlCache.Empty();
			}
			else
			{
				bRes = CefBrowser::CreateBrowser(info, static_cast<CefRefPtr<CefClient>>(m_pHandler), (LPCTSTR)_T(""), settings);
			}
			if (bRes)
			{
				OutputDebugString(m_sHomePage);
				OutputDebugString(_T("Create Chrome Control Success\r\n"));
			}
			m_pHandler->SetControl(this);
		}
	}
	return TRUE;
}

void DuiLib::CChromeUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	if (rc.right - rc.left > 0)
	{
		if (NULL == m_pHandler)
		{
			CreateChromeWnd(m_pManager->GetPaintWindow(), rc);
		}
		else
		{
			HWND hWnd = m_pHandler->GetBrowserWnd();
			if (IsWindow(hWnd))
			{
				MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
			}
		}
	}
}

void DuiLib::CChromeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("homepage")) == 0 ) SetHomePage(pstrValue);
	else if( _tcscmp(pstrName, _T("autonavi")) == 0 ) SetAutoNavi(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("loadmsg")) == 0 ) SetPopMessage(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("popup")) == 0 ) SetNoPopup(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("loadhide")) == 0 ) SetLoadHide(_tcscmp(pstrValue, _T("true")) == 0);
	CControlUI::SetAttribute(pstrName, pstrValue);
}

void DuiLib::CChromeUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	if (m_pHandler)
	{
		HWND hWnd = m_pHandler->GetBrowserWnd();
		if (::IsWindow(hWnd))
		{
			int nShowType = bVisible;
			if ( bVisible )
				nShowType =  SW_SHOWNOACTIVATE;
			::ShowWindow(hWnd, nShowType);
		}
	}
}

void DuiLib::CChromeUI::SetInternVisible(bool bVisible)
{
	CControlUI::SetInternVisible(bVisible);
	if (m_pHandler)
	{
		HWND hWnd = m_pHandler->GetBrowserWnd();
		::ShowWindow(hWnd, bVisible);
	}
}

void DuiLib::CChromeUI::Refresh()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			browser->Reload();
		}
	}
}

void DuiLib::CChromeUI::Stop()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			browser->StopLoad();
		}
	}
}

BOOL DuiLib::CChromeUI::CanGoBack()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			return browser->CanGoBack();
		}
	}
	return FALSE;
}

void DuiLib::CChromeUI::GoBack()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			browser->GoBack();
		}
	}
}

BOOL DuiLib::CChromeUI::CanGoForword()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			return browser->CanGoForward();
		}
	}
	return FALSE;
}

void DuiLib::CChromeUI::GoForward()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			browser->GoForward();
		}
	}
}

void DuiLib::CChromeUI::NavigateUrl(LPCTSTR lpszUrl)
{
	m_sUrlCache = lpszUrl;
	if ( m_bLoadHide ) SetVisible(false);
	if ( m_pHandler )
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			CefRefPtr<CefFrame> frame = browser->GetMainFrame();
			if (frame.get())
			{
				CDuiString szUrl(lpszUrl);
				if (szUrl.IsEmpty())
				{
					return;
				}
				frame->LoadURL(lpszUrl);
			}
		}
	}
}

void DuiLib::CChromeUI::NavigateHomePage()
{
	if (m_sHomePage.IsEmpty())
	{
		return;
	}
	NavigateUrl(m_sHomePage);
}

void DuiLib::CChromeUI::ClearHistory()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			browser->ClearHistory();
		}
	}
}

//得到当前访问的URL
BOOL DuiLib::CChromeUI::GetUrl(TCHAR* pszUrl)
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			CefRefPtr<CefFrame> frame = browser->GetMainFrame();
			if (frame.get())
			{
				CefString strUrl = frame->GetURL();
				MultiByteToWideChar(CP_ACP, 0, strUrl.ToString().c_str(), strUrl.length()+1, pszUrl, 512);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void DuiLib::CChromeUI::UIT_Eval(CefRefPtr<CefFrame> frame, const CefString& code)
{
	CefRefPtr<CefV8Context> context = frame->GetV8Context();
	if (context.get())
	{
		if (context->Enter())
		{
			CefRefPtr<CefV8Value> vRet;
			CefRefPtr<CefV8Exception> vE;
			context->Eval(code, vRet, vE);
			context->Exit();
		}
	}
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CEF_EXECJSFUNC_EVENT);
	if (hEvent)
	{
		SetEvent(hEvent);
	}
}

//执行Eval
BOOL DuiLib::CChromeUI::Eval(LPCTSTR lpszCode)
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			CefString strCode;
			strCode.FromWString(lpszCode);
			CefRefPtr<CefFrame> frame = browser->GetMainFrame();
			if (frame.get())
			{
				CefRefPtr<CefV8Value> vRet;
				CefPostTask(TID_UI, NewCefRunnableFunction(UIT_Eval, frame, strCode));
				HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, CEF_EXECJSFUNC_EVENT);
				if (hEvent)
				{
					WaitForSingleObject(hEvent, 1000);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

void DuiLib::CChromeUI::UIT_ExecuteJsFunction(CefRefPtr<CefFrame> frame, const CefString& name, const vector<VARIANT>& vList, const VARIANT& vRet)
{
	CefRefPtr<CefV8Context> context = frame->GetV8Context();
	if (context.get())
	{
		if (context->Enter())
		{
			CefRefPtr<CefV8Value> window = context->GetGlobal();
			if (window.get())
			{
				if (window->HasValue("ST"))
				{
					CefRefPtr<CefV8Value> st = window->GetValue("ST");
					if (st.get())
					{
						CefRefPtr<CefV8Value> myFun = st->GetValue(name);
						if (myFun.get() && myFun->IsFunction())
						{
							CefRefPtr<CefV8Value> argv;
							CefV8ValueList varList;
							for (int i = 0; i < (int)vList.size(); i++)
							{
								if (vList[i].vt == VT_BSTR)
								{
									argv = CefV8Value::CreateString(vList[i].bstrVal);
								}
								else if (vList[i].vt == VT_BOOL)
								{
									if (vList[i].boolVal == VARIANT_FALSE)
									{
										argv = CefV8Value::CreateBool(false);
									}
									else
									{
										argv = CefV8Value::CreateBool(true);
									}
								}
								else if (vList[i].vt == VT_I4)
								{
									argv = CefV8Value::CreateInt(vList[i].intVal);
								}
								else if (vList[i].vt == VT_UI8)
								{
									argv = CefV8Value::CreateUInt((vList[i].uintVal));
								}
								varList.push_back(argv);
							}
							CefRefPtr<CefV8Value> vRes = myFun->ExecuteFunction(window, varList);
							if (vRes.get())
							{
							}
						}
					}
				}
				else
				{
					CefRefPtr<CefV8Value> myFun = window->GetValue(name);
					if (myFun.get() && myFun->IsFunction())
					{
						CefRefPtr<CefV8Value> argv;
						CefV8ValueList varList;
						for (int i = 0; i < (int)vList.size(); i++)
						{
							if (vList[i].vt == VT_BSTR)
							{
								argv = CefV8Value::CreateString(vList[i].bstrVal);
							}
							else if (vList[i].vt == VT_BOOL)
							{
								if (vList[i].boolVal == VARIANT_FALSE)
								{
									argv = CefV8Value::CreateBool(false);
								}
								else
								{
									argv = CefV8Value::CreateBool(true);
								}
							}
							else if (vList[i].vt == VT_I4)
							{
								argv = CefV8Value::CreateInt(vList[i].intVal);
							}
							else if (vList[i].vt == VT_UI8)
							{
								argv = CefV8Value::CreateUInt((vList[i].uintVal));
							}
							varList.push_back(argv);
						}
						CefRefPtr<CefV8Value> vRes = myFun->ExecuteFunction(window, varList);
						if (vRes.get())
						{
						}
					}
				}
			}
			context->Exit();
		}
	}
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CEF_EXECJSFUNC_EVENT);
	if (hEvent)
	{
		SetEvent(hEvent);
	}
}

BOOL DuiLib::CChromeUI::ExecuteJsFunc(LPCTSTR lpszName, vector<VARIANT>& var, VARIANT& vRet)
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			CefString strName;
			strName.FromWString(lpszName);
			CefRefPtr<CefFrame> frame = browser->GetMainFrame();
			if (frame.get())
			{
				VARIANT vTmp;
				CefPostTask(TID_UI, NewCefRunnableFunction(UIT_ExecuteJsFunction, frame, strName, var, vTmp));
				HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, CEF_EXECJSFUNC_EVENT);
				if (hEvent)
				{
					WaitForSingleObject(hEvent, 1000);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

void DuiLib::CChromeUI::IOT_SetCookie(CefRefPtr<CefCookieManager> manager, const CefString& url, CookieVector* cookies)
{
	bool bRes = true;
	CookieVector::const_iterator it = cookies->begin();
	for (; it != cookies->end(); ++it)
	{
		bRes = manager->SetCookie(url, *it);
	}
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CEF_SETCOOKIE_EVENT);
	if (hEvent)
	{
		SetEvent(hEvent);
	}
}


//设置指定Url对应的Cookie
BOOL	DuiLib::CChromeUI::SetCookie(LPCTSTR lpszUrl, vector<string>& cookieList)
{
	if (m_pHandler)
	{
		CookieVector cookies;
		CefRefPtr<CefCookieManager> Manager = CefCookieManager::GetGlobalManager();
		if (Manager.get())
		{
			for (int i = 0; i < (int)cookieList.size(); i++)
			{
				CefCookie cookie;
				BOOL bHttpOnly = FALSE;
				string strTmp = cookieList[i];
				string::size_type pos = cookieList[i].find("#HttpOnly_");
				if (pos != cookieList[i].npos)
				{
					bHttpOnly = TRUE;
					strTmp = cookieList[i].substr(10, cookieList[i].size() - 10);
				}
				char szDomain[MAX_PATH] = {0};
				char szTailMatch[MAX_PATH] = {0};
				char szPath[MAX_PATH] = {0};
				char szName[MAX_PATH] = {0};
				char szValue[MAX_PATH] = {0};
				char szSecure[MAX_PATH] = {0};
				DWORD dwTime = 0;
				sscanf(strTmp.c_str(), "%s\t%s\t%s\t%s\t%d\t%s\t%s", szDomain, szTailMatch, szPath, szSecure, &dwTime, szName, szValue);
				BOOL bSecure = FALSE;
				if (strcmp(szSecure, "TRUE") == 0)
				{
					bSecure = TRUE;
				}
				//TODO:暂时使用内存保存Cookie，只要程序关闭，cookie就会被删除，因此未设置过期
				//BOOL bExpire = FALSE;
				//if (dwTime > 0)
				//{
				//	bExpire = TRUE;
				//}
				//struct tm *newtime;
				//if (bExpire)
				//{
				//	cookie.has_expires = true;
				//	__time64_t long_time = dwTime;
				//	newtime = _localtime64(&long_time);
				//	if (newtime)
				//	{
				//		cookie.expires.year = 1970 + newtime->tm_year;
				//		cookie.expires.month = newtime->tm_mon;
				//		cookie.expires.day_of_month = newtime->tm_mday;
				//		cookie.expires.day_of_week = newtime->tm_wday;
				//		cookie.expires.hour = newtime->tm_hour;
				//		cookie.expires.minute = newtime->tm_min;
				//		cookie.expires.second = newtime->tm_sec;
				//	}
				//}
				if (bHttpOnly)
				{
					cookie.httponly = true;
				}
				if (bSecure)
				{
					cookie.secure = true;
				}
				CefString(&cookie.name).FromString(szName);
				CefString(&cookie.value).FromString(szValue);
				CefString(&cookie.domain).FromString(szDomain);
				CefString(&cookie.path).FromString(szPath);
				cookies.push_back(cookie);
			}
			CefString strUrl;
			strUrl.FromWString(lpszUrl);
			CefPostTask(TID_IO, NewCefRunnableFunction(IOT_SetCookie, Manager, strUrl, &cookies));
		}
		HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, CEF_SETCOOKIE_EVENT);
		if (hEvent)
		{
			WaitForSingleObject(hEvent, 1000);
		}
	}
	return FALSE;
}

////得到指定Url对应的Cookie
//void	DuiLib::CChromeUI::GetCookie(LPCTSTR lpszUrl, LPCTSTR lpszName, LPCTSTR lpszValue, LPCTSTR lpszDomain, LPCTSTR lpszPath)
//{
//	CefRefPtr<CefCookieManager> Manager = CefCookieManager::GetGlobalManager();
//	if (Manager.get())
//	{
//		vector<CefCookie> cookies;
//		bool bRes = Manager->VisitUrlCookies(CefString(lpszUrl), false, new CChromeUICookieVisitor(&cookies));
//		if (bRes)
//		{
//			HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CEF_GETCOOKIE_EVENT);
//			if (hEvent)
//			{
//				if (WAIT_OBJECT_0 == WaitForSingleObject(hEvent, 4000))
//				{
//					if (cookies.size() > 0)
//					{
//						if (lpszName)
//						{
//							_tcscpy((TCHAR*)lpszName, CefString(&cookies[0].name).c_str());
//						}
//						if (lpszValue)
//						{
//							_tcscpy((TCHAR*)lpszValue, CefString(&cookies[0].value).c_str());
//						}
//						if (lpszDomain)
//						{
//							_tcscpy((TCHAR*)lpszDomain, CefString(&cookies[0].domain).c_str());
//						}
//						if (lpszPath)
//						{
//							_tcscpy((TCHAR*)lpszPath, CefString(&cookies[0].path).c_str());
//						}
//					}
//				}
//			}
//		}
//	}
//}

void DuiLib::CChromeUI::IOT_DeleteCookie(CefRefPtr<CefCookieManager> manager, const CefString& url, const CefString& name)
{ 
	bool bRes = manager->DeleteCookies(url, name);
}

//删除Cookie
BOOL	DuiLib::CChromeUI::DeleteCookie(LPCTSTR lpszUrl, LPCTSTR lpszName)
{
	CefRefPtr<CefCookieManager> Manager = CefCookieManager::GetGlobalManager();
	if (Manager.get())
	{
		CefString strUrl;
		CefString strName;
		if (lpszUrl)
		{
			strUrl.FromWString(lpszUrl);
		}
		if (lpszName)
		{
			strName.FromWString(lpszName);
		}
		CefPostTask(TID_IO, NewCefRunnableFunction(IOT_DeleteCookie, Manager, strUrl, strName));
	}
	return TRUE;
}

void DuiLib::CChromeUI::CloseBrowser()
{
	if (m_pHandler)
	{
		CefRefPtr<CefBrowser> browser = m_pHandler->GetBrowser();
		if (browser.get())
		{
			browser->CloseBrowser();
		}
	}
}

//自定义JS函数调用时
bool DuiLib::CChromeUI::OnExecute(const CefString& name, 
			   CefRefPtr<CefV8Value> object, 
			   const CefV8ValueList& arguments, 
			   CefRefPtr<CefV8Value>& retval, 
			   CefString& exception)
{
    bool bSuccess = true;

	if (m_fJScallback)
	{
		int nLen = (int)arguments.size();
		TCHAR* szStr = NULL;
		vector<VARIANT> argv(nLen);
		for (int i = 0; i < nLen; i++)
		{
			VariantInit(&argv[i]);
			if (arguments[i]->IsString())
			{
				V_VT(&argv[i]) = VT_BSTR;
				V_BSTR(&argv[i]) = SysAllocString(arguments[i]->GetStringValue().c_str());;
			}
			else if (arguments[i]->IsInt())
			{
				V_VT(&argv[i]) = VT_I4;
				V_I4(&argv[i]) = arguments[i]->GetIntValue();
			}
			else if (arguments[i]->IsUInt())
			{
				V_VT(&argv[i]) = VT_UI4;
				V_UI4(&argv[i]) = arguments[i]->GetUIntValue();
			}
			else if (arguments[i]->IsBool())
			{
				V_VT(&argv[i]) = VT_BOOL;
				V_BOOL(&argv[i]) = arguments[i]->GetBoolValue();;				
			}
		}

		VARIANT Var;
		VariantInit(&Var);
		bSuccess = m_fJScallback(name, argv, &Var);
		if (Var.vt != VT_EMPTY)
		{
			switch (Var.vt)
			{
			case VT_BSTR:
				retval = CefV8Value::CreateString(Var.bstrVal);
				break;
			case VT_INT:
            case VT_I4:
				retval = CefV8Value::CreateInt(Var.intVal);
				break;
			case VT_BOOL:
				{
					if (Var.boolVal == VARIANT_FALSE)
					{
						retval = CefV8Value::CreateBool(false);
					}
					else
					{
						retval = CefV8Value::CreateBool(true);
					}
				}
				break;
			default:
				break;
			}
			VariantClear(&Var);
		}
		else
		{
			retval = CefV8Value::CreateNull();
		}
	}
	return bSuccess;
}

BOOL DuiLib::CChromeUI::OnContextCreated(OLECHAR*** var,int& nItem)
{
	if (m_fCreateJSFunc)
	{
		m_fCreateJSFunc(var,nItem);
	}
	if (m_bPopMessage)
	{
		PostMessage(m_pManager->GetPaintWindow(), WM_CEF_CONTEXTCREATE, 0, 0);
	}
	return TRUE;
}

void DuiLib::CChromeUI::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	if ( m_bLoadHide && !m_bStop ) SetVisible(true);
	if (m_bPopMessage)
	{
		PostMessage(m_pManager->GetPaintWindow(), WM_CEF_LOADEND, 0, 0);
	}
}

void DuiLib::CChromeUI::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	if (m_bPopMessage)
	{
		PostMessage(m_pManager->GetPaintWindow(), WM_CEF_LOADSTART, 0, 0);
	}
	CefRefPtr<CefDOMVisitor> visitor = CefRefPtr<CefDOMVisitor>(new CCustomDOMVisitor(this));
	if (visitor.get())
	{
		frame->VisitDOM(visitor);
	}
}

void DuiLib::CChromeUI::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (m_rcItem.right > m_rcItem.left)
	{
		HWND hWnd = browser->GetWindowHandle();
		if (IsWindow(hWnd))
		{
			MoveWindow(hWnd, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, TRUE);
		}
	}
}

void DuiLib::CChromeUI::OnCustomScheme(string url)
{
	if (m_fCustomScheme)
	{
		m_fCustomScheme(url);
	}
}

void DuiLib::CChromeUI::OnNavStateChange(bool canGoBack, bool canGoForword)
{
	if (m_fNavStateChanged)
	{
		m_fNavStateChanged(canGoBack, canGoForword);
	}
}

bool DuiLib::CChromeUI::InitWebkit(bool bMulti_Thread_Msg_Loop)
{
	CefRefPtr<CefApp> app;
	CefSettings settings;
	settings.log_severity = LOGSEVERITY_DISABLE;
	settings.multi_threaded_message_loop = bMulti_Thread_Msg_Loop;
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szSWFFile[MAX_PATH];
	_tsplitpath_s(szPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, NULL, NULL, NULL);
	_tmakepath_s(szPath, MAX_PATH, szDrive, szDir, NULL, NULL);
	wsprintf(szSWFFile, _T("%s%s"), szPath, FLASH_PLUGIN_FILENAME);
	cef_string_t str;
	memset(&str, 0, sizeof(str));
	settings.extra_plugin_paths = cef_string_list_alloc();
	CefString(&str).FromWString(szSWFFile);
	cef_string_list_append(settings.extra_plugin_paths, &str);
	return CefInitialize(settings, app);
}

void DuiLib::CChromeUI::RunWebkitMessageLoop()
{
	CefRunMessageLoop();
}

void DuiLib::CChromeUI::ShutdownWebkit()
{
	//延时退出，解决CEF1退出时由于请求未结束导致的异常
	//非最终解决方案，由于CEF1已不再更新，考虑使用CEF3来进行替换
	Sleep(5000);
	CefShutdown();
}

bool DuiLib::CChromeUI::OnBeforeBrowse(TCHAR* pszUrl)
{
	if ( m_bLoadHide ) SetVisible(false);
	if (m_fBeforeBrowse)
	{
		return m_fBeforeBrowse(pszUrl);
	}
	return false;
}

void DuiLib::CChromeUI::OnDOMContentLoaded()
{
	if (m_bPopMessage)
	{
		PostMessage(m_pManager->GetPaintWindow(), WM_CEF_DOMCONTENTLOADED, 0, 0);
	}
}

bool DuiLib::CChromeUI::OnBeforePopup( CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings )
{
	if ( url.length() && m_bPopup )
	{
		NavigateUrl(url.c_str());
		return true;
	}
	return false;
}

void DuiLib::CChromeUI::SetNoPopup( BOOL bPopup )
{
	m_bPopup = bPopup;
}

void DuiLib::CChromeUI::SetStop( BOOL bStop )
{
	m_bStop = bStop;
}

void DuiLib::CChromeUI::SetLoadHide( BOOL bLoadHide )
{
	m_bLoadHide = bLoadHide;
}

void DuiLib::CChromeUI::SetInit()
{
	if ( !m_pHandler )
	{
		RECT rc = {-1,-1,0,0};
		CreateChromeWnd(m_pManager->GetPaintWindow(), rc);
	}
}

DuiLib::fOnShowDevTool DuiLib::CChromeUI::m_fShowDevTool = NULL;

//////////////////////////////////////////////////////////////////////////
// EventHandler
//////////////////////////////////////////////////////////////////////////
DuiLib::CChromeUIEventHandler::CChromeUIEventHandler()
{
	m_pControl = NULL;
	m_bEditableFocus = FALSE;
}

void DuiLib::CChromeUIEventHandler::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	// 得到脚本window对象
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	// 创建external对象，隶属于window
	CefRefPtr<CefV8Value> external = CefV8Value::CreateObject(NULL);
	object->SetValue(STNTS_CEFTOJS_OBJECT, external, V8_PROPERTY_ATTRIBUTE_NONE);

	if (m_pControl)
	{
		OLECHAR** var;
        int nItem = 0;
		if (m_pControl->OnContextCreated(&var,nItem))
		{
			if (!m_pV8Hander.get())
			{
				m_pV8Hander = new CChromeV8Handler();
				m_pV8Hander->SetControl(m_pControl);
			}
			if (m_pV8Hander)
			{
				CefRefPtr<CefV8Value> valueFun;
				for (int i = 0; i < (int)nItem; i++)
				{
					valueFun = CefV8Value::CreateFunction(var[i], (CefRefPtr<CefV8Handler>)m_pV8Hander);
					external->SetValue(var[i], valueFun, V8_PROPERTY_ATTRIBUTE_NONE);
					HeapFree(GetProcessHeap(),0,var[i]);
				}
			}
		}
	}
}

void DuiLib::CChromeUIEventHandler::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	if (m_pV8Hander)
	{
		m_pV8Hander = NULL;
	}
}

void DuiLib::CChromeUIEventHandler::SetControl(CChromeUI* pControl)
{
	m_pControl = pControl;
	////TODO:使用自定义协议列表记录协议名称，避免重复注册
	////重复注册协议Debug版会出现断言，Release版正常
	//if (CefRegisterCustomScheme("gamerun", true, false, false))
	//{
	//	CefRegisterSchemeHandlerFactory("gamerun", CefString(), new CCustomSchemeHandlerFactory(pControl));
	//}
}

void DuiLib::CChromeUIEventHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	if (m_pControl)
	{
		m_pControl->OnLoadEnd(browser, frame, httpStatusCode);
	}
}

void DuiLib::CChromeUIEventHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	if (m_pControl)
	{
		m_pControl->OnLoadStart(browser, frame);
	}
}

bool DuiLib::CChromeUIEventHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& failedUrl, CefString& errorText)
{
	if (m_pControl)
	{
		CDuiString strUrl = m_pControl->GetErrorPage();
		if (strUrl.IsEmpty())
		{
			m_pControl->SetVisible(false);
		}
		else
		{
			m_pControl->NavigateUrl(strUrl);
		}
	}
	return false;
}

bool DuiLib::CChromeUIEventHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript)
{
	if (isAfterJavaScript)
	{
		if (code == VK_F12)
		{
			m_pControl->ShowDevTool(TRUE);
		}
		else if (code == VK_F5 && type != KEYEVENT_CHAR)
		{
			m_pControl->Refresh();
			return true;
		}
		else if (code == VK_BACK && !m_bEditableFocus)
		{
			return true;
		}
	}
	return false;
}

void DuiLib::CChromeUIEventHandler::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
	m_bEditableFocus = FALSE;
	if (node.get() && node->IsFormControlElement())
	{
		CefString str = node->GetFormControlElementType();
		if ((str == "text") || (str == "textarea") || (str == "password"))
		{
			m_bEditableFocus = TRUE;
		}
	}
}

void DuiLib::CChromeUIEventHandler::OnNavStateChange(CefRefPtr<CefBrowser> browser, bool canGoBack, bool canGoForward)
{
	if (m_pControl)
	{
		m_pControl->OnNavStateChange(canGoBack, canGoForward);
	}
}

bool DuiLib::CChromeUIEventHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavType navType, bool isRedirect)
{
	if (m_pControl)
	{
		return m_pControl->OnBeforeBrowse((TCHAR*)request->GetURL().c_str());
	}
	return false;
}

void DuiLib::CChromeUIEventHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	__super::OnAfterCreated(browser);
	if (m_pControl)
	{
		m_pControl->OnAfterCreated(browser);
	}
}

bool DuiLib::CChromeUIEventHandler::OnBeforePopup( CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings )
{
	BOOL bRes = false;
	if ( m_pControl )
	{
		bRes = m_pControl->OnBeforePopup(parentBrowser, popupFeatures, windowInfo, url, client, settings);
	}
	if ( !bRes )
		return __super::OnBeforePopup(parentBrowser, popupFeatures, windowInfo, url, client, settings);
	else
		return bRes;
}

//////////////////////////////////////////////////////////////////////////
// Cookie
//////////////////////////////////////////////////////////////////////////
DuiLib::CChromeUICookieVisitor::CChromeUICookieVisitor(vector<CefCookie>* cookies)
{
	cookies_ = cookies;
	CreateEvent(NULL, FALSE, FALSE, CEF_GETCOOKIE_EVENT);
}

DuiLib::CChromeUICookieVisitor::~CChromeUICookieVisitor()
{
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CEF_GETCOOKIE_EVENT);
	if (hEvent)
	{
		SetEvent(hEvent);
	}
}


bool DuiLib::CChromeUICookieVisitor::Visit(const CefCookie& cookie, int count, int total, bool& deleteCookie)
{
	cookies_->push_back(cookie);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Scheme
//////////////////////////////////////////////////////////////////////////
DuiLib::CCustomSchemeHandler::CCustomSchemeHandler(CChromeUI* pControl)
{
	m_pControl = pControl;
}

bool DuiLib::CCustomSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefSchemeHandlerCallback> callback)
{
	string strUrl = request->GetURL();
	if (m_pControl)
	{
		m_pControl->OnCustomScheme(strUrl);
	}
	return false;
}

void DuiLib::CCustomSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl)
{

}

bool DuiLib::CCustomSchemeHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefSchemeHandlerCallback> callback)
{
	return false;
}

void DuiLib::CCustomSchemeHandler::Cancel()
{

}

DuiLib::CCustomSchemeHandlerFactory::CCustomSchemeHandlerFactory(CChromeUI* pControl)
{
	m_pControl = pControl;
}

CefRefPtr<CefSchemeHandler> DuiLib::CCustomSchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser, const CefString& scheme_name, CefRefPtr<CefRequest> request)
{
	return new CCustomSchemeHandler(m_pControl);
}

//////////////////////////////////////////////////////////////////////////
//CChromeV8Handler
DuiLib::CChromeV8Handler::CChromeV8Handler()
{
	m_pControl = NULL;
}

bool DuiLib::CChromeV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	if (m_pControl)
	{
		return m_pControl->OnExecute(name, object, arguments, retval, exception);
	}
	return false;
}

void DuiLib::CChromeV8Handler::SetControl(CChromeUI* pControl)
{
	m_pControl = pControl;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//DOM Event
DuiLib::CCustomDOMListener::CCustomDOMListener(CChromeUI* pControl)
{
	m_pControl = pControl;
}

void DuiLib::CCustomDOMListener::HandleEvent(CefRefPtr<CefDOMEvent> event)
{
	if (m_pControl)
	{
		m_pControl->OnDOMContentLoaded();
	}
}

DuiLib::CCustomDOMVisitor::CCustomDOMVisitor(CChromeUI* pControl)
{
	m_pControl = pControl;
}

void DuiLib::CCustomDOMVisitor::Visit(CefRefPtr<CefDOMDocument> document)
{
	if (m_pControl)
	{
		CefRefPtr<CefDOMEventListener> listener = CefRefPtr<CefDOMEventListener>(new CCustomDOMListener(m_pControl));
		document->GetDocument()->AddEventListener(L"DOMContentLoaded", listener, true);
	}
}