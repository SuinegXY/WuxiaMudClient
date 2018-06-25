#ifndef __UIPROGRESS_H__
#define __UIPROGRESS_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CProgressUI : public CLabelUI
	{
	public:
		CProgressUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(TEventUI& event);

		bool IsHorizontal();
		void SetHorizontal(bool bHorizontal = true);
		bool IsStretchForeImage();
		void SetStretchForeImage(bool bStretchForeImage = true);
		int GetMinValue() const;
		void SetMinValue(int nMin);
		int GetMaxValue() const;
		void SetMaxValue(long long nMax);
		int GetValue() const;
		void SetValue(long long nValue);
		LPCTSTR GetForeImage() const;
		void SetForeImage(LPCTSTR pStrImage);
		void SetReversed(bool bReversed = true);
		void SetClick(bool bClick = true);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintStatusImage(HDC hDC);

	protected:
		bool m_bReversed;
		bool m_bHorizontal;
		bool m_bStretchForeImage;
		bool m_bClick;
		UINT m_uButtonState;
		long long m_nMax;
		long long m_nMin;
		long long m_nValue;

		CDuiString m_sForeImage;
		CDuiString m_sForeImageModify;
	};

} // namespace DuiLib

#endif // __UIPROGRESS_H__
