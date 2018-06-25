#ifndef _CLog_h_
#define _CLog_h_

class CLogger
{
	InstanceDefine(CLogger);

public:
	CLogger();
	void Refresh();

	void SetHead(const char* pszHead);
	void Log(const char* pszContant);
	void Log(const char* pszTitle, const char* pszContant);
	void Log(const char* pszTitle, const char* pszContant, const char* pszEnd);
	void Log(const char* pszTitle, BOOL bYes);
	void Log(const char* pszTitle, const char* pszFormat, DWORD dwValue);
#ifdef UNICODE
	void Log(const char* pszTitle, const WCHAR* pszContant);
#endif

private:
	void Write(const char* pszContant);
	string GetTime();

private:
	bool m_bDebug;
	bool m_bLog;
	string m_strHead;
};

#endif