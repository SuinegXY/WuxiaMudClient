#include "stdafx.h"
#include "Installer.h"
#include "HttpClient.h"
#include <commdlg.h>
#include "Engine\AdbPort.h"
#include "Engine\adb.h"
#include "Object\User.h"

InstanceRealize(CInstaller);

HWND   CInstaller::m_hLauncherWnd	= NULL;
HANDLE CInstaller::m_hSemaphore		= NULL;
string CInstaller::m_strDownloadUrl = "";
string CInstaller::m_strInstallPath = "";

void CInstaller::init()
{
	m_hSemaphore = CreateSemaphore(NULL, 1, 1, _T("MobileSimualtorInstallWaitForStartup"));
}

void CInstaller::InstallCloudApk( const char* pszDownloadUrl )
{
	if ( pszDownloadUrl && *pszDownloadUrl )
	{
		m_strDownloadUrl = pszDownloadUrl;
		CloseHandle(CreateThread(NULL, 0, DownloadThread, NULL, 0, NULL));
	}
}

void CInstaller::InstallLocalApk(HWND hWnd)
{
	OPENFILENAMEA ofn;
	char szFile[260];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "APKÎÄ¼þ (*.apk)\0*.apk\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "C:\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if ( GetOpenFileNameA(&ofn) == TRUE )
	{
		m_hLauncherWnd = hWnd;
		m_strInstallPath = szFile;
		CloseHandle(CreateThread(NULL, 0, InstallThread, NULL, 0, NULL));
	}
}

void CInstaller::InstallStart()
{
	WaitForSingleObject(m_hSemaphore, INFINITE);
}

void CInstaller::InstallOver()
{
	LONG lPrev = 0;
	ReleaseSemaphore(m_hSemaphore, 1, &lPrev);
}

DWORD WINAPI CInstaller::DownloadThread( LPVOID lp )
{
	if ( m_strDownloadUrl.length() == 0 ) return -1;

	string strPath = GetTrueLocalPath() + "\\Download\\apk";

	HttpClient http;
	int nRes = http.DownloadFile(m_strDownloadUrl.c_str(), strPath.c_str(), FALSE);
	if ( nRes != CURLE_OK ) return -1;

	CInstaller::GetInstance()->DoInstall(strPath.c_str());

	return 0;
}

DWORD WINAPI CInstaller::InstallThread( LPVOID lp )
{
	::PostMessage(m_hLauncherWnd, WM_INTEGRATION_INSTALL_START, NULL, NULL);
	CInstaller::GetInstance()->DoInstall(m_strInstallPath.c_str());
	::PostMessage(m_hLauncherWnd, WM_INTEGRATION_INSTALL_OVER, NULL, NULL);

	return 0;
}

void CInstaller::DoInstall(const char* pszFile)
{
	CInstaller::GetInstance()->InstallStart();
	CADBHandler::GetInstance()->SetPort(CADBPort::GetInstance()->GetHostPort());
	CADBHandler::GetInstance()->Install2(pszFile);
	string strParser = CADBHandler::GetInstance()->AaptParser(pszFile);
	string strValue;
	do 
	{
		size_t nIndexHead = strParser.find("name");
		if ( nIndexHead == -1 ) break;
		nIndexHead = strParser.find("'", nIndexHead+1);
		if ( nIndexHead == -1 ) break;
		size_t nIndexEnd = strParser.find("'", nIndexHead+1);
		if ( nIndexEnd == -1 ) break;
		strValue = strParser.substr(nIndexHead+1, nIndexEnd-nIndexHead-1);
	} while (FALSE);
	CUser::GetInstance()->SendBroadcast(4, strValue.length() ? strValue.c_str() : NULL);
	CInstaller::GetInstance()->InstallOver();
}
