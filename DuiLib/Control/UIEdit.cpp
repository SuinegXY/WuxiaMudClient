#include "stdafx.h"
#include "UIEdit.h"

namespace DuiLib
{
	#define  ID_EDIT_PASTE_ANDOPEN     200
	#define  ID_EDIT_UNDO			   201
	#define  ID_EDIT_CUT               202
	#define  ID_EDIT_COPY              203
	#define  ID_EDIT_PASTE             204
	#define  ID_EDIT_CLEAR             205
	#define  ID_EDIT_SELECT_ALL		   206
	class CEditWnd : public CWindowWnd
	{
	public:
		CEditWnd();

		void Init(CEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		//kk:add 2012-11-30 editaddr扩展右键弹出菜单 
		bool SetEditPopUpMenu();
		bool SetEditBossKey();
		DWORD GetStyle();
		BOOL CanUndo();
		DWORD GetSel();
		//为bosskey特别写的函数
		
		CDuiString GetKeyASCIIToText(UINT nChar);
		void SetBossKeyText();
		void SetCtrlKeyDown();
		void SetShiftKeyDown();
		void SetAltKeyDown();
		void SetKey(UINT nKey);
		void SetDealKeySelf();
		void SetAddrEdit();
		void SetSpeicalKey(bool bSpeical);

	protected:
		CEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		//kk:add
		bool m_bIsKeyDown; //左键是否单击过
		bool m_bIsPopMenu; //是否弹出自己的菜单
		bool m_bBossKeyed;
		bool m_bDealKeySelf;
		bool m_bIsAddrEdit;
		bool m_bSpeical;

		HMENU m_popupmenu;
		bool m_bCtrlKeyDown;
		bool m_bShiftKeyDown;
		bool m_bAltKeyDown;
		UINT m_nKey;//按键

		
	};

	void CEditWnd::SetBossKeyText()
	{
		if (m_nKey < 1)
		{
			Edit_SetText(m_hWnd,_T(""));
			return ;
		}
		CDuiString szText = _T("");
		CDuiString szKey = _T("");
		szKey = GetKeyASCIIToText(m_nKey);
		if ( m_bSpeical )
		{
			if (m_bCtrlKeyDown)
			{
				szText = _T("Ctrl + ");
			}
			if (m_bShiftKeyDown)
			{
				szText += L"Shift + ";
			}
			if (m_bAltKeyDown)
			{
				szText += L"Alt + ";
			}
		}
		if (!szKey.IsEmpty())
		{
			szText += szKey;
			//这个有了之后 检查所有快捷键 是否有冲突
			//把当前的值给edit 然后判断发送一 个消息 去检测下是否和本程序中快捷键冲突 如果没冲突那么给old如果冲突那么就从old恢复
			m_pOwner->SetKey(m_nKey);
			if (m_bCtrlKeyDown)
			{
				m_pOwner->SetCtrlKeyDown();
			}
			if (m_bShiftKeyDown)
			{
				m_pOwner->SetShiftKeyDown();
			}
			if (m_bAltKeyDown)
			{
				m_pOwner->SetAltKeyDown();
			}
		}
		if (!szText.IsEmpty())
		{
			Edit_SetText(m_hWnd, szText.GetData());
			m_pOwner->GetManager()->SendNotify(m_pOwner, _T("checkshortcutkey"));
			m_pOwner->NeedUpdate();
			return ;
		}
		Edit_SetText(m_hWnd,_T(""));

	}

	CDuiString CEditWnd::GetKeyASCIIToText(UINT nChar)
	{
		CDuiString szKey = _T("");
		if (nChar>='A'&&nChar<='Z')
		{
			TCHAR   lpszFile[2];
			lpszFile[0]=nChar;
			lpszFile[1]='\0';
			szKey.Format(_T("%s"),lpszFile);
 		}
		else if (nChar>='0'&&nChar<='9')
		{
			TCHAR   lpszFile[2];
			lpszFile[0]=nChar;
			lpszFile[1]='\0';
			szKey.Format(_T("%s"),lpszFile);
		}
		else
		{
			switch(nChar)
			{
			case 32:
				szKey = L"Space";
				break;
			case 27:
				szKey = L" ";
				break;
			case 192:
				szKey = L"~";
				break;
			case 112:
				szKey = L"F1";
				break;
			case 113:
				szKey = L"F2";
				break;
			case 114:
				szKey = L"F3";
				break;
			case 115:
				szKey = L"F4";
				break;
			case 116:
				szKey = L"F5";
				break;
			case 117:
				szKey = L"F6";
				break;
			case 118:
				szKey = L"F7";
				break;
			case 119:
				szKey = L"F8";
				break;
			case 120:
				szKey = L"F9";
				break;
			case 121:
				szKey = L"F10";
				break;
			case 122:
				szKey = L"F11";
				break;
			case 123:
				szKey = L"F12";
				break;
			case 189:
				szKey = L"-";
				break;
			case 187:
				szKey = L"=";
				break;
			case 8:
				szKey = L" ";
				break;
			case 219:
				szKey = L"[";
				break;
			case 13:
				szKey = L"Enter";
				break;
			case 220:
				szKey = L"\\";
				break;
			case 221:
				szKey = L"]";
				break;
			case 222:
				szKey = L"'";
				break;
			case 186:
				szKey = L";";
				break;
			case 188:
				szKey = L",";
				break;
			case 190:
				szKey = L".";
				break;
			case 191:
				szKey = L"/";
				break;
			case 37:
				szKey = L"Left";
				break;
			case 38:
				szKey = L"Up";
				break;
			case 39:
				szKey = L"Right";
				break;
			case 40:
				szKey = L"Down";
				break;
			case 45:
				szKey = L"Insert";
				break;
			case 46:
				szKey = L"Delete";
				break;
			case 33:
				szKey = L"Page Up";
				break;
			case 34:
				szKey = L"Page Down";
				break;
			case 35:
				szKey = L"End";
				break;
			case 36:
				szKey = L"Home";
				break;
			case 96:
				szKey = !m_bSpeical ? L"0" : L"Num0";
				break;
			case 97:
				szKey = !m_bSpeical ? L"1" : L"Num1";
				break;
			case 98:
				szKey = !m_bSpeical ? L"2" : L"Num2";
				break;
			case 99:
				szKey = !m_bSpeical ? L"3" : L"Num3";
				break;
			case 100:
				szKey = !m_bSpeical ? L"4" : L"Num4";
				break;
			case 101:
				szKey = !m_bSpeical ? L"5" : L"Num5";
				break;
			case 102:
				szKey = !m_bSpeical ? L"6" : L"Num6";
				break;
			case 103:
				szKey = !m_bSpeical ? L"7" : L"Num7";
				break;
			case 104:
				szKey = !m_bSpeical ? L"8" : L"Num8";
				break;
			case 105:
				szKey = !m_bSpeical ? L"9" : L"Num9";
				break;
			case 106:
				szKey = !m_bSpeical ? L" " : L"Num*";
				break;
			case 107:
				szKey = !m_bSpeical ? L" " : L"Num+";
				break;
			case 109:
				szKey = !m_bSpeical ? L" " : L"Num-";
				break;
			case 111:
				szKey = !m_bSpeical ? L" " : L"Num/";
				break;
			}
		}
		return szKey;
	}

	void CEditWnd::SetCtrlKeyDown()
	{
		m_bCtrlKeyDown = true;
	}
	void CEditWnd::SetShiftKeyDown()
	{
		m_bShiftKeyDown = true;
	}
	void CEditWnd::SetAltKeyDown()
	{
		m_bShiftKeyDown = true;
	}
	void CEditWnd::SetKey(UINT nKey)
	{
		m_nKey = nKey;
	}

	void CEditWnd::SetDealKeySelf()
	{
		m_bDealKeySelf=true;
	}

	CEditWnd::CEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
	{
		m_bIsPopMenu = false;
		m_bBossKeyed = false;
		m_bCtrlKeyDown = false;
		m_bShiftKeyDown = false;
		m_bAltKeyDown = false;
		m_nKey = 0;
		m_bDealKeySelf=false;
	}

	DWORD CEditWnd::GetStyle()
	{
		return (DWORD)::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L);
	}

	BOOL CEditWnd::CanUndo()
	{
		return (BOOL)::SendMessage(m_hWnd,EM_CANUNDO,0, 0);
	}

	DWORD CEditWnd::GetSel()
	{
		return (DWORD)::SendMessage(m_hWnd, EM_GETSEL, 0, 0L);
	}

	bool CEditWnd::SetEditPopUpMenu()
	{
		if (m_bIsPopMenu)
		{
			return true;
		}
		m_bIsPopMenu = true;
		return true;
	}

	bool CEditWnd::SetEditBossKey()
	{
		if ( !m_bBossKeyed )
		{
			m_bBossKeyed = true;
		}
		return true;
	}

	void CEditWnd::SetAddrEdit()
	{
		if (m_bIsAddrEdit)
		{
			return ;
		}
		m_bIsAddrEdit = true;
		return ;
	}

	void CEditWnd::Init(CEditUI* pOwner)
	{
		m_pOwner = pOwner;
		m_popupmenu = NULL;
		m_bIsKeyDown = false;
		RECT rcPos = CalPos();
		UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
		if( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);
		//Styls
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);
		m_bInit = true;    
	}

	RECT CEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if( lEditHeight < rcPos.GetHeight() ) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}
		return rcPos;
	}

	LPCTSTR CEditWnd::GetWindowClassName() const
	{
		return _T("EditWnd");
	}

	LPCTSTR CEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CEditWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		if (m_popupmenu)
		{
			DestroyMenu(m_popupmenu);
		}
		m_pOwner->Invalidate();
		// Clear reference and die
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;

		if(m_bDealKeySelf)
		{
			if(uMsg==WM_CHAR)
			{
				::PostMessage(GetParent(m_hWnd),WM_EDITCHAR,wParam,(LPARAM)m_hWnd);//编辑框按键消息处理	
				return lRes;
			}

			if(uMsg==WM_KEYDOWN)
			{
				::SendMessage(GetParent(m_hWnd),WM_EDITKEYDOWN,wParam,(LPARAM)m_hWnd);//编辑框按键消息处理	
				return lRes;
			}

		}
		

		if (m_bBossKeyed)
		{
			if (uMsg == WM_CHAR)
			{
				return lRes;
			}
		}

		if (m_bIsAddrEdit)
		{
			if (uMsg == WM_CHAR)
			{
				//获取选中
				int nstatsel = 0;
				int nendsel = 0;
				::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nstatsel, (LPARAM)&nendsel);
			//	DWORD nSel = GetSel();
				TCHAR* szText ;
				int nTextLeng = ::GetWindowTextLength(m_hWnd) + 1;
				szText = new TCHAR[nTextLeng];
				if (nstatsel>0&&nendsel>0&&nendsel>nstatsel)
				{
					Edit_GetText(m_hWnd,szText,nTextLeng);
					CDuiString szEditText = szText;
					CDuiString szSelFirstText = wParam;
					CDuiString szFirstText = szEditText.Mid(nstatsel,1);
					if (_tcscmp(szFirstText,szSelFirstText) == 0)
					{
						Edit_SetSel(m_hWnd,nstatsel+1,nendsel);
						CDuiString szSelText = szEditText.Mid(nstatsel+1,nendsel - nstatsel);
						m_pOwner->SetSelText(szSelText);
						//设置选中部分的字
						::SendMessage(GetParent(m_hWnd),WM_USER + 177,0,0);
						BOOL Tmp = TRUE;
						lRes = OnEditChanged(uMsg, wParam, lParam, Tmp);
						//触发下消息从新进行查询下
						delete szText;
						return lRes;
					}
				}
				delete szText;
			}
		}
		BOOL bHandled = TRUE;
		if (uMsg == WM_LBUTTONDBLCLK)
		{
			::SendMessage(m_hWnd, EM_SETSEL, 0, -1);
			return lRes;
		}
		if ( uMsg == WM_CREATE )
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_SETFOCUS);
		}
 		if (uMsg == WM_CONTEXTMENU)
		{//右键菜单
			//POINT pt = 
			if (m_bIsPopMenu)
			{
				POINT pt;
				pt.x=LOWORD(lParam);
				pt.y=HIWORD(lParam);
 			//	ClientToScreen(m_hWnd,&pt);		
				if (m_popupmenu)
				{
					DestroyMenu(m_popupmenu);
				}
				m_popupmenu = ::CreatePopupMenu();
				BOOL bReadOnly = GetStyle() & ES_READONLY;
				DWORD flags = IsClipboardFormatAvailable(CF_TEXT) &&
					!bReadOnly ? 0 : MF_GRAYED;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_PASTE_ANDOPEN,_T("粘贴并打开(&O)   Ctrl+Shift+X"));
				AppendMenu(m_popupmenu,MF_SEPARATOR,0,NULL);
				flags = CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_UNDO,_T("撤销(&U)               Ctrl+Z"));
				AppendMenu(m_popupmenu,MF_SEPARATOR,0,NULL);
				DWORD sel = GetSel();
				flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
				flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_CUT,_T("剪切(&T)               Ctrl+X"));
				flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_COPY,_T("复制(&C)               Ctrl+C"));
				flags = IsClipboardFormatAvailable(CF_TEXT) &&
					!bReadOnly ? 0 : MF_GRAYED;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_PASTE,_T("粘贴(&P)               Ctrl+V"));
				flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_CLEAR,_T("删除(&D)                  Del"));
				AppendMenu(m_popupmenu,MF_SEPARATOR,0,NULL);
				int len = GetWindowTextLength(m_hWnd);
				flags = (!len || (LOWORD(sel) == 0 && HIWORD(sel) ==
					len)) ? MF_GRAYED : 0;
				AppendMenu(m_popupmenu,MF_STRING|flags,ID_EDIT_SELECT_ALL,_T("全选(&A)               Ctrl+A"));
 				TrackPopupMenu(m_popupmenu,0x0002L,pt.x,pt.y,0,m_hWnd,NULL);
				DestroyMenu(m_popupmenu);
				m_popupmenu = NULL;
				return S_OK;
			}
		}
		if (uMsg == WM_COMMAND)
		{
			UINT uID = LOWORD(wParam);
			if (m_bIsPopMenu)
			{
				switch(uID)
				{
				case ID_EDIT_PASTE_ANDOPEN:
					::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
					//发送给主窗口 浏览准备浏览网站

					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(GetParent(m_hWnd),WM_USER+165/*WM_ADDREDIT_OPENURL*/,wParam,lParam);
					break;
				case ID_EDIT_UNDO:
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(m_hWnd, EM_UNDO, 0, 0L);
					break;
				case ID_EDIT_CUT:
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(m_hWnd, WM_CUT, 0, 0L);
					break;
				case ID_EDIT_COPY:
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(m_hWnd, WM_COPY, 0, 0L);
					break;
				case ID_EDIT_PASTE:
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
					break;
				case ID_EDIT_CLEAR:
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
					break;
				case ID_EDIT_SELECT_ALL:
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);		//发给主窗口，不要变化了
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);	//发给主窗口，不要变化了

					::SendMessage(m_hWnd, EM_SETSEL, 0, -1);
					break;
				}
				return S_OK;
			}
		}
		if (uMsg == WM_KILLFOCUS)
		{
			m_nKey = 0;
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_KILLFOCUS);
		}
		else if ( uMsg == WM_SETFOCUS )
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_SETFOCUS);
		}
		if (uMsg == WM_KEYUP)
		{
			if (m_bBossKeyed)
			{
				if (m_bCtrlKeyDown||m_bShiftKeyDown)
				{
					if (m_nKey < 1 || m_nKey == VK_MENU || m_nKey == VK_SHIFT || m_nKey == VK_CONTROL)
					{
						m_nKey = 0;
						m_bCtrlKeyDown = false;
						m_bShiftKeyDown = false;
						m_bAltKeyDown = false;
						m_pOwner->ClearKeyDown();
						m_nKey = m_pOwner->GetOldKey();
						bool bSkip = false;
						if (m_pOwner->GetOldCtrlKeyDown()>0)
						{
							m_bCtrlKeyDown = true;
							bSkip = true;
						}
						if (m_pOwner->GetOldShiftKeyDown()>0)
						{
							m_bShiftKeyDown = true;
							bSkip = true;
						}
						if (m_pOwner->GetOldAltKeyDown()>0)
						{
							m_bAltKeyDown = true;
							bSkip = true;
						}
						if ( !bSkip || !m_bSpeical )
						{
							SetBossKeyText();
						}
					}
				}
				m_nKey = 0;
				m_bCtrlKeyDown = false;
				m_bShiftKeyDown = false;
				m_bAltKeyDown = false;
				m_pOwner->ClearKeyDown();
				return 0;
			}
		}
		if (uMsg == WM_SYSKEYUP)
		{
			if (m_bBossKeyed)
			{
				if (m_bAltKeyDown)
				{
					if (m_nKey < 1 || m_nKey == VK_MENU || m_nKey == VK_SHIFT || m_nKey == VK_CONTROL)
					{
						m_nKey = 0;
						m_bCtrlKeyDown = false;
						m_bShiftKeyDown = false;
						m_bAltKeyDown = false;
						m_pOwner->ClearKeyDown();
						m_nKey = m_pOwner->GetOldKey();
						bool bSkip = false;
						if (m_pOwner->GetOldCtrlKeyDown()>0)
						{
							m_bCtrlKeyDown = true;
							bSkip = true;
						}
						if (m_pOwner->GetOldShiftKeyDown()>0)
						{
							m_bShiftKeyDown = true;
							bSkip = true;
						}
						if (m_pOwner->GetOldAltKeyDown()>0)
						{
							m_bAltKeyDown = true;
							bSkip = true;
						}
						if ( !bSkip || !m_bSpeical )
						{
							SetBossKeyText();
						}
					}
				}
				m_nKey = 0;
				m_bCtrlKeyDown = false;
				m_bShiftKeyDown = false;
				m_bAltKeyDown = false;
				m_pOwner->ClearKeyDown();
				return 0;
			}
		}
		if (uMsg == WM_SYSKEYDOWN)
		{
			if (m_bBossKeyed)
			{
				if (::GetAsyncKeyState(VK_MENU)<0)
				{
					m_bAltKeyDown = true;
					m_pOwner->SetAltKeyDown();
				}
				else
				{
					int x = 0;
				}
				m_nKey = wParam;
// 				if (m_nKey == 70)
// 				{
// 					int x = 0;
// 				}
				//m_pOwner->SetKey(m_nKey);
				SetBossKeyText();
				return 0;
			}
		}
		if ( uMsg == WM_KEYDOWN )
		{

			if (m_bBossKeyed)
			{
				if (::GetAsyncKeyState(VK_SHIFT)<0)
				{
					m_bShiftKeyDown = true;
					m_pOwner->SetShiftKeyDown();
				}
				if (::GetAsyncKeyState(VK_CONTROL)<0)
				{
					m_bCtrlKeyDown = true;
					m_pOwner->SetCtrlKeyDown();
				}
				m_nKey = wParam;
// 				if (m_nKey == 70)
// 				{
// 					int x = 0;
// 				}
				//m_pOwner->SetKey(m_nKey);
				m_pOwner->SetOldKey(m_nKey);
				SetBossKeyText();
				return -1;
			}
			int nstatsel = 0;
			int nendsel = 0;
			::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nstatsel, (LPARAM)&nendsel);
			if(wParam == 8 || wParam == 46)
			{
				if (nstatsel>0&&nendsel>0&&nendsel>nstatsel)
				{
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,-1,-1);	
				}
				else
				{
					::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,-1,0);
					::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,0,0);
				}
			}
			else
			{
				::SendMessage(GetParent(m_hWnd),WM_USER+172/*WM_SETADDREDITBACKSPACEFLAG*/,0,0);
				::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,0,0);
			}
		//	::SendMessage(GetParent(m_hWnd),WM_USER+109/*WM_SETADDRLISTSHOWSTATE*/,wParam,lParam);
		}
		if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if( uMsg == OCM_COMMAND ) {
			if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) 
			{
			//	m_bIsKeyDown = false;
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}//此处增加3个消息 为了满足addredit要求
		else if ( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_DOWN)
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, _T("editdown"));
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_UP )
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, _T("editup"));
		}
		else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) 
		{//修改增加一个系统按钮判断
			if (::GetAsyncKeyState(VK_CONTROL)<0)
			{
				m_pOwner->GetManager()->SendNotify(m_pOwner, _T("editctrlandenter"));
			}
			else
			{
				m_pOwner->GetManager()->SendNotify(m_pOwner, _T("return"));
			}
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == 'X')
		{
			if (::GetAsyncKeyState(VK_CONTROL)<0 && ::GetAsyncKeyState(VK_SHIFT)<0)
			{
				::SendMessage(m_hWnd, WM_PASTE, 0, 0L);
				//发送给主窗口 浏览准备浏览网站
				::SendMessage(GetParent(m_hWnd),WM_USER+165/*WM_ADDREDIT_OPENURL*/,wParam,lParam);
			}
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == 'A')
		{
			if (::GetAsyncKeyState(VK_CONTROL)<0)
			{
				::SendMessage(m_hWnd, EM_SETSEL, 0, -1);
			}
		}
		else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
			if( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
			::SetBkMode((HDC)wParam, TRANSPARENT);
			DWORD dwTextColor = m_pOwner->GetTextColor();
			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
			if( m_hBkBrush == NULL ) {
				DWORD clrColor = m_pOwner->GetNativeEditBkColor();
				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			}
			return (LRESULT)m_hBkBrush;
		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if( !m_bInit ) return 0;
		if( m_pOwner == NULL ) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if( pstr == NULL ) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen);
		m_pOwner->m_sText = pstr;
		m_pOwner->GetManager()->SendNotify(m_pOwner, _T("textchanged"));
		return 0;
	}

	void CEditWnd::SetSpeicalKey( bool bSpeical )
	{
		m_bSpeical = bSpeical;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CEditUI::CEditUI() : m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
		m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_uButtonState(0), 
		m_dwEditbkColor(0xFFFFFFFF), m_iWindowStyls(0),
		m_dwTextBackgroundColor(0xffcccccc)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);
		m_bIsPopMenu = false;
		m_bBossKey = false;
		m_nCtrlKeyDown = 0;
		m_nShiftKeyDown = 0;
		m_nAltKeyDown = 0;
		m_nKey = 0;

		m_nOldCtrlKeyDown = 0;
		m_nOldShiftKeyDown = 0;
		m_nOldAltKeyDown = 0;
		m_nOldKey = 0;
		m_szSelText = _T("");
		m_bDealKeyBySelf=false;
	}

	void CEditUI::SetDealKeyBySelf()
	{
		m_bDealKeyBySelf=true;
	}

	LPCTSTR CEditUI::GetClass() const
	{
		return _T("EditUI");
	}

	LPVOID CEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Edit")) == 0 ) return static_cast<CEditUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CEditUI::SetEditPopUpMenu()
	{
		m_bIsPopMenu = true;
	}

	void CEditUI::SetBossKeyState(bool bBosskeyed, bool bSpecial)
	{
		m_bBossKey = bBosskeyed;
		m_bSpeicalKey = bSpecial;
	}

	bool CEditUI::GetBossKeyState()
	{
		return m_bBossKey;
	}

	void CEditUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}
		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindow != NULL ) return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if( m_pWindow ) return;
			m_pWindow = new CEditWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			m_pWindow->SetSpeicalKey(m_bSpeicalKey);
			if(m_bDealKeyBySelf)
			{
				m_pWindow->SetDealKeySelf();
			}
			if (m_bIsPopMenu)
			{
				m_pWindow->SetEditPopUpMenu();
			}
			if (m_bIsAddrEdit)
			{
				m_pWindow->SetAddrEdit();
			}
			if (m_bBossKey)
			{
				m_pWindow->SetEditBossKey();
				if (GetText().IsEmpty())
				{
					if ( m_bSpeicalKey )
					{
						if (m_nOldCtrlKeyDown > 0)
						{
							m_pWindow->SetCtrlKeyDown();
						}
						if (m_nOldShiftKeyDown > 0)
						{
							m_pWindow->SetShiftKeyDown();
						}
						if (m_nOldAltKeyDown > 0)
						{
							m_pWindow->SetAltKeyDown();
						}
					}
					if (m_nOldKey > 0)
					{
						m_pWindow->SetKey(m_nOldKey);
						m_pWindow->SetBossKeyText();
					}
				}
			}
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL )
				{
					m_pWindow = new CEditWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);
					m_pWindow->SetSpeicalKey(m_bSpeicalKey);
					if(m_bDealKeyBySelf)
					{
						m_pWindow->SetDealKeySelf();
					}
					if (m_bIsPopMenu)
					{
						m_pWindow->SetEditPopUpMenu();
					}
					if (m_bIsAddrEdit)
					{
						m_pWindow->SetAddrEdit();
					}
					if (m_bBossKey)
					{
						m_pWindow->SetEditBossKey();
						if (GetText().IsEmpty())
						{
							if ( m_bSpeicalKey )
							{
								if (m_nOldCtrlKeyDown > 0)
								{
									m_pWindow->SetCtrlKeyDown();
								}
								if (m_nOldShiftKeyDown > 0)
								{
									m_pWindow->SetShiftKeyDown();
								}
								if (m_nOldAltKeyDown > 0)
								{
									m_pWindow->SetAltKeyDown();
								}
							}
							if (m_nOldKey > 0)
							{
								m_pWindow->SetKey(m_nOldKey);
								m_pWindow->SetBossKeyText();
							}
						}
					}
					if( PtInRect(&m_rcItem, event.ptMouse) )
					{
						int nSize = GetWindowTextLength(*m_pWindow);
						if( nSize == 0 )
							nSize = 1;

						Edit_SetSel(*m_pWindow, 0, nSize);
					}
				}
				else if( m_pWindow != NULL )
				{
#if 1
					int nSize = GetWindowTextLength(*m_pWindow);
					if( nSize == 0 )
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
#else
					POINT pt = event.ptMouse;
					pt.x -= m_rcItem.left + m_rcTextPadding.left;
					pt.y -= m_rcItem.top + m_rcTextPadding.top;
					::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		CLabelUI::DoEvent(event);
	}

	void CEditUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
		Invalidate();
	}

	void CEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
	}

	UINT CEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CEditUI::SetReadOnly(bool bReadOnly)
	{
		if( m_bReadOnly == bReadOnly ) return;

		m_bReadOnly = bReadOnly;
		if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
		Invalidate();
	}

	bool CEditUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CEditUI::SetNumberOnly(bool bNumberOnly)
	{
		if( bNumberOnly )
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls |= ~ES_NUMBER;
		}
	}

	bool CEditUI::IsNumberOnly() const
	{
		return m_iWindowStyls&ES_NUMBER ? true:false;
	}

	int CEditUI::GetWindowStyls() const 
	{
		return m_iWindowStyls;
	}

	void CEditUI::SetPasswordMode(bool bPasswordMode)
	{
		if( m_bPasswordMode == bPasswordMode ) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
	}

	bool CEditUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	void CEditUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if( m_cPasswordChar == cPasswordChar ) return;
		m_cPasswordChar = cPasswordChar;
		if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
		Invalidate();
	}

	TCHAR CEditUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	LPCTSTR CEditUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CEditUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CEditUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CEditUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CEditUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CEditUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CEditUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CEditUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	void CEditUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	DWORD CEditUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}
	
	CDuiString CEditUI::GetSelText()
	{
		return m_szSelText;
	}
	void CEditUI::SetSelText(LPCTSTR pStrSel)
	{
		m_szSelText = pStrSel;
		return ;
	}
	void CEditUI::SetSel(long nStartChar, long nEndChar)
	{
		if( m_pWindow != NULL ) Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
	}

	void CEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if( m_pWindow != NULL ) Edit_ReplaceSel(*m_pWindow, lpszReplace);
	}

	void CEditUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CEditUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CEditUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("numberonly")) == 0 ) SetNumberOnly(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("maxchar")) == 0 ) SetMaxChar(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("textbackground")) == 0 ) SetTextBackground(pstrValue);
		else if( _tcscmp(pstrName, _T("textbackgroundcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextBackgroundColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CEditUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
				else return;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
			else return;
		}
	}

	void CEditUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() && m_sTextBackground.IsEmpty() ) return;

		CDuiString sText = m_sText;
		if( m_bPasswordMode ) {
			sText.Empty();
			LPCTSTR p = m_sText.GetData();
			while( *p != _T('\0') ) {
				sText += m_cPasswordChar;
				p = ::CharNext(p);
			}
		}

		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		if ( !IsFocused() && m_sText.IsEmpty() && !m_sTextBackground.IsEmpty() )
		{
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sTextBackground, m_dwTextBackgroundColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else if( IsEnabled() ) {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);

		}
	}

	void CEditUI::SetCtrlKeyDown()
	{
		m_nCtrlKeyDown = VK_CONTROL;
	}
	void CEditUI::SetShiftKeyDown()
	{
		m_nShiftKeyDown = VK_SHIFT;
	}
	void CEditUI::SetAltKeyDown()
	{
		m_nAltKeyDown = VK_MENU;
	}
	void CEditUI::SetKey(UINT nKey)
	{
		m_nKey = nKey;
	}
	void CEditUI::ClearKeyDown()
	{
		m_nCtrlKeyDown = 0;
		m_nShiftKeyDown = 0;
		m_nAltKeyDown = 0;
		m_nKey = 0;
	}

	void CEditUI::SetOldCtrlKeyDown()
	{
		m_nOldCtrlKeyDown = VK_CONTROL;
	}
	void CEditUI::SetOldShiftKeyDown()
	{
		m_nOldShiftKeyDown = VK_SHIFT;
	}
	void CEditUI::SetOldAltKeyDown()
	{
		m_nOldAltKeyDown = VK_MENU;
	}
	void CEditUI::SetOldKey(UINT nKey)
	{
		m_nOldKey = nKey;
	}

	UINT CEditUI::GetOldCtrlKeyDown()
	{
		return m_nOldCtrlKeyDown;
	}
	UINT CEditUI::GetOldShiftKeyDown()
	{
		return m_nOldShiftKeyDown;
	}
	UINT CEditUI::GetOldAltKeyDown()
	{
		return m_nOldAltKeyDown;
	}
	UINT CEditUI::GetOldKey()
	{
		return m_nOldKey;
	}
	CDuiString CEditUI::GetKeyASCIIToText(UINT nChar)
	{
		CDuiString szKey = _T("");
		if (nChar>='A'&&nChar<='Z')
		{
			TCHAR   lpszFile[2];
			lpszFile[0]=nChar;
			lpszFile[1]='\0';
			szKey.Format(_T("%s"),lpszFile);
		}
		else if (nChar>='0'&&nChar<='9')
		{
			TCHAR   lpszFile[2];
			lpszFile[0]=nChar;
			lpszFile[1]='\0';
			szKey.Format(_T("%s"),lpszFile);
		}
		else
		{
			switch(nChar)
			{
			case 32:
				szKey = L"Space";
				break;
			case 27:
				szKey = L"Esc";
				break;
			case 192:
				szKey = L"~";
				break;
			case 112:
				szKey = L"F1";
				break;
			case 113:
				szKey = L"F2";
				break;
			case 114:
				szKey = L"F3";
				break;
			case 115:
				szKey = L"F4";
				break;
			case 116:
				szKey = L"F5";
				break;
			case 117:
				szKey = L"F6";
				break;
			case 118:
				szKey = L"F7";
				break;
			case 119:
				szKey = L"F8";
				break;
			case 120:
				szKey = L"F9";
				break;
			case 121:
				szKey = L"F10";
				break;
			case 122:
				szKey = L"F11";
				break;
			case 123:
				szKey = L"F12";
				break;
			case 189:
				szKey = L"-";
				break;
			case 187:
				szKey = L"=";
				break;
			case 8:
				szKey = L"Backspace";
				break;
			case 219:
				szKey = L"[";
				break;
			case 13:
				szKey = L"Enter";
				break;
			case 220:
				szKey = L"\\";
				break;
			case 221:
				szKey = L"]";
				break;
			case 222:
				szKey = L"'";
				break;
			case 186:
				szKey = L";";
				break;
			case 188:
				szKey = L",";
				break;
			case 190:
				szKey = L".";
				break;
			case 191:
				szKey = L"/";
				break;
			case 37:
				szKey = L"←";
				break;
			case 38:
				szKey = L"↑";
				break;
			case 39:
				szKey = L"→";
				break;
			case 40:
				szKey = L"↓";
				break;
			case 45:
				szKey = L"Insert";
				break;
			case 46:
				szKey = L"Delete";
				break;
			case 33:
				szKey = L"Page Up";
				break;
			case 34:
				szKey = L"Page Down";
				break;
			case 35:
				szKey = L"End";
				break;
			case 36:
				szKey = L"Home";
				break;
			case 96:
				szKey = L"Num0";
				break;
			case 97:
				szKey = L"Num1";
				break;
			case 98:
				szKey = L"Num2";
				break;
			case 99:
				szKey = L"Num3";
				break;
			case 100:
				szKey = L"Num4";
				break;
			case 101:
				szKey = L"Num5";
				break;
			case 102:
				szKey = L"Num6";
				break;
			case 103:
				szKey = L"Num7";
				break;
			case 104:
				szKey = L"Num8";
				break;
			case 105:
				szKey = L"Num9";
				break;
			case 106:
				szKey = L"Num*";
				break;
			case 107:
				szKey = L"Num+";
				break;
			case 109:
				szKey = L"Num-";
				break;
			case 111:
				szKey = L"Num/";
				break;
			}
		}
		return szKey;
	}

	void CEditUI::SetBossKeyText()
	{
		if (m_nOldKey < 1)
		{
			SetText(_T(""));
			//出错将全部加载默认的配置函数
			return ;
		}
		CDuiString szText = _T("");
		CDuiString szKey = _T("");
		szKey = GetKeyASCIIToText(m_nOldKey);
		if ( m_bSpeicalKey )
		{
			if (m_nOldCtrlKeyDown)
			{
				szText = L"Ctrl + ";
			}
			if (m_nOldShiftKeyDown)
			{
				szText += L"Shift + ";
			}
			if (m_nOldAltKeyDown)
			{
				szText += L"Alt + ";
			}
		}
		if (!szKey.IsEmpty())
		{
			szText += szKey;
			//这个有了之后 检查所有快捷键 是否有冲突
			SetText(szText.GetData());
			return ;
		}
		SetText(_T(""));
	}

	bool CEditUI::CheckKeyRepeat(UINT nCtrlKey,UINT nShiftKey,UINT nAltKey,UINT nKey)
	{
		if (nCtrlKey == m_nOldCtrlKeyDown && nShiftKey == m_nOldShiftKeyDown && nAltKey == m_nOldAltKeyDown && nKey == m_nOldKey)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	UINT CEditUI::GetCtrlKeyDown()
	{
		return m_nCtrlKeyDown;
	}
	UINT CEditUI::GetShiftKeyDown()
	{
		return m_nShiftKeyDown;
	}	
	UINT CEditUI::GetAltKeyDown()
	{
		return m_nAltKeyDown;
	}
	UINT CEditUI::GetKey()
	{
		return m_nKey;
	}
	void CEditUI::SetShortcutKey()
	{
		m_nOldAltKeyDown = m_nCtrlKeyDown;
		m_nOldShiftKeyDown = m_nShiftKeyDown;
		m_nOldAltKeyDown = m_nAltKeyDown;
		m_nOldKey = m_nKey;
	}

	void CEditUI::SetAddrEdit()
	{
		m_bIsAddrEdit = true;
	}

	void CEditUI::SetTextBackground( LPCTSTR pstrText )
	{
		m_sTextBackground = pstrText;
	}

	void CEditUI::SetTextBackgroundColor( DWORD dwColor )
	{
		m_dwTextBackgroundColor = dwColor;
	}

}
