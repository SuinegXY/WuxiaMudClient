#include <tchar.h>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <Iphlpapi.h>
#include <tcpmib.h>

#pragma comment(lib, "Iphlpapi.lib")

//�Ƚ��ļ��汾�� �汾����Ҫ��ѭ��׼1.0.0.0
//-1 �°汾�Žϵ� 1 �°�Žϸ� 0 ���
int CompareVersionA(char* pszVersionOld, char* pszVersionNew)
{
	if ((NULL == pszVersionOld) || (NULL == pszVersionNew))
	{
		return 0;
	}
	int nVersionOld = 1;
	int nVersionNew = 1;
	char* pVersionOld = pszVersionOld;
	char* pVersionNew = pszVersionNew;
	//�Ƚ�λ��
	//�ж�10.0 > 9.0�������
	pVersionOld++;
	while ((*pVersionOld != '.') && (*pVersionOld != 0))
	{
		pVersionOld++;
		nVersionOld++;
	}
	pVersionNew++;
	while ((*pVersionNew != '.') && (*pVersionNew != 0))
	{
		pVersionNew++;
		nVersionNew++;
	}
	if (nVersionOld > nVersionNew)
	{
		return -1;
	}
	else if (nVersionNew > nVersionOld)
	{
		return 1;
	}
	//λ����ͬ�������Ƚϣ��ָ���ʼ״̬
	pVersionOld = pszVersionOld;
	pVersionNew = pszVersionNew;
	//��һλ
	if (*pVersionOld > *pVersionNew)
	{
		return -1;
	}
	else if (*pszVersionOld < *pszVersionNew)
	{
		return 1;
	}
	else
	{
		//��һλ��ͬ��ѭ���Ƚ�֮��
		while(nVersionOld != 0)
		{
			pVersionOld++;
			pVersionNew++;
			if (*pVersionOld > *pVersionNew)
			{
				return -1;
			}
			else if (*pVersionOld < *pVersionNew)
			{
				return 1;
			}
			nVersionOld -= 1;
		}
		//����ͬ���Ƚ���һλ
		pVersionOld++;
		pVersionNew++;
		if ((*pVersionOld == 0) && (*pVersionNew == 0))
		{
			return 0;
		}
		return CompareVersionA(pVersionOld, pVersionNew);
	}
}

//�Ƚ��ļ��汾�� �汾����Ҫ��ѭ��׼1.0.0.0
//-1 �°汾�Žϵ� 1 �°�Žϸ� 0 ���
int CompareVersionW(WCHAR* pszVersionOld, WCHAR* pszVersionNew)
{
	if ((NULL == pszVersionOld) || (NULL == pszVersionNew))
	{
		return 0;
	}
	int nVersionOld = 1;
	int nVersionNew = 1;
	WCHAR* pVersionOld = pszVersionOld;
	WCHAR* pVersionNew = pszVersionNew;
	//�Ƚ�λ��
	//�ж�10.0 > 9.0�������
	pVersionOld++;
	while ((*pVersionOld != _T('.')) && (*pVersionOld != 0))
	{
		pVersionOld++;
		nVersionOld++;
	}
	pVersionNew++;
	while ((*pVersionNew != _T('.')) && (*pVersionNew != 0))
	{
		pVersionNew++;
		nVersionNew++;
	}
	if (nVersionOld > nVersionNew)
	{
		return -1;
	}
	else if (nVersionNew > nVersionOld)
	{
		return 1;
	}
	//λ����ͬ�������Ƚϣ��ָ���ʼ״̬
	pVersionOld = pszVersionOld;
	pVersionNew = pszVersionNew;
	//��һλ
	if (*pVersionOld > *pVersionNew)
	{
		return -1;
	}
	else if (*pszVersionOld < *pszVersionNew)
	{
		return 1;
	}
	else
	{
		//��һλ��ͬ��ѭ���Ƚ�֮��
		while(nVersionOld != 0)
		{
			pVersionOld++;
			pVersionNew++;
			if (*pVersionOld > *pVersionNew)
			{
				return -1;
			}
			else if (*pVersionOld < *pVersionNew)
			{
				return 1;
			}
			nVersionOld -= 1;
		}
		//����ͬ���Ƚ���һλ
		pVersionOld++;
		pVersionNew++;
		if ((*pVersionOld == 0) && (*pVersionNew == 0))
		{
			return 0;
		}
		return CompareVersionW(pVersionOld, pVersionNew);
	}
}

// �ж�ָ���ļ��Ƿ���ڣ�����·����
BOOL	FileIsExist(LPCTSTR szFileName)
{
	DWORD dwRes = GetFileAttributes(szFileName);
	if (dwRes == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}
	return TRUE;
}

// �ж�ָ���ļ��Ƿ���ڣ�����·����
BOOL	FileIsExistA(const char* pszFileName)
{
	DWORD dwRes = GetFileAttributesA(pszFileName);
	if (dwRes == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}
	return TRUE;
}

//ȥ������·���е��ļ�������չ����szPath��Ҫ����ռ�
BOOL	GetFilePath(TCHAR* szFullPath, TCHAR* szPath)
{
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	_tsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, NULL, NULL, NULL);
	_tmakepath_s(szPath, MAX_PATH, szDrive, szDir, NULL, NULL);
	return TRUE;
}

//ȥ������·���е��ļ�������չ����szPath��Ҫ����ռ�
BOOL	GetFilePathA(const char* szFullPath, char* szPath)
{
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	_splitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, NULL, NULL, NULL);
	_makepath_s(szPath, MAX_PATH, szDrive, szDir, NULL, NULL);
	return TRUE;
}

// �õ�ָ���ļ��Ĵ�С
ULONGLONG	GetFileSize(LPCTSTR szFileName)
{
	if (NULL == szFileName)
	{
		return 0;
	}
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesEx(szFileName, GetFileExInfoStandard, &data))
	{
		ULONGLONG llFileSize = data.nFileSizeHigh;

		llFileSize <<= sizeof(DWORD)*8; //���� 32 λ
		llFileSize += data.nFileSizeLow;
		return llFileSize;
	}
	return 0;
}

// �õ�ָ���ļ��Ĵ�С
ULONGLONG	GetFileSizeA(const char* pszFileName)
{
	if (NULL == pszFileName)
	{
		return 0;
	}
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesExA(pszFileName, GetFileExInfoStandard, &data))
	{
		ULONGLONG llFileSize = data.nFileSizeHigh;

		llFileSize <<= sizeof(DWORD)*8; //���� 32 λ
		llFileSize += data.nFileSizeLow;
		return llFileSize;
	}
	return 0;
}

// ��URL�����ļ���
BOOL GetFileNameFromUrlA(char* szUrl, char* szFileName)
{
	if ((NULL == szUrl) || (NULL == szFileName))
	{
		return FALSE;
	}
	char* pTmp = szUrl;
	pTmp += strlen(szUrl) - 1;
	while((*pTmp != '\\') && (*pTmp != '/') && (pTmp != szUrl))
	{
		pTmp--;
	}
	if (pTmp == szUrl)
	{
		strcpy(szFileName, szUrl);
		return TRUE;
	}
	pTmp++;
	strcpy(szFileName, pTmp);
	return TRUE;
}

// ��URL�����ļ���
BOOL GetFileNameFromUrlW(WCHAR* szUrl, WCHAR* szFileName)
{
	if ((NULL == szUrl) || (NULL == szFileName))
	{
		return FALSE;
	}
	WCHAR* pTmp = szUrl;
	pTmp += wcslen(szUrl) - 1;
	while((*pTmp != '\\') && (*pTmp != '/') && (pTmp != szUrl))
	{
		pTmp--;
	}
	if (pTmp == szUrl)
	{
		wcscpy(szFileName, szUrl);
		return TRUE;
	}
	pTmp++;
	wcscpy(szFileName, pTmp);
	return TRUE;
}

// �õ��ļ��ĺ�׺��
char*	GetFileExtA(char* pszFile)
{
	if (NULL == pszFile) return NULL;
	char* pTmp = pszFile;
	pTmp += strlen(pszFile);
	for (;pTmp != pszFile;pTmp--)
	{
		if (*pTmp == '.')
		{
			return pTmp;
		}
	}
	return NULL;
}

// �õ��ļ��ĺ�׺��
WCHAR*	GetFileExtW(WCHAR* pszFile)
{
	if (NULL == pszFile) return NULL;
	WCHAR* pTmp = pszFile;
	pTmp += wcslen(pszFile);
	for (;pTmp != pszFile;pTmp--)
	{
		if (*pTmp == L'.')
		{
			return pTmp;
		}
	}
	return NULL;
}

// ��·���а����ļ���
BOOL GetFileNameA(char* szFullPath, char* szFileName)
{
	if ((NULL == szFullPath) || (NULL == szFileName))
	{
		return FALSE;
	}
	char* pTmp = szFullPath;
	pTmp += strlen(szFullPath) - 1;
	while((*pTmp != '\\') && (pTmp != szFullPath))
	{
		pTmp--;
	}
	if (pTmp == szFullPath)
	{
		strcpy(szFileName, szFullPath);
		return TRUE;
	}
	pTmp++;
	strcpy(szFileName, pTmp);
	return TRUE;
}

// ��·���а����ļ���
BOOL GetFileNameW(WCHAR* szFullPath, WCHAR* szFileName)
{
	if ((NULL == szFullPath) || (NULL == szFileName))
	{
		return FALSE;
	}
	WCHAR* pTmp = szFullPath;
	pTmp += wcslen(szFullPath) - 1;
	while((*pTmp != '\\') && (*pTmp != '/') && (pTmp != szFullPath))
	{
		pTmp--;
	}
	if (pTmp == szFullPath)
	{
		wcscpy(szFileName, szFullPath);
		return TRUE;
	}
	pTmp++;
	wcscpy(szFileName, pTmp);
	return TRUE;
}

// ͨ��PID��ȡ��������
BOOL GetProcessNameFromId(DWORD dwPid, TCHAR* pszProcessName)
{
	BOOL bRet = FALSE;
	if (pszProcessName == NULL)
	{
		return bRet;
	}
	PROCESSENTRY32 pe32;  
	// ��ʹ������ṹ֮ǰ�����������Ĵ�С  
	pe32.dwSize = sizeof(pe32);   

	// ��ϵͳ�ڵ����н�����һ������  
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if(hProcessSnap == INVALID_HANDLE_VALUE)  
	{  
		OutputDebugString(_T(" CreateToolhelp32Snapshot����ʧ�ܣ� \n"));  
		return -1;  
	}  

	// �������̿��գ�������ʾÿ�����̵���Ϣ  
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore)  
	{  
		if (pe32.th32ProcessID == dwPid)
		{
			_tcscpy(pszProcessName, pe32.szExeFile);
			bRet = TRUE;
			break;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);  
	}  

	// ��Ҫ���������snapshot����  
	::CloseHandle(hProcessSnap);  
	return bRet;
}

typedef DWORD (WINAPI * PFN_AllocateAndGetTcpExTableFromStack)
	(
	PVOID*			ppTcpTable,
	BOOL			bOrder,
	HANDLE			hHeap,
	DWORD			dwFlags,
	DWORD			dwFamily
	);

typedef DWORD (WINAPI * PFN_InternalGetTcpTable2)    
	(    
	PVOID*          pTcpTable,     
	HANDLE			hHeap,
	DWORD			dwFlags
	);

// ����ռ��ĳ���˿ڵĽ���ID
// XP SP2����ϵͳ
BOOL GetProcessIdFromPort(DWORD dwPort, DWORD& dwPid)
{
	HMODULE hModule = LoadLibrary(_T("iphlpapi.dll"));
	if (NULL == hModule)
	{
		return FALSE;
	}
	PFN_AllocateAndGetTcpExTableFromStack pAllocateAndGetTcpExTableFromStack = (PFN_AllocateAndGetTcpExTableFromStack)GetProcAddress(hModule, "AllocateAndGetTcpExTableFromStack");
	if (pAllocateAndGetTcpExTableFromStack != NULL)
	{
		//����ϵͳΪXP
		PMIB_TCPTABLE_OWNER_PID pTcpTable = NULL;
		if (pAllocateAndGetTcpExTableFromStack((PVOID*)&pTcpTable, TRUE, GetProcessHeap(), 0, AF_INET) != 0)
		{
			if (pTcpTable)
			{
				HeapFree(GetProcessHeap(), 0, pTcpTable);
			}
			FreeLibrary(hModule);
			hModule = NULL;
			return FALSE;
		}
		for (UINT i = 0; i < pTcpTable->dwNumEntries; i++)
		{
			if (dwPort == ntohs(0x0000FFFF & pTcpTable->table[i].dwLocalPort))
			{
				dwPid = pTcpTable->table[i].dwOwningPid;
				if (pTcpTable)
				{
					HeapFree(GetProcessHeap(), 0, pTcpTable);
				}
				FreeLibrary(hModule);
				hModule = NULL;
				return TRUE;
			}
		}
	}
	else
	{
		//����ϵͳΪWIN7
		PFN_InternalGetTcpTable2 pInternalGetTcpTable2 = (PFN_InternalGetTcpTable2)GetProcAddress(hModule, "InternalGetTcpTable2");
		if (NULL != pInternalGetTcpTable2)
		{
			PMIB_TCPTABLE_OWNER_PID pTcpTable = NULL;
			if (pInternalGetTcpTable2((PVOID*)&pTcpTable, GetProcessHeap(), 0) != 0)
			{
				if (pTcpTable)
				{
					HeapFree(GetProcessHeap(), 0, pTcpTable);
				}
				FreeLibrary(hModule);
				hModule = NULL;
				return FALSE;
			}
			for (UINT i = 0; i < pTcpTable->dwNumEntries; i++)
			{
				if (dwPort == ntohs(0x0000FFFF & pTcpTable->table[i].dwLocalPort))
				{
					dwPid = pTcpTable->table[i].dwOwningPid;
					if (pTcpTable)
					{
						HeapFree(GetProcessHeap(), 0, pTcpTable);
					}
					FreeLibrary(hModule);
					hModule = NULL;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
