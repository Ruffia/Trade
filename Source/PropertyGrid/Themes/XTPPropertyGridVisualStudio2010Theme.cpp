// XTPPropertyGridVisualStudio2010Theme.cpp : implementation of the
// CXTPPropertyGridVisualStudio2010Theme class.
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
// CXTPPropertyGridVisualStudio2010Theme

CXTPPropertyGridVisualStudio2010Theme::CXTPPropertyGridVisualStudio2010Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpPropertyGridButtonsThemed;
}

CXTPPropertyGridVisualStudio2010Theme::~CXTPPropertyGridVisualStudio2010Theme()
{
}

void CXTPPropertyGridVisualStudio2010Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(RGB(240, 240, 240));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(RGB(168, 179, 194));
	m_clrShadow.SetStandardValue(GetSysColor(COLOR_3DSHADOW));
	m_clrCategorySeparator.SetStandardValue(GetSysColor(COLOR_3DFACE));
	m_clrFace.SetStandardValue(RGB(222, 225, 231));
	m_pMetrics->m_clrHelpBack.SetStandardValue(RGB(222, 225, 231));
	m_pMetrics->m_clrHelpFore.SetStandardValue(0);
}

void CXTPPropertyGridVisualStudio2010Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem,
															 CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcSign(CPoint(XTP_DPI_X(XTP_PGI_EXPAND_BORDER) / 2 - XTP_DPI_X(5),
						rcCaption.CenterPoint().y - XTP_DPI_Y(4)),
				 CSize(XTP_DPI_X(9), XTP_DPI_Y(9)));

	if (pItem->GetIndent() > 0)
	{
		rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1))
							  * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
						  0);
	}

	CPoint pt(rcSign.right - XTP_DPI_X(6), rcSign.top);

	if (pItem->IsExpanded())
	{
		XTPDrawHelpers()->DrawExpandTriangle(&dc, pt, TRUE, m_pMetrics->m_clrFore);
	}
	else
	{
		XTPDrawHelpers()->DrawExpandTriangle(&dc, pt, FALSE, m_clrShadow, m_pMetrics->m_clrBack);
	}
}

XTPScrollBarTheme CXTPPropertyGridVisualStudio2010Theme::GetScrollBarTheme() const
{
	return xtpScrollBarThemeVisualStudio2010;
}
