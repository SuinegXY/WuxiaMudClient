#ifndef _INSTALLER_H_
#define _INSTALLER_H_

#include "RegSource.h"

class CInstaller
{
	InstanceDefine(CInstaller);

public:
	void InstallLocalApk(HWND hWnd);
	void InstallCloudApk(const char* pszDownloadUrl);
	void InstallStart();
	void InstallOver();
	static DWORD WINAPI DownloadThread(LPVOID lp);
	static DWORD WINAPI InstallThread(LPVOID lp);

protected:
	void DoInstall(const char* pszFile);

private:
	static HWND m_hLauncherWnd;
	static HANDLE m_hSemaphore;
	static string m_strDownloadUrl;
	static string m_strInstallPath;
};

#endif