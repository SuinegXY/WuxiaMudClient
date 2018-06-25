#ifndef _BASEINFO_H_
#define _BASEINFO_H_

#include <hash_map>

typedef vector<pair<string, wstring>> vecEvent;

class CBaseInfo
{
public:
	CBaseInfo();
	~CBaseInfo();

	wstring GetInfo(string& strID);
	DWORD GetRGBA(string& strID);
	vecEvent* GetEvent(const char* pszID);

protected:
	void SetInfo(const char* pszID, const WCHAR* pszContant);
	void SetRGBA(const char* pszID, DWORD dwRGBA);
	void SetEvent(const char* pszID, const char* pszEvent, const WCHAR* pszTitle);

private:
	hash_map<string, wstring> m_mapInfo;
	hash_map<string, DWORD> m_mapRGBA;
	hash_map<string, vecEvent*> m_mapEvent;
};

#endif