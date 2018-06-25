#ifndef _REG_H_
#define _REG_H_

namespace RegHelper
{
	void Init();
	BOOL Is64();
	void Bit64Set();
	void Bit64Unset();
	BOOL DeleteRegTree(HKEY hRoot, const char* pszSubKey, BOOL bWow64 = FALSE);
};

#endif