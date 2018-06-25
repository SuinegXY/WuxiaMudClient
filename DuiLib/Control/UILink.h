#ifndef __UILINK_H__
#define __UILINK_H__

#include <string>

#pragma once

namespace DuiLib
{
	class UILIB_API CLinkUI : public CButtonUI
	{
	public:
		CLinkUI();

		void SetLinkUrl(const char* pszLinkUrl);
		virtual bool Activate();

	protected:
		char m_szLinkUrl[MAX_PATH];
	};
}

#endif