//#pragma once
#include "include/cef_client.h"
#include "include/cef_task.h"


//Javascript使用的对象名称 该对象隶属于全局对象window
//所有提供给Javascript使用的接口函数均在该对象中保存
//使用方法window.external.MyFunc(var1, var2, var3)
#define		STNTS_CEFTOJS_OBJECT		"external"

//CEF创建消息
//针对CEF的操作需要在该消息触发时或之后调用，否则无法成功
#define		WM_CEF_CREATE			WM_USER + 0x1000
#define		WM_CEF_LOADEND			WM_USER + 0x1001
#define		WM_CEF_LOADSTART		WM_USER + 0x1002
#define		WM_CEF_CONTEXTCREATE	WM_USER + 0x1003
#define		WM_CEF_DOMCONTENTLOADED	WM_USER + 0x1004	//对应javascript document ready事件

namespace DuiLib
{
	class CChromeUI;
	class CChromeEventHandler 
		: public CefClient
		, public CefDisplayHandler
		, public CefLifeSpanHandler
		, public CefLoadHandler
		, public CefRequestHandler
		, public CefFocusHandler
		, public CefKeyboardHandler
		, public CefMenuHandler
		, public CefPermissionHandler
		, public CefPrintHandler
		, public CefFindHandler
		, public CefJSDialogHandler
		, public CefV8ContextHandler
		, public CefRenderHandler
		, public CefDragHandler
		, public CefGeolocationHandler
		, public CefZoomHandler
	{
	public:
		CChromeEventHandler() {}
		~CChromeEventHandler() {}

		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefMenuHandler> GetMenuHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefPermissionHandler> GetPermissionHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefPrintHandler> GetPrintHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefFindHandler> GetFindHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefV8ContextHandler> GetV8ContextHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE {return this;};
		virtual CefRefPtr<CefZoomHandler> GetZoomHandler() OVERRIDE {return this;};

		// CefLifeSpanHandler methods
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			const CefString& url,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings)
		{
			std::string urlStr = url;
			//打开开发者工具
			if (urlStr.find("chrome-devtools:") == urlStr.npos) 
			{
				ShellExecute(NULL, _T("open"), url.c_str(), NULL, NULL, SW_SHOWNORMAL);
				return true;
			}
			return false;
		}

		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser){
			AutoLock lockscope(this);
			if (!m_pBrowser.get())
			{
				m_pBrowser = browser;
				m_hBrowserWnd = browser->GetWindowHandle();
				HWND hParent = GetParent(m_hBrowserWnd);
				::PostMessage(hParent, WM_CEF_CREATE, 0, 0);
			}
		}
		virtual bool DoClose(CefRefPtr<CefBrowser> browser)
		{
			return false;
		}
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) {
			if (m_hBrowserWnd == browser->GetWindowHandle())
			{
				m_pBrowser = NULL;
			}
		}

		// CefLoadHandler methods
		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame){}
		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) {}
		virtual bool OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& failedUrl,
			CefString& errorText) 
		{	
			return false;
		}

		// CefRequestHandler methods
		virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, 
			CefRefPtr<CefFrame> frame, 
			CefRefPtr<CefRequest> request, 
			NavType navType, 
			bool isRedirect)
		{
			return false;
		}

		virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefRequest> request,
			CefString& redirectUrl,
			CefRefPtr<CefStreamReader>& resourceStream,
			CefRefPtr<CefResponse> response,
			int loadFlags) 
		{
			return false;
		}
		virtual bool GetDownloadHandler(CefRefPtr<CefBrowser> browser,
			const CefString& mimeType,
			const CefString& fileName,
			int64 contentLength,
			CefRefPtr<CefDownloadHandler>& handler)
		{
			return true;
		}

		// CefDisplayHandler methods
		virtual void OnNavStateChange(CefRefPtr<CefBrowser> browser,
			bool canGoBack,
			bool canGoForward) {}
		virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& url) {}
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title) {}
		virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
			const CefString& message,
			const CefString& source,
			int line) 
		{
			return false;
		}

		// CefFocusHandler methods.
		virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefDOMNode> node){}

		// CefKeyboardHandler methods.
		virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
			KeyEventType type,
			int code,
			int modifiers,
			bool isSystemKey,
			bool isAfterJavaScript)
		{
			return false;
		}

		// CefPrintHandler methods.
		virtual bool GetPrintHeaderFooter(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefPrintInfo& printInfo,
			const CefString& url,
			const CefString& title,
			int currentPage,
			int maxPages,
			CefString& topLeft,
			CefString& topCenter,
			CefString& topRight,
			CefString& bottomLeft,
			CefString& bottomCenter,
			CefString& bottomRight)
		{
			return false;
		}

		// CefV8ContextHandler methods
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context)
		{
		}

		virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, 
			CefRefPtr<CefFrame> frame, 
			CefRefPtr<CefV8Context> context)
		{
		}

		// CefDragHandler methods.
		virtual bool OnDragStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> dragData,
			DragOperationsMask mask)
		{
			return true;
		}

		virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> dragData,
			DragOperationsMask mask)
		{
			return true;
		}

		// CefPermissionHandler methods.
		virtual bool OnBeforeScriptExtensionLoad(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& extensionName)
		{
			return false;
		}

		// CefGeolocationHandler methods.
		virtual void OnRequestGeolocationPermission(
			CefRefPtr<CefBrowser> browser,
			const CefString& requesting_url,
			int request_id,
			CefRefPtr<CefGeolocationCallback> callback){}

		// CefMenuHandler methods
		///
		// Called before a context menu is displayed. Return false to display the
		// default context menu or true to cancel the display.
		///
		/*--cef()--*/
		virtual bool OnBeforeMenu(CefRefPtr<CefBrowser> browser,
			const CefMenuInfo& menuInfo) { return true; }

		///
		// Called to optionally override the default text for a context menu item.
		// |label| contains the default text and may be modified to substitute
		// alternate text.
		///
		/*--cef()--*/
		virtual void GetMenuLabel(CefRefPtr<CefBrowser> browser,
			MenuId menuId,
			CefString& label) {}

		///
		// Called when an option is selected from the default context menu. Return
		// false to execute the default action or true to cancel the action.
		///
		/*--cef()--*/
		virtual bool OnMenuAction(CefRefPtr<CefBrowser> browser,                                                                                                      
			MenuId menuId) { return false; }

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
		{
			return false;
		}

		CefRefPtr<CefBrowser> m_pBrowser;
		HWND m_hBrowserWnd;

		CefRefPtr<CefBrowser> GetBrowser() {return m_pBrowser;};
		HWND	GetBrowserWnd() {return m_hBrowserWnd;};

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(CChromeEventHandler);
		// Include the default locking implementation.
		IMPLEMENT_LOCKING(CChromeEventHandler);
	};
}