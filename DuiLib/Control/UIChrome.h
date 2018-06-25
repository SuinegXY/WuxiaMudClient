#ifndef  __UICHROME_H__
#define  __UICHROME_H__

#undef GetNextSibling
#undef GetFirstChild

#include "include/cef_app.h"
#include "include/cef_scheme.h"
#include "Utils/ChromeEventHandler.h"

#pragma once

namespace DuiLib
{
	typedef bool (*fJSCallBack)(const string&, vector<VARIANT>&, VARIANT*);
	typedef void (*fCreateJSFunc)(OLECHAR***,int& nItem);
	typedef void (*fOnCustomScheme)(string);
	typedef void (*fOnNavStateChanged)(bool bCanBack, bool bCanForword);
	typedef bool (*fOnBeforeBrowse)(TCHAR* pszUrl);
	typedef bool (*fOnShowDevTool)();

	//////////////////////////////////////////////////////////////////////////
	//DOM Event
	class CCustomDOMListener : public CefDOMEventListener
	{
	public:
		CCustomDOMListener(CChromeUI* pControl);

		void HandleEvent(CefRefPtr<CefDOMEvent> event);

	private:
		IMPLEMENT_REFCOUNTING(CCustomDOMListener);

		CChromeUI*	m_pControl;
	};

	class CCustomDOMVisitor : public CefDOMVisitor
	{
	public:
		CCustomDOMVisitor(CChromeUI* pControl);

		void Visit(CefRefPtr<CefDOMDocument> document);

	private:
		IMPLEMENT_REFCOUNTING(CCustomDOMVisitor);

		CChromeUI*	m_pControl;
	};

	//////////////////////////////////////////////////////////////////////////
	//Scheme
	class CCustomSchemeHandler : public CefSchemeHandler
	{
	public:
		CCustomSchemeHandler(CChromeUI* pControl);
		///
		// Begin processing the request. To handle the request return true and call
		// HeadersAvailable() once the response header information is available
		// (HeadersAvailable() can also be called from inside this method if header
		// information is available immediately). To cancel the request return false.
		///
		/*--cef()--*/
		virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
			CefRefPtr<CefSchemeHandlerCallback> callback) OVERRIDE;

		///
		// Retrieve response header information. If the response length is not known
		// set |response_length| to -1 and ReadResponse() will be called until it
		// returns false. If the response length is known set |response_length|
		// to a positive value and ReadResponse() will be called until it returns
		// false or the specified number of bytes have been read. Use the |response|
		// object to set the mime type, http status code and other optional header
		// values. To redirect the request to a new URL set |redirectUrl| to the new
		// URL.
		///
		/*--cef()--*/
		virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
			int64& response_length,
			CefString& redirectUrl) OVERRIDE;

		///
		// Read response data. If data is available immediately copy up to
		// |bytes_to_read| bytes into |data_out|, set |bytes_read| to the number of
		// bytes copied, and return true. To read the data at a later time set
		// |bytes_read| to 0, return true and call BytesAvailable() when the data is
		// available. To indicate response completion return false.
		///
		/*--cef()--*/
		virtual bool ReadResponse(void* data_out,
			int bytes_to_read,
			int& bytes_read,
			CefRefPtr<CefSchemeHandlerCallback> callback) OVERRIDE;

		///
		// Request processing has been canceled.
		///
		/*--cef()--*/
		virtual void Cancel() OVERRIDE;

		IMPLEMENT_REFCOUNTING(CCustomSchemeHandler);
		IMPLEMENT_LOCKING(CCustomSchemeHandler);

	private:
		CChromeUI*		m_pControl;
	};

	class CCustomSchemeHandlerFactory : public CefSchemeHandlerFactory
	{
	public:
		CCustomSchemeHandlerFactory(CChromeUI* pControl);

		// Return a new scheme handler instance to handle the request.
		virtual CefRefPtr<CefSchemeHandler> Create(CefRefPtr<CefBrowser> browser,
			const CefString& scheme_name,
			CefRefPtr<CefRequest> request)
			OVERRIDE;

		IMPLEMENT_REFCOUNTING(CCustomSchemeHandlerFactory);

	private:
		CChromeUI*		m_pControl;
	};
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//Cookie
	typedef std::vector<CefCookie> CookieVector;

	class CChromeUICookieVisitor : public CefCookieVisitor
	{
	public:
		CChromeUICookieVisitor(vector<CefCookie>* cookies);
		virtual ~CChromeUICookieVisitor();

		virtual bool Visit(const CefCookie& cookie, int count, int total, bool& deleteCookie);

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(CChromeUICookieVisitor);

		vector<CefCookie>* cookies_;
	};
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//JS函数交互处理
	class CChromeV8Handler : public CefV8Handler
	{
	public:
		CChromeV8Handler();

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception);

		void SetControl(CChromeUI* pControl);

	private:
		CChromeUI*	m_pControl;

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(CChromeV8Handler);
	};
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//CEF消息处理
	class CChromeUIEventHandler : public CChromeEventHandler
	{
	public:
		CChromeUIEventHandler();

		bool OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& failedUrl,
			CefString& errorText);

		void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode);

		void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);

		void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);

		void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);

		bool OnKeyEvent(CefRefPtr<CefBrowser> browser, KeyEventType type, int code, int modifiers, bool isSystemKey, bool isAfterJavaScript);

		void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node);

		void OnNavStateChange(CefRefPtr<CefBrowser> browser, bool canGoBack, bool canGoForward);

		bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavType navType, bool isRedirect); 

		void OnAfterCreated(CefRefPtr<CefBrowser> browser);

		void SetControl(CChromeUI* pControl);

		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,	const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings);

	private:
		CChromeUI* m_pControl;		//对应CEF控件

		CefRefPtr<CChromeV8Handler> m_pV8Hander;

		BOOL	m_bEditableFocus;		//判断当前得到焦点的元素是否为Edit

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(CChromeUIEventHandler);
		// Include the default locking implementation.
		IMPLEMENT_LOCKING(CChromeUIEventHandler);
	};
	//////////////////////////////////////////////////////////////////////////

	template class UILIB_API CefRefPtr<CChromeUIEventHandler>;

	class UILIB_API CChromeUI
		: public CControlUI
	{
	public:
		CChromeUI();

		//初始化CEF环境 bMulti_Thread_Msg_Loop = true CEF在自己的线程中创建消息循环，与窗口消息循环不冲突
		static bool InitWebkit(bool bMulti_Thread_Msg_Loop);
		//启动CEF循环
		static void RunWebkitMessageLoop();
		//关闭CEF环境
		static void ShutdownWebkit();
		//设置CEF Cookie
		static void IOT_SetCookie(CefRefPtr<CefCookieManager> manager,
			const CefString& url, CookieVector* cookies);
		//删除CEF Cookie
		static void IOT_DeleteCookie(CefRefPtr<CefCookieManager> manager,
			const CefString& url, const CefString& name);
		//执行JS函数
		static void UIT_ExecuteJsFunction(CefRefPtr<CefFrame> frame, const CefString& name, 
			const vector<VARIANT>& vList, const VARIANT& vRet);
		//执行eval
		static void UIT_Eval(CefRefPtr<CefFrame> frame, const CefString& code);
		
		//////////////////////////////////////////////////////////////////////////
		//CEF使用之前必须进行初始化，以下是程序示例
		//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
		//{
		//	CChromeUI::InitWebkit();

		//	CPaintManagerUI::SetInstance(hInstance);
		//	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\58Game\\"));

		//	HRESULT Hr = ::CoInitialize(NULL);
		//	if( FAILED(Hr) ) return 0;

		//	C58ClientWnd* pFrame = new C58ClientWnd();
		//	if( pFrame == NULL ) return 0;
		//	pFrame->Create(NULL, _T("58Game"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
		//	pFrame->CenterWindow();
		//	::ShowWindow(*pFrame, SW_SHOW);

		//	CChromeUI::RunWebkitMessageLoop();
		//	::CoUninitialize();
		//	CChromeUI::ShutdownWebkit();
		//	return 0;
		//}
		//////////////////////////////////////////////////////////////////////////

		//设置主页
		void SetHomePage(LPCTSTR lpszUrl);
		//得到主页
		LPCTSTR GetHomePage();

		//设置出错页面
		void SetErrorPage(LPCTSTR lpszUrl);
		//得到出错页面
		LPCTSTR GetErrorPage();

		//设置Cookie
		BOOL	SetCookie(LPCTSTR lpszUrl, vector<string>& cookieList);
		//得到Cookie
		//void	GetCookie(LPCTSTR lpszUrl, vector<string>& cookieList);
		//删除Cookie
		BOOL	DeleteCookie(LPCTSTR lpszUrl, LPCTSTR lpszName);

		//设置是否提供加载页面消息处理
		void SetPopMessage(BOOL bPop);
		//得到是否提供加载页面消息处理
		BOOL GetPopMessage();

		//设置是否启动时加载页面
		void SetAutoNavi(BOOL bAuto);
		//得到是否启动时加载页面
		BOOL GetAutoNavi();
		
		//控件属性方法
		void Init();
		void SetPos(RECT rc);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetVisible(bool bVisible);
		void SetInternVisible(bool bVisible = true);
		void ShowDevTool(BOOL bShow);
		void ClearHistory();
		void DoEvent(TEventUI& event);

		//浏览器功能
		//刷新
		void Refresh();
		//停止
		void Stop();
		//是否能够返回
		BOOL CanGoBack();
		//返回
		void GoBack();
		//是否能够前进
		BOOL CanGoForword();
		//前进
		void GoForward();
		//浏览URL
		void NavigateUrl(LPCTSTR lpszUrl);
		//浏览主页
		void NavigateHomePage();
		//执行JS函数
		BOOL ExecuteJsFunc(LPCTSTR lpszName, vector<VARIANT>& var, VARIANT& vRet);
		//执行Eval
		BOOL Eval(LPCTSTR lpszCode);
		//得到当前访问的URL
		BOOL GetUrl(TCHAR* pszUrl);

		//设置Javascript调用C++函数
		//在窗口处理WM_CEF_CREATE消息时调用
		void SetCreateJSFunction(fCreateJSFunc fcallback) {m_fCreateJSFunc = fcallback;};
		void SetJSCallBack(fJSCallBack fcallback) {m_fJScallback = fcallback;};
		void SetOnCustomScheme(fOnCustomScheme fcallback) {m_fCustomScheme = fcallback;};
		void SetNavStateChanged(fOnNavStateChanged fcallback) {m_fNavStateChanged = fcallback;};
		void SetBeforeBrowse(fOnBeforeBrowse fcallback) {m_fBeforeBrowse = fcallback;};
		void SetShowDevTool(fOnShowDevTool fcallback) {m_fShowDevTool = fcallback;};

		//js函数调用时
		bool OnExecute(const CefString& name, 
			CefRefPtr<CefV8Value> object, 
			const CefV8ValueList& arguments, 
			CefRefPtr<CefV8Value>& retval, 
			CefString& exception);

		BOOL OnContextCreated(OLECHAR*** var,int& nItem);

		void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);

		void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode);

		void OnAfterCreated(CefRefPtr<CefBrowser> browser);

		void OnCustomScheme(string url);

		BOOL CreateChromeWnd(HWND hParent, RECT& rc);

		void CloseBrowser();

		void OnNavStateChange(bool canGoBack, bool canGoForword);

		bool OnBeforeBrowse(TCHAR* pszUrl);

		void OnDOMContentLoaded();

		void SetNoPopup(BOOL bPopup);
		void SetStop(BOOL bStop);
		void SetLoadHide(BOOL bLoadHide);
		bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,	const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings);
		void SetInit();

	private:
		//主页
		CDuiString							m_sHomePage;
		CDuiString							m_sUrlCache;
		//出错页面URL
		CDuiString							m_sErrorPage;
		//CEF消息处理对象
		CefRefPtr<CChromeUIEventHandler>	m_pHandler;
		//JS回调函数
		fJSCallBack							m_fJScallback;
		//指定自定义JS函数的名称
		fCreateJSFunc						m_fCreateJSFunc;
		//自定义协议执行
		fOnCustomScheme						m_fCustomScheme;
		//浏览状态改变
		fOnNavStateChanged					m_fNavStateChanged;
		//即将跳转指定页面
		fOnBeforeBrowse						m_fBeforeBrowse;
		//是否显示窗口
		static fOnShowDevTool				m_fShowDevTool;
		//是否提供加载页面消息处理
		BOOL								m_bPopMessage;
		//是否启动时加载页面
		BOOL								m_bAutoNavi;
		BOOL								m_bPopup;
		BOOL								m_bLoadHide;
		BOOL								m_bStop;
	};
}
#endif