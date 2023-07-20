// XTPTrackThemeOffice2003.cpp : implementation of the CXTPTrackPaintManager class.
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

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"

#include "GridControl/TrackControl/XTPTrackPaintManager.h"
#include "GridControl/TrackControl/Themes/XTPTrackThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeOffice2003
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeOffice2003::CXTPTrackThemeOffice2003()
{
	m_columnStyle = xtpGridColumnShaded;
}

void CXTPTrackThemeOffice2003::DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn,
													CRect rcColumn, BOOL& /*bColumnPressed*/,
													BOOL& /*bDraggingFromHeader*/,
													CXTPGridHeader* pHeader)
{
	CXTPGridColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1)
										  : NULL;

	int nShadowWidth = XTP_DPI_Y(4);

	if (pColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.right - XTP_DPI_X(1), rcColumn.top + XTP_DPI_Y(2),
						 rcColumn.Height() - XTP_DPI_Y(2) - nShadowWidth, m_clrControlDark);

	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - XTP_DPI_Y(3), rcColumn.Width(),
					   MixColor(m_clrHeaderControl, m_clrControlDark, 0.25));
	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - XTP_DPI_Y(2), rcColumn.Width(),
					   MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - XTP_DPI_Y(1), rcColumn.Width(),
					   MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));

	if (!pPrevColumn || pPrevColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + XTP_DPI_Y(2),
						 rcColumn.Height() - XTP_DPI_Y(2) - nShadowWidth, m_clrControlLightLight);
}

void CXTPTrackThemeOffice2003::DrawColumn(CDC* pDC, CXTPGridColumn* pColumn,
										  CXTPGridHeader* pHeader, CRect rcColumn,
										  BOOL bDrawExternal, int /*nShadowWidth*/ /*= 0*/)
{
	// save column parameter for future use in drawing
	BOOL bDraggingFromHeader = bDrawExternal && pColumn->IsDragging() && rcColumn.left == 0
							   && rcColumn.top == 0 && pHeader->IsDragHeader();

	BOOL bPlainColumn = bDrawExternal && !bDraggingFromHeader;

	CXTPTrackPaintManager::DrawColumn(pDC, pColumn, pHeader, rcColumn, bDrawExternal,
									  !bPlainColumn ? XTP_DPI_X(4) : 0);
}

void CXTPTrackThemeOffice2003::DrawSectionDivider(CDC* pDC, const CRect& rc,
												  XTPGridSectionDividerPosition dividerPosition,
												  XTPGridSectionDividerStyle dividerStyle,
												  CXTPPaintManagerColor clrDivider)
{
	if (dividerStyle == xtpGridFixedRowsDividerOutlook)
	{
		pDC->FillSolidRect(rc, m_clrHeaderControl);
		DrawHorizontalLine(pDC, rc.left, rc.bottom - XTP_DPI_Y(2), rc.Width(),
						   MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
		DrawHorizontalLine(pDC, rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(),
						   MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));
	}
	else
	{
		CXTPTrackPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle,
												  clrDivider);
	}
}
