#include "stdafx.h"
#include "UIProgress.h"

namespace DuiLib
{
	CProgressUI::CProgressUI() : m_bHorizontal(true), m_nMin(0), m_nMax(100), m_nValue(0), m_bStretchForeImage(true), m_bReversed(false)
	{
		m_uTextStyle = DT_SINGLELINE | DT_CENTER;
		m_bClick = false;
		m_uButtonState = 0;
		SetFixedHeight(12);
	}

	LPCTSTR CProgressUI::GetClass() const
	{
		return _T("ProgressUI");
	}

	LPVOID CProgressUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_PROGRESS) == 0 ) return static_cast<CProgressUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	bool CProgressUI::IsHorizontal()
	{
		return m_bHorizontal;
	}

	void CProgressUI::SetHorizontal(bool bHorizontal)
	{
		if( m_bHorizontal == bHorizontal ) return;

		m_bHorizontal = bHorizontal;
		Invalidate();
	}

	int CProgressUI::GetMinValue() const
	{
		return m_nMin;
	}

	void CProgressUI::SetMinValue(int nMin)
	{
		m_nMin = nMin;
		Invalidate();
	}

	int CProgressUI::GetMaxValue() const
	{
		return m_nMax;
	}

	void CProgressUI::SetMaxValue(long long nMax)
	{
		m_nMax = nMax;
		Invalidate();
	}

	int CProgressUI::GetValue() const
	{
		return m_nValue;
	}

	void CProgressUI::SetValue(long long nValue)
	{
		m_nValue = nValue;
		Invalidate();
	}

	LPCTSTR CProgressUI::GetForeImage() const
	{
		return m_sForeImage;
	}

	void CProgressUI::SetForeImage(LPCTSTR pStrImage)
	{
		if( m_sForeImage == pStrImage ) return;

		m_sForeImage = pStrImage;
		Invalidate();
	}

	void CProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("min")) == 0 ) SetMinValue(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("max")) == 0 ) SetMaxValue(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("value")) == 0 ) SetValue(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("isstretchfore"))==0) SetStretchForeImage(_tcscmp(pstrValue, _T("true")) == 0? true : false);
		else if( _tcscmp(pstrName, _T("click"))==0) SetClick(_tcscmp(pstrValue, _T("true")) == 0? true : false);
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CProgressUI::PaintStatusImage(HDC hDC)
	{
		if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
		if( m_nValue > m_nMax ) m_nValue = m_nMax;
		if( m_nValue < m_nMin ) m_nValue = m_nMin;

		RECT rc = {0};
		if( m_bHorizontal ) {
			rc.right = (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
			rc.bottom = m_rcItem.bottom - m_rcItem.top;
		}
		else {
			if ( m_bReversed )
			{
				rc.top = 0;
				rc.right = m_rcItem.right - m_rcItem.left;
				rc.bottom = (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
			}
			else
			{
				rc.top = (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
				rc.right = m_rcItem.right - m_rcItem.left;
				rc.bottom = m_rcItem.bottom - m_rcItem.top;
			}
		}

		if( !m_sForeImage.IsEmpty() ) {
			m_sForeImageModify.Empty();
			if (m_bStretchForeImage)
				m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);
			else
				m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d' source='%d,%d,%d,%d'")
				, rc.left, rc.top, rc.right, rc.bottom
				, rc.left, rc.top, rc.right, rc.bottom);

			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage, (LPCTSTR)m_sForeImageModify) ) m_sForeImage.Empty();
			else return;
		}
	}

	bool CProgressUI::IsStretchForeImage()
	{
		return m_bStretchForeImage;
	}

	void CProgressUI::SetStretchForeImage( bool bStretchForeImage /*= true*/ )
	{
		if (m_bStretchForeImage==bStretchForeImage)		return;
		m_bStretchForeImage=bStretchForeImage;
		Invalidate();
	}

	void CProgressUI::SetReversed( bool bReversed /*= true*/ )
	{
		m_bReversed = bReversed;
	}

	void CProgressUI::SetClick( bool bClick /*= true*/ )
	{
		m_bClick = bClick;
	}

	void CProgressUI::DoEvent( TEventUI& event )
	{
		if ( m_bClick )
		{
			if( event.Type == UIEVENT_KILLFOCUS ) 
			{
				if ( m_uButtonState == UISTATE_CAPTURED )
				{
					GetManager()->SendNotify(this, DUI_MSGTYPE_MOVEEND);
				}
				m_uButtonState = 0;
				Invalidate();
			}
			else if( event.Type == UIEVENT_BUTTONDOWN  )
			{
				if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
					m_uButtonState = UISTATE_CAPTURED;
				}
				return;
			}
			else if( event.Type == UIEVENT_MOUSEMOVE )
			{
				if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
					int nLen = m_rcItem.right - m_rcItem.left;
					m_nValue = (event.ptMouse.x - m_rcItem.left)*(m_nMax-m_nMin)/nLen + m_nMin;
					if ( m_nValue < m_nMin ) m_nValue = m_nMin;
					if ( m_nValue > m_nMax ) m_nValue = m_nMax;
					Invalidate();
				}
				return;
			}
			else if( event.Type == UIEVENT_BUTTONUP )
			{
				if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
					//if( ::PtInRect(&m_rcItem, event.ptMouse) );
					int nLen = m_rcItem.right - m_rcItem.left;
					m_nValue = (event.ptMouse.x - m_rcItem.left)*(m_nMax-m_nMin)/nLen + m_nMin;
					if ( m_nValue < m_nMin ) m_nValue = m_nMin;
					if ( m_nValue > m_nMax ) m_nValue = m_nMax;
					GetManager()->SendNotify(this, DUI_MSGTYPE_MOVEEND);
					m_uButtonState = 0;
					Invalidate();
				}
				return;
			}
		}

		CLabelUI::DoEvent(event);
	}

}
