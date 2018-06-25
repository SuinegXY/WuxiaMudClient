#include "stdafx.h"
#include "Item.h"
#include "Util\ATW.h"

InstanceRealize(CItemInfo)

CItemInfo::CItemInfo()
{
}

void CItemInfo::init()
{
	SetInfo("20001", L"干粮", L"块");
	SetEvent("20001", "eat 20001", L"吃");
	SetInfo("20002", L"水袋", L"袋");
	SetEvent("20002", "drink 20002", L"喝");
	SetInfo("20003", L"银票", L"张");
	SetRGBA("20003", 0xff8d4329);
	SetInfo("20004", L"黄金", L"两");
	SetRGBA("20004", 0xffedf006);
	SetInfo("20005", L"白银", L"两");
	SetRGBA("20005", 0xffffffff);
	SetInfo("20006", L"铜板", L"枚");
	SetRGBA("20006", 0xffeea312);
	SetInfo("20010", L"金创药", L"瓶");
	SetEvent("20010", "use 20010", L"使用");
	SetInfo("20011", L"小还丹", L"瓶");
	SetEvent("20011", "use 20011", L"使用");
}

void CItemInfo::SetInfo( const char* pszID, const WCHAR* pszContant, const WCHAR* pszClassifier )
{
	m_mapClassifier.insert(make_pair(pszID, pszClassifier));
	return __super::SetInfo(pszID, pszContant);
}

std::wstring CItemInfo::GetInfo( string& strID, string& strNum )
{
	wstring strContant;
	if ( strNum.length() )
	{
		strContant = NumToCnString(atoi(strNum.c_str()));
		strContant.append(m_mapClassifier[strID]);
	}
	strContant.append(__super::GetInfo(strID));
	return strContant;
}

std::wstring CItemInfo::GetInfo( string& strID, int nNum )
{
	wstring strContant;
	if ( nNum )
	{
		strContant = NumToCnString(nNum);
		strContant.append(m_mapClassifier[strID]);
	}
	strContant.append(__super::GetInfo(strID));
	return strContant;
}
