#pragma once

#include <string>
#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

#if defined(_UNICODE) || defined(UNICODE)
#define TtoA WtoA
#define AtoT AtoW
#define WtoT(unHex) (unHex)
#define TtoW(unHex) (unHex)
typedef std::wstring _tstring;
#else
#define TtoA(unHex) (unHex)
#define AtoT(unHex) (unHex)
#define WtoT WtoA
#define TtoW AtoW
typedef std::string _tstring;
#endif

std::string WtoA(const wchar_t* pwszSrc);
std::string WtoA(const std::wstring &strSrc);

std::wstring AtoW(const char* pszSrc);
std::wstring AtoW(const std::string &strSrc);

std::string WtoUTF8(const wchar_t* pwszSrc);
std::string WtoUTF8(const std::wstring &strSrc);

std::wstring UTF8toW(const char* pszSrc);
std::wstring UTF8toW(const std::string &strSr);

std::string AtoUTF8(const char* src);
std::string AtoUTF8(const std::string &src);

std::string UTF8toA(const char* src);
std::string UTF8toA(const std::string &src);
std::string Utf8Decode(const std::string& strSrc);

std::string UrlEncode(const char* pszContent);
std::string UrlDecode(const char* pszContent);

int IsTextUTF8(const char* pszSrc); 

string GetStringHead(string& strContant, const char* pszSepa);
wstring GetStringHeadW(wstring& strContant, const WCHAR* pszSepa);
wstring GetStringSepa(wstring& strContant, const WCHAR* pszSepa);
int GetStringLength(const char* pszContant);
int GetStringLengthW(const WCHAR* pszContant);
wstring NumToCnString(int nNum);
