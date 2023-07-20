// XTPTrackThemeOffice2013.cpp : implementation of the CXTPTrackPaintManager class.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/Resource.h"
#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridHyperlink.h"

#include "GridControl/TrackControl/XTPTrackPaintManager.h"
#include "GridControl/TrackControl/Themes/XTPTrackThemeOffice2013.h"
#include "GridControl/TrackControl/XTPTrackControlItem.h"
#include "GridControl/TrackControl/XTPTrackControl.h"
#include "GridControl/TrackControl/XTPTrackBlock.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeOffice2013
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeOffice2013::CXTPTrackThemeOffice2013()
{
	m_bMakeUpper  = TRUE;
	m_columnStyle = xtpGridColumnShaded;
	LoadGlyphs();
}

void CXTPTrackThemeOffice2013::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	int nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_GRID_GLYPHS_2013_12,
												   XTP_IDB_GRID_GLYPHS_2013_15,
												   XTP_IDB_GRID_GLYPHS_2013_18,
												   XTP_IDB_GRID_GLYPHS_2013_24);
	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTPToUInt(nBmpID)));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpGridGlyphCount, CSize(0, 0)); // 11
}

void CXTPTrackThemeOffice2013::RefreshMetrics()
{
	// default colors located in OFFICE2013WORD_INI.

	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPGridHyperlinkStyle(XTPIniColor(_T("GridControl"), _T("HyperLink"),
															   RGB(43, 87, 154)),
												   xtpGridTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPGridHyperlinkStyle(XTPIniColor(_T("GridControl"),
																	_T("HyperLinkHover"),
																	RGB(163, 189, 227)),
														xtpGridTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	// Track Control specific colors.

	m_clrBtnFace.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BtnFace"),
					RGB(225, 225, 225))); /* grid and separator color between items */
	m_clrBtnFacePressed.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BtnFacePressed"),
					RGB(243, 243, 243))); /* header column pushed color */
	m_clrBtnText.SetStandardValue(XTPIniColor(_T("GridControl"), _T("BtnText"), RGB(68, 68, 68)));
	m_clrControlDark.SetStandardValue(XTPIniColor(_T("GridControl"), _T("ControlDark"),
												  RGB(171, 171, 171))); /* header border color */
	m_clrControlLightLight.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("ControlLightLight"), RGB(255, 255, 255)));
	m_clrHighlight.SetStandardValue(XTPIniColor(_T("GridControl"), _T("Highlight"),
												RGB(213, 225, 242))); /* row back selected color */
	m_clrHighlightText.SetStandardValue(XTPIniColor(_T("GridControl"), _T("HighlightText"),
													RGB(68, 68, 68))); /* row text selected color */
	m_clrBoldText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BoldText"), RGB(43, 87, 154))); /* row text bold color */
	m_clrBoldTextHighlight.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BoldTextHighlight"),
					RGB(43, 87, 154))); /* row text bold selected color */
	m_clrGroupText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupText"), RGB(68, 68, 68))); /* group text color */
	m_clrGroupTextHilite.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupTextHilite"),
					RGB(68, 68, 68))); /* group text selected color */
	m_clrGroupBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupBack"), RGB(243, 243, 243))); /* group back color */
	m_clrGroupBackHilite.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupBackHilite"),
					RGB(213, 225, 242))); /* group back selected color */
	m_clrSelectedRow.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("SelectedRow"), RGB(225, 225, 225)));
	m_clrSelectedRowText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("SelectedRowText"), RGB(68, 68, 68)));
	m_clrItemShade.SetStandardValue(XTPIniColor(_T("GridControl"), _T("ItemShade"),
												RGB(255, 255, 255))); /* sorted column back color */
	m_clrHotDivider.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("HotDivider"),
					RGB(255, 0, 0))); /* red arrow indicator when moving columns */
	m_clrFreezeColsDivider.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("FreezeColsDivider"), RGB(43, 87, 154)));
	m_clrWindowText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("WindowText"), RGB(68, 68, 68))); /* row text color */
	m_clrControlBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("ControlBack"), RGB(255, 255, 255))); /* row back color */
	m_clrConnector.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("Connector"),
					RGB(171, 171, 171))); /* group-by connector line color */
	m_clrIndentControl.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("IndentControl"), RGB(255, 255, 255)));
	m_clrCaptionText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("CaptionText"), RGB(68, 68, 68))); /* header text color */
	m_clrCaptionTextPressed.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("CaptionTextPressed"),
					RGB(68, 68, 68))); /* header text pushed color */
	m_clrHeaderControl.SetStandardValue(XTPIniColor(_T("GridControl"), _T("HeaderControl"),
													RGB(255, 255, 255))); /* header back color */
	m_clrGroupShadeBorder.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupShadeBorder"), RGB(171, 171, 171)));
	m_clrGroupShadeBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupShadeBack"), RGB(255, 255, 255)));
	m_clrGroupShadeText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupShadeText"), RGB(68, 68, 68)));
	m_clrGroupRowText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupRowText"), RGB(171, 171, 171)));
	m_clrGroupBoxBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupBoxBack"),
					RGB(255, 255, 255))); /* group-by area back color */
	m_crlNoGroupByText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("NoGroupByText"),
					RGB(68, 68, 68))); /* group-by area text color */
	m_clrGradientColumnSeparator.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GradientColumnSeparator"), RGB(171, 171, 171)));
	m_clrGlyph.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("Glyph"), RGB(171, 171, 171))); /* sort arrow color */
	m_clrGlyphHighlight.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GlyphHighlight"),
					RGB(171, 171, 171))); /* sort arrow highlight color */

	// Track Control specific colors.

	m_clrWorkArea = XTPIniColor(_T("GridControl.TrackControl"), _T("WorkArea"), RGB(250, 250, 250));
	m_clrTrackHeader		= XTPIniColor(_T("GridControl.TrackControl"), _T("TrackHeader"),
									  RGB(255, 255, 255));
	m_clrTrackTimeArea		= XTPIniColor(_T("GridControl.TrackControl"), _T("TrackTimeArea"),
									  RGB(255, 255, 255));
	m_clrTimeHeaderDarkDark = XTPIniColor(_T("GridControl.TrackControl"), _T("TimeHeaderDarkDark"),
										  RGB(225, 225, 225));
	m_clrTimeHeaderDark		= XTPIniColor(_T("GridControl.TrackControl"), _T("TimeHeaderDark"),
									  RGB(243, 243, 243));
	m_clrTimeSliderBackground = XTPIniColor(_T("GridControl.TrackControl"),
											_T("TimeSliderBackground"), RGB(243, 243, 243));
	m_clrTimeHeaderDivider	= XTPIniColor(_T("GridControl.TrackControl"), _T("TimeHeaderDivider"),
											RGB(225, 225, 225));
	m_clrScrollBarLight		  = XTPIniColor(_T("GridControl.TrackControl"), _T("ScrollBarLight"),
										RGB(243, 243, 243));
	m_clrScrollBarDark		  = XTPIniColor(_T("GridControl.TrackControl"), _T("ScrollBarDark"),
										RGB(243, 243, 243));
	m_clrMarker = XTPIniColor(_T("GridControl.TrackControl"), _T("Marker"), RGB(247, 168, 171));
	m_clrSelectedArea		 = XTPIniColor(_T("GridControl.TrackControl"), _T("SelectedArea"),
									   RGB(43, 87, 154));
	m_clrTimeLineTick		 = XTPIniColor(_T("GridControl.TrackControl"), _T("TimeLineTick"),
									   RGB(35, 35, 35));
	m_clrMarkerBorder		 = XTPIniColor(_T("GridControl.TrackControl"), _T("MarkerBorder"),
									   RGB(43, 90, 127));
	m_clrMarkerBack			 = XTPIniColor(_T("GridControl.TrackControl"), _T("MarkerBack"),
								   RGB(153, 192, 231));
	m_clrTrackMarkerBorder   = XTPIniColor(_T("GridControl.TrackControl"), _T("TrackMarkerBorder"),
										   RGB(0, 0, 0));
	m_clrTrackMarkerText	 = XTPIniColor(_T("GridControl.TrackControl"), _T("TrackMarkerText"),
									   RGB(0, 0, 0));
	m_clrTrackMarkerTextBack = XTPIniColor(_T("GridControl.TrackControl"),
										   _T("TrackMarkerTextBack"), RGB(255, 255, 255));
	m_clrTrackMarkerTextBorder = XTPIniColor(_T("GridControl.TrackControl"),
											 _T("TrackMarkerTextBorder"), RGB(123, 123, 123));
	m_clrTimeLinePosition	  = XTPIniColor(_T("GridControl.TrackControl"), _T("TimeLinePosition"),
										 RGB(205, 0, 0));
	m_clrTimeHeaderBorder	  = XTPIniColor(_T("GridControl.TrackControl"), _T("TimeHeaderBorder"),
										 RGB(35, 35, 35));
	m_clrScrollBarBorderDark   = XTPIniColor(_T("GridControl.TrackControl"),
											 _T("ScrollBarBorderDark"), RGB(0, 0, 0));
	m_clrScrollBarBorderLight  = XTPIniColor(_T("GridControl.TrackControl"),
											 _T("ScrollBarBorderLight"), RGB(155, 155, 155));
	m_clrWorkAreaScrollBarLight  = XTPIniColor(_T("GridControl.TrackControl"),
											   _T("WorkAreaScrollBarLight"), RGB(153, 192, 231));
	m_clrWorkAreaScrollBarDark   = XTPIniColor(_T("GridControl.TrackControl"),
											   _T("WorkAreaScrollBarDark"), RGB(27, 102, 157));
	m_clrWorkAreaScrollBarBorder = XTPIniColor(_T("GridControl.TrackControl"),
											   _T("WorkAreaScrollBarBorder"), RGB(43, 90, 127));
	m_clrSelectedBorder			 = XTPIniColor(_T("GridControl.TrackControl"), _T("SelectedBorder"),
									   RGB(0, 0, 0));

	SetGridColor(m_clrBtnFace);

	m_nGroupGridLineStyle = xtpGridGridSolid;

	m_xtpBrushVeriticalGrid.DeleteObject();
	m_xtpBrushHorizontalGrid.DeleteObject();
	m_xtpBrushTreeStructure.DeleteObject();
	m_xtpBrushGroupGridLineStyle.DeleteObject();

	LOGFONT lf;
	m_xtpFontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);
}

void CXTPTrackThemeOffice2013::DrawColumnText(CDC* pDC, CRect rcText, CString strCaption,
											  int nHeaderAlignment, BOOL bIsHeader,
											  BOOL bColumnPressed)
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
		if (nHeightDiff > 1)
			rcText.top += nHeightDiff / 2;
		else if (nHeightDiff < 0)
		{
			rcText.top += XTP_DPI_Y(3);
		}
	}
	else
	{
		rcText.OffsetRect(XTP_DPI_X(2), 0);
		uFlags |= DT_SINGLELINE | DT_VCENTER;
	}

	pDC->DrawText(strCaption, &rcText, uFlags);
}

void CXTPTrackThemeOffice2013::DrawPlainColumnBackground(CDC* pDC, CRect rcColumn)
{
	pDC->Draw3dRect(rcColumn, m_clrControlDark, m_clrControlDark);
}

void CXTPTrackThemeOffice2013::DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn,
													CRect rcColumn, BOOL& bColumnPressed,
													BOOL& bDraggingFromHeader,
													CXTPGridHeader* pHeader)
{
	CXTPGridColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1)
										  : NULL;

	if (!bDraggingFromHeader)
	{
		if (bColumnPressed)
		{
			pDC->FillSolidRect(rcColumn, m_clrBtnFacePressed);
		}

		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - XTP_DPI_Y(1), rcColumn.Width(),
						   m_clrControlDark);

		if (pPrevColumn && pColumn->GetDrawHeaderDivider())
			DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + XTP_DPI_Y(3),
							 rcColumn.Height() - XTP_DPI_Y(6), m_clrControlDark);

		if (pColumn->GetControl()->IsGroupByVisible())
			DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(),
							   m_clrControlDark);
	}
}

void CXTPTrackThemeOffice2013::DrawColumnInverted(CDC* pDC, CRect rcColumn, BOOL /*bColumnPressed*/,
												  BOOL bDraggingFromHeader, int nShadowWidth)
{
	if (bDraggingFromHeader)
	{
		rcColumn.bottom += nShadowWidth;
		pDC->InvertRect(&rcColumn);
	}
}

void CXTPTrackThemeOffice2013::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown,
											BOOL bDraggingFromHeader, int /*nShadowWidth*/,
											BOOL bColumnPressed)
{
	if (rcTriangle.Width() > XTP_DPI_X(10) && !bDraggingFromHeader)
	{
		CPoint pt(rcTriangle.CenterPoint());
		pt.Offset(5, 0);

		if (bToDown)
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - XTP_DPI_X(4), pt.y - XTP_DPI_Y(2)),
									  CPoint(pt.x, pt.y + XTP_DPI_Y(2)),
									  CPoint(pt.x + XTP_DPI_X(4), pt.y - XTP_DPI_Y(2)),
									  bColumnPressed ? m_clrGlyphHighlight : m_clrGlyph);
		}
		else
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - XTP_DPI_X(4), pt.y + XTP_DPI_Y(2)),
									  CPoint(pt.x, pt.y - XTP_DPI_Y(2)),
									  CPoint(pt.x + XTP_DPI_X(4), pt.y + XTP_DPI_Y(2)),
									  bColumnPressed ? m_clrGlyphHighlight : m_clrGlyph);
		}
	}
}

void CXTPTrackThemeOffice2013::DrawWindowFrame(CDC* pDC, CRect rcWindow)
{
	if (::IsWindow(m_pControl->GetSafeHwnd()))
	{
		if (m_pControl->GetExStyle() & WS_EX_STATICEDGE)
		{
			pDC->Draw3dRect(rcWindow, m_clrBtnFace, m_clrBtnFace);
		}
		else
		{
			CXTPTrackPaintManager::DrawWindowFrame(pDC, rcWindow);
		}
	}
}

void CXTPTrackThemeOffice2013::FillGroupRowMetrics(CXTPGridGroupRow* pRow,
												   XTP_GRIDRECORDITEM_METRICS* pMetrics,
												   BOOL bPrinting)
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

void CXTPTrackThemeOffice2013::DrawGroupRowBack(CDC* pDC, CRect rcRow,
												XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	if (pMetrics->clrBackground != XTP_GRID_COLOR_DEFAULT)
	{
		pDC->FillSolidRect(rcRow, pMetrics->clrBackground);
		DrawHorizontalLine(pDC, rcRow.left, rcRow.top - XTP_DPI_Y(1), rcRow.Width(), m_clrBtnFace);
		DrawHorizontalLine(pDC, rcRow.left, rcRow.top, rcRow.Width(), m_clrBtnFace);
	}
}

void CXTPTrackThemeOffice2013::DrawGroupRow(CDC* pDC, CXTPGridGroupRow* pRow, CRect rcRow,
											XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	CXTPGridControl* pControl = pRow->GetControl();
	BOOL bControlFocused	  = pControl->HasFocus();

	if (pRow->IsFocused() && bControlFocused && !pDC->IsPrinting() && pControl->IsRowFocusVisible())
	{
		m_clrGroupShadeBorder   = m_clrGroupBackHilite;
		pMetrics->clrForeground = m_clrGroupTextHilite;
	}
	else
	{
		m_clrGroupShadeBorder   = m_clrGroupBack;
		pMetrics->clrForeground = m_clrGroupText;
	}

	pMetrics->clrBackground = m_clrGroupShadeBorder;

	CXTPTrackPaintManager::DrawGroupRow(pDC, pRow, rcRow, pMetrics);
}

void CXTPTrackThemeOffice2013::DrawFocusedRow(CDC* pDC, CRect rcRow)
{
	if (m_pControl && ((CXTPGridControl*)m_pControl)->IsIconView())
		rcRow.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

	pDC->Draw3dRect(rcRow, m_clrBtnFace, m_clrBtnFace);
	DrawHorizontalLine(pDC, rcRow.left, rcRow.bottom, rcRow.Width(), m_clrBtnFace);

	COLORREF clrTextColor = pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
	COLORREF clrBkColor   = pDC->SetBkColor(m_clrBtnFace);

	pDC->DrawFocusRect(rcRow);
	pDC->SetTextColor(clrTextColor);
	pDC->SetBkColor(clrBkColor);
}

int CXTPTrackThemeOffice2013::GetRowHeight(CDC* pDC, CXTPGridRow* pRow)
{
	if (m_pControl->m_bThemeMetrics)
	{
		if (!pRow->IsGroupRow())
			return m_nRowHeight + (IsGridVisible(FALSE) ? XTP_DPI_Y(1) : 0);

		return m_nRowHeight + XTP_DPI_Y(6);
	}
	else
	{
		return CXTPTrackPaintManager::GetRowHeight(pDC, pRow);
	}
}

CRect CXTPTrackThemeOffice2013::GetGroupRowTextSize(CRect rcBitmap, CRect rcRow, int nBitmapWidth,
													int nNoIconWidth, int nTextOffset)
{
	if (m_pControl->m_bThemeMetrics)
	{
		CRect rcText(rcBitmap.left, rcRow.top, rcRow.right, rcRow.bottom);
		rcText.left += nBitmapWidth;
		rcText.left += nNoIconWidth;
		rcText.left += nTextOffset;
		rcText.left += m_nTreeTextIndentGroupRow;
		return rcText;
	}
	else
	{
		return CXTPTrackPaintManager::GetGroupRowTextSize(rcBitmap, rcRow, nBitmapWidth,
														  nNoIconWidth, nTextOffset);
	}
}

CRect CXTPTrackThemeOffice2013::GetGroupRowBmpSize(CRect rcRow, int nBitmapOffset)
{
	if (m_pControl->m_bThemeMetrics)
	{
		CRect rcBitmap(rcRow);
		rcBitmap.OffsetRect(nBitmapOffset, 0);
		return rcBitmap;
	}
	else
	{
		return CXTPTrackPaintManager::GetGroupRowBmpSize(rcRow, nBitmapOffset);
	}
}

int CXTPTrackThemeOffice2013::DrawTrackBlock(CDC* pDC, CRect rc, BOOL bSelected,
											 CXTPTrackBlock* pTrackBlock)
{
	if (!pTrackBlock)
		return 0;

	BOOL bLocked = pTrackBlock->IsLocked();

	int nHeight;

	if (pTrackBlock->m_nHeightFixed != 0)
		nHeight = pTrackBlock->m_nHeightFixed;
	else
		nHeight = int((rc.Height()) * pTrackBlock->m_dHeightPercent);

	if (nHeight != rc.Height())
	{
		if (pTrackBlock->m_nVerticalAlignment == DT_TOP)
		{
			rc.bottom = rc.top + nHeight;
		}
		else if (pTrackBlock->m_nVerticalAlignment == DT_BOTTOM)
		{
			rc.top = rc.bottom - nHeight;
		}
		else
		{
			rc.top	= (rc.top + rc.bottom - nHeight) / 2;
			rc.bottom = rc.top + nHeight;
		}
	}
	if (pTrackBlock->m_bLocked)
		bLocked = TRUE;

	CXTPTrackControl* pTrackControl = pTrackBlock->m_pItem->GetTrackControl();

	rc.left  = pTrackControl->PositionToTrack(pTrackBlock->m_nPosition);
	rc.right = pTrackControl->PositionToTrack(pTrackBlock->m_nPosition + pTrackBlock->m_nLength);

	COLORREF clrBorder = XTPDrawHelpers()->DarkenColor(75, pTrackBlock->m_clrBlock);

	CRect rcBorder(rc);

	pDC->FillSolidRect(rc, bLocked ? clrBorder : pTrackBlock->m_clrBlock);

	if (bLocked)
	{
		pDC->Draw3dRect(rcBorder, m_clrSelectedBorder, m_clrSelectedBorder);
	}
	else if (bSelected)
	{
		pDC->Draw3dRect(rcBorder, m_clrSelectedBorder, m_clrSelectedBorder);
		rcBorder.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		pDC->Draw3dRect(rcBorder, m_clrSelectedBorder, m_clrSelectedBorder);

		// draw left sizing gripper.

		int x = rcBorder.left - XTP_DPI_X(3);
		int y = rcBorder.bottom - (rcBorder.Height() / 2) - XTP_DPI_Y(3);

		CXTPDrawHelpers::DrawColorRect(pDC, CRect(x, y, x + XTP_DPI_X(6), y + XTP_DPI_Y(5)),
									   m_clrTrackMarkerTextBack, m_clrSelectedBorder);

		// draw right sizing gripper.

		x = rcBorder.right - XTP_DPI_X(3);
		y = rcBorder.bottom - (rcBorder.Height() / 2) - XTP_DPI_Y(3);

		CXTPDrawHelpers::DrawColorRect(pDC, CRect(x, y, x + XTP_DPI_X(6), y + XTP_DPI_Y(5)),
									   m_clrTrackMarkerTextBack, m_clrSelectedBorder);
	}
	else
	{
		pDC->Draw3dRect(rcBorder, clrBorder, clrBorder);
	}

	if (!pTrackBlock->m_strCaption.IsEmpty())
	{
		pDC->SetTextColor(bLocked ? m_clrGroupRowText : m_clrBtnText);

		CRect rcText(rc);
		rcText.DeflateRect(XTP_DPI_X(4), 0);

		XTPMarkupDrawText(pTrackBlock->m_pItem->GetTrackControl()->GetMarkupContext(),
						  pDC->GetSafeHdc(), pTrackBlock->m_strCaption, rcText,
						  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}

	pTrackBlock->m_rcBlock = rc;
	return 1;
}
