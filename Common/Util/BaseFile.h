//////////////////////////////////////////////////////////////////////////
//���ú���

// �õ��ļ��ĺ�׺��
char*		GetFileExtA(char* pszFile);

// �õ��ļ��ĺ�׺��
WCHAR*		GetFileExtW(WCHAR* pszFile);

// �õ�ָ���ļ��Ĵ�С
ULONGLONG	GetFileSize(LPCTSTR szFileName);

// �õ�ָ���ļ��Ĵ�С
ULONGLONG	GetFileSizeA(const char* pszFileName);

// �ж�ָ���ļ��Ƿ���ڣ�����·����
BOOL	FileIsExist(LPCTSTR szFileName);

// �ж�ָ���ļ��Ƿ���ڣ�����·����
BOOL	FileIsExistA(const char* pszFileName);

//ȥ������·���е��ļ�������չ����szPath��Ҫ����ռ䣬ȡ�õ�·������"D:\Res\"
BOOL	GetFilePath(WCHAR* szFullPath, WCHAR* szPath);

//ȥ������·���е��ļ�������չ����szPath��Ҫ����ռ�
BOOL	GetFilePathA(const char* szFullPath, char* szPath);

//�Ƚ��ļ��汾�� �汾����Ҫ��ѭ��׼1.0.0.0
//-1 �°汾�Žϵ� 1 �°�Žϸ� 0 ���
int		CompareVersionA(char* pszVersionOld, char* pszVersionNew);

//�Ƚ��ļ��汾�� �汾����Ҫ��ѭ��׼1.0.0.0
//-1 �°汾�Žϵ� 1 �°�Žϸ� 0 ���
int		CompareVersionW(WCHAR* pszVersionOld, WCHAR* pszVersionNew);

// ��URL�����ļ���
BOOL GetFileNameFromUrlA(char* szUrl, char* szFileName);

// ��URL�����ļ���
BOOL GetFileNameFromUrlW(WCHAR* szUrl, WCHAR* szFileName);

// ��·���а����ļ���
BOOL GetFileNameA(char* szFullPath, char* szFileName);

// ��·���а����ļ���
BOOL GetFileNameW(WCHAR* szFullPath, WCHAR* szFileName);

// ͨ��PID��ȡ��������
BOOL GetProcessNameFromId(DWORD dwPid, TCHAR* pszProcessName);

// ����ռ��ĳ���˿ڵĽ���ID
BOOL GetProcessIdFromPort(DWORD dwPort, DWORD& dwPid);
