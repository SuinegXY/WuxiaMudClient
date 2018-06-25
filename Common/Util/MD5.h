#pragma once
#include <string>
using namespace std;
// CMD5

class CMD5 
{
public:
	CMD5();
	virtual ~CMD5();
	static char* MD5String(const char* string ); 
	static bool MD5Check( char *md5string, char* string ); 

	static char* MD5String( FILE *pfile) ;
	static bool  MD5Check(const char *md5string, FILE *file );
	static char* MD5String( FILE *pfile,void* pdatabuf,unsigned int len);
public:
	static wstring Str2Md5(wstring const & Str);
};


