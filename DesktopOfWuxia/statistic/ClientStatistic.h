#pragma once

enum EnumKeyEvent
{
	EKE_NONE		= 0,
	EKE_TOP_OPEN	= 10,
	EKE_TOP_CLOSE	= 11,
	EKE_CONFIG_POP	= 20,
	EKE_SETTING		= 21,
	EKE_FEEDBACK	= 22,
	EKE_KEY_MANAGER = 30,
	EKE_KEY_OPEN	= 31,
	EKE_KEY_CLOSE	= 32,
	EKE_KEY_VOLUME	= 33,
	EKE_KEY_SHAKE	= 34,
	EKE_KEY_INSTALL	= 35,
	EKE_KEY_MULTI	= 36,
	EKE_KEY_LOGIN	= 37,
	EKE_KEY_MUTE	= 38,
	EKE_KEY_FWZS	= 39,
	EKE_SCREEN_SHOT	= 40,
	EKE_KEY_LOGIN_BARRAGE = 41,
	EKE_KEY_CHIJI	= 42,
	EKE_KEY_CHIJI2	= 43,
	EKE_KEY_CLOUD1	= 44,
	EKE_KEY_CLOUD2	= 45,
	EKE_KEY_KEYSHOW = 46,
	EKE_KEY_UPLOAD1 = 47,
	EKE_KEY_UPLOAD2 = 48,
	EKE_KEY_APLAY1	= 49,
	EKE_KEY_APLAY2	= 51,
	EKE_KEY_BARRAGE_OPEN	= 52,
	EKE_KEY_BARRAGE_CLOSE	= 53,
	EKE_BOSSKEY		= 50,
	EKE_FULLWINDOW_OPEN		= 60,
	EKE_FULLWINDOW_CLOSE	= 61,
	//EKE_GOBACK		= 70,
	//EKE_GOHOME		= 71,
	EKE_SWITH		= 72,
	EKE_QR_OPEN		= 80,
	EKE_AD_LOAD		= 90,
	EKE_AD_EXIT		= 91,
	EKE_AD_DOUYU	= 92,
	EKE_USER_LOGIN	= 100,
	EKE_USER_SETUP	= 101,
	EKE_USER_SETDOWN= 102,
	EKE_USER_CENTER	= 103,
	EKE_USER_LOGOUT	= 104,
	EKE_USER_BIND	= 105,
};

enum EnumStartupType
{
	EST_NONE	= 0,
	EST_START	= 1,
	EST_ENGINE	= 2,
	EST_CLOSE	= 3,
};

enum EnumUserAction
{
	EUA_LOGIN_NORMAL = 1,
	EUA_LOGIN_PHONE,
	EUA_LOGIN_QQ,
	EUA_LOGIN_WX,
	EUA_LOGIN_WB,
	EUA_KEYSET_UP = 2001,
	EUA_KEYSET_DOWN,
};

class CConfigManager;
class CClientStatistic
{
	InstanceDefine(CClientStatistic);
public:
	CClientStatistic();
	~CClientStatistic(void);
	void SetManager(CConfigManager* pManager);

public:
	BOOL	StatisticOver();

	void	PackStartup(EnumStartupType est);
	void	PackGameStart(const char* pszPackage);
	void	PackGameEnd();
	void	PackGameChange(const char* pszPackage);
	void	PackKeyEvent(EnumKeyEvent eke, BOOL bHotkey = FALSE);
	void	PackInstallToPhone(int nState, const char* pszPackage = NULL );
	void	PackUserAction(EnumUserAction eua);

protected:
	static DWORD WINAPI WorkerThread(LPVOID lParam);
	HANDLE	GetWorkerSemaphore();
	char*	GetStaticData();
	BOOL	RemoveStaticData();
	BOOL	PostStaticData(char* szUrl);

	string	GetTime();
	const char* GetStringData(string& strContent);
private:
	CConfigManager*	m_pCfgManager;
	HANDLE			m_hWorker;
	CStdPtrArray	m_arrData;
	//CDuiPtrArray	m_arrData;

	string	m_strPackage;
	string	m_strStartTime;
	int		m_nPackageVersion;
};

