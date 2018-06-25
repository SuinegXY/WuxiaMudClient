#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H

//////////////////////////////////////////////////////////////////////////
//简单的HTTP通信类

#include <string>
#include <vector>
using namespace std;

#include "../curl/curl.h"

class HttpClient
{
public:
	HttpClient(void);
	~HttpClient(void);

public:
	void Post(const string& strUrl, const string& strPost,HWND hWnd);
	void Get(const string& strUrl,HWND hWnd);
    /*******************************************************
    * 函数名:Post
    * 函数功能:HTTP POST请求
    * 参数个数: 4
    **** @Param[IN]: strUrl ---- URL地址
    **** @Param[IN]: strPost ---- POST数据
    **** @Param[IN]: pfnWriteFunc ---- 接收响应的回调函数
    **** @Param[IN]: strResponse ---- 接收来自Web服务器的响应
    * 返回值 : CURLE_OK表示成功
              CURLE_OPERATION_TIMEDOUT 操作超时.
    *******************************************************/
	int Post(const string& strUrl, const string& strPost, curl_write_callback pfnWriteFunc,string& strResponse);

	    /*******************************************************
    * 函数名:GetHeader
    * 函数功能:HTTP GET请求获取Header
    * 参数个数: 4
    **** @Param[IN]: strUrl ---- URL地址
    **** @Param[IN]: strPost ---- POST数据
    **** @Param[IN]: pfnWriteFunc ---- 接收响应的回调函数
    **** @Param[IN]: strResponse ---- 接收来自Web服务器的响应
    * 返回值 : CURLE_OK表示成功
              CURLE_OPERATION_TIMEDOUT 操作超时.
    *******************************************************/
	int GetHeader(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse);

    /*******************************************************
    * 函数名:Get
    * 函数功能:HTTP GET请求
    * 参数个数: 3
    **** @Param[IN]: strUrl ---- URL地址请求地址
    **** @Param[IN]: pfnWriteFunc ---- 接收响应的回调函数
    **** @Param[IN]: strResponse ---- 接收来自Web服务器的响应
    * 返回值 : CURLE_OK表示成功
              CURLE_OPERATION_TIMEDOUT 操作超时.
    *******************************************************/
	int Get(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse);

#ifdef USE_OPENSSL
    /*******************************************************
    * 函数名:SSLPost
    * 函数功能:HTTPS POST请求
    * 参数个数: 4
    **** @Param[IN]: strUrl ---- URL地址
    **** @Param[IN]: strPost ---- POST数据
    **** @Param[IN]: pfnWriteFunc ---- 接收响应的回调函数
    **** @Param[IN]: strResponse ---- 接收来自Web服务器的响应
    * 返回值 : CURLE_OK表示成功
              CURLE_OPERATION_TIMEDOUT 操作超时.
    *******************************************************/
	int SSLPost(const string& strUrl, const string& strPost, curl_write_callback pfnWriteFunc,string& strResponse, const char* pCaPath = NULL);

    /*******************************************************
    * 函数名:SSLGet
    * 函数功能:HTTPS Get请求
    * 参数个数: 3
    **** @Param[IN]: strUrl ---- URL地址
    **** @Param[IN]: pfnWriteFunc ---- 接收响应的回调函数
    **** @Param[IN]: strResponse ---- 接收来自Web服务器的响应
    * 返回值 : CURLE_OK表示成功
              CURLE_OPERATION_TIMEDOUT 操作超时.
    *******************************************************/
	int SSLGet(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse, const char* pCaPath = NULL);
#endif

    /*******************************************************
    * 函数名:DownLoadFile
    * 函数功能:HTTP下载
    * 参数个数: 6
    **** @Param[IN]: strUrl ---- URL地址
    **** @Param[IN]: pfnWriteFunc ---- 接收响应的回调函数
    **** @Param[IN]: pFile ---- FILE文件句柄
    **** @Param[IN]: pfnProgFunc ---- 接收下载进度的回调函数
    **** @Param[IN]: pContext ---- 传递给进度回调函数的参数
    **** @Param[IN]: DownLoadTimeOut ---- 下载任务的超时时间
    * 返回值 : CURLE_OK表示成功
              CURLE_OPERATION_TIMEDOUT 操作超时.
    *******************************************************/
    int DownLoadFile(const string& strUrl,curl_write_callback pfnWriteFunc,FILE* pFile,curl_progress_callback pfnProgFunc,void* pContext,int DownLoadTimeOut = 600, unsigned long long llFileSize = 0);
	int DownloadFile(const char* pszUrl, const char* pszPath, BOOL bDelete = TRUE);
	string Get(const char* pszUrl);

public:
    /*******************************************************
    * 函数名:SetCallBackFunc
    * 函数功能:设置调试状态的回调
    * 参数个数: 2
    **** @Param[IN]: bDebug ---- 是否启用调试
    **** @Param[IN]: pfnDebug ---- 调试回调函数
    * 返回值:0表示成功.
    *******************************************************/
	void SetCallBackFunc(bool bDebug,curl_debug_callback pfnDebug);
	static size_t GetHeaderResponse(char *ptr, size_t size, size_t nmemb, void *userdata);

public:
    CURL* GetCurl() const {return m_pCurl;}

public:
    //static AutoLock m_CookieLock;
    static void   SaveCookieList(CURL* pCurl);
    static vector<string> m_vszCookies;   // Cookies信息

private:
    CURL* m_pCurl;                      // Curl对象指针
	bool  m_bDebug;                     // 是否启用调试
    curl_debug_callback m_pfnDebug;     // 调试回调函数
};

#endif // _HTTPCLIENT_H