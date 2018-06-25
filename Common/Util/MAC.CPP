#include "stdafx.h"
#include "MAC.H"
#include <IPHlpApi.h>
#include <WinInet.h>
#include <string>
#pragma comment(lib,"Iphlpapi.lib")

CMAC::CMAC()
{
	StartNet();
}

CMAC::~CMAC()
{
	StopNet();
}

UINT64	CMAC::GetMAC()
{
	SOCKET				Socket;
	sockaddr			SockAddr;
	UINT32				IP;
	int					Len;
	map<UINT32 , UINT64>	IPMACList;
	UINT64				MAC;

	GetIPMACList(IPMACList);

	if(IPMACList.size() == 0 )	return 0; //在本机没有找到网卡

	MAC = IPMACList.begin()->second;

	if(IPMACList.size() == 1 )	return MAC;//只有一块网卡，就直接返回

	Socket = socket(AF_INET,SOCK_STREAM,0);
	if(Socket == INVALID_SOCKET)
		return MAC;//如果错误，就返回第一块网卡
	ZeroMemory((void *)&SockAddr,sizeof(sockaddr));
	((sockaddr_in *)&SockAddr)->sin_family = AF_INET; 
	((sockaddr_in *)&SockAddr)->sin_port = htons(80);
	IP = DnsToIP("www.baidu.com" , 4000 );
	if(IP == 0 )	return MAC;
	((sockaddr_in *)&SockAddr)->sin_addr.s_addr=IP;

	if( ::connect( Socket , &SockAddr , sizeof(sockaddr) ) == SOCKET_ERROR )
	{
		closesocket(Socket);
		return MAC;//如果错误，就返回第一块网卡
	}
	Len = sizeof(sockaddr);
	getsockname(Socket ,&SockAddr , &Len);

	IP = ((sockaddr_in *)&SockAddr)->sin_addr.s_addr;

	closesocket(Socket);

	if ( IPMACList[IP] )
	{
		return IPMACList[IP];
	}
	else
	{
		return MAC;
	}
}

VOID	CMAC::StartNet()
{
	WSADATA				WsaData;
	WSAStartup( 0x0202 , &WsaData );
}

UINT32	CMAC::DnsToIP(const char *Host ,UINT32	dwMaxWaitTime )
{

	char                    *P;
	struct hostent          *pHostent;
	UINT32					Tick = GetTickCount();
	LSTATUS					lStatus;

	while(GetTickCount() - Tick < dwMaxWaitTime )
	{
		pHostent = gethostbyname(Host);
		if( pHostent != NULL )
		{
			P = pHostent->h_addr_list[0];
			return *((UINT32 *) P);
		}
		lStatus = GetLastError();
		Sleep( 100 );
		SetLastError(lStatus);
	}

	return 0;
}

LSTATUS	CMAC::GetIPMACList( map<UINT32 , UINT64>& IP_MACMapList)
{
	PIP_ADAPTER_INFO		pAdapterInfo = NULL;
	PIP_ADAPTER_INFO		pAdapter = NULL;
	ULONG					ulOutBufLen;
	UINT32					IP;
	UINT64					MAC;
	LSTATUS					lStatus;

	do 
	{
		ulOutBufLen = sizeof (IP_ADAPTER_INFO);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if(pAdapterInfo == NULL )	break;

		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			free(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
			if (pAdapterInfo == NULL)	break;
		}

		if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != NO_ERROR ) //读取适配器信息
			break;

		pAdapter = pAdapterInfo;
		while( pAdapter )
		{
			MAC = *(UINT64*)pAdapter->Address;

			std::string strDescription = pAdapter->Description;
			if ( MAC == 0 )
			{
				pAdapter = pAdapter->Next;
				continue;
			}

			IP = inet_addr(pAdapter->IpAddressList.IpAddress.String);
			if(IP != 0 ) 
			{
				IP_MACMapList[IP] = MAC;
			}
			pAdapter = pAdapter->Next;
		}

	} while (FALSE);
	lStatus = GetLastError();
	if(pAdapterInfo)	free(pAdapterInfo);
	return lStatus;
}

VOID	CMAC::StopNet()
{
	WSACleanup();
}