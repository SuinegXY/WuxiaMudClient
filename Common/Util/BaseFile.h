//////////////////////////////////////////////////////////////////////////
//公用函数

// 得到文件的后缀名
char*		GetFileExtA(char* pszFile);

// 得到文件的后缀名
WCHAR*		GetFileExtW(WCHAR* pszFile);

// 得到指定文件的大小
ULONGLONG	GetFileSize(LPCTSTR szFileName);

// 得到指定文件的大小
ULONGLONG	GetFileSizeA(const char* pszFileName);

// 判断指定文件是否存在（绝对路径）
BOOL	FileIsExist(LPCTSTR szFileName);

// 判断指定文件是否存在（绝对路径）
BOOL	FileIsExistA(const char* pszFileName);

//去掉绝对路径中的文件名与扩展名，szPath需要分配空间，取得的路径类似"D:\Res\"
BOOL	GetFilePath(WCHAR* szFullPath, WCHAR* szPath);

//去掉绝对路径中的文件名与扩展名，szPath需要分配空间
BOOL	GetFilePathA(const char* szFullPath, char* szPath);

//比较文件版本号 版本号需要遵循标准1.0.0.0
//-1 新版本号较低 1 新版号较高 0 相等
int		CompareVersionA(char* pszVersionOld, char* pszVersionNew);

//比较文件版本号 版本号需要遵循标准1.0.0.0
//-1 新版本号较低 1 新版号较高 0 相等
int		CompareVersionW(WCHAR* pszVersionOld, WCHAR* pszVersionNew);

// 从URL剥离文件名
BOOL GetFileNameFromUrlA(char* szUrl, char* szFileName);

// 从URL剥离文件名
BOOL GetFileNameFromUrlW(WCHAR* szUrl, WCHAR* szFileName);

// 从路径中剥离文件名
BOOL GetFileNameA(char* szFullPath, char* szFileName);

// 从路径中剥离文件名
BOOL GetFileNameW(WCHAR* szFullPath, WCHAR* szFileName);

// 通过PID获取进程名称
BOOL GetProcessNameFromId(DWORD dwPid, TCHAR* pszProcessName);

// 查找占用某个端口的进程ID
BOOL GetProcessIdFromPort(DWORD dwPort, DWORD& dwPid);
