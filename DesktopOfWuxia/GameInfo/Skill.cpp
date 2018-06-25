#include "stdafx.h"
#include "Skill.h"

InstanceRealize(CSkillInfo);

CSkillInfo::CSkillInfo()
{

}

void CSkillInfo::init()
{
	SetInfo("40001", L"读书识字");
	SetInfo("40010", L"基本招架");
	SetInfo("40011", L"基本轻功");
	SetInfo("40012", L"基本内功");
	SetInfo("40013", L"基本拳法");
	//--------------------------//
	SetInfo("40101", L"游侠身法");
	//--------------------------//
	SetInfo("40201", L"游侠心法");
	//--------------------------//
	SetInfo("40301", L"太祖长拳");
	//--------------------------//

}
