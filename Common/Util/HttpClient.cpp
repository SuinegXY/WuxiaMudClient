#include "stdafx.h"
#include "HttpClient.h"
#include <io.h>


#define CONNECT_TIME_OUT   (10)         // 连接超时时间(秒)
#define DNSCACHE_TIME_OUT  (259200)     // DNS缓存时间(60*60*72)秒

vector<string> HttpClient::m_vszCookies;

size_t GetWriteResponse(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string* pStrRes = (string*)userdata;
	if (NULL == pStrRes || NULL == ptr || 0 == size*nmemb)
	{
		return -1;
	}
	size_t resSize = size*nmemb;
	pStrRes->append(ptr, resSize);
	return resSize;
}

size_t DownLoadWriteFunc(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	if ( userdata )
	{
		FILE *fp = (FILE*)userdata;
		size_t written = fwrite(ptr, size, nmemb, fp);
		return written;
	}
	else
	{
		return 0;
	}
}

HttpClient::HttpClient(void)
: m_bDebug(false)
, m_pfnDebug(NULL)
, m_pCurl(NULL)
{
    // CURL初始化
    curl_global_init(CURL_GLOBAL_ALL);
    m_pCurl = curl_easy_init();
}

HttpClient::~HttpClient(void)
{
    // CURL释放
    curl_easy_cleanup(m_pCurl);
    curl_global_cleanup();
}

int HttpClient::Post(const string& strUrl, const string& strPost, curl_write_callback pfnWriteFunc,string& strResponse)
{
    // 如果没有初始化,则首先初始化CURL
	if(NULL == m_pCurl)
	{
        m_pCurl = curl_easy_init();
        if(NULL == m_pCurl)
		    return CURLE_FAILED_INIT;
	}

	if ( NULL == pfnWriteFunc )
	{
		pfnWriteFunc = &GetWriteResponse;
	}

    // 设置是否启用调试
	if(m_bDebug && NULL != m_pfnDebug)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, m_pfnDebug);
	}

	curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());                 // 设置URL地址
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE,"");
    if(!m_vszCookies.empty())
    {
        for (int i=0;i<(int)m_vszCookies.size();i++)
        {
            curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST,m_vszCookies[0].c_str());
        }
    }
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);                             // 设置POST方式
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, strPost.c_str());         // 设置POST数据
	curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, NULL);                  // 不需要读回调
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, pfnWriteFunc);         // 设置写回调
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void*)&strResponse);      // 接收响应数据的参数
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1);                         //
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);    // 连接等待超时
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CONNECT_TIME_OUT);           // 传输超时
    curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, DNSCACHE_TIME_OUT);// 缓存DNS(72小时)

    // 执行
    CURLcode res;
	res = curl_easy_perform(m_pCurl);

	return res;
}

int HttpClient::GetHeader(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse)
{
	if(NULL == m_pCurl)
	{
		m_pCurl = curl_easy_init();
		if(NULL == m_pCurl)
			return CURLE_FAILED_INIT;
	}

	if ( NULL == pfnWriteFunc )
	{
		pfnWriteFunc = &GetWriteResponse;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, m_pfnDebug);
	}
	curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE,"");
	if(!m_vszCookies.empty())
	{
		for (int i=0;i<(int)m_vszCookies.size();i++)
		{
			curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST,m_vszCookies[0].c_str());
		}
	}
	curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, pfnWriteFunc);
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERDATA, (void*)&strResponse);
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CONNECT_TIME_OUT);
	curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, DNSCACHE_TIME_OUT);// 缓存DNS(72小时)

	CURLcode res;
	res = curl_easy_perform(m_pCurl);
	if(res == CURLE_OK)
		SaveCookieList(m_pCurl);

	return res;
}

int HttpClient::Get(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse)
{
    if(NULL == m_pCurl)
    {
        m_pCurl = curl_easy_init();
        if(NULL == m_pCurl)
            return CURLE_FAILED_INIT;
    }

	if ( NULL == pfnWriteFunc )
	{
		pfnWriteFunc = &GetWriteResponse;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, m_pfnDebug);
	}
    curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE,"");
    if(!m_vszCookies.empty())
    {
        for (int i=0;i<(int)m_vszCookies.size();i++)
        {
            curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST,m_vszCookies[0].c_str());
        }
    }
	curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, pfnWriteFunc);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void*)&strResponse);
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CONNECT_TIME_OUT);
    curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, DNSCACHE_TIME_OUT);// 缓存DNS(72小时)

    CURLcode res;
	res = curl_easy_perform(m_pCurl);
    if(res == CURLE_OK)
        SaveCookieList(m_pCurl);

	return res;
}

string HttpClient::Get( const char* pszUrl )
{
	string strCallback;
	if ( Get(pszUrl, NULL, strCallback) == CURLE_OK )
	{
		return strCallback;
	}
	return "";
}

#ifdef USE_OPENSSL
int HttpClient::SSLPost(const string& strUrl, const string& strPost, curl_write_callback pfnWriteFunc,
                        string& strResponse, const char* pCaPath/* = NULL*/)
{
    if(NULL == m_pCurl)
    {
        m_pCurl = curl_easy_init();
        if(NULL == m_pCurl)
            return CURLE_FAILED_INIT;
    }

	if ( NULL == pfnWriteFunc )
	{
		pfnWriteFunc = &GetWriteResponse;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, m_pfnDebug);
	}

	curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE,"");
    if(!m_vszCookies.empty())
    {
        for (int i=0;i<(int)m_vszCookies.size();i++)
        {
            curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST,m_vszCookies[0].c_str());
        }
    }
	else
		curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST, "ALL");
	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, pfnWriteFunc);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	else
	{
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(m_pCurl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CONNECT_TIME_OUT);
    curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, DNSCACHE_TIME_OUT);// 缓存DNS(72小时)

    CURLcode res = CURLE_OK;
	res = curl_easy_perform(m_pCurl);
    if(res == CURLE_OK)
        SaveCookieList(m_pCurl);

	return res;
}

int HttpClient::SSLGet(const string& strUrl, curl_write_callback pfnWriteFunc,string& strResponse, const char* pCaPath/* = NULL*/)
{
    if(NULL == m_pCurl)
    {
        m_pCurl = curl_easy_init();
        if(NULL == m_pCurl)
            return CURLE_FAILED_INIT;
    }

	if ( NULL == pfnWriteFunc )
	{
		pfnWriteFunc = &GetWriteResponse;
	}

	if(m_bDebug)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, m_pfnDebug);
	}

	curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE,"");
    if(!m_vszCookies.empty())
    {
        for (int i=0;i<(int)m_vszCookies.size();i++)
        {
            curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST,m_vszCookies[0].c_str());
        }
    }
	curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, pfnWriteFunc);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(m_pCurl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CONNECT_TIME_OUT);
    curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, DNSCACHE_TIME_OUT);// 缓存DNS(72小时)

    CURLcode res = CURLE_OK;
	res = curl_easy_perform(m_pCurl);
    if(res == CURLE_OK)
        SaveCookieList(m_pCurl);

	return res;
}
#endif

size_t HttpClient::GetHeaderResponse(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return size * nmemb;
}

int HttpClient::DownLoadFile(const string& strUrl,curl_write_callback pfnWriteFunc,FILE* pFile,
                             curl_progress_callback pfnProgFunc,void* pContext,int DownLoadTimeOut/* = 600*/,
							 unsigned long long llFileSize )
{
    if(NULL == m_pCurl)
    {
        m_pCurl = curl_easy_init();
        if(NULL == m_pCurl)
            return CURLE_FAILED_INIT;
    }

	if ( NULL == pfnWriteFunc )
	{
		pfnWriteFunc = &DownLoadWriteFunc;
	}

    if(m_bDebug && NULL != m_pfnDebug)
    {
        curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, m_pfnDebug);
    }

    if(pfnProgFunc)
    {
        curl_easy_setopt(m_pCurl,CURLOPT_NOPROGRESS,0);                     // 启用进度回调
        curl_easy_setopt(m_pCurl,CURLOPT_PROGRESSFUNCTION,pfnProgFunc);     // 设置进度回调函数
    }

	if ( pContext )
	{
		curl_easy_setopt(m_pCurl,CURLOPT_PROGRESSDATA,pContext);            // 传入回调函数的参数
	}

	// 获取Http Header信息
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, GetHeaderResponse);
    curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE,"");
    if(!m_vszCookies.empty())
    {
        for (int i=0;i<(int)m_vszCookies.size();i++)
        {
            curl_easy_setopt(m_pCurl,CURLOPT_COOKIELIST,m_vszCookies[0].c_str());
        }
    }
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_easy_setopt(m_pCurl, CURLOPT_FAILONERROR,TRUE);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, pfnWriteFunc);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, (void*)pFile);
	//curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1);
	//curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, true);
	//curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);
    curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, DownLoadTimeOut);
	curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, DNSCACHE_TIME_OUT); // 缓存DNS(72小时)
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 5);

	if ( llFileSize )
	{
		unsigned long long llSize = 0;
		int nError = curl_easy_getinfo(m_pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &llSize);

		if ( llFileSize < llSize )
		{
			char szRange[12] = {0};
			sprintf_s(szRange, "%u-", llFileSize);
			curl_easy_setopt(m_pCurl, CURLOPT_RANGE, szRange);
		}
		else
		{
			return CURLE_OK;
		}
	}

    CURLcode res;
    res = curl_easy_perform(m_pCurl);
    if(res == CURLE_OK)
        SaveCookieList(m_pCurl);

    return res;
}


void HttpClient::SetCallBackFunc(bool bDebug,curl_debug_callback pfnDebug)
{
    m_bDebug = m_bDebug;

    if(m_bDebug)
        m_pfnDebug = pfnDebug;
}

void HttpClient::SaveCookieList(CURL* pCurl)
{
    if(NULL != pCurl)
    {
        CURLcode res;
        struct curl_slist* cookies = NULL;
        struct curl_slist* nc = NULL;

        // 获取CookList(NetScape/Mozilla格式)
        res = curl_easy_getinfo(pCurl, CURLINFO_COOKIELIST, &cookies);
        if (res != CURLE_OK) 
        {
            return;
        }
        if(cookies != NULL)
            m_vszCookies.clear();

        nc = cookies;
        while (nc) 
        {
            m_vszCookies.push_back(nc->data);
            nc = nc->next;
        }

        curl_slist_free_all(cookies);
    }
}

int HttpClient::DownloadFile( const char* pszUrl, const char* pszPath, BOOL bDelete )
{
	int nErrorTimes = 0;
	int bRes = -1;
	do
	{
		long long llFileSize = 0;
		if ( access(pszPath, 0) == 0 )
		{
			if ( bDelete )
			{
				DeleteFileA(pszPath);
			}
			else
			{
				WIN32_FILE_ATTRIBUTE_DATA data;
				if (GetFileAttributesExA(pszPath, GetFileExInfoStandard, &data))
				{
					llFileSize = data.nFileSizeHigh;

					llFileSize <<= sizeof(DWORD)*8;
					llFileSize += data.nFileSizeLow;
				}
			}
		}

		FILE* pFile = fopen(pszPath, "wb+");
		bRes = DownLoadFile(pszUrl, NULL, pFile, NULL, NULL, 600, llFileSize);
		fclose(pFile);
		if ( bRes == CURLE_OK ) break;
		if ( bDelete )
		{
			DeleteFileA(pszPath);
		}
		nErrorTimes ++;
		if ( nErrorTimes > 5 )
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}
	while ( bRes != CURLE_OK );
	return bRes;
}
