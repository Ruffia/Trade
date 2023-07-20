// XTPColorDialog.cpp : implementation file
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
#include "Common/Math/XTPMathUtils.h"

#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPVC80Helpers.h" // Visual Studio 2005 helper functions
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Controls/Resource.h"
#include "Controls/Defines.h"
#include "Controls/Dialog/XTPColorDialog.h"
#include "Controls/Dialog/XTPColorPageCustom.h"
#include "Controls/Dialog/XTPColorPageStandard.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WS_EX_NOINHERITLAYOUT
#	define WS_EX_NOINHERITLAYOUT 0x00100000L
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPColorDialog

IMPLEMENT_DYNAMIC(CXTPColorDialog, CPropertySheet)

CXTPColorDialog::CXTPColorDialog(COLORREF clrNew, COLORREF clrCurrent, DWORD dwFlags /*= 0L*/,
								 CWnd* pWndParent /*= NULL*/)
	: m_nTextHeight(0)
	, m_nGapSize(0)
{
	CString strCaption;
	XTPResourceManager()->LoadString(&strCaption, XTP_IDS_COLOR_CAPTION);

	Construct(strCaption, pWndParent);

	m_psh.dwFlags = (m_psh.dwFlags & ~PSH_HASHELP) | PSH_NOAPPLYNOW;

	m_clrNew	 = clrNew;
	m_clrCurrent = clrCurrent;
	m_dwStyle	= dwFlags;

	AddPage(new CXTPColorPageStandard(this));
	AddPage(new CXTPColorPageCustom(this));
}

void CXTPColorDialog::AddPage(CPropertyPage* pPage)
{
	CPropertySheet::AddPage(pPage);

	LPCDLGTEMPLATE pResource = XTPResourceManager()->LoadDialogTemplate(
		(UINT)(UINT_PTR)pPage->m_psp.pszTemplate);

	if (pResource)
	{
		pPage->m_psp.pResource = pResource;
		pPage->m_psp.dwFlags |= PSP_DLGINDIRECT;
	}
}

CXTPColorDialog::~CXTPColorDialog()
{
	int iPage;
	for (iPage = 0; iPage < GetPageCount(); ++iPage)
	{
		CPropertyPage* pPage = GetPage(iPage);
		SAFE_DELETE(pPage);
	}
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPColorDialog, CPropertySheet)
	//{{AFX_MSG_MAP(CXTPColorDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_MESSAGE(WM_XTP_EYEDROPPER_MOVING, OnEyeDropperMoving)
	ON_MESSAGE(WM_XTP_EYEDROPPER_SELECT, OnEyeDropperSelect)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPColorDialog message handlers

void CXTPColorDialog::CalculateRects()
{
	CRect rcClient;
	CRect rcBtnOK;
	CRect rcBtnCancel;
	CRect rcTabCtrl;
	CRect rcItem;

	GetClientRect(&rcClient);

	// Obtain text height.
	CClientDC dc(this);
	CXTPFontDC fontDC(&dc, GetFont());

	CRect rcText;
	dc.DrawText(_T("AbCd"), rcText, DT_CALCRECT | DT_LEFT);
	m_nTextHeight = rcText.Height();

	// get the tab control size.
	CTabCtrl* pTabCtrl = GetTabControl();
	pTabCtrl->GetWindowRect(&rcTabCtrl);
	ScreenToClient(&rcTabCtrl);
	m_nGapSize = rcTabCtrl.left - rcClient.left;

	// get the size of the first tab item.
	pTabCtrl->GetItemRect(0, &rcItem);

	// get the OK button size.
	CButton* pBtnOK = (CButton*)GetDlgItem(IDOK);
	pBtnOK->GetWindowRect(&rcBtnOK);
	ScreenToClient(&rcBtnOK);

	// get the Cancel button size.
	CButton* pBtnCancel = (CButton*)GetDlgItem(IDCANCEL);
	pBtnCancel->GetWindowRect(&rcBtnCancel);
	ScreenToClient(&rcBtnCancel);

	// set position of the Cancel button.
	rcBtnCancel.left  = rcClient.right - rcBtnOK.Width();
	rcBtnCancel.right = rcClient.right - m_nGapSize;

	// resize the tab control
	rcTabCtrl.left   = m_nGapSize;
	rcTabCtrl.right  = rcClient.right - rcBtnOK.Width() - 2 * m_nGapSize;
	rcTabCtrl.bottom = rcClient.bottom - m_nGapSize; // rcBtnCancel.top - 15;
	pTabCtrl->MoveWindow(&rcTabCtrl);

	// reposition the OK button.
	rcBtnOK		   = rcBtnCancel;
	rcBtnOK.top	= rcTabCtrl.top + rcItem.Height() + 1;
	rcBtnOK.bottom = rcBtnOK.top + rcBtnCancel.Height();
	pBtnOK->MoveWindow(&rcBtnOK);

	// reposition the Cancel button.
	rcBtnCancel		   = rcBtnOK;
	rcBtnCancel.top	= rcBtnOK.bottom + m_nGapSize;
	rcBtnCancel.bottom = rcBtnCancel.top + rcBtnOK.Height();
	pBtnCancel->MoveWindow(&rcBtnCancel);

	// reposition the hex display
	if (::IsWindow(m_wndHexEdit.m_hWnd))
	{
		CRect rcWnd;
		rcWnd		 = rcBtnCancel;
		rcWnd.top	= rcBtnCancel.bottom + m_nGapSize;
		rcWnd.bottom = rcWnd.top + m_nTextHeight + XTP_DPI_Y(4);
		m_wndHexEdit.MoveWindow(&rcWnd);
	}

	// reposition the eye dropper.
	if (::IsWindow(m_wndEyeDropper.m_hWnd))
	{
		CRect rcWnd;
		m_wndEyeDropper.GetWindowRect(&rcWnd);

		CSize size   = rcWnd.Size();
		rcWnd.right  = rcBtnOK.right;
		rcWnd.left   = rcBtnOK.right - size.cx;
		rcWnd.bottom = rcTabCtrl.bottom;
		rcWnd.top	= rcTabCtrl.bottom - size.cy;

		m_wndEyeDropper.MoveWindow(&rcWnd);
	}

	// resize the property sheet.
	CXTPWindowRect rcWindow(this);
	ClientToScreen(&rcTabCtrl);
	rcWindow.bottom = rcTabCtrl.bottom + m_nGapSize * 2;
	MoveWindow(&rcWindow);
}

BOOL CXTPColorDialog::OnInitDialog()
{
	if (GetExStyle() & WS_EX_LAYOUTRTL)
	{
		ModifyStyleEx(0, WS_EX_LAYOUTRTL | WS_EX_NOINHERITLAYOUT);
	}

	BOOL bResult = CPropertySheet::OnInitDialog();

	CalculateRects();
	SetActivePage(0);

	if (m_wndHexEdit.GetSafeHwnd())
	{
		m_wndHexEdit.SetFont(GetFont());
	}

	return bResult;
}

void CXTPColorDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));

	CXTPFontDC fontDC(&dc, GetFont());

	// get the Cancel button size.
	CXTPWindowRect rcBtnCancel(GetDlgItem(IDCANCEL));
	ScreenToClient(&rcBtnCancel);

	// construct the size for the the new / current color box.
	CXTPClientRect rect(this);
	rect.right = rcBtnCancel.right;
	rect.left  = rcBtnCancel.left;
	rect.top   = rcBtnCancel.bottom + m_nGapSize
			   + (::IsWindow(m_wndHexEdit.m_hWnd) ? m_nTextHeight + m_nGapSize : 0) + m_nTextHeight
			   + m_nGapSize;
	rect.bottom = rect.top + rect.Width();

	// draw the borders for the new / current color box.
	if (m_dwStyle & CPS_XTP_SHOW3DSELECTION)
	{
		dc.Draw3dRect(&rect, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DDKSHADOW));

		rect.DeflateRect(3, 3);
		dc.Draw3dRect(&rect, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
	}
	else
	{
		dc.Draw3dRect(&rect, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));
	}

	// draw the new text string.
	CRect rcText(rect.left, rect.top - m_nGapSize - m_nTextHeight, rect.right,
				 rect.top - m_nGapSize);

	CString strText;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strText, XTP_IDS_NEW));
	dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// draw the current text string.
	rcText.SetRect(rect.left, rect.bottom + m_nGapSize, rect.right,
				   rect.bottom + m_nGapSize + m_nTextHeight);

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strText, XTP_IDS_CURRENT));
	dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rect.DeflateRect(1, 1);
	int cy = rect.Height() / 2;

	// fill the new color box.
	m_rcNew		   = rect;
	m_rcNew.bottom = rect.top + cy;
	dc.FillSolidRect(&m_rcNew, m_clrNew);

	// fill the current color box.
	m_rcCurrent		= rect;
	m_rcCurrent.top = rect.bottom - cy;
	dc.FillSolidRect(&m_rcCurrent, m_clrCurrent);
}

void CXTPColorDialog::SetNewColor(COLORREF clr, BOOL bNotify /*= TRUE*/)
{
	m_clrNew = clr;

	if (IsWindowVisible())
	{
		CClientDC dc(this);
		dc.FillSolidRect(&m_rcNew, m_clrNew);

		if (m_dwStyle & CPS_XTP_SHOWHEXVALUE)
		{
			m_wndHexEdit.SetWindowText(RGBtoHex(m_clrNew));
		}
	}

	if (bNotify && GetTabControl() && GetTabControl()->GetSafeHwnd())
	{
		int iPage;
		for (iPage = 0; iPage < GetPageCount(); ++iPage)
		{
			CWnd* pWnd = GetPage(iPage);

			if (::IsWindow(pWnd->GetSafeHwnd()))
			{
				pWnd->SendMessage(WM_XTP_UPDATECOLOR, (WPARAM)m_clrNew, (LPARAM)m_hWnd);
			}
		}
	}
}

void CXTPColorDialog::SetCurrentColor(COLORREF clr)
{
	m_clrCurrent = clr;

	if (IsWindowVisible())
	{
		CClientDC dc(this);
		dc.FillSolidRect(&m_rcCurrent, m_clrCurrent);
	}
}

BOOL CXTPColorDialog::CopyToClipboard(const CString& strText)
{
	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();

		HGLOBAL hGlobalBuff = ::GlobalAlloc(GMEM_MOVEABLE,
											XTPToSizeTChecked(strText.GetLength() + 1));
		if (!hGlobalBuff)
		{
			::CloseClipboard();
			return FALSE;
		}

		CHAR* szBuffer = (CHAR*)::GlobalLock(hGlobalBuff);

		WCSTOMBS_S(szBuffer, strText, XTPToSizeTChecked(strText.GetLength() + 1));
		::GlobalUnlock(hGlobalBuff);

		if (::SetClipboardData(CF_TEXT, hGlobalBuff) == NULL)
			return FALSE;

		::CloseClipboard();
		return TRUE;
	}

	return FALSE;
}

CString AFX_CDECL CXTPColorDialog::RGBtoHex(COLORREF clr)
{
	CString strHex;
	strHex.Format(_T("#%02X%02X%02X"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
	return strHex;
}

int CXTPColorDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_dwStyle & CPS_XTP_SHOWHEXVALUE)
	{
		if (!m_wndHexEdit.CreateEx(WS_EX_STATICEDGE, _T("EDIT"), RGBtoHex(m_clrNew),
								   WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER,
								   CRect(0, 0, 0, 0), this, AFX_IDC_CHANGE))
		{
			TRACE0("Failed to create edit control.\n");
			return -1;
		}
	}

	if (m_dwStyle & CPS_XTP_SHOWEYEDROPPER)
	{
		if (!m_wndEyeDropper.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, AFX_IDC_BROWSE))
		{
			TRACE0("Failed to create eye dropper.\n");
			return -1;
		}
	}

	((CXTPColorPageStandard*)GetPage(0))->SetColor(m_clrNew);

	return 0;
}

LRESULT CXTPColorDialog::OnEyeDropperMoving(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	COLORREF color = (COLORREF)wParam;
	SetNewColor(color, FALSE);
	return 0;
}

LRESULT CXTPColorDialog::OnEyeDropperSelect(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	COLORREF color = (COLORREF)wParam;
	SetNewColor(color, TRUE);
	return 0;
}

void CXTPColorDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPropertySheet::OnLButtonDown(nFlags, point);

	if (m_rcCurrent.PtInRect(point))
	{
		SetNewColor(m_clrCurrent);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPEyeDropper

const TCHAR XTPEYEDROPPER_CLASSNAME[] = _T("XTPEyeDropper");

AFX_STATIC BOOL RegisterEyeDropperClass()
{
	WNDCLASS wndcls;
	::ZeroMemory(&wndcls, sizeof(wndcls));

	wndcls.hInstance	 = XTPGetInstanceHandle();
	wndcls.style		 = CS_DBLCLKS;
	wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.hCursor		 = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.lpszClassName = XTPEYEDROPPER_CLASSNAME;
	wndcls.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

	return AfxRegisterClass(&wndcls);
}

CXTPEyeDropper::CXTPEyeDropper()
	: m_nPadding(2)
	, m_icoSize(24, 24)
	, m_bPreInit(TRUE)
	, m_bSetClipboard(FALSE)
	, m_bShowPreview(FALSE)
	, m_hIcon(NULL)
	, m_hCurEyeDropper(NULL)
	, m_hCurPrevious(NULL)
	, m_color(COLORREF_NULL)
{
	VERIFY(RegisterEyeDropperClass());

	m_hIcon = XTPResourceManager()->LoadIcon(XTP_IDI_EYEDROPPER, CSize(24, 24));

	m_hCurEyeDropper = XTPResourceManager()->LoadCursor(XTP_IDC_EYEDROPPER);

	m_color = ::GetSysColor(COLOR_3DFACE);
}

CXTPEyeDropper::~CXTPEyeDropper()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPEyeDropper, CWnd)
	//{{AFX_MSG_MAP(CXTPEyeDropper)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPEyeDropper message handlers

BOOL CXTPEyeDropper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
							const RECT& rect, CWnd* pParentWnd, UINT nID,
							CCreateContext* pContext /*=NULL*/)
{
	UNREFERENCED_PARAMETER(lpszClassName);
	return CWnd::Create(XTPEYEDROPPER_CLASSNAME, lpszWindowName, dwStyle, rect, pParentWnd, nID,
						pContext);
}

BOOL CXTPEyeDropper::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CXTPEyeDropper::Create(XTPEYEDROPPER_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CXTPEyeDropper::Init()
{
	CRect rc;
	GetWindowRect(&rc);

	rc.bottom = rc.top + m_icoSize.cy;
	rc.right  = rc.left + m_icoSize.cx;

	if (m_bShowPreview)
	{
		rc.right += m_icoSize.cx + m_nPadding * 2;
	}

	SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_FRAMECHANGED);

	return TRUE;
}

void CXTPEyeDropper::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if (m_bPreInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPEyeDropper::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPEyeDropper::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreInit = FALSE;

	return TRUE;
}

CRect CXTPEyeDropper::GetColorRect()
{
	CRect rc = GetDropperRect();
	rc.OffsetRect(m_icoSize.cx + m_nPadding, 0);
	return rc;
}

CRect CXTPEyeDropper::GetDropperRect()
{
	return CRect(0, 0, m_icoSize.cx, m_icoSize.cy);
}

void CXTPEyeDropper::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(&rc, ::GetSysColor(COLOR_3DFACE));

	DrawDropper(&memDC);
	DrawColor(&memDC);

	dc.BitBlt(0, 0, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

BOOL CXTPEyeDropper::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTPEyeDropper::DrawDropper(CDC* pDC)
{
	CRect rc = GetDropperRect();

	if (m_hCurPrevious == NULL)
	{
		pDC->DrawState(rc.TopLeft(), rc.Size(), m_hIcon, DST_ICON | DSS_NORMAL, (HBRUSH)NULL);
	}

	pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));

	rc.DeflateRect(1, 1);

	pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));
}

void CXTPEyeDropper::DrawColor(CDC* pDC)
{
	if (m_bShowPreview)
	{
		CRect rc = GetColorRect();

		pDC->FillSolidRect(rc, m_color);

		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
	}
}

BOOL CXTPEyeDropper::SetClipboardText(LPCTSTR lpszClipText)
{
	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();

		HGLOBAL hGlobal = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE,
									  (_tcslen(lpszClipText) + 1) * sizeof(TCHAR));
		if (!hGlobal)
			return FALSE;

		LPTSTR lpszGlobal = (LPTSTR)::GlobalLock(hGlobal);

		STRCPY_S(lpszGlobal, _tcslen(lpszClipText) + 1, lpszClipText);

		::GlobalUnlock(hGlobal);

#ifdef _UNICODE
		HANDLE hData = ::SetClipboardData(CF_UNICODETEXT, hGlobal);
#else
		HANDLE hData = ::SetClipboardData(CF_TEXT, hGlobal);
#endif

		::CloseClipboard();

		return (hData != NULL);
	}

	return FALSE;
}

BOOL CXTPEyeDropper::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (::GetCapture() == m_hWnd && m_hCurEyeDropper)
	{
		::SetCursor(m_hCurEyeDropper);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPEyeDropper::OnMouseMove(UINT nFlags, CPoint point)
{
	if (::GetCapture() == m_hWnd)
	{
		CRect rc;
		GetClientRect(&rc);

		if (!rc.PtInRect(point))
		{
			CPoint pt;
			::GetCursorPos(&pt);

			CWindowDC dc(NULL);
			m_color = dc.GetPixel(pt.x, pt.y);

			CWnd* pWnd = GetOwner();
			if (pWnd)
			{
				pWnd->SendMessage(WM_XTP_EYEDROPPER_MOVING, (WPARAM)m_color,
								  MAKELPARAM(pt.x, pt.y));
			}

			RedrawWindow();
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CXTPEyeDropper::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	if (m_hCurEyeDropper)
	{
		m_hCurPrevious = ::SetCursor(m_hCurEyeDropper);
	}

	SetCapture();
	RedrawWindow();
}

void CXTPEyeDropper::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	if (::GetCapture() == m_hWnd)
	{
		CPoint pt;
		::GetCursorPos(&pt);

		CWindowDC dc(NULL);
		m_color = dc.GetPixel(pt.x, pt.y);

		CWnd* pWnd = GetOwner();
		if (pWnd)
		{
			pWnd->SendMessage(WM_XTP_EYEDROPPER_SELECT, (WPARAM)m_color, MAKELPARAM(pt.x, pt.y));
		}

		if (m_bSetClipboard)
		{
			CString strClip;
			strClip.Format(_T("%i,%i,%i"), GetRValue(m_color), GetGValue(m_color),
						   GetBValue(m_color));

			SetClipboardText(strClip);
		}

		m_hCurPrevious = NULL;

		ReleaseCapture();
		RedrawWindow();
	}
}
