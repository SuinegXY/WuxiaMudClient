#include "stdafx.h"
#include "Object.h"

InstanceRealize(CObjectInfo);

CObjectInfo::CObjectInfo()
{
}

void CObjectInfo::init()
{
	SetInfo("50101", L"´óÊ÷");
	SetRGBA("50101", 0xff08b85f);
	SetEvent("50101", "climb", L"ÅÀÊ÷");
	SetInfo("50102", L"¾Æ×ÀÊ£·¹");
	SetEvent("50102", "eat", L"³ÔÊ£·¹");
	SetInfo("50103", L"Ë®Ô´");
	SetRGBA("50103", 0xff3cbaf4);
	SetEvent("50103", "drink", L"ºÈË®");
	SetInfo("50104", L"Ä¾²Ä");
	SetEvent("50104", "cut", L"¿³²ñ");
	SetInfo("50105", L"Â¯Ôî");
	SetEvent("50105", "heat", L"ÉÕ·¹");
	SetInfo("50106", L"É¨°Ñ");
	SetEvent("50106", "clean", L"´òÉ¨");
	SetInfo("50107", L"±øÆ÷¼Ü");
	SetInfo("50108", L"´²");
	SetEvent("50108", "sleep", L"Ë¯¾õ");

	SetInfo("50201", L"¿××Óµä¼®");
	SetEvent("50201", "learn 40101", L"×êÑÐ");
	SetInfo("50202", L"ÆÑÍÅ");
	SetEvent("50202", "dazuo", L"´ò×ø");
}
