#include "stdafx.h"
#include "UILink.h"

namespace DuiLib
{
	CLinkUI::CLinkUI()
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
		ZeroMemory(m_szLinkUrl, MAX_PATH);
	}

	void CLinkUI::SetLinkUrl( const char* pszLinkUrl )
	{
		if ( pszLinkUrl )
		{
			sprintf(m_szLinkUrl, "\"%s\"", pszLinkUrl);
		}
	}

	bool CLinkUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if ( strlen(m_szLinkUrl) )
		{
			::ShellExecuteA(NULL, "open", "explorer.exe", m_szLinkUrl, NULL, SW_SHOW);
		}
		return true;
	}

}
