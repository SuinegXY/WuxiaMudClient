#include "stdafx.h"
#include "Object.h"

InstanceRealize(CObjectInfo);

CObjectInfo::CObjectInfo()
{
}

void CObjectInfo::init()
{
	SetInfo("50101", L"����");
	SetRGBA("50101", 0xff08b85f);
	SetEvent("50101", "climb", L"����");
	SetInfo("50102", L"����ʣ��");
	SetEvent("50102", "eat", L"��ʣ��");
	SetInfo("50103", L"ˮԴ");
	SetRGBA("50103", 0xff3cbaf4);
	SetEvent("50103", "drink", L"��ˮ");
	SetInfo("50104", L"ľ��");
	SetEvent("50104", "cut", L"����");
	SetInfo("50105", L"¯��");
	SetEvent("50105", "heat", L"�շ�");
	SetInfo("50106", L"ɨ��");
	SetEvent("50106", "clean", L"��ɨ");
	SetInfo("50107", L"������");
	SetInfo("50108", L"��");
	SetEvent("50108", "sleep", L"˯��");

	SetInfo("50201", L"���ӵ伮");
	SetEvent("50201", "learn 40101", L"����");
	SetInfo("50202", L"����");
	SetEvent("50202", "dazuo", L"����");
}
