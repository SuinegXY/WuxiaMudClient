#include "stdafx.h"
#include "Parser.h"
#include "Define.h"
#include "Logger.h"
#include "Util\ATW.h"

InstanceRealize(CEventParser);

void CEventParser::init()
{

}

CEventParser::CEventParser()
{

}

CEventParser::~CEventParser()
{

}

void CEventParser::EventReg( const char* pszEventName, Callback pCallback )
{
	if ( pszEventName == NULL || *pszEventName == NULL || pCallback == NULL ) return;
	m_mapEvent.insert(make_pair(pszEventName, pCallback));
}

void CEventParser::EventDo( string& strContant )
{
	CLogger::GetInstance()->Log("收到消息", strContant.c_str());
	string strEventName = GetStringHead(strContant, EVENT_SEPERATE);
	if ( strEventName.length() )
	{
		Callback pCallback = m_mapEvent[strEventName];
		if ( pCallback )
		{
			pCallback(strContant.length() ? strContant.c_str() : NULL);
		}
		else
		{
			CLogger::GetInstance()->Log(__FUNCTION__, strEventName.c_str(), "事件回调未找到");
		}
	}
}
