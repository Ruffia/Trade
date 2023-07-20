// XTPEdit.cpp : implementation of the CXTPEdit class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPRegExp.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Defines.h"
#include "Controls/Resource.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/Util/XTPFunctions.h"

#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2000.h"
#include "Controls/Edit/Themes/XTPEditThemeOfficeXP.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2003.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2013.h"
#include "Controls/Edit/Themes/XTPEditThemeFlat.h"
#include "Controls/Edit/Themes/XTPEditThemeUltraFlat.h"
#include "Controls/Edit/Themes/XTPEditThemeResource.h"
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2010.h"
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2012.h"
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2015.h"
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2017.h"
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2019.h"
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2022.h"
#include "Controls/Edit/Themes/XTPEditThemeNativeWindows10.h"
#include "Controls/Edit/XTPEdit.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4097)

/////////////////////////////////////////////////////////////////////////////
// CXTPMaskEdit
/////////////////////////////////////////////////////////////////////////////

CXTPEdit::CXTPEdit()
	: m_pTheme(NULL)
{
	m_bPreSubclassInit = TRUE;
	m_bHighlighted	 = FALSE;
	m_bFocused		   = FALSE;
	m_bShowBorder	  = TRUE;
	m_pRegExp		   = NULL;

	SetTheme(xtpControlThemeDefault);
}

CXTPEdit::~CXTPEdit()
{
	CMDTARGET_RELEASE(m_pTheme);
	SAFE_DELETE(m_pRegExp);
}

IMPLEMENT_DYNAMIC(CXTPEdit, CEdit)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPEdit, CEdit)
	ON_MASKEDIT_REFLECT()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)

	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)

	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)

	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)

	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)

	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)

	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)

	ON_WM_CTLCOLOR_REFLECT()

	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPEdit::SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault /*= NULL*/)
{
	return TBase::SetEditMask(lpszMask, lpszLiteral, lpszDefault);
}

void CXTPEdit::Init()
{
	RefreshMetrics();

	if (m_bShowBorder)
	{
		m_bShowBorder = (0 != (GetExStyle() & WS_EX_CLIENTEDGE));
	}

	if (m_bShowBorder)
	{
		ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);
		ModifyStyle(WS_BORDER, 0, SWP_FRAMECHANGED);
		PostMessage(WM_NCPAINT);
	}
}

void CXTPEdit::PreSubclassWindow()
{
	TBase::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (TBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!TBase::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPEdit::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		// keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(XTP_DPI_X(5), XTP_DPI_Y(5));
	}

	SetFocus();

	CMenu menu;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadMenu(&menu, XTP_IDM_POPUP));

	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	CXTPImageManager* pImageManager = new CXTPImageManager();
	if (pImageManager)
	{
		CString type	= _T("RT_XAML");
		HMODULE hModule = XTPResourceManager()->GetResourceHandle();

		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_COPY, ID_EDIT_COPY,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_CUT, ID_EDIT_CUT,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_PASTE, ID_EDIT_PASTE,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_UNDO, ID_EDIT_UNDO,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_CANCEL, ID_EDIT_CLEAR,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
	}

	XTPContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner,
				   pImageManager, FALSE);
}

void CXTPEdit::OnEditUndo()
{
	MaskUndo();
}

void CXTPEdit::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanUndo());
}

void CXTPEdit::OnEditCut()
{
	MaskCut();
}

void CXTPEdit::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(HasSelection() && ((GetStyle() & ES_READONLY) == 0));
}

void CXTPEdit::OnEditCopy()
{
	MaskCopy();
}

void CXTPEdit::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(HasSelection());
}

void CXTPEdit::OnEditPaste()
{
	MaskPaste();
}

void CXTPEdit::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanPaste());
}

void CXTPEdit::OnEditClear()
{
	MaskClear();
}

void CXTPEdit::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(HasSelection() && ((GetStyle() & ES_READONLY) == 0));
}

void CXTPEdit::OnEditSelectAll()
{
	SetSel(0, -1);
}

void CXTPEdit::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetWindowTextLength() > 0);
}

BOOL CXTPEdit::CanPaste()
{
	return IsWindowEnabled()
		   && ((GetStyle() & ES_READONLY) == 0) & ::IsClipboardFormatAvailable(CF_TEXT);
}

BOOL CXTPEdit::HasSelection() const
{
	int nStartChar, nEndChar;
	GetSel(nStartChar, nEndChar);
	return (nStartChar < nEndChar);
}

BOOL CXTPEdit::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

BOOL CXTPEdit::IsPatternValid()
{
	if (!m_pRegExp)
		return TRUE;

	CString strWindowText;
	GetWindowText(strWindowText);

	int nCount = m_pRegExp->Execute(strWindowText);
	if (nCount != 1)
		return FALSE;

	long nStart, nLength;
	m_pRegExp->GetMatch(0, &nStart, &nLength);

	return nStart == 0 && nLength == strWindowText.GetLength();
}

void CXTPEdit::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	// Get the client rect.
	CXTPClientRect rectClient(this);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dcPaint);
	::FillRect(memDC, rectClient, GetClientBrush(&memDC));

	// Now let the window do its default painting...
	CEdit::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	if (!IsPatternValid())
	{
		CString strText;
		GetWindowText(strText);

		CRect rect;
		const int iIncrement = XTP_DPI_X(2);

		// Get the rect for the entire edit control
		((CEdit*)this)->GetRect(&rect);

		// Check that wavy line will not over-write edit control border...
		if (rectClient.bottom - rect.bottom >= iIncrement + XTP_DPI_X(1))
		{
			CSize size;
			int iX = 0;

			rect.bottom -= iIncrement - XTP_DPI_X(2);

			// We must use the correct font when using GetTextExtent..!
			CXTPFontDC font(&memDC, CWnd::GetFont());
			CXTPPenDC pen(memDC, 0xFF);

			BOOL bLayoutRLT = (0 != (GetExStyle() & WS_EX_RIGHT));

			if (bLayoutRLT)
			{
				size = memDC.GetTextExtent(strText);

				// Check for case of empty string
				if (size.cx == 0)
					size.cx = iIncrement * 3;
				// Dont draw off the end of the edit control
				else if (size.cx > rect.Width())
					size.cx = rect.Width();

				int iOffset = 0;

				if (GetStyle() & ES_CENTER)
				{
					iOffset = (rect.Width() - size.cx) / 2;
				}

				rect.right -= iOffset;
				// Starting x co-ordinate to start drawing from
				iX = rect.right;
				// Start at the bottom left of the edit control
				memDC.MoveTo(rect.right, rect.bottom);

				// Draw the wavy line like in Microsoft Word
				while (iX > rect.right - size.cx)
				{
					iX -= iIncrement;

					if (iX < rect.right - size.cx)
						break;

					memDC.LineTo(iX, rect.bottom + iIncrement);
					iX -= iIncrement;

					if (iX < rect.right - size.cx)
						break;

					memDC.LineTo(iX, rect.bottom);
				}
			}
			else
			{
				// This part deals with the *displayed* Text Extent
				size = memDC.GetTextExtent(strText.Mid(
					((CEdit*)this)->CharFromPos(CPoint(rect.left + XTP_DPI_X(1), rect.top))));

				// Check for case of empty string
				if (size.cx == 0)
					size.cx = iIncrement * 3;
				// Dont draw off the end of the edit control
				else if (size.cx > rect.right)
					size.cx = rect.right;

				int iOffset = 0;

				if (GetStyle() & ES_CENTER)
				{
					iOffset = (rect.Width() - size.cx) / 2;
				}
				else if (GetStyle() & ES_RIGHT)
				{
					iOffset = rect.Width() - size.cx;
				}

				rect.left += iOffset;
				// Starting x co-ordinate to start drawing from
				iX = rect.left;
				// Start at the bottom left of the edit control
				memDC.MoveTo(rect.left, rect.bottom);

				// Draw the wavy line like in Microsoft Word
				while (iX < size.cx + rect.left)
				{
					iX += iIncrement;

					if (iX > size.cx + rect.left)
						break;

					memDC.LineTo(iX, rect.bottom + iIncrement);
					iX += iIncrement;

					if (iX > size.cx + rect.left)
						break;

					memDC.LineTo(iX, rect.bottom);
				}
			}
		}
		else
		{
			TRACE(_T("Edit Control too small to draw wavy line!\r\n"));
		}
	}
}

void CXTPEdit::OnInitMenuPopup(CMenu* pPopupMenu, UINT /*nIndex*/, BOOL bSysMenu)
{
	Default();

	if (!bSysMenu)
	{
		ASSERT(pPopupMenu != NULL);
		if (!pPopupMenu)
			return;

		// check the enabled state of various menu items
		CCmdUI state;
		state.m_pMenu = pPopupMenu;
		ASSERT(state.m_pOther == NULL);

		state.m_nIndexMax = XTPToUIntChecked(pPopupMenu->GetMenuItemCount());
		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
		{
			state.m_nID = pPopupMenu->GetMenuItemID(XTPToIntChecked(state.m_nIndex));
			if (state.m_nID == 0)
				continue; // menu separator or invalid cmd - ignore it

			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pMenu != NULL);
			if (state.m_nID == (UINT)-1)
			{
				// possibly a popup menu, route to first item of that popup
				state.m_pSubMenu = pPopupMenu->GetSubMenu(XTPToIntChecked(state.m_nIndex));
				if (state.m_pSubMenu == NULL)
					continue;

				state.m_nID = state.m_pSubMenu->GetMenuItemID(0);
				if (state.m_nID == 0 || state.m_nID == (UINT)-1)
					continue; // first item of popup can't be routed to

				state.DoUpdate(this, FALSE); // popups are never auto disabled
			}
			else
			{
				// normal menu item
				// Auto enable/disable if command is _not_ a system command
				state.m_pSubMenu = NULL;
				state.DoUpdate(this, state.m_nID < 0xF000);
			}
		}
	}
}

bool CXTPEdit::Initialize(CWnd* /*pParentWnd*/)
{
	// edit control must be valid in order to initialize.
	ASSERT(this);
	if (!::IsWindow(m_hWnd))
		return false;

	// set the font and parent for the browse edit.
	SetFont(&XTPAuxData().xtpFont);

	return true;
}

XTPControlTheme CXTPEdit::GetThemeID()
{
	if (m_pTheme)
	{
		return m_pTheme->GetThemeID();
	}

	return xtpControlThemeDefault;
}

CXTPEditTheme* CXTPEdit::GetTheme()
{
	return m_pTheme;
}

LRESULT CXTPEdit::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

void CXTPEdit::SetTheme(XTPControlTheme nTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	switch (nTheme)
	{
		case xtpControlThemeResource: m_pTheme = new CXTPEditThemeResource(); break;

		case xtpControlThemeOffice2000: m_pTheme = new CXTPEditThemeOffice2000(); break;

		case xtpControlThemeOfficeXP: m_pTheme = new CXTPEditThemeOfficeXP(); break;

		case xtpControlThemeFlat: m_pTheme = new CXTPEditThemeFlat(); break;

		case xtpControlThemeUltraFlat: m_pTheme = new CXTPEditThemeUltraFlat(); break;

		case xtpControlThemeOffice2013: m_pTheme = new CXTPEditThemeOffice2013(); break;

		case xtpControlThemeOffice2003: m_pTheme = new CXTPEditThemeOffice2003(); break;

		case xtpControlThemeVisualStudio2010: m_pTheme = new CXTPEditThemeVisualStudio2010(); break;

		case xtpControlThemeVisualStudio2012:
		case xtpControlThemeVisualStudio2012Light:
		case xtpControlThemeVisualStudio2012Dark:
			m_pTheme = new CXTPEditThemeVisualStudio2012(nTheme);
			break;

		case xtpControlThemeVisualStudio2015: m_pTheme = new CXTPEditThemeVisualStudio2015(); break;

		case xtpControlThemeVisualStudio2017: m_pTheme = new CXTPEditThemeVisualStudio2017(); break;

		case xtpControlThemeVisualStudio2019: m_pTheme = new CXTPEditThemeVisualStudio2019(); break;

		case xtpControlThemeVisualStudio2022: m_pTheme = new CXTPEditThemeVisualStudio2022(); break;

		case xtpControlThemeNativeWindows10: m_pTheme = new CXTPEditThemeNativeWindows10(); break;

		case xtpControlThemeDefault:
		case xtpControlThemeNativeWinXP:
		case xtpControlThemeVisualStudio2005:
		case xtpControlThemeVisualStudio2008:
		default: m_pTheme = new CXTPEditThemeDefault(); break;
	}

	m_pTheme->RefreshMetrics(this);

	RedrawEdit();
}

void CXTPEdit::RefreshMetrics()
{
	if (m_pTheme)
	{
		m_pTheme->RefreshMetrics(this);
	}
}

void CXTPEdit::RedrawEdit()
{
	if (::IsWindow(m_hWnd))
	{
		SendMessage(WM_NCPAINT);
		RedrawWindow();
	}
}

void CXTPEdit::OnNcPaint()
{
	TBase::OnNcPaint();

	if (m_bShowBorder && m_pTheme)
	{
		CWindowDC dc(this);

		CRect rcWnd;
		GetWindowRect(&rcWnd);
		rcWnd.OffsetRect(-rcWnd.TopLeft());

		m_pTheme->DrawNcBorders(&dc, rcWnd, this);
		m_pTheme->DrawNcScrollBack(&dc, rcWnd, this);
	}
}

void CXTPEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	TBase::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (!m_bShowBorder)
		return;

	lpncsp[0].rgrc->top += 2;
	lpncsp[0].rgrc->left += 2;
	lpncsp[0].rgrc->right -= 2;
	lpncsp[0].rgrc->bottom -= 2;
}

void CXTPEdit::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	BOOL bHot = rc.PtInRect(point);
	if (IsFlatStyle() && bHot != m_bHighlighted)
	{
		m_bHighlighted = bHot;
		RedrawFocusedFrame();

		if (bHot)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT };
			_TrackMouseEvent(&tme);
		}
	}

	TBase::OnMouseMove(nFlags, point);
}

void CXTPEdit::OnKillFocus(CWnd* pNewWnd)
{
	TBase::OnKillFocus(pNewWnd);

	m_bFocused = FALSE;
	RedrawFocusedFrame();
}

void CXTPEdit::OnSetFocus(CWnd* pOldWnd)
{
	TBase::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	RedrawFocusedFrame();
}

void CXTPEdit::RedrawFocusedFrame()
{
	if (m_pTheme && !m_pTheme->RedrawFocusedFrame() && !IsFlatStyle())
	{
		return;
	}

	if (!m_bShowBorder)
		return;

	RedrawEdit();
}

HBRUSH CXTPEdit::GetClientBrush(CDC* pDC)
{
	if (m_pTheme)
	{
		return *m_pTheme->GetClientBrush(pDC, this);
	}

	BOOL bEnabled = IsWindowEnabled() && ((GetStyle() & ES_READONLY) == 0);

	HBRUSH hBrush = (HBRUSH)::SendMessage(::GetParent(m_hWnd),
										  UINT(!bEnabled ? WM_CTLCOLORSTATIC : WM_CTLCOLOREDIT),
										  (WPARAM)pDC->GetSafeHdc(), (LPARAM)m_hWnd);

	if (hBrush)
		return hBrush;

	return GetSysColorBrush(!bEnabled ? COLOR_3DFACE : COLOR_WINDOW);
}

void CXTPEdit::SetUseVisualStyle(BOOL bUseVisualStyle /* = TRUE*/)
{
	if (m_pTheme)
	{
		m_pTheme->UseWinTheme(bUseVisualStyle);

		RefreshMetrics();
		RedrawEdit();
	}
}

BOOL CXTPEdit::OnChange()
{
	if (m_pRegExp)
	{
		Invalidate(FALSE);
	}
	return FALSE;
}

void CXTPEdit::SetPattern(LPCTSTR lpszPattern)
{
	SAFE_DELETE(m_pRegExp);

	if (lpszPattern && lpszPattern[0] != 0)
	{
		m_pRegExp = new CXTPRegExp();
		m_pRegExp->SetPattern(lpszPattern);
	}

	if (m_hWnd)
	{
		Invalidate(FALSE);
	}
}

void CXTPEdit::AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignemnt)
{
	if (IsFlatStyle())
		return;

	if (!m_bShowBorder && ((GetStyle() & WS_BORDER) == 0))
		return;

	if (m_pTheme)
	{
		m_pTheme->AdjustBuddyRect(pBuddy, rcUpDown, nAlignemnt);
	}
}

void CXTPEdit::DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignemnt)
{
	if (m_pTheme)
	{
		m_pTheme->DrawBuddyBorders(pBuddy, pDC, rc, nAlignemnt, this);
	}
}

void CXTPEdit::OnSysColorChange()
{
	TBase::OnSysColorChange();

	RefreshMetrics();
	Invalidate(FALSE);

	SendMessage(WM_NCPAINT);
}

HBRUSH CXTPEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hBrush = NULL;

	if (m_pTheme)
	{
		CBrush* pBrush = m_pTheme->CtlColor(pDC, nCtlColor, this);

		if (pBrush)
		{
			hBrush = *pBrush;
		}
	}

	return hBrush;
}

BOOL CXTPEdit::IsFlatStyle() const
{
	return (m_pTheme && m_pTheme->IsFlatStyle());
}

void CXTPEdit::SetFlatStyle(BOOL bFlatStyle /* = TRUE*/)
{
	if (m_pTheme)
	{
		m_pTheme->SetFlatStyle(bFlatStyle);
		RedrawEdit();
	}
}

BOOL CXTPEdit::GetUseVisualStyle() const
{
	return m_pTheme ? m_pTheme->m_bUseWinTheme : FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPEditor

#pragma warning(push)
#pragma warning(disable : 4996)

IMPLEMENT_DYNAMIC(CXTPEditor_Deprecated, CXTPEdit)

void CXTPEditor_Deprecated::RefreshMetrics()
{
	CXTPEdit::RefreshMetrics();
}

#pragma warning(pop)
