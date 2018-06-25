#include "stdafx.h"
#include <io.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#include "statistic/ConfigManager.h"
#include "statistic/ClientStatistic.h"
#include "Logger.h"
#include "MainFrame.h"

BOOL CheckSingleton()
{
	string strMutex = "SUINEG_DESKTOP_OF_WUXIA";
	HANDLE hMute = CreateMutexA(NULL, FALSE, strMutex.c_str());
	if ( ERROR_ALREADY_EXISTS == GetLastError() )
	{
		CloseHandle(hMute);
		hMute = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
{
	HANDLE hDumpFile = CreateFileA(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		return FALSE;
	}

	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
	return TRUE;
}

LONG CrashHandler(EXCEPTION_POINTERS *pException)  
{
	string szFile = GetTrueLocalPath() + "\\crashdump.dmp";
	if ( access(szFile.c_str(), 0) == 0 )
	{
		DeleteFileA(szFile.c_str());
	}
	CreateDumpFile(szFile.c_str(), pException);

	return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashHandler);
	CLogger::GetInstance()->SetHead("Sui--->Wuxia->");

	//if ( !CheckSingleton() )
	//{
	//	return 0;
	//}

	WSADATA wsData;  
	::WSAStartup(MAKEWORD(2,2), &wsData);
	CPaintManagerUI::SetInstance(hInstance);
	CConfigManager::GetInstance()->Init();
	CClientStatistic::GetInstance()->SetManager(CConfigManager::GetInstance());

	CMainFrameWnd* pMainWnd = new CMainFrameWnd;
	if ( pMainWnd )
	{
		pMainWnd->Create(NULL, _T("×ÀÃæÎäÏÀ"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 400, 200);
		SetWindowLong(pMainWnd->GetHWND(), GWL_STYLE, GetWindowLong(pMainWnd->GetHWND() ,GWL_STYLE) & ~WS_MAXIMIZEBOX);
		pMainWnd->Move();
		pMainWnd->ShowWindow();
	}

	CPaintManagerUI::MessageLoop();
	try
	{
		::WSACleanup();  
	} catch (...){}
	return 0;
}