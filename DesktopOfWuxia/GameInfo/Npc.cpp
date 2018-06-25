#include "stdafx.h"
#include "Npc.h"
#include "Util\ATW.h"

InstanceRealize(CNpcInfo)

CNpcInfo::CNpcInfo()
{
}

void CNpcInfo::init()
{
	SetInfo("30101", L"「武林大使」 新手使者");
	SetAsk("30101", "初入江湖");
	SetAsk("30101", "饥饿");
	SetAsk("30101", "口渴");
	SetAsk("30101", "学武");
	SetAsk("30101", "其他");

	SetInfo("30102", L"店小二");
	SetAsk("30102", "客店");

	SetInfo("30103", L"店小二");
	SetAsk("30103", "买卖");

	SetInfo("30104", L"「未亡人」 孙婆婆");
	SetEvent("30104", "quest", L"帮助");
	SetEvent("30104", "complete", L"完成");
	SetEvent("30104", "fail", L"放弃");

	SetInfo("30105", L"扬州守将");
	SetAsk("30105", "兵器库");

	SetInfo("30106", L"掌柜");
	SetAsk("30106", "存钱");
	SetAsk("30106", "取钱");
	SetEvent("30106", "cun all", L"全存");

	SetInfo("30107", L"掌柜");
	SetAsk("30107", "买卖");
	SetAsk("30107", "当赎");

	SetInfo("30108", L"牙人");
	SetAsk("30108", "买房");

	SetInfo("30109", L"掌柜");

	SetInfo("30110", L"「杀人神医」 平一指");
	SetAsk("30110", "买药");
	SetEvent("30110", "heal", L"治疗");

	SetInfo("30111", L"夫子");
	SetEvent("30111", "ask 30111 学习读书写字", L"关于学习");
	SetEvent("30111", "learn 40001", L"学习读写");
	SetEvent("30111", "learn stop", L"停止学习");

	SetInfo("30112", L"木人");
	SetEvent("30112", "fight 30112", L"比试");

	SetInfo("30113", L"老者");
	SetAsk("30113", "学习武功");
	SetEvent("30113", "learn 40013", L"基本拳法");
	SetEvent("30113", "learn 40011", L"基本轻功");
	SetEvent("30113", "learn 40012", L"基本内功");
	SetEvent("30113", "learn 40010", L"基本招架");
	SetEvent("30113", "learn 40301", L"太祖长拳");
	SetEvent("30113", "learn 40101", L"游侠身法");
	SetEvent("30113", "learn 40201", L"游侠心法");
	SetEvent("30113", "learn stop", L"停止学习");

	SetInfo("30201", L"「孔家后人」 孔越");
	SetEvent("30201", "quest", L"帮助");
	SetEvent("30201", "complete", L"完成");
	SetEvent("30201", "fail", L"放弃");
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
		return L"不堪一击";
	}
	if ( nExp < 100 )
	{
		return L"毫不足虑";
	}
	if ( nExp < 200 )
	{
		return L"不足挂齿";
	}
	if ( nExp < 400 )
	{
		return L"初学乍练";
	}
	if ( nExp < 800 )
	{
		return L"勉勉强强";
	}
	if ( nExp < 1600 )
	{
		return L"初窥门径";
	}
	if ( nExp < 3200 )
	{
		return L"初出茅庐";
	}
	if ( nExp < 6400 )
	{
		return L"略知一二";
	}
	if ( nExp < 12800 )
	{
		return L"普普通通";
	}
	if ( nExp < 25600 )
	{
		return L"平平淡淡";
	}
	if ( nExp < 51200 )
	{
		return L"平淡无奇";
	}
	if ( nExp < 100000 )
	{
		return L"粗通皮毛";
	}
	if ( nExp < 150000 )
	{
		return L"半生不熟";
	}
	if ( nExp < 200000 )
	{
		return L"半生不熟";
	}
	if ( nExp < 250000 )
	{
		return L"略有小成";
	}
	if ( nExp < 300000 )
	{
		return L"已有小成";
	}
	if ( nExp < 350000 )
	{
		return L"鹤立鸡群";
	}
	if ( nExp < 400000 )
	{
		return L"驾轻就熟";
	}
	if ( nExp < 450000 )
	{
		return L"青出于蓝";
	}
	if ( nExp < 500000 )
	{
		return L"已有大成";
	}
	return L"惊世骇俗";
}
