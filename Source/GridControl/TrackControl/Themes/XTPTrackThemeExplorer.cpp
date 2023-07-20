// XTPTrackThemeExplorer.cpp : implementation of the CXTPTrackPaintManager class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"

#include "GridControl/TrackControl/XTPTrackPaintManager.h"
#include "GridControl/TrackControl/Themes/XTPTrackThemeExplorer.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeExplorer
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeExplorer::CXTPTrackThemeExplorer()
{
	m_columnStyle = xtpGridColumnExplorer;
}

void CXTPTrackThemeExplorer::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown,
										  int nShadowWidth)
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

static BOOL XTPGridGlyphIsCheckbox(int iGlyph)
{
	return (xtpGridGlyphEnabledUnchecked == iGlyph) || (xtpGridGlyphEnabledChecked == iGlyph)
		   || (xtpGridGlyphDisabledUnchecked == iGlyph) || (xtpGridGlyphDisabledChecked == iGlyph)
		   || (xtpGridGlyphEnabledTristate == iGlyph);
}

CSize CXTPTrackThemeExplorer::DrawGlyph(CDC* pDC, CRect rcColumn, int iGlyph)
{
	CSize size(0, 0);

	if (XTPGridGlyphIsCheckbox(iGlyph) && m_themeButton->IsAppThemeActive())
	{
		m_themeButton->GetThemePartSize(NULL, BP_CHECKBOX, 0, NULL, TS_TRUE, &size);

		if (pDC)
		{
			CRect rectCheckBox(CPoint(rcColumn.left + XTP_DPI_X(1),
									  (rcColumn.top + rcColumn.bottom - size.cy) / 2),
							   size);

			int iState = -1;

			switch (iGlyph)
			{
				case xtpGridGlyphEnabledUnchecked: iState = CBS_UNCHECKEDNORMAL; break;
				case xtpGridGlyphEnabledChecked: iState = CBS_CHECKEDNORMAL; break;
				case xtpGridGlyphDisabledUnchecked: iState = CBS_UNCHECKEDDISABLED; break;
				case xtpGridGlyphDisabledChecked: iState = CBS_CHECKEDDISABLED; break;
				case xtpGridGlyphEnabledTristate: iState = CBS_MIXEDNORMAL; break;
			} // switch

			m_themeButton->DrawThemeBackground(pDC->m_hDC, BP_CHECKBOX, iState, &rectCheckBox,
											   NULL);
		}
	}
	else
	{
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
	}

	return size;
}

BOOL CXTPTrackThemeExplorer::IsColumHotTrackingEnabled() const
{
	return m_bHotTracking && m_themeHeader->IsAppThemeActive();
}

void CXTPTrackThemeExplorer::DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn,
												  BOOL& bColumnPressed,
												  BOOL& /*bDraggingFromHeader*/,
												  CXTPGridHeader* pHeader)
{
	CXTPGridColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1)
										  : NULL;

	if (m_themeHeader->IsAppThemeActive())
	{
		int iState = bColumnPressed ? HIS_PRESSED : pColumn->IsHotTracking() ? HIS_HOT : HIS_NORMAL;

		m_themeHeader->DrawThemeBackground(pDC->GetSafeHdc(), HP_HEADERITEM, iState, &rcColumn,
										   NULL);
	}
	else
	{
		if (bColumnPressed)
		{
			pDC->Draw3dRect(rcColumn, m_clrControlDark, m_clrControlDark);
		}
		else
		{
			pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrBtnText);
			rcColumn.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pDC->Draw3dRect(rcColumn, m_clrHeaderControl, m_clrControlDark);
			if (!pColumn->GetDrawHeaderDivider())
			{
				DrawVerticalLine(pDC, rcColumn.right - XTP_DPI_X(1), rcColumn.top,
								 rcColumn.Height() - XTP_DPI_Y(1), m_clrHeaderControl);
				DrawVerticalLine(pDC, rcColumn.right, rcColumn.top,
								 rcColumn.Height() - XTP_DPI_Y(1), m_clrHeaderControl);
				pDC->SetPixel(rcColumn.right, rcColumn.top - XTP_DPI_Y(1), m_clrControlLightLight);
				pDC->SetPixel(rcColumn.right, rcColumn.bottom - XTP_DPI_Y(1), m_clrControlDark);
			}
			if (pPrevColumn && !pPrevColumn->GetDrawHeaderDivider())
			{
				DrawVerticalLine(pDC, rcColumn.left - XTP_DPI_X(1), rcColumn.top,
								 rcColumn.Height() - XTP_DPI_Y(1), m_clrHeaderControl);
				pDC->SetPixel(rcColumn.left - XTP_DPI_X(1), rcColumn.bottom - XTP_DPI_Y(1),
							  m_clrControlDark);
			}
		}
	}
	bColumnPressed = FALSE;
}

void CXTPTrackThemeExplorer::DrawSectionDivider(CDC* pDC, const CRect& rc,
												XTPGridSectionDividerPosition dividerPosition,
												XTPGridSectionDividerStyle dividerStyle,
												CXTPPaintManagerColor clrDivider)
{
	if (dividerStyle == xtpGridFixedRowsDividerOutlook)
	{
		pDC->FillSolidRect(rc, m_clrHeaderControl);
		pDC->Draw3dRect(rc, m_clrControlLightLight, m_clrBtnText);

		CRect rcTmp(rc);
		rcTmp.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		pDC->Draw3dRect(rcTmp, m_clrHeaderControl, m_clrControlDark);
	}
	else
	{
		CXTPTrackPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle,
												  clrDivider);
	}
}
