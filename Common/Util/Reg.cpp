#include <Windows.h>
#include <shlwapi.h>
#include "Reg.h"

namespace RegHelper
{

	typedef BOOL (__stdcall *ISWOW64PROCESS) (HANDLE, PBOOL);
	typedef BOOL (__stdcall *WOW64DISABLEWOW64FSREDIRECTION)(PVOID*);  
	typedef BOOL (__stdcall *WOW64REVERTWOW64FSREDIRECTION)(PVOID);
	WOW64DISABLEWOW64FSREDIRECTION pfWow64DisableWow64FsRedirection = NULL;
	WOW64REVERTWOW64FSREDIRECTION pfWow64RevertWow64FsRedirection = NULL;
	BOOL bWow64 = FALSE;
	BOOL bBit = FALSE;
	PVOID pVoid = NULL;

	void Init()
	{
		static BOOL bOnce = FALSE;
		if ( bOnce ) return;
		bOnce = TRUE;
		HMODULE hLibrary = LoadLibrary(L"Kernel32.dll");  
		if ( hLibrary )  
		{
			try
			{
				ISWOW64PROCESS pIsWow64Process = (ISWOW64PROCESS)GetProcAddress(hLibrary, "IsWow64Process");
				pfWow64DisableWow64FsRedirection  =  (WOW64DISABLEWOW64FSREDIRECTION)GetProcAddress(hLibrary, "Wow64DisableWow64FsRedirection");   
				pfWow64RevertWow64FsRedirection  =  (WOW64REVERTWOW64FSREDIRECTION) GetProcAddress(hLibrary, "Wow64RevertWow64FsRedirection");
				if ( pIsWow64Process )
				{
					pIsWow64Process(GetCurrentProcess(), &bWow64);
				}
			}
			catch ( ... )
			{
				pfWow64DisableWow64FsRedirection = NULL;
				pfWow64RevertWow64FsRedirection = NULL;
			}
		}
	}

	BOOL Is64()
	{
		return bWow64;
	}

	void Bit64Set()
	{
		if ( bWow64 && pfWow64DisableWow64FsRedirection )
		{
			bBit = TRUE;
			pfWow64DisableWow64FsRedirection(&pVoid);
		}
	}

	void Bit64Unset()
	{
		if ( bWow64 && pfWow64RevertWow64FsRedirection )
		{
			bBit = FALSE;
			pfWow64RevertWow64FsRedirection(pVoid);
		}
	}

	BOOL DeleteRegTree( HKEY hRoot, const char* pszSubKey, BOOL bWow64 )
	{
		HKEY hKey;
		char szKey[255];
		DWORD dwLength;
		DWORD dwSize=0;
		BOOL  bFlag = TRUE;
		DWORD dwParam = bWow64 ? KEY_READ|KEY_WOW64_64KEY : KEY_READ ;
		if ( RegOpenKeyExA( hRoot, pszSubKey, 0, dwParam, &hKey) != 0 ) bFlag = FALSE;
		if ( RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwSize, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != 0 ) bFlag = FALSE;
		if ( dwSize )
		{
			for ( DWORD nIndex = 0; nIndex < dwSize; ++nIndex )
			{     
				dwLength = 255;
				if ( RegEnumKeyExA(hKey, nIndex, szKey, &dwLength, NULL, NULL, NULL, NULL) == 0 ) 
				{
					DeleteRegTree(hKey, szKey);
				}   
				else
				{
					bFlag = FALSE;
				}
			}

		}
		if ( RegDeleteKeyA(hRoot, pszSubKey) != 0 ) bFlag = FALSE;
		SHDeleteKeyA(hKey, "");
		RegCloseKey(hKey);
		return bFlag;
	}

}