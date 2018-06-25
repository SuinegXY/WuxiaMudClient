#include "stdafx.h"
#include "UIGifAnim.h"

namespace DuiLib
{
	CGifAnimUI::CGifAnimUI(void)
		:m_pGif(NULL)
		,m_uTimerID(0)
		,m_nPreUpdateDelay(0)
		,m_isUpdateTime(false)
	{
	}

	CGifAnimUI::~CGifAnimUI(void)
	{
		m_pManager->KillTimer(this, m_uTimerID);

		if (m_pGif)
		{
			delete m_pGif;
			m_pGif = NULL;
		}
	}

	void CGifAnimUI::SetNormalGifFile(LPCTSTR pstrName)
	{
		if(pstrName == NULL) return;

		if (m_uTimerID == 0 && m_pManager)
		{
			m_uTimerID = m_pManager->GetLocalTimerID();;
		}

		if (m_pGif)
		{
			m_pManager->KillTimer(this, m_uTimerID);
			m_nPreUpdateDelay = 0;
			delete m_pGif;
			m_pGif = NULL;
		}

		m_pGif = CRenderEngine::LoadGif(STRINGorID(pstrName),0, 0);

		Invalidate();
	}

	void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("bkimage")) == 0 )
		{
			SetNormalGifFile(pstrValue);
		}

		CLabelUI::SetAttribute(pstrName,pstrValue);
	}

	void CGifAnimUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == m_uTimerID )
		{
			m_isUpdateTime = true;
			Invalidate();
			return;
		}

		CLabelUI::DoEvent(event);
	}

	void CGifAnimUI::SetVisible(bool bVisible)
	{
		if(bVisible == false)
			m_pManager->KillTimer(this, m_uTimerID);
		else
			m_nPreUpdateDelay = 0;
		CLabelUI::SetVisible(bVisible);
	}

	void CGifAnimUI::PaintBkImage(HDC hDC)
	{
		if(m_pGif)
		{
			TImageInfo* pImageInfo = NULL;
			if (m_isUpdateTime)
			{
				m_isUpdateTime = false;
				pImageInfo = m_pGif->GetNextFrameInfo();
			}
			else
			{
				pImageInfo = m_pGif->GetCurrentFrameInfo();
			}
			if (pImageInfo)
			{
				RECT rcBmpPart = {0};
				RECT rcCorners = {0};
				rcBmpPart.right = pImageInfo->nX;
				rcBmpPart.bottom = pImageInfo->nY;
				CRenderEngine::DrawImage(hDC,pImageInfo->hBitmap,m_rcItem, m_rcPaint,rcBmpPart,rcCorners,pImageInfo->alphaChannel,255);
				if (m_nPreUpdateDelay != pImageInfo->delay)
				{
					m_pManager->KillTimer(this, m_uTimerID);
					m_pManager->SetTimer(this, m_uTimerID, pImageInfo->delay);
					m_nPreUpdateDelay = pImageInfo->delay;
				}
			}
		}
	}
}