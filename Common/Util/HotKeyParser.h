#ifndef _HOTKEYPARSER_H_
#define _HOTKEYPARSER_H_

#define KEY_COUNT	5

class CHotKeyParser
{
	InstanceDefine(CHotKeyParser);
public:
	CHotKeyParser();

	void init(HWND hWnd);

	BOOL RegisterGlobalKey( int nName, const WCHAR* pwszAtomName, const WCHAR* pwszKeyName );
	BOOL RegisterLocalKey( int nName, const WCHAR* pwszKeyName );
	void UnregisterKey( int nName );

	int GetHotKeyIndex(WPARAM wParam);

protected:
	int GetHotKey( const WCHAR* pszKeyName, int &nCtrl, int &nShift, int &nAlt, int &vKey );
	int GetKeyByString( const WCHAR* pwszKey );
private:
	HWND m_hWnd;
	DWORD m_unID[KEY_COUNT];
	DWORD m_unAtom[KEY_COUNT];
	DWORD m_unKey[KEY_COUNT];
	DWORD m_unMod[KEY_COUNT];
};

#endif