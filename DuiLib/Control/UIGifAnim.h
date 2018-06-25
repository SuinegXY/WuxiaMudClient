#ifndef _UIGIFANIM_H_
#define _UIGIFANIM_H_

namespace DuiLib
{
	class UILIB_API CGifAnimUI : public CLabelUI
	{
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		LPCTSTR GetClass() const
		{
			return _T("GifAnimUI");
		}

		LPVOID GetInterface( LPCTSTR pstrName )
		{
			if( _tcscmp(pstrName, _T("GifAnimUI")) == 0 ) 
				return static_cast<CGifAnimUI*>(this);

			return CControlUI::GetInterface(pstrName);
		}

		void SetNormalGifFile(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual void DoEvent(TEventUI& event);
		virtual void SetVisible(bool bVisible = true);//设置控件可视性
		virtual void PaintBkImage(HDC hDC);
	protected:

		bool m_isUpdateTime;
		CGifHandler* m_pGif;
		int m_nPreUpdateDelay;
		UINT m_uTimerID;
	};
}
#endif // _UIGIFANIM_H_
