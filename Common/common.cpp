#include "stdafx.h"
#include "common.h"
#include <windows.h>
#include <tlhelp32.h>
#include <io.h>
#include "Util/stdstring.h"
#include <WtsApi32.h>
#pragma comment(lib, "WtsApi32.lib")
#include <UserEnv.h>
#pragma comment(lib, "Userenv.lib")

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")
#include <direct.h>

#include "Util/HttpClient.h"
#include "Util/ATW.h"
#include "Logger.h"
#include "statistic/ConfigManager.h"

wstring Utf8ToUnicode(const char* buf)
{
	int len = ::MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
	if (len == 0) return L"";

	std::vector<wchar_t> unicode(len);
	::MultiByteToWideChar(CP_UTF8, 0, buf, -1, &unicode[0], len);

	return &unicode[0];
}

void CenterWindowEx(HWND hwnd)
{ 
   RECT rcParent, rc;
   GetClientRect(hwnd, &rc);
   HWND hParent = GetParent(hwnd);
   if( hParent==NULL ) hParent = GetDesktopWindow();
   GetClientRect(hParent, &rcParent);
   OffsetRect(&rc, -rc.left, -rc.top);
   OffsetRect(&rcParent, -rcParent.left, -rcParent.top);
   MoveWindow(hwnd, (rcParent.right-rc.right)/2, (rcParent.bottom-rc.bottom)/2, rc.right, rc.bottom, TRUE);
}

string AnsiToUTF8( const char* strSrc )
{
	string strGBK = strSrc;
	string strOutUTF8 = "";  
	WCHAR* str1;  
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
	str1 = new WCHAR[n];  
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  
	char* str2 = new char[n];  
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  
	strOutUTF8 = str2;  
	delete[]str1;  
	str1 = NULL;  
	delete[]str2;  
	str2 = NULL;  
	return strOutUTF8;  
}

string UnicodeToAnsi( const WCHAR* strSrc )
{
	string	strRet;
	char*	pBuff	= NULL;
	int		nCount	= 0;

	nCount = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	if (0 == nCount)
	{
		strRet = "";
		goto __Error_Exit;
	}

	pBuff = new char[nCount+1];
	if (NULL == pBuff)
	{
		strRet = "";
		goto __Error_Exit;
	}

	nCount = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, pBuff, nCount, NULL, NULL);
	if (0 == nCount)
	{
		strRet = "";
		goto __Error_Exit;
	}

	strRet = pBuff;

__Error_Exit:
	if (NULL != pBuff)
	{
		delete []pBuff;
	}

	return strRet;
}

wstring AnsiToUnicode( const char* strSrc )
{
	wstring	wstrRet;
	WCHAR*	pBuff = NULL;
	int		nCount = 0;

	nCount = MultiByteToWideChar(CP_ACP, 0, strSrc, -1, NULL, 0) * sizeof(WCHAR);
	if (0 == nCount)
	{
		wstrRet = L"";
		goto __Error_Exit;
	}

	pBuff = new WCHAR[nCount+1];
	if (NULL == pBuff)
	{
		wstrRet = L"";
		goto __Error_Exit;
	}

	nCount = MultiByteToWideChar(CP_ACP, 0, strSrc, -1, pBuff, nCount);
	if (0 == nCount)
	{
		wstrRet = L"";
		goto __Error_Exit;
	}

	wstrRet = pBuff;

__Error_Exit:
	if (NULL != pBuff)
	{
		delete []pBuff;
	}

	return wstrRet;
}

wstring UTF8ToUnicode( const char* strSrc )
{
	wstring	wstrRet;
	WCHAR*	pBuff = NULL;
	int		nCount = 0;

	nCount = MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, NULL, 0) * sizeof(WCHAR);
	if (0 == nCount)
	{
		wstrRet = L"";
		goto __Error_Exit;
	}

	pBuff = new WCHAR[nCount+1];
	if (NULL == pBuff)
	{
		wstrRet = L"";
		goto __Error_Exit;
	}

	nCount = MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, pBuff, nCount);
	if (0 == nCount)
	{
		wstrRet = L"";
		goto __Error_Exit;
	}

	wstrRet = pBuff;

__Error_Exit:
	if (NULL != pBuff)
	{
		delete []pBuff;
	}

	return wstrRet;
}

DWORD GetProcessIdByName(LPCTSTR lpFilename)
{
	DWORD dwProcessId = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
	{
		return 0;
	}
	if(!Process32First(hProcessSnap,&pe32))
	{
		CloseHandle(hProcessSnap);
		return 0;
	}

	do
	{
		if(lstrcmpi(pe32.szExeFile,lpFilename) == 0)
		{
			dwProcessId = pe32.th32ProcessID;
			break;
		}
	}
	while(Process32Next(hProcessSnap,&pe32));

	CloseHandle(hProcessSnap);

	return dwProcessId;
}

string GetProcessPathByName( LPCTSTR lpFilename )
{
	TCHAR szLog[500] = {0};
	string strPath;
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
	{
		DWORD dwError = GetLastError();
		wsprintf(szLog,_T("CreateToolhelp32Snapshot Error = %d\n"),dwError);
		OutputDebugString(szLog);
		return 0;
	}
	if(!Process32First(hProcessSnap,&pe32))
	{
		DWORD dwError = GetLastError();
		wsprintf(szLog,_T("Process32First Error = %d\n"),dwError);
		OutputDebugString(szLog);
		CloseHandle(hProcessSnap);
		return 0;
	}

	do
	{
		if(lstrcmpi(pe32.szExeFile,lpFilename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, pe32.th32ProcessID);  
			char szBuffer[MAX_PATH];
			HMODULE hModule;
			DWORD dwNeeded;
			if ( ::EnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwNeeded) )
			{
				GetModuleFileNameExA((HMODULE)hProcess, hModule, szBuffer, MAX_PATH);
				strPath = szBuffer;
			}
			break;
		}
	}
	while(Process32Next(hProcessSnap,&pe32));

	CloseHandle(hProcessSnap);

	return strPath;
}

void KillSvchost()
{
	int nID[0xffff] = {0};
	TCHAR szLog[500] = {0};
	DWORD dwProcessId = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
	{
		DWORD dwError = GetLastError();
		wsprintf(szLog,_T("CreateToolhelp32Snapshot Error = %d\n"),dwError);
		OutputDebugString(szLog);
		return ;
	}
	if(!Process32First(hProcessSnap,&pe32))
	{
		DWORD dwError = GetLastError();
		wsprintf(szLog,_T("Process32First Error = %d\n"),dwError);
		OutputDebugString(szLog);
		CloseHandle(hProcessSnap);
		return ;
	}
	do
	{
		if(lstrcmpi(pe32.szExeFile, L"svchost.exe") == 0)
		{
			nID[pe32.th32ParentProcessID]++;
		}
	}
	while(Process32Next(hProcessSnap,&pe32));
	CloseHandle(hProcessSnap);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) 
	{
		DWORD dwError = GetLastError();
		wsprintf(szLog,_T("CreateToolhelp32Snapshot Error = %d\n"),dwError);
		OutputDebugString(szLog);
		return ;
	}
	if(!Process32First(hProcessSnap,&pe32))
	{
		DWORD dwError = GetLastError();
		wsprintf(szLog,_T("Process32First Error = %d\n"),dwError);
		OutputDebugString(szLog);
		CloseHandle(hProcessSnap);
		return ;
	}
	do
	{
		if(lstrcmpi(pe32.szExeFile, L"svchost.exe") == 0)
		{
			if ( nID[pe32.th32ParentProcessID] == 1 )
			{
				KillProcess(pe32.th32ProcessID);
			}
		}
	}
	while(Process32Next(hProcessSnap,&pe32));
	CloseHandle(hProcessSnap);
}

int KillProcess(DWORD dwId)   //根据进程ID杀进程  
{  
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwId);  
	if ( hProcess == NULL ) 
	{  
		return -1;  
	}  
	TerminateProcess(hProcess, 0);  
	return -1;  
}

bool KillProcessEx(const TCHAR* pwszName)
{
	int nTimes = 0;
	DWORD dwProcessId = GetProcessIdByName(pwszName);
	if ( dwProcessId == 0 )
	{
		return false;
	}
	else while ( dwProcessId )
	{
		nTimes ++;
		if ( nTimes > 5 )
		{
			return false;
		}
		KillProcess(dwProcessId);
		Sleep(10);
		dwProcessId = GetProcessIdByName(pwszName);
	}
	return true;
}

HMODULE GetCurrentModule()
{
	return reinterpret_cast<HMODULE>(&__ImageBase);
}

string GetTrueLocalPath()
{
	static CStdStringA strPath = "";
	if ( strPath.length() == 0 )
	{
		char szPath[MAX_PATH]={0};
		GetModuleFileNameA(GetCurrentModule(), szPath, MAX_PATH);
		strPath = szPath;
		strPath = strPath.substr(0, strPath.find_last_of('\\', strPath.length()));
		if ( strPath.c_str()[0] > 'G' )
		{
			char szDiskName[2] = {strPath.c_str()[0], '\0'};
			HKEY hKey;
			BOOL bOK = FALSE;
			if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Century Network\\yileyoo\\MountList", 0, KEY_ALL_ACCESS|KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS )  
			{//Matrix环境
				int nIndex = 0;
				while (true)  
				{  
					char subKeyName[MAX_PATH] = "";  
					if ( RegEnumKeyA(hKey, nIndex, subKeyName, MAX_PATH) != ERROR_SUCCESS )
					{
						break;  
					}

					string strRegPath = "SOFTWARE\\Century Network\\yileyoo\\MountList\\";
					strRegPath.append(subKeyName);
					HKEY hEnumKey;
					if ( RegOpenKeyExA(HKEY_LOCAL_MACHINE, strRegPath.c_str(), 0, KEY_READ|KEY_WOW64_64KEY, &hEnumKey) != ERROR_SUCCESS )
					{
						break;
					}

					char szPath[MAX_PATH]={0};
					DWORD dwSize = MAX_PATH;
					if ( RegQueryValueExA(hEnumKey, "LocalDrive", 0, 0, (LPBYTE)szPath, &dwSize) != ERROR_SUCCESS )
					{
						break;
					}

					if ( strcmp(szPath, szDiskName) == 0 )
					{
						char szNewPath[MAX_PATH]={0};
						DWORD dwSize = MAX_PATH;
						if ( RegQueryValueExA(hEnumKey, "LocalPath", 0, 0, (LPBYTE)szNewPath, &dwSize) == ERROR_SUCCESS )
						{
							char szDiskPath[3] = {strPath.c_str()[0], ':', '\0'};
							strPath.Replace(szDiskPath, szNewPath);
							bOK = TRUE;
							break;
						}
					}
					RegCloseKey(hEnumKey);
					nIndex++;
				}
				RegCloseKey(hKey);
			}
			if ( !bOK )
			{//通用环境
				string strEndPath = strPath.substr(2);

				WIN32_FIND_DATAA wfd;
				string strDirPath = "C:\\SuperDisk\\";
				string strFindPath = strDirPath + "*.*";
				HANDLE hFile = FindFirstFileA(strFindPath.c_str(), &wfd);
				vector<string> vecFile;
				if ( hFile != INVALID_HANDLE_VALUE )
				{
					while ( FindNextFileA(hFile, &wfd) ) 
					{
						if (wfd.cFileName[0] == '.') continue;
						else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							string strKeyPath = strDirPath;
							strKeyPath.append(wfd.cFileName).append(strEndPath);
							if ( access(strKeyPath.c_str(), 0) == 0 )
							{
								strPath = strKeyPath;
								break;
							}
						}
						else continue;
					}
				}
			}
		}
	}
	return string(strPath);
}

HWND GetFocusWnd()
{
	const DWORD tid = GetWindowThreadProcessId(GetForegroundWindow(),NULL);
	AttachThreadInput(tid, GetCurrentThreadId(),TRUE);
	HWND hWnd = GetFocus();
	AttachThreadInput(tid, GetCurrentThreadId(),FALSE);
	return hWnd;
}

HWND FindWindowExceptWnd( HWND hWnd, const WCHAR* pwszClassName, const WCHAR* pwszWindowName )
{
	HWND hFind = NULL;
	hFind = FindWindow(pwszClassName, pwszWindowName);
	while ( hFind == hWnd )
	{
		hFind = FindWindowEx(NULL, hFind, pwszClassName, pwszWindowName);
	}
	return hFind;
}

BOOL GetRegValue( HKEY hRoot, const char* pszPath, const char* pszName, DWORD dwType, void* pValue, DWORD dwSize )
{
	HKEY hKey = NULL;
	HRESULT hResult = RegOpenKeyExA(hRoot, pszPath, 0, KEY_READ, &hKey);
	if ( hResult == ERROR_SUCCESS )
	{
		hResult = RegQueryValueExA(hKey, pszName, 0, &dwType, (BYTE*)pValue, &dwSize);
		RegCloseKey(hKey);
		return hResult == ERROR_SUCCESS;
	}
	return  FALSE;
}

void GetWinClientRect( RECT& _rect )
{
	HWND hWnd = GetDesktopWindow();
	::GetClientRect(hWnd, &_rect);
	hWnd = ::FindWindowA("Shell_TrayWnd", NULL);

	RECT rectFix;
	::GetWindowRect(hWnd, &rectFix);
	if ( _rect.top == rectFix.top && _rect.bottom == rectFix.bottom )
	{
		if ( rectFix.left == 0 ) _rect.left = rectFix.right;
		else _rect.right = rectFix.left;
	}
	else
	{
		if ( rectFix.top == 0 ) _rect.top = rectFix.bottom;
		else _rect.bottom = rectFix.top;
	}
}

int ReplaceFileContent(const char* pszFileName, const char* pszOldContent, const char* pszNewContent)
{
	FILE* pFile = fopen(pszFileName, "r");
	if ( pFile )
	{
		CStdStringA pszContent;
		char buffer[10];
		memset(buffer, 0, sizeof(char)*10);
		while ( fread_s(buffer, 10, sizeof(char), 1, pFile) )
		{
			pszContent += buffer;
		}
		fclose(pFile);
		pszContent.Replace(pszOldContent, pszNewContent);
		pFile = fopen(pszFileName, "w");
		if ( pFile )
		{
			fwrite(pszContent, 1, pszContent.length(), pFile);
			fclose(pFile);
		}
	}
	return 0;
}

void RunCmdAndWait( const char* pszCmd )
{
	CLogger::GetInstance()->Log("RunCmd", pszCmd);

	CStdStringA strCmd(pszCmd);
	STARTUPINFOA si;   
	PROCESS_INFORMATION pi;   
	ZeroMemory(&si, sizeof(si));   
	si.cb = sizeof(si);   
	ZeroMemory(&pi, sizeof(pi));   
	if( CreateProcessA(NULL, strCmd.GetBuffer(), NULL, NULL, FALSE,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi) )   
	{   
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);   
		CloseHandle(pi.hProcess);
		Sleep(1);
		strCmd.ReleaseBuffer();
	}
}

void RunCmdAndNoWait( const char* pszCmd )
{
	CStdStringA strCmd(pszCmd);
	STARTUPINFOA si;   
	PROCESS_INFORMATION pi;   
	ZeroMemory(&si, sizeof(si));   
	si.cb = sizeof(si);   
	ZeroMemory(&pi, sizeof(pi));   
	if( CreateProcessA(NULL, strCmd.GetBuffer(), NULL, NULL, FALSE,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi) )   
	{   
		WaitForSingleObject(pi.hProcess, 100);
		CloseHandle(pi.hThread);   
		CloseHandle(pi.hProcess);
		Sleep(1);
		strCmd.ReleaseBuffer();
	}
}

BOOL CopyFileToPath( const char* pszExistPath, const char* pszNewPath, const char* pszFileName, BOOL bFailIfExist )
{
	string strPathFrom = pszExistPath;
	strPathFrom.append("\\");
	strPathFrom.append(pszFileName);
	string strPathTo = pszNewPath;
	strPathTo.append("\\");
	strPathTo.append(pszFileName);
	return CopyFileA(strPathFrom.c_str(), strPathTo.c_str(), bFailIfExist);
}

void CopyDirectory( const char* pszExistPath, const char* pszNewPath )
{
	_mkdir(pszNewPath);
	WIN32_FIND_DATAA wfd;
	string strDirPath = pszExistPath;
	strDirPath.append("\\");
	size_t index = strDirPath.find("\\\\");
	if ( index != -1 )
	{
		strDirPath = strDirPath.substr(0, index) + strDirPath.substr(index + 1);
	}
	string strFindPath = strDirPath + "*.*";
	HANDLE hFile = FindFirstFileA(strFindPath.c_str(), &wfd);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if (wfd.cFileName[0] == '.') continue;
			else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				string strExistPath = strDirPath + wfd.cFileName;
				string strNewPath = string(pszNewPath) + "\\" + wfd.cFileName;
				CopyDirectory(strExistPath.c_str(), strNewPath.c_str());
			}
			else
			{
				CopyFileToPath(pszExistPath, pszNewPath, wfd.cFileName, FALSE);
			}
		} while (FindNextFileA(hFile, &wfd));
	}
	FindClose(hFile);
}

string ReadFileContent( const char* pszFilePath )
{
	FILE* pFile = fopen(pszFilePath, "rb");
	string strContent = "";
	if ( pFile )
	{
		char buffer[10];
		memset(buffer, 0, sizeof(char)*10);
		while ( fread_s(buffer, 10, sizeof(char), 1, pFile) )
		{
			strContent += buffer;
		}
		fclose(pFile);
	}
	return strContent;
}

std::wstring GetAnsiObjectName( const char* pszNameHead, const char* pszNameEnd )
{
	string strName = pszNameHead;
	strName.append(pszNameEnd);
	return AtoW(strName);
}

