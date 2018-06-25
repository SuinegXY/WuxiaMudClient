#include "stdafx.h"
#include "Logger.h"
#include <shlobj.h>
#include <io.h>
#include <atlbase.h>
#include <time.h>
#include "Util\ATW.h"

#define		FILE_DEBUG		"\\D87E5DCE49BAD13694326517AF065C78"
#define		FILE_LOG		"\\D87E5DCE49BAD13694326517AF065C79"

InstanceRealize(CLogger);

void CLogger::init()
{
	SetHead("Sui--->");
}

CLogger::CLogger()
{
	m_bDebug	= FALSE;
	m_bLog		= FALSE;
	Refresh();
}

void CLogger::SetHead( const char* pszHead )
{
	m_strHead = pszHead;
}

void CLogger::Log( const char* pszContant )
{
	return Write(pszContant);
}

void CLogger::Log( const char* pszTitle, const char* pszContant )
{
	if ( pszContant == NULL ) return;
	string strContent = pszTitle;
	strContent.append(":\t");
	strContent.append(pszContant);
	Log(strContent.c_str());
}
void CLogger::Log(const char* pszTitle, const char* pszContant, const char* pszEnd)
{
	if ( pszContant == NULL ) return;
	string strContent = pszTitle;
	strContent.append(":\t");
	strContent.append(pszContant);
	strContent.append(pszEnd);
	Log(strContent.c_str());
}

#ifdef UNICODE
void CLogger::Log(const char* pszTitle, const WCHAR* pszContant)
{
	if ( pszContant == NULL ) return;
	string strContent = pszTitle;
	strContent.append(":\t");
	strContent.append(WtoA(pszContant).c_str());
	Log(strContent.c_str());
}
#endif

void CLogger::Log(const char* pszTitle, BOOL bYes)
{
	string strContent = pszTitle;
	strContent.append(":\t");
	strContent.append(bYes ? "Yes" : "No");
	Log(strContent.c_str());
}

void CLogger::Log(const char* pszTitle, const char* pszFormat, DWORD dwValue)
{
	char szBuffer[MAX_PATH] = {0};
	sprintf(szBuffer, pszFormat, dwValue);
	string strContent = pszTitle;
	strContent.append(":\t");
	strContent.append(szBuffer);
	Log(strContent.c_str());
}

void CLogger::Refresh()
{
	m_bDebug = FALSE;
	m_bLog = FALSE;
	static string strPath;
	if ( strPath.length() == 0 )
	{
		char szPath[MAX_PATH]={0};
		SHGetSpecialFolderPathA(NULL, szPath, CSIDL_DESKTOP, 0);
		strPath = szPath;
	}
	OutputDebugStringA((string("Sui--->")+strPath).c_str());
	if ( access((strPath+FILE_DEBUG).c_str(), 0) == 0 )
	{
		m_bDebug = TRUE;
		OutputDebugStringA("Sui--->DebugView打开");
	}
	else
	{
		OutputDebugStringA("Sui--->DebugView关闭");
	}
	if ( access((strPath+FILE_LOG).c_str(), 0) == 0 )
	{
		OutputDebugStringA("Sui--->文件日志打开");
	}
	else
	{
		OutputDebugStringA("Sui--->文件日志关闭");
	}
}

void CLogger::Write( const char* pszContant )
{
	if ( m_bDebug )
	{
		string strContent = m_strHead + pszContant;
		//if ( pszContant[strlen(pszContant)-1] == 0x0a )
			strContent.append("\r\n");
		OutputDebugStringA(strContent.c_str());
	}
	if ( m_bLog )
	{
		static string strLogPath;
		if ( strLogPath.length() == 0 )
		{
			char szPath[MAX_PATH]={0};
			GetModuleFileNameA(reinterpret_cast<HMODULE>(&__ImageBase), szPath, MAX_PATH);
			strLogPath = szPath;
			strLogPath = strLogPath.substr(0, strLogPath.find_last_of('\\', strLogPath.length()));
			strLogPath.append("\\");
			strLogPath.append(GetTime().c_str());
			strLogPath.append(".log");
		}
		FILE* file = fopen(strLogPath.c_str(), "ab+");
		if ( file )
		{
			string strContent = GetTime();
			strContent.append("\t\t");
			strContent.append(pszContant);
			if ( pszContant[strlen(pszContant)-1] != 0x0a )
				strContent.append("\r\n");
			fwrite(strContent.c_str(), sizeof(char), strContent.length(), file);
			fclose(file);
		}
	}
}

string CLogger::GetTime()
{
	char szTime[MAX_PATH] = {0};
	SYSTEMTIME st;
	GetLocalTime(&st);
	struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
	__time64_t dstTime = (DWORD)mktime(&gm);
	struct tm tTime;
	localtime_s(&tTime,&dstTime);
	strftime(szTime, 32, "%Y-%m-%d %H-%M-%S", &tTime);
	return szTime;
}

