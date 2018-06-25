static const char cBasChar[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";  
static const char cKeyChar[] = "DfvwE34WXI019glmRdF2noMhixSTjkNp6qrYOABcy5CPQstuJK8LGHe7UVZabz"; 
static const int nBaseLength = 62; 
static const char cKey[] = "Suineg";
static const int nKeyLength = 6;
static const char base64Char1[] = "Dfv9wEW1XIg4lmRd0FnoM7hi2xSTjkNpqrYO56ABcyCPQstuJKLGH3eUVZ8abz";
static const char base64Char2[] = "1XGDmKL2xabfv9wEWd0NpqH3eUoM7hiSQstuJcrYAnP6Ig4VZ8O5BFTyCRlzjk";
//代码被盗后的处理2,采用新密串

void ___Swap(char& a, char& b)
{
	int c = a;
	a = b;
	b = c;
}

void Suineg_Encode2(char* _content, int _length)
{
	for ( int i = 0; i < _length; i++ )
	{
		for ( int j = 0; j < nBaseLength; j++ )
		{
			if ( _content[i] == cBasChar[j] )
			{
				_content[i] = cKeyChar[j];
				break;
			}
		}
	}
}

void Suineg_Decode2(char* _content, int _length)
{
	for ( int i = 0; i < _length; i++ )
	{
		for ( int j = 0; j < nBaseLength; j++ )
		{
			if ( _content[i] == cKeyChar[j] )
			{
				_content[i] = cBasChar[j];
				break;
			}
		}
	}
}


void Suineg_Encode1(char* _result, char* _content, int _length, const char* base64Char)
{
	for ( int i = 0; i < _length; i++ )
	{
		_content[i] += cKey[((i-1)/2)%nKeyLength];
		_content[i] -= cKey[((i+1)*2)%nKeyLength];
		bool ok = false;
		char num = 53;
		while ( !ok )
		{
			if ( (int)_content[i] < 0 )
			{
				_content[i] += nBaseLength;
				num -= 1;
			}
			else if ( (int)_content[i] >= nBaseLength )
			{
				_content[i] -= nBaseLength;
				num += 1;
			}
			else
				ok = true;
		}
		_result[i*2] = base64Char[_content[i]];
		_result[i*2+1] = num;
	}
	_result[_length*2] = '\0';
}

void Suineg_Decode1(char* _result, char* _content, int _length, const char* base64Char)
{
	for ( int i = 0; i < _length/2; i++ )
	{
		int num = -1;
		for ( int j = 0; j < nBaseLength; j++ )
		{
			if ( base64Char[j] == _content[i*2] )
			{
				num = j;
				break;
			}
		}
		num += (_content[i*2+1] - 53)*nBaseLength;
		_result[i] = num + cKey[((i+1)*2)%nKeyLength] - cKey[((i-1)/2)%nKeyLength];
	}
	_result[_length/2] = '\0';
}

char* Suineg_Encode(char* _content, int _length, const char* base64Char)
{
	char* result = new char[_length*2+1];
	___Swap(_content[1], _content[_length-1]);
	___Swap(_content[3], _content[_length-3]);
	Suineg_Encode1(result, _content, _length, base64Char);
	Suineg_Encode2(result, _length*2);
	return result;
}

char* Suineg_Decode(char* _content, int _length, const char* base64Char)
{
	char* result = new char[_length/2+1];
	Suineg_Decode2(_content, _length);
	Suineg_Decode1(result, _content, _length, base64Char);
	___Swap(result[1], result[_length/2-1]);
	___Swap(result[3], result[_length/2-3]);
	return result;
}

string MobileSimulatorEncode(const char* pszPackageName)
{
	static string strKey = "sTntS";
	string strUncode = strKey + pszPackageName;
	int nLen = strUncode.length();
	char pszBuffer[2048] = {0};
	strcpy(pszBuffer, strUncode.c_str());
	char* pszEncode = Suineg_Encode(pszBuffer, nLen, base64Char2);
	string strEncode = pszEncode;
	delete[] pszEncode;
	return strEncode;
}

BOOL MobileSimulatorDecode(string& strDecode, const char* pszCode, BOOL bFromMD = FALSE)
{
	static string strKey = "sTntS";
	int nLen = strlen(pszCode);
	char pszBuffer[2048] = {0};
	strcpy(pszBuffer, pszCode);
	char* pszDecode = Suineg_Decode(pszBuffer, nLen, base64Char2);
	string strDecodeBuffer = pszDecode;
	delete[] pszDecode;
	if ( strDecodeBuffer.length() < 5 ) return FALSE;
	string strValue = strDecodeBuffer.substr(0, 5);
	if ( strcmp(strValue.c_str(), strKey.c_str()) != 0 )
	{
		if ( bFromMD )
		{
			char pszBufferOld[2048] = {0};
			strcpy(pszBufferOld, pszCode);
			char* pszDecodeOld = Suineg_Decode(pszBufferOld, nLen, base64Char1);
			string strDecodeBufferOld = pszDecodeOld;
			delete[] pszDecodeOld;
			if ( strDecodeBufferOld.length() < 5 ) return FALSE;
			string strValueOld = strDecodeBufferOld.substr(0, 5);
			if ( strcmp(strValueOld.c_str(), strKey.c_str()) == 0 )
			{
				strDecode = strDecodeBufferOld.substr(5);
				return TRUE;
			}
		}
		return FALSE;
	}
	strDecode = strDecodeBuffer.substr(5);
	return TRUE;
}