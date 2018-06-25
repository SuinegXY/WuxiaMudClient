// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#define _CRT_SECURE_NO_DEPRECATE

#pragma warning (disable : 4098)

#include <windows.h>
#include <objbase.h>
#include <ShellAPI.h>
#include "../Common/common.h"

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <WinSock2.h>
#include <dbt.h>
#include <tlhelp32.h>
#include <algorithm>
using namespace std;

#define InstanceDefine(className) \
public:\
	static className* GetInstance();\
	void init();\
private:\
	static className* s_pInstance;

#define InstanceRealize(className) \
	className* className::s_pInstance = NULL;\
	className* className::GetInstance()\
{\
	if ( s_pInstance == NULL )\
{\
	s_pInstance = new className;\
	if ( s_pInstance )\
{\
	s_pInstance->init();\
}\
}\
	return s_pInstance;\
}

#define InstanceDefineNoCreate(className) \
public:\
	static className* GetInstance();\
private:\
	static className* s_pInstance;

#define InstanceRealizeNoCreate(className) \
	className* className::s_pInstance = NULL;\
	className* className::GetInstance()\
{\
	return s_pInstance;\
}

#define ControlAction(pControl, action, ...) if ( pControl ) pControl->action(__VA_ARGS__);

#define SafeCheck(p) if ( p == NULL ) return;
