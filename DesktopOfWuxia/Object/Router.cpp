#include "stdafx.h"
#include "Router.h"
#include "Logger.h"
#include "Util\ATW.h"
#include "Util\stdstring.h"
#include "Util\HWnd.h"
#include "GameInfo\User.h"
#include "Object\Parser.h"
#include "json\JsonHelper.h"
#include "statistic\ConfigManager.h"

#define WEB1			"10.0.6.56"
//#define WEB1			"10.0.4.165"
#define WEB2			"10.0.4.151"
#define PORT			20000
#define BUFFER_LENGH	65535

InstanceRealize(CRouter);

CRouter::CRouter()
{
}

void CRouter::init()
{
}

DWORD WINAPI CRouter::ReadConnectionThreat( LPVOID lp )
{
	CRouter* p = CRouter::GetInstance();

	while ( p->m_sk != INVALID_SOCKET )
	{
		char szBuffer[BUFFER_LENGH] = {0};
		if ( recv(p->m_sk, szBuffer, BUFFER_LENGH, 0) > 0 )
		{
			string strAnsi = UTF8toA(szBuffer);
			memset(szBuffer, 0, BUFFER_LENGH);
			p->ParserData(strAnsi.c_str());
		}
		Sleep(100);
	}

	return 0;
}

void CRouter::ParserData( const char* pszContant )
{
	if ( pszContant == NULL || *pszContant == NULL ) return;
	string strContant = pszContant;
	int nIndex = 0;
	while ( strContant.length() )
	{
		static const char szSeperate[2] = {0x0a, '\0'};
		string strLine = GetStringHead(strContant, szSeperate);
		size_t index = strContant.find(szSeperate);
		if ( strLine.length() )
		{
			CEventParser::GetInstance()->EventDo(strLine);
			nIndex ++;
		}
	}
}

BOOL CRouter::Start()
{
	if ( m_sk != INVALID_SOCKET ) closesocket(m_sk);
	m_bStart = FALSE;

	//string strIP = GetIP(WEB1);
	//if ( strIP.length() == 0 )
	//{
	//	strIP = GetIP(WEB2);
	//}
	//if ( strIP.length() == 0 )
	//{
	//	return FALSE;
	//}
	string strIP = WEB1;
	m_strChannel = CConfigManager::GetInstance()->GetHostGid();
	do 
	{
		m_sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if ( m_sk == INVALID_SOCKET ) break;

		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = inet_addr(strIP.c_str());
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(PORT);

		if ( ::connect(m_sk, (sockaddr*)&addrSrv, sizeof(addrSrv)) == -1 ) break;

		CloseHandle(CreateThread(NULL, NULL, ReadConnectionThreat, this, NULL, NULL));

		m_bStart = TRUE;
		string strHostName = CConfigManager::GetInstance()->GetHostName();
		//strHostName[strHostName.length()] = '\0';
		SetChannel(m_strChannel.c_str(), CConfigManager::GetInstance()->GetHostName());
	} while (FALSE);
	return m_bStart;
}

BOOL CRouter::Stop()
{
	if ( m_sk != INVALID_SOCKET )
	{
		closesocket(m_sk);
		m_sk = INVALID_SOCKET;
	}
	return TRUE;
}

BOOL CRouter::SetChannel( const char* pszChannel, const char* pszPcName )
{
	BOOL bSend = TRUE;
	if ( m_sk != INVALID_SOCKET )
	{
		string strContant = "connect ";
		strContant.append(pszChannel);
		strContant.append(" ");
		strContant.append(pszPcName);
		if ( SendSK(strContant.c_str()) )
		{
			bSend = TRUE;
		}
	}
	return bSend;
}

BOOL CRouter::Send( const char* pszContant )
{
	return SendSK(pszContant);
}

BOOL CRouter::Send( const WCHAR* pwszContant )
{
	return SendSK(WtoA(pwszContant).c_str());
}

BOOL CRouter::Send( const char* pszEvent, const char* pszContant )
{
	string strContant = pszEvent;
	strContant.append(" ");
	strContant.append(pszContant);
	return SendSK(strContant.c_str());
}

BOOL CRouter::Send( const char* pszEvent, const WCHAR* pszContant )
{
	string strContant = pszEvent;
	strContant.append(" ");
	strContant.append(WtoA(pszContant));
	return SendSK(strContant.c_str());
}

BOOL CRouter::Send( const char* pszEvent, const char* pszID, const char* pszContant )
{
	string strContant = pszEvent;
	strContant.append(" ");
	strContant.append(pszID);
	strContant.append(" ");
	strContant.append(pszContant);
	return SendSK(strContant.c_str());
}

BOOL CRouter::SendSK( const char* pszContant )
{
	CLogger::GetInstance()->Log("Barrage:Send", pszContant);

	BOOL bSend = FALSE;
	//if ( CUser::GetInstance()->GetUser().length() == 0 )
	//{//提示要登录;
	//	CLauncherWnd::GetInstance()->ShowLogin();
	//	return bSend;
	//}
	//if ( m_bBlank == TRUE )
	//{//提示要认证
	//	return bSend;
	//}
	if ( m_sk != INVALID_SOCKET )
	{
		string strUtf8 = AtoUTF8(pszContant);
		//static const char szSeperate[3] = {0x0d, 0x0a, '\0'};
		static const char szSeperate[2] = {0x0a, '\0'};
		strUtf8.append(szSeperate);
		bSend = (send(m_sk, strUtf8.c_str(), strUtf8.length(), 0) != SOCKET_ERROR);
		CLogger::GetInstance()->Log("Barrage:发送", bSend);
	}

	return bSend;
}

BOOL CRouter::GetAuth()
{
	return m_bAuth;
}

void CRouter::SetAuth()
{
	m_bAuth = TRUE;
}

string CRouter::GetIP(const char* pszWeb)
{ 
	HOSTENT* pHE = ::gethostbyname(pszWeb);
	if (NULL == pHE) return "";  
	return inet_ntoa(*((in_addr*)pHE->h_addr_list[0]));  
}

