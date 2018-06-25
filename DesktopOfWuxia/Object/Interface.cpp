#include "stdafx.h"
#include "Define.h"
#include "Interface.h"
#include "Util\MD5.h"
#include "Util\HttpClient.h"
#include "Util\STAES.h"

//#define NO_AES

string CInterface::Get( const char* pszUrl, const char* pszFormat, ... )
{
	char szBuffer[1024] = {0};
	va_list args;
	va_start(args, pszFormat);
	vsprintf(szBuffer, pszFormat, args);
	va_end(args);
	string strParam = GetParam(szBuffer);
	string strCallback;
	HttpClient http;
	if ( http.Get(string(pszUrl)+strParam, NULL, strCallback) != CURLE_OK )
	{
		strCallback = "";
	}
	return strCallback;
}

string CInterface::Post( const char* pszUrl, const char* pszFormat, ... )
{
	char szBuffer[1024] = {0};
	va_list args;
	va_start(args, pszFormat);
	vsprintf(szBuffer, pszFormat, args);
	va_end(args);
	string strParam = GetParam(szBuffer);
	string strCallback;
	HttpClient http;
	if ( http.Post(pszUrl, strParam, NULL, strCallback) != CURLE_OK )
	{
		strCallback = "";
	}
	return strCallback;
}

string CInterface::GetParam( const char* pszParam )
{
	string strParam = string("app_id=wuxia&") + pszParam;
	return GetSign(strParam.c_str());
}

string CInterface::GetSign( const char* pszContent )
{
	string strParam = pszContent;
	string strSign = strParam + AES_KEY;
	strSign = CMD5::MD5String(strSign.c_str());
	transform(strSign.begin(), strSign.end(), strSign.begin(), ::tolower);
	strParam = strParam + "&sign=" + strSign;
	return strParam;
}

string CInterface::AESCode( const char* pszSource )
{
#ifndef NO_AES
	CAES aes;
	char* szBuffer;
	aes.AESEncryptOutputBase64Code((void*)pszSource, strlen(pszSource), &szBuffer, AES_KEY, strlen(AES_KEY));
	string strAesCode = szBuffer;
	free(szBuffer);
	return strAesCode;
#else
	return pszSource;
#endif
}
