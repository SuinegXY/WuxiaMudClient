#include "stdAfx.h"
#include "ClientStatistic.h"
#include "ConfigManager.h"
#include "Util/HttpClient.h"
#include "Util/stdstring.h"
#include <process.h>
#include "Util/ATW.h"
#include "Logger.h"

InstanceRealize(CClientStatistic);

void CClientStatistic::init()
{
	if (NULL == m_hWorker)
	{
		m_hWorker = CreateSemaphore(NULL, 0, MAXLONG, _T("MobileSimulatorStatisticSemaphore"));
	}
	CloseHandle(CreateThread(NULL, 0, WorkerThread, this, 0, 0));
}

CClientStatistic::CClientStatistic()
{
	m_hWorker = NULL;
}

CClientStatistic::~CClientStatistic(void)
{
}

void CClientStatistic::SetManager( CConfigManager* pManager )
{
	m_pCfgManager = pManager;
}

DWORD WINAPI CClientStatistic::WorkerThread(LPVOID lParam)
{
	CClientStatistic* pTongji = (CClientStatistic*)lParam;
	if (pTongji)
	{
		HttpClient client;
		while(TRUE)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(pTongji->GetWorkerSemaphore(), INFINITE))
			{
				char* pParam = pTongji->GetStaticData();
				if (pParam)
				{
					string strRes;
					//if (CURLE_OK == client.Post(STNTS_TONGJI_URL, pParam, NULL, strRes))
					//{
					//	CLogger::GetInstance()->Log("Statistic", pParam);
					//}
					pTongji->RemoveStaticData();
					delete pParam;
					pParam = NULL;
				}
			}
		}
	}
	return 0;
}

BOOL CClientStatistic::PostStaticData(char* szUrl)
{
	if (szUrl)
	{
		CStdStringA strUrl = szUrl;
		strUrl.Replace("(null)", "");
		char* pData = (char*)malloc(strUrl.length()+1);
		if ( pData )
		{
			strcpy(pData, strUrl.c_str());
			m_arrData.Add(pData);
			LONG dwPrev = 0;
			ReleaseSemaphore(m_hWorker, 1, &dwPrev);
			return TRUE;
		}
	}
	return FALSE;
}

char* CClientStatistic::GetStaticData()
{
	if (m_arrData.GetSize() > 0)
	{
		return (char*)m_arrData.GetAt(0);
	}
	return NULL;
}

BOOL CClientStatistic::RemoveStaticData()
{
	if (m_arrData.GetSize() > 0)
	{
		m_arrData.Remove(0);
		return TRUE;
	}
	return FALSE;
}

HANDLE CClientStatistic::GetWorkerSemaphore()
{
	return m_hWorker;
}

BOOL CClientStatistic::StatisticOver()
{
	if ( m_arrData.GetSize() == 0 )
	{
		return TRUE;
	}
	return FALSE;
}

std::string CClientStatistic::GetTime()
{
	char szTime[32] = {0};
	__time64_t timeSrc = (__time64_t)m_pCfgManager->GetUnixTime();
	struct tm tTime;
	localtime_s(&tTime,&timeSrc);
	strftime(szTime, 32, "%Y-%m-%d %H:%M:%S", &tTime);
	return string(szTime);
}

const char* CClientStatistic::GetStringData( string& strContent )
{
	if ( strContent.length() )
	{
		return strContent.c_str();
	}
	return NULL;
}

void CClientStatistic::PackStartup( EnumStartupType est )
{
	if (m_pCfgManager)
	{
		char szUrl[1024] = {0};
		//sprintf(szUrl, STNTS_TONGJI_STARTUP, m_pCfgManager->GetHostGid(), m_pCfgManager->GetHostMacAddress(), m_pCfgManager->GetHostName(), 
		//	m_pCfgManager->GetStartId(), GetTime().c_str(), m_pCfgManager->GetPeriodId(), m_pCfgManager->GetVersion(), m_pCfgManager->GetChannelId(),
		//	NULL, est);
		//PostStaticData(szUrl);
	}
}

void CClientStatistic::PackGameStart(const char* pszPackage)
{
	if ( m_pCfgManager && pszPackage )
	{
		m_strPackage = pszPackage;
		m_nPackageVersion = -1;
		//m_nPackageVersion = CADBHandler::GetInstance()->GetPackageVersion(pszPackage);
		m_strStartTime	= GetTime();

		//char szUrl[1024] = {0};
		//sprintf(szUrl, STNTS_TONGJI_GAMEINFO, m_pCfgManager->GetHostGid(), m_pCfgManager->GetHostMacAddress(), m_pCfgManager->GetHostName(), 
		//	m_pCfgManager->GetStartId(), GetTime().c_str(), m_pCfgManager->GetPeriodId(), m_pCfgManager->GetVersion(), m_pCfgManager->GetChannelId(),
		//	pszPackage, m_nPackageVersion, m_strStartTime.c_str(), NULL);
		//PostStaticData(szUrl);
	}
}

void CClientStatistic::PackGameEnd()
{
	if ( m_pCfgManager && m_strPackage.length() )
	{
		//char szUrl[1024] = {0};
		//sprintf(szUrl, STNTS_TONGJI_GAMEINFO, m_pCfgManager->GetHostGid(), m_pCfgManager->GetHostMacAddress(), m_pCfgManager->GetHostName(), 
		//	m_pCfgManager->GetStartId(), GetTime().c_str(), m_pCfgManager->GetPeriodId(), m_pCfgManager->GetVersion(), m_pCfgManager->GetChannelId(),
		//	m_strPackage.c_str(), m_nPackageVersion, m_strStartTime.c_str(), GetTime().c_str());
		//PostStaticData(szUrl);

		m_strPackage = "";
		m_nPackageVersion = -1;
		m_strStartTime = "";
	}
}

void CClientStatistic::PackGameChange(const char* pszPackage)
{
	PackGameEnd();
	PackGameStart(pszPackage);
}

void CClientStatistic::PackKeyEvent( EnumKeyEvent eke, BOOL bHotkey )
{
	if (m_pCfgManager)
	{
		//char szUrl[1024] = {0};
		//sprintf(szUrl, STNTS_TONGJI_ACTIONINFO, m_pCfgManager->GetHostGid(), m_pCfgManager->GetHostMacAddress(), m_pCfgManager->GetHostName(), 
		//	m_pCfgManager->GetStartId(), GetTime().c_str(), m_pCfgManager->GetPeriodId(), m_pCfgManager->GetVersion(), m_pCfgManager->GetChannelId(),
		//	GetStringData(m_strPackage), m_nPackageVersion, eke, bHotkey);
		//PostStaticData(szUrl);
	}
}

void CClientStatistic::PackUserAction( EnumUserAction eua )
{
	if ( m_pCfgManager == NULL ) return;
	//char szUrl[1024] = {0};
	//sprintf(szUrl, STNTS_TONGJI_USERACTION, m_pCfgManager->GetHostGid(), m_pCfgManager->GetHostMacAddress(), m_pCfgManager->GetHostName(), 
	//	m_pCfgManager->GetStartId(), GetTime().c_str(), m_pCfgManager->GetPeriodId(), m_pCfgManager->GetVersion(), m_pCfgManager->GetChannelId(),
	//	NULL, NULL, eua);
	//PostStaticData(szUrl);
}
