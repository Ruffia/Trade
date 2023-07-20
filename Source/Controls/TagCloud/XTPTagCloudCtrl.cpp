// XTPTagCloudCtrl.cpp : implementation file
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
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPFontManager.h"

#include "Controls/Resource.h"
#include "Controls/Defines.h"
#include "Controls/TagCloud/XTPTagCloudCtrl.h"
#include "Controls/XTPControlsIIDs.h"

#ifdef _XTP_ACTIVEX
#	include "../../../Workspace/ActiveX/Controls/TagCloudStyle.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================================
// CXTPTagCloudStyle
//===========================================================================

CXTPTagCloudStyle::CXTPTagCloudStyle()
	: m_nScore(0)
	, m_clrText(COLORREF_NULL)
	, m_clrTextHover(COLORREF_NULL)
	, m_clrBorder(COLORREF_NULL)
{
}

CXTPTagCloudStyle::CXTPTagCloudStyle(const CXTPTagCloudStyle& style)
	: m_nScore(0)
	, m_clrText(COLORREF_NULL)
	, m_clrTextHover(COLORREF_NULL)
	, m_clrBorder(COLORREF_NULL)
{
	Copy(&style, this);
}

CXTPTagCloudStyle::CXTPTagCloudStyle(int nScore, COLORREF clrText, COLORREF clrTextHover,
									 COLORREF clrBorder, LOGFONT& lf)
	: m_nScore(nScore)
	, m_clrText(clrText)
	, m_clrTextHover(clrTextHover)
	, m_clrBorder(clrBorder)
{
	SetFont(lf);
}

void CXTPTagCloudStyle::SetFont(LOGFONT& lf)
{
	if (m_xtpFont.GetSafeHandle())
		m_xtpFont.DeleteObject();

	m_xtpFont.CreateFontIndirect(&lf);
}

CXTPTagCloudStyle& CXTPTagCloudStyle::operator=(const CXTPTagCloudStyle& style)
{
	Copy(&style, this);
	return *this;
}

void AFX_CDECL CXTPTagCloudStyle::Copy(const CXTPTagCloudStyle* pSource, CXTPTagCloudStyle* pDest)
{
	if (pSource && pDest)
	{
		pDest->m_nScore		  = pSource->m_nScore;
		pDest->m_clrText	  = pSource->m_clrText;
		pDest->m_clrTextHover = pSource->m_clrTextHover;
		pDest->m_clrBorder	= pSource->m_clrBorder;

		LOGFONT lf;
		::GetObject(pSource->m_xtpFont.m_hObject, sizeof(lf), &lf);
		pDest->SetFont(lf);
	}
}

//===========================================================================
// CXTPTagCloudStyleManager
//===========================================================================

CXTPTagCloudStyleManager::CXTPTagCloudStyleManager()
{
	InitDefaultStyles();
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPTagCloudStyleManager::~CXTPTagCloudStyleManager()
{
	RemoveAll();
}

INT_PTR CXTPTagCloudStyleManager::AddStyle(CXTPTagCloudStyle* pStyle)
{
	if (!pStyle || ScoreExists(pStyle->m_nScore))
	{
		ASSERT(0); // Error - Score already exists or is not a valid pointer.
		return -1;
	}

	return m_arrStyles.Add(pStyle);
}

INT_PTR CXTPTagCloudStyleManager::AddStyle(int nScore, COLORREF clrText, COLORREF clrTextHover,
										   COLORREF clrBorder, LOGFONT& lf)
{
	return AddStyle(new CXTPTagCloudStyle(nScore, clrText, clrTextHover, clrBorder, lf));
}

BOOL CXTPTagCloudStyleManager::RemoveStyle(int nScore)
{
	BOOL bRemoved = FALSE;
	INT_PTR i	 = 0;

	// search for the specified score and if it exists, remove
	// it from the array.

	for (i = m_arrStyles.GetUpperBound(); i >= 0; i--)
	{
		CXTPTagCloudStyle* pStyle = m_arrStyles[i];
		if (pStyle && pStyle->m_nScore == nScore)
		{
			SAFE_DELETE(pStyle);
			m_arrStyles.RemoveAt(i);
			bRemoved = TRUE;
			break;
		}
	}

	// if the score was found and removed, renumber the
	// remaining scores sequentially.

	if (bRemoved)
	{
		for (i = m_arrStyles.GetUpperBound(); i >= 0; i--)
		{
			CXTPTagCloudStyle* pStyle = m_arrStyles[i];
			pStyle->m_nScore		  = static_cast<int>(i + 1);
		}
	}

	return bRemoved;
}

void CXTPTagCloudStyleManager::RemoveAll()
{
	for (INT_PTR i = 0; i < m_arrStyles.GetSize(); i++)
	{
		SAFE_DELETE(m_arrStyles[i]);
	}

	m_arrStyles.RemoveAll();
}

void CXTPTagCloudStyleManager::InitDefaultStyles()
{
	RemoveAll();

	LOGFONT lf;
	XTPFontManager()->GetFont().GetLogFont(&lf);

	CWindowDC dc(NULL);
	lf.lfHeight = -::MulDiv(9, dc.GetDeviceCaps(LOGPIXELSY), 72);
	AddStyle(1, RGB(107, 199, 255), RGB(255, 152, 29), COLORREF_NULL, lf);

	lf.lfHeight = -::MulDiv(11, dc.GetDeviceCaps(LOGPIXELSY), 72);
	AddStyle(2, RGB(33, 105, 198), RGB(255, 152, 29), COLORREF_NULL, lf);

	lf.lfHeight = -::MulDiv(14, dc.GetDeviceCaps(LOGPIXELSY), 72);
	AddStyle(3, RGB(49, 162, 165), RGB(255, 152, 29), COLORREF_NULL, lf);

	lf.lfHeight = -::MulDiv(18, dc.GetDeviceCaps(LOGPIXELSY), 72);
	AddStyle(4, RGB(119, 185, 0), RGB(255, 152, 29), COLORREF_NULL, lf);

	lf.lfHeight = -::MulDiv(28, dc.GetDeviceCaps(LOGPIXELSY), 72);
	AddStyle(5, RGB(66, 66, 66), RGB(255, 152, 29), COLORREF_NULL, lf);
}

CXTPTagCloudStyle* CXTPTagCloudStyleManager::GetStyle(int nScore)
{
	for (INT_PTR i = m_arrStyles.GetUpperBound(); i >= 0; i--)
	{
		CXTPTagCloudStyle* pStyle = m_arrStyles[i];

		if (pStyle->m_nScore == nScore)
		{
			return pStyle;
		}
	}
	return NULL;
}

BOOL CXTPTagCloudStyleManager::ScoreExists(int nScore)
{
	return (GetStyle(nScore) != NULL);
}

BOOL CXTPTagCloudStyleManager::SetColors(int nScore, COLORREF clrText, COLORREF clrTextHover,
										 COLORREF clrBorder /*=COLORREF_NULL*/)
{
	CXTPTagCloudStyle* pStyle = GetStyle(nScore);
	if (pStyle)
	{
		// color must be valid.
		if (clrText != COLORREF_NULL)
		{
			pStyle->m_clrText = clrText;
		}

		// color must be valid.
		if (clrTextHover != COLORREF_NULL)
		{
			pStyle->m_clrTextHover = clrTextHover;
		}

		// if COLORREF_NULL no border is drawn.
		pStyle->m_clrBorder = clrBorder;

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPTagCloudStyleManager::SetFont(int nScore, LOGFONT& lf)
{
	CXTPTagCloudStyle* pStyle = GetStyle(nScore);
	if (pStyle)
	{
		pStyle->SetFont(lf);
		return TRUE;
	}

	return FALSE;
}

//===========================================================================
// CXTPTagCloudTagArray
//===========================================================================

void CXTPTagCloudTagArray::Sort(XTPTagCloudSort nSort, BOOL bAsc)
{
	INT_PTR nSize = GetSize();
	if (nSize == 0)
		return;

	XTP_TAGCLOUDTAG** ppData = GetData();
	if (!ppData)
		return;

	switch (nSort)
	{
		case xtpTagCloudAlpha:
			qsort(ppData, XTPToSizeTChecked(nSize), sizeof(ppData[0]),
				  bAsc ? CompareNameAsc : CompareNameDesc);
			break;
		case xtpTagCloudNumeric:
			qsort(ppData, XTPToSizeTChecked(nSize), sizeof(ppData[0]),
				  bAsc ? CompareScoreAsc : CompareScoreDesc);
			break;
	}
}

int AFX_CDECL CXTPTagCloudTagArray::CompareName(XTP_TAGCLOUDTAG* pTagA, XTP_TAGCLOUDTAG* pTagB,
												BOOL bAsc)
{
	CString strText1 = bAsc ? pTagA->strItem : pTagB->strItem;
	CString strText2 = bAsc ? pTagB->strItem : pTagA->strItem;

	return lstrcmp(strText1, strText2);
}

int AFX_CDECL CXTPTagCloudTagArray::CompareScore(XTP_TAGCLOUDTAG* pTagA, XTP_TAGCLOUDTAG* pTagB,
												 BOOL bAsc)
{
	int nScore1 = bAsc ? pTagA->nScore : pTagB->nScore;
	int nScore2 = bAsc ? pTagB->nScore : pTagA->nScore;

	return (nScore1 == nScore2) ? 0 : (nScore1 < nScore2) ? -1 : 1;
}

void CXTPTagCloudTagArray::RemoveAll()
{
	CArray<XTP_TAGCLOUDTAG*, XTP_TAGCLOUDTAG*>::RemoveAll();
}

void CXTPTagCloudTagArray::RemoveAt(INT_PTR nIndex, INT_PTR nCount /*=1*/)
{
	CArray<XTP_TAGCLOUDTAG*, XTP_TAGCLOUDTAG*>::RemoveAt(nIndex, nCount);
}

//===========================================================================
// CXTPTagCloudCtrl
//===========================================================================

CXTPTagCloudCtrl::CXTPTagCloudCtrl()
	: m_bActive(FALSE)
	, m_bAsc(TRUE)
	, m_hCursor(NULL)
	, m_pHover(NULL)
	, m_pCurSel(NULL)
{
	RestoreDefaults();
	RegisterWindowClass();

	// try to load the system hand cursor.
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);

	// if not found, use the toolkit version
	if (m_hCursor == NULL)
		m_hCursor = XTPResourceManager()->LoadCursor(XTP_IDC_HAND);

	m_themeWrapper.OpenThemeData(0, L"BUTTON");
}

CXTPTagCloudCtrl::~CXTPTagCloudCtrl()
{
	RemoveAllInternal();
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTagCloudCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTPTagCloudCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

const TCHAR XTPTAGCLOUDCTRL_CLASSNAME[] = _T("XTPTagCloudCtrl");

BOOL CXTPTagCloudCtrl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, XTPTAGCLOUDCTRL_CLASSNAME,
											  CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);
}

BOOL CXTPTagCloudCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CWnd::Create(XTPTAGCLOUDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	if (!::IsWindow(m_wndToolTip.m_hWnd))
	{
		m_wndToolTip.Create(this, TTS_NOPREFIX);
		m_wndToolTip.AddTool(this);
	}

	return TRUE;
}

void CXTPTagCloudCtrl::AddTag(XTP_TAGCLOUDTAG* pTag)
{
	ASSERT(pTag);

	if (!pTag)
		return;

	m_arrTags.Add(pTag);

	if (m_nSortOrder != xtpTagCloudNone)
	{
		m_arrTagsSorted.RemoveAll();
		m_arrTagsSorted.Copy(m_arrTags);
		m_arrTagsSorted.Sort(m_nSortOrder, m_bAsc);
	}
}

void CXTPTagCloudCtrl::RemoveTag(XTP_TAGCLOUDTAG* pTag)
{
	ASSERT(pTag);

	if (!pTag)
		return;

	for (INT_PTR i = m_arrTags.GetUpperBound(); i >= 0; i--)
	{
		if (pTag == m_arrTags[i])
		{
			if (pTag == m_pCurSel)
				SetCurSel(NULL);

			if (pTag == m_pHover)
				m_pHover = NULL;

			SAFE_DELETE(pTag);
			m_arrTags.RemoveAt(i);
			break;
		}
	}

	if (m_nSortOrder != xtpTagCloudNone)
	{
		m_arrTagsSorted.RemoveAll();
		m_arrTagsSorted.Copy(m_arrTags);
		m_arrTagsSorted.Sort(m_nSortOrder, m_bAsc);
	}
}

void CXTPTagCloudCtrl::RemoveAll()
{
	SetCurSel(NULL);
	m_pHover = NULL;

	RemoveAllInternal();
}

void CXTPTagCloudCtrl::RemoveAllInternal()
{
	for (INT_PTR i = m_arrTags.GetUpperBound(); i >= 0; i--)
	{
		XTP_TAGCLOUDTAG* pTag = m_arrTags[i];
		SAFE_DELETE(pTag);
		m_arrTags.RemoveAt(i);
	}

	m_arrTagsSorted.RemoveAll();
}

XTP_TAGCLOUDTAG* CXTPTagCloudCtrl::HitTest(const CPoint& point)
{
	CXTPTagCloudTagArray& arrTags = GetTagArray();

	for (INT_PTR i = 0; i < arrTags.GetSize(); i++)
	{
		if (arrTags[i] && arrTags[i]->rcItem.PtInRect(point))
		{
			return arrTags[i];
		}
	}

	return NULL;
}

BOOL CXTPTagCloudCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

void CXTPTagCloudCtrl::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rcClient(this);

	CXTPBufferDC memDC(dc);
	OnDraw(&memDC, rcClient);
}

void CXTPTagCloudCtrl::OnNcPaint()
{
	if ((m_themeWrapper.IsAppThemeReady() == FALSE) || (m_clrBorder != COLORREF_NULL))
	{
		if (GetStyle() & WS_BORDER)
		{
			CXTPWindowRect rc(this);
			rc.OffsetRect(-rc.TopLeft());

			CWindowDC dc(this);

			if (m_clrBorder != COLORREF_NULL)
			{
				dc.Draw3dRect(0, 0, rc.Width(), rc.Height(), m_clrBorder, m_clrBorder);
			}
			else
			{
				dc.Draw3dRect(0, 0, rc.Width(), rc.Height(), ::GetSysColor(COLOR_3DSHADOW),
							  ::GetSysColor(COLOR_3DHIGHLIGHT));

				rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

				dc.Draw3dRect(XTP_DPI_X(1), XTP_DPI_Y(1), rc.Width(), rc.Height(),
							  ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DFACE));
			}

			return;
		}
	}

	CWnd::OnNcPaint();
}

void CXTPTagCloudCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if ((m_themeWrapper.IsAppThemeReady() == FALSE) || (m_clrBorder != COLORREF_NULL))
	{
		if (GetStyle() & WS_BORDER)
		{
			if (m_clrBorder != COLORREF_NULL)
			{
				// adjust non-client area for border space
				lpncsp->rgrc[0].left += XTP_DPI_X(1);
				lpncsp->rgrc[0].top += XTP_DPI_Y(1);
				lpncsp->rgrc[0].right -= XTP_DPI_X(1);
				lpncsp->rgrc[0].bottom -= XTP_DPI_Y(1);
				return;
			}
		}
	}

	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CXTPTagCloudCtrl::OnDraw(CDC* pDC, const CRect& rcClient)
{
	CXTPDCSaveState saveDC(pDC); // safe device context state.

	if (m_clrBack.clrDark == m_clrBack.clrLight)
	{
		pDC->FillSolidRect(rcClient, m_clrBack);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, &rcClient, m_clrBack, m_bHorz);
	}

	CXTPTagCloudTagArray& arrTags = GetTagArray();

	for (INT_PTR i = 0; i < arrTags.GetSize(); i++)
	{
		if (arrTags[i])
		{
			CXTPTagCloudStyle* pStyle = XTPTabCloudStyleManager()->GetStyle(arrTags[i]->nScore);
			ASSERT(pStyle != NULL);

			if (pStyle)
			{
				pDC->SelectObject(&pStyle->m_xtpFont);
				pDC->SetTextColor((m_pHover == arrTags[i]) ? pStyle->m_clrTextHover
														   : pStyle->m_clrText);
				pDC->SetBkMode(TRANSPARENT);
				pDC->DrawText(arrTags[i]->strItem, arrTags[i]->rcItem,
							  DT_VCENTER | DT_CENTER | DT_SINGLELINE);

				if (m_bFocusRect && m_pCurSel == arrTags[i])
				{
					pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
					pDC->DrawFocusRect(arrTags[i]->rcItem);
				}
				else
				{
					if (pStyle->m_clrBorder != COLORREF_NULL)
					{
						pDC->Draw3dRect(arrTags[i]->rcItem, pStyle->m_clrBorder,
										pStyle->m_clrBorder);
					}
				}
			}
		}
	}
}

void CXTPTagCloudCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	SetCurSel(HitTest(point));
}

void CXTPTagCloudCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
	OnNotify(HitTest(point), WM_XTP_TAGCLOUDDBLCLK);
}

void CXTPTagCloudCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);
	OnNotify(HitTest(point), WM_XTP_TAGCLOUDRIGHTCLK);
}

void CXTPTagCloudCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	RecalcLayout();
}

CSize CXTPTagCloudCtrl::CalcRowLayout(const CRect& rcClient, const CXTPTagCloudTagArray& arrRow)
{
	CSize szRow(0, 0);
	INT_PTR i = 0;

	// get max row height.
	for (i = 0; i < arrRow.GetSize(); i++)
	{
		XTP_TAGCLOUDTAG* pTag = arrRow[i];
		if (!pTag)
			continue;

		CSize szTag = pTag->rcItem.Size();

		szRow.cy = max(szRow.cy, szTag.cy);
		szRow.cx += szTag.cx + m_nPadding;
	}

	// determine the left starting point.
	int x = rcClient.left + ((rcClient.Width() - szRow.cx) / 2);

	// calculate each tag position.
	for (i = 0; i < arrRow.GetSize(); i++)
	{
		XTP_TAGCLOUDTAG* pTag = arrRow[i];
		if (!pTag)
			continue;

		int cy = pTag->rcItem.Height();
		int cx = pTag->rcItem.Width();

		pTag->rcItem.top	= pTag->rcItem.top + ((szRow.cy - cy) / 2);
		pTag->rcItem.bottom = pTag->rcItem.top + cy;
		pTag->rcItem.left   = x;
		pTag->rcItem.right  = pTag->rcItem.left + cx;

		x = pTag->rcItem.right + m_nPadding;
	}

	return szRow;
}

void CXTPTagCloudCtrl::RecalcLayout()
{
	CWindowDC dc(NULL);

	CXTPClientRect rcClient(this);
	rcClient.DeflateRect(m_nMargin, m_nMargin);

	int cy = 0;

	CPoint pt(rcClient.TopLeft());

	CXTPTagCloudTagArray arrRow;

	CXTPTagCloudTagArray& arrTags = GetTagArray();
	INT_PTR nSize				  = arrTags.GetSize();

	for (INT_PTR i = 0; i < nSize; i++)
	{
		XTP_TAGCLOUDTAG* pTag = arrTags[i];
		if (!pTag)
			continue;

		CXTPTagCloudStyle* pStyle = XTPTabCloudStyleManager()->GetStyle(pTag->nScore);
		ASSERT(pStyle != NULL);

		if (!pStyle)
			continue;

		CXTPFontDC fontDC(&dc, &pStyle->m_xtpFont);

		CRect rcWord(0, 0, 0, 0);
		dc.DrawText(pTag->strItem, rcWord, DT_CENTER | DT_SINGLELINE | DT_CALCRECT);

		if (pt.x + rcWord.Width() + m_nPadding > rcClient.right)
		{
			pt.y += CalcRowLayout(rcClient, arrRow).cy;
			pt.x = rcClient.left;

			arrRow.RemoveAll();
			cy = 0;
		}

		pTag->rcItem.top	= pt.y;
		pTag->rcItem.bottom = pt.y + rcWord.Height();
		pTag->rcItem.left   = pt.x;
		pTag->rcItem.right  = pt.x + rcWord.Width();

		arrRow.Add(pTag);

		pt.x = pTag->rcItem.right + m_nPadding;

		cy = max(cy, rcWord.Height());
	}

	CalcRowLayout(rcClient, arrRow);
	RedrawWindow();
}

void CXTPTagCloudCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
#ifdef _XTP_ACTIVEX
	MSG ms;
	DWORD dw  = point.y;
	DWORD dwY = dw << 16;
	dw		  = point.x;
	dw |= dwY;
	ms.message = WM_MOUSEMOVE;
	ms.hwnd	= m_hWnd;
	ms.wParam  = nFlags;
	ms.lParam  = dw;
	ms.time	= GetTickCount();
	ms.pt.x	= point.x;
	ms.pt.y	= point.y;
	m_wndToolTip.RelayEvent(&ms);
#endif

	CWnd::OnMouseMove(nFlags, point);

	XTP_TAGCLOUDTAG* pHover = HitTest(point);

	if (pHover != m_pHover)
	{
		m_pHover = pHover;

		if (m_bTipEnabled)
		{
			if (m_pHover)
			{
				CString strToolTip;
				strToolTip.Format(XTP_IDS_TAGCLOUD_TIP, m_pHover->strItem.operator LPCTSTR(),
								  m_pHover->nScore);

				m_wndToolTip.Activate(TRUE);
				m_wndToolTip.UpdateTipText(strToolTip, this);
			}
			else
			{
				m_wndToolTip.Activate(FALSE);
				m_wndToolTip.UpdateTipText(_T(""), this);
			}
		}
	}

	SetActive(PointInRect());
}

BOOL CXTPTagCloudCtrl::PointInRect()
{
	if (::IsWindow(m_hWnd))
	{
		CXTPWindowRect rc(this);

		CPoint pt;
		GetCursorPos(&pt);

		return rc.PtInRect(pt);
	}

	return FALSE;
}

void CXTPTagCloudCtrl::SetActive(BOOL bActive)
{
	if (bActive == FALSE)
	{
		m_pHover = NULL;
	}

	if (m_bActive != bActive)
	{
		m_bActive = bActive;

		if (m_bActive)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT };
			_TrackMouseEvent(&tme);
		}
	}

	RedrawWindow();
}

void CXTPTagCloudCtrl::OnMouseLeave()
{
	Default();
	SetActive(FALSE);
}

BOOL CXTPTagCloudCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if (HitTest(point) && m_hCursor)
	{
		SetCursor(m_hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CXTPTagCloudCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (::IsWindow(m_wndToolTip.m_hWnd) && m_bTipEnabled)
	{
		m_wndToolTip.RelayEvent(pMsg);
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CXTPTagCloudCtrl::EnableTooltip(BOOL bTipEnabled)
{
	m_bTipEnabled = bTipEnabled;

	if (::IsWindow(m_wndToolTip.m_hWnd))
	{
		m_wndToolTip.Activate(m_bTipEnabled);
	}
}

BOOL CXTPTagCloudCtrl::OnNotify(XTP_TAGCLOUDTAG* pTag, UINT nEventCode)
{
	if (!::IsWindow(m_hWnd))
		return FALSE;

	if (nEventCode == WM_XTP_TAGCLOUDDBLCLK || nEventCode == WM_XTP_TAGCLOUDRIGHTCLK
		|| nEventCode == WM_XTP_TAGCLOUDSELCHANGE)
	{
		m_pCurSel = pTag;

		SetFocus();
		RedrawWindow();

		CWnd* pOwner = GetOwner();
		ASSERT(pOwner);

		if (::IsWindow(pOwner->GetSafeHwnd()))
		{
			pOwner->SendMessage(nEventCode, (WPARAM)(XTP_TAGCLOUDTAG*)m_pCurSel);
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPTagCloudCtrl::SetCurSel(XTP_TAGCLOUDTAG* pCurSel)
{
	if (m_pCurSel != pCurSel)
	{
		// if selection has changed notify owner.
		OnNotify(pCurSel, WM_XTP_TAGCLOUDSELCHANGE);
	}
}

void CXTPTagCloudCtrl::SetSortOrder(XTPTagCloudSort nSortOrder, BOOL bAsc)
{
	m_bAsc		 = bAsc;
	m_nSortOrder = nSortOrder;

	switch (m_nSortOrder)
	{
		case xtpTagCloudNone: { m_arrTagsSorted.RemoveAll();
		}
		break;
		case xtpTagCloudAlpha:
		case xtpTagCloudNumeric:
		{
			m_arrTagsSorted.RemoveAll();
			m_arrTagsSorted.Copy(m_arrTags);
			m_arrTagsSorted.Sort(m_nSortOrder, m_bAsc);
		}
		break;
	}
	if (GetSafeHwnd())
		RecalcLayout();
}

CXTPTagCloudTagArray& CXTPTagCloudCtrl::GetTagArray()
{
	return (m_nSortOrder == xtpTagCloudNone) ? m_arrTags : m_arrTagsSorted;
}

void CXTPTagCloudCtrl::RestoreDefaults()
{
	m_nMargin	 = XTP_DPI_X(8);
	m_nPadding	= XTP_DPI_X(4);
	m_bHorz		  = FALSE;
	m_bTipEnabled = TRUE;
	m_bFocusRect  = TRUE;
	m_nSortOrder  = xtpTagCloudNone;

	m_clrBack.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	m_clrBack.SetCustomValue(::GetSysColor(COLOR_WINDOW));
	m_clrBorder.SetStandardValue(COLORREF_NULL);
}

//===========================================================================
// XTPTabCloudStyleManager()
//===========================================================================

CXTPTagCloudStyleManager* AFX_CDECL XTPTabCloudStyleManager()
{
	return &CXTPSingleton<CXTPTagCloudStyleManager>::Instance();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTagCloudStyleManager, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "InitDefaultStyles", 1, OleInitDefaultStyles,
					 VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "Remove", 3, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "RemoveAll", 4, OleRemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "GetStyle", 5, OleGet, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "SetStyle", 6, OleSetStyle, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "ScoreExists", 7, OleScoreExists, VT_BOOL, VTS_I4)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "CreateStyle", 8, OleCreateStyle, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "Count", 9, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTagCloudStyleManager, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPTagCloudStyleManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPTagCloudStyleManager, XTPDIID_ITagCloudsStyles, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTagCloudStyleManager, XTPDIID_ITagCloudsStyles)
IMPLEMENT_ENUM_VARIANT(CXTPTagCloudStyleManager)

void CXTPTagCloudStyleManager::OleInitDefaultStyles()
{
	InitDefaultStyles();
}

void CXTPTagCloudStyleManager::OleAdd(LPDISPATCH pStyle)
{
	LPDISPATCH pTagStyleDisp = NULL;
	HRESULT hr = pStyle->QueryInterface(XTPDIID_ITagCloudStyle, (void**)&pTagStyleDisp);
	if (FAILED(hr))
	{
		AfxThrowOleException(hr);
	}

	if (!pTagStyleDisp)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	CTagCloudStyle* pTagStyle = XTP_DYNAMIC_DOWNCAST_REMOTE_(CTagCloudStyle,
															 CTagCloudStyle::FromIDispatchSafe(
																 pTagStyleDisp));
	if (!pTagStyle)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	pTagStyle->m_nScore = static_cast<int>(m_arrStyles.GetSize()) + 1;
	LOGFONT logFont;
	pTagStyle->m_Font.GetLogFont(&logFont);
	AddStyle(pTagStyle->m_nScore, pTagStyle->m_TextColor, pTagStyle->m_TextHoverColor,
			 pTagStyle->m_BorderColor, logFont);
	pTagStyleDisp->Release();
}

void CXTPTagCloudStyleManager::OleRemove(long Score)
{
	RemoveStyle(Score);
}

void CXTPTagCloudStyleManager::OleRemoveAll()
{
	RemoveAll();
}

LPDISPATCH CXTPTagCloudStyleManager::OleGet(long Score)
{
	CXTPTagCloudStyle* pStyle	= GetStyle(Score);
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CTagCloudStyle);
	CObject* pObject			 = pRuntimeClass->CreateObject();
	CTagCloudStyle* pDispStyle   = DYNAMIC_DOWNCAST(CTagCloudStyle, pObject);
	pDispStyle->m_nScore		 = pStyle->m_nScore;
	pDispStyle->m_TextColor		 = pStyle->m_clrText;
	pDispStyle->m_TextHoverColor = pStyle->m_clrTextHover;
	pDispStyle->m_BorderColor	= pStyle->m_clrBorder;
	LOGFONT lf;
	pStyle->m_xtpFont.GetLogFont(&lf);
	pDispStyle->m_Font.DeleteObject();
	pDispStyle->m_Font.CreateFontIndirect(&lf);
	return XTPGetDispatch(pDispStyle, FALSE);
}

void CXTPTagCloudStyleManager::OleSetStyle(LPDISPATCH pStyle)
{
	LPDISPATCH pTagStyleDisp = NULL;
	HRESULT hr = pStyle->QueryInterface(XTPDIID_ITagCloudStyle, (void**)&pTagStyleDisp);
	if (FAILED(hr))
	{
		AfxThrowOleException(hr);
	}

	if (!pTagStyleDisp)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	CTagCloudStyle* pTagStyle = XTP_DYNAMIC_DOWNCAST_REMOTE_(CTagCloudStyle,
															 CTagCloudStyle::FromIDispatchSafe(
																 pTagStyleDisp));
	if (!pTagStyle)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	if (!ScoreExists(pTagStyle->m_nScore))
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	LOGFONT logFont;
	pTagStyle->m_Font.GetLogFont(&logFont);
	SetColors(pTagStyle->m_nScore, pTagStyle->m_TextColor, pTagStyle->m_TextHoverColor,
			  pTagStyle->m_BorderColor);
	SetFont(pTagStyle->m_nScore, logFont);
	pTagStyleDisp->Release();
}

BOOL CXTPTagCloudStyleManager::OleScoreExists(long Score)
{
	return ScoreExists(Score);
}

LPDISPATCH CXTPTagCloudStyleManager::OleCreateStyle()
{
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CTagCloudStyle);
	CObject* pObject			 = pRuntimeClass->CreateObject();
	CTagCloudStyle* pDispStyle   = DYNAMIC_DOWNCAST(CTagCloudStyle, pObject);
	pDispStyle->m_nScore		 = -1;
	pDispStyle->m_BorderColor	= COLORREF_NULL;
	pDispStyle->m_TextColor		 = RGB(66, 66, 66);
	pDispStyle->m_TextHoverColor = RGB(255, 152, 29);
	pDispStyle->m_Font.DeleteObject();
	LOGFONT lf;
	XTPFontManager()->GetFont().GetLogFont(&lf);
	pDispStyle->m_Font.DeleteObject();
	pDispStyle->m_Font.CreateFontIndirect(&lf);
	return XTPGetDispatch(pDispStyle, FALSE);
}

void CXTPTagCloudStyleManager::OnFinalRelease()
{
	CXTPCmdTarget::OnFinalRelease();
}

long CXTPTagCloudStyleManager::OleGetItemCount()
{
	return static_cast<long>(m_arrStyles.GetSize());
}

LPDISPATCH CXTPTagCloudStyleManager::OleGetItem(long nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrStyles.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CTagCloudStyle);
	CObject* pObject			 = pRuntimeClass->CreateObject();
	CTagCloudStyle* pDispStyle   = DYNAMIC_DOWNCAST(CTagCloudStyle, pObject);
	pDispStyle->m_nScore		 = m_arrStyles[nIndex]->m_nScore;
	pDispStyle->m_BorderColor	= m_arrStyles[nIndex]->m_clrBorder;
	pDispStyle->m_TextColor		 = m_arrStyles[nIndex]->m_clrText;
	pDispStyle->m_TextHoverColor = m_arrStyles[nIndex]->m_clrTextHover;
	pDispStyle->m_Font.DeleteObject();
	LOGFONT lf;
	m_arrStyles[nIndex]->m_xtpFont.GetLogFont(&lf);
	pDispStyle->m_Font.DeleteObject();
	pDispStyle->m_Font.CreateFontIndirect(&lf);
	return XTPGetDispatch(pDispStyle, FALSE);
}

#endif /*_XTP_ACTIVEX*/
