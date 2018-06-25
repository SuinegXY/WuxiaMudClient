#include "stdafx.h"
#include "Item.h"
#include "Util\ATW.h"

InstanceRealize(CItemInfo)

CItemInfo::CItemInfo()
{
}

void CItemInfo::init()
{
	SetInfo("20001", L"����", L"��");
	SetEvent("20001", "eat 20001", L"��");
	SetInfo("20002", L"ˮ��", L"��");
	SetEvent("20002", "drink 20002", L"��");
	SetInfo("20003", L"��Ʊ", L"��");
	SetRGBA("20003", 0xff8d4329);
	SetInfo("20004", L"�ƽ�", L"��");
	SetRGBA("20004", 0xffedf006);
	SetInfo("20005", L"����", L"��");
	SetRGBA("20005", 0xffffffff);
	SetInfo("20006", L"ͭ��", L"ö");
	SetRGBA("20006", 0xffeea312);
	SetInfo("20010", L"��ҩ", L"ƿ");
	SetEvent("20010", "use 20010", L"ʹ��");
	SetInfo("20011", L"С����", L"ƿ");
	SetEvent("20011", "use 20011", L"ʹ��");
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
