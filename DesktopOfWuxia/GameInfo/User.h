#ifndef _USERINFO_H_
#define _USERINFO_H_

#include "BaseInfo.h"

class CUser : public CBaseInfo
{
	InstanceDefine(CUser);

public:
	void SetWnd(HWND hFrameWnd);
	void UserLogin(const char* pszJson, BOOL bSend = TRUE);
	void UserLogout();
	string GetUser();
	string GetJson();
	string GetUserName();
	BOOL GetNew();

private:
	string m_strUserID;
	string m_strUserName;
	string m_strJson;
	string m_strUserHeadUrl;
	string m_strUserHeadPath;
	HANDLE m_hGetUserHeadThread;

	HWND m_hFrameWnd;
	BOOL m_bNew;
};

#endif

