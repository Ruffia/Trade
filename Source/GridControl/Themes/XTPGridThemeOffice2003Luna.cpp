// XTPGridThemeOffice2003Luna.cpp : implementation of the CXTPGridPaintManager class.
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
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridControl.h"

#include "GridControl/Themes/XTPGridThemeOffice2003Luna.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeOffice2003Luna
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeOffice2003Luna::CXTPGridThemeOffice2003Luna()
{
	m_columnStyle = xtpGridColumnOffice2003;
}

void CXTPGridThemeOffice2003Luna::RefreshMetrics()
{
	CXTPGridPaintManager::RefreshMetrics();

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
			case xtpSystemThemeBlue:
			case xtpSystemThemeRoyale:
			case xtpSystemThemeAero:
				m_clrGroupShadeBorder.SetStandardValue(RGB(123, 164, 224));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(190, 218, 251));
				m_clrGroupRowText.SetStandardValue(RGB(55, 104, 185));
				SetGridColor(RGB(234, 233, 225));
				break;

			case xtpSystemThemeOlive:
				m_clrGroupShadeBorder.SetStandardValue(RGB(181, 196, 143));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(175, 186, 145));
				m_clrGroupRowText.SetStandardValue(RGB(115, 137, 84));
				SetGridColor(RGB(234, 233, 225));
				break;

			case xtpSystemThemeSilver:
				m_clrGroupShadeBorder.SetStandardValue(RGB(165, 164, 189));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(229, 229, 235));
				m_clrGroupRowText.SetStandardValue(RGB(112, 111, 145));
				SetGridColor(RGB(229, 229, 235));
				break;
		}
	}

	m_grcGradientColumn.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
																	   GetXtremeColor(COLOR_WINDOW),
																	   0xcd),
										 GetXtremeColor(COLOR_3DFACE));
	m_grcGradientColumnHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_grcGradientColumnPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrGradientColumnShadow.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrGradientColumnSeparator.SetStandardValue(GetXtremeColor(XPCOLOR_SEPARATOR));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_grcGradientColumn.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226));
			m_clrGradientColumnShadow.SetStandardValue(RGB(59, 97, 156));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(106, 140, 203));
			m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
			m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
			break;

		case xtpSystemThemeOlive:
			m_grcGradientColumn.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145));
			m_clrGradientColumnShadow.SetStandardValue(RGB(96, 128, 88));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(96, 128, 88));
			m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
			m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
			break;

		case xtpSystemThemeSilver:
			m_grcGradientColumn.SetStandardValue(RGB(243, 244, 250), RGB(153, 151, 181));
			m_clrGradientColumnShadow.SetStandardValue(RGB(124, 124, 148));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(110, 109, 143));
			m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
			m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
			break;
	}
}

void CXTPGridThemeOffice2003Luna::FillFooter(CDC* pDC, CRect rcFooter)
{
	XTPDrawHelpers()->GradientFill(pDC, rcFooter, m_grcGradientColumn.clrDark,
								   m_grcGradientColumn.clrLight, FALSE);

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrControlBack);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(1), rcFooter.Width(),
					   m_clrGradientColumnShadow);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(2), rcFooter.Width(),
					   MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.25));
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + XTP_DPI_Y(3), rcFooter.Width(),
					   MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.1));

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - XTP_DPI_Y(1), rcFooter.Width(),
					   m_clrGradientColumnShadow);
}

BOOL CXTPGridThemeOffice2003Luna::IsColumHotTrackingEnabled() const
{
	return m_bHotTracking;
}

void CXTPGridThemeOffice2003Luna::DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn,
													   CRect rcColumn, BOOL& bColumnPressed,
													   BOOL& /*bDraggingFromHeader*/,
													   CXTPGridHeader* pHeader)
{
	CXTPGridColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1)
										  : NULL;

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

	if (!pPrevColumn || pPrevColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + XTP_DPI_Y(3),
						 rcColumn.Height() - XTP_DPI_Y(2) - XTP_DPI_Y(4), m_clrControlLightLight);

	if (pColumn->GetControl()->IsGroupByVisible())
		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(),
						   m_clrGradientColumnShadow);

	bColumnPressed = FALSE;
}

void CXTPGridThemeOffice2003Luna::DrawColumnFooterDivider(CDC* pDC, CXTPGridColumn* pColumn,
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

void CXTPGridThemeOffice2003Luna::DrawSectionDivider(CDC* pDC, const CRect& rc,
													 XTPGridSectionDividerPosition dividerPosition,
													 XTPGridSectionDividerStyle dividerStyle,
													 CXTPPaintManagerColor clrDivider)
{
	if (dividerStyle == xtpGridFixedRowsDividerOutlook)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcGradientColumn.clrLight,
									   m_grcGradientColumn.clrDark, FALSE);
		DrawHorizontalLine(pDC, rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(),
						   m_clrGradientColumnShadow);
	}
	else
	{
		CXTPGridPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle,
												 clrDivider);
	}
}
