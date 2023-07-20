// XTPGridThemeResource.cpp : implementation of the CXTPGridThemeResource class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridControl.h"

#include "GridControl/Themes/XTPGridThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeResource
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeResource::CXTPGridThemeResource()
{
	m_columnStyle = xtpGridColumnResource;
}

void CXTPGridThemeResource::RefreshMetrics()
{
	CXTPGridPaintManager::RefreshMetrics();

	if (m_clrColumnOffice2007CustomTheme != COLORREF_NULL)
	{
		COLORREF clrHsl = XTPDrawHelpers()->RGBtoHSL(m_clrColumnOffice2007CustomTheme);
		BYTE Hue		= GetRValue(clrHsl);
		BYTE Sat		= GetGValue(clrHsl);

		m_grcGradientColumnPushed.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 212)),
												   XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 239)));
		m_grcGradientColumn.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 239)),
											 XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 212)));
		m_clrGradientColumnShadow.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(
			RGB(Hue, static_cast<BYTE>(static_cast<float>(Sat) * 0.525f), 145)));
		m_clrGradientColumnSeparator.SetStandardValue(
			XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 192)));
		m_clrGroupBoxBack.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 227)));
		m_crlNoGroupByText.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(
			RGB(Hue, static_cast<BYTE>(static_cast<float>(Sat) * 0.75f), 79)));
		m_clrGroupShadeBack.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(
			RGB(Hue, static_cast<BYTE>(static_cast<float>(Sat) * 0.88f), 208)));
	}
	else
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		if (pImages->IsValid())
		{
			m_grcGradientColumn.SetStandardValue(pImages->GetImageColor(_T("GridControl"),
																		_T("NormalColumnLight"),
																		RGB(252, 253, 255)),
												 pImages->GetImageColor(_T("GridControl"),
																		_T("NormalColumnDark"),
																		RGB(196, 221, 255)));
			m_grcGradientColumnPushed.SetStandardValue(
				pImages->GetImageColor(_T("GridControl"), _T("PressedColumnLight"),
									   RGB(196, 221, 255)),
				pImages->GetImageColor(_T("GridControl"), _T("PressedColumnDark"),
									   RGB(252, 253, 255)));

			m_clrGradientColumnShadow.SetStandardValue(
				pImages->GetImageColor(_T("GridControl"), _T("ColumnShadow"), RGB(101, 147, 207)));
			m_clrGradientColumnSeparator.SetStandardValue(
				pImages->GetImageColor(_T("GridControl"), _T("ColumnSeparator"),
									   RGB(154, 198, 255)));
			m_clrGroupBoxBack.SetStandardValue(pImages->GetImageColor(_T("GridControl"),
																	  _T("GroupBoxBackground"),
																	  RGB(227, 239, 255)));
			m_crlNoGroupByText.SetStandardValue(pImages->GetImageColor(_T("GridControl"),
																	   _T("GroupBoxTextColor"),
																	   RGB(50, 75, 44)));
			m_clrGroupShadeBack.SetStandardValue(pImages->GetImageColor(_T("GridControl"),
																		_T("GroupShadeBackground"),
																		RGB(190, 218, 251)));
			m_clrGroupShadeBorder.SetStandardValue(pImages->GetImageColor(_T("GridControl"),
																		  _T("GroupShadeBorder"),
																		  RGB(111, 157, 217)));
			m_clrIndentControl.SetStandardValue(pImages->GetImageColor(_T("GridControl"),
																	   _T("GroupShadeIndent"),
																	   RGB(253, 238, 201)));
			m_clrHighlight.SetStandardValue(
				pImages->GetImageColor(_T("GridControl"), _T("SelectionBackground")));
			m_clrHighlightText.SetStandardValue(0);

			m_nGroupGridLineStyle = (XTPGridGridStyle)pImages->GetImageInt(
				_T("GridControl"), _T("GroupShadeBorderLineStyle"), xtpGridGridSolid);
		}
		else
		{
			m_grcGradientColumnPushed.SetStandardValue(RGB(196, 221, 255), RGB(252, 253, 255));
			m_grcGradientColumn.SetStandardValue(RGB(252, 253, 255), RGB(196, 221, 255));
			m_clrGradientColumnShadow.SetStandardValue(RGB(101, 147, 207));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(154, 198, 255));
			m_clrGroupBoxBack.SetStandardValue(RGB(227, 239, 255));
			m_crlNoGroupByText.SetStandardValue(RGB(50, 75, 44));
			m_clrGroupShadeBack.SetStandardValue(RGB(190, 218, 251));
			m_clrGroupShadeBorder.SetStandardValue(RGB(111, 157, 217));
			m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
		}
	}

	m_grcGradientColumnHot.SetStandardValue(m_grcGradientColumn);
	m_clrGroupRowText.SetStandardValue(m_crlNoGroupByText);
	m_clrConnector.SetStandardValue(m_clrGroupShadeBorder);
}

CSize CXTPGridThemeResource::DrawCollapsedBitmap(CDC* pDC, const CXTPGridRow* pRow, CRect& rcBitmap)
{
	// Check for resource image dll
	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("GridExpandButton"));
	if (pImage)
	{
		rcBitmap.left += XTP_DPI_X(4);

		// use first image as size for bitmap
		CRect rcSrc(pImage->GetSource(0, 4));

		CSize sizeGlyph(rcSrc.Size());

		if (pDC)
		{
			POINT ptIcon;
			ptIcon.x = rcBitmap.left;

			ptIcon.y = rcBitmap.top + (rcBitmap.Height() - rcSrc.Height()) / 2;

			CRect rcDest(ptIcon, rcSrc.Size());

			int nState = pRow->IsExpanded() ? 0 : 2;

			if (pRow == pRow->GetControl()->m_pHotExpandButtonRow)
				nState++;

			pImage->DrawImage(pDC, rcDest, pImage->GetSource(nState, 4));
		}

		if (sizeGlyph.cx != 0 && pDC != NULL)
			rcBitmap.right = rcBitmap.left + sizeGlyph.cx;

		return CSize(XTP_DPI_X(2) + sizeGlyph.cx, XTP_DPI_Y(2) + sizeGlyph.cy);
	}
	else
	{
		return CXTPGridPaintManager::DrawCollapsedBitmap(pDC, pRow, rcBitmap);
	}
}

void CXTPGridThemeResource::DrawNoGroupByText(CDC* pDC, CRect rcItem)
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

			XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), m_xtpFontCaption,
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
	pDC->DrawText(m_strNoGroupBy, &rcItem, DT_END_ELLIPSIS | DT_CENTER | DT_NOPREFIX);
}

void CXTPGridThemeResource::FillFooter(CDC* pDC, CRect rcFooter)
{
	XTPDrawHelpers()->GradientFill(pDC, rcFooter, m_grcGradientColumn.clrDark,
								   m_grcGradientColumn.clrLight, FALSE);

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrGroupBoxBack);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(1), rcFooter.Width(),
					   m_clrGradientColumnShadow);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(2), rcFooter.Width(),
					   MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.25));
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(3), rcFooter.Width(),
					   MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.1));

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - XTP_DPI_Y(1), rcFooter.Width(),
					   m_clrGradientColumnShadow);
}

void CXTPGridThemeResource::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth,
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
										 + (nShadowWidth ? 1 : 0)));
			return;
		}

		CPoint pt(rcTriangle.CenterPoint());

		if (bToDown)
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - XTP_DPI_X(4), pt.y - XTP_DPI_Y(2)),
									  CPoint(pt.x, pt.y + XTP_DPI_Y(2)),
									  CPoint(pt.x + XTP_DPI_X(4), pt.y - XTP_DPI_Y(2)),
									  m_clrGradientColumnSeparator);
		}
		else
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - XTP_DPI_X(4), pt.y + XTP_DPI_Y(2)),
									  CPoint(pt.x, pt.y - XTP_DPI_Y(2)),
									  CPoint(pt.x + XTP_DPI_X(4), pt.y + XTP_DPI_Y(2)),
									  m_clrGradientColumnSeparator);
		}
	}
}

void CXTPGridThemeResource::DrawPlainColumnBackground(CDC* pDC, CRect rcColumn)
{
	XTPDrawHelpers()->GradientFill(pDC, rcColumn, m_grcGradientColumn, FALSE);
	pDC->Draw3dRect(rcColumn, m_clrGradientColumnShadow, m_clrGradientColumnShadow);
}

void CXTPGridThemeResource::DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn,
												 BOOL& bColumnPressed,
												 BOOL& /*bDraggingFromHeader*/,
												 CXTPGridHeader* /*pHeader*/)
{
	XTPDrawHelpers()->GradientFill(pDC, rcColumn,
								   bColumnPressed
									   ? m_grcGradientColumnPushed
									   : pColumn->IsHotTracking() ? m_grcGradientColumnHot
																  : m_grcGradientColumn,
								   FALSE);

	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - XTP_DPI_Y(1), rcColumn.Width(),
					   m_clrGradientColumnShadow);

	if (pColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.right - XTP_DPI_X(1), rcColumn.top + XTP_DPI_Y(2),
						 rcColumn.Height() - XTP_DPI_Y(2) - XTP_DPI_Y(4),
						 m_clrGradientColumnSeparator);

	if (pColumn->GetControl()->IsGroupByVisible())
		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(),
						   m_clrGradientColumnShadow);

	bColumnPressed = FALSE;
}

void CXTPGridThemeResource::DrawColumnFooterDivider(CDC* pDC, CXTPGridColumn* pColumn,
													CRect rcColumn)
{
	if (pColumn->GetDrawFooterDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right, rcColumn.top + XTP_DPI_Y(6),
						 rcColumn.Height() - XTP_DPI_Y(9), m_clrControlLightLight);
		DrawVerticalLine(pDC, rcColumn.right - XTP_DPI_X(1), rcColumn.top + XTP_DPI_Y(5),
						 rcColumn.Height() - XTP_DPI_Y(9), m_clrGradientColumnSeparator);
	}
}

void CXTPGridThemeResource::DrawColumnText(CDC* pDC, CRect rcText, CString strCaption,
										   int nHeaderAlignment, BOOL bIsHeader,
										   BOOL bColumnPressed)
{
	UNREFERENCED_PARAMETER(bIsHeader);
	UNREFERENCED_PARAMETER(bColumnPressed);

	pDC->SetTextColor(m_clrCaptionText);

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

void CXTPGridThemeResource::DrawSectionDivider(CDC* pDC, const CRect& rc,
											   XTPGridSectionDividerPosition dividerPosition,
											   XTPGridSectionDividerStyle dividerStyle,
											   CXTPPaintManagerColor clrDivider)
{
	if (dividerStyle == xtpGridFixedRowsDividerOutlook)
	{
		pDC->FillSolidRect(rc, MixColor(m_grcGradientColumn.clrLight, m_grcGradientColumn.clrDark,
										0.4));
		DrawHorizontalLine(pDC, rc.left, rc.top, rc.Width(), RGB(255, 255, 255));
		DrawHorizontalLine(pDC, rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(),
						   m_clrGradientColumnSeparator);
	}
	else
	{
		CXTPGridPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle,
												 clrDivider);
	}
}
