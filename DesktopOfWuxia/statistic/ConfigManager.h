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
	//����ID
	char					m_szChannelId[32];
	//����ID
	char					m_szStartId[64];
	//����ID
	char					m_szActiveId[64];
	//����ID
	char					m_szInsertId[64];
	//��������ID
	char					m_szPeriodId[64];
	//����httpserver��url
	TCHAR					m_szServerUrl[MAX_PATH];
	//��������ϵͳ
	char					m_szHostOS[32];
	//����MAC��ַ
	char					m_szHostMac[32];
	//��������ID
	char					m_szHostGid[32];
	//����IP��ַ
	char					m_szHostIP[16];
	//����������
	char					m_szHostName[128];
	//����汾
	char					m_szVersion[32];
	//������Դ·��
	TCHAR					m_szLocalPath[MAX_PATH];
	//�������ְ汾
	DWORD					m_dwVersion;
};

#endif

