// XTPDialogBar.cpp : implementation of the CXTPDialogBar class.
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

#include "CommandBars/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPDialogBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_XTP_COMMANDBAR(CXTPDialogBar, CXTPToolBar)

class CXTPDialogBar::CControlButtonHide : public CXTPControlButton
{
public:
	CControlButtonHide()
	{
		m_dwFlags = xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable;
	}
	void Draw(CDC* pDC);
	void SetBeginGroup(BOOL /*bBeginGroup*/)
	{
	}
	void OnExecute()
	{
		ASSERT(m_pParent);
		if (m_pParent)
			((CXTPToolBar*)m_pParent)->SetVisible(FALSE);
	}
};

void CXTPDialogBar::CControlButtonHide::Draw(CDC* pDC)
{
	GetPaintManager()->DrawControlEntry(pDC, this);
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolClose, m_rcControl,
									 GetPaintManager()->GetControlTextColor(this));
}

class CXTPDialogBar::CControlCaptionPopup : public CXTPControlPopup
{
public:
	CControlCaptionPopup()
	{
		m_dwFlags = xtpFlagLeftPopup | xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable;
		m_bCaptionPopup = FALSE;
	}
	void Draw(CDC* pDC);

	void AdjustExcludeRect(CRect& rc, BOOL /*bVertical*/)
	{
		GetPaintManager()->AdjustExcludeRect(rc, this, FALSE);
	}
	BOOL OnSetPopup(BOOL bPopup)
	{
		if (m_bCaptionPopup)
			return CXTPControlPopup::OnSetPopup(bPopup);
		return TRUE;
	}

public:
	BOOL m_bCaptionPopup;
};

void CXTPDialogBar::CControlCaptionPopup::Draw(CDC* pDC)
{
	ASSERT(NULL != pDC);

	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPFontDC font(pDC, pPaintManager->GetSmCaptionFont());

	CRect rcButton = GetRect();

	if (m_bCaptionPopup)
	{
		pPaintManager->DrawControlEntry(pDC, this);
	}

	pDC->SetTextColor(pPaintManager->GetControlTextColor(this));

	CRect rcText(rcButton);
	rcText.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_Y(m_bCaptionPopup ? 15 : 2), 0);
	pDC->DrawText(GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER);

	COLORREF clrText = pDC->GetTextColor();

	if (m_bCaptionPopup)
	{
		CRect rc(rcButton.right - rcButton.Height(), rcButton.top, rcButton.right, rcButton.bottom);
		rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rc, clrText);
	}
}

class CXTPDialogBar::CCaptionPopupBar : public CXTPPopupBar
{
public:
	CCaptionPopupBar()
	{
		m_popupFlags = xtpPopupDown;
	}
	void UpdateFlags()
	{
		CXTPPopupBar::UpdateFlags();
		m_popupFlags = xtpPopupDown | xtpPopupLeft;

		m_ptPopup.x = (m_popupFlags & xtpPopupDown) ? m_rcExclude.left : m_rcExclude.right;
		m_ptPopup.y = (m_popupFlags & xtpPopupDown) ? m_rcExclude.bottom : m_rcExclude.top;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPDialogBar

CXTPDialogBar::CXTPDialogBar()
{
	m_rcBorders		  = XTP_DPI(CRect(3, 3, 3, 3));
	m_bResizable	  = TRUE;
	m_bCaptionVisible = TRUE;

	m_szDockingClient = m_szFloatingClient = XTP_DPI(CSize(200, 200));
	m_szMinTrackRect					   = XTP_DPI(CSize(50, 50));
	m_bCaptionPopup						   = TRUE;

	m_hwndChild = 0;

	m_pPopupBar		 = new CCaptionPopupBar();
	m_nCaptionHeight = 0;

	m_bCustomizable = FALSE;

	m_bActive = FALSE;

	EnableAutomation();
}

CXTPDialogBar::~CXTPDialogBar()
{
	m_pPopupBar->InternalRelease();
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPDialogBar, CXTPToolBar)
	//{{AFX_MSG_MAP(CXTPDialogBar)
	ON_WM_NCHITTEST_EX()
	ON_WM_SIZE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPDialogBar message handlers

CSize CXTPDialogBar::CalcDockingLayout(int nLength, DWORD dwMode, int /*nWidth*/)
{
	return CalcSize(nLength, dwMode, m_szDockingClient);
}

CRect CXTPDialogBar::GetMargins()
{
	CRect rcBorder = m_rcBorders;

	if (m_bCaptionVisible)
	{
		CSize szBar = GetPaintManager()->GetDefaultCommandBarSize(this);
		rcBorder.top += szBar.cy;
	}

	return rcBorder;
}

CSize CXTPDialogBar::CalcSize(int nLength, DWORD dwMode, CSize szClient)
{
	CRect rcBorder   = m_rcBorders;
	CSize sizeResult = szClient;

	sizeResult.cx += rcBorder.left + rcBorder.right;
	sizeResult.cy += rcBorder.top + rcBorder.bottom;

	if (!m_bCaptionVisible)
	{
		m_nCaptionHeight = 0;

		if (dwMode & LM_STRETCH)
		{
			if (dwMode & LM_VERTDOCK)
				sizeResult.cy = max(nLength, sizeResult.cy);
			else
				sizeResult.cx = max(nLength, sizeResult.cx);
		}
		return sizeResult;
	}

	CSize szBar		 = GetPaintManager()->GetDefaultCommandBarSize(this);
	m_nCaptionHeight = szBar.cy + rcBorder.top;

	sizeResult.cy += szBar.cy;

	if (dwMode & LM_STRETCH)
	{
		if (dwMode & LM_VERTDOCK)
			sizeResult.cy = max(nLength, sizeResult.cy);
		else
			sizeResult.cx = max(nLength, sizeResult.cx);
	}

	if (dwMode & LM_HORZ)
		sizeResult.cx = min(nLength, sizeResult.cx);
	else
		sizeResult.cy = min(nLength, sizeResult.cy);

	int nRightBorder = sizeResult.cx - rcBorder.right;

	CXTPControl* pHide = m_pControls->FindControl(XTP_ID_TOOLBAR_HIDE);
	if (!pHide && IsCloseable())
	{
		pHide = (CXTPControlPopup*)m_pControls->Add(new CControlButtonHide(), XTP_ID_TOOLBAR_HIDE,
													_T(""), -1, TRUE);
	}
	if (pHide && !IsCloseable())
	{
		pHide->SetHideFlags(xtpHideGeneric);
	}
	else if (pHide)
	{
		pHide->SetRect(CRect(CPoint(nRightBorder - XTP_DPI_X(16),
									m_rcBorders.top + szBar.cy / 2 - XTP_DPI_Y(8)),
							 XTP_DPI(CSize(16, 16))));
		nRightBorder -= XTP_DPI_X(16);
	}

	CControlCaptionPopup* pCaption = (CControlCaptionPopup*)m_pControls->FindControl(
		XTP_ID_DIALOGBAR_CAPTION);
	if (!pCaption)
	{
		pCaption = (CControlCaptionPopup*)m_pControls->Add(new CControlCaptionPopup(),
														   XTP_ID_DIALOGBAR_CAPTION, _T(""), -1,
														   TRUE);
		pCaption->SetCommandBar(m_pPopupBar);
	}

	pCaption->m_bCaptionPopup = m_bCaptionPopup && (m_pPopupBar->GetControlCount() != 0);

	int nCaptionHeight = XTP_DPI_Y(16);
	int nCaptionTop	= m_rcBorders.top + szBar.cy / 2 - nCaptionHeight / 2;
	pCaption->SetRect(CRect(m_rcBorders.left + szBar.cx, nCaptionTop, nRightBorder,
							nCaptionTop + nCaptionHeight));
	pCaption->SetCaption(m_strCaption.IsEmpty() ? GetTitle() : m_strCaption);

	return sizeResult;
}

CSize CXTPDialogBar::CalcDynamicLayout(int /*nLength*/, DWORD dwMode)
{
	return CalcSize(32000, dwMode, m_szFloatingClient);
}

LRESULT CXTPDialogBar::OnNcHitTest(CPoint point)
{
	if (!m_bResizable)
		return CXTPCommandBar::OnNcHitTest(point);

	CRect rectWindow;
	GetWindowRect(&rectWindow);
	rectWindow.InflateRect(-XTP_DPI_X(3), -XTP_DPI_Y(3));

	if (m_barPosition != xtpBarFloating)
	{
		if (m_barPosition == xtpBarBottom)
		{
			if (point.y <= rectWindow.top)
				return HTTOP;
		}
		else
		{
			if ((point.y >= rectWindow.bottom)
				&& (((m_dwFlags & xtpFlagStretched) == 0) || !IsVerticalPosition(GetPosition())))
				return HTBOTTOM;
		}

		if (m_barPosition == xtpBarRight)
		{
			if (point.x <= rectWindow.left)
				return HTLEFT;
		}
		else
		{
			if ((point.x >= rectWindow.right)
				&& (((m_dwFlags & xtpFlagStretched) == 0) || IsVerticalPosition(GetPosition())))
				return HTRIGHT;
		}
	}
	else
	{
		if ((point.y < rectWindow.top) && (point.x < rectWindow.left))
			return HTTOPLEFT;
		if ((point.y < rectWindow.top) && (point.x >= rectWindow.right))
			return HTTOPRIGHT;
		if ((point.y >= rectWindow.bottom) && (point.x < rectWindow.left))
			return HTBOTTOMLEFT;
		if ((point.y >= rectWindow.bottom) && (point.x >= rectWindow.right))
			return HTBOTTOMRIGHT;
		if (point.y < rectWindow.top)
			return HTTOP;
		if (point.y >= rectWindow.bottom)
			return HTBOTTOM;
		if (point.x < rectWindow.left)
			return HTLEFT;
		if (point.x >= rectWindow.right)
			return HTRIGHT;
	}

	return CXTPCommandBar::OnNcHitTest(point);
}

void CXTPDialogBar::OnResize(CRect rcWindow, int nHitTest)
{
	CRect rcMargins = GetMargins();

	if (rcWindow.Width() < m_szMinTrackRect.cx || rcWindow.Height() < m_szMinTrackRect.cy)
		return;

	if (GetPosition() == xtpBarFloating)
	{
		m_szFloatingClient = CSize(rcWindow.Width() - rcMargins.left - rcMargins.right,
								   rcWindow.Height() - rcMargins.top - rcMargins.bottom);

		CalcDynamicLayout(0, 0);
		MoveWindow(rcWindow);
		Redraw();
	}
	else
	{
		CSize szDockingClient = CSize(rcWindow.Width() - rcMargins.left - rcMargins.right,
									  rcWindow.Height() - rcMargins.top - rcMargins.bottom);

		if (nHitTest == HTRIGHT || nHitTest == HTLEFT)
			m_szDockingClient.cx = szDockingClient.cx;
		else
			m_szDockingClient.cy = szDockingClient.cy;

		OnRecalcLayout();
	}
}

int CXTPDialogBar::OnMouseHitTest(CPoint pt)
{
	CXTPWindowRect rc(this);
	rc.bottom = rc.top + m_nCaptionHeight;

	return rc.PtInRect(pt) ? HTCLIENT : HTERROR;
}

void CXTPDialogBar::MoveChildWindow(CRect rc)
{
	if (m_hwndChild)
	{
		CRect rcMargins = GetMargins();
		CRect rcChild(rcMargins.left, rcMargins.top, rc.Width() - rcMargins.right,
					  rc.Height() - rcMargins.bottom);

		::SetWindowPos(m_hwndChild, 0, rcChild.left, rcChild.top, rcChild.Width(), rcChild.Height(),
					   SWP_NOZORDER | SWP_NOACTIVATE);
		::InvalidateRect(m_hwndChild, 0, FALSE);
	}
}

void CXTPDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CXTPToolBar::OnSize(nType, cx, cy);

	MoveChildWindow(CRect(0, 0, cx, cy));
}

void CXTPDialogBar::SetChild(CWnd* pChildWnd)
{
	m_hwndChild = pChildWnd->m_hWnd;

	MoveChildWindow(CXTPClientRect(this));
	DelayLayout();
}

void CXTPDialogBar::SetCaption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
	DelayLayout();
}

void CXTPDialogBar::SetCaptionPopup(int nMenuResourceID)
{
	CMenu menu;
	VERIFY(XTPResourceManager()->LoadMenu(&menu, XTPToUInt(nMenuResourceID)));
	m_pPopupBar->LoadMenu(menu.GetSubMenu(0));
}

void CXTPDialogBar::SetSize(CSize sz)
{
	m_szDockingClient = m_szFloatingClient = sz;
	DelayLayout();
}

CString CXTPDialogBar::GetCaption() const
{
	return m_strCaption;
}

void CXTPDialogBar::SetCaptionVisible(BOOL bVisible)
{
	m_bCaptionVisible = bVisible;
	DelayLayout();
}

LRESULT CXTPDialogBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	CWnd* pFocus = GetFocus();
	BOOL bActive = (pFocus->GetSafeHwnd()
					&& (pFocus == this || IsChild(pFocus)
						|| (pFocus->GetOwner()->GetSafeHwnd() && IsChild(pFocus->GetOwner()))));

	if (bActive != m_bActive)
	{
		m_bActive = bActive;
		Redraw();
	}

	return CXTPToolBar::OnIdleUpdateCmdUI(wParam, lParam);
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPDialogBar, CXTPToolBar)
	DISP_PROPERTY_NOTIFY_ID(CXTPDialogBar, "Caption", 100, m_strCaption, OnRecalcLayout, VT_BSTR)
	DISP_PROPERTY_ID(CXTPDialogBar, "Resizable", 101, m_bResizable, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDialogBar, "CaptionPopupBar", 102, OleGetCaptionPopupBar,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPDialogBar, "ChildHandle", 103, OleGetChildHandle, OleSetChildHandle,
						VT_HANDLE)
	DISP_FUNCTION_ID(CXTPDialogBar, "SetSize", 104, OleSetSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_PROPERTY_ID(CXTPDialogBar, "ShowCaption", 105, m_bCaptionVisible, VT_BOOL)
	DISP_PROPERTY_ID(CXTPDialogBar, "CaptionPopup", 106, m_bCaptionPopup, VT_BOOL)

END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDialogBar, XTPDIID_IDialogBar)

BEGIN_INTERFACE_MAP(CXTPDialogBar, CXTPToolBar)
	INTERFACE_PART(CXTPDialogBar, XTPDIID_IDialogBar, Dispatch)
END_INTERFACE_MAP()

LPDISPATCH CXTPDialogBar::OleGetCaptionPopupBar()
{
	return m_pPopupBar->GetIDispatch(TRUE);
}

HWND CXTPDialogBar::OleGetChildHandle()
{
	return m_hwndChild;
}

void CXTPDialogBar::OleSetChildHandle(HWND hWnd)
{
	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
	}

	m_hwndChild = hWnd;

	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_CHILD);

	::SetParent(hWnd, GetSafeHwnd());
	::ShowWindow(m_hwndChild, SW_SHOWNOACTIVATE);

	MoveChildWindow(CXTPClientRect(this));
}

void CXTPDialogBar::OleSetSize(int cx, int cy)
{
	SetSize(CSize(cx, cy));
}

#endif
