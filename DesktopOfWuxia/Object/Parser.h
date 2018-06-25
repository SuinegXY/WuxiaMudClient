#ifndef _PARSER_H_
#define _PARSER_H_

#include <hash_map>

typedef void (*Callback)(const char*);
class CEventParser
{
	InstanceDefine(CEventParser);
public:
	CEventParser();
	~CEventParser();

	void EventReg(const char* pszEventName, Callback pCallback);
	void EventDo(string& strContant);

private:
	hash_map<string, Callback> m_mapEvent;
};

#endif