#ifndef _NPCINFO_H_
#define _NPCINFO_H_

#include "BaseInfo.h"

class CNpcInfo : public CBaseInfo
{
	InstanceDefine(CNpcInfo);
public:
	CNpcInfo();

	void SetAsk(const char* pszID, const char* pszWhat);
	wstring GetExpDescribe(string& strExp);
};

#endif