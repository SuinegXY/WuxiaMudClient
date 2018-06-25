#include "stdafx.h"
#include "ATW.h"

std::string __do_w_to_a_utf8(const wchar_t* pwszText, UINT uCodePage)
{
	if( pwszText == NULL ) return "";
	int nNeedSize = WideCharToMultiByte(uCodePage, 0, pwszText, -1, NULL, 0, NULL, NULL);
	if( 0 == nNeedSize ) return "";
	std::string strRet("");
	char* pRet = new char[nNeedSize + 1];
	if ( pRet )
	{
		memset(pRet, 0, nNeedSize + 1);
		if ( WideCharToMultiByte(uCodePage, 0, pwszText, -1, pRet, nNeedSize, NULL, NULL) )
		{
			strRet = pRet;
		}
		delete []pRet;
	}
	return strRet;
}

std::wstring __do_a_utf8_to_w(const char* pszText, UINT uCodePage)
{
	if( pszText == NULL ) return L"";
	int nNeedSize = MultiByteToWideChar(uCodePage, 0, pszText, -1, NULL, 0);
	if( 0 == nNeedSize ) return L"";
	std::wstring strRet(L"");
	wchar_t *pRet = new wchar_t[nNeedSize + 1];
	if (pRet)
	{
		memset(pRet, 0, (nNeedSize + 1) * sizeof(wchar_t));
		if( MultiByteToWideChar(uCodePage, 0, pszText, -1, pRet, nNeedSize) )
		{
			strRet = pRet;
		}
		delete []pRet;
	}
	return strRet;
}

std::string WtoA(const std::wstring &strText)
{
	return __do_w_to_a_utf8(strText.c_str(), CP_ACP);
}

std::string WtoA(const wchar_t *pwszText)
{
	return __do_w_to_a_utf8(pwszText, CP_ACP);
}

std::wstring AtoW(const std::string &strText)
{
	return __do_a_utf8_to_w(strText.c_str(), CP_ACP);
}

std::wstring AtoW(const char* pszText)
{
	return __do_a_utf8_to_w(pszText, CP_ACP);
}

std::string WtoUTF8(const std::wstring &strText)
{
	return __do_w_to_a_utf8(strText.c_str(), CP_UTF8);
}

std::string WtoUTF8(const wchar_t *pwszText)
{
	return __do_w_to_a_utf8(pwszText, CP_UTF8);
}

std::wstring UTF8toW(const std::string &strText)
{
	return __do_a_utf8_to_w(strText.c_str(), CP_UTF8);
}

std::wstring UTF8toW(const char* pszText)
{
	return __do_a_utf8_to_w(pszText, CP_UTF8);
}

std::string UTF8toA(const std::string &src)
{
	return WtoA(UTF8toW(src));
}

std::string UTF8toA(const char *src)
{
	return WtoA(UTF8toW(src));
}

std::string AtoUTF8(const std::string &src)
{
	return WtoUTF8(AtoW(src));
}

std::string AtoUTF8(const char* src)
{
	return WtoUTF8(AtoW(src));
}

int IsTextUTF8(const char* pszSrc)
{
	const unsigned char* puszSrc = (const unsigned char*)pszSrc; // һ��Ҫ�޷��ŵ�,�з��ŵıȽϾͲ���ȷ��.
	// ������û��BOM��ʾ EF BB BF
	if( puszSrc[0] != 0 && puszSrc[0] == 0xEF && 
		puszSrc[1] != 0 && puszSrc[1] == 0xBB &&
		puszSrc[2] != 0 && puszSrc[2] == 0xBF)
	{
		return 0;
	}

	bool bIsNextByte = false;
	int nBytes = 0;
	const unsigned char* pCur = (const unsigned char*)pszSrc;
	
	while( pCur[0] != 0 )
	{
		if(!bIsNextByte)
		{
			bIsNextByte = true;
			if ( (pCur[0] >> 7) == 0) { bIsNextByte = false; nBytes = 1; bIsNextByte = false; } // ���λΪ0, ANSI ���ݵ�.
			else if ((pCur[0] >> 5) == 0x06) { nBytes = 2; } // ����5λ���� 110 -> 2�ֽڱ����UTF8�ַ������ֽ�
			else if ((pCur[0] >> 4) == 0x0E) { nBytes = 3; } // ����4λ���� 1110 -> 3�ֽڱ����UTF8�ַ������ֽ�
			else if ((pCur[0] >> 3) == 0x1E) { nBytes = 4; } // ����3λ���� 11110 -> 4�ֽڱ����UTF8�ַ������ֽ�
			else if ((pCur[0] >> 2) == 0x3E) { nBytes = 5; } // ����2λ���� 111110 -> 5�ֽڱ����UTF8�ַ������ֽ�
			else if ((pCur[0] >> 1) == 0x7E) { nBytes = 6; } // ����1λ���� 1111110 -> 6�ֽڱ����UTF8�ַ������ֽ�
			else
			{
				nBytes = -1; // �Ƿ���UTF8�ַ���������ֽ�
				break;
			}
		}
		else
		{
			if ((pCur[0] >> 6) == 0x02) // ����,�����ֽڱ����� 10xxx ��ͷ
			{
				nBytes--;
				if (nBytes == 1) bIsNextByte = false; // �� nBytes = 1ʱ, ˵����һ���ֽ�Ӧ�������ֽ�.
			}
			else
			{
				nBytes = -2;
				break;
			}
		}

		// ����һ���ַ�
		pCur++;
	}

	if( nBytes == 1) return 0;
	else return nBytes;
}

BYTE ToHex(const BYTE &x)
{
	return x > 9 ? x -10 + 'A': x + '0';
}

unsigned char FromHex(unsigned char x) 
{ 
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	return y;
}

string UrlEncode(const char* pszContent)
{
	string sOut;
	string strIn = pszContent;
	for( size_t ix = 0; ix < strIn.size(); ix++ )
	{      
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)strIn[ix] ) )
		{      
			buf[0] = strIn[ix];
		}
		else if ( isspace( (BYTE)strIn[ix] ) )
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = ToHex( (BYTE)strIn[ix] >> 4 );
			buf[2] = ToHex( (BYTE)strIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

string UrlDecode(const char* pszContent)
{
	string strOut;
	size_t length = strlen(pszContent);
	for (size_t i = 0; i < length; i++)
	{
		if (pszContent[i] == '+') strOut += ' ';
		else if (pszContent[i] == '%')
		{
			unsigned char high = FromHex((unsigned char)pszContent[++i]);
			unsigned char low = FromHex((unsigned char)pszContent[++i]);
			strOut += high*16 + low;
		}
		else strOut += pszContent[i];
	}
	return strOut;
}

int Utf8Bytes(char* pszSrc, WCHAR* pszOut, int nType)
{
	int nCount = 0;
	int nLen = strlen(pszSrc);
	for (int nIndex=0; nIndex<nLen; nIndex++)
	{
		char szHex[5] = {0};
		char szAscII[2] = {0};
		if ( *(pszSrc+nIndex) == '\\' )
		{
			nIndex++;
			if ( nIndex < nLen )
			{
				if ( *(pszSrc+nIndex)=='u' )
				{
					if ( nType == 2 )
					{
						memcpy(szHex, pszSrc+nIndex+1, 4);
						unsigned int unHex = 0;
						sscanf_s(szHex,"%04x", &unHex);
						pszOut[nCount++] = unHex;
					}
					else if( nType == 1 )
					{
						nCount++;
					}
					nIndex+=4;
				}
			}
		}
		else
		{
			if ( nType == 2 )
			{
				memcpy(szAscII, pszSrc+nIndex, 1);
				unsigned int unHex = 0;
				sscanf_s(szAscII, "%c", &unHex);
				pszOut[nCount++] = unHex;
			}
			else if( nType == 1 )
			{
				nCount++;
			}
		}
	}

	return nCount;
}

std::string Utf8Decode( const std::string& strSrc )
{
	int nLen = strSrc.length();
	char szSrc[1024] = {0};
	strcpy(szSrc, strSrc.c_str());
	int nBytes = Utf8Bytes(szSrc, NULL, 1);
	WCHAR* wszBytes = new WCHAR[nBytes + sizeof(WCHAR)];
	Utf8Bytes(szSrc, wszBytes, 2);
	wszBytes[nBytes]=0; 
	string strValue = WtoA((wchar_t*)wszBytes);
	delete[] wszBytes;
	return strValue;
}

string GetStringHead( string& strContant, const char* pszSepa )
{
	string strHead;
	size_t nIndex = strContant.find(pszSepa);
	if ( nIndex != -1 )
	{
		strHead = strContant.substr(0, nIndex);
		strContant = strContant.substr(nIndex+strlen(pszSepa));
	}
	else
	{
		strHead = strContant;
		strContant = "";
	}
	return strHead;
}

wstring GetStringHeadW( wstring& strContant, const WCHAR* pszSepa )
{
	wstring strHead;
	size_t nIndex = strContant.find(pszSepa);
	if ( nIndex != -1 )
	{
		strHead = strContant.substr(0, nIndex);
		strContant = strContant.substr(nIndex+_tcslen(pszSepa));
	}
	else
	{
		strHead = strContant;
		strContant = L"";
	}
	return strHead;
}

std::wstring GetStringSepa( wstring& strContant, const WCHAR* pszSepa )
{
	wstring strHead;
	size_t nIndex = strContant.find(pszSepa);
	if ( nIndex != -1 )
	{
		strHead = strContant.substr(0, nIndex);
		strContant = strContant.substr(nIndex+_tcslen(pszSepa));
	}
	return strHead;
}

int GetStringLength( const char* pszContant )
{
	int nLen = strlen(pszContant);
	int nWidth = nLen*9 + 2;
	for ( int i = 0; i < nLen; ++i )
	{
		if ( pszContant[i] < 0 )
		{
			nWidth -= 1;
		}
	}
	return nWidth;
}

int GetStringLengthW( const WCHAR* pszContant )
{
	string strContant = WtoA(pszContant);
	int nLen = strContant.length();
	int nWidth = nLen*10 + 2;
	for ( int i = 0; i < nLen; ++i )
	{
		if ( strContant.c_str()[i] < 0 )
		{
			nWidth -= 2;
		}
	}
	return nWidth;
}

std::wstring NumToCnString( int nNum )
{
	////1234,5678 = һǧ������ʮ�� �� ��ǧ������ʮ��
	////1004,0608 = һǧ���� �� �������
	if ( nNum == 0 ) return L"��";
	else if ( nNum > 99999999 ) return L"������";
	static wstring szCn[10] = {
		L"", L"һ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��",
	};
	int nCale = 0;
	wstring wstrCn = L"";
	do 
	{
		BOOL bCanZero = FALSE;
		BOOL bZero = FALSE;
		if ( nNum > 9999 )
		{
			nCale = nNum / 10000;
			nNum = nNum - nCale*10000;
		}
		else
		{
			nCale = nNum;
			nNum = 0;
		}
		if ( nCale > 999 )
		{
			wstrCn.append(szCn[nCale/1000]);
			wstrCn.append(L"ǧ");
			nCale %= 1000;
			bCanZero = TRUE;
		}
		if ( nCale > 99 )
		{
			wstrCn.append(szCn[nCale/100]);
			wstrCn.append(L"��");
			nCale %= 100;
			bCanZero = TRUE;
		}
		else if ( bCanZero )
		{
			bZero = TRUE;
			bCanZero = FALSE;
		}
		if ( nCale > 9 )
		{
			if ( bZero )
			{
				wstrCn.append(L"��");
				bZero = FALSE;
			}
			wstrCn.append(szCn[nCale/10]);
			wstrCn.append(L"ʮ");
			nCale %= 10;
		}
		else if ( bCanZero )
		{
			bZero = TRUE;
		}
		if ( nCale )
		{
			if ( bZero )
			{
				wstrCn.append(L"��");
				bZero = FALSE;
			}
			wstrCn.append(szCn[nCale]);
		}
		if ( nNum > 0 )
		{
			wstrCn.append(L"��");
		}
	} while ( nNum > 0 );
	return wstrCn;
}

