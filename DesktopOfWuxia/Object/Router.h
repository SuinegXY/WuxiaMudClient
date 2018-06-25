#ifndef _ROUTER_H_
#define _ROUTER_H_

class CRouter
{
	InstanceDefine(CRouter);
public:
	CRouter();

	BOOL Start();
	BOOL Stop();
	BOOL Send(const char* pszContant);
	BOOL Send(const WCHAR* pwszContant);
	BOOL Send(const char* pszEvent, const char* pszContant);
	BOOL Send(const char* pszEvent, const WCHAR* pszContant);
	BOOL Send(const char* pszEvent, const char* pszID, const char* pszContant);
	BOOL SetChannel(const char* pszChannel, const char* pszPcName);

	void SetAuth();
	BOOL GetAuth();

protected:
	string GetIP(const char* pszWeb);
	static DWORD WINAPI ReadConnectionThreat(LPVOID lp);
	BOOL SendSK(const char* pszContant);
	void ParserData(const char* pszContent);

private:
	SOCKET m_sk;
	BOOL m_bStart;
	HWND m_hAttachWnd;
	int m_nHeight;
	int m_nWidth;
	int m_nLine;

	int m_nLineMax;
	int m_nSpeed;
	int m_nFont;
	DWORD m_dwColor;
	BYTE m_byTrans;

	string m_strChannel;
	BOOL m_bFirst;
	BOOL m_bAuth;
};

#endif