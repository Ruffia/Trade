// XTPPropertyGridVisualStudio2005Theme.cpp : implementation of the
// CXTPPropertyGridVisualStudio2005Theme class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridItem.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVisualStudio2005Theme

CXTPPropertyGridVisualStudio2005Theme::CXTPPropertyGridVisualStudio2005Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpPropertyGridButtonsThemed;
}

void CXTPPropertyGridVisualStudio2005Theme::DrawCategoryCaptionBackground(
	CDC* pDC, CXTPPropertyGridItem* /*pItem*/, CRect rc)
{
	pDC->FillSolidRect(rc, m_pMetrics->m_clrLine);

	XTPDrawHelpers()->GradientFill(pDC,
								   CRect(rc.left, rc.bottom - XTP_DPI_Y(2), rc.right,
										 rc.bottom - XTP_DPI_Y(1)),
								   m_pMetrics->m_clrLine, GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);
}

void CXTPPropertyGridVisualStudio2005Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrShadow.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
}

void CXTPPropertyGridVisualStudio2005Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem,
															 CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcButton(CPoint(XTP_DPI_X(XTP_PGI_EXPAND_BORDER) / 2 - XTP_DPI_X(5),
						  rcCaption.CenterPoint().y - XTP_DPI_Y(4)),
				   XTP_DPI(CSize(9, 9)));

	if (pItem->GetIndent() > 0)
		rcButton.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1))
								* XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
							0);

	CDC* pDC = &dc;

	const int n1pxX = XTP_DPI_X(1);
	const int n1pxY = XTP_DPI_Y(1);
	const int n9pxX = XTP_DPI_X(9);
	const int n9pxY = XTP_DPI_Y(9);

	COLORREF clrButton = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrFrame  = GetXtremeColor(COLOR_3DSHADOW);
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top + n1pxY, n9pxX - n1pxX * 2,
					   n9pxY - n1pxY * 2, clrButton);

	// borders
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top, n9pxX - n1pxX * 2, n1pxY, clrFrame);
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.bottom - n1pxY, n9pxX - n1pxX * 2, n1pxY,
					   clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + n1pxY, n1pxX, n9pxY - n1pxY * 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - n1pxX, rcButton.top + n1pxY, n1pxX, n9pxY - n1pxY * 2,
					   clrFrame);

	// white blink
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top + n1pxY, n9pxX - n1pxX * 2, XTP_DPI_Y(3),
					   0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top + XTP_DPI_Y(4), n9pxX - XTP_DPI_X(4),
					   n1pxY * 2, 0xFFFFFF);

	// plus/minus
	pDC->FillSolidRect(rcButton.left + n1pxX * 2, rcButton.top + XTP_DPI_Y(4), n9pxX - n1pxX * 4,
					   n1pxY, 0);
	if (!pItem->IsExpanded())
	{
		pDC->FillSolidRect(rcButton.left + XTP_DPI_X(4), rcButton.top + n1pxY * 2, n1pxX,
						   n9pxY - n1pxY * 4, 0);
	}
}

XTPScrollBarTheme CXTPPropertyGridVisualStudio2005Theme::GetScrollBarTheme() const
{
	return xtpScrollBarThemeVisualStudio2005;
}
