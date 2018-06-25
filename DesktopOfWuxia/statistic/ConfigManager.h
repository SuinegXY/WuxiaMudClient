#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#define ValueDefineAndGet(type, name, prefix)\
public:\
##type Get##name()\
	{\
	return m_##prefix##name;\
	}\
private:\
##type m_##prefix##name;

#define ValueDefineAndGetArray(type, name, prefix, num)\
public:\
##type* Get##name()\
	{\
	return m_##prefix##name;\
	}\
private:\
##type m_##prefix##name;

#define ValueDefineSetAndGet(type, name, prefix)\
public:\
##type Get##name()\
	{\
	return m_##prefix##name;\
	}\
	void Set##name(##type value)\
	{\
	m_##prefix##name = value;\
	}\
private:\
##type m_##prefix##name;

class CConfigManager
{
public:
	CConfigManager(void);
	~CConfigManager(void);

	static CConfigManager* GetInstance();

	void Init();

	UINT64	GetMac();
	char*	GetChannelId();
	BOOL	GetProcessID(LPCTSTR pProcessName , DWORD dwParentProcessID , DWORD& ProcessID );
	DWORD	GetSystemProcessId();
	char*	GetStartId();
	string	GetTime();
	char*	GetPeriodId();
	char*	GetHostMacAddress();
	char*	GetHostGid();
	char*	GetHostOS();
	char*	GetHostIP();
	char*	GetHostName();
	int		GetVersionNumber();
	char*	GetVersion();
	DWORD	GetUnixTime();

private:
	static CConfigManager* s_pThis;
	//渠道ID
	char					m_szChannelId[32];
	//启动ID
	char					m_szStartId[64];
	//激活ID
	char					m_szActiveId[64];
	//插入ID
	char					m_szInsertId[64];
	//启动周期ID
	char					m_szPeriodId[64];
	//保存httpserver的url
	TCHAR					m_szServerUrl[MAX_PATH];
	//本机操作系统
	char					m_szHostOS[32];
	//本机MAC地址
	char					m_szHostMac[32];
	//本机网吧ID
	char					m_szHostGid[32];
	//本机IP地址
	char					m_szHostIP[16];
	//本机主机名
	char					m_szHostName[128];
	//程序版本
	char					m_szVersion[32];
	//本地资源路径
	TCHAR					m_szLocalPath[MAX_PATH];
	//程序数字版本
	DWORD					m_dwVersion;
};

#endif

