#ifndef Interface_h__
#define Interface_h__

class CInterface
{
public:
	static string Get(const char* pszUrl, const char* pszFormat, ...);
	static string Post(const char* pszUrl, const char* pszFormat, ...);
	static string AESCode(const char* pszSource);

protected:
	static string GetParam(const char* pszParam);
	static string GetSign(const char* pszContent);
};

#endif
