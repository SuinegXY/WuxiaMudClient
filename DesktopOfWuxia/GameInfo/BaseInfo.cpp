#include "stdafx.h"
#include "BaseInfo.h"

CBaseInfo::CBaseInfo()
{

}

CBaseInfo::~CBaseInfo()
{
	hash_map<string, vecEvent*>::iterator pMapEvent = m_mapEvent.begin();
	while ( pMapEvent != m_mapEvent.end() )
	{
		if ( pMapEvent->second )
		{
			delete pMapEvent->second;
			pMapEvent->second = NULL;
		}
		++ pMapEvent;
	}
}

wstring CBaseInfo::GetInfo( string& strID )
{
	return m_mapInfo[strID];
}

DWORD CBaseInfo::GetRGBA( string& strID )
{
	DWORD dwRGBA = m_mapRGBA[strID];
	return dwRGBA ? dwRGBA : 0xffcccccc;
}

void CBaseInfo::SetInfo( const char* pszID, const WCHAR* pszContant )
{
	m_mapInfo.insert(make_pair(pszID, pszContant));
}

void CBaseInfo::SetRGBA( const char* pszID, DWORD dwRGBA )
{
	m_mapRGBA.insert(make_pair(pszID, dwRGBA));
}

vecEvent* CBaseInfo::GetEvent( const char* pszID )
{
	return m_mapEvent[pszID];
}

void CBaseInfo::SetEvent( const char* pszID, const char* pszEvent, const WCHAR* pszTitle )
{
	if ( m_mapEvent[pszID] == NULL )
	{
		vecEvent* pVec = new vecEvent;
		pVec->push_back(make_pair(pszEvent, pszTitle));
		m_mapEvent[pszID] = pVec;
	}
	else
	{
		m_mapEvent[pszID]->push_back(make_pair(pszEvent, pszTitle));
	}
}
