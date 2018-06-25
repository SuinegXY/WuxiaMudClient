#ifndef _MAC_H_
#define _MAC_H_
#include <Windows.h>
#include <map>
using std::map;

class CMAC
{
public:
	CMAC();
	~CMAC();

public:
	UINT64	GetMAC();
private:
	VOID		StartNet();
	UINT32		DnsToIP(const char *Host ,UINT32	dwMaxWaitTime );
	LSTATUS		GetIPMACList( map<UINT32 , UINT64>& IP_MACMapList);
	VOID		StopNet();
};

#endif