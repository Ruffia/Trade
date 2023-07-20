// XTPGridPaintManager.cpp : implementation of the CXTPGridPaintManager class.
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

#include "GridControl/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPSystemMetrics.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridRecordItemRange.h"
#include "GridControl/XTPGridInplaceControls.h"
#include "GridControl/XTPGridRecordItemControls.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridHyperlink.h"
#include "GridControl/XTPGridPageSetupDialog.h"
#include "GridControl/XTPGridBorder.h"
#include "GridControl/XTPGridThickness.h"
#include "GridControl/XTPGridSection.h"
#include "GridControl/XTPGridControlIIDs.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"

#	include "Controls/Combo/XTPComboBox.h"
#	include "Controls/Combo/XTPComboBoxTheme.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

#define XTP_HLINK_SPACE_X ((TCHAR)1)

AFX_INLINE int GetTextExtentX(CDC* pDC, const CString& strString)
{
	CString str(strString);
	str.Replace(XTP_HLINK_SPACE_X, _T(' '));

	int nExt1 = pDC->GetTextExtent(str).cx;
	if (pDC->IsPrinting())
	{
		int nExt2 = pDC->GetOutputTextExtent(str).cx;
		return max(nExt1, nExt2);
	}
	return nExt1;
}

AFX_INLINE int GetTextExtentX(CDC* pDC, LPCTSTR pszString, int nCount)
{
	CString str(pszString, nCount);
	str.Replace(XTP_HLINK_SPACE_X, _T(' '));

	int nExt1 = pDC->GetTextExtent(str).cx;
	if (pDC->IsPrinting())
	{
		int nExt2 = pDC->GetOutputTextExtent(str).cx;
		return max(nExt1, nExt2);
	}
	return nExt1;
}

//////////////////////////////////////////////////////////////////////
// CXTPGridPaintManager

IMPLEMENT_DYNAMIC(CXTPGridPaintManager, CXTPCmdTarget);

CXTPGridPaintManager::CXTPGridPaintManager()
	: m_pHyperlinkStyle(NULL)
	, m_pHyperlinkHoverStyle(NULL)
	, m_nTreeTextIndentRowParent(XTP_DPI_X(3))
	, m_nTreeTextIndentRowChildren(XTP_DPI_X(5))
	, m_nTreeTextIndentGroupRow(XTP_DPI_X(5))
	, m_pGlyphs(NULL)
	, m_pBorder(NULL)
	, m_pControl(NULL)
{
	m_themeHeader = new CXTPWinThemeWrapper();
	m_themeButton = new CXTPWinThemeWrapper();
	m_themeCombo  = new CXTPWinThemeWrapper();
	m_themeSpin   = new CXTPWinThemeWrapper();

#ifdef _XTP_INCLUDE_CONTROLS
	m_bUseThemedControls = FALSE;
	m_pCombo			 = NULL;
#endif

	// Set up default border
	m_pBorder = new CXTPGridBorder();
	m_pBorder->GetBorderThickness()->Set(0, 0, 0, XTP_DPI_Y(1));
	m_pBorder->GetEdge(xtpGridBorderEdgeBottom)->SetLineStyle(xtpGridLineStyleSolid);

	m_pMarkupUIElement_NoGroupBy = NULL;

	m_clrPreviewText.SetStandardValue(RGB(0, 0, 255));
	// m_clrColumnOffice2007CustomTheme.SetStandardValue(RGB(196, 221, 255));
	m_clrColumnOffice2007CustomTheme = COLORREF_NULL;
	m_clrFormulaSum					 = RGB(255, 0, 0);	 // Color for Formula Sum fields
	m_clrFormulaSubSum				 = RGB(0, 0, 255);	 // Color for Formula SubSum fields
	m_clrAlternativeBackground		 = RGB(224, 224, 224); // Color for Alternative Row Background
	m_bUseAlternativeBackground		 = FALSE;

	m_nMaxPreviewLines = 3;
	m_szGlyph		   = CSize(XTP_DPI_X(12), XTP_DPI_Y(12));

	m_bThemedInplaceButtons = TRUE;

	// defaults
	m_nGroupGridLineHeight		 = XTP_DPI_X(2);
	m_nResizingWidth			 = 1;
	m_bIsPreviewMode			 = FALSE;
	m_bShadeGroupHeadings		 = FALSE;
	m_bGroupRowTextBold			 = FALSE;
	m_bUseGroupRowSelectionState = FALSE;
	m_bPrintGroupRowIcon		 = TRUE;
	m_bShadeSortColumn			 = TRUE;
	m_columnStyle				 = xtpGridColumnShaded;

	m_treeStructureStyle = xtpGridTreeStructureSolid;

	m_nGroupGridLineStyle	 = xtpGridGridSolid;
	m_nGridLineStylePreviousV = xtpGridLineStyleNone;
	m_nGridLineStylePreviousH = xtpGridLineStyleNone;

	m_nRowHeightDefault					 = XTP_DPI_Y(18);
	m_nFixedColumnsIndent				 = 0;
	m_bMoveScrollbarOnFixedColumnsIndent = FALSE;
	m_bBaseTreeIndent					 = FALSE;

	m_nTextIndentForGroupRowsWithNoIcon = -1;

	m_bRevertAlignment			  = FALSE;
	m_bUseEditTextAlignment		  = TRUE;
	m_bUseColumnTextAlignment	 = FALSE;
	m_bHotTracking				  = TRUE;
	m_bInvertColumnOnClick		  = TRUE;
	m_bColumnPlusMinusSign		  = FALSE;
	m_RecordNumberWidth			  = XTP_DPI_Y(20);
	m_bShowLockIcon				  = FALSE;
	m_bUseShellIcon				  = FALSE;
	m_bShowNonActiveInPlaceButton = FALSE;
	m_bForceShowTooltip			  = FALSE;
	m_bCleanTooltip				  = FALSE;
	m_bGrayIfDisable			  = TRUE;
	m_bRecOrRowNum				  = TRUE;
	m_iRecOrRowStart			  = 0;

	m_bSortTriangleOffsetFromRight = FALSE;

	m_bUseDropMarker = TRUE;

	m_nTreeColumnLineLength   = 0;
	m_nBaseParentRecordIndent = 0;

	m_bMakeUpper = FALSE;

	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strNoItems, XTP_IDS_GRID_NOITEMS));
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strNoGroupBy, XTP_IDS_GRID_NOGROUPBY));
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strNoFieldsAvailable, XTP_IDS_GRID_NOFIELDSAVAILABLE));

	if (XTPResourceManager()->LoadString(&m_strSortBy, XTP_IDS_GRID_SORTBY))
		m_strSortBy += _T(" ");

	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strIncreasing, XTP_IDS_GRID_INCREASING));
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strDecreasing, XTP_IDS_GRID_DECREASING));
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strExpand, XTP_IDS_GRID_EXPAND));
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&m_strCollapse, XTP_IDS_GRID_COLLAPSE));

	m_bHideSelection = FALSE;
	m_rcPreviewIndent.SetRect(XTP_DPI_X(10), 0, XTP_DPI_X(10), XTP_DPI_Y(2));

	m_nTreeIndent		 = XTP_DPI_X(20);
	m_bPrintSortTriangle = TRUE;

	m_DrawSortTriangleStyle = xtpGridDrawSortTriangleDefault;

	// get system parameters
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	VERIFY(m_xtpFontPreview.CreateFontIndirect(&lfIcon));

	SetCaptionFont(lfIcon);
	SetTextFont(lfIcon);
	LoadGlyphs();

	m_bFixedRowHeight		  = TRUE;
	m_bColumnWidthWYSIWYG	 = FALSE;
	m_bLastColumnWidthWYSIWYG = FALSE;
	m_bShowWYSIWYGMarkers	 = TRUE;
	m_bPrintVirtualPageNumber = FALSE;
	m_bPrintPageRectangle	 = TRUE;
	m_bPrintWatermark		  = FALSE;
	m_PrintPageWidth		  = 0;
	m_bDrawGridForEmptySpace  = FALSE;

	m_bFixedInplaceButtonHeight = FALSE;
	m_nFreezeColsDividerStyle   = xtpGridFreezeColsDividerThin | xtpGridFreezeColsDividerShade
								| xtpGridFreezeColsDividerHeader;

	m_nTreeStructureStyle		 = 0;
	m_bForceDynamicMarkupForCell = FALSE;
	m_bForceShowDropMarker		 = FALSE;
	m_bCompactText				 = FALSE;
	m_bAllowMergeCells			 = FALSE;

	RefreshMetrics();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridPaintManager::~CXTPGridPaintManager()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);

	XTPMarkupReleaseElement(m_pMarkupUIElement_NoGroupBy);

	SAFE_DELETE(m_themeHeader);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeCombo);
	SAFE_DELETE(m_themeSpin);

#ifdef _XTP_INCLUDE_CONTROLS
	SAFE_DELETE(m_pCombo);
#endif

	SAFE_DELETE(m_pBorder);
}

void CXTPGridPaintManager::SetFixedRowHeight(BOOL bFixedRowHeight)
{
	BOOL bRedraw	  = m_bFixedRowHeight != bFixedRowHeight;
	m_bFixedRowHeight = bFixedRowHeight;

	if (m_pControl && bRedraw)
	{
		m_pControl->AdjustLayout();
		m_pControl->RedrawControl();
	}
}

COLORREF CXTPGridPaintManager::GetControlBackColor(CXTPGridControl* /*pControl*/)
{
	return m_clrControlBack;
}

void CXTPGridPaintManager::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	int nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_GRID_GLYPHS_12, XTP_IDB_GRID_GLYPHS_15,
												   XTP_IDB_GRID_GLYPHS_18, XTP_IDB_GRID_GLYPHS_24);
	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTPToUInt(nBmpID)));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpGridGlyphCount, CSize(0, 0)); // 11
}

void CXTPGridPaintManager::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPGridHyperlinkStyle(RGB(0, 0, 255), xtpGridTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPGridHyperlinkStyle(RGB(0, 0, 255),
														xtpGridTextDecorationUnderline);

	RefreshXtremeColors(FALSE);

	m_clrBtnFace.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrBtnText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrControlDark.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrHighlight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrWindowText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_clrControlLightLight.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT));
	m_clrControlBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrConnector.SetStandardValue(m_clrWindowText);

	m_clrBtnFacePressed.SetStandardValue(m_clrBtnFace);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnText);
	m_clrCaptionTextPressed.SetStandardValue(m_clrBtnText);
	SetGridColor(m_clrControlDark);
	m_clrHeaderControl.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);

	m_clrSelectedRow.SetStandardValue(m_clrBtnFace);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);

	m_clrItemShade.SetStandardValue(RGB(245, 245, 245));
	m_clrHotDivider.SetStandardValue(RGB(0xFF, 0, 0));

	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);

	m_clrGradientColumnSeparator.SetStandardValue(m_clrControlDark);

	m_themeHeader->OpenThemeData(0, L"HEADER");
	m_themeButton->OpenThemeData(0, L"BUTTON");
	m_themeCombo->OpenThemeData(0, L"COMBOBOX");
	m_themeSpin->OpenThemeData(0, L"SPIN");

	m_nGroupGridLineStyle	 = xtpGridGridSolid;
	m_nGridLineStylePreviousV = xtpGridLineStyleNone;
	m_nGridLineStylePreviousH = xtpGridLineStyleNone;

	m_xtpBrushVeriticalGrid.DeleteObject();
	m_xtpBrushHorizontalGrid.DeleteObject();
	m_xtpBrushTreeStructure.DeleteObject();
	m_xtpBrushGroupGridLineStyle.DeleteObject();

	m_clrFreezeColsDivider.SetStandardValue(RGB(0, 0, 222));

	m_clrDisableBitmapLightest.SetStandardValue(RGB(250, 250, 250));
	m_clrDisableBitmapDarkest.SetStandardValue(RGB(128, 128, 128));

	LOGFONT lf;
	m_xtpFontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);

#ifdef _XTP_INCLUDE_CONTROLS
	// themed controls

	if (m_bUseThemedControls && m_pControl)
	{
		m_pCombo = new CXTPComboBox();
		ASSERT(m_pCombo);
		m_pCombo->Create(WS_CHILD, CRect(0, 0, 0, 0), m_pControl, 1);
	}
#endif
}

#ifdef _XTP_INCLUDE_CONTROLS
void CXTPGridPaintManager::SetControlsTheme(int eTheme)
{
	ASSERT(m_bUseThemedControls);

	if (m_bUseThemedControls)
	{
		ASSERT(m_pCombo);
		m_pCombo->SetTheme((XTPControlTheme)eTheme);
	}
}
#endif

void CXTPGridPaintManager::FillRow(CDC* pDC, CXTPGridRow* pRow, CRect rcRow)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pRow);

	if (m_bUseAlternativeBackground && pRow->GetIndex() % 2 == 1)
		pDC->FillSolidRect(rcRow, m_clrAlternativeBackground);
}

void CXTPGridPaintManager::DrawGridPattern(CDC* pDC, CBrush* pBrush, CRect rc, const void* pPattern,
										   COLORREF clr)
{
	DrawGridPattern(pDC, pBrush, rc, pPattern, clr, FALSE);
}

template<class TBrush>
static void DrawGridPatternImpl(CDC* pDC, TBrush* pBrush, CRect rc, const void* pPattern,
								COLORREF clr, COLORREF clrControlBack, BOOL bUpdateBrush)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBrush);
	ASSERT(NULL != pPattern);

	if (bUpdateBrush)
	{
		pBrush->DeleteObject();
	}

	if (pBrush->GetSafeHandle() == 0)
	{
		CBitmap bmp;
		bmp.CreateBitmap(8, 8, 1, 1, pPattern);
		pBrush->CreatePatternBrush(&bmp);
	}

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&*pBrush);

	COLORREF clrOldText = pDC->SetTextColor(clrControlBack);
	COLORREF clrOldBk   = pDC->SetBkColor(clr);

	PatBlt(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);

	pDC->SetTextColor(clrOldText);
	pDC->SetBkColor(clrOldBk);

	pDC->SelectObject(pOldBrush);
}

void CXTPGridPaintManager::DrawGridPattern(CDC* pDC, CBrush* pBrush, CRect rc, const void* pPattern,
										   COLORREF clr, BOOL bUpdateBrush)
{
	DrawGridPatternImpl(pDC, pBrush, rc, pPattern, clr, m_clrControlBack, bUpdateBrush);
}

void CXTPGridPaintManager::DrawGridPattern(CDC* pDC, CXTPBrush* pBrush, CRect rc,
										   const void* pPattern, COLORREF clr, BOOL bUpdateBrush)
{
	DrawGridPatternImpl(pDC, pBrush, rc, pPattern, clr, m_clrControlBack, bUpdateBrush);
}

static void CreateLineStyleBrush(CXTPBrush* pBrush, const void* pPattern)
{
	ASSERT_VALID(pBrush);
	ASSERT(NULL != pPattern);

	if (NULL == pBrush->GetSafeHandle())
	{
		CBitmap bmp;
		bmp.CreateBitmap(8, 8, 1, 1, pPattern);
		pBrush->CreatePatternBrush(&bmp);
	}
}

void CXTPGridPaintManager::CreateLineStyleBrushes()
{
	// Horizontal
	static const unsigned short _DotsSmallH[] = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };
	static const unsigned short _DotsLargeH[] = { 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33 };
	static const unsigned short _DashH[]	  = { 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0 };

	CreateLineStyleBrush(&m_xtpBrushDotsSmallH, _DotsSmallH);
	CreateLineStyleBrush(&m_xtpBrushDotsLargeH, _DotsLargeH);
	CreateLineStyleBrush(&m_xtpBrushDashH, _DashH);

	// Vertical
}

XTPGridLineStyle CXTPGridPaintManager::GetGridStyle(BOOL bVertical) const
{
	CXTPGridBorder* pBorder = GetDefaultItemBorder();

	if (bVertical)
	{
		return pBorder->GetEdge(xtpGridBorderEdgeRight)->GetLineStyle();
	}
	else
	{
		return pBorder->GetEdge(xtpGridBorderEdgeBottom)->GetLineStyle();
	}
}

void CXTPGridPaintManager::SetGridStyle(BOOL bVertical, XTPGridLineStyle lineStyle)
{
	CXTPGridBorder* pBorder = GetDefaultItemBorder();

	if (bVertical)
	{
		switch (lineStyle)
		{
			case xtpGridLineStyleNone: pBorder->GetBorderThickness()->SetRight(0); break;
			default: pBorder->GetBorderThickness()->SetRight(XTP_DPI_X(1)); break;
		}

		pBorder->GetEdge(xtpGridBorderEdgeRight)->SetLineStyle(lineStyle);
	}
	else
	{
		switch (lineStyle)
		{
			case xtpGridLineStyleNone: pBorder->GetBorderThickness()->SetBottom(0); break;
			default: pBorder->GetBorderThickness()->SetBottom(XTP_DPI_Y(1)); break;
		}

		pBorder->GetEdge(xtpGridBorderEdgeBottom)->SetLineStyle(lineStyle);
	}
}

void CXTPGridPaintManager::SetColumnStyle(XTPGridColumnStyle columnStyle)
{
	if (::IsWindow(m_pControl->GetSafeHwnd()))
	{
		switch (columnStyle)
		{
			case xtpGridColumnShaded: m_pControl->SetTheme(xtpGridThemeOffice2003); break;
			case xtpGridColumnFlat: m_pControl->SetTheme(xtpGridThemeOfficeXP); break;
			case xtpGridColumnExplorer: m_pControl->SetTheme(xtpGridThemeExplorer); break;
			case xtpGridColumnOffice2003: m_pControl->SetTheme(xtpGridThemeOffice2003Luna); break;
			case xtpGridColumnResource: m_pControl->SetTheme(xtpGridThemeResource); break;
		}
	}
}

void CXTPGridPaintManager::DrawGrid(CDC* pDC, XTPGridOrientation orientation, CRect rcGrid)
{
	if (xtpGridOrientationHorizontal == orientation)
	{
		XTPGridLineStyle nCurrentGridStyleH = GetGridStyle(FALSE);
		BOOL bUpdateBrush					= (m_nGridLineStylePreviousH != nCurrentGridStyleH);
		if (bUpdateBrush)
			m_nGridLineStylePreviousH = nCurrentGridStyleH;

		switch (nCurrentGridStyleH)
		{
			case xtpGridGridSolid:
			{
				DrawHorizontalLine(pDC, rcGrid.left, rcGrid.bottom, rcGrid.Width(), GetGridColor());
				break;
			}
			case xtpGridGridSmallDots:
			{
				static const unsigned short cb[] =
					{ 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };
				DrawGridPattern(pDC, (CXTPBrush*)&m_xtpBrushHorizontalGrid,
								CRect(rcGrid.left, rcGrid.bottom, rcGrid.right,
									  rcGrid.bottom + XTP_DPI_Y(1)),
								cb, GetGridColor(), bUpdateBrush);
				break;
			}
			case xtpGridGridLargeDots:
			{
				static const unsigned short cb[] =
					{ 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33 };
				DrawGridPattern(pDC, (CXTPBrush*)&m_xtpBrushHorizontalGrid,
								CRect(rcGrid.left, rcGrid.bottom, rcGrid.right,
									  rcGrid.bottom + XTP_DPI_Y(1)),
								cb, GetGridColor(), bUpdateBrush);
				break;
			}
			case xtpGridGridDashes:
			{
				static const unsigned short cb[] =
					{ 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0 };
				DrawGridPattern(pDC, (CXTPBrush*)&m_xtpBrushHorizontalGrid,
								CRect(rcGrid.left, rcGrid.bottom, rcGrid.right,
									  rcGrid.bottom + XTP_DPI_Y(1)),
								cb, GetGridColor(), bUpdateBrush);
				break;
			}
		}
	}
	else
	{
		XTPGridLineStyle nCurrentGridStyleV = GetGridStyle(TRUE);
		BOOL bUpdateBrush					= (m_nGridLineStylePreviousV != nCurrentGridStyleV);
		if (bUpdateBrush)
			m_nGridLineStylePreviousV = nCurrentGridStyleV;

		int nPos = rcGrid.right - XTP_DPI_X(1);
		switch (nCurrentGridStyleV)
		{
			case xtpGridGridSolid:
			{
				DrawVerticalLine(pDC, nPos, rcGrid.top, rcGrid.Height(), GetGridColor());
				break;
			}
			case xtpGridGridSmallDots:
			{
				static const unsigned short cb[] =
					{ 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };
				DrawGridPattern(pDC, (CXTPBrush*)&m_xtpBrushVeriticalGrid,
								CRect(nPos, rcGrid.top, nPos + XTP_DPI_X(1), rcGrid.bottom), cb,
								GetGridColor(), bUpdateBrush);
				break;
			}
			case xtpGridGridLargeDots:
			{
				static const unsigned short cb[] =
					{ 0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33 };
				DrawGridPattern(pDC, (CXTPBrush*)&m_xtpBrushVeriticalGrid,
								CRect(nPos, rcGrid.top, nPos + XTP_DPI_X(1), rcGrid.bottom), cb,
								GetGridColor(), bUpdateBrush);
				break;
			}
			case xtpGridGridDashes:
			{
				static const unsigned short cb[] = { 0xf0, 0xf0, 0xf0, 0xf0, 0xf, 0xf, 0xf, 0xf };
				DrawGridPattern(pDC, (CXTPBrush*)&m_xtpBrushVeriticalGrid,
								CRect(nPos, rcGrid.top, nPos + XTP_DPI_X(1), rcGrid.bottom), cb,
								GetGridColor(), bUpdateBrush);
				break;
			}
		}
	}
}

void CXTPGridPaintManager::DrawFreezeColsDivider(CDC* pDC, const CRect& rc,
												 CXTPGridControl* pControl, CXTPGridRow* pRow)
{
	BOOL bShowAlways = !!(m_nFreezeColsDividerStyle & xtpGridFreezeColsDividerShowAlways);

	if ((!bShowAlways && pControl->GetGridHeader()->IsAutoColumnSizing())
		|| (m_nFreezeColsDividerStyle == 0))
	{
		m_nFixedColumnsIndent = 0;
		return;
	}

	CRect rcDvX = rc;

	if ((m_nFreezeColsDividerStyle & xtpGridFreezeColsDividerShade)
		&& pControl->GetScrollPos(SB_HORZ) && !pControl->IsFullColumnScrolling())
	{
		rcDvX.left  = rcDvX.right - XTP_DPI_X(1);
		rcDvX.right = rcDvX.left + XTP_DPI_X(4);
		XTPDrawHelpers()->GradientFill(pDC, rcDvX, m_clrFreezeColsDivider, RGB(255, 255, 255),
									   TRUE);
	}
	else if (m_nFreezeColsDividerStyle
			 & (xtpGridFreezeColsDividerThin | xtpGridFreezeColsDividerBold))
	{
		int nWidth = (m_nFreezeColsDividerStyle & xtpGridFreezeColsDividerBold) ? XTP_DPI_X(2)
																				: XTP_DPI_X(1);
		rcDvX.left = rcDvX.right - nWidth;
		pDC->FillSolidRect(&rcDvX, m_clrFreezeColsDivider);
	}

	int nMask = (xtpGridFreezeColsDividerThin | xtpGridFreezeColsDividerBold
				 | xtpGridFreezeColsDividerShade);
	if (!pDC->IsPrinting() && (m_nFreezeColsDividerStyle & nMask) && pRow && pRow->IsSelected())
	{
		pDC->InvertRect(&rcDvX);
	}

	m_nFixedColumnsIndent = rcDvX.left;
}

void CXTPGridPaintManager::DrawVerticalTimeLineMarker(CDC* pDC, int xPos, CXTPGridControl* pControl,
													  COLORREF color)
{
	CRect rcClipBox = pControl->GetGridRectangle();
	rcClipBox.left  = xPos;
	rcClipBox.right = xPos + XTP_DPI_X(1);
	pDC->FillSolidRect(&rcClipBox, color);
}

void CXTPGridPaintManager::FillGroupRowMetrics(CXTPGridGroupRow* pRow,
											   XTP_GRIDRECORDITEM_METRICS* pMetrics, BOOL bPrinting)
{
	ASSERT(pRow && pMetrics);
	if (!pRow || !pRow->GetControl() || !pMetrics)
		return;

	BOOL bControlFocused = pRow->GetControl()->HasFocus();

	pMetrics->clrForeground = m_clrGroupRowText;
	pMetrics->clrBackground = XTP_GRID_COLOR_DEFAULT;

	if (pRow->IsSelected() && bControlFocused && !bPrinting)
	{
		pMetrics->clrForeground = m_clrHighlightText;
		pMetrics->clrBackground = m_clrHighlight;
	}
	else if (m_bShadeGroupHeadings)
	{
		pMetrics->clrForeground = m_clrGroupShadeText;
		pMetrics->clrBackground = m_clrGroupShadeBack;
	}

	pMetrics->pFont = m_bGroupRowTextBold ? &m_xtpFontBoldText : &m_xtpFontText;
}

CRect CXTPGridPaintManager::GetGroupRowTextSize(CRect rcBitmap, CRect rcRow, int nBitmapWidth,
												int nNoIconWidth, int nTextOffset)
{
	CRect rcText(rcBitmap.left, rcRow.top, rcRow.right, rcRow.bottom);
	rcText.top = rcText.bottom - rcRow.Height() / 2 - XTP_DPI_Y(7);
	rcText.left += nBitmapWidth;
	rcText.left += nNoIconWidth;
	rcText.left += nTextOffset;
	rcText.left += m_nTreeTextIndentGroupRow;
	return rcText;
}

CRect CXTPGridPaintManager::GetGroupRowBmpSize(CRect rcRow, int nBitmapOffset)
{
	CRect rcBitmap(rcRow);
	rcBitmap.OffsetRect(nBitmapOffset, (m_szGlyph.cy / 2) - XTP_DPI_Y(1));
	return rcBitmap;
}

void CXTPGridPaintManager::DrawGroupRowBack(CDC* pDC, CRect rcRow,
											XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	if (pMetrics->clrBackground != XTP_GRID_COLOR_DEFAULT)
	{
		pDC->FillSolidRect(rcRow, pMetrics->clrBackground);
	}
}

void CXTPGridPaintManager::DrawGroupRow(CDC* pDC, CXTPGridGroupRow* pRow, CRect rcRow,
										XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	if (m_bMakeUpper)
	{
		pMetrics->strText.MakeUpper();
	}

	CXTPGridControl* pControl = pRow->GetControl();
	BOOL bControlFocused	  = pControl->HasFocus();

	DrawGroupRowBack(pDC, rcRow, pMetrics);
	pDC->SetTextColor(pMetrics->clrForeground);

	ASSERT(pMetrics->pFont);
	CXTPFontDC font(pDC, pMetrics->pFont);

	int nRowLevel = pRow->GetTreeDepth();
	int nIndent   = pControl->GetIndent(nRowLevel + 1);

	CRect rcFocus(rcRow.left + nIndent, rcRow.top, rcRow.right, rcRow.bottom);

	if (m_nGroupGridLineStyle != xtpGridGridSolid)
	{
		LOGBRUSH lbr = { 0 };
		CBrush brush;
		if (NULL != (HBRUSH)m_xtpBrushGroupGridLineStyle)
		{
			m_xtpBrushGroupGridLineStyle.GetLogBrush(&lbr);
			brush.CreateBrushIndirect(&lbr);
		}

		const unsigned short cb[] = { 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc };
		DrawGridPattern(pDC, &brush,
						CRect(rcRow.left, rcRow.bottom - XTP_DPI_Y(1), rcRow.right, rcRow.bottom),
						cb, m_clrGroupShadeBorder);

		if (NULL == (HBRUSH)m_xtpBrushGroupGridLineStyle && NULL != (HBRUSH)brush)
		{
			brush.GetLogBrush(&lbr);
			m_xtpBrushGroupGridLineStyle.CreateBrushIndirect(&lbr);
		}
	}
	else
	{
		pDC->FillSolidRect(rcRow.left, rcRow.bottom - m_nGroupGridLineHeight, rcRow.Width(),
						   m_nGroupGridLineHeight, m_clrGroupShadeBorder);
		rcFocus.bottom--;
	}

	if (pRow->IsFocused() && bControlFocused && !pDC->IsPrinting() && pControl->IsRowFocusVisible())
	{
		DrawFocusedRow(pDC, rcFocus);
	}

	if (nIndent > 0)
	{
		FillIndent(pDC, CRect(rcRow.left, rcRow.top, rcRow.left + nIndent, rcRow.bottom));
	}

	rcRow.left += nIndent;

	int nBitmapOffset = 0;
	int nTextOffset   = 0;
	CSize szIcon(0, 0);

	int nGroupRowIconAlignment = pMetrics->nGroupRowIconAlignment;

	// check and set defaults if need
	if ((nGroupRowIconAlignment & xtpGroupRowIconHmask) == 0)
	{
		nGroupRowIconAlignment |= xtpGroupRowIconLeft;
	}

	if ((nGroupRowIconAlignment & xtpGroupRowIconVmask) == 0)
	{
		nGroupRowIconAlignment |= xtpGroupRowIconVCenter;
	}

	if (pMetrics->nGroupRowIcon != XTP_GRID_NOICON)
	{
		CXTPImageManagerIcon* pIcon =
			pControl->GetImageManager()->GetImage(XTPToUInt(pMetrics->nGroupRowIcon), 0);

		if (pIcon)
		{
			szIcon = CSize(pIcon->GetWidth(), pIcon->GetHeight());
		}

		if (nGroupRowIconAlignment & xtpGroupRowIconLeft)
		{
			nBitmapOffset = szIcon.cx + XTP_DPI_X(5);
		}

		if (nGroupRowIconAlignment & xtpGroupRowIconBeforeText)
		{
			nTextOffset = szIcon.cx + XTP_DPI_X(10);
		}
	}

	CRect rcBitmap = GetGroupRowBmpSize(rcRow, nBitmapOffset);

	int nBitmapWidth(0);
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon) && !pRow->IsLockExpand())
	{
		nBitmapWidth = DrawCollapsedBitmap(pDC, pRow, rcBitmap).cx;
	}
	else
	{
		rcBitmap.right = rcBitmap.left;
	}

	if (!pDC->IsPrinting())
	{
		pRow->SetCollapseRect(rcBitmap);
	}

	int nNoIconWidth = 0;

	if (pRow->IsExpanded() && m_pGlyphs->GetImage(0) == 0
		&& m_nTextIndentForGroupRowsWithNoIcon != -1 && pMetrics->nGroupRowIcon == XTP_GRID_NOICON)
		nNoIconWidth = m_nTextIndentForGroupRowsWithNoIcon;

	else if (pRow->IsExpanded() == FALSE && m_pGlyphs->GetImage(1) == 0
			 && m_nTextIndentForGroupRowsWithNoIcon != -1
			 && pMetrics->nGroupRowIcon == XTP_GRID_NOICON)
		nNoIconWidth = m_nTextIndentForGroupRowsWithNoIcon;

	CRect rcText = GetGroupRowTextSize(rcBitmap, rcRow, nBitmapWidth, nNoIconWidth, nTextOffset);

	int nRightMask = (xtpGroupRowIconAfterText | xtpGroupRowIconRight);
	if (pMetrics->nGroupRowIcon != XTP_GRID_NOICON && (nGroupRowIconAlignment & nRightMask))
		rcText.right -= szIcon.cx;

	if (m_bUseGroupRowSelectionState && !pDC->IsPrinting())
		rcText.left += (szIcon.cx + XTP_DPI_X(10));

	BOOL bDrawn	= FALSE;
	int nTextWidth = 0;

	if (pMetrics->strText != pRow->GetCaption() && pControl->GetMarkupContext() && !m_bMakeUpper)
	{
		CXTPMarkupUIElement* pMarkupUIElement = XTPMarkupParseText(pControl->GetMarkupContext(),
																   pMetrics->strText);
		if (pMarkupUIElement)
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement),
									(HFONT)pMetrics->pFont->GetSafeHandle(),
									pMetrics->clrForeground);

			nTextWidth = XTPMarkupMeasureElement(pMarkupUIElement, rcText.Width(), INT_MAX).cx;
			XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), &rcText);

			XTPMarkupReleaseElement(pMarkupUIElement);
			bDrawn = TRUE;
		}
	}

	if (!bDrawn && pRow->m_pMarkupUIElement)
	{
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(pRow->m_pMarkupUIElement),
								(HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);

		nTextWidth = XTPMarkupMeasureElement(pRow->m_pMarkupUIElement, rcText.Width(), INT_MAX).cx;

		XTPMarkupRenderElement(pRow->m_pMarkupUIElement, pDC->GetSafeHdc(), &rcText);

		bDrawn = TRUE;
	}

	if (!bDrawn)
	{
		UINT nFlags		= DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX;
		CString sSource = pMetrics->strText;

		// If no group formula is present then do not process "["
		int pos = pRow->GetFormula().IsEmpty() ? -1 : sSource.Find(_T('['));

		int nGroupTextRight = rcText.left;

		if (pos == -1) // non formula case
		{
			pDC->DrawText(sSource, &rcText, nFlags);
		}
		BOOL bDrawnBefore = FALSE;
		while (pos > -1)
		{
			if (!bDrawnBefore)
			{
				CString sBefore = sSource.Left(pos);
				pDC->DrawText(sBefore, &rcText, nFlags);
				bDrawnBefore = TRUE;

				nGroupTextRight = rcText.left + pDC->GetTextExtent(sBefore).cx;
			}
			CString sAfter = sSource.Mid(pos + 1);
			pos			   = sAfter.Find(_T(']'));

			if (pos > -1)
			{
				CString sNum = sAfter.Left(pos);
				int iCol	 = _ttoi(sNum);
				sSource		 = sAfter.Mid(pos + 1);

				CXTPGridColumn* pColumn = pControl->GetColumns()->Find(iCol);

				pos = sSource.Find(_T('['));
				if (pos > -1)
					sAfter = sSource.Left(pos);
				else
					sAfter = sSource;

				if (pColumn && pColumn->IsVisible())
				{
					int nColCount	  = pControl->GetColumns()->GetCount();
					int nColPrintWidth = 0;
					for (int i = 0; i < nColCount; ++i)
					{
						CXTPGridColumn* pCurrentColumn = pControl->GetColumns()->GetAt(i);
						if (NULL != pCurrentColumn && pCurrentColumn->IsVisible())
						{
							if (pCurrentColumn->GetItemIndex() == iCol)
								break;

							if (IsColumnWidthWYSIWYG())
							{
								nColPrintWidth += pCurrentColumn->GetWidth();
							}
							else
							{
								nColPrintWidth += pCurrentColumn->GetPrintWidth(rcRow.Width());
							}
						}
					}

					CRect rcCol = pColumn->GetRect();

					/*
					if (pDC->IsPrinting())
					{
						if (!m_mapColumnPrintPosition.Lookup(pColumn, rcCol))
							rcCol.SetRectEmpty();
					}
					*/

					rcText.left = rcRow.left + nColPrintWidth;
					if (IsColumnWidthWYSIWYG())
					{
						rcText.right = rcText.left + pColumn->GetWidth();
					}
					else
					{
						rcText.right = rcText.left + pColumn->GetPrintWidth(rcRow.Width());
					}

					++rcText.left;
					--rcText.right;

					int nTextExtentX = pDC->GetTextExtent(sAfter).cx;

					switch (pColumn->GetAlignment())
					{
						case DT_LEFT: break;
						case DT_CENTER: break;
						case DT_RIGHT: rcText.left = rcText.right - nTextExtentX; break;
					}

					if (rcText.left < nGroupTextRight) // Intersect case, move to Right
					{
						rcText.left = nGroupTextRight + XTP_DPI_X(10);
						// rcText.right = rcText.left + nTextExtentX; ! clip text
					}

					if (rcCol.right > rcCol.left)
					{
						pDC->DrawText(sAfter, &rcText,
									  pColumn->GetAlignment() | nFlags | DT_NOPREFIX
										  | DT_SINGLELINE);
					}
				}
			}
		}
		//      pDC->DrawText(pMetrics->strText, &rcText, nFlags);
		// For multiple formulas case need to decode given strText with tags as [C#i1]s1 [C#i2]s2
		// [C#im]sM to draw each piece s1, s2, ... sM under column #i1, column #i2, ... column #iM
		nTextWidth = pDC->GetTextExtent(pMetrics->strText).cx;
	}

	if (pMetrics->nGroupRowIcon != XTP_GRID_NOICON)
	{
		CRect rcIcon(rcRow);

		if (nGroupRowIconAlignment & xtpGroupRowIconBeforeText)
		{
			rcIcon.left = rcBitmap.left + nBitmapWidth + XTP_DPI_X(5);
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconAfterText)
		{
			rcIcon.left = rcText.left + nTextWidth + XTP_DPI_X(5);
			rcIcon.left = min(rcIcon.left, rcRow.right - szIcon.cx - XTP_DPI_X(1));
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconRight)
		{
			rcIcon.left = rcRow.right - szIcon.cx - XTP_DPI_X(1);
		}
		else
		{
			// xtpGroupRowIconLeft
		}

		if (nGroupRowIconAlignment & xtpGroupRowIconVTop)
		{
			rcIcon.bottom = rcIcon.top + szIcon.cy + XTP_DPI_Y(1);
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconVCenterToText)
		{
			rcIcon.top	= rcText.CenterPoint().y - szIcon.cy / 2;
			rcIcon.bottom = rcIcon.top + szIcon.cy + XTP_DPI_Y(1);
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconVBottom)
		{
			rcIcon.top = rcIcon.bottom - szIcon.cy - XTP_DPI_Y(1);
		}
		else
		{
			// xtpGroupRowIconVCenter
		}

		COLORREF clrBk = pMetrics->clrBackground != XTP_GRID_COLOR_DEFAULT ? pMetrics->clrBackground
																		   : RGB(255, 255, 255);

		COLORREF clrBkPrev = pDC->SetBkColor(clrBk);

		DrawBitmap(pDC, pControl, rcIcon, pMetrics->nGroupRowIcon);

		pDC->SetBkColor(clrBkPrev);
	}
	else if (m_bUseGroupRowSelectionState && !pDC->IsPrinting())
	{
		CRect rcIcon(rcBitmap);
		rcIcon.left = (rcBitmap.right + XTP_DPI_X(2));

		BOOL bY(FALSE);
		BOOL bN(FALSE);
		BOOL bOr(FALSE);
		BOOL bR(FALSE);
		if (pRow->GetChilds(FALSE))
		{
			for (int I = 0; I < pRow->GetChilds()->GetCount(); I++)
			{
				CXTPGridRow* pChRow = pRow->GetChilds()->GetAt(I);
				if (pChRow)
				{
					bOr					   = FALSE;
					CXTPGridRecord* pChRec = pChRow->GetRecord();
					if (pChRec)
					{
						bOr = pChRec->IsSelected();
						bR  = TRUE;
					}
					if (pChRow->IsSelected() || (bOr && !pRow->IsExpanded()))
						bY = TRUE;
					else
						bN = TRUE;
				}
			}
		}

		if (bY && !bN)
		{
			DrawGlyph(pDC, rcIcon, xtpGridGlyphEnabledChecked);
		}
		else if (bY && bN)
		{
			DrawGlyph(pDC, rcIcon, xtpGridGlyphDisabledChecked);
		}
		else if (bR)
		{
			DrawGlyph(pDC, rcIcon, xtpGridGlyphEnabledUnchecked);
		}
		else if (pRow->m_bHasSelectedChilds)
		{
			DrawGlyph(pDC, rcIcon, xtpGridGlyphDisabledUnchecked);
			pRow->m_bHasSelectedChilds = FALSE; // Clear local flag
		}
		if (bY) // cascade up
		{
			CXTPGridRow* pParRow = pRow->GetParentRow();
			while (pParRow && pParRow->IsGroupRow())
			{
				pParRow->m_bHasSelectedChilds = TRUE;
				pParRow						  = pParRow->GetParentRow();
			}
		}
	}
}

CSize CXTPGridPaintManager::DrawCollapsedBitmap(CDC* pDC, const CXTPGridRow* pRow, CRect& rcBitmap)
{
	// Draw Standard image from resource.
	rcBitmap.left += XTP_DPI_X(2);
	CSize sizeGlyph = DrawGlyph(pDC, rcBitmap, pRow->IsExpanded() ? 0 : 1);

	if (sizeGlyph.cx != 0 && pDC != NULL)
		rcBitmap.right = rcBitmap.left + sizeGlyph.cx;

	return CSize(XTP_DPI_X(2) + sizeGlyph.cx, XTP_DPI_Y(2) + sizeGlyph.cy);
}

void CXTPGridPaintManager::FillIndent(CDC* pDC, CRect rcRow)
{
	pDC->FillSolidRect(&rcRow, m_clrIndentControl);
}

void CXTPGridPaintManager::FillItemShade(CDC* pDC, CRect rcItem)
{
	// fill item background shade (for example when sorted by this column)
	if (!m_bShadeSortColumn)
		return;

	if (pDC)
		pDC->FillSolidRect(rcItem, m_clrItemShade);
}

void CXTPGridPaintManager::FillHeaderControl(CDC* pDC, CRect rcHeader)
{
	pDC->FillSolidRect(rcHeader, m_clrHeaderControl);
}

void CXTPGridPaintManager::DrawNoGroupByText(CDC* pDC, CRect rcItem)
{
	ASSERT(m_pControl);
	if (!m_pControl)
		return;

	if (m_pControl->GetMarkupContext())
	{
		if (!m_pMarkupUIElement_NoGroupBy || m_strNoGroupBy != m_strMarkupUIElement_NoGroupBy)
		{
			XTPMarkupReleaseElement(m_pMarkupUIElement_NoGroupBy);
			m_pMarkupUIElement_NoGroupBy   = XTPMarkupParseText(m_pControl->GetMarkupContext(),
																m_strNoGroupBy);
			m_strMarkupUIElement_NoGroupBy = m_strNoGroupBy;
		}

		if (m_pMarkupUIElement_NoGroupBy)
		{
			CRect rcGroupBy = m_pControl->GetElementRect(xtpGridElementRectGroupByArea);
			rcGroupBy.left += XTP_DPI_X(9);

			XTPMarkupMeasureElement(m_pMarkupUIElement_NoGroupBy, rcGroupBy.Width(), INT_MAX);

			XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), (HFONT)m_xtpFontCaption,
									m_crlNoGroupByText);
			XTPMarkupRenderElement(m_pMarkupUIElement_NoGroupBy, pDC->GetSafeHdc(), &rcGroupBy);

			return;
		}
	}
	else
	{
		XTPMarkupReleaseElement(m_pMarkupUIElement_NoGroupBy);
		m_strMarkupUIElement_NoGroupBy.Empty();
	}

	int nWidth   = max(XTP_DPI_X(55), pDC->GetTextExtent(m_strNoGroupBy).cx + XTP_DPI_X(8));
	rcItem.right = rcItem.left + nWidth;

	pDC->SetTextColor(m_crlNoGroupByText);

	FillHeaderControl(pDC, rcItem);

	pDC->DrawText(m_strNoGroupBy, &rcItem, DT_END_ELLIPSIS | DT_CENTER | DT_NOPREFIX);
}

void CXTPGridPaintManager::FillFooter(CDC* pDC, CRect rcFooter)
{
	pDC->FillSolidRect(rcFooter, m_clrHeaderControl);

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrControlBack);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(1), rcFooter.Width(),
					   m_clrControlDark);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(2), rcFooter.Width(),
					   MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(3), rcFooter.Width(),
					   MixColor(m_clrHeaderControl, m_clrControlDark, 0.25));

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - XTP_DPI_Y(1), rcFooter.Width(),
					   m_clrControlDark);
}

void CXTPGridPaintManager::DrawHorizontalLine(CDC* pDC, int x, int y, int cx, COLORREF clr)
{
	pDC->FillSolidRect(x, y, cx, XTP_DPI_Y(1), clr);
}

void CXTPGridPaintManager::DrawVerticalLine(CDC* pDC, int x, int y, int cy, COLORREF clr)
{
	pDC->FillSolidRect(x, y, XTP_DPI_X(1), cy, clr);
}

void CXTPGridPaintManager::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown,
										BOOL /*bDraggingFromHeader*/, int nShadowWidth,
										BOOL /*bColumnPressed*/)
{
	CRect rcTRect;
	int iLastImage = xtpGridGlyphCount; // last image in (m_pGlyphs->SetIcons(bmp, 0, 11, CSize(0,
										// 0)) = 11

	if (rcTriangle.Width() > XTP_DPI_X(10))
	{
		// get non-existed image placeholder and fill it with sorting arrows

		CXTPImageManagerIcon* pImage = m_pGlyphs->GetImage(
			XTPToUInt(bToDown ? iLastImage : iLastImage + 1));
		if (pImage)
		{
			pImage->Draw(pDC, CPoint((rcTriangle.left + rcTriangle.right - pImage->GetWidth()) / 2,
									 (rcTriangle.top + rcTriangle.bottom - pImage->GetHeight()) / 2
										 + (nShadowWidth ? XTP_DPI_X(1) : 0)));
			return;
		}

		CPoint pt(rcTriangle.CenterPoint());

		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, XTP_DPI_X(1), m_clrControlLightLight);
		CPen penShadow(PS_SOLID, XTP_DPI_X(1), m_clrControlDark);

		if (bToDown)
		{
			Line(pDC, pt.x, pt.y + XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(-6), &penLight);
			Line(pDC, pt.x - XTP_DPI_X(1), pt.y + XTP_DPI_Y(4), XTP_DPI_X(3), XTP_DPI_Y(-6),
				 &penLight);
			Line(pDC, pt.x - XTP_DPI_X(4), pt.y - XTP_DPI_Y(2), XTP_DPI_X(7), 0, &penShadow);
			Line(pDC, pt.x - XTP_DPI_X(4), pt.y - XTP_DPI_Y(1), XTP_DPI_X(3), XTP_DPI_Y(6),
				 &penShadow);
			Line(pDC, pt.x - XTP_DPI_X(5), pt.y - XTP_DPI_Y(2), XTP_DPI_X(3), XTP_DPI_Y(6),
				 &penShadow);
		}
		else
		{
			Line(pDC, pt.x, pt.y - XTP_DPI_Y(2), XTP_DPI_X(3), XTP_DPI_X(6), &penLight);
			Line(pDC, pt.x + XTP_DPI_X(1), pt.y - XTP_DPI_Y(1), XTP_DPI_X(3), XTP_DPI_X(6),
				 &penLight);
			Line(pDC, pt.x - XTP_DPI_X(3), pt.y + XTP_DPI_Y(4), XTP_DPI_X(6), 0, &penLight);
			Line(pDC, pt.x - XTP_DPI_X(4), pt.y + XTP_DPI_Y(4), XTP_DPI_X(3), XTP_DPI_X(-6),
				 &penShadow);
			Line(pDC, pt.x - XTP_DPI_X(3), pt.y + XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_X(-6),
				 &penShadow);
		}

		penLight.DeleteObject();
		penShadow.DeleteObject();
	}
}

void CXTPGridPaintManager::DrawItemBitmap(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem,
										  int nImage)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return;

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon =
		pDrawArgs->pControl->GetImageManager()->GetImage(XTPToUInt(nImage), 0);

	if (!pIcon)
		return;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcItem.Size());

	int nIconAlign = pDrawArgs->nTextAlign & xtpColumnIconMask;

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDrawArgs->pDC)
	{
		// calculate image position
		if (nIconAlign & xtpColumnIconRight)
		{
			ptIcon.x = rcItem.right - szImage.cx - XTP_DPI_X(1);
			rcItem.right -= (XTP_DPI_X(1) + szImage.cx); // shift text box to the left
		}
		else if (nIconAlign & xtpColumnIconCenter)
		{
			ptIcon.x = (rcItem.left + rcItem.right - szImage.cx) / 2;
		}
		else // xtpColumnIconLeft : by default
		{
			ptIcon.x = rcItem.left + XTP_DPI_X(1);
			rcItem.left += XTP_DPI_X(1) + szImage.cx; // shift text box to the right
		}

		// vertically align
		if (nIconAlign & xtpColumnIconTop)
		{
			ptIcon.y = rcItem.top;
		}
		else if (nIconAlign & xtpColumnIconBottom)
		{
			ptIcon.y = rcItem.bottom - szImage.cy - XTP_DPI_Y(1);
		}
		else // xtpColumnIconVCenter - by default
		{
			ptIcon.y = rcItem.top + (szColumn.cy - szImage.cy) / 2;
		}

		// draw image
		if (pDrawArgs->pDC->IsPrinting())
		{
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDrawArgs->pDC, szImage.cx, szImage.cy);
			CXTPCompatibleDC dcMem(NULL, &bmp);
			dcMem.FillSolidRect(0, 0, szImage.cx, szImage.cy, pDrawArgs->pDC->GetBkColor());

			pIcon->Draw(&dcMem, CPoint(0, 0), xtpImageNormal);
			pDrawArgs->pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem, 0, 0,
								   SRCCOPY);
		}
		else if (pIcon->IsRasterIcon())
		{
			pIcon->Draw(pDrawArgs->pDC, ptIcon, pIcon->GetIcon(), 0,
						pDrawArgs->pRow->IsSelected() ? GetXtremeColor(COLOR_HIGHLIGHT) : CLR_NONE,
						pDrawArgs->pRow->IsSelected() ? GetXtremeColor(COLOR_HIGHLIGHT) : CLR_NONE,
						UINT(pDrawArgs->pRow->IsSelected() ? ILD_SELECTED : 0));
		}
		else
		{
			pIcon->Draw(pDrawArgs->pDC, ptIcon, xtpImageNormal);
		}
	}
}

void CXTPGridPaintManager::DrawWindowFrame(CDC* /*pDC*/, CRect /*rcWindow*/)
{
	if (::IsWindow(m_pControl->GetSafeHwnd()))
	{
		m_pControl->Default();
	}
}

int CXTPGridPaintManager::DrawColumnIcon(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn,
										 CRect rcIcon, int iIcon)
{
	if (!pColumn || !pColumn->GetControl())
	{
		ASSERT(FALSE);
		return 0;
	}

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon =
		pColumn->GetControl()->GetImageManager()->GetImage(XTPToUInt(iIcon), 0);

	if (!pIcon)
		return 0;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcIcon.Size());

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDC)
	{
		ptIcon.x = rcIcon.left + XTP_DPI_X(1);
		ptIcon.y = rcIcon.top + (szColumn.cy - szImage.cy) / 2;

		if (pDC->IsPrinting())
		{
			CPoint ptIconOffset(ptIcon.x - rcColumn.left, ptIcon.y - rcColumn.top);
			CRect rcColumn2(0, 0, rcColumn.Width(), rcColumn.Height());

			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rcColumn2.Width(), rcColumn2.Height());

			CXTPCompatibleDC dcMem(NULL, &bmp);

			// Fill background
			dcMem.FillSolidRect(&rcColumn2, pDC->GetBkColor());
			BOOL bColumnPressed		 = FALSE;
			BOOL bDraggingFromHeader = FALSE;
			DrawColumnBackground(&dcMem, pColumn, rcColumn2, bColumnPressed, bDraggingFromHeader);

			pIcon->Draw(&dcMem, ptIconOffset, xtpImageNormal);

			pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem, ptIconOffset.x,
						ptIconOffset.y, SRCCOPY);
		}
		else
		{
			pIcon->Draw(pDC, ptIcon, xtpImageNormal);
		}
	}

	return XTP_DPI_X(1) + szImage.cx;
}

int CXTPGridPaintManager::DrawBitmap(CDC* pDC, CXTPGridControl* pControl, CRect rcColumn, int iIcon)
{
	ASSERT(pControl);
	if (!pControl)
		return 0;

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon =
		pControl->GetImageManager()->GetImage(-1 == iIcon ? UINT_MAX : XTPToUInt(iIcon), 0);

	if (!pIcon)
		return 0;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcColumn.Size());

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDC)
	{
		ptIcon.x = rcColumn.left + XTP_DPI_X(1);
		ptIcon.y = rcColumn.top + (szColumn.cy - szImage.cy) / 2;

		if (pDC->IsPrinting())
		{
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, szImage.cx, szImage.cy);
			CXTPCompatibleDC dcMem(NULL, &bmp);
			dcMem.FillSolidRect(0, 0, szImage.cx, szImage.cy, pDC->GetBkColor());

			pIcon->Draw(&dcMem, CPoint(0, 0), xtpImageNormal);
			pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem, 0, 0, SRCCOPY);
		}
		else
		{
			pIcon->Draw(pDC, ptIcon, xtpImageNormal);
		}
	}

	return XTP_DPI_X(1) + szImage.cx;
}

CSize CXTPGridPaintManager::DrawShellIcon(CDC* pDC, CRect rcColumn, int iImage)
{
	HICON hIcon = NULL;
	::ExtractIconEx(_T("SHELL32.DLL"), iImage, NULL, &hIcon, 1);

	CSize size = XTPSystemMetrics()->GetSmallIconSize();
	if (!XTPDpiHelper()->IsDpiBitmapScalingEnabled())
		size = XTP_UNDPI(size);

	if (hIcon)
	{
		::DrawIconEx(pDC->GetSafeHdc(), rcColumn.left + XTP_DPI_X(1), rcColumn.top + XTP_DPI_Y(1),
					 hIcon, size.cx, size.cy, 0, NULL, DI_NORMAL);

		::DestroyIcon(hIcon);
	}
	else
	{
		DrawGlyph(pDC, rcColumn, 9); // need index of Lock Icon - now it is 9 and 10 (open lock)
	}

	++size.cx;
	++size.cy;

	return size;
}

CSize CXTPGridPaintManager::DrawGlyph(CDC* pDC, CRect rcColumn, int iGlyph)
{
	CSize size(0, 0);

	CXTPImageManagerIcon* pImage = m_pGlyphs->GetImage(XTPToUInt(iGlyph));

	if (NULL != pImage)
	{
		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		CSize szColumn(rcColumn.Size());

		if (szImage.cy < szColumn.cy && pDC)
		{
			POINT ptIcon;

			ptIcon.x = rcColumn.left + XTP_DPI_X(1);
			ptIcon.y = rcColumn.top + (szColumn.cy - szImage.cy) / 2;
			pImage->Draw(pDC, ptIcon);
		}

		size = CSize(XTP_DPI_X(1) + szImage.cx, XTP_DPI_Y(1) + szImage.cy);
	}

	return size;
}

BOOL CXTPGridPaintManager::FindRowAdjacentToMergedEx(const CXTPGridColumns* pColumns,
													 const CXTPGridRow* pRow, int& nAdjRowIndex,
													 BOOL bUpward)
{
	ASSERT(pColumns);
	ASSERT(pRow);
	ASSERT(pRow->GetSection());
	ASSERT(pRow->GetSection()->GetRows());
	ASSERT(pRow->GetIndex() == nAdjRowIndex);

	if (pColumns == NULL || pRow == NULL || pRow->IsGroupRow())
		return FALSE;

	if (pRow->GetSection() == NULL || pRow->GetRecord() == NULL
		|| pRow->GetSection()->GetRows() == NULL)
		return FALSE;

	CXTPGridRows* pRows			   = pRow->GetSection()->GetRows();
	CXTPGridRecordItem* pMergeItem = NULL;

	int nStep		 = bUpward ? -1 : 1;
	int nColumn		 = 0;
	int nColumnCount = pColumns->GetCount();
	int nRow		 = 0;
	int nRowLast	 = pRows->GetCount() - 1;
	int nNextRow	 = nAdjRowIndex + nStep;

	if (nNextRow < 0 || nNextRow > nRowLast)
		return FALSE;

	for (; nColumn < nColumnCount; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			CXTPGridRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);
			if (pItem && pItem->IsMerged()
				&& pItem->GetMergeItem()
					   != pMergeItem) // pItem isn't in other group of merged cells
			{
				pMergeItem = pItem->GetMergeItem();

				for (nRow = pRow->GetIndex() + nStep; nRow >= 0 && nRow <= nRowLast; nRow += nStep)
				{
					CXTPGridRow* pRowNext = pRows->GetAt(nRow);
					if (pRowNext && pRowNext->GetRecord() && !pRowNext->IsGroupRow())
					{
						CXTPGridRecordItem* pItemNext = pRowNext->GetRecord()->GetItem(pColumn);
						if (pItemNext && pItemNext->GetMergeItem() == pMergeItem)
						{
							continue;
						}
					}
					break; // from for() nRow
				}
				nRow -= nStep; // step back

				if (bUpward ? (nAdjRowIndex > nRow) : (nAdjRowIndex < nRow))
					nAdjRowIndex = nRow;

				if (bUpward ? (nAdjRowIndex == 0) : (nAdjRowIndex == nRowLast))
					break; // from for() nColumn
			}
		}
	} // for() nColumn

	if (pRow->GetIndex() != nAdjRowIndex && nAdjRowIndex > 0 && nAdjRowIndex < nRowLast)
		return TRUE; // FindRowAdjacentToMergedEx(pColumns, pRows->GetAt(nAdjRowIndex), bUpward);

	return FALSE;
}

int CXTPGridPaintManager::FindRowAdjacentToMerged(const CXTPGridColumns* pColumns,
												  const CXTPGridRow* pRow, BOOL bUpward)
{
	ASSERT(pColumns);
	ASSERT(pRow);
	if (pColumns == NULL || pRow == NULL)
		return 0;

	CXTPGridRows* pRows = pRow->GetSection()->GetRows();
	int nAdjRowIndex	= pRow->GetIndex();
	BOOL bOnceAgain		= FALSE;
	do
	{
		CXTPGridRow* pAdjRow = pRows->GetAt(nAdjRowIndex);
		bOnceAgain			 = FindRowAdjacentToMergedEx(pColumns, pAdjRow, nAdjRowIndex,
												 bUpward); // continue in the same direction
	} while (bOnceAgain);

	return nAdjRowIndex;
}

int CXTPGridPaintManager::RecalcTotalHightOfMergedRows(CXTPGridColumns* pColumns, CDC* pDC,
													   CXTPGridRow* pRow, int nRowHeight,
													   int nTotalWidth)
{
	ASSERT(pColumns);
	ASSERT(pDC);
	ASSERT(pRow);
	if (pColumns == NULL || pDC == NULL || pRow == NULL)
		return 0;

	const int nMergeRowTop	= FindRowAdjacentToMerged(pColumns, pRow, TRUE);
	const int nMergeRowBottom = FindRowAdjacentToMerged(pColumns, pRow, FALSE);

	CXTPGridRows* pRows = pRow->GetSection()->GetRows();
	CRect crEmpty(0, 0, 0, 0);
	int nColumnCount = pColumns->GetCount();

	CXTPGridRecordMergeItems mergeItems;
	int nMRShift	   = nMergeRowTop; // Merge Rows Shift
	int nMergeRowsCnt  = nMergeRowBottom - nMergeRowTop + 1;
	int* mergeRows	 = new int[XTPToUIntChecked(nMergeRowsCnt)];
	int* mergeRowsUsed = new int[XTPToUIntChecked(nMergeRowsCnt)];
	int* mergeColumns  = new int[XTPToUIntChecked(nColumnCount)];

	ASSERT(mergeRows != NULL);
	ASSERT(mergeRowsUsed != NULL);
	ASSERT(mergeColumns != NULL);

	int nIndex = 0;
	for (; nIndex < nMergeRowsCnt; nIndex++)
	{
		mergeRows[nIndex]	 = 0;
		mergeRowsUsed[nIndex] = 0;
	}

	mergeRows[pRow->GetIndex() - nMRShift]	 = nRowHeight;
	mergeRowsUsed[pRow->GetIndex() - nMRShift] = 1;

	int nRow = 0;
	for (nRow = nMergeRowTop; nRow <= nMergeRowBottom; nRow++)
	{
		// calc height of rows without merge cells
		CXTPGridRow* pRowNext = pRows->GetAt(nRow);

		if (!pRowNext->IsItemsVisible())
			continue;

		if (pRowNext != pRow) // we have height of pRow in nRowHeight already
		{
			mergeRows[nRow - nMRShift]	 = GetRowHeightEx(pDC, pRowNext, nTotalWidth, FALSE);
			mergeRowsUsed[nRow - nMRShift] = 1; // default value for ignored rows is 0
		}

		int nColumnPos = 0;
		// save to mergeItems all merged cells which related to height of merge item
		for (int nColumnNext = 0; nColumnNext < nColumnCount; nColumnNext++)
		{
			CXTPGridColumn* pColumnNext = pColumns->GetAt(nColumnNext);
			if (pColumnNext && pColumnNext->IsVisible())
			{
				nColumnPos++;
				CXTPGridRecordItem* pItemNext = pRowNext->GetRecord()->GetItem(pColumnNext);
				if (pItemNext && pItemNext->IsMerged())
				{
					CXTPGridRecordMergeItemId itemId(nColumnPos, nColumnNext, nRow);
					mergeItems[pItemNext->GetMergeItem()].AddItem(itemId, crEmpty);
				}
			}
		}
	}

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++) // get width of all columns
	{
		CXTPGridColumn* pColumnNext = pColumns->GetAt(nColumn);
		if (pColumnNext && pColumnNext->IsVisible())
		{
			if (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG)
				mergeColumns[nColumn] = pColumnNext->GetPrintWidth(nTotalWidth);
			else
				mergeColumns[nColumn] = pColumnNext->GetWidth();
		}
		else
			mergeColumns[nColumn] = 0;
	}

	POSITION pos = mergeItems.GetStartPosition();
	while (pos)
	{
		CXTPGridRecordItem* pItem;
		CXTPGridRecordMergeItem mergeItem;

		mergeItems.GetNextAssoc(pos, pItem, mergeItem);

		CXTPGridRecordItemRange range;
		CRect rcItem;

		while (mergeItem.FindRange(range, rcItem))
		{
			// get sum of merged row height
			int nRowsRangeHeight = 0;

			for (nIndex = range.m_nRecordFrom; nIndex <= range.m_nRecordTo; nIndex++)
			{
				nRowsRangeHeight += mergeRows[nIndex - nMRShift];
			}

			int nColumnsWidth = 0;
			for (nIndex = range.m_nColumnFrom; nIndex <= range.m_nColumnTo; nIndex++)
			{
				nColumnsWidth += mergeColumns[nIndex];
			}

			XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
			drawArgs.pControl = pRow->GetControl();
			drawArgs.pDC	  = pDC;
			drawArgs.pRow	 = pRow;
			drawArgs.pItem	= pItem;
			drawArgs.pColumn  = pColumns->GetAt(range.m_nColumnFrom); // to support MultiLine Mode

			int nItemHeight  = GetItemHeight(drawArgs, nColumnsWidth);
			int nDeltaHeight = nItemHeight - nRowsRangeHeight;

			if (nDeltaHeight <= 0)
			{
				continue; // from while(FindRange())
			}

			int nRowsRangeCount = 0;
			for (nIndex = range.m_nRecordFrom; nIndex <= range.m_nRecordTo; nIndex++)
			{
				if (mergeRowsUsed[nIndex - nMRShift])
					nRowsRangeCount++;
			}

			if (nRowsRangeCount == 1)
			{
				for (nIndex = range.m_nRecordFrom; nIndex <= range.m_nRecordTo; nIndex++)
				{
					if (mergeRowsUsed[nIndex - nMRShift])
					{
						mergeRows[nIndex - nMRShift] += nDeltaHeight;
						break;
					}
				}
				continue; // from while(FindRange())
			}

			//	// simple algorithm
			//		int nPerRow		= nDeltaHeight / nRowsRangeCount;
			//		int nResidual	= nDeltaHeight % nRowsRangeCount;
			//		for (nIndex = range.m_nRecordFrom; nIndex <= range.m_nRecordTo; nIndex++)
			//			if (mergeRowsUsed[nIndex - nMRShift])
			//				mergeRows[nIndex - nMRShift] += nPerRow + (nResidual-- > 0 ? 1 : 0);

			// average algorithm

			int nRowHeightMax = 0;
			for (nIndex = range.m_nRecordFrom; nIndex <= range.m_nRecordTo; nIndex++)
			{
				if (mergeRowsUsed[nIndex - nMRShift]
					&& nRowHeightMax < mergeRows[nIndex - nMRShift])
				{
					nRowHeightMax = mergeRows[nIndex - nMRShift];
				}
			}

			int nShortageToMax = nRowHeightMax * nRowsRangeCount - nRowsRangeHeight;

			if (nDeltaHeight >= nShortageToMax)
			{
				nDeltaHeight -= nShortageToMax;
				int nPerRow   = nDeltaHeight / nRowsRangeCount;
				int nResidual = nDeltaHeight % nRowsRangeCount;
				for (nIndex = range.m_nRecordFrom; nIndex <= range.m_nRecordTo; nIndex++)
				{
					if (mergeRowsUsed[nIndex - nMRShift])
						mergeRows[nIndex - nMRShift] = nRowHeightMax + nPerRow
													   + (nResidual-- > 0 ? 1 : 0);
				}
			}
			else
			{
				int nNewAverage			= (nRowsRangeHeight + nDeltaHeight) / nRowsRangeCount;
				int nNewRowsRangeCount  = nRowsRangeCount;
				int nNewRowsRangeHeight = nRowsRangeHeight;
				int nRecord;
				for (nRecord = range.m_nRecordFrom; nRecord <= range.m_nRecordTo; nRecord++)
				{
					if (mergeRowsUsed[nRecord - nMRShift]
						&& mergeRows[nRecord - nMRShift] >= nNewAverage)
					{
						nNewRowsRangeHeight -= mergeRows[nRecord - nMRShift];
						nNewRowsRangeCount--;
					}
				}
				nNewAverage = (nNewRowsRangeHeight + nDeltaHeight) / nNewRowsRangeCount;

				for (nRecord = range.m_nRecordFrom; nRecord <= range.m_nRecordTo; nRecord++)
				{
					if (mergeRows[nRecord - nMRShift] < nNewAverage)
					{
						nDeltaHeight -= nNewAverage - mergeRows[nRecord - nMRShift];
						mergeRows[nRecord - nMRShift] = nNewAverage;
					}
				}
			}
		} // while(FindRange())
	}	 // while (pos)

	// save height of rows
	for (nRow = nMergeRowTop; nRow <= nMergeRowBottom; nRow++)
	{
		CXTPGridRow* pRowNext = pRows->GetAt(nRow);
		if (!pRowNext->IsItemsVisible())
			continue;

		pRowNext->SetMergeHeight(mergeRows[nRow - nMRShift]);
	}

	int retValue = mergeRows[pRow->GetIndex() - nMRShift];

	delete[] mergeRows;
	delete[] mergeRowsUsed;
	delete[] mergeColumns;

	return retValue;
}

int CXTPGridPaintManager::GetItemHeight(XTP_GRIDRECORDITEM_DRAWARGS& drawArgs, int nWidth)
{
	ASSERT(drawArgs.pItem);
	ASSERT(drawArgs.pColumn);
	ASSERT(drawArgs.pControl);
	ASSERT(drawArgs.pDC);
	ASSERT(drawArgs.pRow);

	drawArgs.nTextAlign						 = drawArgs.pColumn->GetAlignment();
	XTP_GRIDRECORDITEM_METRICS* pItemMetrics = new XTP_GRIDRECORDITEM_METRICS;
	int nHeight								 = 0;

	drawArgs.pRow->GetItemMetrics(&drawArgs, pItemMetrics);

	CXTPFontDC fnt(drawArgs.pDC, pItemMetrics->pFont);

	CRect rcItem(0, 0, nWidth - XTP_DPI_X(4), 0);
	drawArgs.pRow->ShiftTreeIndent(rcItem, drawArgs.pColumn);

	drawArgs.pItem->GetCaptionRect(&drawArgs, rcItem);

	int nIconW(0), nIconH(0), nIconIndex(drawArgs.pItem->GetIconIndex()),
		mIconIndex(pItemMetrics->nItemIcon);

	if (nIconIndex != XTP_GRID_NOICON)
	{
		CXTPImageManagerIcon* pIcon =
			drawArgs.pControl->GetImageManager()->GetImage(XTPToUInt(nIconIndex), 0);

		if (NULL != pIcon)
		{
			nIconW = pIcon->GetWidth();
			nIconH = pIcon->GetHeight();
		}
	}
	else if (mIconIndex != XTP_GRID_NOICON)
	{
		CXTPImageManagerIcon* pIcon =
			drawArgs.pControl->GetImageManager()->GetImage(XTPToUInt(mIconIndex), 0);

		if (pIcon)
		{
			nIconW = pIcon->GetWidth();
			nIconH = pIcon->GetHeight();
		}
	}
	if (drawArgs.pItem->GetMarkupUIElement())
	{
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(drawArgs.pItem->GetMarkupUIElement()),
								(HFONT)pItemMetrics->pFont->GetSafeHandle(), COLORREF_NULL);

		int nCalculatedHeight = XTPMarkupMeasureElement(drawArgs.pItem->GetMarkupUIElement(),
														rcItem.Width(), INT_MAX)
									.cy;
		nHeight = max(nHeight, nCalculatedHeight);
	}
	else
	{
		BOOL bWordBreak = !!(pItemMetrics->nColumnAlignment & DT_WORDBREAK);

		CString strText;
		if (drawArgs.pControl->IsVirtualMode())
			strText = pItemMetrics->strText;
		else
			strText = drawArgs.pItem->GetCaption(drawArgs.pColumn);

		ReplaceInHyperLinks(drawArgs.pItem, strText, XTP_HLINK_SPACE_X);

		int iWid = rcItem.Width();
		iWid -= nIconW;
		iWid -= 3; // tolerance level
		int nCalculatedHeight = CalculateRowHeight(drawArgs.pDC, strText, iWid, bWordBreak);
		nHeight				  = max(nHeight, nCalculatedHeight);
	}
	nHeight = max(nHeight, nIconH);

	int iExtraPix = XTP_DPI_Y(5);
	nHeight = max(nHeight + iExtraPix, m_nRowHeight) + (IsGridVisible(FALSE) ? XTP_DPI_Y(1) : 0);

	CMDTARGET_RELEASE(pItemMetrics);
	return nHeight;
}

int CXTPGridPaintManager::GetRowHeightEx(CDC* pDC, CXTPGridRow* pRow, int nTotalWidth,
										 const BOOL firstLevel)
{
	BOOL bProcessMerged		  = FALSE;
	CXTPGridControl* pControl = DYNAMIC_DOWNCAST(CXTPGridControl, pRow->GetControl());

	if (NULL == pControl)
	{
		return 0;
	}

	if (!pControl->IsIconView())
	{
		if (pControl->m_bFreeHeightMode)
		{
			if (pRow->m_nFreeHeight == 0)
				pRow->m_nFreeHeight = pControl->m_nDefaultRowFreeHeight;

			if (pRow->GetRecord())
			{
				if (pRow->GetRecord()->m_nFreeHeight > 0)
					pRow->m_nFreeHeight = pRow->GetRecord()->m_nFreeHeight;
				else
					pRow->GetRecord()->m_nFreeHeight = pRow->m_nFreeHeight;
			}

			return pRow->m_nFreeHeight;
		}
		if (m_bFixedRowHeight || pRow->IsGroupRow() || !pRow->IsItemsVisible())
			return GetRowHeight(pDC, pRow);

		if (pRow->GetRecord() == NULL)
		{
			return 0;
		}

		if (pControl->IsCellMergingAllowed() && pRow->GetMergeHeight() != -1)
		{
			return pRow->GetMergeHeight();
		}

		CXTPGridColumns* pColumns = pControl->GetColumns();
		int nColumnCount		  = pColumns->GetCount();

		XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
		drawArgs.pControl = pRow->GetControl();
		drawArgs.pDC	  = pDC;
		drawArgs.pRow	 = pRow;

		int nHeight = m_nRowHeight + (IsGridVisible(FALSE) ? XTP_DPI_Y(1) : 0);

		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
			if (pColumn && pColumn->IsVisible())
			{
				CXTPGridRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);
				if (!pItem)
					continue;

				if (pItem->IsMerged())
				{
					if (firstLevel && !bProcessMerged)
						bProcessMerged = TRUE;

					continue; // process merged cells later
				}
				drawArgs.pItem   = pItem;
				drawArgs.pColumn = pColumn; // to support MultiLine Mode

				int nWidth = (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG)
								 ? pColumn->GetPrintWidth(nTotalWidth)
								 : pColumn->GetWidth();

				nHeight = max(GetItemHeight(drawArgs, nWidth), nHeight);
			}
		}

		if (bProcessMerged)
			nHeight = RecalcTotalHightOfMergedRows(pColumns, pDC, pRow, nHeight, nTotalWidth);

		return nHeight;
	}
	else
	{
		if (pControl->GetRowsPerLine() && pRow->GetIndex() % pControl->GetRowsPerLine())
			return 0;
		else
			return pControl->m_iIconHeightSpacing;
	}
}

int CXTPGridPaintManager::GetRowHeight(CDC* pDC, CXTPGridRow* pRow)
{
	UNUSED_ALWAYS(pDC);

	int nRowHeight = m_nRowHeight;

	if (pRow->IsGroupRow())
	{
		if (m_bShadeGroupHeadings)
		{
			nRowHeight += XTP_DPI_Y(6);
		}
		else
		{
			nRowHeight += XTP_DPI_Y(16);
		}
	}
	else
	{
		if (IsGridVisible(xtpGridOrientationHorizontal))
		{
			nRowHeight += XTP_DPI_Y(1);
		}
	}

	return nRowHeight;
}

int CXTPGridPaintManager::GetRowHeight(CDC* pDC, CXTPGridRow* pRow, int nTotalWidth)
{
	int nHeight = GetRowHeightEx(pDC, pRow, nTotalWidth);

#ifdef _XTP_ACTIVEX
	if (m_pControl->m_bCustomMeasureRow)
	{
		m_pControl->FireMeasureRow(pRow->GetIDispatch(FALSE),
								   (OLE_HANDLE) reinterpret_cast<LONG_PTR>(pDC->GetSafeHdc()),
								   nTotalWidth, &nHeight);

		_variant_t vtHeight = (long)nHeight;
		m_pControl->FireMeasureRowV(pRow->GetIDispatch(FALSE),
									(OLE_HANDLE) reinterpret_cast<LONG_PTR>(pDC->GetSafeHdc()),
									nTotalWidth, &vtHeight);
		nHeight = (long)vtHeight;
	}
#endif

	return nHeight;
}

BOOL CXTPGridPaintManager::IsColumHotTrackingEnabled() const
{
	return m_bHotTracking;
}

void CXTPGridPaintManager::DrawPlainColumnBackground(CDC* pDC, CRect rcColumn)
{
	pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrControlDark);
	pDC->MoveTo(rcColumn.left, rcColumn.bottom);
	pDC->LineTo(rcColumn.right, rcColumn.bottom);
	pDC->LineTo(rcColumn.right, rcColumn.top - XTP_DPI_Y(1));
}

void CXTPGridPaintManager::DrawColumnBackground(CDC* /*pDC*/, CXTPGridColumn* /*pColumn*/,
												CRect /*rcColumn*/, BOOL& /*bColumnPressed*/,
												BOOL& /*bDraggingFromHeader*/,
												CXTPGridHeader* /*pHeader*/)
{
}

void CXTPGridPaintManager::DrawRowNumber(CDC* pDC, CRect rcNumber, CXTPGridRow* pRow)
{
	if (!m_bRecOrRowNum) // Row # case
	{
		int indRow = pRow->GetIndex();
		if (m_iRecOrRowStart > 0)
			indRow += m_iRecOrRowStart;
		CString sRec;
		sRec.Format(_T(" %d "), indRow);

		if (pRow->IsSelected())
		{
			if (pRow->GetControl()->HasFocus())
				pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_HIGHLIGHT));
			else
				pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_BTNFACE));

			CXTPFontDC font(pDC, GetTextFont(), RGB(255, 255, 255));
			// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
			// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
			// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
			rcNumber.top++;
			pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
		}
		else
		{
			// pDC->FillSolidRect(rcNumber, RGB(224,224,224));
			// if we want to indicate read-only mode of record's numbers
			CXTPFontDC font(pDC, GetTextFont(), GetXtremeColor(COLOR_HIGHLIGHT));
			// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
			// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
			// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
			rcNumber.top++;
			pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
		}
	}
	else // Record # case
	{
		CXTPGridRecord* pRec = pRow->GetRecord();
		if (pRec)
		{
			int indRec = pRec->GetIndex();

			if (m_iRecOrRowStart > 0)
				indRec += m_iRecOrRowStart;

			CString sRec;
			sRec.Format(_T(" %d "), indRec);
			if (pRow->IsSelected())
			{
				if (pRow->GetControl()->HasFocus())
					pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_HIGHLIGHT));
				else
					pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_BTNFACE));

				CXTPFontDC font(pDC, GetTextFont(), RGB(255, 255, 255));
				// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
				// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
				// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
				rcNumber.top++;
				pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
			}
			else
			{
				// pDC->FillSolidRect(rcNumber, RGB(224,224,224));
				// if we want to indicate read-only mode of record's numbers
				CXTPFontDC font(pDC, GetTextFont(), GetXtremeColor(COLOR_HIGHLIGHT));
				// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
				// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
				// pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
				rcNumber.top++;
				pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
			}
		}
	}
}

void CXTPGridPaintManager::DrawColumnFooterDivider(CDC* pDC, CXTPGridColumn* pColumn,
												   CRect rcColumn)
{
	if (pColumn->GetDrawFooterDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right, rcColumn.top + XTP_DPI_Y(6),
						 rcColumn.Height() - XTP_DPI_Y(9), m_clrControlLightLight);
		DrawVerticalLine(pDC, rcColumn.right - XTP_DPI_X(1), rcColumn.top + XTP_DPI_Y(5),
						 rcColumn.Height() - XTP_DPI_Y(9), m_clrControlDark);
	}
}

void CXTPGridPaintManager::DrawColumnFooter(CDC* pDC, CXTPGridColumn* pColumn,
											CXTPGridHeader* /*pHeader*/, CRect rcColumn)
{
	DrawColumnFooterDivider(pDC, pColumn, rcColumn);

	pDC->SetBkMode(TRANSPARENT);
	CXTPFontDC font(pDC, pColumn->GetFooterFont(), m_clrCaptionText);

	CString strCaption = pColumn->GetFooterText();
	CSize sizeText	 = pDC->GetTextExtent(strCaption);
	CRect rcText(rcColumn);

	rcText.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(2), XTP_DPI_X(3), XTP_DPI_Y(2));

	if (pColumn->GetFooterAlignment() != DT_LEFT)
	{
		int nLength = XTP_DPI_X(6) + sizeText.cx;

		if (rcText.Width() > nLength)
		{
			if (pColumn->GetFooterAlignment() & DT_RIGHT)
				rcText.left = rcText.right - nLength;
			if (pColumn->GetFooterAlignment() & DT_CENTER)
				rcText.left = (rcText.left + rcText.right - nLength) / 2;
		}
	}

	UINT uFlags = DT_END_ELLIPSIS | DT_NOPREFIX;

	if (!m_bFixedRowHeight && (pColumn->GetFooterAlignment() & DT_WORDBREAK))
	{
		uFlags |= DT_WORDBREAK | DT_WORD_ELLIPSIS;

		// try to center vertically because DT_VCENTER works only for DT_SINGLELINE;
		CRect rcTextReal = rcText;
		pDC->DrawText(strCaption, &rcTextReal, uFlags | DT_CALCRECT);

		int nHeightDiff = rcText.Height() - rcTextReal.Height();
		if (nHeightDiff > XTP_DPI_Y(1))
		{
			rcText.top += nHeightDiff / 2;
		}
	}
	else
	{
		uFlags |= DT_SINGLELINE | DT_VCENTER;
	}

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left)
	{
		pDC->DrawText(strCaption, &rcText, uFlags);
	}
}

void CXTPGridPaintManager::DrawColumnText(CDC* pDC, CRect rcText, CString strCaption,
										  int nHeaderAlignment, BOOL bIsHeader, BOOL bColumnPressed)
{
	UNREFERENCED_PARAMETER(bIsHeader);

	pDC->SetTextColor(bColumnPressed ? m_clrCaptionTextPressed : m_clrCaptionText);

	UINT uFlags = DT_END_ELLIPSIS | DT_NOPREFIX;
	if (nHeaderAlignment & DT_RIGHT)
		uFlags |= DT_RIGHT;
	else if (nHeaderAlignment & DT_CENTER)
		uFlags |= DT_CENTER;

	if ((nHeaderAlignment & DT_WORDBREAK) > 0)
	{
		uFlags |= DT_WORDBREAK | DT_WORD_ELLIPSIS;

		// try to center vertically because DT_VCENTER works only for DT_SINGLELINE;
		CRect rcTextReal = rcText;
		pDC->DrawText(strCaption, &rcTextReal, uFlags | DT_CALCRECT);

		int nHeightDiff = rcText.Height() - rcTextReal.Height();
		if (nHeightDiff > XTP_DPI_Y(1))
			rcText.top += nHeightDiff / 2;
		else if (nHeightDiff < 0)
		{
			rcText.top += XTP_DPI_Y(3);
		}
	}
	else
	{
		uFlags |= DT_SINGLELINE | DT_VCENTER;
	}

	pDC->DrawText(strCaption, &rcText, uFlags);
}

void CXTPGridPaintManager::DrawColumn(CDC* pDC, CXTPGridColumn* pColumn, CXTPGridHeader* pHeader,
									  CRect rcColumn, BOOL bDrawExternal, int nShadowWidth /*= 0*/)
{
	// save column parameter for future use in drawing
	BOOL bDraggingFromHeader = bDrawExternal && pColumn->IsDragging() && rcColumn.left == 0
							   && rcColumn.top == 0 && pHeader->IsDragHeader();

	BOOL bPlainColumn = bDrawExternal && !bDraggingFromHeader;

	BOOL bColumnPressed = pColumn->IsDragging() && !bDrawExternal && pHeader->IsDragHeader();

	CRect rcColumn0 = rcColumn;
	// draw
	int nIconID		   = pColumn->GetIconID();
	CString strCaption = pColumn->GetCaption();

	if (bPlainColumn)
	{
		DrawPlainColumnBackground(pDC, rcColumn);
	}
	else
	{
		DrawColumnBackground(pDC, pColumn, rcColumn, bColumnPressed, bDraggingFromHeader, pHeader);
	}

	pDC->SetBkColor(m_clrHeaderControl);

	rcColumn.bottom = rcColumn.bottom - XTP_DPI_Y(1) - nShadowWidth;

	CRect rcText(rcColumn);
	if (!pColumn->IsAutoSize() && !bPlainColumn)
		rcText.left += pColumn->GetIndent();

	BOOL bHasSortTriangle = pColumn->HasSortTriangle() && pColumn->IsSortable()
							&& (!pDC->IsPrinting() || m_bPrintSortTriangle);

	if (m_DrawSortTriangleStyle == xtpGridDrawSortTriangleNever)
		bHasSortTriangle = FALSE;

	CXTPFontDC font(pDC, &m_xtpFontCaption);

	if (bPlainColumn == FALSE && m_bMakeUpper == TRUE)
	{
		strCaption.MakeUpper();
	}

	CSize sizeText(0, 0);
	CXTPMarkupUIElement* pMarkupUIElement = pColumn->GetMarkupUIElement();
	if (pMarkupUIElement)
	{
		sizeText = XTPMarkupMeasureElement(pMarkupUIElement, INT_MAX, INT_MAX);
	}
	else
	{
		sizeText = pDC->GetTextExtent(strCaption);
	}

	int nHeaderAlignment = pColumn->GetHeaderAlignment();
	BOOL bTextRight		 = (nHeaderAlignment & DT_RIGHT) > 0;
	BOOL bTextCenter	 = (nHeaderAlignment & DT_CENTER) > 0;
	BOOL bTextLeft		 = !bTextRight && !bTextCenter;
	BOOL bIconRight		 = (nHeaderAlignment & xtpColumnIconRight) > 0;
	BOOL bIconCenter	 = (nHeaderAlignment & xtpColumnIconCenter) > 0;
	if (strCaption.GetLength() > 0 && bIconCenter)
	{
		bIconCenter = FALSE;
		bIconRight  = TRUE;
	}
	int nIconWidth = DrawBitmap(NULL, pColumn->GetControl(), rcText, nIconID) + XTP_DPI_X(2);

	const int nTriangleGap  = (bPlainColumn ? XTP_DPI_X(17) : XTP_DPI_X(10));
	const int nTriangleSize = XTP_DPI_X(16);

	if (bHasSortTriangle && m_DrawSortTriangleStyle != xtpGridDrawSortTriangleAlways
		&& (XTP_DPI_X(6) + sizeText.cx + nTriangleGap + nTriangleSize + XTP_DPI_X(2))
			   > rcColumn.Width())
		bHasSortTriangle = FALSE;

	if (!bPlainColumn) // PlainColumn don't use PlusMinus Glyph and Column Icon
	{
		if (pColumn->IsPlusMinus() && !pDC->IsPrinting() && pColumn->GetNextVisualBlock() != 0)
		{
			int iIconWidthExtra = DrawGlyph(NULL, rcText, 1).cx;
			if (iIconWidthExtra > 0)
			{
				CRect rcIcon(rcText);
				rcIcon.left = rcIcon.right - iIconWidthExtra - XTP_DPI_X(2);
				if (m_bColumnPlusMinusSign) //[+|-] glyph
					DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 1 : 0);
				else //[<|>] glyph
				{
					if (pColumn->GetNextVisualBlock() > 0)
						DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 7 : 6);
					else if (pColumn->GetNextVisualBlock() < 0)
						DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 7 : 6);
				}
				rcText.right = rcIcon.left;
			}
		}

		// Check - do we have enough space to draw Icon
		if (m_bCompactText && nIconID != XTP_GRID_NOICON && rcText.Width() <= 2 * nIconWidth
			&& strCaption.GetLength() > 0)
		{
			nIconID = XTP_GRID_NOICON; // can't draw
		}

		if (!bTextLeft && !m_bSortTriangleOffsetFromRight)
		{
			int nLength = XTP_DPI_X(6) + sizeText.cx;

			if (nIconID != XTP_GRID_NOICON)
				nLength += nIconWidth;

			if (bHasSortTriangle)
				nLength += nTriangleGap + nTriangleSize + XTP_DPI_X(2);

			if (rcText.Width() > nLength)
			{
				if (bTextRight)
					rcText.left = rcText.right - nLength;
				else if (bTextCenter)
				{
					rcText.left  = (rcText.left + rcText.right - nLength) / 2;
					rcText.right = rcText.left + nLength;
				}
			}
		}

		if (nIconID != XTP_GRID_NOICON)
		{
			CRect rcIcon(rcText);
			rcIcon.bottom += nShadowWidth;
			if (bIconCenter) // No Text case only
			{
				int w = rcIcon.Width() - nIconWidth;
				rcIcon.left += w / 2;
				DrawColumnIcon(pDC, pColumn, rcColumn0, rcIcon, nIconID);
				rcText.left = rcIcon.left + nIconWidth + XTP_DPI_X(2);
			}
			else if (bIconRight)
			{
				rcIcon.left = rcIcon.right - nIconWidth;
				DrawColumnIcon(pDC, pColumn, rcColumn0, rcIcon, nIconID);
				rcText.right = rcIcon.left;
			}
			else
			{
				rcText.left += DrawColumnIcon(pDC, pColumn, rcColumn0, rcIcon, nIconID);
				rcText.left += XTP_DPI_X(2);
			}
		}
	}
	else
	{
		nIconID	= -1;
		nIconWidth = 0;
	}
	pDC->SetBkMode(TRANSPARENT);

	rcText.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);

	CRect rcTriangle(rcText);

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left
		&& pColumn->GetDisplayOptions()->Column()->IsShowText())
	{
		if (bHasSortTriangle)
		{
			if ((sizeText.cx + nTriangleGap + nTriangleSize > rcText.Width())
				&& (m_DrawSortTriangleStyle == xtpGridDrawSortTriangleAlways)
				&& (rcText.Width() > nTriangleSize))
			{
				rcTriangle.left = rcTriangle.right - nTriangleSize;
			}
			else
			{
				rcTriangle.left += sizeText.cx + nTriangleGap;
			}
			rcText.right -= nTriangleGap + nTriangleSize + XTP_DPI_X(2);
		}

		if (pMarkupUIElement)
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement),
									(HFONT)m_xtpFontCaption.GetSafeHandle(), m_clrCaptionText);
			XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), &rcText);
		}
		else
		{
			DrawColumnText(pDC, rcText, strCaption, nHeaderAlignment, bPlainColumn, bColumnPressed);
		}
	}

	if (bHasSortTriangle && rcTriangle.Width() >= nTriangleSize)
	{
		if (m_bSortTriangleOffsetFromRight)
			rcTriangle.left = rcTriangle.right - nTriangleSize;
		else
			rcTriangle.right = rcTriangle.left + nTriangleSize;

		if (rcTriangle.right <= rcColumn.right)
			DrawTriangle(pDC, rcTriangle, pColumn->IsSortedDecreasing(), bDraggingFromHeader,
						 nShadowWidth, bColumnPressed);
	}

	DrawColumnInverted(pDC, rcColumn, bColumnPressed, bDraggingFromHeader, nShadowWidth);
}

void CXTPGridPaintManager::DrawColumnInverted(CDC* pDC, CRect rcColumn, BOOL bColumnPressed,
											  BOOL /*bDraggingFromHeader*/, int nShadowWidth)
{
	if (bColumnPressed && m_bInvertColumnOnClick)
	{
		rcColumn.bottom += nShadowWidth;
		pDC->InvertRect(&rcColumn);
	}
}

int CXTPGridPaintManager::CalcColumnHeight(CDC* pDC, CXTPGridColumn* pColumn, int nTotalWidth)
{
	CString strCaption = pColumn->GetCaption();

	if (strCaption.GetLength() == 0 || !pColumn->GetDisplayOptions()->Column()->IsShowText())
		return XTP_DPI_Y(8);

	if (m_bMakeUpper == TRUE)
	{
		strCaption.MakeUpper();
	}

	int nWidth = (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG) ? pColumn->GetPrintWidth(nTotalWidth)
															   : pColumn->GetWidth();
	CRect rcColumn = pColumn->GetRect();
	rcColumn.right = rcColumn.left + nWidth;

	CRect rcText(rcColumn);

	if (!pColumn->IsAutoSize())
		rcText.left += pColumn->GetIndent();

	BOOL bHasSortTriangle = pColumn->HasSortTriangle()
							&& (!pDC->IsPrinting() || m_bPrintSortTriangle);
	const int nTriangleGap   = XTP_DPI_Y(10); // not plain column
	const int nTriangleSize  = XTP_DPI_Y(16);
	const int nTriangleMagic = XTP_DPI_Y(2);
	const int nTriangleWidth = nTriangleGap + nTriangleSize + nTriangleMagic;

	CXTPFontDC font(pDC, &m_xtpFontCaption);
	CSize sizeText(0, 0);
	CXTPMarkupUIElement* pMarkupUIElement = pColumn->GetMarkupUIElement();
	if (pMarkupUIElement)
	{
		sizeText = XTPMarkupMeasureElement(pMarkupUIElement, INT_MAX, INT_MAX);
	}
	else
	{
		sizeText = pDC->GetTextExtent(strCaption);
	}

	if (bHasSortTriangle)
	{
		if (m_DrawSortTriangleStyle == xtpGridDrawSortTriangleAlways
			|| m_DrawSortTriangleStyle == xtpGridDrawSortTriangleDefault
				   && (XTP_DPI_X(6) + sizeText.cx + nTriangleWidth) <= rcColumn.Width())
		{
			rcText.right -= nTriangleWidth;
		}
	}

	if (pColumn->IsPlusMinus() && !pDC->IsPrinting() && pColumn->GetNextVisualBlock() != 0)
	{
		int iIconWidthExtra = DrawGlyph(NULL, rcText, 1).cx;
		if (iIconWidthExtra > 0)
		{
			CRect rcIcon(rcText);
			rcIcon.left = rcIcon.right - iIconWidthExtra - XTP_DPI_X(2);
			if (m_bColumnPlusMinusSign) //[+|-] glyph
				DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 1 : 0);
			else //[<|>] glyph
			{
				if (pColumn->GetNextVisualBlock() > 0)
					DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 7 : 6);
				else if (pColumn->GetNextVisualBlock() < 0)
					DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 7 : 6);
			}
			rcText.right = rcIcon.left;
		}
	}

	int nIconID = pColumn->GetIconID();
	if (nIconID != XTP_GRID_NOICON)
	{
		// shift column left by bitmap drawing place
		rcText.left += DrawBitmap(NULL, pColumn->GetControl(), rcText, nIconID);
		// shift column left to the column right to disallow drawing text on Bitmap column headers
		rcText.left += XTP_DPI_X(2);
	}

	rcText.DeflateRect(XTP_DPI_X(3), 0, XTP_DPI_X(3), 0);

	if (rcText.right > rcText.left)
	{
		UINT uFlags = DT_END_ELLIPSIS | DT_NOPREFIX;
		uFlags |= (pColumn->GetHeaderAlignment() & DT_WORDBREAK) ? DT_WORDBREAK | DT_WORD_ELLIPSIS
																 : DT_SINGLELINE | DT_VCENTER;
		uFlags |= DT_CALCRECT;

		pDC->DrawText(strCaption, &rcText, uFlags);

		return rcText.Height() + XTP_DPI_Y(8);
	}

	return XTP_DPI_Y(8);
}

void CXTPGridPaintManager::FillGroupByControl(CDC* pDC, CRect& rcGroupBy)
{
	pDC->FillSolidRect(rcGroupBy, m_clrGroupBoxBack);
}

void CXTPGridPaintManager::DrawConnector(CDC* pDC, CPoint ptFrom, CPoint ptTo)
{
	DrawVerticalLine(pDC, ptFrom.x, ptFrom.y, ptTo.y - ptFrom.y, m_clrConnector);
	DrawHorizontalLine(pDC, ptFrom.x, ptTo.y, ptTo.x - ptFrom.x, m_clrConnector);
}

void CXTPGridPaintManager::DrawFocusedRow(CDC* pDC, CRect rcRow)
{
	if (m_pControl && ((CXTPGridControl*)m_pControl)->IsIconView())
		rcRow.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

	COLORREF clrTextColor = pDC->SetTextColor(m_clrControlBack);
	COLORREF clrBkColor   = pDC->SetBkColor(m_clrWindowText);
	pDC->DrawFocusRect(rcRow);
	pDC->SetTextColor(clrTextColor);
	pDC->SetBkColor(clrBkColor);
}

int CXTPGridPaintManager::GetPreviewLinesCount(CDC* pDC, CRect& rcText, const CString& strText)
{
	if (rcText.Width() < XTP_DPI_Y(1))
		return 0;

	if (pDC->GetTextExtent(strText).cx / rcText.Width() > m_nMaxPreviewLines)
		return m_nMaxPreviewLines;

	CRect rcCalc(rcText.left, 0, rcText.right, 0);
	pDC->DrawText(strText, &rcCalc, DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);

	return min(rcCalc.Height() / pDC->GetTextExtent(_T(" "), 1).cy, m_nMaxPreviewLines);
}

void CXTPGridPaintManager::SetTextFont(LOGFONT& lf)
{
	m_xtpFontText.DeleteObject();
	m_xtpFontBoldText.DeleteObject();

	// Normal text
	m_xtpFontText.CreateFontIndirect(&lf);

	// Bold text
	LOGFONT lfBold  = lf;
	lfBold.lfWeight = FW_BOLD;
	m_xtpFontBoldText.CreateFontIndirect(&lfBold);

	CWindowDC dc(NULL);
	CXTPFontDC font(&dc, &m_xtpFontText);

	int nFontHeight = dc.GetTextExtent(_T(" "), 1).cy;
	m_nRowHeight	= max(m_nRowHeightDefault, nFontHeight + XTP_DPI_Y(4));
}

void CXTPGridPaintManager::SetCaptionFont(LOGFONT& lf)
{
	m_xtpFontCaption.DeleteObject();
	VERIFY(m_xtpFontCaption.CreateFontIndirect(&lf));

	CWindowDC dc(NULL);
	CXTPFontDC font(&dc, &m_xtpFontCaption);

	font.SetFont(&m_xtpFontCaption);
	int nFontHeight = dc.GetTextExtent(_T(" "), 1).cy;
	m_nHeaderHeight = max(XTP_DPI_Y(22), nFontHeight + XTP_DPI_Y(8));
	m_nFooterHeight = m_nHeaderHeight;
}

int CXTPGridPaintManager::GetFooterHeight(CXTPGridControl* pControl, CDC* pDC, int nTotalWidth)
{
	ASSERT(pControl && pDC);

	if (!pControl || !pDC)
	{
		return m_nFooterHeight;
	}

	CXTPGridColumns* pColumns = pControl->GetColumns();
	int nColumnCount		  = pColumns->GetCount();
	int nHeight				  = XTP_DPI_Y(22);

	CXTPFontDC font(pDC);

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			CString strCaption = pColumn->GetFooterText();

			int nWidth = (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG)
							 ?
							 // int nWidth = (pDC->IsPrinting()) ?
							 pColumn->GetPrintWidth(nTotalWidth)
							 : pColumn->GetWidth();
			CRect rcText(0, 0, nWidth, XTP_DPI_Y(22));
			rcText.DeflateRect(XTP_DPI_X(3), 0, XTP_DPI_X(3), 0);

			UINT uFlags = DT_CALCRECT | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX;
			if (!m_bFixedRowHeight && (pColumn->GetFooterAlignment() & DT_WORDBREAK))
				// uFlags |= DT_WORDBREAK;
				uFlags |= DT_WORDBREAK | DT_WORD_ELLIPSIS;
			else
				uFlags |= DT_SINGLELINE;

			font.SetFont(pColumn->GetFooterFont());
			pDC->DrawText(strCaption, &rcText, uFlags);

			nHeight = max(nHeight, rcText.Height() + XTP_DPI_Y(8));
		}
	}
	return nHeight;
}

int CXTPGridPaintManager::GetHeaderHeight(CXTPGridControl* pControl, CDC* pDC, int nTotalWidth)
{
	ASSERT(pControl && pDC);

	if (m_bFixedRowHeight || !pControl || !pDC)
	{
		return GetHeaderHeight();
	}

	CXTPGridColumns* pColumns = pControl->GetColumns();
	int nColumnCount		  = pColumns->GetCount();
	int nHeight				  = m_nHeaderHeight;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			if (pColumn->GetHeaderAlignment() & DT_WORDBREAK)
			{
				int nColHeight = CalcColumnHeight(pDC, pColumn, nTotalWidth);

				nHeight = max(nHeight, nColHeight);
			}

			// calculate icon height.
			int nIconID = pColumn->GetIconID();
			if (nIconID != XTP_GRID_NOICON)
			{
				CXTPImageManagerIcon* pIcon =
					pColumn->GetControl()->GetImageManager()->GetImage(XTPToUInt(nIconID), 0);

				if (pIcon)
					nHeight = max(nHeight, pIcon->GetHeight() + XTP_DPI_Y(2));
			}
		}
	}

	return nHeight;
}

void CXTPGridPaintManager::SetHeaderHeight(int nHeight)
{
	if (nHeight < 0)
		nHeight = 0;

	m_nHeaderHeight = nHeight;
}

void CXTPGridPaintManager::SetPreviewIndent(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcPreviewIndent.SetRect(nLeft, nTop, nRight, nBottom);
}

BOOL CXTPGridPaintManager::DrawInplaceComboButton(CDC* pDC, CRect* pRc)
{
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon) && m_bShowNonActiveInPlaceButton)
	{
		CRect rect(*pRc);

		if (m_bThemedInplaceButtons && m_themeCombo->IsAppThemeActive())
		{
#ifdef _XTP_INCLUDE_CONTROLS
			if (m_bUseThemedControls)
			{
				ASSERT(m_pCombo);
				m_pCombo->GetTheme()->DrawButton(pDC, m_pCombo, pRc);
				m_pCombo->GetTheme()->DrawButtonTriangle(pDC, m_pCombo, pRc);
			}
			else
			{
#endif
				m_themeCombo->DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON, CBXS_NORMAL, pRc, NULL);
#ifdef _XTP_INCLUDE_CONTROLS
			}
#endif
		}
		else
		{
			CRect rcFrame(rect);
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_3DFACE),
							GetXtremeColor(COLOR_3DDKSHADOW));
			rcFrame.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));

			CXTPPenDC pen(*pDC, GetXtremeColor(COLOR_BTNTEXT));
			CXTPBrushDC brush(*pDC, GetXtremeColor(COLOR_BTNTEXT));

			CPoint pt = rect.CenterPoint();

			CPoint pts[3];
			pts[0] = CPoint(pt.x - XTP_DPI_X(3), pt.y - XTP_DPI_Y(1));
			pts[1] = CPoint(pt.x + XTP_DPI_X(3), pt.y - XTP_DPI_Y(1));
			pts[2] = CPoint(pt.x, pt.y + XTP_DPI_Y(2));
			pDC->Polygon(pts, 3);
		}

		return TRUE;
	}
	return FALSE;
}

BOOL CXTPGridPaintManager::DrawInplaceExpandButton(CDC* pDC, CRect* pRc)
{
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon) && m_bShowNonActiveInPlaceButton)
	{
		CRect rect(*pRc);
		pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

		if (m_bThemedInplaceButtons && m_themeButton->IsAppThemeActive())
		{
			m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON, PBS_NORMAL, pRc, NULL);
		}
		else
		{
			CRect rcFrame(rect);
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_3DFACE),
							GetXtremeColor(COLOR_3DDKSHADOW));
			rcFrame.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
		}
		pDC->Rectangle(rect.left + XTP_DPI_X(3), rect.bottom - XTP_DPI_Y(5),
					   rect.left + XTP_DPI_X(5), rect.bottom - XTP_DPI_Y(3));
		pDC->Rectangle(rect.left + XTP_DPI_X(7), rect.bottom - XTP_DPI_Y(5),
					   rect.left + XTP_DPI_X(9), rect.bottom - XTP_DPI_Y(3));
		pDC->Rectangle(rect.left + XTP_DPI_X(11), rect.bottom - XTP_DPI_Y(5),
					   rect.left + XTP_DPI_X(13), rect.bottom - XTP_DPI_Y(3));

		return TRUE;
	}
	return FALSE;
}

BOOL CXTPGridPaintManager::DrawInplaceSpinButton(CDC* pDC, CRect* pRc)
{
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon) && m_bShowNonActiveInPlaceButton
		&& m_bThemedInplaceButtons && m_themeSpin->IsAppThemeActive())
	{
		int nHeight = pRc->Height() / 2;
		pRc->bottom -= nHeight;
		m_themeSpin->DrawThemeBackground(*pDC, SPNP_UP, UPS_NORMAL, pRc, NULL);
		pRc->OffsetRect(0, nHeight);
		m_themeSpin->DrawThemeBackground(*pDC, SPNP_DOWN, UPS_NORMAL, pRc, NULL);
		return TRUE;
	}
	return FALSE;
}

void CXTPGridPaintManager::DrawInplaceButton(CDC* pDC, CXTPGridInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);
	BOOL bButtonIconDrawn = FALSE;

	if (m_bThemedInplaceButtons)
	{
		if (pButton->GetID() == XTP_ID_GRID_COMBOBUTTON && m_themeCombo->IsAppThemeActive())
		{
#ifdef _XTP_INCLUDE_CONTROLS
			if (m_bUseThemedControls)
			{
				ASSERT(m_pCombo);
				m_pCombo->SetHighlighted(pButton->IsPressed());

				m_pCombo->GetTheme()->DrawButton(pDC, m_pCombo, rect);
				m_pCombo->GetTheme()->DrawButtonTriangle(pDC, m_pCombo, rect);
			}
			else
			{
#endif
				m_themeCombo->DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON,
												  pButton->IsPressed() ? CBXS_PRESSED : CBXS_NORMAL,
												  rect, NULL);
#ifdef _XTP_INCLUDE_CONTROLS
			}
#endif
			return;
		}

		if (pButton->GetID() == XTP_ID_GRID_EXPANDBUTTON && m_themeButton->IsAppThemeActive())
		{
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

			m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON,
											   pButton->IsPressed()
												   ? PBS_PRESSED
												   : CWnd::GetFocus() == pButton ? PBS_DEFAULTED
																				 : PBS_NORMAL,
											   rect, 0);

			if (!DrawInplaceButtonIcon(pDC, pButton))
			{
				// draw dots
				pDC->Rectangle(rect.left + XTP_DPI_X(3), rect.bottom - XTP_DPI_Y(5),
							   rect.left + XTP_DPI_X(5), rect.bottom - XTP_DPI_Y(3));
				pDC->Rectangle(rect.left + XTP_DPI_X(7), rect.bottom - XTP_DPI_Y(5),
							   rect.left + XTP_DPI_X(9), rect.bottom - XTP_DPI_Y(3));
				pDC->Rectangle(rect.left + XTP_DPI_X(11), rect.bottom - XTP_DPI_Y(5),
							   rect.left + XTP_DPI_X(13), rect.bottom - XTP_DPI_Y(3));
			}
			return;
		}

		if (pButton->GetID() == XTP_ID_GRID_SPINBUTTON && m_themeSpin->IsAppThemeActive())
		{
			int nHeight = rect.Height() / 2;
			rect.bottom -= nHeight;
			m_themeSpin->DrawThemeBackground(*pDC, SPNP_UP,
											 pButton->GetState() == SPNP_UP ? UPS_PRESSED
																			: UPS_NORMAL,
											 rect, 0);
			rect.OffsetRect(0, nHeight);
			m_themeSpin->DrawThemeBackground(*pDC, SPNP_DOWN,
											 pButton->GetState() == SPNP_DOWN ? UPS_PRESSED
																			  : UPS_NORMAL,
											 rect, 0);
			return;
		}
	}
	if (pButton->GetID() == XTP_ID_GRID_COMBOBUTTON)
	{
		DrawInplaceButtonFrame(pDC, pButton);
	}
	else if (pButton->GetID() == XTP_ID_GRID_EXPANDBUTTON)
	{
		DrawInplaceButtonFrame(pDC, pButton);
		bButtonIconDrawn = DrawInplaceButtonIcon(pDC, pButton);
	}
	else if (pButton->GetID() == XTP_ID_GRID_SPINBUTTON)
	{
		CXTPClientRect rcClient((CWnd*)pButton);
		int nHeight = rcClient.Height() / 2;
		rcClient.bottom -= nHeight;
		for (int i = 0; i < 2; i++)
		{
			pDC->FillSolidRect(rcClient, GetXtremeColor(COLOR_3DFACE));

			if (i == 0 && pButton->GetState() == SPNP_UP
				|| i == 1 && pButton->GetState() == SPNP_DOWN)
			{
				pDC->Draw3dRect(rcClient, GetXtremeColor(COLOR_3DDKSHADOW),
								GetXtremeColor(COLOR_WINDOW));
			}
			else
			{
				pDC->Draw3dRect(rcClient, GetXtremeColor(COLOR_3DFACE),
								GetXtremeColor(COLOR_3DDKSHADOW));
				rcClient.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
				pDC->Draw3dRect(rcClient, GetXtremeColor(COLOR_WINDOW),
								GetXtremeColor(COLOR_3DSHADOW));
				rcClient.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			}
			rcClient.OffsetRect(0, nHeight);
		}
	}

	CXTPPenDC pen(*pDC, GetXtremeColor(COLOR_BTNTEXT));
	CXTPBrushDC brush(*pDC, GetXtremeColor(COLOR_BTNTEXT));

	if (pButton->GetID() == XTP_ID_GRID_COMBOBUTTON)
	{
		CPoint pt = rect.CenterPoint();

		CPoint pts[3];
		pts[0] = CPoint(pt.x - XTP_DPI_X(3), pt.y - XTP_DPI_Y(1));
		pts[1] = CPoint(pt.x + XTP_DPI_X(3), pt.y - XTP_DPI_Y(1));
		pts[2] = CPoint(pt.x, pt.y + XTP_DPI_Y(2));
		pDC->Polygon(pts, 3);
	}

	if (pButton->GetID() == XTP_ID_GRID_EXPANDBUTTON && !bButtonIconDrawn)
	{
		pDC->Rectangle(rect.left + XTP_DPI_X(3), rect.bottom - XTP_DPI_Y(5),
					   rect.left + XTP_DPI_X(5), rect.bottom - XTP_DPI_Y(3));
		pDC->Rectangle(rect.left + XTP_DPI_X(7), rect.bottom - XTP_DPI_Y(5),
					   rect.left + XTP_DPI_X(9), rect.bottom - XTP_DPI_Y(3));
		pDC->Rectangle(rect.left + XTP_DPI_X(11), rect.bottom - XTP_DPI_Y(5),
					   rect.left + XTP_DPI_X(13), rect.bottom - XTP_DPI_Y(3));
	}

	if (pButton->GetID() == XTP_ID_GRID_SPINBUTTON)
	{
		CPoint pt = rect.CenterPoint();
		pt.y -= rect.Height() / 4;

		CPoint pts[3];
		pts[0] = CPoint(pt.x - XTP_DPI_X(2), pt.y + XTP_DPI_Y(1));
		pts[1] = CPoint(pt.x + XTP_DPI_X(2), pt.y + XTP_DPI_Y(1));
		pts[2] = CPoint(pt.x, pt.y - XTP_DPI_Y(1));
		pDC->Polygon(pts, 3);

		pt.y += rect.Height() / 2;
		pts[0] = CPoint(pt.x - XTP_DPI_X(2), pt.y - XTP_DPI_Y(1));
		pts[1] = CPoint(pt.x + XTP_DPI_X(2), pt.y - XTP_DPI_Y(1));
		pts[2] = CPoint(pt.x, pt.y + XTP_DPI_Y(1));
		pDC->Polygon(pts, 3);
	}
}

void CXTPGridPaintManager::DrawInplaceButtonFrame(CDC* pDC, CXTPGridInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);

	pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

	if (pButton->IsPressed())
	{
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_WINDOW));
	}
	else
	{
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
		rect.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
	}
}

BOOL CXTPGridPaintManager::DrawInplaceButtonIcon(CDC* pDC, CXTPGridInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);

	int nIconIndex = pButton->GetIconIndex();
	if (nIconIndex == XTP_GRID_NOICON)
		return FALSE;

	CXTPImageManagerIcon* pIcon =
		pButton->pControl->GetImageManager()->GetImage(XTPToUInt(nIconIndex), 0);
	if (!pIcon)
		return FALSE;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CPoint ptIcon(max(rect.left, rect.left + (rect.Width() - szImage.cx) / 2),
				  max(rect.top, rect.top + (rect.Height() - szImage.cy) / 2));
	szImage.cx = min(szImage.cx, rect.Width());
	szImage.cy = min(szImage.cy, rect.Height());
	pIcon->Draw(pDC, ptIcon, xtpImageNormal, szImage);

	return TRUE;
}

void CXTPGridPaintManager::DrawTreeStructureLine(CDC* pDC, int x, int y, int cx, int cy,
												 COLORREF clr)
{
	if (m_treeStructureStyle == xtpGridTreeStructureDots)
	{
		LOGBRUSH lbr = { 0 };
		CBrush brush;
		if (NULL != (HBRUSH)m_xtpBrushTreeStructure)
		{
			m_xtpBrushTreeStructure.GetLogBrush(&lbr);
			brush.CreateBrushIndirect(&lbr);
		}

		const unsigned short cb[] = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };
		DrawGridPattern(pDC, &brush, CRect(x, y, x + cx, y + cy), cb, m_clrControlDark);

		if (NULL == (HBRUSH)m_xtpBrushTreeStructure && NULL != (HBRUSH)brush)
		{
			brush.GetLogBrush(&lbr);
			m_xtpBrushTreeStructure.CreateBrushIndirect(&lbr);
		}
	}
	else
	{
		pDC->FillSolidRect(x, y, cx, cy, clr);
	}
}

void CXTPGridPaintManager::DrawTreeStructure(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
											 XTP_GRIDRECORDITEM_METRICS* pMetrics, CRect rcItem,
											 CSize sizeGlyph)
{
	if (m_treeStructureStyle == xtpGridTreeStructureNone)
		return;

	CDC* pDC				  = pDrawArgs->pDC;
	CXTPGridRow* pRow		  = pDrawArgs->pRow;
	COLORREF clrTreeStructure = pMetrics->clrForeground;

	if (pDrawArgs->pControl->IsVirtualMode())
	{
		if (pMetrics->nVirtRowLevel == 0)
			return;
	}
	else
	{
		if (pRow->IsGroupRow()
			|| (m_nTreeStructureStyle == 0
				&& (!pRow->GetParentRow() || pRow->GetParentRow()->IsGroupRow())))
			return;
	}

	int nTreeIndent = m_nTreeIndent;
	int nHeight		= rcItem.Height();

	LONG lHorLineOffset = nHeight / 2;
	int nIconAlign		= pDrawArgs->nTextAlign & xtpColumnIconMask;
	switch (nIconAlign)
	{
		case xtpColumnIconTop: lHorLineOffset = sizeGlyph.cy / 2; break;
		case xtpColumnIconBottom:
			lHorLineOffset = rcItem.bottom - rcItem.top - XTP_DPI_Y(2) - sizeGlyph.cy / 2;
			break;
	}

	if (m_nTreeStructureStyle > 0)
		rcItem.OffsetRect((sizeGlyph.cy + XTP_DPI_Y(1)) / 2 + XTP_DPI_Y(2), 0);

	if (pDrawArgs->pControl->IsVirtualMode())
	{
		BOOL bLastChild = (pMetrics->nVirtRowFlags & xtpVirtRowLastChild) > 0;
		rcItem.OffsetRect((sizeGlyph.cy + XTP_DPI_Y(1)) / 2 + XTP_DPI_Y(2), 0); // extra shift
		int nVirtIndent = rcItem.left;
		int nLevel		= pMetrics->nVirtRowLevel;

		if (nLevel == 1)
		{
			DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - XTP_DPI_Y(1), XTP_DPI_X(1),
								  lHorLineOffset + XTP_DPI_Y(1), clrTreeStructure);
			DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top + lHorLineOffset,
								  sizeGlyph.cx / 2 + XTP_DPI_X(1), XTP_DPI_Y(1), clrTreeStructure);

			if (!bLastChild)
				DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - XTP_DPI_Y(1), XTP_DPI_X(1),
									  nHeight + XTP_DPI_Y(1), clrTreeStructure);
		}

		if (nLevel > 1)
		{
			while (nLevel > 1)
			{
				nVirtIndent -= nTreeIndent;
				if (nLevel % 2 == 1)
				{
					if (bLastChild)
					{
						DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - XTP_DPI_Y(1),
											  XTP_DPI_X(1), lHorLineOffset + XTP_DPI_Y(1),
											  clrTreeStructure);
						DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top + lHorLineOffset,
											  sizeGlyph.cx / 2 + XTP_DPI_X(1), XTP_DPI_Y(1),
											  clrTreeStructure);
						bLastChild = FALSE;
					}
					else
						DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - XTP_DPI_Y(1),
											  XTP_DPI_X(1), nHeight + XTP_DPI_Y(1),
											  clrTreeStructure);
				}
				nLevel /= 2;
			}
		}
	}
	else
	{
		CXTPGridRow* pNextRow = pRow->GetNextSiblingRow();
		if (pNextRow)
			DrawTreeStructureLine(pDC, rcItem.left, rcItem.top - XTP_DPI_Y(1), XTP_DPI_X(1),
								  nHeight + XTP_DPI_Y(1), clrTreeStructure);
		else
			DrawTreeStructureLine(pDC, rcItem.left, rcItem.top - XTP_DPI_Y(1), XTP_DPI_X(1),
								  lHorLineOffset + XTP_DPI_Y(1), clrTreeStructure);

		DrawTreeStructureLine(pDC, rcItem.left, rcItem.top + lHorLineOffset,
							  ((!pDrawArgs->pRow->HasChildren() && (m_nTreeColumnLineLength > 0))
								   ? m_nTreeColumnLineLength
								   : sizeGlyph.cx / 2 + XTP_DPI_X(1)),
							  XTP_DPI_Y(1), clrTreeStructure);

		for (;;)
		{
			pRow = pRow->GetParentRow();

			if (pRow == NULL || pRow->IsGroupRow()
				|| (m_nTreeStructureStyle == 0
					&& (pRow->GetParentRow() == 0 || pRow->GetParentRow()->IsGroupRow())))
				break;

			rcItem.OffsetRect(-nTreeIndent, 0);

			if (pRow->GetNextSiblingRow())
				DrawTreeStructureLine(pDC, rcItem.left, rcItem.top - XTP_DPI_Y(1), XTP_DPI_X(1),
									  rcItem.Height() + XTP_DPI_Y(1), clrTreeStructure);
		}
	}
}

int CXTPGridPaintManager::DrawString(int* pnCurrDrawPos, CDC* pDC, const CString& strDraw,
									 CRect rcDraw, UINT nFormat)
{
	int nCharCount = 0;

	switch (nFormat)
	{
		case DT_END_ELLIPSIS:
		{
			// in this case char count only for identify if string printed partially
			*pnCurrDrawPos += GetTextExtentX(pDC, strDraw);
			if (*pnCurrDrawPos >= rcDraw.right)
				nCharCount = -1;
			else
				nCharCount = strDraw.GetLength();

			pDC->DrawText(strDraw, &rcDraw, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
			break;
		}
		case DT_WORDBREAK:
		{
			const TCHAR seps[] = _T(" \t\n");
			TCHAR* lpszContext = 0;
			CString strOut	 = _T("");
			CString strSrc	 = strDraw;
			TCHAR* szWord	  = STRTOK_S(strSrc.GetBuffer(strSrc.GetLength()), seps, &lpszContext);
			int nRightStrMargin = GetTextExtentX(pDC, CString(szWord));

			if (nRightStrMargin > rcDraw.Width())
			{
				strOut = szWord;
			}
			else
			{
				while (szWord != NULL && nRightStrMargin < rcDraw.Width())
				{
					strOut			= strOut + szWord + _T(" ");
					szWord			= STRTOK_S(NULL, seps, &lpszContext);
					nRightStrMargin = GetTextExtentX(pDC, strOut + szWord);
				}
			}

			nCharCount = strOut.GetLength();
			*pnCurrDrawPos += GetTextExtentX(pDC, strOut);
			pDC->DrawText(strOut, &rcDraw, DT_LEFT | DT_NOPREFIX);
			strSrc.ReleaseBuffer();
			break;
		}
		case DT_LEFT:
		case DT_RIGHT:
		case DT_CENTER:
		{
			*pnCurrDrawPos += GetTextExtentX(pDC, strDraw);
			if (*pnCurrDrawPos >= rcDraw.right)
				nCharCount = -1;
			else
				nCharCount = strDraw.GetLength();

			pDC->DrawText(strDraw, &rcDraw, nFormat | DT_NOPREFIX);
			break;
		}

	} // switch

	return nCharCount;
}

void CXTPGridPaintManager::DrawTextLine(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
										XTP_GRIDRECORDITEM_METRICS* pMetrics,
										const CString& strText, CRect rcItem, int nFlag,
										int& nCharCounter, int& nHyperlinkCounter)
{
	CXTPGridControl* pControl = DYNAMIC_DOWNCAST(CXTPGridControl, pDrawArgs->pRow->GetControl());

	if (!pControl || !pControl->IsIconView())
	{
		CDC* pDC				  = pDrawArgs->pDC;
		CXTPGridRecordItem* pItem = pDrawArgs->pItem;
		UINT nAlingFlag			  = 0;
		int nHyperlink			  = nHyperlinkCounter; // Hyperlink index
		int nCharCount			  = nCharCounter;	  // Number of actually printed chars
		CXTPGridHyperlink* pHl	= NULL;
		int nHyperlinks			  = pItem->GetHyperlinksCount();

		int nCurrStrPos = nCharCount;
		CRect rcText;
		rcText.CopyRect(&rcItem);

		if (nHyperlinks == 0)
		{
			pDC->DrawText(strText, &rcText,
						  UINT(pDrawArgs->nTextAlign | nFlag | DT_NOPREFIX | DT_SINGLELINE));
			return;
		}

		// calculate rect for drawing text for text alignment feature
		pDC->DrawText(strText, &rcText, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);

		if (rcText.Width() < rcItem.Width())
		{
			switch (pDrawArgs->nTextAlign & (DT_RIGHT | DT_CENTER))
			{
				case DT_RIGHT: rcText.left = rcItem.right - rcText.Width(); break;
				case DT_CENTER:
					rcText.left = rcItem.left + (rcItem.Width() - rcText.Width()) / 2;
					break;
			}
		}
		rcText.right = rcItem.right;

		int nCurrDrawPos = rcText.left;

		while (strText.GetLength() > nCurrStrPos && nCurrDrawPos < rcText.right)
		{
			CString strOut;
			if (nHyperlinks > nHyperlink)
			{
				pHl	= pItem->GetHyperlinkAt(nHyperlink);
				strOut = strText.Mid(nCurrStrPos, pHl->m_nHyperTextBegin - nCurrStrPos);
				if (strOut.GetLength() > 0) // print str
				{
					// replace LF, CR characters (if any)
					strOut.Remove(_T('\n'));
					strOut.Remove(_T('\r'));

					nCharCount  = DrawString(&nCurrDrawPos, pDC, strOut, rcText, XTPToUInt(nFlag));
					rcText.left = nCurrDrawPos > rcText.right ? rcText.right : nCurrDrawPos;

					// if previous string was cut - stop draw
					if (nCharCount < strOut.GetLength())
					{
						nCharCount += nCurrStrPos;
						break;
					}
				}

				// print link
				nCharCount  = DrawLink(&nCurrDrawPos, pDrawArgs, pMetrics, pHl, strText, rcText,
									   nFlag);
				rcText.left = nCurrDrawPos > rcText.right ? rcText.right : nCurrDrawPos;

				// update current position in string
				nCurrStrPos = pHl->m_nHyperTextBegin + pHl->m_nHyperTextLen;
				nHyperlink++;

				// if previous string was cut - stop draw
				if (nCharCount < (pHl->m_nHyperTextLen))
				{
					nCharCount += nCurrStrPos;
					break;
				}
			}
			else
			{
				// print whole string without links or tail of string
				strOut	 = strText.Mid(nCurrStrPos, strText.GetLength() - nCurrStrPos);
				nCharCount = DrawString(&nCurrDrawPos, pDC, strOut, rcText, XTPToUInt(nFlag));
				nCharCount += nCurrStrPos;
				break; // all was drawn!
			}
		}

		pDC->SetTextAlign(nAlingFlag);
		nCharCounter = nCharCount;

		return;
	}
	else
	{
		CXTPGridPaintManager::DrawMultiLineText(pDrawArgs, pMetrics, strText, rcItem);
	}
}

int CXTPGridPaintManager::GetPreviewItemHeight(CDC* pDC, CXTPGridRow* pRow, int nWidth, int nHeight)
{
#ifdef _XTP_ACTIVEX
	if (m_pControl->m_bCustomMeasurePreviewItem)
	{
		m_pControl->FireMeasurePreviewItem(pRow->GetIDispatch(FALSE),
										   (OLE_HANDLE) reinterpret_cast<LONG_PTR>(
											   pDC->GetSafeHdc()),
										   nWidth, &nHeight);

		_variant_t vtHeight = (long)nHeight;
		m_pControl->FireMeasurePreviewItemV(pRow->GetIDispatch(FALSE),
											(OLE_HANDLE) reinterpret_cast<LONG_PTR>(
												pDC->GetSafeHdc()),
											nWidth, &vtHeight);
		nHeight = (long)vtHeight;
	}
#else
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pRow);
	UNREFERENCED_PARAMETER(nWidth);
#endif
	return nHeight;
}

void CXTPGridPaintManager::DrawItemCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
										   XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
#ifdef _XTP_ACTIVEX
	if (!OnDrawAction(pDrawArgs))
		return;
#endif

	CString strText = pMetrics->strText;
	CRect& rcItem   = pDrawArgs->rcItem;
	rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0);

	// draw item text
	if (!strText.IsEmpty())
	{
		CXTPFontDC font(pDrawArgs->pDC, pMetrics->pFont);
		if (!m_bFixedRowHeight) // && (pDrawArgs->nTextAlign & DT_WORDBREAK))
		{
			BOOL bWordBreak = !!(pDrawArgs->nTextAlign & DT_WORDBREAK);

			if (pDrawArgs->nTextAlign & DT_VCENTER)
			{
				// try to center vertically because DT_VCENTER works only for DT_SINGLELINE;

				ReplaceInHyperLinks(pDrawArgs->pItem, strText, XTP_HLINK_SPACE_X);

				int nHeightRequired = CalculateRowHeight(pDrawArgs->pDC, strText, rcItem.Width(),
														 bWordBreak);

				int nHeightDiff = rcItem.Height() - nHeightRequired;

				if (nHeightDiff >= XTP_DPI_Y(4))
					rcItem.top += nHeightDiff / 2 - XTP_DPI_Y(2);

				strText = pMetrics->strText;
			}

			DrawMultiLineText(pDrawArgs, pMetrics, strText, rcItem, bWordBreak);
		}
		else
		{
			int nCharCount = 0;
			int nHyperlink = 0;
			DrawTextLine(pDrawArgs, pMetrics, strText, &rcItem, DT_END_ELLIPSIS, nCharCount,
						 nHyperlink);
		}
	}
}

void CXTPGridPaintManager::DrawItemControl(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
										   CXTPGridRecordItemControl* pControl, CRect& rcItem)
{
	if (!pControl)
		return;

	CFont* pFont = pControl->GetFont();
	if (!pFont || !pFont->GetSafeHandle())
		pFont = &m_xtpFontText;

	CXTPFontDC dc(pDrawArgs->pDC, pControl->GetFont(), pControl->GetCaptionColor());
	CRect rcControl = rcItem;
	// calculate control's rectangle
	// horizontal
	int nControlWidth = pControl->GetSize().cx;
	if (nControlWidth > 0)
	{
		if (pControl->GetAlignment() == xtpItemControlRight)
		{
			rcControl.left = rcControl.right - nControlWidth;
			rcItem.right -= nControlWidth;
		}
		else
		{
			rcControl.right = rcControl.left + nControlWidth;
			rcItem.left += nControlWidth;
		}
	}
	else
	{
		// return;
		if (pControl->GetAlignment() == xtpItemControlRight)
		{
			rcControl.left = rcControl.right;
		}
		else
		{
			rcControl.right = rcControl.left;
		}
	}

	int nControlHeight = pControl->GetSize().cy;
	if (nControlHeight > 0 && nControlHeight < rcItem.Height())
	{
		rcControl.top	= rcItem.top + (rcItem.Height() - nControlHeight) / 2;
		rcControl.bottom = rcControl.top + nControlHeight;
	}

	// handle alignment
	// set control's rectangle
	pControl->SetRect(rcControl);
	switch (pControl->GetType())
	{
		case xtpItemControlTypeButton:
		{
			// draw button
			CXTPGridRecordItemButton* pButton = DYNAMIC_DOWNCAST(CXTPGridRecordItemButton,
																 pControl);
			ASSERT(pButton);
			int nState = pButton->GetEnable() ? pButton->GetState() : PBS_DISABLED;

			if (m_bThemedInplaceButtons || pButton->GetThemed())
			{
				m_themeButton->DrawThemeBackground(*pDrawArgs->pDC, BP_PUSHBUTTON, nState,
												   rcControl, 0);
			}
			else
			{
				pDrawArgs->pDC->FillSolidRect(rcControl, GetXtremeColor(COLOR_3DFACE));

				if (nState == PBS_PRESSED)
				{
					pDrawArgs->pDC->Draw3dRect(rcControl, GetXtremeColor(COLOR_3DDKSHADOW),
											   GetXtremeColor(COLOR_WINDOW));
				}
				else
				{
					pDrawArgs->pDC->Draw3dRect(rcControl, GetXtremeColor(COLOR_3DFACE),
											   GetXtremeColor(COLOR_3DDKSHADOW));
					rcControl.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
					pDrawArgs->pDC->Draw3dRect(rcControl, GetXtremeColor(COLOR_WINDOW),
											   GetXtremeColor(COLOR_3DSHADOW));
				}
			}
			// calculate rectangle for drawing icon or caption
			if (nState == PBS_PRESSED)
				rcControl.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(1), XTP_DPI_Y(1));
			else
				rcControl.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			// draw button icon
			int nIconIndex = pButton->GetIconIndex(nState);
			if (nIconIndex != XTP_GRID_NOICON)
			{
				CXTPImageManagerIcon* pIcon =
					pDrawArgs->pControl->GetImageManager()->GetImage(XTPToUInt(nIconIndex), 0);
				if (pIcon)
				{
					CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
					CPoint ptIcon(max(rcControl.left,
									  rcControl.left + (rcControl.Width() - szImage.cx) / 2),
								  max(rcControl.top,
									  rcControl.top + (rcControl.Height() - szImage.cy) / 2));
					szImage.cx = min(szImage.cx, rcControl.Width());
					szImage.cy = min(szImage.cy, rcControl.Height());
					if (pDrawArgs->pDC->IsPrinting())
					{
						CBitmap bmp;
						bmp.CreateCompatibleBitmap(pDrawArgs->pDC, szImage.cx, szImage.cy);
						CXTPCompatibleDC dcMem(NULL, &bmp);
						dcMem.FillSolidRect(0, 0, szImage.cx, szImage.cy,
											GetXtremeColor(COLOR_3DFACE));
						pIcon->Draw(&dcMem, CPoint(0, 0), xtpImageNormal);
						pDrawArgs->pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem,
											   0, 0, SRCCOPY);
					}
					else
						pIcon->Draw(pDrawArgs->pDC, ptIcon, xtpImageNormal, szImage);
				}
			}
			// draw button caption
			if (!pButton->GetCaption().IsEmpty())
			{
				COLORREF clrPrev = pDrawArgs->pDC->SetTextColor(
					pButton->GetEnable() ? pButton->GetCaptionColor()
										 : GetXtremeColor(XPCOLOR_GRAYTEXT));
				// Issue 20355 - fix
				int iAl = pButton->GetAlignment();
				if (iAl == xtpItemControlLeft)
					pDrawArgs->pDC->DrawText(pButton->GetCaption(), &rcControl,
											 DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
				else if (iAl == xtpItemControlRight)
					pDrawArgs->pDC->DrawText(pButton->GetCaption(), &rcControl,
											 DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
				else
					pDrawArgs->pDC->DrawText(pButton->GetCaption(), &rcControl,
											 DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

				pDrawArgs->pDC->SetTextColor(clrPrev);
			}
			break;
		}
	}
}

void CXTPGridPaintManager::ReplaceInHyperLinks(CXTPGridRecordItem* pItem, CString& rstrText,
											   TCHAR chReplace)
{
	ASSERT(pItem);
	if (!pItem)
		return;

	int nHyperlinks = pItem->GetHyperlinksCount();
	for (int nHLink = 0; nHLink < nHyperlinks; nHLink++)
	{
		CXTPGridHyperlink* pHLink = pItem->GetHyperlinkAt(nHLink);
		ASSERT(pHLink);
		if (!pHLink)
			continue;

		int nI0 = max(0, min(pHLink->m_nHyperTextBegin, rstrText.GetLength() - 1));
		int nI1 = max(0, min(pHLink->m_nHyperTextBegin + pHLink->m_nHyperTextLen,
							 rstrText.GetLength()));
		for (int i = nI0; i < nI1; i++)
		{
			if (rstrText.GetAt(i) == _T(' '))
			{
				rstrText.SetAt(i, chReplace);
			}
		}
	}
}

void CXTPGridPaintManager::DrawMultiLineText(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
											 XTP_GRIDRECORDITEM_METRICS* pMetrics,
											 const CString& strText, CRect rcItem, BOOL bWordBreak)
{
	CDC* pDC				  = pDrawArgs->pDC;
	CXTPGridRecordItem* pItem = pDrawArgs->pItem;

	int nCurrStrPos = 0, nStringWidth = 0, nWordWidth = 0;
	int nStartString = -1, nEndString = -1;

	CRect rcText;
	rcText.CopyRect(&rcItem);

	if (0 == pItem->GetHyperlinksCount()) // no hyperlinks
	{
		if (strText.GetLength() > 0)
		{
			if (bWordBreak)
				pDC->DrawText(strText, &rcText,
							  UINT(DT_NOPREFIX | pDrawArgs->nTextAlign | DT_WORD_ELLIPSIS
								   | DT_WORDBREAK));
			else
				pDC->DrawText(strText, &rcText,
							  UINT(DT_NOPREFIX | pDrawArgs->nTextAlign | DT_WORD_ELLIPSIS));
		}
		return;
	}

	int nLineHeight = pDC->GetTextExtent(_T(" "), 1).cy;
	CString strSrc  = strText + _T(" ");
	strSrc.Replace(_T('\r'), _T(' '));

	ReplaceInHyperLinks(pItem, strSrc, XTP_HLINK_SPACE_X);

	CString strWord, strString;
	int nTextLength = strSrc.GetLength();

	TCHAR* lpszBuff = strSrc.GetBuffer(nTextLength + 1);
	TCHAR cChar		= lpszBuff[nCurrStrPos];

	int nWordLength = 0;
	TCHAR* lpszWord = strWord.GetBuffer(nTextLength + 1);

	// parsing for the words (tokens)
	while (_T('\0') != (cChar = lpszBuff[nCurrStrPos++]))
	{
		if (cChar == _T(' ') || cChar == _T('\t') || cChar == _T('\n'))
		{
			if (lpszWord[0] == _T('\0') && cChar != _T('\n'))
				continue;

			if (strString.IsEmpty())
			{
				nStartString = nCurrStrPos - nWordLength;
				nEndString   = nCurrStrPos;
			}
			nWordWidth = GetTextExtentX(pDC, lpszWord, nWordLength);

			if (nWordWidth > rcItem.Width() && bWordBreak || cChar == _T('\n'))
			{
				if (!strString.IsEmpty())
				{
					DrawSingleLineText(pDrawArgs, pMetrics, strSrc, rcText, nStartString - 1,
									   nEndString - 1, GetTextExtentX(pDC, strString));

					rcText.top += nLineHeight;
					rcText.left = rcItem.left;
				}

				// the word exceeds the cell's width
				DrawSingleLineText(pDrawArgs, pMetrics, strSrc, rcText,
								   nCurrStrPos - nWordLength - 1, nCurrStrPos - 1, nWordWidth);

				if ((nCurrStrPos - nWordLength) != nCurrStrPos)
				{
					rcText.top += nLineHeight;
					rcText.left = rcItem.left;
				}

				strString.Empty();

				lpszWord[0] = _T('\0');
				nWordLength = 0;
				continue;
			}

			int nPrevStringWidth = GetTextExtentX(pDC, strString);

			if (!strString.IsEmpty())
				strString += _T(" ");

			strString += lpszWord;
			nStringWidth = GetTextExtentX(pDC, strString);

			if (nStringWidth > rcItem.Width() && bWordBreak)
			{
				DrawSingleLineText(pDrawArgs, pMetrics, strSrc, rcText, nStartString - 1,
								   nEndString - 1, nPrevStringWidth);

				rcText.top += nLineHeight;
				rcText.left = rcItem.left;

				nStartString = nCurrStrPos - nWordLength;
				strString	= lpszWord;
			}

			nEndString  = nCurrStrPos;
			lpszWord[0] = _T('\0');
			nWordLength = 0;
			continue;
		}

		ASSERT(nWordLength <= nTextLength);
		lpszWord[nWordLength]	 = cChar;
		lpszWord[nWordLength + 1] = _T('\0');

		++nWordLength;
	}

	// the rest of the string
	if (strString.GetLength())
	{
		DrawSingleLineText(pDrawArgs, pMetrics, strSrc, rcText, nStartString - 1,
						   strText.GetLength(), GetTextExtentX(pDC, strString));
	}

	strSrc.ReleaseBuffer();
	strWord.ReleaseBuffer();
}

void CXTPGridPaintManager::DrawSingleLineText(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
											  XTP_GRIDRECORDITEM_METRICS* pMetrics,
											  const CString& strText, CRect rcItem, int nStartPos,
											  int nEndPos, int nActualWidth)
{
	CDC* pDC				  = pDrawArgs->pDC;
	CXTPGridRecordItem* pItem = pDrawArgs->pItem;
	CXTPGridHyperlink* pHl	= NULL;

	// alignment
	CRect rcText(rcItem);
	int nFlag = pDrawArgs->nTextAlign & (DT_LEFT | DT_RIGHT | DT_CENTER);

	if (nFlag & DT_RIGHT)
		rcText.left = rcText.right - nActualWidth;
	else if (nFlag & DT_CENTER)
		rcText.left += (rcText.Width() - nActualWidth) / 2;

	// Left align the string, that exceeds the cell's width.
	rcText.left = max(rcText.left, rcItem.left);

	int nCurrDrawPos	= rcText.left;
	int nHyperlink		= 0; // Hyperlink index
	int nCurrStrPos		= nStartPos;
	int nBeginHyperlink = 0xFFFF;
	int nHyperlinks		= pItem->GetHyperlinksCount();

	CString strWord, strPre, strPost, strSrc = strText + _T(" ");
	LPTSTR lpszBuff = strSrc.GetBuffer(strSrc.GetLength());
	TCHAR cChar		= lpszBuff[nCurrStrPos];
	strSrc.ReleaseBuffer();
	BOOL bNeedsBlank = FALSE;

	int nBlankWidth = GetTextExtentX(pDC, _T(" "), 1);

	while (nCurrStrPos <= nEndPos && _T('\0') != (cChar = lpszBuff[nCurrStrPos++]))
	{
		if (cChar == _T(' ') || cChar == _T('\t') || cChar == _T('\n'))
		{
			if (strWord.IsEmpty())
				continue;

			if (bNeedsBlank)
			{
				// between words
				pDC->DrawText(_T(" "), &rcText, DT_LEFT);
				rcText.left += nBlankWidth;
			}

			// as hyperlinks are not sorted in the array, check all
			for (nHyperlink = 0; nHyperlink < nHyperlinks; ++nHyperlink)
			{
				pHl				= pItem->GetHyperlinkAt(nHyperlink);
				nBeginHyperlink = pHl->m_nHyperTextBegin;

				// validate the hyperlink
				if (nBeginHyperlink >= 0 && nBeginHyperlink <= strText.GetLength() - 1
					&& pHl->m_nHyperTextLen > 1
					&& nBeginHyperlink + pHl->m_nHyperTextLen <= strText.GetLength())
				{
					// particular case: hyperlink begins with blank(s)
					BOOL bInclude = FALSE;
					if (nStartPos > nBeginHyperlink)
					{
						CString strExtracted = strText.Mid(nBeginHyperlink,
														   nStartPos - nBeginHyperlink);
						if (_tcsspnp(strExtracted.GetBuffer(strExtracted.GetLength()), _T(" "))
							== NULL)
							bInclude = TRUE;
						strExtracted.ReleaseBuffer();
					}

					// does the hyperlink belong to the string
					if (nBeginHyperlink >= nStartPos
							&& nBeginHyperlink + pHl->m_nHyperTextLen <= nEndPos
						|| bInclude)
					{
						// does the word contain the hyperlink
						CString strExtracted = strText.Mid(nBeginHyperlink, pHl->m_nHyperTextLen);
						strExtracted.TrimLeft();
						if (strWord.Find(strExtracted) != -1)
						{
							if (nBeginHyperlink < nCurrStrPos)
							{
								// before hyperlink
								strPre = strWord.Mid(0, strWord.GetLength()
															- (nCurrStrPos - nBeginHyperlink - 1));
								if (!strPre.IsEmpty())
								{
									pDC->DrawText(strPre, &rcText,
												  DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
									rcText.left += GetTextExtentX(pDC, strPre);
								}

								// the hyperlink
								nCurrDrawPos = rcText.left;
								DrawLink(&nCurrDrawPos, pDrawArgs, pMetrics, pHl, strText, rcText,
										 DT_LEFT | DT_END_ELLIPSIS, TRUE);
								rcText.left = nCurrDrawPos;

								// after hyperlink
								int n = strWord.GetLength()
										- (nCurrStrPos
										   - (nBeginHyperlink + pHl->m_nHyperTextLen + 1));
								if (n >= 0 && (strWord.GetLength() - n) > 0)
								{
									strPost = strWord.Mid(n, strWord.GetLength() - n);
									if (!strPost.IsEmpty())
									{
										pDC->DrawText(strPost, &rcText,
													  DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
										rcText.left += GetTextExtentX(pDC, strPost);
									}
								}

								strWord.Empty();
								bNeedsBlank = TRUE;
								continue;
							}
						}
					}
				}
			} // the hyperlink validation

			// non-hyperlink words
			pDC->DrawText(strWord, &rcText, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
			rcText.left += GetTextExtentX(pDC, strWord);

			strWord.Empty();
			bNeedsBlank = TRUE;
			continue;
		}

		strWord += cChar;
	}
}

int CXTPGridPaintManager::CalculateRowHeight(CDC* pDC, const CString& strText, int nMaxWidth,
											 BOOL bWordBreak)
{
	CString strWord, strBuff(strText);
	strBuff += _T(" ");

	int nLineHeight = pDC->GetTextExtent(_T(" "), 1).cy;

	// just calculate new line characters
	if (!bWordBreak)
	{
		int nCount = 0, nPos = -1;

		do
		{
			nCount++;
			nPos = strBuff.Find(_T('\n'), nPos + 1);
		} while (nPos != -1);

		return nLineHeight * nCount;
	}

	CRect rc(0, 0, nMaxWidth, nLineHeight);
	// int h = pDC->DrawText(strBuff, &rc, DT_CALCRECT | DT_NOPREFIX | DT_WORDBREAK);
	int h = pDC->DrawText(strBuff, &rc,
						  DT_CALCRECT | DT_NOPREFIX | DT_WORDBREAK | DT_WORD_ELLIPSIS);
	return max(nLineHeight, h);
}

CRect CXTPGridPaintManager::CalculateMaxTextRect(CDC* pDC, const CString& strText,
												 const CRect& rcMaxRect, BOOL bWordBreak,
												 BOOL bLimitSize, UINT uiFlags)
{
	CRect rc(rcMaxRect);
	pDC->DrawText(strText, rc,
				  DT_CALCRECT | DT_NOPREFIX | ((bWordBreak) ? DT_WORDBREAK : 0) | uiFlags);

	if (bLimitSize)
	{
		rc.bottom = rc.top + min(rc.Height(), rcMaxRect.Height());
		rc.right  = rc.left + min(rc.Width(), rcMaxRect.Width());
	}
	return rc;
}

int CXTPGridPaintManager::DrawLink(int* pnCurrDrawPos, XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_GRIDRECORDITEM_METRICS* pMetrics,
								   CXTPGridHyperlink* pHyperlink, CString strText, CRect rcLink,
								   int nFlag, BOOL bTrim)
{
	int nCount = pHyperlink->m_nHyperTextLen;

	int nDiff = pHyperlink->m_nHyperTextBegin + pHyperlink->m_nHyperTextLen - strText.GetLength();
	if (nDiff > 0)
		nCount = max(0, pHyperlink->m_nHyperTextLen - nDiff);

	if (pHyperlink->m_nHyperTextBegin < 0 || pHyperlink->m_nHyperTextBegin >= strText.GetLength()
		|| nCount <= 0)
		return 0;

	CDC* pDC	   = pDrawArgs->pDC;
	CString strOut = strText.Mid(pHyperlink->m_nHyperTextBegin, nCount);

	if (bTrim)
	{
		strOut.TrimLeft();
		strOut.TrimRight();
	}
	strOut.Replace(XTP_HLINK_SPACE_X, _T(' '));

	BOOL bSelected = pDrawArgs->pRow->IsSelected() && pDrawArgs->pRow->GetControl()->HasFocus()
					 && (!pDrawArgs->pColumn || !pDrawArgs->pRow->GetControl()->GetFocusedColumn()
						 || pDrawArgs->pColumn
								!= pDrawArgs->pRow->GetControl()->GetFocusedColumn());

	LOGFONT lfLink;
	pMetrics->pFont->GetLogFont(&lfLink);

	COLORREF crLink(RGB(0, 0, 255));

	if (bSelected)
	{
		crLink = m_clrHighlightText;
	}
	else
	{
		if (pDrawArgs->pControl->m_pHotHyperlink == pHyperlink)
		{
			if (xtpGridTextDecorationUnderline == m_pHyperlinkHoverStyle->GetTextDecoration())
			{
				lfLink.lfUnderline = TRUE;
			}
			crLink = m_pHyperlinkHoverStyle->GetColor();
		}
		else
		{
			if (xtpGridTextDecorationUnderline == m_pHyperlinkStyle->GetTextDecoration())
			{
				lfLink.lfUnderline = TRUE;
			}
			crLink = m_pHyperlinkStyle->GetColor();
		}
	}

	CXTPFont fontLink;
	fontLink.CreateFontIndirect(&lfLink);
	CXTPFontDC fontDC(pDC, &fontLink, crLink);

	TEXTMETRIC textMetrics;
	pDC->GetTextMetrics(&textMetrics);
	int nCharCount = 0;
	//! pHyperlink->m_rcHyperSpot.left = pDC->GetCurrentPosition().x; // update hyperlink left bound
	//! before drawing
	pHyperlink->m_rcHyperSpot.left = *pnCurrDrawPos;
	nCharCount = DrawString(pnCurrDrawPos, pDC, strOut, rcLink, XTPToUInt(nFlag));

	// update hyperlink spot
	pHyperlink->m_rcHyperSpot.top = rcLink.top;
	//! pHyperlink->m_rcHyperSpot.right = pDC->GetCurrentPosition().x;
	pHyperlink->m_rcHyperSpot.right  = *pnCurrDrawPos;
	pHyperlink->m_rcHyperSpot.bottom = rcLink.top + textMetrics.tmHeight;

	return nCharCount;
}

void CXTPGridPaintManager::DrawSectionDivider(CDC* pDC, const CRect& rc,
											  XTPGridSectionDividerPosition dividerPosition,
											  XTPGridSectionDividerStyle dividerStyle,
											  CXTPPaintManagerColor clrDivider)
{
	switch (dividerStyle)
	{
		case xtpGridFixedRowsDividerNone:
		{
			// Nothing
		}
		break;

		case xtpGridFixedRowsDividerOutlook:
		{
			pDC->FillSolidRect(rc, m_clrHeaderControl);
			pDC->Draw3dRect(rc, m_clrControlLightLight, m_clrControlDark);
		}
		break;

		case xtpGridFixedRowsDividerThin:
		case xtpGridFixedRowsDividerBold: { pDC->FillSolidRect(rc, clrDivider);
		}
		break;

		case xtpGridFixedRowsDividerShade:
		{
			int nScrollPos   = m_pControl->GetScrollPos(SB_VERT);
			int nScrollLimit = m_pControl->GetScrollLimit(SB_VERT);

			if ((xtpGridSectionDividerPositionBottom == dividerPosition) && (nScrollPos > 0))
			{
				CRect rcShade(rc);
				rcShade.top = rcShade.bottom;
				rcShade.bottom += XTP_DPI_Y(4);

				XTPDrawHelpers()->GradientFill(pDC, rcShade, clrDivider, RGB(255, 255, 255), FALSE);
			}

			if ((xtpGridSectionDividerPositionTop == dividerPosition)
				&& (nScrollPos != nScrollLimit))
			{
				CRect rcShade(rc);
				rcShade.bottom = rcShade.top;
				rcShade.top -= XTP_DPI_Y(4);

				XTPDrawHelpers()->GradientFill(pDC, rcShade, RGB(255, 255, 255), clrDivider, FALSE);
			}
		}
		break;
	}
}

int CXTPGridPaintManager::GetSectionDividerHeight(XTPGridFixedRowsDividerStyle dividerStyle)
{
	int nHeight = 0;

	switch (dividerStyle & ~xtpGridFixedRowsDividerShade)
	{
		case xtpGridFixedRowsDividerNone: nHeight = 0; break;
		case xtpGridFixedRowsDividerThin: nHeight = XTP_DPI_Y(1); break;
		case xtpGridFixedRowsDividerBold: nHeight = XTP_DPI_Y(2); break;
		case xtpGridFixedRowsDividerOutlook: nHeight = XTP_DPI_Y(8); break;
	}

	return nHeight;
}

BOOL CXTPGridPaintManager::IsGridVisible(BOOL bVertical) const
{
	return IsGridVisible(bVertical ? xtpGridOrientationVertical : xtpGridOrientationHorizontal);
}

BOOL CXTPGridPaintManager::IsGridVisible(XTPGridOrientation orientation) const
{
	switch (orientation)
	{
		case xtpGridOrientationHorizontal: return xtpGridGridNoLines != GetGridStyle(FALSE);

		case xtpGridOrientationVertical: return xtpGridGridNoLines != GetGridStyle(TRUE);

		default: return FALSE;
	}
}

int CXTPGridPaintManager::GetPrintPageWidth()
{
	if (0 == m_PrintPageWidth)
	{
		CPrintInfo printInfo;
		CXTPGridViewPrintOptions printOptions;

		if (AfxGetApp()->GetPrinterDeviceDefaults(&printInfo.m_pPD->m_pd))
		{
			if (NULL == printInfo.m_pPD->m_pd.hDC)
			{
				printInfo.m_pPD->CreatePrinterDC();
				ASSERT(NULL != printInfo.m_pPD->m_pd.hDC);
			}

			CPreviewDC dc;
			dc.SetAttribDC(printInfo.m_pPD->m_pd.hDC);

			int nResHorz = dc.GetDeviceCaps(HORZRES);
			int nResVert = dc.GetDeviceCaps(VERTRES);

			int nLogPixelsX = dc.GetDeviceCaps(LOGPIXELSX);
			int nLogPixelsY = dc.GetDeviceCaps(LOGPIXELSY);

			printInfo.m_rectDraw.SetRect(0, 0, nResHorz, nResVert);

			dc.SetMapMode(MM_ANISOTROPIC);
			dc.SetViewportExt(nLogPixelsX, nLogPixelsY);
			dc.SetWindowExt(96, 96);
			dc.DPtoLP(&printInfo.m_rectDraw);
			dc.ReleaseAttribDC();

			CDC* pDC		= m_pControl->GetDC();
			CRect rcMargins = printOptions.GetMarginsLP(pDC);
			m_pControl->ReleaseDC(pDC);

			printInfo.m_rectDraw.DeflateRect(rcMargins);
			m_PrintPageWidth = printInfo.m_rectDraw.Width();
		}
	}

	return m_PrintPageWidth;
}

COLORREF CXTPGridPaintManager::GetGridColor() const
{
	return GetDefaultItemBorder()->GetBorderColor();
}

COLORREF CXTPGridPaintManager::SetGridColor(COLORREF clrGridLine)
{
	COLORREF clrOldColor(clrGridLine);

	CXTPGridBorder* pBorder = GetDefaultItemBorder();

	if (NULL != pBorder)
	{
		clrOldColor = pBorder->GetBorderColor();
		pBorder->SetBorderColor(clrGridLine);
	}

	return clrOldColor;
}

CXTPGridBorder* CXTPGridPaintManager::GetDefaultItemBorder() const
{
	return m_pBorder;
}

CXTPGridBorder* CXTPGridPaintManager::GetItemBorder(const CXTPGridRecordItem* pItem) const
{
	CXTPGridBorder* pBorder = NULL;

	if (NULL != pItem)
	{
		pBorder = pItem->GetBorder();
	}

	if (NULL == pBorder)
	{
		pBorder = GetDefaultItemBorder();
	}

	return pBorder;
}

COLORREF CXTPGridPaintManager::MixColor(COLORREF clrLight, COLORREF clrDark, double dFactor)
{
	return RGB(GetRValue(clrLight) - dFactor * (GetRValue(clrLight) - GetRValue(clrDark)),
			   GetGValue(clrLight) - dFactor * (GetGValue(clrLight) - GetGValue(clrDark)),
			   GetBValue(clrLight) - dFactor * (GetBValue(clrLight) - GetBValue(clrDark)));
}

void CXTPGridPaintManager::Line(CDC* pDC, int x, int y, int cx, int cy, CPen* pPen)
{
	ASSERT_VALID(pDC);

	CPen* pOldPen = pDC->SelectObject(pPen);
	pDC->MoveTo(x, y);
	pDC->LineTo(x + cx, y + cy);
	pDC->SelectObject(pOldPen);
}

BOOL CXTPGridPaintManager::OnDrawAction(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs)
{
#ifdef _XTP_ACTIVEX
	BOOL bDoDefault = TRUE;

	CRect& rcItem = pDrawArgs->rcItem;
	CDC* pDC	  = pDrawArgs->pDC;

	if (m_pControl->m_bCustomDrawItem && pDrawArgs->pColumn)
	{
		m_pControl->FireDrawItem(pDrawArgs->pRow->GetIDispatch(FALSE),
								 pDrawArgs->pColumn->GetIDispatch(FALSE),
								 pDrawArgs->pItem->GetIDispatch(FALSE),
								 (OLE_HANDLE) reinterpret_cast<LONG_PTR>(pDC->GetSafeHdc()), rcItem,
								 &bDoDefault);

		if (bDoDefault)
		{
			_variant_t vtDoDefault = VARIANT_TRUE;
			m_pControl->FireDrawItemV(pDrawArgs->pRow->GetIDispatch(FALSE),
									  pDrawArgs->pColumn->GetIDispatch(FALSE),
									  pDrawArgs->pItem->GetIDispatch(FALSE),
									  (OLE_HANDLE) reinterpret_cast<LONG_PTR>(pDC->GetSafeHdc()),
									  rcItem, &vtDoDefault);

			bDoDefault = 0 != (VARIANT_BOOL)vtDoDefault;
		}

		return bDoDefault;
	}

	if (m_pControl->m_bCustomDrawPreviewItem && !pDrawArgs->pColumn)
	{
		// no reference to Column
		m_pControl->FireDrawPreviewItem(pDrawArgs->pRow->GetIDispatch(FALSE),
										pDrawArgs->pItem->GetIDispatch(FALSE),
										(OLE_HANDLE) reinterpret_cast<LONG_PTR>(pDC->GetSafeHdc()),
										rcItem, &bDoDefault);

		if (bDoDefault)
		{
			_variant_t vtDoDefault = VARIANT_TRUE;
			m_pControl->FireDrawPreviewItemV(
				pDrawArgs->pRow->GetIDispatch(FALSE), pDrawArgs->pItem->GetIDispatch(FALSE),
				(OLE_HANDLE) reinterpret_cast<LONG_PTR>(pDC->GetSafeHdc()), rcItem, &vtDoDefault);

			bDoDefault = 0 != (VARIANT_BOOL)vtDoDefault;
		}

		return bDoDefault;
	}

#else
	UNREFERENCED_PARAMETER(pDrawArgs);
#endif
	return TRUE;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridHyperlinkStyle, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridHyperlinkStyle, Color, 1, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridHyperlinkStyle, TextDecoration, 2, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridHyperlinkStyle, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridHyperlinkStyle, XTPDIID_IGridHyperlinkStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridHyperlinkStyle, XTPDIID_IGridHyperlinkStyle)

COLORREF CXTPGridHyperlinkStyle::OleGetColor()
{
	return m_color;
}

void CXTPGridHyperlinkStyle::OleSetColor(COLORREF crColor)
{
	m_color = crColor;
}

int CXTPGridHyperlinkStyle::OleGetTextDecoration()
{
	return m_textDecoration;
}

void CXTPGridHyperlinkStyle::OleSetTextDecoration(int textDecoration)
{
	m_textDecoration = (XTPGridTextDecoration)textDecoration;
}

BEGIN_DISPATCH_MAP(CXTPGridPaintManager, CXTPCmdTarget)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, VerticalGridStyle, 1, VT_I4)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, HorizontalGridStyle, 2, VT_I4)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GridLineColor, 16, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPGridPaintManager, "FixedRowHeight", 38, IsFixedRowHeight,
						SetFixedRowHeight, VT_BOOL)

	DISP_PROPERTY_ID(CXTPGridPaintManager, "ShadeGroupHeadings", 3, m_bShadeGroupHeadings, VT_BOOL)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, TextFont, 4, VT_FONT)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, CaptionFont, 5, VT_FONT)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, PreviewTextFont, 6, VT_FONT)

	DISP_PROPERTY_NOTIFY_ID(CXTPGridPaintManager, "ColumnStyle", 7, m_columnStyle,
							OleSetColumnStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ShadeSortColumn", 8, m_bShadeSortColumn, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "HideSelection", 9, m_bHideSelection, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "NoItemsText", 10, m_strNoItems, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "NoGroupByText", 11, m_strNoGroupBy, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "MaxPreviewLines", 12, m_nMaxPreviewLines, VT_I4)
	DISP_FUNCTION_ID(CXTPGridPaintManager, "SetPreviewIndent", 13, SetPreviewIndent, VT_EMPTY,
					 VTS_I4 VTS_I4 VTS_I4 VTS_I4)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, BackColor, 14, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ForeColor, 15, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ShadeSortColor, 17, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupIndentColor, 18, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupShadeBackColor, 19, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupShadeBorderColor, 20, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupForeColor, 21, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, CaptionBackColor, 22, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, CaptionForeColor, 23, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupBoxBackColor, 27, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupByNoItemsTextColor, 68, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, HighlightBackColor, 28, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, HighlightForeColor, 29, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, BoldTextHighlight, 383, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, BoldText, 384, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, PreviewTextColor, 33, VT_COLOR)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, CaptionBackGradientColorLight, 41, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, CaptionBackGradientColorDark, 42, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ColumnHotGradientColorLight, 43, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ColumnHotGradientColorDark, 44, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ColumnPushedGradientColorLight, 45, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ColumnPushedGradientColorDark, 46, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ColumnShadowGradient, 1011, VT_COLOR)

	DISP_PROPERTY_ID(CXTPGridPaintManager, "TreeIndent", 24, m_nTreeIndent, VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "TreeStructureStyle", 25, m_treeStructureStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "NoFieldsAvailableText", 26, m_strNoFieldsAvailable,
					 VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "DrawSortTriangleAlways", 30, m_bPrintSortTriangle,
					 VT_BOOL)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, GroupShadeForeColor, 31, VT_COLOR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseColumnTextAlignment", 32, m_bUseColumnTextAlignment,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "SortByText", 34, m_strSortBy, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "SortByDecreasingText", 348, m_strDecreasing, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "SortByIncreasingText", 349, m_strIncreasing, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "InvertColumnOnClick", 35, m_bInvertColumnOnClick,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "HotTracking", 36, m_bHotTracking, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "GroupRowTextBold", 37, m_bGroupRowTextBold, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseEditTextAlignment", 39, m_bUseEditTextAlignment,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "RevertAlignment", 40, m_bRevertAlignment, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ThemedInplaceButtons", 51, m_bThemedInplaceButtons,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "FixedInplaceButtonHeight", 52,
					 m_bFixedInplaceButtonHeight, VT_BOOL)

	DISP_PROPERTY_ID(CXTPGridPaintManager, "FreezeColsDividerStyle", 53, m_nFreezeColsDividerStyle,
					 VT_I4)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, FreezeColsDividerColor, 54, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, HotDividerColor, 55, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, FormulaSumColor, 83, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, FormulaSubSumColor, 84, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, AlternativeBackgroundColor, 92, VT_COLOR)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, HeaderRowsDividerStyle, 56, VT_I4)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, HeaderRowsDividerColor, 57, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, FooterRowsDividerStyle, 58, VT_I4)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, FooterRowsDividerColor, 59, VT_COLOR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "DrawGridForEmptySpace", 60, m_bDrawGridForEmptySpace,
					 VT_BOOL)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, SelectedRowBackColor, 62, VT_COLOR)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, SelectedRowForeColor, 61, VT_COLOR)

	XTP_DISP_PROPERTY_EX_ID(CXTPGridPaintManager, ColumnOffice2007CustomThemeBaseColor, 63,
							VT_COLOR)
	DISP_FUNCTION_ID(CXTPGridPaintManager, "RefreshMetrics", 64, OleRefreshMetrics, VT_EMPTY,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPGridPaintManager, "HyperlinkStyle", 350, OleGetHyperlinkStyle, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridPaintManager, "HyperlinkHoverStyle", 351, OleGetHyperlinkHoverStyle,
					 VT_DISPATCH, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPGridPaintManager, "Glyphs", 66, OleGetGlyphs, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "DrawSortTriangleStyle", 67, m_DrawSortTriangleStyle,
					 VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ColumnWidthWYSIWYG", 69, m_bColumnWidthWYSIWYG, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ColumnPlusMinusSign", 70, m_bColumnPlusMinusSign,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ShowLockIcon", 71, m_bShowLockIcon, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseShellIcon", 72, m_bUseShellIcon, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ExpandText", 73, m_strExpand, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "CollapseText", 74, m_strCollapse, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "RecOrRowNumber", 75, m_bRecOrRowNum, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "StartRecOrRowNumber", 76, m_iRecOrRowStart, VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "LastColumnWidthWYSIWYG", 77, m_bLastColumnWidthWYSIWYG,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseGroupRowSelectionState", 78,
					 m_bUseGroupRowSelectionState, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ForceDynamicMarkupForCell", 79,
					 m_bForceDynamicMarkupForCell, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "PrintPageRectangle", 80, m_bPrintPageRectangle, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ForceShowDropMarker", 81, m_bForceShowDropMarker,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ShowNonActiveInPlaceButton", 85,
					 m_bShowNonActiveInPlaceButton, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "PrintWatermark", 86, m_bPrintWatermark, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "BaseTreeIndent", 87, m_bBaseTreeIndent, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ShowWYSIWYGMarkers", 88, m_bShowWYSIWYGMarkers, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "ForceShowTooltip", 89, m_bForceShowTooltip, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "GrayIfDisable", 90, m_bGrayIfDisable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseAlternativeBackground", 91,
					 m_bUseAlternativeBackground, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "CleanTooltip", 93, m_bCleanTooltip, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "MoveScrollbarOnFixedColumnsIndent", 94,
					 m_bMoveScrollbarOnFixedColumnsIndent, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "CompactText", 95, m_bCompactText, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "AllowMergeCells", 96, m_bAllowMergeCells, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseDropMarker", 334, m_bUseDropMarker, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "TextIndentForGroupRowsWithNoIcon", 335,
					 m_nTextIndentForGroupRowsWithNoIcon, VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "SortTriangleOffsetFromRight", 336,
					 m_bSortTriangleOffsetFromRight, VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "TreeColumnLineLength", 337, m_nTreeColumnLineLength,
					 VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "BaseParentRecordIndent", 338, m_nBaseParentRecordIndent,
					 VT_I4)
	DISP_PROPERTY_ID(CXTPGridPaintManager, "UseUpperCaseCaptions", 356, m_bMakeUpper, VT_BOOL)

	DISP_PROPERTY_ID(CXTPGridPaintManager, "GroupGridLineHeight", 1014, m_nGroupGridLineHeight,
					 VT_I4)

	DISP_PROPERTY_EX_ID(CXTPGridPaintManager, "HeaderHeight", 355, OleGetHeaderHeight,
						OleSetHeaderHeight, VT_I4)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridPaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridPaintManager, XTPDIID_IGridPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridPaintManager, XTPDIID_IGridPaintManager)

void CXTPGridPaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPGridPaintManager::OleSetColumnStyle()
{
	SetColumnStyle(m_columnStyle);

	RefreshMetrics();
}

LPDISPATCH CXTPGridPaintManager::OleGetHyperlinkStyle()
{
	return XTPGetDispatch(m_pHyperlinkStyle);
}

LPDISPATCH CXTPGridPaintManager::OleGetHyperlinkHoverStyle()
{
	return XTPGetDispatch(m_pHyperlinkHoverStyle);
}

COLORREF CXTPGridPaintManager::OleGetGridLineColor()
{
	return GetGridColor();
}

void CXTPGridPaintManager::OleSetGridLineColor(COLORREF crGridLine)
{
	SetGridColor(crGridLine);
}

int CXTPGridPaintManager::OleGetVerticalGridStyle()
{
	return GetGridStyle(TRUE);
}

int CXTPGridPaintManager::OleGetHeaderHeight()
{
	return GetHeaderHeight();
}

void CXTPGridPaintManager::OleSetHeaderHeight(int nValue)
{
	SetHeaderHeight(nValue);
}

void CXTPGridPaintManager::OleSetVerticalGridStyle(int nGridStyle)
{
	SetGridStyle(TRUE, (XTPGridGridStyle)nGridStyle);
}

int CXTPGridPaintManager::OleGetHorizontalGridStyle()
{
	return GetGridStyle(FALSE);
}

void CXTPGridPaintManager::OleSetHorizontalGridStyle(int nGridStyle)
{
	SetGridStyle(FALSE, (XTPGridGridStyle)nGridStyle);
}

LPFONTDISP CXTPGridPaintManager::OleGetTextFont()
{
	return AxCreateOleFont(&m_xtpFontText, this,
						   (LPFNFONTCHANGED)&CXTPGridPaintManager::OleSetTextFont);
}

void CXTPGridPaintManager::OleSetTextFont(LPFONTDISP pFontDisp)
{
	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		SetTextFont(lf);
	}
}

LPDISPATCH CXTPGridPaintManager::OleGetGlyphs()
{
	return XTPGetDispatch(m_pGlyphs);
}

LPFONTDISP CXTPGridPaintManager::OleGetPreviewTextFont()
{
	return AxCreateOleFont(&m_xtpFontPreview, this,
						   (LPFNFONTCHANGED)&CXTPGridPaintManager::OleSetPreviewTextFont);
}

void CXTPGridPaintManager::OleSetPreviewTextFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_xtpFontPreview.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_xtpFontPreview.CreateFontIndirect(&lf);
	}
}

LPFONTDISP CXTPGridPaintManager::OleGetCaptionFont()
{
	return AxCreateOleFont(&m_xtpFontCaption, this,
						   (LPFNFONTCHANGED)&CXTPGridPaintManager::OleSetCaptionFont);
}

void CXTPGridPaintManager::OleSetCaptionFont(LPFONTDISP pFontDisp)
{
	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		SetCaptionFont(lf);
	}
}

COLORREF CXTPGridPaintManager::OleGetColumnOffice2007CustomThemeBaseColor()
{
	return m_clrColumnOffice2007CustomTheme;
}

void CXTPGridPaintManager::OleSetColumnOffice2007CustomThemeBaseColor(COLORREF oleColor)
{
	m_clrColumnOffice2007CustomTheme = AxTranslateColor(oleColor);
	RefreshMetrics();
}

XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, BackColor, m_clrControlBack)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ForeColor, m_clrWindowText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ShadeSortColor, m_clrItemShade)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupIndentColor, m_clrIndentControl)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupShadeBackColor, m_clrGroupShadeBack)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupShadeBorderColor, m_clrGroupShadeBorder)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupForeColor, m_clrGroupRowText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, CaptionBackColor, m_clrHeaderControl)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, CaptionForeColor, m_clrCaptionText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupBoxBackColor, m_clrGroupBoxBack)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupByNoItemsTextColor, m_crlNoGroupByText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, HighlightBackColor, m_clrHighlight)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, HighlightForeColor, m_clrHighlightText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, BoldTextHighlight, m_clrBoldTextHighlight)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, BoldText, m_clrBoldText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, GroupShadeForeColor, m_clrGroupShadeText)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, PreviewTextColor, m_clrPreviewText)

XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, SelectedRowBackColor, m_clrSelectedRow)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, SelectedRowForeColor, m_clrSelectedRowText)

XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, CaptionBackGradientColorLight,
							 m_grcGradientColumn.clrLight)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, CaptionBackGradientColorDark,
							 m_grcGradientColumn.clrDark)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ColumnHotGradientColorLight,
							 m_grcGradientColumnHot.clrLight)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ColumnHotGradientColorDark,
							 m_grcGradientColumnHot.clrDark)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ColumnPushedGradientColorLight,
							 m_grcGradientColumnPushed.clrLight)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ColumnPushedGradientColorDark,
							 m_grcGradientColumnPushed.clrDark)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, ColumnShadowGradient, m_clrGradientColumnShadow)

XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, FreezeColsDividerColor, m_clrFreezeColsDivider)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, HotDividerColor, m_clrHotDivider)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, HeaderRowsDividerColor, m_clrHeaderRowsDivider)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, FooterRowsDividerColor, m_clrFooterRowsDivider)
// XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, HyperLinkColor, m_clrHyper)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, FormulaSumColor, m_clrFormulaSum)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, FormulaSubSumColor, m_clrFormulaSubSum)
XTP_IMPLEMENT_PROPERTY_COLOR(CXTPGridPaintManager, AlternativeBackgroundColor,
							 m_clrAlternativeBackground)

#endif
