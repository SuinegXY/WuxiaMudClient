#ifndef _ITEMINFO_H_
#define _ITEMINFO_H_

#include "BaseInfo.h"

class CItemInfo : public CBaseInfo
{
	InstanceDefine(CItemInfo);
public:
	CItemInfo();

	wstring GetInfo(string& strID, string& strNum);
	wstring GetInfo(string& strID, int nNum);

protected:
	void SetInfo(const char* pszID, const WCHAR* pszContant, const WCHAR* pszClassifier);

private:
	hash_map<string, wstring> m_mapClassifier;
};

#endif