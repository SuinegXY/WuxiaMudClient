#ifndef _CONVERSATION_H_
#define _CONVERSATION_H_
#include <string>
#include <Windows.h>
using std::string;

class CConversation
{
public:
	CConversation();
	~CConversation();

public:
	string GetConversation(LPCSTR lpszGid , UINT64 dwMAC);

private:
	string	GetTime();
	BOOL	GetProcessID(LPCTSTR pProcessName, DWORD dwParentProcessID , DWORD&	ProcessID);
	DWORD	GetSystemProcessId();
};

#endif