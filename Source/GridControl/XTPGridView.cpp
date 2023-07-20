// XTPGridView.cpp : implementation of the CXTPGridView class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "GridControl/Resource.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridView.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridSection.h"
#include "GridControl/XTPGridSelectedRows.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridPageSetupDialog.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPGridView

IMPLEMENT_DYNCREATE(CXTPGridView, CView)

CXTPGridView::CXTPGridView()
	: m_pScrollBarVer(NULL)
	, m_pScrollBarHor(NULL)
	, m_pSlider(NULL)

	, m_nColumnStart(0)
	, m_nColumnEnd(0)
{
	m_pGrid			= NULL;
	m_pPrintOptions = new CXTPGridViewPrintOptions();

	m_bPrintSelection = FALSE;
	m_bPaginated	  = FALSE;

	m_bPrintDirect			 = FALSE;
	m_bResizeControlWithView = TRUE;

	m_bAllowCut   = TRUE;
	m_bAllowPaste = TRUE;

	m_nStartIndex = 0;
	m_bSwitchMode = FALSE;
}

CXTPGridView::~CXTPGridView()
{
	CMDTARGET_RELEASE(m_pPrintOptions);
	SAFE_DELETE(m_pGrid);
}

void CXTPGridView::SetTheme(XTPGridPaintTheme paintTheme, BOOL bEnableMetrics)
{
	switch (paintTheme)
	{
		case xtpGridThemeResource:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeResource);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeResource);
			break;

		case xtpGridThemeVisualStudio2012Light:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2012Light);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2012Light);
			break;

		case xtpGridThemeVisualStudio2012Dark:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2012Dark);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2012Dark);
			break;

		case xtpGridThemeVisualStudio2012Blue:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2012Light);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2012Light);
			break;

		case xtpGridThemeOffice2013:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeOffice2013);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeOffice2013);
			break;

		case xtpGridThemeVisualStudio2015:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2015);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2015);
			break;

		case xtpGridThemeVisualStudio2017:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2017);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2017);
			break;

		case xtpGridThemeVisualStudio2019:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2019);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2019);
			break;

		case xtpGridThemeVisualStudio2022:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2022);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeVisualStudio2022);
			break;

		case xtpGridThemeNativeWindows10:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeNativeWindows10);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeNativeWindows10);
			break;

		default:
			m_wndVScrollBar.SetTheme(xtpScrollBarThemeWindowsDefault);
			m_wndHScrollBar.SetTheme(xtpScrollBarThemeWindowsDefault);
			break;
	}

	GetGridCtrl().SetTheme(paintTheme, bEnableMetrics);
}

CXTPGridControl& CXTPGridView::GetGridCtrl() const
{
	return m_pGrid == NULL ? (CXTPGridControl&)m_wndGrid : *m_pGrid;
}

void CXTPGridView::SetGridCtrl(CXTPGridControl* pGrid)
{
	if (::IsWindow(m_wndGrid.GetSafeHwnd()))
		m_wndGrid.DestroyWindow();

	m_pGrid = pGrid;
}

void CXTPGridView::SetScrollBarCtrl(CScrollBar* pScrollBar, BOOL bHor)
{
	if (bHor)
	{
		m_pScrollBarHor = pScrollBar;
	}
	else
	{
		m_pScrollBarVer = pScrollBar;
	}
}

void CXTPGridView::SetSliderCtrl(CSliderCtrl* pSlider)
{
	m_pSlider = pSlider;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridView, CView)
	//{{AFX_MSG_MAP(CXTPGridView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPGridView drawing

void CXTPGridView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPGridView::OnPaint()
{
	Default();
}

BOOL CXTPGridView::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPGridView diagnostics

#ifdef _DEBUG
void CXTPGridView::AssertValid() const
{
	CView::AssertValid();
}

void CXTPGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXTPGridView message handlers

CScrollBar* CXTPGridView::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_HORZ && m_pScrollBarHor)
		return m_pScrollBarHor;

	if (nBar == SB_VERT && m_pScrollBarVer)
		return m_pScrollBarVer;

	return CView::GetScrollBarCtrl(nBar);
}

void CXTPGridView::UpdateScrollBars()
{
	int nScroll = GetSystemMetrics(SM_CXVSCROLL);
	// SM_CXVSCROLL SM_CYHSCROLL SM_CXBORDER SM_CYBORDER
	int dx = 0;
	if (GetPaintManager()->m_bMoveScrollbarOnFixedColumnsIndent)
		dx = GetPaintManager()->m_nFixedColumnsIndent;

	BOOL bV = (m_pScrollBarVer != NULL) && (m_pScrollBarVer->GetSafeHwnd() != NULL)
			  && 0 != (m_pScrollBarVer->GetStyle() & WS_VISIBLE);
	BOOL bH = (m_pScrollBarHor != NULL) && (m_pScrollBarHor->GetSafeHwnd() != NULL)
			  && 0 != (m_pScrollBarHor->GetStyle() & WS_VISIBLE);

	CRect rc;
	GetClientRect(&rc);
	int cx = rc.Width();
	int cy = rc.Height();

	int CX = cx;
	int CY = cy;

	if (bV || bH)
	{
		if (bV && bH)
		{
			cx -= nScroll;
			cy -= nScroll;
		}
		else if (bV)
		{
			cx -= nScroll;
		}
		else if (bH)
		{
			cy -= nScroll;
		}

		// Right-to-left
		if (GetGridCtrl().IsLayoutRTL())
		{
			if (bV)
			{
				m_pScrollBarVer->MoveWindow(0, 0, nScroll, cy);
			}
			if (bH)
			{
				m_pScrollBarHor->MoveWindow(0, cy, CX - dx, nScroll);

				if (WS_EX_LAYOUTRTL != (m_pScrollBarHor->GetExStyle() & WS_EX_LAYOUTRTL))
				{
					// Turn RTL on
					m_pScrollBarHor->ModifyStyleEx(0, WS_EX_LAYOUTRTL);
				}
			}

			if (m_pSlider)
			{
				if (dx)
				{
					m_pSlider->MoveWindow(CX - dx, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pSlider->MoveWindow(CX - dx, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_HIDE);
				}
			}
		}
		// Left-to-right
		else
		{
			if (bV)
			{
				m_pScrollBarVer->MoveWindow(cx, 0, nScroll, cy);
			}
			if (bH)
			{
				m_pScrollBarHor->MoveWindow(dx, cy, CX - dx, nScroll);

				if (WS_EX_LAYOUTRTL == (m_pScrollBarHor->GetExStyle() & WS_EX_LAYOUTRTL))
				{
					// Turn RTL off
					m_pScrollBarHor->ModifyStyleEx(WS_EX_LAYOUTRTL, 0);
				}
			}

			if (m_pSlider)
			{
				if (dx)
				{
					m_pSlider->MoveWindow(0, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pSlider->MoveWindow(0, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	else
	{
		nScroll = 0;
	}

	if (m_bResizeControlWithView && GetGridCtrl().GetSafeHwnd())
	{
		if (nScroll > 0)
		{
			if (GetGridCtrl().IsLayoutRTL())
			{
				if (bV)
					GetGridCtrl().MoveWindow(nScroll, 0, cx, cy);
				else
					GetGridCtrl().MoveWindow(0, 0, cx, cy);
			}
			else
			{
				if (bV)
					GetGridCtrl().MoveWindow(0, 0, cx, cy);
				else
					GetGridCtrl().MoveWindow(0, 0, cx, cy);
			}
		}
		else
		{
			GetGridCtrl().MoveWindow(0, 0, CX, CY);
		}
	}
}

void CXTPGridView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pScrollBarVer && pScrollBar == m_pScrollBarVer)
	{
		GetGridCtrl().OnVScroll(nSBCode, nPos, 0);
	}

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CXTPGridView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pScrollBarHor && pScrollBar == m_pScrollBarHor)
	{
		GetGridCtrl().OnHScroll(nSBCode, nPos, 0);
	}

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CXTPGridView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		UpdateScrollBars();
	}
}

void CXTPGridView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	UpdateScrollBars();
}

BOOL CXTPGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

int CXTPGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create scrollbars
	m_wndVScrollBar.Create(WS_CHILD | SBS_VERT, CRect(0, 0, 0, 0), this, 100);
	m_wndHScrollBar.Create(WS_CHILD | SBS_HORZ, CRect(0, 0, 0, 0), this, 100);

	// Set scrollbars
	SetScrollBarCtrl(&m_wndVScrollBar, FALSE);
	SetScrollBarCtrl(&m_wndHScrollBar, TRUE);

	// Create Grid controls
	if (!GetGridCtrl().Create(WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP
								  | WS_VISIBLE,
							  CRect(0, 0, 0, 0), this, XTP_ID_GRID_CONTROL))
	{
		TRACE(_T("Failed to create Grid control window\n"));
		return -1;
	}

	return 0;
}

void CXTPGridView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	if (IsWindow(GetGridCtrl().GetSafeHwnd()))
		GetGridCtrl().SetFocus();
}

/////////////////////////////////////////////////////////////////////////////

BOOL CXTPGridView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if (GetGridCtrl().IsIconView())
	{
		GetGridCtrl().SetIconView(FALSE);
		m_bSwitchMode = TRUE;
	}

	m_bShowRowNumber = GetGridCtrl().IsShowRowNumber();
	GetGridCtrl().ShowRowNumber(FALSE);

	if (GetGridCtrl().GetSelectedRows()->GetCount() > 0)
		pInfo->m_pPD->m_pd.Flags &= ~PD_NOSELECTION;

	pInfo->m_bDirect = m_bPrintDirect;

	// default preparation
	if (!DoPreparePrinting(pInfo))
		return FALSE;

	m_bPrintSelection = pInfo->m_pPD->PrintSelection();

	return TRUE;
}

void CXTPGridView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_aPageStart.RemoveAll();

	if (m_pPrintOptions->m_bBlackWhitePrinting)
		m_bmpGrayDC.DeleteObject();

	if (m_bSwitchMode)
	{
		GetGridCtrl().SetIconView(TRUE);
		m_bSwitchMode = FALSE;
	}
	GetGridCtrl().ShowRowNumber(m_bShowRowNumber);
}

CXTPGridPaintManager* CXTPGridView::GetPaintManager() const
{
	return GetGridCtrl().GetPaintManager();
}

void CXTPGridView::PrintHeader(CDC* pDC, CRect rcHeader)
{
	CXTPGridColumns* pColumns = GetGridCtrl().GetColumns();

	GetPaintManager()->FillHeaderControl(pDC, rcHeader);

	int x = rcHeader.left;
	int nWidth;

	for (int nColumn = m_nColumnStart; nColumn < m_nColumnEnd; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn->IsVisible())
		{
			if (GetPaintManager()->IsColumnWidthWYSIWYG())
			{
				nWidth = pColumn->GetWidth();
			}
			else
			{
				nWidth = pColumn->GetPrintWidth(rcHeader.Width());
			}
			CRect rcItem(x, rcHeader.top, x + nWidth, rcHeader.bottom);
			GetPaintManager()->DrawColumn(pDC, pColumn, GetGridCtrl().GetGridHeader(), rcItem);
			x += nWidth;
		}
	}
}

void CXTPGridView::PrintFooter(CDC* pDC, CRect rcFooter)
{
	CXTPGridColumns* pColumns = GetGridCtrl().GetColumns();

	GetPaintManager()->FillFooter(pDC, rcFooter);

	int x = rcFooter.left;
	int nWidth(0);
	for (int nColumn = m_nColumnStart; nColumn < m_nColumnEnd; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (!pColumn->IsVisible())
			continue;

		if (GetPaintManager()->IsColumnWidthWYSIWYG())
			nWidth = pColumn->GetWidth();
		else
			nWidth = pColumn->GetPrintWidth(rcFooter.Width());
		CRect rcItem(x, rcFooter.top, x + nWidth, rcFooter.bottom);
		GetPaintManager()->DrawColumnFooter(pDC, pColumn, GetGridCtrl().GetGridHeader(), rcItem);
		x += nWidth;
	}
}

int CXTPGridView::PrintGrid(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, int nRowFrom,
							int nColumnFrom, int nColumnTo)
{
	CXTPGridControl& wndGrid = GetGridCtrl();

	int nMaxPages = -1;
	if (pInfo->GetMaxPage() != 65535)
		nMaxPages = XTPToIntChecked(pInfo->GetMaxPage());

	if (GetPaintManager()->m_bPrintWatermark)
	{
		CRect rcWater(rcPage);
		CRect rcClient(0, 0, rcWater.Width(), rcWater.Height());

		wndGrid.DrawWatermark(pDC, rcWater, rcClient);
	}

	int nHeaderHeight = PrintPageHeader(pDC, pInfo, rcPage, FALSE,
										XTPToIntChecked(pInfo->m_nCurPage), nMaxPages,
										XTPToIntChecked(m_nStartIndex),
										(int)GetPaintManager()->m_arStartCol.GetSize() - 1);

	int nFooterHeight = PrintPageFooter(pDC, pInfo, rcPage, FALSE,
										XTPToIntChecked(pInfo->m_nCurPage), nMaxPages,
										XTPToIntChecked(m_nStartIndex),
										(int)GetPaintManager()->m_arStartCol.GetSize() - 1);

	CRect rcRows(rcPage);

	if (GetPaintManager()->m_bPrintPageRectangle)
	{
		rcRows.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	}

	rcRows.top += nHeaderHeight;
	rcRows.bottom -= nFooterHeight;
	int nFooterTop = rcRows.bottom;

	// print header rows (if exist and visible)
	if (wndGrid.GetHeaderRows()->GetCount() > 0 && GetGridCtrl().IsHeaderRowsVisible())
	{
		// print on the first page, at least
		if (0 == nRowFrom || m_pPrintOptions->m_bRepeatHeaderRows)
		{
			int nHeight = 0;
			wndGrid.DrawRows(pDC, rcRows, rcRows.top, wndGrid.GetHeaderRows(), 0, nColumnFrom,
							 nColumnTo, &nHeight);
			rcRows.top += nHeight;

			// print divider
			rcRows.top += PrintFixedRowsDivider(pDC, rcRows, TRUE);
		}
	}

	// calculate space for footer rows
	int nNeedForFooterRows = 0;

	if (wndGrid.GetFooterRows()->GetCount() > 0 && GetGridCtrl().IsFooterRowsVisible())
	{
		nNeedForFooterRows = wndGrid.GetRowsHeight(GetGridCtrl().GetFooterRows(), rcRows.Width());
		nNeedForFooterRows += GetPaintManager()->GetSectionDividerHeight(
			wndGrid.m_pSectionFooter->GetDividerStyle());

		if (m_pPrintOptions->m_bRepeatFooterRows)
		{
			// decrease space for body rows, if footer rows to be repeated on every page.
			rcRows.bottom = nFooterTop - nNeedForFooterRows;
		}
	}

	// print body rows
	int nPrintedBodyRowsHeight = 0;
	nRowFrom = wndGrid.DrawRows(pDC, rcRows, rcRows.top, wndGrid.GetRows(), nRowFrom, nColumnFrom,
								nColumnTo, &nPrintedBodyRowsHeight);

	// print footer rows (if exist and visible)
	if (nNeedForFooterRows > 0)
	{
		CRect rcFooterRows(rcRows);

		if (wndGrid.IsFooterRowsPinnedPrinted())
		{
			rcFooterRows.top = rcRows.top + nPrintedBodyRowsHeight; // immediately after body rows
			rcFooterRows.bottom = nFooterTop;
		}
		else
		{
			rcFooterRows.top	= nFooterTop - nNeedForFooterRows;
			rcFooterRows.bottom = nFooterTop;
		}

		// one more check, if there is enough space for footer divider + footer rows
		if (rcFooterRows.Height() >= nNeedForFooterRows)
		{
			// print divider
			rcFooterRows.top += PrintFixedRowsDivider(pDC, rcFooterRows, FALSE);

			// print footer rows
			int nHeight = 0;
			wndGrid.DrawRows(pDC, rcFooterRows, rcFooterRows.top, wndGrid.GetFooterRows(), 0,
							 nColumnFrom, nColumnTo, &nHeight);
		}
	}

	if (GetPaintManager()->m_bPrintPageRectangle)
	{
		pDC->Draw3dRect(rcPage, 0, 0);
	}

	return nRowFrom;
}

int CXTPGridView::PrintFixedRowsDivider(CDC* pDC, const CRect& rc, BOOL bHeaderRows)
{
	CRect rcDivider(rc);

	XTPGridSectionDividerPosition dividerPosition =
		bHeaderRows ? GetGridCtrl().m_pSectionHeader->GetDividerPosition()
					: GetGridCtrl().m_pSectionFooter->GetDividerPosition();

	XTPGridFixedRowsDividerStyle dividerStyle =
		bHeaderRows ? GetGridCtrl().m_pSectionHeader->GetDividerStyle()
					: GetGridCtrl().m_pSectionFooter->GetDividerStyle();

	CXTPPaintManagerColor dividerColor = bHeaderRows
											 ? GetGridCtrl().m_pSectionHeader->GetDividerColor()
											 : GetGridCtrl().m_pSectionFooter->GetDividerColor();

	int nHeight = GetPaintManager()->GetSectionDividerHeight(dividerStyle);

	rcDivider.bottom = rcDivider.top + nHeight;

	GetPaintManager()->DrawSectionDivider(pDC, rcDivider, dividerPosition, dividerStyle,
										  dividerColor);

	return nHeight;
}

int CXTPGridView::PrintPage(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, int nRowFrom,
							int nColumnFrom, int nColumnTo)
{
	if (!m_pPrintOptions || !pDC || !pInfo)
	{
		ASSERT(FALSE);
		return INT_MAX;
	}

	CRect rcPageHeader = rcPage;
	CRect rcPageFooter = rcPage;

	CString strTitle = CXTPPrintPageHeaderFooter::GetParentFrameTitle(this);

	int N = (int)GetPaintManager()->m_arStartCol.GetSize() - 1;
	int nVirPage(0);
	if (N > 0)
		nVirPage = XTPToIntChecked(1 + m_nStartIndex);

	if (GetPaintManager()->m_bPrintVirtualPageNumber)
	{
		m_pPrintOptions->GetPageHeader()->FormatTexts(pInfo, strTitle, nVirPage);
		m_pPrintOptions->GetPageFooter()->FormatTexts(pInfo, strTitle, nVirPage);
	}
	else
	{
		m_pPrintOptions->GetPageHeader()->FormatTexts(pInfo, strTitle);
		m_pPrintOptions->GetPageFooter()->FormatTexts(pInfo, strTitle);
	}

	m_pPrintOptions->GetPageHeader()->Draw(pDC, rcPageHeader);
	m_pPrintOptions->GetPageFooter()->Draw(pDC, rcPageFooter);

	CRect rcGrid = rcPage;
	rcGrid.top += rcPageHeader.Height() + XTP_DPI_Y(2);
	rcGrid.bottom -= rcPageFooter.Height() + XTP_DPI_Y(2);

	nRowFrom = PrintGrid(pDC, pInfo, rcGrid, nRowFrom, nColumnFrom, nColumnTo);

	return nRowFrom;
}

int CXTPGridView::PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, BOOL bOnlyCalculate,
								  int nPageNumber, int nNumberOfPages, int nHorizontalPageNumber,
								  int nNumberOfHorizontalPages)
{
	UNREFERENCED_PARAMETER(pInfo);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfPages);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);
	UNREFERENCED_PARAMETER(bOnlyCalculate);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);

	int nHeaderHeight = 0;

	if (GetPaintManager()->m_bPrintPageRectangle)
		rcPage.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

	if (GetGridCtrl().IsHeaderVisible())
		nHeaderHeight = GetPaintManager()->GetHeaderHeight(&GetGridCtrl(), pDC, rcPage.Width());

	rcPage.bottom = rcPage.top + nHeaderHeight;

	if (nHeaderHeight)
		PrintHeader(pDC, rcPage);

	return nHeaderHeight;
}

int CXTPGridView::PrintPageFooter(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, BOOL bOnlyCalculate,
								  int nPageNumber, int nNumberOfPages, int nHorizontalPageNumber,
								  int nNumberOfHorizontalPages)
{
	UNREFERENCED_PARAMETER(pInfo);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfPages);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);
	UNREFERENCED_PARAMETER(bOnlyCalculate);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);

	int nFooterHeight = 0;

	if (GetPaintManager()->m_bPrintPageRectangle)
		rcPage.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

	if (GetGridCtrl().IsFooterVisible())
		nFooterHeight = GetPaintManager()->GetFooterHeight(&GetGridCtrl(), pDC, rcPage.Width());

	rcPage.top = rcPage.bottom - nFooterHeight;

	if (nFooterHeight)
		PrintFooter(pDC, rcPage);

	return nFooterHeight;
}

int CXTPGridView::SetupStartCol(CDC* pDC, CPrintInfo* pInfo)
{
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(96, 96);
	pDC->OffsetWindowOrg(0, 0);

	pInfo->m_rectDraw.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	pDC->DPtoLP(&pInfo->m_rectDraw);

	CRect rcMargins = m_pPrintOptions->GetMarginsLP(pDC);
	CRect rc		= pInfo->m_rectDraw;
	rc.DeflateRect(rcMargins);
	GetPaintManager()->m_PrintPageWidth = rc.Width();
	GetPaintManager()->m_arStartCol.RemoveAll();
	GetPaintManager()->m_arStartCol.Add(0);
	CXTPGridColumns* pColumns = GetGridCtrl().GetColumns();
	int nColumnCount		  = pColumns->GetCount();

	if (GetPaintManager()->IsColumnWidthWYSIWYG())
	{
		int x = 0;
		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
			if (pColumn && pColumn->IsVisible())
			{
				int nWidth = pColumn->GetWidth();

				if (x + nWidth <= GetPaintManager()->m_PrintPageWidth)
				{
					x += nWidth;
				}
				else
				{
					GetPaintManager()->m_arStartCol.Add(XTPToUIntChecked(nColumn));
					x = nWidth;
				}
			}
		}
	}
	GetPaintManager()->m_arStartCol.Add(XTPToUIntChecked(nColumnCount));

	return (int)GetPaintManager()->m_arStartCol.GetSize() - 1;
}

// struct CPrintInfo // Printing information structure
//  CPrintDialog* m_pPD;     // pointer to print dialog
//  BOOL m_bPreview;         // TRUE if in preview mode
//  BOOL m_bDirect;          // TRUE if bypassing Print Dialog
//  BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
//  UINT m_nCurPage;         // Current page
//  UINT m_nNumPreviewPages; // Desired number of preview pages
//  CString m_strPageDesc;   // Format string for page number display
//  void SetMinPage(UINT nMinPage);
//  void SetMaxPage(UINT nMaxPage);
//  UINT GetMinPage() const;
//  UINT GetMaxPage() const;
//  UINT GetFromPage() const;
//  UINT GetToPage() const;
void CXTPGridView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_aPageStart.RemoveAll();
	m_aPageStart.Add(0);

	int nVirtualPages = SetupStartCol(pDC, pInfo);
	if (pInfo->m_bPreview)
	{
		pInfo->m_nNumPreviewPages		= 1;
		AfxGetApp()->m_nNumPreviewPages = pInfo->m_nNumPreviewPages;
	}

	CString str1, str2;
	if (m_pPrintOptions && m_pPrintOptions->GetPageHeader())
		str1 = m_pPrintOptions->GetPageHeader()->m_strFormatString;
	if (m_pPrintOptions && m_pPrintOptions->GetPageFooter())
		str2 = m_pPrintOptions->GetPageFooter()->m_strFormatString;

	if (nVirtualPages > 1 && m_pPrintOptions->GetPageHeader()->m_strFormatString.IsEmpty())
		m_pPrintOptions->GetPageHeader()->m_strFormatString = _T(" &p/&P ");

	UINT pFr = pInfo->GetFromPage();
	UINT pTo = pInfo->GetToPage();
	if (pFr > 0)
	{
		pInfo->SetMinPage(pFr);
		pInfo->m_nCurPage = pFr;
	}

	if (pTo < 65535)
	{
		pInfo->SetMaxPage(pTo);
	}

	if (GetGridCtrl().m_bForcePagination)
	{
		m_bPaginated = FALSE;
	}

	if ((!pInfo->m_bPreview || GetGridCtrl().m_bForcePagination)
		&& (str1.Find(_T("&P")) >= 0 || nVirtualPages > 1 || pFr > 1 || pTo < 65535
			|| str2.Find(_T("&P")) >= 0))
	{
		m_nStartIndex  = 0;
		m_nColumnStart = XTPToIntChecked(GetPaintManager()->m_arStartCol.GetAt(0));
		m_nColumnEnd   = XTPToIntChecked(GetPaintManager()->m_arStartCol.GetAt(1));

		int nCurPage	  = XTPToIntChecked(pInfo->m_nCurPage);
		pInfo->m_nCurPage = 65535;

		if (PaginateTo(pDC, pInfo))
		{
			UINT nMaxPage = static_cast<UINT>(m_aPageStart.GetSize()) - 1 - m_nStartIndex;
			nMaxPage	  = max(pInfo->GetMinPage(), nMaxPage);
			pInfo->SetMaxPage(nMaxPage);
		}

		pInfo->m_nCurPage = XTPToUIntChecked(nCurPage);
		if (GetGridCtrl().m_bForcePagination)
		{
			m_bPaginated = TRUE;
		}
	}
	m_nStartIndex  = 0;
	m_nColumnStart = XTPToIntChecked(GetPaintManager()->m_arStartCol.GetAt(0));
	m_nColumnEnd   = XTPToIntChecked(GetPaintManager()->m_arStartCol.GetAt(1));
}

void CXTPGridView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if (!m_pPrintOptions || !pDC || !pInfo)
	{
		ASSERT(FALSE);
		return;
	}
	CRect rcMargins = m_pPrintOptions->GetMarginsLP(pDC);
	CRect rc		= pInfo->m_rectDraw;
	rc.DeflateRect(rcMargins);

	UINT nVirtualPages = (UINT)GetPaintManager()->m_arStartCol.GetSize() - 1;
	UINT nPage		   = pInfo->m_nCurPage;
	int nSize		   = (int)m_aPageStart.GetSize();

	ASSERT(nPage <= (UINT)nSize);

	UINT nIndex = m_aPageStart[XTPToIntPtrChecked(nPage - 1)];
	UINT mIndex(0);

	// if (m_bPaginated)
	{
		m_nStartIndex  = (pInfo->m_nCurPage - 1) % nVirtualPages;
		m_nColumnStart = XTPToIntChecked(
			GetPaintManager()->m_arStartCol.GetAt(XTPToIntPtrChecked(m_nStartIndex)));
		m_nColumnEnd = XTPToIntChecked(
			GetPaintManager()->m_arStartCol.GetAt(XTPToIntPtrChecked(m_nStartIndex + 1)));
	}

	// TRACE(_T("CXTPGridView::OnPrint %d %d %d %d %d\n"), pInfo->m_nCurPage, nVirtualPages,
	// m_nStartIndex, m_nColumnStart, m_nColumnEnd);

	if (!m_pPrintOptions->m_bBlackWhitePrinting)
	{
		mIndex = XTPToUIntChecked(
			PrintPage(pDC, pInfo, rc, XTPToIntChecked(nIndex), m_nColumnStart, m_nColumnEnd));
	}
	else
	{
		CRect rc00(0, 0, rc.Width(), rc.Height());

		CDC memDC;
		VERIFY(memDC.CreateCompatibleDC(pDC));
		memDC.m_bPrinting = TRUE;

		if (!m_bmpGrayDC.m_hObject || m_bmpGrayDC.GetBitmapDimension() != rc00.Size())
		{
			m_bmpGrayDC.DeleteObject();
			m_bmpGrayDC.CreateCompatibleBitmap(pDC, rc00.Width(), rc00.Height());
		}

		CXTPBitmapDC autoBitmap(&memDC, &m_bmpGrayDC);

		memDC.FillSolidRect(rc00, RGB(255, 255, 255));

		mIndex = XTPToUIntChecked(
			PrintPage(&memDC, pInfo, rc00, XTPToIntChecked(nIndex), m_nColumnStart, m_nColumnEnd));

		int nCC = max(0, min(m_pPrintOptions->m_nBlackWhiteContrast, 255));
		XTPImageManager()->BlackWhiteBitmap(memDC, rc00, nCC);

		pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	}

	if (!m_bPaginated)
	{
		m_nStartIndex++;
		if (m_nStartIndex >= nVirtualPages) // done with the page!
			m_nStartIndex = 0;

		m_nColumnStart = XTPToIntChecked(
			GetPaintManager()->m_arStartCol.GetAt(XTPToIntPtrChecked(m_nStartIndex)));
		m_nColumnEnd = XTPToIntChecked(
			GetPaintManager()->m_arStartCol.GetAt(XTPToIntPtrChecked(m_nStartIndex + 1)));

		if (m_nStartIndex > 0)
		{
			if (nPage == (UINT)nSize)
				m_aPageStart.Add(nIndex);
			else if (nPage < (UINT)nSize)
				m_aPageStart[XTPToIntPtrChecked(nPage)] = nIndex;
		}
		else
		{
			if (nPage == (UINT)nSize)
				m_aPageStart.Add(mIndex);
			else if (nPage < (UINT)nSize)
				m_aPageStart[XTPToIntPtrChecked(nPage)] = mIndex;

			if ((int)mIndex == GetGridCtrl().GetRows()->GetCount())
				pInfo->SetMaxPage(pInfo->m_nCurPage);
		}
	}

	if (pInfo->m_bPreview)
	{
		pInfo->m_nCurPage++;
	}
}

extern BOOL CALLBACK _XTPAbortProc(HDC, int);

BOOL CXTPGridView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(this);
	ASSERT(pDC);

	UINT nVirtualPages = (UINT)GetPaintManager()->m_arStartCol.GetSize() - 1;
	BOOL bAborted	  = FALSE;
	CXTPPrintingDialog PrintStatus(this);
	CString strTemp;
	if (GetParentFrame())
		GetParentFrame()->GetWindowText(strTemp);

	PrintStatus.SetWindowText(_T("Calculating pages..."));
	PrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTemp);
	PrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME, pInfo->m_pPD->GetDeviceName());
	PrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, pInfo->m_pPD->GetPortName());
	PrintStatus.ShowWindow(SW_SHOW);
	PrintStatus.UpdateWindow();

	CRect rectSave						   = pInfo->m_rectDraw;
	UINT nPageSave						   = pInfo->m_nCurPage;
	BOOL bBlackWhiteSaved				   = m_pPrintOptions->m_bBlackWhitePrinting;
	m_pPrintOptions->m_bBlackWhitePrinting = FALSE;

	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT)m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);

	pDC->IntersectClipRect(0, 0, 0, 0);
	UINT nCurPage	 = (UINT)m_aPageStart.GetSize();
	pInfo->m_nCurPage = nCurPage;

	while (pInfo->m_nCurPage < nPageSave && pInfo->m_nCurPage <= pInfo->GetMaxPage())
	{
		if (pInfo->m_bPreview)
			ASSERT(pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize());

		OnPrepareDC(pDC, pInfo);

		if (!pInfo->m_bContinuePrinting)
			break;
		if (nVirtualPages > 1)
			strTemp.Format(_T("%d [%d - %d]"), pInfo->m_nCurPage,
						   1 + (pInfo->m_nCurPage / nVirtualPages),
						   1 + (pInfo->m_nCurPage % nVirtualPages));
		else
			strTemp.Format(_T("%d"), pInfo->m_nCurPage);

		PrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, strTemp);

		pInfo->m_rectDraw.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);

		OnPrint(pDC, pInfo);

		if (!pInfo->m_bPreview)
			pInfo->m_nCurPage++;

		if (pInfo->GetMaxPage() == 65535)
			pInfo->SetMaxPage(max(pInfo->GetMaxPage(), pInfo->m_nCurPage));

		if (!_XTPAbortProc(0, 0))
		{
			bAborted = TRUE;
			break;
		}
	}
	PrintStatus.DestroyWindow();

	BOOL bResult = !bAborted && (pInfo->m_nCurPage == nPageSave || nPageSave == 65535);

	pInfo->m_bContinuePrinting = bResult;
	pDC->RestoreDC(-1);
	m_pPrintOptions->m_bBlackWhitePrinting = bBlackWhiteSaved;
	pInfo->m_nCurPage					   = nPageSave;

	pInfo->m_rectDraw = rectSave;
	ASSERT(this);

	return bResult;
}

void CXTPGridView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(this);
	ASSERT(pDC);
	if (!pInfo)
		return;

	if (!m_bPaginated)
	{
		int nRowCount	  = GetGridCtrl().GetRows()->GetCount();
		int nSize		   = (int)m_aPageStart.GetSize();
		int nPage		   = XTPToIntChecked(pInfo->m_nCurPage);
		UINT nVirtualPages = (UINT)GetPaintManager()->m_arStartCol.GetSize() - 1;
		UINT nVirPage(0);
		if (nVirtualPages > 0)
			nVirPage = 1 + m_nStartIndex;

		if (nPage == 1 && nRowCount == 0) // First page?
			pInfo->m_bContinuePrinting = TRUE;
		else if (nVirPage > 0 && nVirPage < nVirtualPages) // not finished page
			pInfo->m_bContinuePrinting = TRUE;
		else if (nPage == nSize && m_aPageStart[nPage - 1] >= (UINT)nRowCount
				 && m_nStartIndex >= nVirtualPages - 1) // Last page?
			pInfo->m_bContinuePrinting = FALSE;			// can't paginate to that page
		else if (nPage > nSize && m_nStartIndex > nVirtualPages - 1
				 && !PaginateTo(pDC, pInfo))	// Can be last page?
			pInfo->m_bContinuePrinting = FALSE; // can't paginate to that page

		if (pInfo->m_nCurPage > pInfo->GetMaxPage())
			pInfo->m_bContinuePrinting = FALSE;
	}

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(96, 96);
	pDC->OffsetWindowOrg(0, 0);

	if (pInfo->m_bPreview) // PRINT MODE in RTL does not work!
	{
		if (GetGridCtrl().GetExStyle() & WS_EX_RTLREADING)
		{
			pDC->SetTextAlign(TA_RTLREADING);
		}

		if (GetGridCtrl().GetExStyle() & WS_EX_LAYOUTRTL)
		{
			XTPDrawHelpers()->SetContextRTL(pDC, LAYOUT_RTL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Clipboard operations

void CXTPGridView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetGridCtrl().CanCopy());
}

void CXTPGridView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bAllowCut && GetGridCtrl().CanCut());
}

void CXTPGridView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bAllowPaste && GetGridCtrl().CanPaste());
}

void CXTPGridView::OnEditCut()
{
	if (m_bAllowCut)
		GetGridCtrl().Cut();
}

void CXTPGridView::OnEditCopy()
{
	GetGridCtrl().Copy();
}

void CXTPGridView::OnEditPaste()
{
	if (m_bAllowPaste)
		GetGridCtrl().Paste();
}

void CXTPGridView::OnFilePageSetup()
{
	DWORD dwFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE;
	CXTPGridPageSetupDialog dlgPageSetup(GetPrintOptions(), dwFlags, this);

	XTPGetPrinterDeviceDefaults(dlgPageSetup.m_psd.hDevMode, dlgPageSetup.m_psd.hDevNames);

	int nDlgRes = (int)dlgPageSetup.DoModal();

	if (nDlgRes == IDOK)
		AfxGetApp()->SelectPrinter(dlgPageSetup.m_psd.hDevNames, dlgPageSetup.m_psd.hDevMode,
								   FALSE);
}
