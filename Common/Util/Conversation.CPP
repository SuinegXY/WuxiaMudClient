#include "stdafx.h"
#include "Conversation.H"
#include "Util/MD5.h"
#include <tchar.h>
#include <Psapi.h>
#include <TlHelp32.h>
#pragma comment(lib , "psapi.lib")

CConversation::CConversation()
{

}

CConversation::~CConversation()
{

}

BOOL	CConversation::GetProcessID(LPCTSTR pProcessName , DWORD dwParentProcessID , DWORD&	ProcessID )
{
	HANDLE						hSnapshot	= INVALID_HANDLE_VALUE;
	PROCESSENTRY32				Pro32		= {0};

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hSnapshot == INVALID_HANDLE_VALUE )	return FALSE;
	Pro32.dwSize = sizeof(PROCESSENTRY32);
	if(Process32First(hSnapshot , &Pro32))
	{
		do 
		{
			if(lstrcmpi(pProcessName , Pro32.szExeFile) == 0 &&Pro32.th32ParentProcessID == dwParentProcessID)
			{
				ProcessID = Pro32.th32ProcessID;
				CloseHandle(hSnapshot);
				return TRUE;
			}
		}while (Process32Next(hSnapshot , &Pro32));
	}
	CloseHandle(hSnapshot);
	return FALSE;//没找到
}

DWORD	CConversation::GetSystemProcessId()
{
	DWORD		dwProcessID = 0;
	DWORD		SystemProcessID;

	GetProcessID(_T("system") , 0 , SystemProcessID);
	if(SystemProcessID == 0 )	SystemProcessID = 4;//默认为4

	GetProcessID(_T("smss.exe") , SystemProcessID , dwProcessID);

	if(dwProcessID == 0 )	SystemProcessID = 8;
	GetProcessID(_T("smss.exe") , SystemProcessID , dwProcessID);

	return dwProcessID;
}

string	CConversation::GetTime()
{
	DWORD		dwProcessID = 0;
	HANDLE		hProcess = NULL;
	FILETIME	CreateTime , ExitTime , KernelTime , UserTime  ,LocalFileTime;
	SYSTEMTIME	SystemTime;
	CHAR		szTime[128] = {0};

	do 
	{
		dwProcessID = GetSystemProcessId();
		if(dwProcessID == 0 )	break;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION , FALSE , dwProcessID);
		if(hProcess == NULL )	break;

		if(!GetProcessTimes(hProcess , &CreateTime , &ExitTime , &KernelTime , &UserTime))	break;

		FileTimeToLocalFileTime(&CreateTime , &LocalFileTime);
		FileTimeToSystemTime(&LocalFileTime , &SystemTime);

		sprintf_s(szTime , _countof(szTime) , "%d:%d:%d:%d-%d/%d/%d" , 
			SystemTime.wMilliseconds , 
			SystemTime.wSecond,
			SystemTime.wMinute,
			SystemTime.wHour,
			SystemTime.wDay,
			SystemTime.wMonth,
			SystemTime.wYear
			);
	} while (FALSE);

	if(hProcess)	CloseHandle(hProcess);

	return szTime;
}

string CConversation::GetConversation(LPCSTR lpszGid , UINT64 dwMAC)
{
	return string();
}
