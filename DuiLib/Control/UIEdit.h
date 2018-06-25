#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

#define  WM_EDITKEYDOWN		(WM_USER+1000)
#define  WM_EDITCHAR		(WM_USER+1001)

namespace DuiLib
{
	class CEditWnd;

	class UILIB_API CEditUI : public CLabelUI
	{
		friend class CEditWnd;
	public:
		CEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnable = true);
		void SetText(LPCTSTR pstrText);
		void SetMaxChar(UINT uMax);
		UINT GetMaxChar();
		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;
		void SetPasswordMode(bool bPasswordMode);
		bool IsPasswordMode() const;
		void SetPasswordChar(TCHAR cPasswordChar);
		TCHAR GetPasswordChar() const;
		void SetNumberOnly(bool bNumberOnly);
		bool IsNumberOnly() const;
		int GetWindowStyls() const;

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetNativeEditBkColor(DWORD dwBkColor);
		DWORD GetNativeEditBkColor() const;

	    CDuiString GetSelText();
		void SetSelText(LPCTSTR pStrSel);
		void SetSel(long nStartChar, long nEndChar);
		void SetSelAll();
		void SetReplaceSel(LPCTSTR lpszReplace);

		void SetPos(RECT rc);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		virtual void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);

		void SetEditPopUpMenu();

		void SetBossKeyState(bool bBosskeyed = true, bool bSpecial = true);

		bool GetBossKeyState();

		void SetCtrlKeyDown();
		void SetShiftKeyDown();
		void SetAltKeyDown();
		void SetKey(UINT nKey);
		void ClearKeyDown();

		void SetOldCtrlKeyDown();
		void SetOldShiftKeyDown();
		void SetOldAltKeyDown();
		void SetOldKey(UINT nKey);

		UINT GetOldCtrlKeyDown();
		UINT GetOldShiftKeyDown();
		UINT GetOldAltKeyDown();
		UINT GetOldKey();

		UINT GetCtrlKeyDown();
		UINT GetShiftKeyDown();
		UINT GetAltKeyDown();
		UINT GetKey();

		CDuiString GetKeyASCIIToText(UINT nChar);
		void SetBossKeyText();
		bool CheckKeyRepeat(UINT nCtrlKey,UINT nShiftKey,UINT nAltKey,UINT nKey);

		void SetShortcutKey();

		void SetAddrEdit();

		void SetDealKeyBySelf();

		void SetTextBackground(LPCTSTR pstrText);
		void SetTextBackgroundColor(DWORD dwColor);

	protected:
		CEditWnd* m_pWindow;

		UINT m_uMaxChar;
		bool m_bReadOnly;
		bool m_bPasswordMode;
		TCHAR m_cPasswordChar;
		UINT m_uButtonState;
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;
		DWORD m_dwEditbkColor;
		int m_iWindowStyls;
		bool m_bIsPopMenu;
		bool m_bBossKey;//kk:add//ÀÏ°å¼üÓÃµÄedit
		bool m_bSpeicalKey;

		UINT m_nCtrlKeyDown;
		UINT m_nShiftKeyDown;
		UINT m_nAltKeyDown;
		UINT m_nKey;

		UINT m_nOldCtrlKeyDown;
		UINT m_nOldShiftKeyDown;
		UINT m_nOldAltKeyDown;
		UINT m_nOldKey;
		bool m_bIsAddrEdit;
		bool m_bDealKeyBySelf;
		CDuiString m_szSelText;

		CDuiString m_sTextBackground;
		DWORD m_dwTextBackgroundColor;
	};
}
#endif // __UIEDIT_H__