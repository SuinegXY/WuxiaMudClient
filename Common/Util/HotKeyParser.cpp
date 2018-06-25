#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include "HotKeyParser.h"
#include "..\Logger.h"

//#define NO_HOTKEY

InstanceRealize(CHotKeyParser);

CHotKeyParser::CHotKeyParser()
{
	ZeroMemory(m_unID, sizeof(UINT)*KEY_COUNT);
	ZeroMemory(m_unAtom, sizeof(UINT)*KEY_COUNT);
	ZeroMemory(m_unKey, sizeof(UINT)*KEY_COUNT);
	ZeroMemory(m_unMod, sizeof(UINT)*KEY_COUNT);
}

void CHotKeyParser::init()
{
	m_hWnd = NULL;
}

void CHotKeyParser::init( HWND hWnd )
{
	m_hWnd = hWnd;
}

BOOL CHotKeyParser::RegisterGlobalKey( int nName, const WCHAR* pwszAtomName, const WCHAR* pwszKeyName )
{
#ifndef NO_HOTKEY
	if ( m_hWnd == NULL ) return FALSE;
	int nCtrl, nShift, nAlt, vKey;
	GetHotKey(pwszKeyName, nCtrl, nShift, nAlt, vKey);

	int nMod=0;
	if ( nCtrl )	nMod |= MOD_CONTROL;
	if ( nAlt )		nMod |= MOD_ALT;
	if ( nShift )	nMod |= MOD_SHIFT;
	if ( vKey != -1 && m_unID[nName] == NULL )
	{
		m_unID[nName] = GlobalAddAtom( pwszAtomName );
		if( RegisterHotKey(m_hWnd, m_unID[nName], nMod, vKey) )
		{
			//if ( m_unAtom[nName] ) GlobalDeleteAtom(m_unAtom[nName]);
			//m_unAtom[nName] = GlobalAddAtom( pwszKeyName );
			//CLogger::GetInstance()->Log("注册成功:", pwszAtomName);
			return TRUE;
		}
		//else
		//{
			//int err = GetLastError();
			//CLogger::GetInstance()->Log("注销失败:", pwszAtomName);
		//}
	}
#endif

	return FALSE;
}

BOOL CHotKeyParser::RegisterLocalKey( int nName, const WCHAR* pwszKeyName )
{
	if ( m_hWnd == NULL ) return FALSE;
	int nCtrl, nShift, nAlt, vKey;
	GetHotKey(pwszKeyName, nCtrl, nShift, nAlt, vKey);

	int nMod=0;
	if ( nCtrl )	nMod |= MOD_CONTROL;
	if ( nAlt )		nMod |= MOD_ALT;
	if ( nShift )	nMod |= MOD_SHIFT;
	if ( vKey != -1 )
	{
		UINT atom = GlobalFindAtom( pwszKeyName );
		if ( atom )
		{
			for ( int i = 0; i < KEY_COUNT; i++ )
			{
				if ( i != nName && atom == m_unAtom[i] )
				{
					return FALSE;
				}
			}
		}
		if ( m_unAtom[nName] ) GlobalDeleteAtom(m_unAtom[nName]);
		m_unAtom[nName] = GlobalAddAtom( pwszKeyName );
		m_unKey[nName] = vKey;
		m_unMod[nName] = nMod;
		return TRUE;
	}

	return FALSE;
}

void CHotKeyParser::UnregisterKey( int nName )
{
	if ( m_unID[nName] )
	{
		BOOL bDelete = UnregisterHotKey(m_hWnd, m_unID[nName]);
		//CLogger::GetInstance()->Log("注销:", bDelete);
		GlobalDeleteAtom(m_unID[nName]);
		m_unID[nName] = 0;
	}
	if ( m_unAtom[nName] )
	{
		GlobalDeleteAtom(m_unAtom[nName]);
	}
	m_unKey[nName] = 0;
	m_unMod[nName] = 0;
}

int CHotKeyParser::GetHotKey( const WCHAR* pszKeyName, int &nCtrl, int &nShift, int &nAlt, int &vKey )
{
	nCtrl	= 0;
	nShift	= 0;
	nAlt	= 0;
	vKey	= -1;

	if( wcsstr(pszKeyName, L"Ctrl") )
	{
		nCtrl = 1;
	}
	if( wcsstr(pszKeyName, L"Alt") )
	{
		nAlt = 1;
	}
	if( wcsstr(pszKeyName, L"Shift") )
	{
		nShift = 1;
	}

	int offset = 0;
	if ( nCtrl )	offset += 7;
	if ( nShift )	offset += 8;
	if ( nAlt )		offset += 6;

	const wchar_t* szVkey = pszKeyName + offset;
	vKey = GetKeyByString(szVkey);
	return 0;
}

int CHotKeyParser::GetKeyByString( const WCHAR* pwszKey )
{
	if( _tcslen(pwszKey) == 1 )
	{
		if( pwszKey[0] >= 'A' && pwszKey[0] <= 'Z' )
		{
			return pwszKey[0];
		}
		if( pwszKey[0] >= '0' && pwszKey[0] <= '9' )
		{
			return pwszKey[0];
		}
	}

	if ( _tcscmp(pwszKey, L"Backspace") == 0 )	return 8;
	if ( _tcscmp(pwszKey, L"Enter") == 0 )		return 13;
	if ( _tcscmp(pwszKey, L"Esc") == 0 )		return 27;
	if ( _tcscmp(pwszKey, L"Space") == 0 )		return 32;
	if ( _tcscmp(pwszKey, L"Page Up") == 0 )	return 33;
	if ( _tcscmp(pwszKey, L"Page Down") == 0 )	return 34;
	if ( _tcscmp(pwszKey, L"End") == 0 )		return 35;
	if ( _tcscmp(pwszKey, L"Home") == 0 )		return 36;
	if ( _tcscmp(pwszKey, L"↑") == 0 )			return 38;
	if ( _tcscmp(pwszKey, L"→") == 0 )			return 39;
	if ( _tcscmp(pwszKey, L"←") == 0 )			return 37;
	if ( _tcscmp(pwszKey, L"↓") == 0 )			return 40;
	if ( _tcscmp(pwszKey, L"Insert") == 0 )		return 45;
	if ( _tcscmp(pwszKey, L"Delete") == 0 )		return 46;
	if ( _tcscmp(pwszKey, L"Num0") == 0 )		return 96;
	if ( _tcscmp(pwszKey, L"Num1") == 0 )		return 97;
	if ( _tcscmp(pwszKey, L"Num2") == 0 )		return 98;
	if ( _tcscmp(pwszKey, L"Num3") == 0 )		return 99;
	if ( _tcscmp(pwszKey, L"Num4") == 0 )		return 100;
	if ( _tcscmp(pwszKey, L"Num5") == 0 )		return 101;
	if ( _tcscmp(pwszKey, L"Num6") == 0 )		return 102;
	if ( _tcscmp(pwszKey, L"Num7") == 0 )		return 103;
	if ( _tcscmp(pwszKey, L"Num8") == 0 )		return 104;
	if ( _tcscmp(pwszKey, L"Num9") == 0 )		return 105;
	if ( _tcscmp(pwszKey, L"Num*") == 0 )		return 106;
	if ( _tcscmp(pwszKey, L"Num+") == 0 )		return 107;
	if ( _tcscmp(pwszKey, L"Num-") == 0 )		return 109;
	if ( _tcscmp(pwszKey, L"Num.") == 0 )		return 110;
	if ( _tcscmp(pwszKey, L"Num/") == 0 )		return 111;
	if ( _tcscmp(pwszKey, L"F1") == 0 )			return VK_F1;
	if ( _tcscmp(pwszKey, L"F2") == 0 )			return VK_F2;
	if ( _tcscmp(pwszKey, L"F3") == 0 )			return VK_F3;
	if ( _tcscmp(pwszKey, L"F4") == 0 )			return VK_F4;
	if ( _tcscmp(pwszKey, L"F5") == 0 )			return VK_F5;
	if ( _tcscmp(pwszKey, L"F6") == 0 )			return VK_F6;
	if ( _tcscmp(pwszKey, L"F7") == 0 )			return VK_F7;
	if ( _tcscmp(pwszKey, L"F8") == 0 )			return VK_F8;
	if ( _tcscmp(pwszKey, L"F9") == 0 )			return VK_F9;
	if ( _tcscmp(pwszKey, L"F10") == 0 )		return VK_F10;
	if ( _tcscmp(pwszKey, L"F11") == 0 )		return VK_F11;
	if ( _tcscmp(pwszKey, L"F12") == 0 )		return VK_F12;
	if ( _tcscmp(pwszKey, L";") == 0 )			return 186;
	if ( _tcscmp(pwszKey, L"=") == 0 )			return 187;
	if ( _tcscmp(pwszKey, L",") == 0 )			return 188;
	if ( _tcscmp(pwszKey, L"-") == 0 )			return 189;
	if ( _tcscmp(pwszKey, L".") == 0 )			return 190;
	if ( _tcscmp(pwszKey, L"/") == 0 )			return 191;
	if ( _tcscmp(pwszKey, L"~") == 0 )			return 192;
	if ( _tcscmp(pwszKey, L"[") == 0 )			return 219;
	if ( _tcscmp(pwszKey, L"\\") == 0 )			return 220;
	if ( _tcscmp(pwszKey, L"]") == 0 )			return 221;
	if ( _tcscmp(pwszKey, L"'") == 0 )			return 222;

	return -1;
}

int CHotKeyParser::GetHotKeyIndex( WPARAM wParam )
{
	for ( int i = 0; i < KEY_COUNT; i++ )
	{
		if ( wParam == m_unID[i] )
		{
			return i;
		}
		////Local
		//if ( wParam == m_unKey[i] )
		//{
		//	int nMod=0;
		//	if ( GetKeyState(VK_CONTROL) & 0x8000 )	nMod |= MOD_CONTROL;
		//	if ( GetKeyState(VK_MENU) & 0x8000 )	nMod |= MOD_ALT;
		//	if ( GetKeyState(VK_SHIFT) & 0x8000 )	nMod |= MOD_SHIFT;
		//	if ( nMod == m_unMod[i] )
		//	{
		//		return i;
		//	}
		//}
	}
	return -1;
}
