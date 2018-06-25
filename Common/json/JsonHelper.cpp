#include "stdafx.h"
#include "JsonHelper.h"

namespace SuinegJsonHelper
{
	bool GetValue( Json::Value root, const char* pszKey, void* pValue, Json::ValueType vt )
	{
		if ( root.type() == Json::ValueType::arrayValue )
		{
			if ( root.size() )
			{
				root = root[UINT(0)];
			}
		}
		if ( root.empty() )
		{
			return false;
		}
		if ( root.type() != Json::objectValue )
		{
			return false;
		}
		if ( vt != root[pszKey].type() )
		{
			return false;
		}
		switch ( root[pszKey].type() )
		{
		case Json::objectValue:
		case Json::arrayValue:
			{
				*((Json::Value*)pValue) = root[pszKey];
			}
			break;
		case Json::intValue:
			{
				int n = root[pszKey].asInt();
				memcpy(pValue, &n, sizeof(n));
			}
			break;
		case Json::stringValue:
			{
				string str = root[pszKey].asString();
				if ( str.length() )
				{
					memcpy(pValue, str.c_str(), str.length());
				}
			}
			break;
		case Json::booleanValue:
			{
				bool b = root[pszKey].asBool();
				memcpy(pValue, &b, sizeof(b));
			}
			break;
		}
		return true;
	}

	bool GetValue( string& strContent, const char* pszKey, void* pValue, Json::ValueType vt )
	{
		if ( strContent.length() == 0 ) return false;
		Json::Reader reader;
		Json::Value root;
		if ( !reader.parse(strContent.c_str(), root) ) return false;
		return GetValue(root, pszKey, pValue, vt);
	}

	Json::Value GetValueObject( string& strContent, const char* pszKey )
	{
		Json::Value value;
		GetValue(strContent, pszKey, &value, Json::objectValue);
		return value;
	}

	int GetValueInt( Json::Value& root, const char* pszKey )
	{
		int n = 0;
		GetValue(root, pszKey, &n, Json::intValue);
		return n;
	}

	int GetValueInt( string& strContent, const char* pszKey )
	{
		int n = 0;
		GetValue(strContent, pszKey, &n, Json::intValue);
		return n;
	}

	int GetValueInt( string& strContent, const char* pszRoot, const char* pszKey )
	{
		int n = 0;
		GetValue(GetValueObject(strContent, pszRoot), pszKey, &n, Json::intValue);
		return n;
	}

	std::string GetValueString( Json::Value& root, const char* pszKey )
	{
		char psz[8096] = {0};
		GetValue(root, pszKey, psz, Json::stringValue);
		return psz;
	}

	std::string GetValueString( string& strContent, const char* pszKey )
	{
		char psz[8096] = {0};
		GetValue(strContent, pszKey, psz, Json::stringValue);
		return psz;
	}

	string GetValueString( string& strContent, const char* pszRoot, const char* pszKey )
	{
		char psz[8096] = {0};
		GetValue(GetValueObject(strContent, pszRoot), pszKey, psz, Json::stringValue);
		return psz;
	}

	bool GetValueBool( string& strContent, const char* pszKey )
	{
		bool b = false;
		GetValue(strContent, pszKey, &b, Json::booleanValue);
		return b;
	}
}