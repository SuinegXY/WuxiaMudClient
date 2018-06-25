
#pragma once

#include <atlbase.h>

#ifndef NO_DUILIB
#include "../DuiLib/UIlib.h"
using namespace DuiLib;
#else
#include <tchar.h>
#include <string>
using namespace std;
#endif

#define		GET_ARRAY_LEN(array)	sizeof(array) / sizeof(array[0])

HMODULE GetCurrentModule();
HWND GetFocusWnd();
void CenterWindowEx(HWND hwnd);
DWORD GetProcessIdByName(LPCTSTR lpFilename);
string GetProcessPathByName(LPCTSTR lpFilename);
bool KillProcessEx(const TCHAR* pwszName);
void KillSvchost();
wstring UTF8ToUnicode( const char* strSrc );
string UnicodeToAnsi( const WCHAR* strSrc );
wstring AnsiToUnicode( const char* strSrc );
string AnsiToUTF8( const char* strSrc );
wstring Utf8ToUnicode(const char* buf);
string GetTrueLocalPath();//½áÎ²²»´ø\  
int KillProcess(DWORD dwId);
HWND FindWindowExceptWnd(HWND hWnd, const WCHAR* pwszClassName, const WCHAR* pwszWindowName);
BOOL GetRegValue(HKEY hRoot, const char* pszPath, const char* pszName, DWORD nType, void* pValue, DWORD dwSize);
void GetWinClientRect( RECT& _rect );

int ReplaceFileContent(const char* pszFileName, const char* pszOldContent, const char* pszNewContent);
void RunCmdAndWait( const char* pszCmd );
void RunCmdAndNoWait( const char* pszCmd );
BOOL CopyFileToPath( const char* pszExistPath, const char* pszNewPath, const char* pszFileName, BOOL bFailIfExist );
void CopyDirectory(const char* pszExistPath, const char* pszNewPath);

string ReadFileContent( const char* pszFilePath);

wstring GetAnsiObjectName(const char* pszNameHead, const char* pszNameEnd);
