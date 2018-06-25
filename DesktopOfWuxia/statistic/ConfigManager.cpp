#include "stdafx.h"
#include <shlobj.h>
#include <io.h>
#include <time.h> 
#include <stdlib.h>
#include "common.h"
#include "ConfigManager.h"
#include "Util/MAC.H"
#include "Util/MD5.h"
#include "Util/stdstring.h"

#pragma warning(disable:4996)

#define				DEFAULT_CHANNEL_ID				"9000000001"
#define				DEFAULT_NETBAR_ID				"10000"

CConfigManager* CConfigManager::s_pThis = NULL;

CConfigManager* CConfigManager::GetInstance()
{
	if ( s_pThis == NULL )
	{
		s_pThis = new CConfigManager;
	}
	return s_pThis;
}

CConfigManager::CConfigManager(void)
{
	s_pThis = this;
	memset(m_szChannelId, 0, 32);
	memset(m_szStartId, 0, 64);
	memset(m_szPeriodId, 0, 64);
	memset(m_szInsertId, 0, 64);
	memset(m_szServerUrl,0, MAX_PATH);
	memset(m_szHostOS, 0, 32);
	memset(m_szHostMac, 0, 32);
	memset(m_szHostGid, 0, 32);
	memset(m_szHostIP, 0, 16);
	memset(m_szHostName, 0, 128);
	memset(m_szVersion, 0 ,32);
	memset(m_szActiveId, 0 ,64);
	memset(m_szLocalPath, 0, MAX_PATH);
	m_dwVersion = 0;
}


CConfigManager::~CConfigManager(void)
{
}

void	CConfigManager::Init()
{
	GetChannelId();
	GetStartId();
	GetHostMacAddress();
	GetHostGid();
	GetHostOS();
	GetHostIP();
	GetHostName();
	GetVersionNumber();
	GetVersion();

	string strBit;
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if ( si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		strBit = "64_";
	}
	else
	{	
		strBit = "32_";
	}

	string strOS;
	SYSTEM_INFO info;        //用SYSTEM_INFO结构判断64位AMD处理器 
	GetSystemInfo(&info);    //调用GetSystemInfo函数填充结构 
	OSVERSIONINFOEX os; 
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);   
	strOS = ("unknown OperatingSystem.");
	if(GetVersionEx((OSVERSIONINFO *)&os))
	{ 
		//下面根据版本信息判断操作系统名称 
		switch(os.dwMajorVersion)//判断主版本号
		{
		case 4:
			switch(os.dwMinorVersion)//判断次版本号 
			{ 
			case 0:
				if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)
					strOS = ("Microsoft_Windows_NT4.0"); //1996年7月发布 
				else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
					strOS = ("win95");
				break;
			case 10:
				strOS = ("win98");
				break;
			case 90:
				strOS = ("winme");
				break;
			}
			break;

		case 5:
			switch(os.dwMinorVersion)	//再比较dwMinorVersion的值
			{ 
			case 0:
				strOS = ("win2000");//1999年12月发布
				break;

			case 1:
				strOS = ("winxp");//2001年8月发布Microsoft_Windows_XP
				break;

			case 2:
				if(os.wProductType==VER_NT_WORKSTATION 
					&& info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
				{
					strOS = ("Microsoft_WindowsXP_Professional_x64_Edition");
				}
				else if(GetSystemMetrics(SM_SERVERR2)==0)
					strOS = ("Microsoft_WindowsServer_2003");//2003年3月发布 
				else if(GetSystemMetrics(SM_SERVERR2)!=0)
					strOS = ("Microsoft_WindowsServer_2003R2");
				break;
			}
			break;

		case 6:
			switch(os.dwMinorVersion)
			{
			case 0:
				if(os.wProductType == VER_NT_WORKSTATION)
					strOS = ("vista");
				else
					strOS = ("Microsoft_WindowsServer_2008");//服务器版本 
				break;
			case 1:
				if(os.wProductType == VER_NT_WORKSTATION)
					strOS = ("win7");//Microsoft_Windows_7
				else
					strOS = ("Microsoft_WindowsServer_2008R2");
				break;
			}
			break;
		}
	}
}

//得到渠道IDyi j 
char*	CConfigManager::GetChannelId()
{
	if (m_szChannelId[0] != 0)
	{
		//渠道ID已存在，直接返回
		return m_szChannelId;
	}
	//默认渠道ID
	strcpy_s(m_szChannelId, DEFAULT_CHANNEL_ID);
	return m_szChannelId;
}


BOOL	CConfigManager::GetProcessID(LPCTSTR pProcessName , DWORD dwParentProcessID , DWORD&	ProcessID )
{
	HANDLE						hSnapshot	= INVALID_HANDLE_VALUE;
	PROCESSENTRY32				Pro32		= {0};

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hSnapshot == INVALID_HANDLE_VALUE )	return FALSE;
	Pro32.dwSize = sizeof(PROCESSENTRY32);
	if(Process32First(hSnapshot , &Pro32))
	{
		do 
		{
			if(lstrcmpi(pProcessName , Pro32.szExeFile) == 0 &&Pro32.th32ParentProcessID == dwParentProcessID)
			{
				ProcessID = Pro32.th32ProcessID;
				CloseHandle(hSnapshot);
				return TRUE;
			}
		}while (Process32Next(hSnapshot , &Pro32));
	}
	CloseHandle(hSnapshot);
	return FALSE;//没找到
}


DWORD	CConfigManager::GetSystemProcessId()
{
	DWORD		dwProcessID = 0;
	DWORD		SystemProcessID;

	GetProcessID(_T("system") , 0 , SystemProcessID);
	if(SystemProcessID == 0 )	SystemProcessID = 4;//默认为4

	GetProcessID(_T("smss.exe") , SystemProcessID , dwProcessID);

	if(dwProcessID == 0 )	SystemProcessID = 8;
	GetProcessID(_T("smss.exe") , SystemProcessID , dwProcessID);

	return dwProcessID;
}

typedef struct  
{  
	LARGE_INTEGER liKeBootTime;  
	LARGE_INTEGER liKeSystemTime;  
	LARGE_INTEGER liExpTimeZoneBias;  
	ULONG uCurrentTimeZoneId;  
	DWORD dwReserved;  
} SYSTEM_TIME_INFORMATION;   

typedef long (__stdcall *fnNtQuerySystemInformation)(
	IN  UINT SystemInformationClass,
	OUT PVOID SystemInformation,
	IN  ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL); 
static fnNtQuerySystemInformation NtQuerySystemInformation = NULL; 

string	CConfigManager::GetTime()
{
	CHAR szTime[128] = {0};

	NtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQuerySystemInformation");
	if ( NtQuerySystemInformation )
	{
		SYSTEM_TIME_INFORMATION sti;  
		#define SystemTimeInformation 3  
		LONG status = NtQuerySystemInformation(SystemTimeInformation, &sti, sizeof(sti), 0);

		if( NO_ERROR == status )  
		{
			FILETIME ft;  
			SYSTEMTIME SystemTime;  
			memcpy(&ft, &sti.liKeBootTime, sizeof(ft));  
			FileTimeToLocalFileTime(&ft, &ft);  
			FileTimeToSystemTime(&ft, &SystemTime);  

			sprintf_s(szTime , _countof(szTime) , "%d:%d:%d:%d-%d/%d/%d" , 
				SystemTime.wMilliseconds , 
				SystemTime.wSecond,
				SystemTime.wMinute,
				SystemTime.wHour,
				SystemTime.wDay,
				SystemTime.wMonth,
				SystemTime.wYear
				);
		}
	}
	else
	{
		DWORD		dwProcessID = 0;
		HANDLE		hProcess = NULL;
		FILETIME	CreateTime , ExitTime , KernelTime , UserTime  ,LocalFileTime;
		SYSTEMTIME	SystemTime;

		do 
		{
			dwProcessID = GetSystemProcessId();
			if(dwProcessID == 0 )	break;

			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION , FALSE , dwProcessID);
			if(hProcess == NULL )	break;

			if(!GetProcessTimes(hProcess , &CreateTime , &ExitTime , &KernelTime , &UserTime))	break;

			FileTimeToLocalFileTime(&CreateTime , &LocalFileTime);
			FileTimeToSystemTime(&LocalFileTime , &SystemTime);

			sprintf_s(szTime , _countof(szTime) , "%d:%d:%d:%d-%d/%d/%d" , 
				SystemTime.wMilliseconds , 
				SystemTime.wSecond,
				SystemTime.wMinute,
				SystemTime.wHour,
				SystemTime.wDay,
				SystemTime.wMonth,
				SystemTime.wYear
				);
		} while (FALSE);

		if(hProcess)	CloseHandle(hProcess);
	}

	return szTime;
}

//生成方式与bootid方式一致
char*	CConfigManager::GetStartId()
{
	if (m_szStartId[0]!=0)
	{
		//启动ID已存在，直接返回
		return m_szStartId;
	}
	return m_szStartId;
}

char*	CConfigManager::GetPeriodId()
{
	if (m_szPeriodId[0]!=0)
	{
		//启动ID已存在，直接返回
		return m_szPeriodId;
	}
	return m_szPeriodId;
}

//得到本机的MAC地址
char*	CConfigManager::GetHostMacAddress()
{
	if (m_szHostMac[0] != 0)
	{
		return m_szHostMac;
	}
	CMAC mac;
	UINT64 macinfo = mac.GetMAC();
	BYTE *macarry = (BYTE*)&macinfo;
	sprintf(m_szHostMac,"%02x-%02x-%02x-%02x-%02x-%02x",macarry[0],macarry[1],macarry[2],macarry[3],macarry[4],macarry[5]);
	return m_szHostMac;
}

//获取网吧ID
char*	CConfigManager::GetHostGid()
{
	if (m_szHostGid[0] != 0)
	{
		return m_szHostGid;
	}
	//都没有拿到，返回一个默认的GID
	strcpy_s(m_szHostGid, DEFAULT_NETBAR_ID);
	return m_szHostGid;
}

//获取本机操作系统
char*	CConfigManager::GetHostOS()
{
	if (m_szHostOS[0] != 0)
	{
		return m_szHostOS;
	}
    SYSTEM_INFO info;                                   //用SYSTEM_INFO结构判断64位AMD处理器 
    GetSystemInfo(&info);                               //调用GetSystemInfo函数填充结构 
    OSVERSIONINFOEX os; 
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);		/*在调用函数前必须用sizeof(OSVERSIONINFOEXA)填充dwOSVersionInfoSize结构成员*/ 
    if(GetVersionEx((OSVERSIONINFO *)&os))              /*调用GetVersionEx函数OSVERSIONINFOEX结构必须将指针类型强制转换*/
    {
        //下面根据版本信息判断操作系统名称
        switch(os.dwMajorVersion)
		{                     
			//判断主版本号 
            case 4: 
                switch(os.dwMinorVersion)
				{                
					//判断次版本号 
                    case 0: 
                        if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)
                            strcpy(m_szHostOS, "Microsoft Windows NT 4.0");                //1996年7月发布 
                        else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS) 
                            strcpy(m_szHostOS, "Microsoft Windows 95"); 
                        break; 
                    case 10: 
                        strcpy(m_szHostOS, "Microsoft Windows 98"); 
                        break; 
                    case 90: 
                        strcpy(m_szHostOS, "Microsoft Windows Me"); 
                        break; 
                } 
                break; 
            case 5: 
                switch(os.dwMinorVersion)
				{               //再比较dwMinorVersion的值 
                    case 0: 
                       strcpy(m_szHostOS, "Microsoft Windows 2000");                    //1999年12月发布 
                        break; 
                    case 1: 
                        strcpy(m_szHostOS, "Microsoft Windows XP");                    //2001年8月发布 
                        break; 
                    case 2: 
                        if(os.wProductType==VER_NT_WORKSTATION && info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
                            strcpy(m_szHostOS, "Microsoft Windows XP Professional x64 Edition"); 
                        else if(GetSystemMetrics(SM_SERVERR2)==0) 
                            strcpy(m_szHostOS, "Microsoft Windows Server 2003");        //2003年3月发布 
                        else if(GetSystemMetrics(SM_SERVERR2)!=0) 
                            strcpy(m_szHostOS, "Microsoft Windows Server 2003 R2"); 
                        break; 
                } 
                break; 
            case 6: 
                switch(os.dwMinorVersion){ 
                    case 0: 
                        if(os.wProductType==VER_NT_WORKSTATION)/*

                        VER_NT_WORKSTATION是桌面系统 */
                            strcpy(m_szHostOS, "Microsoft Windows Vista"); 
                        else 
                            strcpy(m_szHostOS, "Microsoft Windows Server 2008");          //服务器版本 
                        break; 
                    case 1: 
                        if(os.wProductType==VER_NT_WORKSTATION) 
                            strcpy(m_szHostOS, "Microsoft Windows 7"); 
                        else 
                            strcpy(m_szHostOS, "Microsoft Windows Server 2008 R2"); 
                        break; 
                } 
                break; 
            default:
                strcpy(m_szHostOS, "未知操作系统"); 
        }
    } 
	return m_szHostOS;
}

//获取本机IP地址
char*	CConfigManager::GetHostIP()
{
	if (m_szHostIP[0] != 0)
	{
		return m_szHostIP;
	}
	WSADATA wsaData;  
	char name[155];      //定义用于存放获得主机名的变量   
	PHOSTENT hostinfo;   
	if (WSAStartup(MAKEWORD(2,0), &wsaData) == 0)   
	{
		if(gethostname(name, sizeof(name)) == 0)   
		{
			if((hostinfo = gethostbyname(name)) != NULL)   
			{
				strcpy(m_szHostIP, inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list));
			}  
		}  
		WSACleanup();
	}
	return m_szHostIP;
}

//获取主机名
char*	CConfigManager::GetHostName()
{
	if (m_szHostName[0] != 0)
	{
		return m_szHostName;
	}
	gethostname(m_szHostName, 128);
	return m_szHostName;
}

//获取程序版本号
int		CConfigManager::GetVersionNumber()
{
	if (m_dwVersion != 0)
	{
		return m_dwVersion;
	}
	DWORD dwVersion = 0;
	TCHAR* pBuf = NULL;
	VS_FIXEDFILEINFO* pVsInfo = NULL;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	TCHAR szFileName[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	int iVerInfoSize = GetFileVersionInfoSize(szFileName,NULL);   
	if(iVerInfoSize!= 0)
	{
		pBuf = new TCHAR[iVerInfoSize];
		if (pBuf)
		{
			if(GetFileVersionInfoW(szFileName, 0, iVerInfoSize, pBuf))     
			{     
				if(VerQueryValue(pBuf, _T("\\"),(void**)&pVsInfo,&iFileInfoSize))     
				{
					dwVersion += HIWORD(pVsInfo->dwFileVersionMS) * 1000;
					dwVersion += LOWORD(pVsInfo->dwFileVersionMS) * 100;
					dwVersion += HIWORD(pVsInfo->dwFileVersionLS) * 10;
					dwVersion += LOWORD(pVsInfo->dwFileVersionLS);
				}
			}     
			delete   pBuf;
			pBuf = NULL;
		}
	}
	return (m_dwVersion = dwVersion);
}

//获取程序版本号
char*	CConfigManager::GetVersion()
{
	if (m_szVersion[0] != 0)
	{
		return m_szVersion;
	}
	TCHAR* pBuf = NULL;
	VS_FIXEDFILEINFO* pVsInfo = NULL;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	TCHAR szFileName[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	int iVerInfoSize = GetFileVersionInfoSize(szFileName,NULL);   
	if(iVerInfoSize!= 0)
	{
		pBuf = new TCHAR[iVerInfoSize];  
		if(GetFileVersionInfoW(szFileName, 0, iVerInfoSize, pBuf))     
		{     
			if(VerQueryValue(pBuf, _T("\\"),(void**)&pVsInfo,&iFileInfoSize))     
			{
				sprintf(m_szVersion, "%d.%d.%d.%d", 
					HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), 
					HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
			}
		}     
		delete   pBuf;
		pBuf = NULL;
	}
	else
	{
		sprintf(m_szVersion, "%d.%d.%d.%d", 1, 0, 0, 0);
	}
	return m_szVersion;
}

//获取Unix时间戳
DWORD	CConfigManager::GetUnixTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
	return (DWORD)mktime(&gm);
}

UINT64 CConfigManager::GetMac()
{
	CMAC mac;
	UINT64 un64Mac = mac.GetMAC();

	return un64Mac;
}

