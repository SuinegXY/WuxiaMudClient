#include "stdafx.h"
#include "Npc.h"
#include "Util\ATW.h"

InstanceRealize(CNpcInfo)

CNpcInfo::CNpcInfo()
{
}

void CNpcInfo::init()
{
	SetInfo("30101", L"�����ִ�ʹ�� ����ʹ��");
	SetAsk("30101", "���뽭��");
	SetAsk("30101", "����");
	SetAsk("30101", "�ڿ�");
	SetAsk("30101", "ѧ��");
	SetAsk("30101", "����");

	SetInfo("30102", L"��С��");
	SetAsk("30102", "�͵�");

	SetInfo("30103", L"��С��");
	SetAsk("30103", "����");

	SetInfo("30104", L"��δ���ˡ� ������");
	SetEvent("30104", "quest", L"����");
	SetEvent("30104", "complete", L"���");
	SetEvent("30104", "fail", L"����");

	SetInfo("30105", L"�����ؽ�");
	SetAsk("30105", "������");

	SetInfo("30106", L"�ƹ�");
	SetAsk("30106", "��Ǯ");
	SetAsk("30106", "ȡǮ");
	SetEvent("30106", "cun all", L"ȫ��");

	SetInfo("30107", L"�ƹ�");
	SetAsk("30107", "����");
	SetAsk("30107", "����");

	SetInfo("30108", L"����");
	SetAsk("30108", "��");

	SetInfo("30109", L"�ƹ�");

	SetInfo("30110", L"��ɱ����ҽ�� ƽһָ");
	SetAsk("30110", "��ҩ");
	SetEvent("30110", "heal", L"����");

	SetInfo("30111", L"����");
	SetEvent("30111", "ask 30111 ѧϰ����д��", L"����ѧϰ");
	SetEvent("30111", "learn 40001", L"ѧϰ��д");
	SetEvent("30111", "learn stop", L"ֹͣѧϰ");

	SetInfo("30112", L"ľ��");
	SetEvent("30112", "fight 30112", L"����");

	SetInfo("30113", L"����");
	SetAsk("30113", "ѧϰ�书");
	SetEvent("30113", "learn 40013", L"����ȭ��");
	SetEvent("30113", "learn 40011", L"�����Ṧ");
	SetEvent("30113", "learn 40012", L"�����ڹ�");
	SetEvent("30113", "learn 40010", L"�����м�");
	SetEvent("30113", "learn 40301", L"̫�泤ȭ");
	SetEvent("30113", "learn 40101", L"������");
	SetEvent("30113", "learn 40201", L"�����ķ�");
	SetEvent("30113", "learn stop", L"ֹͣѧϰ");

	SetInfo("30201", L"���׼Һ��ˡ� ��Խ");
	SetEvent("30201", "quest", L"����");
	SetEvent("30201", "complete", L"���");
	SetEvent("30201", "fail", L"����");
}

void CNpcInfo::SetAsk( const char* pszID, const char* pszWhat )
{
	string strAsk = "ask ";
	strAsk.append(pszID);
	strAsk.append(" ");
	strAsk.append(pszWhat);
	SetEvent(pszID, strAsk.c_str(), AtoW(pszWhat).c_str());
}

std::wstring CNpcInfo::GetExpDescribe( string& strExp )
{
	int nExp = atoi(strExp.c_str());
	if ( nExp == 0 )
	{
		return L"����һ��";
	}
	if ( nExp < 100 )
	{
		return L"��������";
	}
	if ( nExp < 200 )
	{
		return L"����ҳ�";
	}
	if ( nExp < 400 )
	{
		return L"��ѧէ��";
	}
	if ( nExp < 800 )
	{
		return L"����ǿǿ";
	}
	if ( nExp < 1600 )
	{
		return L"�����ž�";
	}
	if ( nExp < 3200 )
	{
		return L"����é®";
	}
	if ( nExp < 6400 )
	{
		return L"��֪һ��";
	}
	if ( nExp < 12800 )
	{
		return L"����ͨͨ";
	}
	if ( nExp < 25600 )
	{
		return L"ƽƽ����";
	}
	if ( nExp < 51200 )
	{
		return L"ƽ������";
	}
	if ( nExp < 100000 )
	{
		return L"��ͨƤë";
	}
	if ( nExp < 150000 )
	{
		return L"��������";
	}
	if ( nExp < 200000 )
	{
		return L"��������";
	}
	if ( nExp < 250000 )
	{
		return L"����С��";
	}
	if ( nExp < 300000 )
	{
		return L"����С��";
	}
	if ( nExp < 350000 )
	{
		return L"������Ⱥ";
	}
	if ( nExp < 400000 )
	{
		return L"�������";
	}
	if ( nExp < 450000 )
	{
		return L"�������";
	}
	if ( nExp < 500000 )
	{
		return L"���д��";
	}
	return L"��������";
}
