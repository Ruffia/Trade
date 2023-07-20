// XTPControlEdit.cpp : implementation of the CXTPControlEdit class.
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

#include "StdAfx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPMouseManager.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef LRESULT(CALLBACK* SUBCLASSPROC)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
										UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
typedef BOOL(WINAPI* PFNSETWINDOWSUBCLASS)(HWND, SUBCLASSPROC, UINT_PTR, DWORD_PTR);
typedef BOOL(WINAPI* PFNREMOVEWINDOWSUBCLASS)(HWND, SUBCLASSPROC, UINT_PTR);
typedef LRESULT(WINAPI* PFNDEFSUBCLASSPROC)(HWND, UINT, WPARAM, LPARAM);

static PFNSETWINDOWSUBCLASS SetWindowSubclassPtr;
static PFNREMOVEWINDOWSUBCLASS RemoveWindowSubclassPtr;
static PFNDEFSUBCLASSPROC DefSubclassProcPtr;

CXTPCommandBarEditCtrl::CRichEditContext::CRichEditContext()
{
	WNDCLASS wc;
	int nVersion = xtpRichEditWindowClassDefault;
	if (XTPLoadWindowClass(xtpWindowClassRichEdit, xtpWindowClassVersionHighest, &nVersion, &wc))
	{
		m_hInstance	= wc.hInstance;
		m_strClassName = wc.lpszClassName;
		m_bRichEdit2   = (xtpRichEditWindowClassVersion20 <= nVersion);
	}
}

CXTPCommandBarEditCtrl::CRichEditContext::~CRichEditContext()
{
}

CXTPCommandBarEditCtrl::CRichEditContext& CXTPCommandBarEditCtrl::GetRichEditContext()
{
	return CXTPSingleton<CRichEditContext>::Instance();
}

// CXTPCommandBarEditCtrl

IMPLEMENT_DYNCREATE(CXTPCommandBarEditCtrl, CEdit)

CXTPCommandBarEditCtrl::CXTPCommandBarEditCtrl()
{
	m_bImeMode			  = FALSE;
	m_bComposited		  = FALSE;
	m_bIgonoreEditChanged = FALSE;
}

BOOL CXTPCommandBarEditCtrl::IsCommandEnabled(UINT nID)
{
	if (nID == ID_EDIT_PASTE)
	{
		return ((GetStyle() & (ES_READONLY | WS_DISABLED)) == 0)
			   && ::IsClipboardFormatAvailable(CF_TEXT);
	}
	else if (nID == ID_EDIT_CUT || nID == ID_EDIT_COPY)
	{
		int nStartChar, nEndChar;
		GetSel(nStartChar, nEndChar);
		return (nStartChar != nEndChar)
			   && ((nID == ID_EDIT_COPY) || ((GetStyle() & (ES_READONLY | WS_DISABLED)) == 0));
	}
	return TRUE;
}

BOOL CXTPCommandBarEditCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra,
									  AFX_CMDHANDLERINFO* /*pHandlerInfo*/)
{
	if ((nID == ID_EDIT_PASTE || nID == ID_EDIT_CUT || nID == ID_EDIT_COPY)
		&& (nCode == CN_UPDATE_COMMAND_UI))
	{
		CCmdUI* pCmdUI = (CCmdUI*)pExtra;
		pCmdUI->Enable(IsCommandEnabled(nID));
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCommandBarEditCtrl::OnCommand(WPARAM wParam, LPARAM)
{
	UINT nID = LOWORD(wParam);

	if (nID == ID_EDIT_PASTE)
		Paste();
	else if (nID == ID_EDIT_COPY)
		Copy();
	else if (nID == ID_EDIT_CUT)
		Cut();
	else
		return FALSE;

	return TRUE;
}

BOOL CXTPCommandBarEditCtrl::ShowContextMenu(CXTPControl* pControl, CPoint point)
{
	CString strPrompt;
	if (!strPrompt.LoadString(ID_EDIT_CUT))
		return FALSE;

	CXTPCommandBar* pParentBar = pControl->GetParent();

	CXTPCommandBars* pCommandBars = pParentBar->GetCommandBars();

	CXTPMouseManager* pMouseManager = pCommandBars ? pCommandBars->GetMouseManager()
												   : XTPMouseManager();
	pMouseManager->UnlockMouseMove();

	ClientToScreen(&point);

	MSG msg;
	if (PeekMessage(&msg, m_hWnd, EM_SETSEL, EM_SETSEL, PM_NOREMOVE))
	{
		SendMessage(EM_SETSEL, 0, -1);
	}

	CXTPPopupBar* pPopupBar  = CXTPPopupBar::CreatePopupBar(pCommandBars);
	CXTPControl* pControlCut = pPopupBar->GetControls()->Add(xtpControlButton, ID_EDIT_CUT);
	pControlCut->SetEnabled(IsCommandEnabled(ID_EDIT_CUT));
	CXTPControl* pControlCopy = pPopupBar->GetControls()->Add(xtpControlButton, ID_EDIT_COPY);
	pControlCopy->SetEnabled(IsCommandEnabled(ID_EDIT_COPY));
	CXTPControl* pControlPaste = pPopupBar->GetControls()->Add(xtpControlButton, ID_EDIT_PASTE);
	pControlPaste->SetEnabled(IsCommandEnabled(ID_EDIT_PASTE));

	pPopupBar->SetGrabFocus(FALSE);

	UINT nAlign  = UINT(pControl->GetParent()->GetExStyle() & (WS_EX_LAYOUTRTL | WS_EX_RIGHT)
							? TPM_RIGHTALIGN
							: TPM_LEFTALIGN);
	int nCommand = CXTPCommandBars::TrackPopupMenu(pPopupBar,
												   UINT(TPM_NONOTIFY | TPM_RECURSE | TPM_RETURNCMD
														| nAlign),
												   point.x, point.y, this, NULL, this);

	if (nCommand > 0)
	{
		OnCommand(XTPToWPARAM(nCommand), 0);
	}

	pMouseManager->LockMouseMove();

	pPopupBar->InternalRelease();

	if (pParentBar->GetSafeHwnd())
	{
		pParentBar->Redraw();
		pParentBar->UpdateWindow();
	}

	return TRUE;
}

BOOL CXTPCommandBarEditCtrl::CreateEdit(DWORD dwStyle, CWnd* pParentWnd)
{
	if (GetRichEditContext().m_hInstance)
	{
		if (!CWnd::Create(GetRichEditContext().m_strClassName, 0, dwStyle | ES_NOOLEDRAGDROP,
						  CRect(0, 0, 0, 0), pParentWnd, 0))
		{
			return FALSE;
		}

		SendMessage(EM_SETEVENTMASK, 0, ENM_CHANGE);
		SendMessage(EM_SETTEXTMODE, TM_PLAINTEXT | TM_SINGLELEVELUNDO);
	}
	else
	{
		if (!Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, 0))
			return FALSE;
	}

	SetDlgCtrlID(0); // Prevent Visual Studio 2010 to assign non zero ID.

	return TRUE;
}

BOOL CXTPCommandBarEditCtrl::IsDialogCode(UINT nChar, LPARAM lParam)
{
	const BOOL bNavigateKeys = (nChar == VK_BACK) || (nChar >= VK_PRIOR && nChar <= VK_DELETE)
							   || ((nChar == 'C' || nChar == 'V' || nChar == 'X' || nChar == 'Z')
								   && (GetKeyState(VK_CONTROL) < 0));

	if (!bNavigateKeys)
		return FALSE;

	MSG msg;
	msg.message = WM_KEYDOWN;
	msg.hwnd	= m_hWnd;
	msg.wParam  = (WPARAM)nChar;
	msg.lParam  = lParam;

	if (IsDialogMessage(&msg))
		return TRUE;

	return FALSE;
}

void CXTPCommandBarEditCtrl::SetWindowTextEx(LPCTSTR lpszString)
{
	m_bIgonoreEditChanged = TRUE;
	SetWindowText(0);
	m_bIgonoreEditChanged = FALSE;
	SetWindowText(lpszString);
}

BOOL CXTPCommandBarEditCtrl::IsImeMode() const
{
	return m_bImeMode;
}

void CXTPCommandBarEditCtrl::GetWindowTextEx(CString& rString)
{
#ifdef _UNICODE
	if (GetRichEditContext().m_bRichEdit2)
	{
		GETTEXTLENGTHEX gtl = { GTL_DEFAULT, 1200 };

		int nLen = (int)SendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&gtl);

		if (nLen > 0)
		{
			GETTEXTEX gt = { (nLen + 1) * sizeof(TCHAR), GT_DEFAULT, 1200, 0, 0 };

			SendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)rString.GetBufferSetLength(nLen));
			rString.ReleaseBuffer();
		}
		else
		{
			rString.Empty();
		}
	}
	else
#endif
	{
		GetWindowText(rString);
	}
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCommandBarEditCtrl, CEdit)
	ON_MESSAGE_VOID(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_MESSAGE_VOID(WM_IME_ENDCOMPOSITION, OnImeEndComposition)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPCommandBarEditCtrl::OnImeStartComposition()
{
	Default();
	m_bImeMode = TRUE;
}

void CXTPCommandBarEditCtrl::OnImeEndComposition()
{
	Default();
	m_bImeMode = FALSE;
}

void CXTPCommandBarEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	m_bImeMode = FALSE;
}

void CXTPCommandBarEditCtrl::OnPaint()
{
	Default();

	if (m_bComposited)
	{
		CClientDC dc(this);
		CXTPPaintManager::FillCompositeAlpha(&dc, CXTPClientRect(this));
	}
}

// CXTPControlEdit

IMPLEMENT_XTP_CONTROL(CXTPControlEdit, CXTPControl)

CXTPControlEdit::CXTPControlEdit()
{
	EnableAutomation();

	m_controlType	  = xtpControlEdit;
	m_nWidth		   = XTP_DPI_X(100);
	m_nLabelWidth	  = 0;
	m_bLabel		   = FALSE;
	m_strEditText	  = _T("");
	m_pEdit			   = NULL;
	m_bReadOnly		   = FALSE;
	m_bDelayReposition = FALSE;
	m_bDelayDestroy	= FALSE;
	m_bFocused		   = FALSE;

	m_pAutoCompleteWnd		   = NULL;
	m_dwShellAutoCompleteFlags = 0;
	m_bEditChanged			   = TRUE;
	m_dwEditStyle			   = 0;

	m_nEditIconId = 0;
	m_nTextLimit  = 0;

	m_bShowSpinButtons = FALSE;
	m_nBuddyButtonId   = 0;

	m_bNumericOnly = FALSE;

	static CXTPModuleHandle modComCtl32Dll;
	if (modComCtl32Dll.LoadLibrary(_T("ComCtl32.dll")))
	{
		if (!SetWindowSubclassPtr)
			modComCtl32Dll.GetProcAddress((FARPROC*)&SetWindowSubclassPtr, "SetWindowSubclass",
										  MAKELONG(0, 6));
		if (!RemoveWindowSubclassPtr)
			modComCtl32Dll.GetProcAddress((FARPROC*)&RemoveWindowSubclassPtr,
										  "RemoveWindowSubclass", MAKELONG(0, 6));
		if (!DefSubclassProcPtr)
			modComCtl32Dll.GetProcAddress((FARPROC*)&DefSubclassProcPtr, "DefSubclassProc",
										  MAKELONG(0, 6));
	}
}

CXTPControlEdit::~CXTPControlEdit()
{
	if (m_pEdit)
	{
		m_pEdit->m_pControl = NULL;
		delete m_pEdit;
	}
	SAFE_DELETE(m_pAutoCompleteWnd);
}

void CXTPControlEdit::OnEditChanged()
{
	NotifySite(EN_CHANGE);
}

CString CXTPControlEdit::GetEditText() const
{
	CString strEditText = _GetEditText();

	return strEditText == GetEditHint() ? CString() : strEditText;
}

CString CXTPControlEdit::_GetEditText() const
{
	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_bEditChanged)
	{
		m_pEdit->GetWindowTextEx(m_strEditText);
		m_bEditChanged = FALSE;
	}
	return m_strEditText;
}

void CXTPControlEdit::SetEditText(const CString& strText)
{
	_SetEditText(strText.IsEmpty() ? GetEditHint() : strText);
}

void CXTPControlEdit::_SetEditText(const CString& strText)
{
	if (m_pEdit && m_pEdit->GetSafeHwnd() && _GetEditText() != strText)
	{
		m_pEdit->SetWindowTextEx(strText);
	}
	m_strEditText  = strText;
	m_bEditChanged = FALSE;
}

BOOL CXTPControlEdit::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	ASSERT(IsFocused());

	if (!IsFocused() || !m_pEdit->GetSafeHwnd())
		return FALSE;

	if (m_pEdit && m_pEdit->m_bImeMode)
		return FALSE_EXIT;

	if (nChar == VK_RETURN || nChar == VK_TAB)
		return FALSE;

	if (nChar == VK_ESCAPE)
	{
		SetEditText(m_strLastText);
		SetFocused(FALSE);

		return GetParent()->GetType() == xtpBarTypePopup;
	}

	if (m_pEdit && m_pEdit->GetSafeHwnd() && GetKeyState(VK_MENU) < 0
		&& GetKeyState(VK_CONTROL) >= 0)
	{
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		if (pCommandBars && pCommandBars->OnFrameAccel(nChar))
			return TRUE;
	}

	if (GetEnabled() && IsSpinButtonsVisible() && (nChar == VK_UP || nChar == VK_DOWN)
		&& !GetReadOnly())
	{
		NotifySpinChanged(1, nChar == VK_UP ? +1 : -1);
		if (m_pEdit && ::GetFocus() == m_pEdit->GetSafeHwnd())
			m_pEdit->SetSel(0, -1);
		return TRUE;
	}

	if (m_pAutoCompleteWnd && m_pAutoCompleteWnd->IsDialogCode(nChar, lParam))
		return TRUE;

	if (m_pEdit->IsDialogCode(nChar, lParam))
		return TRUE;

	return FALSE_EXIT;
}

CXTPControlEditCtrl* CXTPControlEdit::CreateEditControl()
{
	return new CXTPControlEditCtrl();
}

void CXTPControlEdit::EnableShellAutoComplete(DWORD dwFlags /*= SHACF_FILESYSTEM | SHACF_URLALL*/)
{
	m_dwShellAutoCompleteFlags = dwFlags;
	SAFE_DELETE(m_pAutoCompleteWnd);

	if (m_pEdit->GetSafeHwnd() && dwFlags)
	{
		m_pAutoCompleteWnd = new CXTPControlComboBoxAutoCompleteWnd();

		if (FAILED(m_pAutoCompleteWnd->ShellAutoComplete(m_pEdit->GetSafeHwnd(),
														 m_dwShellAutoCompleteFlags)))
		{
			SAFE_DELETE(m_pAutoCompleteWnd);
		}
	}
}

BOOL CXTPControlEdit::HasFocus() const
{
	return IsFocused();
}

BOOL CXTPControlEdit::IsFocusable() const
{
	return m_pEdit && m_pEdit->GetSafeHwnd();
}

void CXTPControlEdit::SetRect(CRect rcControl)
{
	CString strEditText = GetEditText();

	if (!m_pEdit)
	{
		m_pEdit				= CreateEditControl();
		m_pEdit->m_pControl = this;
	}

	if (m_bDelayDestroy && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
		m_bDelayDestroy = FALSE;
	}
	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetExStyle() & WS_EX_LAYOUTRTL)
	{
		m_pEdit->DestroyWindow();
	}

	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetParent() != m_pParent)
	{
		m_pEdit->DestroyWindow();
	}

	if (m_rcControl == rcControl && !(m_pEdit && !m_pEdit->GetSafeHwnd())
		&& (m_bDelayReposition == FALSE))
	{
		if (!(m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetParent() != m_pParent))
			return;
	}

	m_rcControl = rcControl;

	if (m_pEdit)
	{
		if (!m_pEdit->GetSafeHwnd())
		{
			m_pEdit->CreateEdit(WS_CHILD | ES_AUTOHSCROLL | m_dwEditStyle, m_pParent);

			if (!GetEnabled())
				m_pEdit->EnableWindow(FALSE);

			if (m_dwShellAutoCompleteFlags != 0)
				EnableShellAutoComplete(m_dwShellAutoCompleteFlags);

			if (m_nTextLimit)
				m_pEdit->LimitText(m_nTextLimit);

			if (m_bNumericOnly)
				SetNumericOnly(m_bNumericOnly);
		}

		m_pEdit->SetFont(GetPaintManager()->GetIconFont(), FALSE);
		m_pEdit->SetMargins(0, 0);
		m_pEdit->UpdateCharFormat();
		if (m_pEdit->GetSafeHwnd() != ::GetFocus())
			SetEditText(strEditText);
		m_pEdit->m_bComposited = HasDwmCompositedRect();

		m_pEdit->SetReadOnly(m_bReadOnly);
		rcControl.DeflateRect(m_nLabelWidth + XTP_DPI_X(1 + 3), XTP_DPI_Y(3),
							  XTP_DPI_X(3) + (m_bShowSpinButtons ? (XTP_DPI_X(18) - 1) : 0)
								  + (m_nBuddyButtonId > 0 ? XTP_DPI_X(18) : 0),
							  XTP_DPI_Y(3));

		if (GetPaintManager()->GetEditHeight() > 0)
			rcControl.OffsetRect(0, XTP_DPI_Y(1));

		DeflateEditRect(rcControl);
		m_pEdit->MoveWindow(rcControl);
		ShowHideEditControl();
	}
	m_bDelayReposition = FALSE;
}

void CXTPControlEdit::DrawEditText(CDC* pDC, CRect rcText)
{
	if (m_nEditIconId > 0)
	{
		CSize szIcon = GetIconSize();

		CXTPImageManagerIcon* pIcon =
			m_pParent->GetImageManager()->GetImage(XTPToUInt(m_nEditIconId), szIcon.cx);
		if (pIcon)
		{
			szIcon = CXTPPaintManager::AdjustDpiIconSize(pIcon, szIcon, this, GetRect().Size());

			pIcon->Draw(pDC, CPoint(rcText.left, rcText.CenterPoint().y - szIcon.cy / 2),
						(GetEnabled() ? xtpImageNormal : xtpImageDisabled), szIcon);
		}
	}
}

void CXTPControlEdit::DeflateEditRect(CRect& rcControl)
{
	if (m_nEditIconId > 0)
	{
		CSize szIcon = GetIconSize();

		CXTPImageManagerIcon* pIcon =
			m_pParent->GetImageManager()->GetImage(XTPToUInt(m_nEditIconId), szIcon.cx);
		if (pIcon)
		{
			szIcon = GetPaintManager()->AdjustDpiIconSize(pIcon, szIcon, this, GetRect().Size());

			rcControl.left += szIcon.cx + XTP_DPI_X(2);
		}
	}
}

void CXTPControlEdit::ShowHideEditControl()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->SetWindowPos(0, 0, 0, 0, 0,
							  UINT(SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
								   | (m_pControls && m_pParent && IsVisible()
											  && m_pParent->GetSafeHwnd()
										  ? SWP_SHOWWINDOW
										  : SWP_HIDEWINDOW)));
	}
}

void CXTPControlEdit::OnRemoved()
{
	ShowHideEditControl();

	if (!m_pControls && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
	}

	CXTPControl::OnRemoved();
}

void CXTPControlEdit::SetReadOnly(BOOL bReadOnly /* = TRUE */)
{
	m_bReadOnly = bReadOnly;

	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->SetReadOnly(bReadOnly);
	}
}

BOOL CXTPControlEdit::GetReadOnly() const
{
	return m_bReadOnly;
}

void CXTPControlEdit::OnEnabledChanged()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->EnableWindow(GetEnabled());
		m_pEdit->UpdateCharFormat();
	}
}

void CXTPControlEdit::SetParent(CXTPCommandBar* pParent)
{
	CXTPControl::SetParent(pParent);

	if (!pParent && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
	}
}

BOOL CXTPControlEdit::OnSetSelected(int bSelected)
{
	if (!CXTPControl::OnSetSelected(bSelected))
		return FALSE;

	if (!bSelected)
	{
		SetFocused(FALSE);
	}

	if (m_pEdit->GetSafeHwnd())
	{
		m_pEdit->UpdateCharFormat();
	}

	return TRUE;
}

void CXTPControlEdit::OnUnderlineActivate()
{
	m_pParent->HideKeyboardTips();
	SetFocused(TRUE);
}

void CXTPControlEdit::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (IsCustomizeMode())
	{
		CustomizeStartDrag(pt);
	}
	else
	{
		if (bKeyboard && CWnd::GetFocus() == m_pEdit)
		{
			OnExecute();
		}
		else if (!bKeyboard && GetSpinButtonsRect().PtInRect(pt) && GetEnabled() && !GetReadOnly())
		{
			TrackSpinButton(pt);
		}
		else if (!bKeyboard && GetBuddyButtonRect().PtInRect(pt) && GetEnabled())
		{
			TrackBuddyButton(pt);
		}
		else
		{
			m_pEdit->SetFocus();
		}
	}
}

void CXTPControlEdit::NotifySpinChanged(int increment, int direction)
{
	NMXTPUPDOWN nm;
	nm.iDelta = increment * direction;

	if (NotifySite(direction > 0 ? XTP_FN_SPINUP : XTP_FN_SPINDOWN, &nm) != 0)
		return;

	int nPos = _ttoi(GetEditText());
	nPos += nm.iDelta;

	while (nPos % increment)
	{
		nPos += direction;
	}

	CString strEditText;
	strEditText.Format(_T("%i"), nPos);
	SetEditText(strEditText);

	NotifySite(CBN_XTP_EXECUTE);
}

void CXTPControlEdit::TrackSpinButton(CPoint pt)
{
	m_pParent->SetSelected(-1);
	m_pParent->SetSelected(GetIndex(), FALSE);
#define TID_TIMER 435

	CRect rc = GetSpinButtonsRect();
	CRect rcTop(rc.left, rc.top, rc.right, rc.CenterPoint().y);
	CRect rcBottom(rc.left, rcTop.bottom, rc.right, rc.bottom);

	const BOOL bTop			 = rcTop.PtInRect(pt);
	const CRect rcActiveRect = bTop ? rcTop : rcBottom;
	const int direction		 = bTop ? +1 : -1;
	int increment			 = 1;

	m_bSelected = m_bPressed = bTop ? XTP_EDITSPIN_UP : XTP_EDITSPIN_DOWN;
	RedrawParent(FALSE);

	NotifySpinChanged(increment, direction);

	m_pParent->SetCapture();

	m_pParent->SetTimer(TID_TIMER, 530, NULL);
	BOOL bFirst   = TRUE;
	DWORD dwStart = GetTickCount();

	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (msg.message == WM_LBUTTONUP)
		{
			break;
		}

		if (msg.message == WM_MOUSEMOVE)
		{
			pt = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));

			if (OnSetSelected(rcActiveRect.PtInRect(pt)))
			{
				dwStart = GetTickCount();
				RedrawParent();
			}
			continue;
		}
		if (msg.message == WM_TIMER && msg.wParam == TID_TIMER)
		{
			if (m_bSelected)
			{
				UINT uElapsed = (UINT)((GetTickCount() - dwStart) / 1024);

				increment = 1;
				if (uElapsed >= 5)
					increment = 20;
				else if (uElapsed >= 2)
					increment = 5;

				NotifySpinChanged(increment, direction);
			}

			if (bFirst)
			{
				m_pParent->SetTimer(TID_TIMER, 100, NULL);
			}
			bFirst = FALSE;
		}

		DispatchMessage(&msg);

		if (CWnd::GetCapture() != m_pParent)
			break;
	}

	m_pParent->KillTimer(TID_TIMER);
	m_bPressed = FALSE;
	ReleaseCapture();

	RedrawParent(FALSE);

	if (GetParent()->GetPosition() != xtpBarPopup)
		GetParent()->SetTrackingMode(FALSE);
}

void CXTPControlEdit::TrackBuddyButton(CPoint pt)
{
	m_pParent->SetSelected(-1);
	m_pParent->SetSelected(GetIndex(), FALSE);

	CRect rc = GetBuddyButtonRect();

	m_bSelected = m_bPressed = XTP_EDITBUDDYBUTTON;
	RedrawParent(FALSE);

	m_pParent->SetCapture();

	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (msg.message == WM_LBUTTONUP)
		{
			break;
		}

		if (msg.message == WM_MOUSEMOVE)
		{
			pt = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));

			BOOL bSelected = rc.PtInRect(pt) ? XTP_EDITBUDDYBUTTON : 1;

			if (bSelected != m_bSelected)
			{
				m_bSelected = bSelected;
				RedrawParent();
			}
			continue;
		}

		DispatchMessage(&msg);

		if (CWnd::GetCapture() != m_pParent)
			break;
	}

	BOOL bExecute = m_bSelected == XTP_EDITBUDDYBUTTON;

	m_bPressed  = FALSE;
	m_bSelected = 1;

	ReleaseCapture();

	RedrawParent(FALSE);

	if (bExecute)
	{
		NotifySite(XTP_FN_BUDDYBUTTONCLICK);
	}
}

CRect CXTPControlEdit::GetSpinButtonsRect() const
{
	if (!IsSpinButtonsVisible())
		return CRect(0, 0, 0, 0);

	CRect rc(GetRect());
	return CRect(rc.right - XTP_DPI_X(18), rc.top + XTP_DPI_Y(1), rc.right - XTP_DPI_X(1),
				 rc.bottom - XTP_DPI_Y(1));
}

CRect CXTPControlEdit::GetBuddyButtonRect() const
{
	if (!IsBuddyButtonVisible())
		return CRect(0, 0, 0, 0);

	CRect rc(GetRect());
	rc = CRect(rc.right - XTP_DPI_X(18 + 1), rc.top + XTP_DPI_Y(1), rc.right - XTP_DPI_X(1),
			   rc.bottom - XTP_DPI_Y(1));

	if (IsSpinButtonsVisible())
		rc.OffsetRect(-(XTP_DPI_X(18) - 1), 0);

	return rc;
}

void CXTPControlEdit::OnMouseMove(CPoint point)
{
	CXTPControl::OnMouseMove(point);

	BOOL bSelected = m_bSelected;

	if (GetEnabled() && IsSpinButtonsVisible() && bSelected)
	{
		CRect rc(GetSpinButtonsRect());

		CRect rcTop(rc.left, rc.top, rc.right, rc.CenterPoint().y);
		CRect rcBottom(rc.left, rcTop.bottom, rc.right, rc.bottom);

		if (rcTop.PtInRect(point))
		{
			bSelected = XTP_EDITSPIN_UP;
		}
		else if (rcBottom.PtInRect(point))
		{
			bSelected = XTP_EDITSPIN_DOWN;
		}
		else if ((m_bSelected == XTP_EDITSPIN_UP || m_bSelected == XTP_EDITSPIN_DOWN)
				 && !rc.PtInRect(point))
		{
			bSelected = 1;
		}
	}

	if (GetEnabled() && IsBuddyButtonVisible() && bSelected)
	{
		CRect rc(GetBuddyButtonRect());

		if (rc.PtInRect(point))
		{
			bSelected = XTP_EDITBUDDYBUTTON;
		}
		else if (m_bSelected == XTP_EDITBUDDYBUTTON && !rc.PtInRect(point))
		{
			bSelected = 1;
		}
	}

	if (bSelected != m_bSelected)
	{
		m_bSelected = bSelected;
		RedrawParent(FALSE);
	}
}

void CXTPControlEdit::OnMouseHover()
{
	if (!XTPMouseManager()->IsMouseLocked())
		CXTPControl::OnMouseHover();
}

void CXTPControlEdit::SetHideFlags(DWORD dwFlags)
{
	if (m_dwHideFlags != dwFlags)
	{
		m_dwHideFlags = dwFlags;

		ShowHideEditControl();
		DelayLayoutParent();
	}
}

void CXTPControlEdit::OnActionChanged(int nProperty)
{
	CXTPControl::OnActionChanged(nProperty);

	if (nProperty == 2)
	{
		ShowHideEditControl();
	}

	if (nProperty == 4)
	{
		if (_GetEditText().IsEmpty() && !GetEditHint().IsEmpty())
		{
			_SetEditText(GetEditHint());
		}
	}
}

void CXTPControlEdit::OnActionChanging(int nProperty)
{
	if (nProperty == 4 && !GetEditHint().IsEmpty() && _GetEditText() == GetEditHint())
	{
		_SetEditText(_T(""));
	}
}

void CXTPControlEdit::OnCalcDynamicSize(DWORD dwMode)
{
	if (dwMode & LM_VERTDOCK)
		SetHideFlags(m_dwHideFlags | xtpHideDockingPosition);
	else
		SetHideFlags(m_dwHideFlags & ~xtpHideDockingPosition);
}

void CXTPControlEdit::SetEditHint(LPCTSTR lpszEditHint)
{
	if (!GetEditHint().IsEmpty() && _GetEditText() == GetEditHint())
	{
		_SetEditText(_T(""));
	}

	m_strEditHint = lpszEditHint;

	if (_GetEditText().IsEmpty() && !GetEditHint().IsEmpty())
	{
		_SetEditText(GetEditHint());
	}
}

CString CXTPControlEdit::GetEditHint() const
{
	return !m_strEditHint.IsEmpty() ? m_strEditHint
									: m_pAction ? m_pAction->GetEditHint() : CString();
}

void CXTPControlEdit::SetTextLimit(int nTextLimit)
{
	m_nTextLimit = nTextLimit;

	if (m_pEdit && m_pEdit->GetSafeHwnd())
		m_pEdit->LimitText(m_nTextLimit);
}

int CXTPControlEdit::GetTextLimit() const
{
	return m_nTextLimit;
}

DWORD CXTPControlEdit::GetEditStyle() const
{
	return m_dwEditStyle;
}

void CXTPControlEdit::SetEditStyle(DWORD dwStyle)
{
	m_dwEditStyle = dwStyle;

	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_bDelayDestroy = TRUE;
		DelayLayoutParent();
	}
}

void CXTPControlEdit::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		CXTPControl::Copy(pControl, bRecursive);

		CXTPControlEdit* pControlEdit = DYNAMIC_DOWNCAST(CXTPControlEdit, pControl);
		if (!pControlEdit)
			return;

		m_dwEditStyle = pControlEdit->m_dwEditStyle;
		SetEditText(pControlEdit->GetEditText());
		m_bLabel = pControlEdit->m_bLabel;
		SetReadOnly(pControlEdit->m_bReadOnly);
		SetEditHint(pControlEdit->m_strEditHint);
		EnableShellAutoComplete(pControlEdit->m_dwShellAutoCompleteFlags);
		m_bShowSpinButtons = pControlEdit->m_bShowSpinButtons;
		m_nTextLimit	   = pControlEdit->m_nTextLimit;
		m_nBuddyButtonId   = pControlEdit->m_nBuddyButtonId;
	}
}

BOOL CXTPControlEdit::Compare(CXTPControl* pOther)
{
	if (!CXTPControl ::Compare(pOther))
		return FALSE;

	if (!pOther->IsKindOf(RUNTIME_CLASS(CXTPControlEdit)))
		return FALSE;

	return TRUE;
}

BOOL CXTPControlEdit::IsFocused() const
{
	return m_bFocused;
}

void CXTPControlEdit::SetFocused(BOOL bFocused)
{
	if (m_bFocused == bFocused)
		return;

	m_bFocused = bFocused;

	if (bFocused)
	{
		if (m_pEdit->GetSafeHwnd() && ::GetFocus() != m_pEdit->GetSafeHwnd())
			m_pEdit->SetFocus();

		m_pParent->SetTrackingMode(TRUE, FALSE);
		m_pParent->SetSelected(m_nIndex);
		m_pParent->LockMouseMove(this);

		NotifySite(EN_SETFOCUS);
	}
	else
	{
		if (m_pEdit->GetSafeHwnd() && ::GetFocus() == m_pEdit->GetSafeHwnd())
			::SetFocus(m_pParent->GetTrackFocus());

		m_pParent->UnlockMouseMove();

		NotifySite(EN_KILLFOCUS);
	}

	RedrawParent();
}

void CXTPControlEdit::OnSetFocus(CWnd* pOldWnd)
{
	if (!GetEditHint().IsEmpty() && m_pEdit->GetSafeHwnd() && GetEditHint() == _GetEditText())
	{
		_SetEditText(_T(""));
	}

	m_strLastText = GetEditText();

	if (m_pParent->GetTrackFocus() == 0)
		m_pParent->SetTrackFocus(pOldWnd ? pOldWnd->GetSafeHwnd()
										 : m_pParent->GetSite()->GetSafeHwnd());

	m_pParent->SetPopuped(-1);

	SetFocused(TRUE);
	if (m_pAutoCompleteWnd)
		m_pAutoCompleteWnd->SetupMessageHook(TRUE);
}

void CXTPControlEdit::OnKillFocus()
{
	if (!GetEditHint().IsEmpty() && m_pEdit->GetSafeHwnd() && _GetEditText().IsEmpty())
	{
		_SetEditText(GetEditHint());
	}

	SetFocused(FALSE);
	if (m_pAutoCompleteWnd)
		m_pAutoCompleteWnd->SetupMessageHook(FALSE);
}

BOOL CXTPControlEdit::IsImageVisible() const
{
	if (GetParent()->GetType() == xtpBarTypePopup)
		return GetImage(0) != NULL;

	BOOL bImageVisible = (CXTPControl::GetStyle() == xtpButtonIcon
						  || CXTPControl::GetStyle() == xtpButtonIconAndCaption)
						 && (GetImage(0) != NULL);

	return bImageVisible;
}

BOOL CXTPControlEdit::IsCaptionVisible() const
{
	if (GetParent()->GetType() == xtpBarTypePopup)
		return TRUE;

	if (GetCaption().IsEmpty())
		return FALSE;

	XTPButtonStyle buttonStyle = GetStyle();

	if ((buttonStyle == xtpButtonCaption) || (buttonStyle == xtpButtonIconAndCaption)
		|| (buttonStyle == xtpButtonIconAndCaptionBelow))
		return TRUE;

	return m_bLabel;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPControlEditCtrl, CXTPCommandBarEditCtrl)
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_MESSAGE(XTP_TTM_WINDOWFROMPOINT, OnWindowFromPoint)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEditChanged)
	ON_MESSAGE(WM_ENABLE, OnEnableDisable)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

// [IP] This trick (handling WM_ENABLE) allows to skip using COLOR_GRAYTEXT for disabled control
// background (default Win32) You can now handle the WM_CTLCOLOR/CTL_COLOREDIT message in the parent
// or the reflected (=WM_CTLCOLOR) message in the derived subclass as you normally would. And you
// can still use IsWindowEnabled should you care to.
LRESULT CXTPControlEditCtrl::OnEnableDisable(WPARAM, LPARAM)
{
	Invalidate();

	return 0;
}

CXTPControlEditCtrl::CXTPControlEditCtrl()
{
	m_pControl = NULL;
}

void CXTPControlEditCtrl::OnEditChanged()
{
	m_pControl->m_bEditChanged = TRUE;

	if (::GetFocus() == m_hWnd && !m_bIgonoreEditChanged)
		m_pControl->OnEditChanged();

	SetMargins(0, 0);
	UpdateCharFormat();
}

void CXTPControlEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN && ::GetFocus() == m_hWnd)
	{
		m_pControl->OnExecute();
		return;
	}

	CXTPCommandBarEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPControlEditCtrl::UpdateCharFormat()
{
	CHARFORMAT2 cf = m_pControl->GetDefaultCharFormat();
	cf.cbSize	  = GetRichEditContext().m_bRichEdit2 ? sizeof(CHARFORMAT2) : sizeof(CHARFORMAT);

	::SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, XTPToLPARAM(&cf));
	::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, FALSE, XTPToLPARAM(cf.crBackColor));
}

void CXTPControlEditCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
}

void CXTPControlEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		if (m_pControl->IsCustomizeMovable())
			m_pControl->OnClick();
	}
	else
		CEdit::OnLButtonDown(nFlags, point);
}

void CXTPControlEditCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		ClientToScreen(&point);
		m_pControl->GetParent()->ScreenToClient(&point);

		m_pControl->GetParent()->OnRButtonDown(nFlags, point);
		return;
	}

	m_pControl->SetFocused(TRUE);

	if (!ShowContextMenu(m_pControl, point))
		CXTPCommandBarEditCtrl::OnRButtonDown(nFlags, point);
}

void CXTPControlEditCtrl::OnDestroy()
{
	if (m_pControl)
		GetWindowTextEx(m_pControl->m_strEditText);
	CEdit::OnDestroy();
}

void CXTPControlEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	if (!m_pControl->IsCustomizeMode())
	{
		m_pControl->OnSetFocus(pOldWnd);
		CEdit::OnSetFocus(pOldWnd);

		SetSel(0, 0);
		::PostMessage(m_hWnd, EM_SETSEL, 0, -1);
	}
}

void CXTPControlEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CXTPCommandBarEditCtrl::OnKillFocus(pNewWnd);
	m_pControl->OnKillFocus();
}

void CXTPControlEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CEdit::OnMouseMove(nFlags, point);

	MapWindowPoints(m_pControl->GetParent(), &point, 1);
	m_pControl->GetParent()->OnMouseMove(nFlags, point);
}

CHARFORMAT2 CXTPControlEdit::GetDefaultCharFormat()
{
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.dwMask = CFM_COLOR | CFM_BACKCOLOR;

	CXTPPaintManager* pPaintManager = GetPaintManager();

	COLORREF clrEditTextNormal   = GetXtremeColor(COLOR_WINDOWTEXT);
	COLORREF clrEditTextDisabled = GetXtremeColor(COLOR_GRAYTEXT);

	if (!pPaintManager->m_clrEditTextNormal.IsDefaultValue())
	{
		clrEditTextNormal = pPaintManager->m_clrEditTextNormal;
	}

	if (!pPaintManager->m_clrEditTextDisabled.IsDefaultValue())
	{
		clrEditTextDisabled = pPaintManager->m_clrEditTextDisabled;
	}

	if (GetEnabled())
	{
		cf.crTextColor = clrEditTextNormal;

		if (!GetEditHint().IsEmpty() && GetEditHint() == _GetEditText())
		{
			cf.crTextColor = clrEditTextDisabled;
		}
	}
	else
	{
		cf.crTextColor = clrEditTextDisabled;
	}

	cf.crBackColor = GetPaintManager()->GetControlEditBackColor(this);

	return cf;
}

BOOL CXTPControlEditCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPCommandBar* pCommandBar = m_pControl->GetParent();

	if (pCommandBar)
	{
		pCommandBar->FilterToolTipMessage(message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

LRESULT CXTPControlEditCtrl::OnWindowFromPoint(WPARAM, LPARAM)
{
	return 1;
}

static BOOL IsClipboardNumericOnly(HWND hWnd)
{
	BOOL bRes = TRUE;

	if (::IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		if (::OpenClipboard(hWnd))
		{
			HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);
			if (hData != NULL)
			{
				TCHAR* pData = (TCHAR*)::GlobalLock(hData);
				if (pData)
				{
					while (*pData != _T('\0'))
					{
						if (_T('0') <= *pData && *pData <= _T('9'))
						{
							pData++;
						}
						else
						{
							bRes = FALSE;
							break;
						}
					}

					::GlobalUnlock(hData);
				}
			}

			::CloseClipboard();
		}
	}

	return bRes;
}

static LRESULT CALLBACK InputCheckProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
									   UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	UNREFERENCED_PARAMETER(uIdSubclass);

	CXTPControlEdit* pObj = reinterpret_cast<CXTPControlEdit*>(dwRefData);
	if (!pObj)
		return NULL;

	switch (uMsg)
	{
		case WM_NCDESTROY:
		{
			pObj->SetNumericOnly(FALSE);
			break;
		}

		case WM_CHAR:
		{
			if (_T('0') <= static_cast<TCHAR>(wParam) && static_cast<TCHAR>(wParam) <= _T('9'))
				break;

			return NULL;
		}

		case WM_PASTE:
		{
			if (!IsClipboardNumericOnly(pObj->GetEditCtrl()->GetSafeHwnd()))
			{
				::MessageBeep(MB_ICONEXCLAMATION);
				return NULL;
			}

			break;
		}

		case WM_KEYDOWN:
		{
			if ((GetAsyncKeyState(VK_CONTROL) && static_cast<TCHAR>(wParam) == _T('V'))
				|| (GetAsyncKeyState(VK_SHIFT) && wParam == VK_INSERT))
			{
				if (!IsClipboardNumericOnly(pObj->GetEditCtrl()->GetSafeHwnd()))
				{
					::MessageBeep(MB_ICONEXCLAMATION);
					return NULL;
				}
			}

			break;
		}
	}

	return DefSubclassProcPtr(hwnd, uMsg, wParam, lParam);
}

BOOL CXTPControlEdit::SetNumericOnly(BOOL bNumericOnly)
{
	BOOL res	   = FALSE;
	m_bNumericOnly = bNumericOnly;

	if (SetWindowSubclassPtr && RemoveWindowSubclassPtr && DefSubclassProcPtr && m_pEdit
		&& m_pEdit->GetSafeHwnd())
	{
		if (m_bNumericOnly)
			res = SetWindowSubclassPtr(m_pEdit->GetSafeHwnd(), InputCheckProc, 0, (DWORD_PTR)this);
		else
			res = RemoveWindowSubclassPtr(m_pEdit->GetSafeHwnd(), InputCheckProc, 0);
	}

	return res;
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlEdit, CXTPControl)
	DISP_PROPERTY_ID(CXTPControlEdit, "ShowLabel", dispidShowLabel, m_bLabel, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "Text", dispidText, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "ReadOnly", 60, GetReadOnly, SetReadOnly, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "EditHandle", 63, OleGetEditHandle, SetNotSupported,
						VT_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "HasFocus", 66, HasFocus, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "EditHint", 67, OleGetEditHint, SetEditHint, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "ShellAutoComplete", 69, OleGetShellAutoComplete,
						OleSetShellAutoComplete, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPControlEdit, "EditIconId", 70, OleGetEditIconId, OleSetEditIconId,
						VT_I4)

	DISP_PROPERTY_EX_ID(CXTPControlEdit, "EditStyle", 71, GetEditStyle, SetEditStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPControlEdit, "ShowSpinButtons", 72, m_bShowSpinButtons, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlEdit, "BuddyButtonId", 81, OleGetBuddyButtonId,
						OleSetBuddyButtonId, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPControlEdit, "TextLimit", 80, GetTextLimit, SetTextLimit, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPControlEdit, "NumericOnly", 82, OleGetNumericOnly, OleSetNumericOnly,
						VT_BOOL)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlEdit, CXTPControl)
	INTERFACE_PART(CXTPControlEdit, XTPDIID_ICommandBarEdit, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlEdit, XTPDIID_ICommandBarEdit)

BSTR CXTPControlEdit::OleGetText()
{
	return GetEditText().AllocSysString();
}

void CXTPControlEdit::OleSetEditIconId(int nId)
{
	SetEditIconId(nId);
}

int CXTPControlEdit::OleGetEditIconId()
{
	return GetEditIconId();
}

HWND CXTPControlEdit::OleGetEditHandle()
{
	return m_pEdit ? m_pEdit->GetSafeHwnd() : 0;
}

void CXTPControlEdit::OleSetText(LPCTSTR strText)
{
	if (!HasFocus())
		SetEditText(strText);
}

BSTR CXTPControlEdit::OleGetEditHint()
{
	return GetEditHint().AllocSysString();
}

DWORD CXTPControlEdit::OleGetShellAutoComplete()
{
	return m_dwShellAutoCompleteFlags;
}

void CXTPControlEdit::OleSetShellAutoComplete(DWORD dwShellAutoCompleteFlags)
{
	EnableShellAutoComplete(dwShellAutoCompleteFlags);
}

int CXTPControlEdit::OleGetBuddyButtonId()
{
	return m_nBuddyButtonId;
}

void CXTPControlEdit::OleSetBuddyButtonId(int nValue)
{
	ShowBuddyButton(nValue);
}

BOOL CXTPControlEdit::OleGetNumericOnly()
{
	return IsNumericOnly();
}

VOID CXTPControlEdit::OleSetNumericOnly(BOOL Value)
{
	SetNumericOnly(Value);
}

#endif
