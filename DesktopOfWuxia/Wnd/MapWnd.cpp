#include "stdafx.h"
#include "MapWnd.h"
#include "Define.h"
#include "GameInfo\Map.h"
#include "Util\ATW.h"

#define ROOM_HEIGHT		20
#define ROOM_WIDTH_ROOM	56
#define ROOM_WIDTH_LINK	10

LPCTSTR CMapWnd::GetWindowClassName() const
{
	return _T("MapFrame");
}

CDuiString CMapWnd::GetSkinFile( void )
{
	return _T("Map.xml");
}

LPCTSTR CMapWnd::GetClass() const
{
	return _T("CMapFrame");
}

CMapWnd::CMapWnd()
{
	szMapIDBuffer1 = 0;
	szMapIDBuffer2 = 0;
}

void CMapWnd::InitWindow( void )
{
	RECT rcDesktop;
	GetWinClientRect(rcDesktop);
	MoveWindow(m_hWnd, rcDesktop.right, VALUE_LIST_WND_CY+VALUE_MAIN_FRAME_CY+VALUE_HEIGHT_DIST, 1, 1, TRUE);
	m_pLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("Layout")));
}

void CMapWnd::SetMapID( const char* pszRoomID )
{
	static string strRoomLast = "";
	if ( szMapIDBuffer1 == pszRoomID[1] && szMapIDBuffer2 == pszRoomID[2] )
	{
		if ( strRoomLast.length() )
		{
			CLabelUI* pRoomLast = m_mapRoom[strRoomLast];
			ControlAction(pRoomLast, SetBorderSize, 0);
		}
		CLabelUI* pRoomNow = m_mapRoom[pszRoomID];
		ControlAction(pRoomNow, SetBorderSize, 1);
	}
	else
	{
		m_mapRoom.clear();
		m_pLayout->RemoveAll();
		szMapIDBuffer1 = pszRoomID[1];
		szMapIDBuffer2 = pszRoomID[2];
		char szMapID[3] = {szMapIDBuffer1, szMapIDBuffer2, '\0'};
		int nHeight = 2;
		int nWidth = 0;
		wstring strMapContant = CMapInfo::GetInstance()->GetInfo(string(szMapID));

		while ( strMapContant.length() ) 
		{
			nHeight += ROOM_HEIGHT;
			wstring strLine = GetStringHeadW(strMapContant, L"`");
			BOOL bCaleWidth = nWidth ? FALSE : TRUE;
			CHorizontalLayoutUI* pLine = new CHorizontalLayoutUI;
			m_pLayout->Add(pLine);
			m_pLayout->SetFixedHeight(ROOM_HEIGHT);
			int nDigit = 0;
			while ( strLine.length() )
			{
				int nWidthBuffer = nDigit%2 == 0 ? ROOM_WIDTH_ROOM : ROOM_WIDTH_LINK;
				++nDigit;
				if ( bCaleWidth )
				{
					nWidth += nWidthBuffer;
				}
				wstring strRoom = GetStringHeadW(strLine, L"~");
				CLabelUI* pRoom = new CLabelUI;
				if ( pRoom )
				{
					pLine->Add(pRoom);
					pRoom->SetFixedHeight(ROOM_HEIGHT);
					pRoom->SetFixedWidth(nWidthBuffer);

					wstring strRoomName = GetStringHeadW(strRoom, L"_");
					if ( strRoomName.length() )
					{
						RECT rc = {0,-2,0,0};
						pRoom->SetTextPadding(rc);
						pRoom->SetFont(0);
						pRoom->SetTextColor(0xffcccccc);
						pRoom->SetText(strRoomName.c_str());
						pRoom->SetAttribute(_T("align"), _T("vcenter"));
						pRoom->SetBorderColor(0xffcc2222);
						if ( strcmp(WtoA(strRoom).c_str(), pszRoomID) == 0 )
						{
							pRoom->SetBorderSize(1);
						}
					}
					m_mapRoom.insert(make_pair(WtoA(strRoom), pRoom));
				}
			}
		};
		if ( nWidth )
		{
			RECT rc;
			GetWindowRect(m_hWnd, &rc);
			MoveWindow(m_hWnd, rc.right-nWidth, rc.top, nWidth, nHeight, TRUE);
		}
	}
	strRoomLast = pszRoomID;
}
