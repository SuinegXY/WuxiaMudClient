#ifndef _JSONHELPER_H_
#define _JSONHELPER_H_

#include <string>
#include "json/json.h"

using namespace std;

namespace SuinegJsonHelper
{
	bool GetValue( Json::Value root, const char* pszKey, void* pValue, Json::ValueType vt );
	bool GetValue( string& strContent, const char* pszKey, void* pValue, Json::ValueType vt );
	int GetValueInt(Json::Value& root, const char* pszKey);
	int GetValueInt(string& strContent, const char* pszKey);
	int GetValueInt(string& strContent, const char* pszRoot, const char* pszKey);
	string GetValueString(Json::Value& root, const char* pszKey);
	string GetValueString(string& strContent, const char* pszKey);
	string GetValueString(string& strContent, const char* pszRoot, const char* pszKey);
	bool GetValueBool(string& strContent, const char* pszKey);
	Json::Value GetValueObject(string& strContent, const char* pszKey);
}

#endif