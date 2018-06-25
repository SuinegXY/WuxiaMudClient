#ifndef _ROLEWND_H_
#define _ROLEWND_H_

#include "Wnd/BaseWnd.h"

class CRoleWnd : public CBaseWnd
{
public:
	CRoleWnd();

	virtual LPCTSTR GetWindowClassName() const; 
	virtual CDuiString GetSkinFile(void);
	virtual LPCTSTR GetClass() const;
	virtual void InitWindow(void);

protected:
	virtual void OnClick(TNotifyUI& msg);

	static void CallbackName(const char* pszContant);
	static void CallbackGift(const char* pszContant);
	static void CallbackExpAndPot(const char* pszContant);
	static void CallbackMoney(const char* pszContant);
	static void CallbackItems(const char* pszContant);
	static void CallbackSkills(const char* pszContant);

	void SetItems(string& strItemsList);
	void AddItem(string& strID);
	void FixItem(string& strID, BOOL bAdd);
	void SetSkill(string& strSkill);

private:
	static CRoleWnd* s_pThis;
	int m_nLevel;
	int m_nGift[4];
	int m_nGiftSkill[4];
	map<string, CButtonUI*> m_mapItems;
	map<string, int> m_mapItemNum;
	map<string, CLabelUI*> m_mapSkillsLevel;
	map<string, CLabelUI*> m_mapSkillsExp;

	CVerticalLayoutUI* m_pItemsLayout;
};

#endif