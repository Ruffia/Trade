// XTPColorSelectorCtrl.cpp : implementation file
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

#include "Common/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPFontManager.h"
#include "Common/XTPSystemMetrics.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Resource.h"
#include "Controls/Defines.h"
#include "Controls/Dialog/XTPColorDialog.h"
#include "Controls/Popup/XTPColorSelectorCtrl.h"
#include "Controls/Util/XTPFunctions.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_STATIC XTP_PICK_BUTTON _StandardColors[] = {
	{ RGB(0x00, 0x00, 0x00), XTP_IDS_CLR_BLACK },  { RGB(0x80, 0x80, 0x80), XTP_IDS_CLR_GRAY },
	{ RGB(0x80, 0x00, 0x00), XTP_IDS_CLR_MAROON }, { RGB(0x80, 0x80, 0x00), XTP_IDS_CLR_OLIVE },
	{ RGB(0x00, 0x80, 0x00), XTP_IDS_CLR_GREEN },  { RGB(0x00, 0x80, 0x80), XTP_IDS_CLR_TEAL },
	{ RGB(0x00, 0x00, 0x80), XTP_IDS_CLR_NAVY },   { RGB(0x80, 0x00, 0x80), XTP_IDS_CLR_PURPLE },
	{ RGB(0xFF, 0xFF, 0xFF), XTP_IDS_CLR_WHITE },  { RGB(0xC0, 0xC0, 0xC0), XTP_IDS_CLR_SILVER },
	{ RGB(0xFF, 0x00, 0x00), XTP_IDS_CLR_RED },	{ RGB(0xFF, 0xFF, 0x00), XTP_IDS_CLR_YELLOW },
	{ RGB(0x00, 0xFF, 0x00), XTP_IDS_CLR_LIME },   { RGB(0x00, 0xFF, 0xFF), XTP_IDS_CLR_AQUA },
	{ RGB(0x00, 0x00, 0xFF), XTP_IDS_CLR_BLUE },   { RGB(0xFF, 0x00, 0xFF), XTP_IDS_CLR_FUCHSIA },
};

AFX_STATIC XTP_PICK_BUTTON _ExtendedColors[] = {
	{ RGB(0x00, 0x00, 0x00), XTP_IDS_CLR_BLACK },
	{ RGB(0x99, 0x33, 0x00), XTP_IDS_CLR_BROWN },
	{ RGB(0x33, 0x33, 0x00), XTP_IDS_CLR_OLIVE_GREEN },
	{ RGB(0x00, 0x33, 0x00), XTP_IDS_CLR_DARK_GREEN },
	{ RGB(0x00, 0x33, 0x66), XTP_IDS_CLR_DARK_TEAL },
	{ RGB(0x00, 0x00, 0x80), XTP_IDS_CLR_DARK_BLUE },
	{ RGB(0x33, 0x33, 0x99), XTP_IDS_CLR_INDIGO },
	{ RGB(0x33, 0x33, 0x33), XTP_IDS_CLR_GRAY80 },
	{ RGB(0x80, 0x00, 0x00), XTP_IDS_CLR_DARK_RED },
	{ RGB(0xFF, 0x66, 0x00), XTP_IDS_CLR_ORANGE },
	{ RGB(0x80, 0x80, 0x00), XTP_IDS_CLR_DARK_YELLOW },
	{ RGB(0x00, 0x80, 0x00), XTP_IDS_CLR_GREEN },
	{ RGB(0x00, 0x80, 0x80), XTP_IDS_CLR_TEAL },
	{ RGB(0x00, 0x00, 0xFF), XTP_IDS_CLR_BLUE },
	{ RGB(0x66, 0x66, 0x99), XTP_IDS_CLR_BLUEGRAY },
	{ RGB(0x80, 0x80, 0x80), XTP_IDS_CLR_GRAY50 },
	{ RGB(0xFF, 0x00, 0x00), XTP_IDS_CLR_RED },
	{ RGB(0xFF, 0x99, 0x00), XTP_IDS_CLR_LIGHTORANGE },
	{ RGB(0x99, 0xCC, 0x00), XTP_IDS_CLR_LIME },
	{ RGB(0x33, 0x99, 0x66), XTP_IDS_CLR_SEA_GREEN },
	{ RGB(0x33, 0xCC, 0xCC), XTP_IDS_CLR_AQUA },
	{ RGB(0x33, 0x66, 0xFF), XTP_IDS_CLR_LIGHT_BLUE },
	{ RGB(0x80, 0x00, 0x80), XTP_IDS_CLR_VIOLET },
	{ RGB(0x99, 0x99, 0x99), XTP_IDS_CLR_GRAY40 },
	{ RGB(0xFF, 0x00, 0xFF), XTP_IDS_CLR_PINK },
	{ RGB(0xFF, 0xCC, 0x00), XTP_IDS_CLR_GOLD },
	{ RGB(0xFF, 0xFF, 0x00), XTP_IDS_CLR_YELLOW },
	{ RGB(0x00, 0xFF, 0x00), XTP_IDS_CLR_BRIGHT_GREEN },
	{ RGB(0x00, 0xFF, 0xFF), XTP_IDS_CLR_TURQUOISE },
	{ RGB(0x00, 0xCC, 0xFF), XTP_IDS_CLR_SKY_BLUE },
	{ RGB(0x99, 0x33, 0x66), XTP_IDS_CLR_PLUM },
	{ RGB(0xC0, 0xC0, 0xC0), XTP_IDS_CLR_GRAY25 },
	{ RGB(0xFF, 0x99, 0xCC), XTP_IDS_CLR_ROSE },
	{ RGB(0xFF, 0xCC, 0x99), XTP_IDS_CLR_TAN },
	{ RGB(0xFF, 0xFF, 0x99), XTP_IDS_CLR_LIGHT_YELLOW },
	{ RGB(0xCC, 0xFF, 0xCC), XTP_IDS_CLR_LIGHT_GREEN },
	{ RGB(0xCC, 0xFF, 0xFF), XTP_IDS_CLR_LIGHT_TURQUOISE },
	{ RGB(0x99, 0xCC, 0xFF), XTP_IDS_CLR_PALE_BLUE },
	{ RGB(0xCC, 0x99, 0xFF), XTP_IDS_CLR_LAVENDER },
	{ RGB(0xFF, 0xFF, 0xFF), XTP_IDS_CLR_WHITE },
};

AFX_STATIC UINT _nStringIDs[10] = {
	XTP_IDS_BACKGROUND1, XTP_IDS_TEXT1,   XTP_IDS_BACKGROUND2, XTP_IDS_TEXT2,   XTP_IDS_ACCENT1,
	XTP_IDS_ACCENT2,	 XTP_IDS_ACCENT3, XTP_IDS_ACCENT4,	 XTP_IDS_ACCENT5, XTP_IDS_ACCENT6,
};

AFX_STATIC int _ShadeMatrix[6][10] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -5, +50, -10, +80, +80, +80, +80, +80, +80, +80 },
	{ -15, +35, -25, +60, +60, +60, +60, +60, +60, +60 },
	{ -25, +25, -50, +40, +40, +40, +40, +40, +40, +40 },
	{ -35, +15, -75, -25, -25, -25, -25, -25, -25, -25 },
	{ -50, +5, -90, -50, -50, -50, -50, -50, -50, -50 },
};

AFX_STATIC XTP_PICK_BUTTON _ThemeColors[] = {
	{ RGB(255, 255, 255), XTP_IDS_CLR_WHITE },  { RGB(0, 0, 0), XTP_IDS_CLR_BLACK },
	{ RGB(231, 230, 230), XTP_IDS_CLR_GRAY25 }, { RGB(68, 84, 106), XTP_IDS_CLR_BLUEGRAY },
	{ RGB(91, 155, 213), XTP_IDS_CLR_BLUE },	{ RGB(237, 125, 49), XTP_IDS_CLR_ORANGE },
	{ RGB(165, 165, 165), XTP_IDS_CLR_GRAY50 }, { RGB(255, 192, 0), XTP_IDS_CLR_GOLD },
	{ RGB(68, 114, 196), XTP_IDS_CLR_BLUE },	{ RGB(112, 173, 71), XTP_IDS_CLR_GREEN },
};

AFX_STATIC XTP_PICK_BUTTON _ThemeStdColors[] = {
	{ RGB(192, 0, 0), XTP_IDS_CLR_DARK_RED },		{ RGB(255, 0, 0), XTP_IDS_CLR_RED },
	{ RGB(255, 192, 0), XTP_IDS_CLR_ORANGE },		{ RGB(255, 255, 0), XTP_IDS_CLR_YELLOW },
	{ RGB(146, 208, 80), XTP_IDS_CLR_LIGHT_GREEN }, { RGB(0, 176, 80), XTP_IDS_CLR_GREEN },
	{ RGB(0, 176, 240), XTP_IDS_CLR_LIGHT_BLUE },   { RGB(0, 112, 192), XTP_IDS_CLR_BLUE },
	{ RGB(0, 32, 96), XTP_IDS_CLR_DARK_BLUE },		{ RGB(112, 48, 160), XTP_IDS_CLR_PURPLE },
};

CUIntArray CXTPColorSelectorCtrl::m_arRecentColors;
CString CXTPColorSelectorCtrl::m_strTitleRecent;

/////////////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrl

CXTPColorSelectorCtrl::CXTPColorSelectorCtrl()
{
	m_pTheme	 = NULL;
	m_pColorDlg  = NULL;
	m_point		 = CPoint(0, 0);
	m_nCols		 = 8;
	m_nCurSel	= -1;
	m_dwPopup	= NULL;
	m_pParentWnd = NULL;
	m_clrColor   = CLR_DEFAULT;
	m_clrDefault = CLR_DEFAULT;
	m_nPressed   = -1;
	m_rcBorders  = CRect(0, 0, 0, 0);
	m_bCaptions  = TRUE;

	m_pExtendedColors = NULL;
	m_nExtendedColors = 0;

	VERIFY(SetTheme(xtpControlThemeDefault));
}

CXTPColorSelectorCtrl::~CXTPColorSelectorCtrl()
{
	CMDTARGET_RELEASE(m_pTheme);

	// Free memory allocated to create color cells.
	while (!m_arCells.IsEmpty())
	{
		COLOR_CELL* pColorCell = m_arCells.RemoveHead();
		SAFE_DELETE(pColorCell);
	}
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPColorSelectorCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTPColorSelectorCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST_EX()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrl message handlers

BOOL CXTPColorSelectorCtrl::SetColors(CXTPPickBtnArray& arColors, const XTP_PICK_BUTTON* pColors,
									  const int nCount)
{
	ASSERT(pColors != NULL);

	if (pColors == NULL)
		return FALSE;

	arColors.RemoveAll();

	for (int i = 0; i < nCount; i++)
	{
		XTP_PICK_BUTTON btn = { 0 };
		btn.clrButton		= pColors[i].clrButton;
		btn.toolTipID		= pColors[i].toolTipID;

		if (btn.toolTipID != 0)
		{
			CString strTip;
			CXTPResourceManager::AssertValid(
				XTPResourceManager()->LoadString(&strTip, btn.toolTipID));
			STRCPY_S(btn.szTip, _countof(btn.szTip), strTip);
		}
		else if (!::IsBadStringPtr(pColors[i].szTip, 256) && _tcslen(pColors[i].szTip) > 0)
		{
			STRCPY_S(btn.szTip, _countof(btn.szTip), pColors[i].szTip);
		}

		arColors.Add(btn);
	}

	return TRUE;
}

CString CXTPColorSelectorCtrl::GetThemeTip(LPCTSTR lpszColor, LPCTSTR lpszDesc, int nShade)
{
	ASSERT(NULL != lpszColor);

	CString strTip;
	strTip.Format(_T("%s, %s"), lpszColor, lpszDesc);

	if (nShade != 0)
	{
		CString strAdjust;
		strAdjust.LoadString(UINT((nShade > 0) ? XTP_IDS_LIGHTER : XTP_IDS_DARKER));

		CString strColorAdjust;
		strColorAdjust.Format(_T("%s, %s %i%%"), strTip.operator LPCTSTR(),
							  strAdjust.operator LPCTSTR(), abs(nShade));
		return strColorAdjust;
	}

	return strTip;
}

BOOL CXTPColorSelectorCtrl::SetThemeColors(CXTPPickBtnArray& arColors, int nCols /*=10*/,
										   LPCTSTR lpszTitle /*=NULL*/)
{
	m_arThemeColors.RemoveAll();
	m_arThemeColors.Copy(arColors);

	if (lpszTitle != NULL)
		m_strTitleTheme = lpszTitle;

	m_nCols = nCols;
	return (m_arThemeColors.GetSize() > 0);
}

BOOL CXTPColorSelectorCtrl::SetThemeColors(XTP_PICK_BUTTON* pColors, int nCount, LPCTSTR lpszTitle,
										   CStringArray& arStrings, int ppShadeMatrix[6][10])
{
	ASSERT(NULL != pColors);
	ASSERT(nCount == 10);

	for (int r = 0; r < 6; ++r) // 6 rows.
	{
		for (int c = 0; c < nCount; ++c) // 10 columns.
		{
			XTP_PICK_BUTTON btn = { 0 };
			btn.clrButton		= CXTPDrawHelpers::AdjustBrightness(pColors[c].clrButton,
																ppShadeMatrix[r][c]);
			btn.toolTipID		= pColors[c].toolTipID;

			if (btn.toolTipID != 0)
			{
				CString strTip;
				CXTPResourceManager::AssertValid(
					XTPResourceManager()->LoadString(&strTip, btn.toolTipID));
				STRCPY_S(btn.szTip, _countof(btn.szTip),
						 GetThemeTip(strTip, arStrings[c], ppShadeMatrix[r][c]));
			}
			else
			{
				STRCPY_S(btn.szTip, _countof(btn.szTip),
						 GetThemeTip(pColors[c].szTip, arStrings[c], ppShadeMatrix[r][c]));
			}

			m_arThemeColors.Add(btn);
		}
	}

	if (lpszTitle != NULL)
		m_strTitleTheme = lpszTitle;

	m_nCols = nCount;
	return (m_arThemeColors.GetSize() > 0);
}

BOOL CXTPColorSelectorCtrl::SetThemeColors(XTP_PICK_BUTTON* pColors /*=NULL*/, int nCount /*=10*/,
										   LPCTSTR lpszTitle /*=NULL*/, UINT* pStringIDs /*=NULL*/,
										   int ppShadeMatrix[6][10] /*=NULL*/)
{
	if (pColors == NULL)
	{
		pColors = _ThemeColors;
		nCount  = _countof(_ThemeColors);
	}

	ASSERT(nCount == 10);
	if (nCount != 10)
		return FALSE; // size of array must be 10.

	if (pStringIDs == NULL)
		pStringIDs = _nStringIDs;

	if (ppShadeMatrix == NULL)
		ppShadeMatrix = _ShadeMatrix;

	CStringArray arStrings;

	for (int i = 0; i < nCount; ++i)
	{
		CString strDesc;
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strDesc, pStringIDs[i]));
		arStrings.Add(strDesc);
	}

	return SetThemeColors(pColors, nCount, lpszTitle, arStrings, ppShadeMatrix);
}

BOOL CXTPColorSelectorCtrl::SetThemeColors(XTP_PICK_BUTTON* pColors, int nCount, LPCTSTR lpszTitle,
										   LPCTSTR* ppszStrings, int ppShadeMatrix[6][10])
{
	ASSERT(pColors != NULL && nCount == 10);
	if (pColors == NULL || nCount != 10)
		return FALSE;

	ASSERT(ppszStrings != NULL);
	if (ppszStrings == NULL)
		return FALSE; // must be valid.

	ASSERT(ppShadeMatrix != NULL);
	if (ppShadeMatrix == NULL)
		return FALSE; // must be valid.

	CStringArray arStrings;

	for (int i = 0; i < nCount; ++i)
	{
		arStrings.Add(ppszStrings[i]);
	}

	return SetThemeColors(pColors, nCount, lpszTitle, arStrings, ppShadeMatrix);
}

BOOL CXTPColorSelectorCtrl::SetStandardColors(CXTPPickBtnArray& arColors, int nCols /*=10*/,
											  LPCTSTR lpszTitle /*=NULL*/)
{
	m_arStandardColors.RemoveAll();
	m_arStandardColors.Copy(arColors);

	if (lpszTitle != NULL)
		m_strTitleStandard = lpszTitle;

	m_nCols = nCols;
	return (m_arStandardColors.GetSize() > 0);
}

BOOL CXTPColorSelectorCtrl::SetStandardColors(XTP_PICK_BUTTON* pColors /*=NULL*/,
											  int nCount /*=10*/, int nCols /*=10*/,
											  LPCTSTR lpszTitle /*=NULL*/)
{
	if (pColors == NULL)
	{
		pColors = _ThemeStdColors;
		nCount  = _countof(_ThemeStdColors);
	}

	m_nCols = nCols;

	if (lpszTitle != NULL)
		m_strTitleStandard = lpszTitle;

	return SetColors(m_arStandardColors, pColors, nCount);
}

CSize CXTPColorSelectorCtrl::CalcSize()
{
	return m_pTheme->CalcSize(this);
}

BOOL CXTPColorSelectorCtrl::Create(CRect rect, CWnd* pParentWnd, DWORD dwPopup, COLORREF clrColor,
								   COLORREF clrDefault /*= CLR_DEFAULT*/)
{
	m_pParentWnd = pParentWnd;
	ASSERT(m_pParentWnd != NULL); // must be valid.
	if (!m_pParentWnd)
		return FALSE;

	AddListener(m_pParentWnd->m_hWnd);

	m_dwPopup	= dwPopup;
	m_clrColor   = clrColor;
	m_clrDefault = clrDefault;
	m_rcWnd		 = rect;

	if (m_strTitleTheme.IsEmpty())
	{
		CXTPResourceManager::AssertValid(
			XTPResourceManager()->LoadString(&m_strTitleTheme, XTP_IDS_THEME_COLORS));
	}

	if (m_strTitleStandard.IsEmpty())
	{
		CXTPResourceManager::AssertValid(
			XTPResourceManager()->LoadString(&m_strTitleStandard, XTP_IDS_STD_COLORS));
	}

	if (m_strTitleRecent.IsEmpty())
	{
		CXTPResourceManager::AssertValid(
			XTPResourceManager()->LoadString(&m_strTitleRecent, XTP_IDS_RECENT_COLORS));
	}

	CSize sz = CalcSize();

	if (dwPopup & WS_POPUP)
	{
		if (m_dwPopup & CPS_XTP_RIGHTALIGN)
		{
			m_rcWnd.left = m_rcWnd.right - sz.cx;
		}
		else
		{
			m_rcWnd.right = m_rcWnd.left + sz.cx;
		}

		m_rcWnd.top	= rect.bottom;
		m_rcWnd.bottom = m_rcWnd.top + sz.cy;
	}
	else
	{
		rect.right  = rect.left + sz.cx;
		rect.bottom = rect.top + sz.cy;
	}

	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	// call the base class for creation.
	if (!CWnd::CreateEx(NULL, wndClass, _T(""), WS_CHILD, m_rcWnd, m_pParentWnd, 0, NULL))
	{
		TRACE0("Failed to create color popup window.\n");
		return FALSE;
	}

	if ((dwPopup & WS_VISIBLE) && ((m_dwPopup & WS_POPUP) == 0))
	{
		ShowWindow(SW_SHOWNA);
	}

	return TRUE;
}

int CXTPColorSelectorCtrl::OnCreate(LPCREATESTRUCT lpCS)
{
	if (CWnd::OnCreate(lpCS) == -1)
		return -1;

	int nIndex	= -1;
	bool bChecked = false;

	// Create the ToolTip control.
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);

	CXTPClientRect rcClient(this);

	int x  = rcClient.left;
	int y  = rcClient.top;
	int cx = rcClient.Width();

	CRect rect;
	m_pTheme->CreateAutoButton(this, nIndex, x, y, cx, bChecked);
	m_pTheme->CreateColorButtons(this, nIndex, x, y, cx, bChecked);
	m_pTheme->CreateRecentColorButtons(this, nIndex, x, y, cx, bChecked);
	m_pTheme->CreateMoreButton(this, nIndex, x, y, cx, bChecked);

	m_nCurSel = -1;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrl Drawing

BOOL CXTPColorSelectorCtrl::IsColorStatic(CDC* pDC) const
{
	if ((GetStyle() & WS_CHILD) != 0)
	{
		CWnd* pWndParent = GetParent();
		if (::IsWindow(pWndParent->GetSafeHwnd()))
		{
			HBRUSH hBrush = (HBRUSH)pWndParent->SendMessage(WM_CTLCOLORSTATIC,
															(WPARAM)pDC->GetSafeHdc(),
															(LPARAM)m_hWnd);

			return (hBrush != NULL);
		}
	}

	return FALSE;
}

void CXTPColorSelectorCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc);

	m_pTheme->FillBackground(&memDC, this, rc);

	DrawColorSelector(&memDC);
}

void CXTPColorSelectorCtrl::DrawColorSelector(CDC* pDC)
{
	m_pTheme->DrawColorSelector(pDC, this);
}

////////////////////////////////////////////////////////////////////////////////

CXTPColorSelectorCtrl::COLOR_CELL* CXTPColorSelectorCtrl::GetCellFromIndex(int iIndex)
{
	POSITION pos = m_arCells.FindIndex(iIndex);
	if (pos != NULL)
		return m_arCells.GetAt(pos);

	return NULL;
}

int CXTPColorSelectorCtrl::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/,
										   UINT /*message*/)
{
	return GetStyle() & WS_POPUP ? MA_NOACTIVATE : (int)Default();
}

void CXTPColorSelectorCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!IsColorDlgVisible())
	{
		if (m_point != point)
		{
			m_point = point;

			POSITION pos;
			for (pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
			{
				COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
				ASSERT(pColorCell != NULL);
				if (!pColorCell)
					continue;

				CRect rcItem = pColorCell->rect;
				if (rcItem.PtInRect(point))
				{
					SelectColorCell(pColorCell);
					TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd };
					_TrackMouseEvent(&tme);

					return;
				}
			}

			SelectColorCell(NULL);
		}

		CWnd::OnMouseMove(nFlags, point);
	}
}

void CXTPColorSelectorCtrl::OnMouseLeave()
{
	if (!IsColorDlgVisible())
	{
		m_nPressed = m_nCurSel = -1;
		Invalidate(FALSE);
	}
}

void CXTPColorSelectorCtrl::SelectColorCell(COLOR_CELL* pColorCell)
{
	m_nCurSel = pColorCell == NULL ? -1 : (int)pColorCell->nIndex;
	Invalidate(FALSE);
}

// Adds a window to send color picker notifications to.
void CXTPColorSelectorCtrl::AddListener(HWND hwndListener)
{
	m_listeners.Add(hwndListener);
}

void CXTPColorSelectorCtrl::RemoveListener(HWND hwndListener)
{
	int i;
	for (i = (int)m_listeners.GetSize() - 1; i >= 0; i--)
	{
		if (m_listeners[i] == hwndListener)
			m_listeners.RemoveAt(i);
	}
}

void CXTPColorSelectorCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!IsColorDlgVisible())
	{
		CWnd::OnLButtonDown(nFlags, point);

		CRect rc;
		GetClientRect(&rc);

		if (rc.PtInRect(point))
		{
			m_nPressed = m_nCurSel;
			Invalidate(FALSE);
		}
	}
}

void CXTPColorSelectorCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!IsColorDlgVisible())
	{
		CWnd::OnLButtonUp(nFlags, point);

		int nCurSel = m_nPressed == m_nCurSel ? m_nCurSel : -1;

		m_nCurSel = m_nPressed = -1;
		Invalidate(FALSE);

		if (nCurSel != -1)
		{
			EndSelection(nCurSel);
		}
	}
}

void CXTPColorSelectorCtrl::SetCurSel(int nIndex)
{
	if (nIndex < 0)
		nIndex = ((m_dwPopup & CPS_XTP_NOAUTOMATIC) == 0) ? 0 : (int)m_arCells.GetCount() - 1;
	if (nIndex >= m_arCells.GetCount())
		nIndex = (int)m_arCells.GetCount() - 1;

	POSITION pos = m_arCells.FindIndex(nIndex);
	if (pos == NULL)
		return;

	COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
	SelectColorCell(pColorCell);
}

void CXTPColorSelectorCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (IsColorDlgVisible())
		return;

	int nLastIndex = (int)m_arCells.GetCount() - 1;

	//          0
	//
	//  1  1  2  3  4  5  6  7  8
	//  2  9 10 11 12 13 14 15 16
	//  3 17 18 19 20 21 22 23 24
	//  4 25 26 27 28 29 30 31 32
	//
	//         33
	//
	//          0
	//
	//  1  1  2  3  4  5  6  7  8
	//  2  9 10 11 12 13 14 15 16
	//  3 17 18 19 20 21 22 23 24
	//  4 25 26 27 28 29 30 31 32
	//  5 33
	//
	//         34

	switch (nChar)
	{
		case VK_DOWN:
		{
			// Check for no current cursor yet
			if (m_nCurSel == -1)
			{
				m_nCurSel = 0;
			}
			// top button to first color cell
			else if (m_nCurSel == 0 && ((m_dwPopup & CPS_XTP_NOAUTOMATIC) == 0))
			{
				m_nCurSel = 1;
			}
			// bottom button to top bottom
			else if (m_nCurSel == nLastIndex)
			{
				m_nCurSel = 0;
			}
			// a color square to the next color square below it
			else
			{
				m_nCurSel += m_nCols;
			}
			SetCurSel(m_nCurSel);
			break;
		}

		case VK_UP:
		{
			// Check for no current cursor yet
			if (m_nCurSel == -1)
			{
				m_nCurSel = nLastIndex;
			}
			// top button to last button
			else if (m_nCurSel == 0)
			{
				m_nCurSel = nLastIndex;
			}
			// bottom button to first color square in the last row
			else if (m_nCurSel == nLastIndex)
			{
				m_nCurSel = (m_dwPopup & CPS_XTP_MORECOLORS) ? m_nCurSel - 1 : m_nCurSel - m_nCols;
			}
			// a color square to another color square just above it
			else
			{
				m_nCurSel -= m_nCols;
			}

			SetCurSel(m_nCurSel);
			break;
		}

		case VK_RIGHT:
		{
			// Check for no current cursor yet
			if (m_nCurSel == -1)
			{
				m_nCurSel = 0;
			}
			else
			{
				++m_nCurSel;
				if (m_nCurSel > nLastIndex)
				{
					m_nCurSel = 0;
				}
			}
			SetCurSel(m_nCurSel);
			break;
		}

		case VK_LEFT:
		{
			// Check for no current cursor yet
			if (m_nCurSel == -1)
			{
				m_nCurSel = nLastIndex;
			}
			else
			{
				--m_nCurSel;
				if (m_nCurSel < 0)
				{
					m_nCurSel = nLastIndex;
				}
			}
			SetCurSel(m_nCurSel);
			break;
		}

		case VK_ESCAPE:
		{
			EndSelection(-1);
			return;
		}
		case VK_RETURN:
		case VK_SPACE:
		{
			EndSelection(m_nCurSel);
			return;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPColorSelectorCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

void CXTPColorSelectorCtrl::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (::IsWindow(m_hWnd))
		RedrawWindow();
}

BOOL CXTPColorSelectorCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SETTINGCHANGE || message == WM_SYSCOLORCHANGE)
	{
		RefreshMetrics();
	}

	if (::IsWindow(m_tooltip.m_hWnd))
	{
		MSG msg;
		msg.message = message;
		msg.wParam  = wParam;
		msg.lParam  = lParam;
		msg.time	= 0;
		msg.hwnd	= m_hWnd;

		m_tooltip.RelayEvent(&msg);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CXTPColorSelectorCtrl::SetTheme(CXTPColorSelectorCtrlTheme* pTheme)
{
	CMDTARGET_RELEASE(m_pTheme);
	m_pTheme = pTheme;

	RefreshMetrics();

	return (m_pTheme != NULL);
}

BOOL CXTPColorSelectorCtrl::SetTheme(XTPControlTheme eTheme)
{
	BOOL bReturn = FALSE;

	switch (eTheme)
	{
		case xtpControlThemeOfficeXP:
			bReturn = SetTheme(new CXTPColorSelectorCtrlThemeOfficeXP());
			break;

		case xtpControlThemeOffice2003:
			bReturn = SetTheme(new CXTPColorSelectorCtrlThemeOffice2003());
			break;

		case xtpControlThemeOffice2013:
			bReturn = SetTheme(new CXTPColorSelectorCtrlThemeOffice2013());
			break;

		case xtpControlThemeResource:
			bReturn = SetTheme(new CXTPColorSelectorCtrlThemeResource());
			break;

		default: bReturn = SetTheme(new CXTPColorSelectorCtrlTheme()); break;
	}

	return bReturn;
}

LRESULT CXTPColorSelectorCtrl::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

int CXTPColorSelectorCtrl::GetCurSel(COLOR_CELL* pColorCell)
{
	pColorCell = GetCellFromIndex(m_nCurSel);
	ASSERT(pColorCell != NULL);
	return m_nCurSel;
}

void CXTPColorSelectorCtrl::EndSelection(int nCurSel)
{
	OnEndSelection(nCurSel, 0);
}

void CXTPColorSelectorCtrl::OnSetFocus(CWnd* pOldWnd)
{
	if (IsColorDlgVisible())
	{
		m_pColorDlg->SetFocus();
		pOldWnd = this;
	}
	CWnd::OnSetFocus(pOldWnd);
}

BOOL CXTPColorSelectorCtrl::LookUpColor(const COLORREF crFind)
{
	int i;

	// try theme color arrays.
	for (i = 0; i < m_arThemeColors.GetSize(); ++i)
	{
		if (crFind == m_arThemeColors[i].clrButton)
			return TRUE;
	}

	// try standard color arrays.
	for (i = 0; i < m_arStandardColors.GetSize(); ++i)
	{
		if (crFind == m_arStandardColors[i].clrButton)
			return TRUE;
	}

	// if the color was already added, return TRUE.
	if (m_dwPopup & CPS_XTP_RECENTCOLORS)
	{
		for (i = 0; i < m_arRecentColors.GetSize(); ++i)
		{
			if (crFind == m_arRecentColors.GetAt(i))
				return TRUE;
		}
	}

	// if the color is already displayed as a default selection return TRUE.
	if (m_dwPopup & CPS_XTP_EXTENDED)
	{
		for (i = 0; i < _countof(_ExtendedColors); ++i)
		{
			if (crFind == _ExtendedColors[i].clrButton)
				return TRUE;
		}
	}
	else
	{
		for (i = 0; i < _countof(_StandardColors); ++i)
		{
			if (crFind == _StandardColors[i].clrButton)
				return TRUE;
		}
	}

	return FALSE;
}

void CXTPColorSelectorCtrl::OnEndSelection(int nCurSel, LPARAM callerParam)
{
	int nMessage = 0;

	// copy off the listeners
	CArray<HWND, HWND> listeners;
	listeners.Append(m_listeners);

	// copy other relevant data
	COLORREF clrColor = m_clrColor;
	DWORD dwPopup	 = m_dwPopup;

	if (nCurSel == (int)m_arCells.GetCount() - 1 && (m_dwPopup & CPS_XTP_MORECOLORS))
	{
		// display dialog and send proper notifications
		m_pColorDlg = new CXTPColorDialog(
			clrColor, clrColor,
			(dwPopup & (CPS_XTP_SHOWHEXVALUE | CPS_XTP_SHOW3DSELECTION | CPS_XTP_SHOWEYEDROPPER)),
			GetParent());

		if (m_pColorDlg->DoModal() == IDOK)
		{
			COLORREF clrDlgColor = m_pColorDlg->GetColor();

			if (clrColor == clrDlgColor)
			{
				nMessage = CPN_XTP_SELCHANGE;
			}
			else
			{
				nMessage = CPN_XTP_SELENDOK;
				clrColor = clrDlgColor;
			}

			// always maintain recent colors.
			while (m_arRecentColors.GetSize() >= m_nCols)
				m_arRecentColors.RemoveAt(0);

			if (!LookUpColor(clrColor))
				m_arRecentColors.Add(clrColor);
		}
		else
		{
			nMessage = CPN_XTP_SELENDCANCEL;
		}

		SAFE_DELETE(m_pColorDlg);
	}

	else if (nCurSel == 0 && ((m_dwPopup & CPS_XTP_NOAUTOMATIC) == 0))
	{
		if (m_dwPopup & CPS_XTP_NOFILL)
		{
			nMessage = CPN_XTP_SELNOFILL;
			clrColor = (ULONG)-1;
		}
		else if (m_clrColor == m_clrDefault)
		{
			nMessage = CPN_XTP_SELCHANGE;
		}
		else
		{
			nMessage = CPN_XTP_SELENDOK;
			clrColor = m_clrDefault;
		}
	}

	else if (nCurSel == -1)
	{
		nMessage = CPN_XTP_SELENDCANCEL;
	}

	else
	{
		COLOR_CELL* pColorCell = GetCellFromIndex(nCurSel);
		ASSERT(pColorCell != NULL);
		if (!pColorCell)
			return;

		COLORREF clrCellColor = pColorCell->clr;

		if (clrColor == clrCellColor)
		{
			nMessage = CPN_XTP_SELCHANGE;
		}
		else
		{
			nMessage = CPN_XTP_SELENDOK;
			clrColor = clrCellColor;
		}
	}

	// Pass this message to the registered listeners
	int i;
	for (i = 0; i < listeners.GetSize(); i++)
	{
		if (::IsWindow(listeners[i]))
		{
			::SendMessage(listeners[i], XTPToUInt(nMessage), (WPARAM)clrColor, callerParam);
		}
	}
}

BOOL CXTPColorSelectorCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPColorSelectorCtrl::SelectColor(COLORREF clr)
{
	POSITION pos;
	for (pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
	{
		COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
		ASSERT(pColorCell != NULL);
		if (!pColorCell)
			continue;

		if (pColorCell->dwStyle & CPS_XTP_PICKBOX)
			pColorCell->bChecked = (clr == pColorCell->clr);
	}

	m_clrColor = clr;
	Invalidate(FALSE);
}

LRESULT CXTPColorSelectorCtrl::OnNcHitTest(CPoint point)
{
	// If we have a valid tooltip pointer, bring the tooltip control
	// to the top of the Z-order.

	if (m_tooltip.GetSafeHwnd() && !IsColorDlgVisible())
	{
		m_tooltip.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);
	}

	return (LRESULT)CWnd::OnNcHitTest(point);
}

bool CXTPColorSelectorCtrl::IsColorDlgVisible()
{
	return (m_pColorDlg && ::IsWindow(m_pColorDlg->m_hWnd));
}

void AFX_CDECL CXTPColorSelectorCtrl::SetRecentColors(const CUIntArray& arRecentColors,
													  LPCTSTR lpszTitle /*=NULL*/)
{
	m_arRecentColors.RemoveAll();
	m_arRecentColors.Copy(arRecentColors);
	m_strTitleRecent = lpszTitle;
}

BOOL CXTPColorSelectorCtrl::ModifyPopupStyle(DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwPopup = (m_dwPopup & ~dwRemove) | dwAdd;

	if (dwPopup == m_dwPopup)
		return FALSE;

	m_dwPopup = dwPopup;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlTheme

CXTPColorSelectorCtrlTheme::CXTPColorSelectorCtrlTheme()
	: m_nAutoButton(XTP_DPI_Y(22))
	, m_nCaption(XTP_DPI_Y(22))
	, m_nMargin(XTP_DPI_X(4))
	, m_sizeButton(XTP_DPI(CSize(18, 18)))
	, m_crBorderHilight(COLORREF_NULL)
	, m_crBorderPressed(COLORREF_NULL)
	, m_crBack(COLORREF_NULL)
	, m_crBackPressed(COLORREF_NULL)
	, m_crBackHilight(COLORREF_NULL)
	, m_crBackChecked(COLORREF_NULL)
	, m_crText(COLORREF_NULL)
	, m_crTextPressed(COLORREF_NULL)
	, m_crPopupBorder(COLORREF_NULL)
{
	m_bThinBorder = FALSE;
}

void CXTPColorSelectorCtrlTheme::RefreshMetrics(CXTPColorSelectorCtrl* /*pOwner*/)
{
	m_crBack		  = GetXtremeColor(COLOR_3DFACE);
	m_crBorder		  = GetXtremeColor(COLOR_3DSHADOW);
	m_crBorderHilight = GetXtremeColor(COLOR_3DHILIGHT);
	m_crBorderPressed = GetXtremeColor(COLOR_3DSHADOW);
	m_crText		  = GetXtremeColor(COLOR_BTNTEXT);
	m_crPopupBorder   = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);
}

void CXTPColorSelectorCtrlTheme::FillBackground(CDC* pDC, CXTPColorSelectorCtrl* pOwner,
												const CRect& rect)
{
	UNREFERENCED_PARAMETER(pOwner);
	pDC->FillSolidRect(&rect, m_crBack);
}

void CXTPColorSelectorCtrlTheme::DrawColorCell(CDC* pDC, CXTPColorSelectorCtrl* pOwner,
											   CXTPColorSelectorCtrl::COLOR_CELL* pColorCell,
											   BOOL bHilite, BOOL bPressed)
{
	pDC->SetBkMode(TRANSPARENT);

	CRect rcItem = pColorCell->rect;

	bool bChecked = pColorCell->bChecked;

	if (bChecked || bHilite || bPressed)
	{
		if (bPressed)
		{
			if (m_crBackPressed != COLORREF_NULL)
			{
				pDC->FillSolidRect(&rcItem, m_crBackPressed);
			}
		}
		else if (bHilite)
		{
			if (m_crBackHilight != COLORREF_NULL)
			{
				pDC->FillSolidRect(&rcItem, m_crBackHilight);
			}
		}
		else if (bChecked)
		{
			if (m_crBackChecked != COLORREF_NULL)
			{
				pDC->FillSolidRect(&rcItem, m_crBackChecked);
			}
			else
			{
				XTPDrawShadedRect(pDC, rcItem);
			}
		}

		pDC->Draw3dRect(rcItem, (bPressed || bChecked) ? m_crBorderPressed : m_crBorderHilight,
						(bPressed || bChecked) ? m_crBorderHilight : m_crBorderPressed);
	}

	CRect rcText(rcItem);
	int nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
	int nLen	= (int)_tcslen(pColorCell->szText);

	COLORREF crCell = pColorCell->clr;

	if (crCell == COLORREF_NULL)
	{
		crCell = m_crBack;
	}

	COLORREF crBorder = ((pColorCell->dwStyle & CPS_XTP_NOFILL)
						 || ((pOwner->GetPopupStyle() & CPS_XTP_COLORBORDERS) == 0))
							? m_crBorder
							: CXTPDrawHelpers::AdjustBrightness(pColorCell->clr, -25);

	int cxText = pDC->GetTextExtent(pColorCell->szText, nLen).cx;

	if (pColorCell->dwStyle != CPS_XTP_MORECOLORS)
	{
		rcItem.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));

		if ((pColorCell->dwStyle & CPS_XTP_PICKBOX) == 0)
		{
			if ((pColorCell->dwStyle & CPS_XTP_NOFILL) == 0)
			{
				rcItem.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
				rcItem.right  = rcItem.left + XTP_DPI_X(11);
				rcItem.bottom = rcItem.top + XTP_DPI_Y(11);
				rcItem.OffsetRect(XTP_DPI_X(1), 0);

				pDC->FillSolidRect(rcItem, crCell);
				pDC->Draw3dRect(rcItem, crBorder, crBorder);

				if (2 * (rcItem.right - rcText.left) + cxText > rcText.Width())
				{
					rcText.left = rcItem.right + XTP_DPI_X(3);
					nFormat		= DT_SINGLELINE | DT_VCENTER;
				}
			}
		}
		else
		{
			pDC->FillSolidRect(rcItem, crCell);
			pDC->Draw3dRect(rcItem, crBorder, crBorder);
		}
	}
	else
	{
		if (pColorCell->bChecked)
		{
			rcItem = pColorCell->rect;
			rcItem.DeflateRect(XTP_DPI_X(5), XTP_DPI_Y(6));
			rcItem.left = rcItem.right - XTP_DPI_X(12);
			pDC->FillSolidRect(rcItem, crCell);
			pDC->Draw3dRect(rcItem, crBorder, crBorder);
		}
	}

	if (nLen)
	{
		CString strText(pColorCell->szText);
		rcText.DeflateRect(XTP_DPI_X(5), 0);

		if (cxText > rcText.Width())
		{
			strText.Replace(_T("..."), _T(""));
			nFormat |= DT_END_ELLIPSIS;
		}

		pDC->SetTextColor(bPressed ? m_crTextPressed : m_crText);
		pDC->DrawText(strText, nLen, rcText, XTPToUInt(nFormat));
	}
}

void CXTPColorSelectorCtrlTheme::DrawColorSelector(CDC* pDC, CXTPColorSelectorCtrl* pOwner)
{
	ASSERT_VALID(pDC);

	pDC->SetBkMode(TRANSPARENT);

	CXTPClientRect rcClient(pOwner);
	CXTPFontDC fontDC(pDC, &XTPFontManager()->GetFont());

	if (pOwner->HasCaptions())
	{
		if (pOwner->GetThemeColors().GetSize() > 0 || pOwner->GetStandardColors().GetSize() > 0)
		{
			if (pOwner->GetThemeColors().GetSize())
			{
				pDC->DrawText(pOwner->GetThemeTitle(), &m_rcThemeText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
			if (pOwner->GetStandardColors().GetSize())
			{
				pDC->DrawText(pOwner->GetStandardTitle(), &m_rcStandardText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
		}
		else
		{
			if (!pOwner->GetCustomTitle().IsEmpty())
			{
				pDC->DrawText(pOwner->GetCustomTitle(), &m_rcStandardText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
			else if ((pOwner->GetPopupStyle() & CPS_XTP_EXTENDED) == 0)
			{
				pDC->DrawText(pOwner->GetStandardTitle(), &m_rcStandardText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
		}

		if (pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS
			&& pOwner->GetPopupStyle() & CPS_XTP_RECENTCOLORS)
		{
			if (pOwner->m_arRecentColors.GetSize() > 0)
			{
				pDC->DrawText(CXTPColorSelectorCtrl::m_strTitleRecent, &m_rcRecentText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
		}
	}

	POSITION pos;
	int nIndex = 0;
	for (pos = pOwner->GetColorCellList().GetHeadPosition(); pos;)
	{
		CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = pOwner->GetColorCellList().GetNext(pos);
		ASSERT(pColorCell != NULL);

		BOOL bHighlighted = (((nIndex == pOwner->GetCurSel()) && (pOwner->GetPressed() == -1))
							 || (nIndex == pOwner->GetPressed()));
		BOOL bPressed	 = ((nIndex == pOwner->GetPressed()) && (nIndex == pOwner->GetCurSel()));

		DrawColorCell(pDC, pOwner, pColorCell, bHighlighted, bPressed);
		nIndex++;
	}
}

AFX_STATIC int CalcHeight(int nColors, int nCols, int cyButton)
{
	ASSERT(0 != nCols);
	return (((nColors + nCols) - 1) / nCols) * cyButton;
}

CSize CXTPColorSelectorCtrlTheme::CalcSize(CXTPColorSelectorCtrl* pOwner)
{
	ASSERT_VALID(pOwner);

	CSize sz;

	// determine the starting width and height.
	sz.cx = pOwner->GetBorderRect().left + (m_sizeButton.cx * pOwner->GetNumCols())
			+ pOwner->GetBorderRect().right;
	sz.cy = pOwner->GetBorderRect().top + pOwner->GetBorderRect().bottom;

	// determine the size of the "Automatic" button.
	if ((pOwner->GetPopupStyle() & CPS_XTP_NOAUTOMATIC) == 0)
	{
		sz.cy += m_nAutoButton;
	}

	// if theme colors are used, calcuate sizes.
	if (pOwner->GetThemeColors().GetSize() > 0 || pOwner->GetStandardColors().GetSize() > 0)
	{
		// determine the size of the theme color section.
		if (pOwner->GetThemeColors().GetSize())
		{
			sz.cy += CalcHeight(static_cast<int>(pOwner->GetThemeColors().GetSize()),
								pOwner->GetNumCols(), m_sizeButton.cy);

			if (pOwner->HasCaptions())
			{
				sz.cy += m_nCaption;
			}
			else
			{
				sz.cy += m_nMargin;
			}
		}

		// determine the size of the standard button section.
		if (pOwner->GetStandardColors().GetSize())
		{
			sz.cy += CalcHeight(static_cast<int>(pOwner->GetStandardColors().GetSize()),
								pOwner->GetNumCols(), m_sizeButton.cy);

			if (pOwner->HasCaptions())
			{
				sz.cy += m_nCaption;
			}
			else
			{
				sz.cy += m_nMargin;
			}
		}
	}
	else
	{
		// check to see if custom colors are used, if so set extended style.
		if (pOwner->GetExtendedColors())
		{
			pOwner->ModifyPopupStyle(0, CPS_XTP_EXTENDED);
			ASSERT(pOwner->GetExtendedCount() > 0);
		}

		// determine the size of the extended color section (custom colors or 40 color block).
		if (pOwner->GetPopupStyle() & CPS_XTP_EXTENDED)
		{
			int nColors = (pOwner->GetExtendedColors() == NULL)
							  ? XTPToIntChecked(_countof(_ExtendedColors))
							  : pOwner->GetExtendedCount();
			sz.cy += CalcHeight(nColors, pOwner->GetNumCols(), m_sizeButton.cy);

			if (pOwner->HasCaptions() && !pOwner->GetCustomTitle().IsEmpty())
			{
				sz.cy += m_nCaption;
			}
			else
			{
				sz.cy += m_nMargin;
			}
		}

		// determine the size of the 16 color section.
		else
		{
			int nColors = _countof(_StandardColors);
			sz.cy += CalcHeight(nColors, pOwner->GetNumCols(), m_sizeButton.cy);

			if (pOwner->HasCaptions())
			{
				sz.cy += m_nCaption;
			}
			else
			{
				sz.cy += m_nMargin;
			}
		}
	}

	// determine the size of the recent colors section.
	if ((pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS) != 0
		&& (pOwner->GetPopupStyle() & CPS_XTP_RECENTCOLORS) != 0)
	{
		if (CXTPColorSelectorCtrl::m_arRecentColors.GetSize() > 0)
		{
			int nColors = min(static_cast<int>(CXTPColorSelectorCtrl::m_arRecentColors.GetSize()),
							  pOwner->GetNumCols());
			sz.cy += CalcHeight(nColors, pOwner->GetNumCols(), m_sizeButton.cy);

			if (pOwner->HasCaptions())
			{
				sz.cy += m_nCaption;
			}
			else
			{
				sz.cy += m_nMargin;
			}
		}
	}

	// determine the size of the "More Colors" button.
	if (pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS)
	{
		sz.cy += m_nAutoButton + m_nMargin;
	}

	return sz;
}

BOOL CXTPColorSelectorCtrlTheme::CreateAutoButton(CXTPColorSelectorCtrl* pOwner, int& nIndex,
												  const int x, int& y, const int cx, bool& bChecked)
{
	ASSERT_VALID(pOwner);

	if (pOwner->GetPopupStyle() & CPS_XTP_NOAUTOMATIC)
		return FALSE;

	y += pOwner->GetBorderRect().top;

	int nResID = (pOwner->GetPopupStyle() & CPS_XTP_NOFILL) ? XTP_IDS_CLR_NOFILL
															: XTP_IDS_CLR_AUTOMATIC;

	CString strText, strColorToolTip;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strText, XTPToUInt(nResID)));

	int nTipIndex = strText.Find(_T('\n'), 0);
	if (nTipIndex > 0)
	{
		strColorToolTip = strText.Mid(nTipIndex + 1);
		strText.ReleaseBuffer(nTipIndex);
	}
	else
	{
		strColorToolTip = strText;
	}

	nIndex++;

	// Create the automatic color button.
	CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
	STRCPY_S(pColorCell->szText, _countof(pColorCell->szText), strText);

	pColorCell->nID		 = XTPToUInt(nResID);
	pColorCell->rect	 = CRect(x + pOwner->GetBorderRect().left, y,
							 cx - pOwner->GetBorderRect().right, y + m_nAutoButton);
	pColorCell->nIndex   = XTPToUIntChecked(nIndex);
	pColorCell->bChecked = false;
	pColorCell->dwStyle  = pOwner->GetPopupStyle();
	pColorCell->clr		 = pOwner->GetDefColor();
	pColorCell->rcBorder = CRect(1, 1, 1, 1);

	pOwner->GetColorCellList().AddTail(pColorCell);
	pOwner->GetToolTipCtrl().AddTool(pOwner, strColorToolTip, pColorCell->rect,
									 XTPToUIntPtr(nResID));

	if (pOwner->GetPopupStyle() & CPS_XTP_NOFILL)
	{
		if (pOwner->GetCurColor() == COLORREF_NULL)
		{
			bChecked			 = true;
			pColorCell->bChecked = true;
		}
	}
	else
	{
		if (pOwner->GetCurColor() == pOwner->GetDefColor())
		{
			bChecked			 = true;
			pColorCell->bChecked = true;
		}
	}

	y += m_nAutoButton;

	return TRUE;
}

BOOL CXTPColorSelectorCtrlTheme::CreateExtendedButtons(CXTPColorSelectorCtrl* pOwner, int& nIndex,
													   const int x, int& y, const int cx,
													   bool& bChecked)
{
	ASSERT_VALID(pOwner);

	// Create the color pick buttons.
	XTP_PICK_BUTTON* pickBtn = (pOwner->GetPopupStyle() & CPS_XTP_EXTENDED)
								   ? (pOwner->GetExtendedColors() ? pOwner->GetExtendedColors()
																  : _ExtendedColors)
								   : _StandardColors;
	int nColors = (pOwner->GetPopupStyle() & CPS_XTP_EXTENDED)
					  ? (pOwner->GetExtendedColors() ? pOwner->GetExtendedCount()
													 : XTPToIntChecked(_countof(_ExtendedColors)))
					  : XTPToIntChecked(_countof(_StandardColors));

	if (pOwner->HasCaptions()
		&& ((pOwner->GetPopupStyle() & CPS_XTP_EXTENDED) == 0
			|| !pOwner->GetCustomTitle().IsEmpty()))
	{
		// set standard text rect.
		m_rcStandardText.left   = x + pOwner->GetBorderRect().left;
		m_rcStandardText.top	= y;
		m_rcStandardText.bottom = y + m_nCaption;
		m_rcStandardText.right  = cx - pOwner->GetBorderRect().right;

		y += m_nCaption;
	}
	else
	{
		y += m_nMargin;
	}

	CRect rcButton(x + pOwner->GetBorderRect().left, y,
				   x + pOwner->GetBorderRect().left + m_sizeButton.cx, y + m_sizeButton.cy);

	for (int i = 0; i < nColors; i++)
	{
		if (i > 0 && ((i % pOwner->GetNumCols()) == 0))
		{
			y += m_sizeButton.cy;
			rcButton = CRect(x + pOwner->GetBorderRect().left, y,
							 x + pOwner->GetBorderRect().left + m_sizeButton.cx,
							 y + m_sizeButton.cy);
		}
		nIndex++;

		COLORREF clrColor = pickBtn[i].clrButton;

		CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
		pColorCell->szText[0]						  = 0;
		pColorCell->nID								  = pickBtn[i].toolTipID;
		pColorCell->rect							  = rcButton;
		pColorCell->nIndex							  = XTPToUIntChecked(nIndex);
		pColorCell->bChecked						  = false;
		pColorCell->dwStyle							  = CPS_XTP_PICKBOX;
		pColorCell->clr								  = clrColor;
		pColorCell->rcBorder						  = CRect(1, 1, 1, 1);

		pOwner->GetColorCellList().AddTail(pColorCell);

		if (pickBtn[i].toolTipID != 0)
		{
			CString strTip;
			CXTPResourceManager::AssertValid(
				XTPResourceManager()->LoadString(&strTip, pickBtn[i].toolTipID));
			pOwner->GetToolTipCtrl().AddTool(pOwner, strTip, pColorCell->rect,
											 pickBtn[i].toolTipID);
		}
		else if (!::IsBadStringPtr(pickBtn[i].szTip, 256) && _tcslen(pickBtn[i].szTip) > 0)
		{
			pOwner->GetToolTipCtrl().AddTool(pOwner, pickBtn[i].szTip, pColorCell->rect, 1);
		}

		if ((pOwner->GetCurColor() == clrColor) && !bChecked)
		{
			bChecked			 = true;
			pColorCell->bChecked = true;
			pOwner->SetCurSel(XTPToIntChecked(pColorCell->nIndex));
		}

		// offset to next column.
		rcButton.OffsetRect(m_sizeButton.cx, 0);
	}

	y += m_sizeButton.cy;

	return TRUE;
}

BOOL CXTPColorSelectorCtrlTheme::CreateThemeButtons(CXTPColorSelectorCtrl* pOwner, int& nIndex,
													const int x, int& y, const int cx,
													bool& bChecked)
{
	ASSERT_VALID(pOwner);

	if (pOwner->GetThemeColors().GetSize())
	{
		if (pOwner->HasCaptions())
		{
			// set theme text rect.
			m_rcThemeText.left   = x + pOwner->GetBorderRect().left;
			m_rcThemeText.top	= y;
			m_rcThemeText.bottom = y + m_nCaption;
			m_rcThemeText.right  = cx - pOwner->GetBorderRect().right;

			y += m_nCaption;
		}
		else
		{
			y += m_nMargin;
		}

		CRect rcButton(x + pOwner->GetBorderRect().left, y,
					   x + pOwner->GetBorderRect().left + m_sizeButton.cx, y + m_sizeButton.cy);

		for (int i = 0; i < pOwner->GetThemeColors().GetSize(); i++)
		{
			nIndex++;

			if (i > 0 && ((i % pOwner->GetNumCols()) == 0))
			{
				y += m_sizeButton.cy;
				rcButton = CRect(x + pOwner->GetBorderRect().left, y,
								 x + pOwner->GetBorderRect().left + m_sizeButton.cx,
								 y + m_sizeButton.cy);
			}

			COLORREF clrColor = pOwner->GetThemeColors()[i].clrButton;

			CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
			pColorCell->szText[0]						  = 0;
			pColorCell->nID								  = pOwner->GetThemeColors()[i].toolTipID;
			pColorCell->rect							  = rcButton;
			pColorCell->nIndex							  = XTPToUIntChecked(nIndex);
			pColorCell->bChecked						  = false;
			pColorCell->dwStyle							  = CPS_XTP_PICKBOX;
			pColorCell->clr								  = clrColor;
			pColorCell->rcBorder						  = CRect(1, 1, 1, 1);

			pOwner->GetColorCellList().AddTail(pColorCell);
			pOwner->GetToolTipCtrl().AddTool(pOwner, pOwner->GetThemeColors()[i].szTip,
											 pColorCell->rect, 1);

			if ((pOwner->GetCurColor() == clrColor) && !bChecked)
			{
				bChecked			 = true;
				pColorCell->bChecked = true;
				pOwner->SetCurSel(XTPToIntChecked(pColorCell->nIndex));
			}

			// offset to next column.
			rcButton.OffsetRect(m_sizeButton.cx, 0);
		}

		y += m_sizeButton.cy;
	}

	if (pOwner->GetStandardColors().GetSize())
	{
		if (pOwner->HasCaptions())
		{
			// set standard text rect.
			m_rcStandardText.left   = x + pOwner->GetBorderRect().left;
			m_rcStandardText.top	= y;
			m_rcStandardText.bottom = y + m_nCaption;
			m_rcStandardText.right  = cx - pOwner->GetBorderRect().right;

			y += m_nCaption;
		}
		else
		{
			y += m_nMargin;
		}

		CRect rcButton(x + pOwner->GetBorderRect().left, y,
					   x + pOwner->GetBorderRect().left + m_sizeButton.cx, y + m_sizeButton.cy);

		for (int i = 0; i < pOwner->GetStandardColors().GetSize(); i++)
		{
			nIndex++;

			if (i > 0 && ((i % pOwner->GetNumCols()) == 0))
			{
				y += m_sizeButton.cy;
				rcButton = CRect(x + pOwner->GetBorderRect().left, y,
								 x + pOwner->GetBorderRect().left + m_sizeButton.cx,
								 y + m_sizeButton.cy);
			}

			COLORREF clrColor = pOwner->GetStandardColors()[i].clrButton;

			CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
			pColorCell->szText[0]						  = 0;
			pColorCell->nID		 = pOwner->GetStandardColors()[i].toolTipID;
			pColorCell->rect	 = rcButton;
			pColorCell->nIndex   = XTPToUIntChecked(nIndex);
			pColorCell->bChecked = false;
			pColorCell->dwStyle  = CPS_XTP_PICKBOX;
			pColorCell->clr		 = clrColor;
			pColorCell->rcBorder = CRect(1, 1, 1, 1);

			pOwner->GetColorCellList().AddTail(pColorCell);
			pOwner->GetToolTipCtrl().AddTool(pOwner, pOwner->GetStandardColors()[i].szTip,
											 pColorCell->rect, 1);

			if ((pOwner->GetCurColor() == clrColor) && !bChecked)
			{
				bChecked			 = true;
				pColorCell->bChecked = true;
				pOwner->SetCurSel(XTPToIntChecked(pColorCell->nIndex));
			}

			// offset to next column.
			rcButton.OffsetRect(m_sizeButton.cx, 0);
		}

		y += m_sizeButton.cy;
	}

	return TRUE;
}

BOOL CXTPColorSelectorCtrlTheme::CreateColorButtons(CXTPColorSelectorCtrl* pOwner, int& nIndex,
													const int x, int& y, const int cx,
													bool& bChecked)
{
	ASSERT_VALID(pOwner);

	if (pOwner->GetThemeColors().GetSize() > 0 || pOwner->GetStandardColors().GetSize() > 0)
	{
		return CreateThemeButtons(pOwner, nIndex, x, y, cx, bChecked);
	}
	else
	{
		return CreateExtendedButtons(pOwner, nIndex, x, y, cx, bChecked);
	}
}

BOOL CXTPColorSelectorCtrlTheme::CreateRecentColorButtons(CXTPColorSelectorCtrl* pOwner,
														  int& nIndex, const int x, int& y,
														  const int cx, bool& bChecked)
{
	ASSERT_VALID(pOwner);

	if ((pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS) == 0
		|| (pOwner->GetPopupStyle() & CPS_XTP_RECENTCOLORS) == 0)
		return FALSE;

	if (pOwner->m_arRecentColors.GetSize() == 0)
		return FALSE;

	int nColors = static_cast<int>(pOwner->m_arRecentColors.GetSize());

	if (nColors == 0)
		return FALSE;

	if (pOwner->HasCaptions())
	{
		// set recent text rect.
		m_rcRecentText.left   = x + pOwner->GetBorderRect().left;
		m_rcRecentText.top	= y;
		m_rcRecentText.bottom = y + m_nCaption;
		m_rcRecentText.right  = cx - pOwner->GetBorderRect().right;

		y += m_nCaption;
	}
	else
	{
		y += m_nMargin;
	}

	CRect rcButton(x + pOwner->GetBorderRect().left, y,
				   x + pOwner->GetBorderRect().left + m_sizeButton.cx, y + m_sizeButton.cy);

	for (int i = 0; i < nColors; i++)
	{
		if (i > 0 && ((i % pOwner->GetNumCols()) == 0))
		{
			y += m_sizeButton.cy;
			rcButton = CRect(x + pOwner->GetBorderRect().left, y,
							 x + pOwner->GetBorderRect().left + m_sizeButton.cx,
							 y + m_sizeButton.cy);
		}
		nIndex++;

		COLORREF clrColor = pOwner->m_arRecentColors.GetAt(i);

		CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
		pColorCell->szText[0]						  = 0;
		pColorCell->nID								  = 0;
		pColorCell->rect							  = rcButton;
		pColorCell->nIndex							  = XTPToUIntChecked(nIndex);
		pColorCell->bChecked						  = false;
		pColorCell->dwStyle							  = CPS_XTP_PICKBOX;
		pColorCell->clr								  = clrColor;
		pColorCell->rcBorder						  = CRect(1, 1, 1, 1);

		pOwner->GetColorCellList().AddTail(pColorCell);

		CString strColor;
		strColor.Format(_T("R:%d, G:%d, B:%d"), GetRValue(clrColor), GetGValue(clrColor),
						GetBValue(clrColor));
		pOwner->GetToolTipCtrl().AddTool(pOwner,
										 (clrColor == COLORREF_NULL) ? CString(_T("Null"))
																	 : strColor,
										 pColorCell->rect, 1);

		if (pOwner->GetCurColor() == clrColor && !bChecked)
		{
			bChecked			 = true;
			pColorCell->bChecked = true;
			pOwner->SetCurSel(XTPToIntChecked(pColorCell->nIndex));
		}

		// offset to next column.
		rcButton.OffsetRect(m_sizeButton.cx, 0);
	}

	y += m_sizeButton.cy;
	return TRUE;
}

BOOL CXTPColorSelectorCtrlTheme::CreateMoreButton(CXTPColorSelectorCtrl* pOwner, int& nIndex,
												  const int x, int& y, const int cx, bool& bChecked)
{
	ASSERT_VALID(pOwner);

	if ((pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS) == 0)
		return FALSE;

	y += m_nMargin;

	CString strText;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strText, XTP_IDS_CLR_MORE));

	nIndex++;
	CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
	STRCPY_S(pColorCell->szText, _countof(pColorCell->szText), strText);

	pColorCell->nID		 = XTP_IDS_CLR_MORE;
	pColorCell->rect	 = CRect(x + pOwner->GetBorderRect().left, y,
							 cx - pOwner->GetBorderRect().right, y + m_nAutoButton);
	pColorCell->nIndex   = XTPToUIntChecked(nIndex);
	pColorCell->bChecked = false;
	pColorCell->dwStyle  = CPS_XTP_MORECOLORS;
	pColorCell->clr		 = pOwner->GetCurColor();
	pColorCell->rcBorder = CRect(1, 1, 1, 1);

	pOwner->GetColorCellList().AddTail(pColorCell);
	pOwner->GetToolTipCtrl().AddTool(pOwner, strText, pColorCell->rect, 1);

	if (pOwner->GetCurColor() != COLORREF_NULL)
	{
		if (bChecked == false)
		{
			bChecked			 = true;
			pColorCell->bChecked = true;
		}
	}

	return TRUE;
}

CSize CXTPColorSelectorCtrlTheme::GetButtonSize() const
{
	return m_sizeButton;
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeOfficeXP

CXTPColorSelectorCtrlThemeOfficeXP::CXTPColorSelectorCtrlThemeOfficeXP()
{
	m_nTheme	  = xtpControlThemeOfficeXP;
	m_bThinBorder = TRUE;
}

void CXTPColorSelectorCtrlThemeOfficeXP::RefreshMetrics(CXTPColorSelectorCtrl* pOwner)
{
	CXTPColorSelectorCtrlTheme::RefreshMetrics(pOwner);

	m_crBorder		  = GetXtremeColor(XPCOLOR_3DSHADOW);
	m_crBorderHilight = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
	m_crBorderPressed = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
	m_crBack		  = GetXtremeColor(XPCOLOR_MENUBAR_FACE);
	m_crBackPressed   = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
	m_crBackHilight   = GetXtremeColor(XPCOLOR_HIGHLIGHT);
	m_crBackChecked   = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED);
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeOffice2003

CXTPColorSelectorCtrlThemeOffice2003::CXTPColorSelectorCtrlThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
}

void CXTPColorSelectorCtrlThemeOffice2003::RefreshMetrics(CXTPColorSelectorCtrl* pOwner)
{
	CXTPColorSelectorCtrlThemeOfficeXP::RefreshMetrics(pOwner);

	m_crBorder		  = GetMSO2003Color(XPCOLOR_3DSHADOW);
	m_crBorderHilight = GetMSO2003Color(XPCOLOR_HIGHLIGHT_BORDER);
	m_crBorderPressed = GetMSO2003Color(XPCOLOR_HIGHLIGHT_BORDER);
	m_crBack		  = GetMSO2003Color(XPCOLOR_MENUBAR_FACE);
	m_crBackPressed   = GetMSO2003Color(XPCOLOR_HIGHLIGHT_PUSHED);
	m_crBackHilight   = GetMSO2003Color(XPCOLOR_HIGHLIGHT);
	m_crBackChecked   = GetMSO2003Color(XPCOLOR_HIGHLIGHT_CHECKED);
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeResource

CXTPColorSelectorCtrlThemeResource::CXTPColorSelectorCtrlThemeResource()
{
	m_nTheme = xtpControlThemeResource;
}

void CXTPColorSelectorCtrlThemeResource::RefreshMetrics(CXTPColorSelectorCtrl* pOwner)
{
	CXTPColorSelectorCtrlThemeOfficeXP::RefreshMetrics(pOwner);

	CXTPResourceImages* pImages = XTPResourceImages();

	m_crBack		  = RGB(250, 250, 250);
	m_crBackPressed   = pImages->GetImageColor(_T("Window"), _T("HighlightPressed"));
	m_crBackHilight   = pImages->GetImageColor(_T("Window"), _T("HighlightSelected"));
	m_crBackChecked   = pImages->GetImageColor(_T("Window"), _T("HighlightChecked"));
	m_crText		  = 0;
	m_crPopupBorder   = pImages->GetImageColor(_T("Window"), _T("WindowFrame"));
	m_crBorderPressed = m_crBorderHilight = m_crPopupBorder;
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeOffice2013

CXTPColorSelectorCtrlThemeOffice2013::CXTPColorSelectorCtrlThemeOffice2013()
{
	m_sizeButton  = XTPSystemMetrics()->GetSmallIconSize();
	m_nMargin	 = XTP_DPI_Y(2);
	m_nCaption	= XTP_DPI_Y(30);
	m_nTheme	  = xtpControlThemeOffice2013;
	m_bThinBorder = TRUE;
}

void CXTPColorSelectorCtrlThemeOffice2013::RefreshMetrics(CXTPColorSelectorCtrl* pOwner)
{
	CXTPColorSelectorCtrlTheme::RefreshMetrics(pOwner);

	m_crBorder		  = XTPIniColor(_T("Controls.ColorPicker"), _T("Border"), RGB(214, 207, 206));
	m_crBorderHilight = XTPIniColor(_T("Controls.ColorPicker"), _T("BorderHighlight"),
									RGB(242, 148, 54));
	m_crBorderPressed = XTPIniColor(_T("Controls.ColorPicker"), _T("BorderPressed"),
									RGB(239, 72, 16));
	m_crBorderInside  = XTPIniColor(_T("Controls.ColorPicker"), _T("BorderInside"),
									RGB(255, 226, 148));
	m_crBack		  = XTPIniColor(_T("Controls.ColorPicker"), _T("Back"), RGB(255, 255, 255));
	m_crBackPressed   = XTPIniColor(_T("Controls.ColorPicker"), _T("BackPressed"),
									RGB(163, 189, 227));
	m_crBackHilight   = XTPIniColor(_T("Controls.ColorPicker"), _T("BackHighlight"),
									RGB(213, 225, 242));
	m_crBackChecked   = XTPIniColor(_T("Controls.ColorPicker"), _T("BackChecked"),
									RGB(194, 213, 242));
	m_crPopupBorder   = XTPIniColor(_T("Controls.ColorPicker"), _T("PopupBorder"),
									RGB(198, 198, 198));
	m_crText		  = XTPIniColor(_T("Controls.ColorPicker"), _T("Text"), RGB(93, 93, 93));
	m_crTextPressed   = XTPIniColor(_T("Controls.ColorPicker"), _T("TextPressed"), RGB(68, 68, 68));
	m_crTitleText = XTPIniColor(_T("Controls.ColorPicker"), _T("TitleText"), RGB(119, 119, 119));
	m_crTitleBack = XTPIniColor(_T("Controls.ColorPicker"), _T("TitleBack"), RGB(238, 238, 238));
}

void CXTPColorSelectorCtrlThemeOffice2013::DrawBorder(CDC* pDC, CRect rcItem, CRect rcBorder,
													  COLORREF crBorder)
{
	if (rcBorder.left == 1)
	{
		pDC->FillSolidRect(rcItem.left, rcItem.top, XTP_DPI_X(1), rcItem.Height(), crBorder);
	}
	if (rcBorder.top == 1)
	{
		pDC->FillSolidRect(rcItem.left + XTP_DPI_X(1), rcItem.top, rcItem.Width() - XTP_DPI_X(2),
						   XTP_DPI_Y(1), crBorder);
	}
	if (rcBorder.right == 1)
	{
		pDC->FillSolidRect(rcItem.right - XTP_DPI_X(1), rcItem.top, XTP_DPI_X(1), rcItem.Height(),
						   crBorder);
	}
	if (rcBorder.bottom == 1)
	{
		pDC->FillSolidRect(rcItem.left + XTP_DPI_X(1), rcItem.bottom - XTP_DPI_Y(1),
						   rcItem.Width() - XTP_DPI_X(2), XTP_DPI_Y(1), crBorder);
	}
}

void CXTPColorSelectorCtrlThemeOffice2013::DrawColor(CDC* pDC, CXTPColorSelectorCtrl* pOwner,
													 CXTPColorSelectorCtrl::COLOR_CELL* pColorCell,
													 CRect rcItem, BOOL bChecked, BOOL bHilite,
													 BOOL bPressed)
{
	ASSERT_VALID(pDC);
	ASSERT(NULL != pColorCell);

	pDC->FillSolidRect(rcItem, (pColorCell->dwStyle & CPS_XTP_NOFILL) ? m_crBack : pColorCell->clr);

	if (bChecked || bHilite || bPressed)
	{
		CRect rcBorder(1, 1, 1, 1);

		if (bPressed)
		{
			DrawBorder(pDC, rcItem, rcBorder, m_crBorderPressed);
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcItem, rcBorder, m_crBorderInside);
		}
		else if (bHilite)
		{
			DrawBorder(pDC, rcItem, rcBorder, m_crBorderHilight);
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcItem, rcBorder, m_crBorderInside);
		}
		else if (bChecked)
		{
			DrawBorder(pDC, rcItem, rcBorder, m_crBorderPressed);
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcItem, rcBorder, m_crBorderInside);
		}
	}
	else
	{
		COLORREF crBorder = ((pColorCell->dwStyle & CPS_XTP_NOFILL)
							 || ((pOwner->GetPopupStyle() & CPS_XTP_COLORBORDERS) == 0))
								? m_crBorder
								: CXTPDrawHelpers::AdjustBrightness(pColorCell->clr, -15);

		DrawBorder(pDC, rcItem, pColorCell->rcBorder, crBorder);
	}
}

void CXTPColorSelectorCtrlThemeOffice2013::DrawColorCell(
	CDC* pDC, CXTPColorSelectorCtrl* pOwner, CXTPColorSelectorCtrl::COLOR_CELL* pColorCell,
	BOOL bHilite, BOOL bPressed)
{
	ASSERT_VALID(pDC);
	ASSERT(NULL != pColorCell);

	pDC->SetBkMode(TRANSPARENT);

	CRect rcItem(pColorCell->rect);
	BOOL bChecked = pColorCell->bChecked;

	COLORREF crCell = pColorCell->clr;
	if (crCell == COLORREF_NULL)
	{
		crCell = m_crBack;
	}

	// Either "More Colors" or "Automatic Color" button.
	if (pColorCell->dwStyle != CPS_XTP_PICKBOX)
	{
		if (bChecked || bHilite || bPressed)
		{
			CRect rcState(rcItem);
			rcState.InflateRect(XTP_DPI_X(2), 0, XTP_DPI_Y(2), XTP_DPI_X(2));

			if (bPressed)
			{
				pDC->FillSolidRect(rcState, m_crBackPressed);
			}
			else if (bHilite)
			{
				pDC->FillSolidRect(rcState, m_crBackHilight);
			}
		}

		int nLen = static_cast<int>(_tcslen(pColorCell->szText));
		ASSERT(nLen > 0);

		if (nLen > 0)
		{
			CRect rcText(rcItem);
			rcText.InflateRect(XTP_DPI_X(-25), 0, XTP_DPI_Y(-10), XTP_DPI_Y(2));

			CString strText(pColorCell->szText);
			rcText.DeflateRect(XTP_DPI_X(5), 0);

			DWORD dwFormat = DT_SINGLELINE | DT_VCENTER;
			if (pDC->GetTextExtent(pColorCell->szText, nLen).cx > rcText.Width())
			{
				strText.Replace(_T("..."), _T(""));
				dwFormat |= DT_END_ELLIPSIS;
			}

			pDC->SetTextColor(bPressed ? m_crTextPressed : m_crText);
			pDC->DrawText(strText, nLen, rcText, dwFormat);
		}
	}

	if (pColorCell->dwStyle == CPS_XTP_PICKBOX)
	{
		rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(1), XTP_DPI_Y(1));
		DrawColor(pDC, pOwner, pColorCell, rcItem, bChecked, bHilite, bPressed);
	}
	else
	{
		// determine color block size.
		CRect rcColor(rcItem);
		rcColor.OffsetRect(XTP_DPI_X(3), XTP_DPI_Y(5));
		rcColor.right  = rcColor.left + XTP_DPI_X(14);
		rcColor.bottom = rcColor.top + XTP_DPI_Y(14);

		if (pColorCell->dwStyle == CPS_XTP_MORECOLORS)
		{
			// draw separator line.
			CRect rcLine(rcItem);
			rcLine.OffsetRect(0, -m_nMargin);
			rcLine.bottom = rcLine.top + XTP_DPI_Y(1);
			rcLine.left -= XTP_DPI_X(3);
			rcLine.right += XTP_DPI_X(3);
			pDC->FillSolidRect(rcLine, m_crTitleBack);

			// draw color block.
			if (bChecked)
			{
				DrawColor(pDC, pOwner, pColorCell, rcColor, FALSE, FALSE, FALSE);
			}
			// draw "More Colors" icon.
			else
			{
				rcColor.InflateRect(0, 0, XTP_DPI_X(1), XTP_DPI_Y(1));
				pDC->FillSolidRect(rcColor, m_crBack);

				// determine the color block size.
				CRect rcSquare(rcColor);
				rcSquare.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
				rcSquare.right  = rcSquare.left + XTP_DPI_X(6);
				rcSquare.bottom = rcSquare.top + XTP_DPI_Y(6);
				pDC->FillSolidRect(rcSquare, RGB(0xEF, 0xC3, 0x84));

				rcSquare.OffsetRect(XTP_DPI_Y(7), 0);
				pDC->FillSolidRect(rcSquare, RGB(0x4A, 0x82, 0xBD));

				rcSquare.OffsetRect(0, XTP_DPI_Y(7));
				pDC->FillSolidRect(rcSquare, RGB(0xDE, 0x61, 0x42));

				rcSquare.OffsetRect(XTP_DPI_Y(-7), 0);
				pDC->FillSolidRect(rcSquare, RGB(0x73, 0xA6, 0x94));
			}
		}
		else
		{
			DrawColor(pDC, pOwner, pColorCell, rcColor, FALSE, FALSE, FALSE);
		}

		if (bChecked)
		{
			rcColor.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcColor, CRect(1, 1, 1, 1), m_crBack);

			rcColor.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcColor, CRect(1, 1, 1, 1), m_crBackPressed);
		}
	}
}

void CXTPColorSelectorCtrlThemeOffice2013::DrawColorSelector(CDC* pDC,
															 CXTPColorSelectorCtrl* pOwner)
{
	pDC->SetBkMode(TRANSPARENT);

	if (pOwner->HasCaptions())
	{
		CXTPClientRect rcClient(pOwner);

		CRect rcCapt(rcClient);
		rcCapt.DeflateRect(XTP_DPI_X(1), 0);

		CXTPFontDC fontDC(pDC, &XTPFontManager()->GetFontBold());
		pDC->SetTextColor(m_crTitleText);

		if (pOwner->GetThemeColors().GetSize() > 0 || pOwner->GetStandardColors().GetSize() > 0)
		{
			if (pOwner->GetThemeColors().GetSize())
			{
				rcCapt.top	= m_rcThemeText.top + m_nMargin;
				rcCapt.bottom = m_rcThemeText.bottom - m_nMargin;
				pDC->FillSolidRect(&rcCapt, m_crTitleBack);

				pDC->DrawText(pOwner->GetThemeTitle(), &m_rcThemeText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
			if (pOwner->GetStandardColors().GetSize())
			{
				rcCapt.top	= m_rcStandardText.top + m_nMargin;
				rcCapt.bottom = m_rcStandardText.bottom - m_nMargin;
				pDC->FillSolidRect(&rcCapt, m_crTitleBack);

				pDC->DrawText(pOwner->GetStandardTitle(), &m_rcStandardText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
		}
		else
		{
			if (!pOwner->GetCustomTitle().IsEmpty())
			{
				rcCapt.top	= m_rcStandardText.top + m_nMargin;
				rcCapt.bottom = m_rcStandardText.bottom - m_nMargin;
				pDC->FillSolidRect(&rcCapt, m_crTitleBack);

				pDC->DrawText(pOwner->GetCustomTitle(), &m_rcStandardText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
			else if ((pOwner->GetPopupStyle() & CPS_XTP_EXTENDED) == 0)
			{
				rcCapt.top	= m_rcStandardText.top + m_nMargin;
				rcCapt.bottom = m_rcStandardText.bottom - m_nMargin;
				pDC->FillSolidRect(&rcCapt, m_crTitleBack);

				pDC->DrawText(pOwner->GetStandardTitle(), &m_rcStandardText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
		}

		if (pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS
			&& pOwner->GetPopupStyle() & CPS_XTP_RECENTCOLORS)
		{
			if (pOwner->m_arRecentColors.GetSize() > 0)
			{
				rcCapt.top	= m_rcRecentText.top + m_nMargin;
				rcCapt.bottom = m_rcRecentText.bottom - m_nMargin;
				pDC->FillSolidRect(&rcCapt, m_crTitleBack);

				pDC->DrawText(CXTPColorSelectorCtrl::m_strTitleRecent, &m_rcRecentText,
							  DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			}
		}
	}

	CXTPFontDC fontDC(pDC, &XTPFontManager()->GetFont());
	pDC->SetTextColor(m_crText);

	POSITION pos;
	int nIndex = 0;
	for (pos = pOwner->GetColorCellList().GetHeadPosition(); pos;)
	{
		CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = pOwner->GetColorCellList().GetNext(pos);
		ASSERT(pColorCell != NULL);

		BOOL bHighlighted = (((nIndex == pOwner->GetCurSel()) && (pOwner->GetPressed() == -1))
							 || (nIndex == pOwner->GetPressed()));
		BOOL bPressed	 = ((nIndex == pOwner->GetPressed()) && (nIndex == pOwner->GetCurSel()));

		DrawColorCell(pDC, pOwner, pColorCell, bHighlighted, bPressed);
		nIndex++;
	}
}

CSize CXTPColorSelectorCtrlThemeOffice2013::CalcSize(CXTPColorSelectorCtrl* pOwner)
{
	CSize sz = CXTPColorSelectorCtrlTheme::CalcSize(pOwner);

	// theme colors
	if (pOwner->GetThemeColors().GetSize() > 0)
	{
		sz.cy -= XTP_DPI_Y(12); // reduce for color rows that have no margin.
	}

	// more colors
	if (pOwner->GetPopupStyle() & CPS_XTP_MORECOLORS)
	{
		sz.cy += m_nMargin;
	}

	return sz;
}

BOOL CXTPColorSelectorCtrlThemeOffice2013::CreateAutoButton(CXTPColorSelectorCtrl* pOwner,
															int& nIndex, const int x, int& y,
															const int cx, bool& bChecked)
{
	y -= m_nMargin;

	if (!CXTPColorSelectorCtrlTheme::CreateAutoButton(pOwner, nIndex, x, y, cx, bChecked))
		return FALSE;

	y += m_nMargin;

	return TRUE;
}

BOOL CXTPColorSelectorCtrlThemeOffice2013::CreateExtendedButtons(CXTPColorSelectorCtrl* pOwner,
																 int& nIndex, const int x, int& y,
																 const int cx, bool& bChecked)
{
	if (!CXTPColorSelectorCtrlTheme::CreateExtendedButtons(pOwner, nIndex, x, y, cx, bChecked))
		return FALSE;

	m_rcStandardText.left += m_nMargin;

	return TRUE;
}

BOOL CXTPColorSelectorCtrlThemeOffice2013::CreateThemeButtons(CXTPColorSelectorCtrl* pOwner,
															  int& nIndex, const int x, int& y,
															  const int cx, bool& bChecked)
{
	ASSERT_VALID(pOwner);

	if (pOwner->GetThemeColors().GetSize())
	{
		if (pOwner->HasCaptions())
		{
			// set theme text rect.
			m_rcThemeText.left   = x + pOwner->GetBorderRect().left + m_nMargin;
			m_rcThemeText.top	= y;
			m_rcThemeText.bottom = y + m_nCaption;
			m_rcThemeText.right  = cx - pOwner->GetBorderRect().right;

			y += m_nCaption;
		}
		else
		{
			y += m_nMargin;
		}

		CRect rcButton(x + pOwner->GetBorderRect().left, y,
					   x + pOwner->GetBorderRect().left + m_sizeButton.cx, y + m_sizeButton.cy);

		for (int i = 0; i < pOwner->GetThemeColors().GetSize(); i++)
		{
			nIndex++;

			if (i > 0 && ((i % pOwner->GetNumCols()) == 0))
			{
				y += m_sizeButton.cy;

				if (i >= 20)
				{
					y -= XTP_DPI_Y(3);
				}

				rcButton = CRect(x + pOwner->GetBorderRect().left, y,
								 x + pOwner->GetBorderRect().left + m_sizeButton.cx,
								 y + m_sizeButton.cy);
			}

			COLORREF clrColor = pOwner->GetThemeColors()[i].clrButton;

			CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
			pColorCell->szText[0]						  = 0;
			pColorCell->nID								  = pOwner->GetThemeColors()[i].toolTipID;
			pColorCell->rect							  = rcButton;
			pColorCell->nIndex							  = XTPToUIntChecked(nIndex);
			pColorCell->bChecked						  = false;
			pColorCell->dwStyle							  = CPS_XTP_PICKBOX;
			pColorCell->clr								  = clrColor;
			pColorCell->rcBorder						  = CRect(1, 1, 1, 1);

			if (i >= 10 && i <= 19)
			{
				pColorCell->rcBorder.bottom = 0;
			}
			else if (i >= 20 && i <= 49)
			{
				pColorCell->rcBorder.top	= 0;
				pColorCell->rcBorder.bottom = 0;
			}
			else if (i >= 50 && i <= 59)
			{
				pColorCell->rcBorder.top = 0;
			}

			pOwner->GetColorCellList().AddTail(pColorCell);
			pOwner->GetToolTipCtrl().AddTool(pOwner, pOwner->GetThemeColors()[i].szTip,
											 pColorCell->rect, 1);

			if ((pOwner->GetCurColor() == clrColor) && !bChecked)
			{
				bChecked			 = true;
				pColorCell->bChecked = true;
				pOwner->SetCurSel(XTPToIntChecked(pColorCell->nIndex));
			}

			// offset to next column.
			rcButton.OffsetRect(m_sizeButton.cx, 0);
		}

		y += m_sizeButton.cy;
	}

	if (pOwner->GetStandardColors().GetSize())
	{
		if (pOwner->HasCaptions())
		{
			// set standard text rect.
			m_rcStandardText.left   = x + pOwner->GetBorderRect().left + m_nMargin;
			m_rcStandardText.top	= y;
			m_rcStandardText.bottom = y + m_nCaption;
			m_rcStandardText.right  = cx - pOwner->GetBorderRect().right;

			y += m_nCaption;
		}
		else
		{
			y += m_nMargin;
		}

		CRect rcButton(x + pOwner->GetBorderRect().left, y,
					   x + pOwner->GetBorderRect().left + m_sizeButton.cx, y + m_sizeButton.cy);

		for (int i = 0; i < pOwner->GetStandardColors().GetSize(); i++)
		{
			nIndex++;

			if (i > 0 && ((i % pOwner->GetNumCols()) == 0))
			{
				y += m_sizeButton.cy;
				rcButton = CRect(x, y, x + m_sizeButton.cy, y + m_sizeButton.cy);
			}

			COLORREF clrColor = pOwner->GetStandardColors()[i].clrButton;

			CXTPColorSelectorCtrl::COLOR_CELL* pColorCell = new CXTPColorSelectorCtrl::COLOR_CELL;
			pColorCell->szText[0]						  = 0;
			pColorCell->nID		 = pOwner->GetStandardColors()[i].toolTipID;
			pColorCell->rect	 = rcButton;
			pColorCell->nIndex   = XTPToUIntChecked(nIndex);
			pColorCell->bChecked = false;
			pColorCell->dwStyle  = CPS_XTP_PICKBOX;
			pColorCell->clr		 = clrColor;
			pColorCell->rcBorder = CRect(1, 1, 1, 1);

			pOwner->GetColorCellList().AddTail(pColorCell);
			pOwner->GetToolTipCtrl().AddTool(pOwner, pOwner->GetStandardColors()[i].szTip,
											 pColorCell->rect, 1);

			if ((pOwner->GetCurColor() == clrColor) && !bChecked)
			{
				bChecked			 = true;
				pColorCell->bChecked = true;
				pOwner->SetCurSel(XTPToIntChecked(pColorCell->nIndex));
			}

			// offset to next column.
			rcButton.OffsetRect(m_sizeButton.cx, 0);
		}

		y += m_sizeButton.cy;
	}

	return TRUE;
}

BOOL CXTPColorSelectorCtrlThemeOffice2013::CreateRecentColorButtons(CXTPColorSelectorCtrl* pOwner,
																	int& nIndex, const int x,
																	int& y, const int cx,
																	bool& bChecked)
{
	if (!CXTPColorSelectorCtrlTheme::CreateRecentColorButtons(pOwner, nIndex, x, y, cx, bChecked))
		return FALSE;

	m_rcRecentText.left += m_nMargin;

	return TRUE;
}

BOOL CXTPColorSelectorCtrlThemeOffice2013::CreateMoreButton(CXTPColorSelectorCtrl* pOwner,
															int& nIndex, const int x, int& y,
															const int cx, bool& bChecked)
{
	y += m_nMargin;

	return CXTPColorSelectorCtrlTheme::CreateMoreButton(pOwner, nIndex, x, y, cx, bChecked);
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeVisualStudio2015

CXTPColorSelectorCtrlThemeVisualStudio2015::CXTPColorSelectorCtrlThemeVisualStudio2015()
{
	m_nCaption	= XTP_DPI_Y(30);
	m_nTheme	  = xtpControlThemeVisualStudio2015;
	m_bThinBorder = TRUE;
}

void CXTPColorSelectorCtrlThemeVisualStudio2015::RefreshMetrics(CXTPColorSelectorCtrl* pOwner)
{
	CXTPColorSelectorCtrlTheme::RefreshMetrics(pOwner);

	m_crBorder		  = XTPIniColor(_T("Controls.ColorPicker"), _T("Border"), RGB(198, 199, 198));
	m_crBorderHilight = XTPIniColor(_T("Controls.ColorPicker"), _T("BorderHighlight"),
									RGB(49, 154, 255));
	m_crBorderPressed = XTPIniColor(_T("Controls.ColorPicker"), _T("BorderPressed"),
									RGB(0, 121, 206));
	m_crBorderInside  = XTPIniColor(_T("Controls.ColorPicker"), _T("BorderInside"),
									RGB(206, 223, 247));
	m_crBack		  = XTPIniColor(_T("Controls.ColorPicker"), _T("Back"), RGB(239, 239, 247));
	m_crBackPressed = XTPIniColor(_T("Controls.ColorPicker"), _T("BackPressed"), RGB(0, 121, 206));
	m_crBackHilight = XTPIniColor(_T("Controls.ColorPicker"), _T("BackHighlight"),
								  RGB(206, 223, 247));
	m_crBackChecked = XTPIniColor(_T("Controls.ColorPicker"), _T("BackChecked"),
								  RGB(239, 239, 247));
	m_crPopupBorder = XTPIniColor(_T("Controls.ColorPicker"), _T("PopupBorder"),
								  RGB(156, 158, 189));
	m_crText		= XTPIniColor(_T("Controls.ColorPicker"), _T("Text"), RGB(24, 28, 24));
	m_crTextPressed = XTPIniColor(_T("Controls.ColorPicker"), _T("TextPressed"),
								  RGB(255, 255, 255));
	m_crTitleText   = XTPIniColor(_T("Controls.ColorPicker"), _T("TitleText"), RGB(66, 69, 66));
	m_crTitleBack   = XTPIniColor(_T("Controls.ColorPicker"), _T("TitleBack"), RGB(231, 227, 231));
}

void CXTPColorSelectorCtrlThemeVisualStudio2015::DrawColorCell(
	CDC* pDC, CXTPColorSelectorCtrl* pOwner, CXTPColorSelectorCtrl::COLOR_CELL* pColorCell,
	BOOL bHilite, BOOL bPressed)
{
	ASSERT(NULL != pColorCell);

	if (pColorCell->dwStyle == CPS_XTP_PICKBOX)
	{
		CXTPColorSelectorCtrlThemeOffice2013::DrawColorCell(pDC, pOwner, pColorCell, bHilite,
															bPressed);
	}
	else
	{
		ASSERT_VALID(pDC);

		pDC->SetBkMode(TRANSPARENT);

		CRect rcItem(pColorCell->rect);
		BOOL bChecked = pColorCell->bChecked;

		COLORREF crCell = pColorCell->clr;
		if (crCell == COLORREF_NULL)
		{
			crCell = m_crBack;
		}

		if (bChecked || bHilite || bPressed)
		{
			CRect rcState(rcItem);
			rcState.InflateRect(XTP_DPI_X(2), 0, XTP_DPI_Y(2), XTP_DPI_X(2));

			if (bPressed)
			{
				pDC->FillSolidRect(rcState, m_crBackPressed);
				pDC->Draw3dRect(rcState, m_crBorderPressed, m_crBorderPressed);
			}
			else if (bHilite)
			{
				pDC->FillSolidRect(rcState, m_crBackHilight);
				pDC->Draw3dRect(rcState, m_crBorderHilight, m_crBorderHilight);
			}
		}

		int nLen = static_cast<int>(_tcslen(pColorCell->szText));
		ASSERT(nLen > 0);

		if (nLen > 0)
		{
			CRect rcText(rcItem);
			rcText.InflateRect(XTP_DPI_X(-25), 0, XTP_DPI_Y(-10), XTP_DPI_Y(2));

			CString strText(pColorCell->szText);
			rcText.DeflateRect(XTP_DPI_X(5), 0);

			DWORD dwFormat = DT_SINGLELINE | DT_VCENTER;
			if (pDC->GetTextExtent(pColorCell->szText, nLen).cx > rcText.Width())
			{
				strText.Replace(_T("..."), _T(""));
				dwFormat |= DT_END_ELLIPSIS;
			}

			pDC->SetTextColor(bPressed ? m_crTextPressed : m_crText);
			pDC->DrawText(strText, nLen, rcText, dwFormat);
		}

		// determine color block size.
		CRect rcColor(rcItem);
		rcColor.OffsetRect(XTP_DPI_X(3), XTP_DPI_Y(5));
		rcColor.right  = rcColor.left + XTP_DPI_X(14);
		rcColor.bottom = rcColor.top + XTP_DPI_Y(14);

		if (pColorCell->dwStyle == CPS_XTP_MORECOLORS)
		{
			// draw separator line.
			CRect rcLine(rcItem);
			rcLine.OffsetRect(0, -m_nMargin);
			rcLine.bottom = rcLine.top + XTP_DPI_Y(1);
			rcLine.left -= XTP_DPI_X(3);
			rcLine.right += XTP_DPI_X(3);
			pDC->FillSolidRect(rcLine, m_crTitleBack);

			// draw color block.
			if (bChecked)
			{
				DrawColor(pDC, pOwner, pColorCell, rcColor, FALSE, FALSE, FALSE);
			}
			// draw "More Colors" icon.
			else
			{
				rcColor.InflateRect(0, 0, XTP_DPI_X(1), XTP_DPI_Y(1));
				pDC->FillSolidRect(rcColor, m_crBack);

				// determine the color block size.
				CRect rcSquare(rcColor);
				rcSquare.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
				rcSquare.right  = rcSquare.left + XTP_DPI_X(6);
				rcSquare.bottom = rcSquare.top + XTP_DPI_Y(6);
				pDC->FillSolidRect(rcSquare, RGB(0xEF, 0xC3, 0x84));

				rcSquare.OffsetRect(XTP_DPI_Y(7), 0);
				pDC->FillSolidRect(rcSquare, RGB(0x4A, 0x82, 0xBD));

				rcSquare.OffsetRect(0, XTP_DPI_Y(7));
				pDC->FillSolidRect(rcSquare, RGB(0xDE, 0x61, 0x42));

				rcSquare.OffsetRect(XTP_DPI_Y(-7), 0);
				pDC->FillSolidRect(rcSquare, RGB(0x73, 0xA6, 0x94));
			}
		}
		else
		{
			DrawColor(pDC, pOwner, pColorCell, rcColor, FALSE, FALSE, FALSE);
		}

		if (bChecked)
		{
			rcColor.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcColor, CRect(1, 1, 1, 1), m_crBack);

			rcColor.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawBorder(pDC, rcColor, CRect(1, 1, 1, 1), m_crBackPressed);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeVisualStudio2017

CXTPColorSelectorCtrlThemeVisualStudio2017::CXTPColorSelectorCtrlThemeVisualStudio2017()
{
	m_nCaption	= XTP_DPI_Y(30);
	m_nTheme	  = xtpControlThemeVisualStudio2017;
	m_bThinBorder = TRUE;
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeVisualStudio2019

CXTPColorSelectorCtrlThemeVisualStudio2019::CXTPColorSelectorCtrlThemeVisualStudio2019()
{
	m_nCaption	= XTP_DPI_Y(30);
	m_nTheme	  = xtpControlThemeVisualStudio2019;
	m_bThinBorder = TRUE;
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeVisualStudio2022

CXTPColorSelectorCtrlThemeVisualStudio2022::CXTPColorSelectorCtrlThemeVisualStudio2022()
{
	m_nCaption	= XTP_DPI_Y(30);
	m_nTheme	  = xtpControlThemeVisualStudio2022;
	m_bThinBorder = TRUE;
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSelectorCtrlThemeNativeWindows10

CXTPColorSelectorCtrlThemeNativeWindows10::CXTPColorSelectorCtrlThemeNativeWindows10()
{
	m_nCaption	= XTP_DPI_Y(30);
	m_nTheme	  = xtpControlThemeNativeWindows10;
	m_bThinBorder = TRUE;
}
