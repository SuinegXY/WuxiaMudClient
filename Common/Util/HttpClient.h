#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H

//////////////////////////////////////////////////////////////////////////
//�򵥵�HTTPͨ����

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
    * ������:Post
    * ��������:HTTP POST����
    * ��������: 4
    **** @Param[IN]: strUrl ---- URL��ַ
    **** @Param[IN]: strPost ---- POST����
    **** @Param[IN]: pfnWriteFunc ---- ������Ӧ�Ļص�����
    **** @Param[IN]: strResponse ---- ��������Web����������Ӧ
    * ����ֵ : CURLE_OK��ʾ�ɹ�
              CURLE_OPERATION_TIMEDOUT ������ʱ.
    *******************************************************/
	int Post(const string& strUrl, const string& strPost, curl_write_callback pfnWriteFunc,string& strResponse);

	    /*******************************************************
    * ������:GetHeader
    * ��������:HTTP GET�����ȡHeader
    * ��������: 4
    **** @Param[IN]: strUrl ---- URL��ַ
    **** @Param[IN]: strPost ---- POST����
    **** @Param[IN]: pfnWriteFunc ---- ������Ӧ�Ļص�����
    **** @Param[IN]: strResponse ---- ��������Web����������Ӧ
    * ����ֵ : CURLE_OK��ʾ�ɹ�
              CURLE_OPERATION_TIMEDOUT ������ʱ.
    *******************************************************/
	int GetHeader(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse);

    /*******************************************************
    * ������:Get
    * ��������:HTTP GET����
    * ��������: 3
    **** @Param[IN]: strUrl ---- URL��ַ�����ַ
    **** @Param[IN]: pfnWriteFunc ---- ������Ӧ�Ļص�����
    **** @Param[IN]: strResponse ---- ��������Web����������Ӧ
    * ����ֵ : CURLE_OK��ʾ�ɹ�
              CURLE_OPERATION_TIMEDOUT ������ʱ.
    *******************************************************/
	int Get(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse);

#ifdef USE_OPENSSL
    /*******************************************************
    * ������:SSLPost
    * ��������:HTTPS POST����
    * ��������: 4
    **** @Param[IN]: strUrl ---- URL��ַ
    **** @Param[IN]: strPost ---- POST����
    **** @Param[IN]: pfnWriteFunc ---- ������Ӧ�Ļص�����
    **** @Param[IN]: strResponse ---- ��������Web����������Ӧ
    * ����ֵ : CURLE_OK��ʾ�ɹ�
              CURLE_OPERATION_TIMEDOUT ������ʱ.
    *******************************************************/
	int SSLPost(const string& strUrl, const string& strPost, curl_write_callback pfnWriteFunc,string& strResponse, const char* pCaPath = NULL);

    /*******************************************************
    * ������:SSLGet
    * ��������:HTTPS Get����
    * ��������: 3
    **** @Param[IN]: strUrl ---- URL��ַ
    **** @Param[IN]: pfnWriteFunc ---- ������Ӧ�Ļص�����
    **** @Param[IN]: strResponse ---- ��������Web����������Ӧ
    * ����ֵ : CURLE_OK��ʾ�ɹ�
              CURLE_OPERATION_TIMEDOUT ������ʱ.
    *******************************************************/
	int SSLGet(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse, const char* pCaPath = NULL);
#endif

    /*******************************************************
    * ������:DownLoadFile
    * ��������:HTTP����
    * ��������: 6
    **** @Param[IN]: strUrl ---- URL��ַ
    **** @Param[IN]: pfnWriteFunc ---- ������Ӧ�Ļص�����
    **** @Param[IN]: pFile ---- FILE�ļ����
    **** @Param[IN]: pfnProgFunc ---- �������ؽ��ȵĻص�����
    **** @Param[IN]: pContext ---- ���ݸ����Ȼص������Ĳ���
    **** @Param[IN]: DownLoadTimeOut ---- ��������ĳ�ʱʱ��
    * ����ֵ : CURLE_OK��ʾ�ɹ�
              CURLE_OPERATION_TIMEDOUT ������ʱ.
    *******************************************************/
    int DownLoadFile(const string& strUrl,curl_write_callback pfnWriteFunc,FILE* pFile,curl_progress_callback pfnProgFunc,void* pContext,int DownLoadTimeOut = 600, unsigned long long llFileSize = 0);
	int DownloadFile(const char* pszUrl, const char* pszPath, BOOL bDelete = TRUE);
	string Get(const char* pszUrl);

public:
    /*******************************************************
    * ������:SetCallBackFunc
    * ��������:���õ���״̬�Ļص�
    * ��������: 2
    **** @Param[IN]: bDebug ---- �Ƿ����õ���
    **** @Param[IN]: pfnDebug ---- ���Իص�����
    * ����ֵ:0��ʾ�ɹ�.
    *******************************************************/
	void SetCallBackFunc(bool bDebug,curl_debug_callback pfnDebug);
	static size_t GetHeaderResponse(char *ptr, size_t size, size_t nmemb, void *userdata);

public:
    CURL* GetCurl() const {return m_pCurl;}

public:
    //static AutoLock m_CookieLock;
    static void   SaveCookieList(CURL* pCurl);
    static vector<string> m_vszCookies;   // Cookies��Ϣ

private:
    CURL* m_pCurl;                      // Curl����ָ��
	bool  m_bDebug;                     // �Ƿ����õ���
    curl_debug_callback m_pfnDebug;     // ���Իص�����
};

#endif // _HTTPCLIENT_H