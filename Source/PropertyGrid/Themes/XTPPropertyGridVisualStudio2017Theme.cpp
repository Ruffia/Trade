// XTPPropertyGridVisualStudio2017Theme.cpp : implementation of the
// CXTPPropertyGridVisualStudio2017Theme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#endif

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
// CXTPPropertyGridVisualStudio2017Theme

CXTPPropertyGridVisualStudio2017Theme::CXTPPropertyGridVisualStudio2017Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridVisualStudio2010Theme(pGrid)
{
	m_bThinSplitter = TRUE;
	m_buttonsStyle  = xtpPropertyGridButtonsThemed;

#ifdef _XTP_INCLUDE_CONTROLS
	m_bUseThemedControls = TRUE;
#endif
}

void CXTPPropertyGridVisualStudio2017Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	// Default colors found in VISUALSTUDIO2017BLUE_INI.

	//-----------Colors of backgrounds-----------

	// color of the background items, and too color of the background propertygrid
	m_pMetrics->m_clrBack.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("Back"), RGB(255, 255, 255)));
	// color for description pane back.
	m_pMetrics->m_clrHelpBack.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("BackHelp"), RGB(255, 255, 255)));
	// color of the background for the highlight item.
	m_clrHighlight.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("BackHighlight"), RGB(51, 153, 255)));
	// color of the background for the not highlight item.
	m_clrNonHighlightBack.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("BackNonHighlight"), RGB(255, 255, 255)));

	//-----------Colors of text-----------

	// color of the text items.
	m_pMetrics->m_clrFore.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("Text"), RGB(0, 0, 0)));
	// color for description pane text.
	m_pMetrics->m_clrHelpFore.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("TextHelp"), RGB(0, 0, 0)));
	// Color ot text in Verb pane
	m_pMetrics->m_clrVerbFace.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("TextVerb"), RGB(0, 0, 0)));
	// color of the text groups.
	m_pMetrics->m_clrCategoryFore.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("TextCategory"), RGB(0, 0, 0)));
	// color of the text of the disabled items.
	m_pMetrics->m_clrReadOnlyFore.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("TextReadOnly"), RGB(109, 109, 109)));
	// color of the text for the highlight item.
	m_clrHighlightText.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("TextHighlight"), RGB(255, 255, 255)));
	// color of the text for the not highlight item.
	m_clrNonHighlightText.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("TextNonHighlight"), RGB(0, 0, 0)));

	//---

	// color appears between different panes of propertygrid, such as verb and description.
	m_clrFace.SetStandardValue(XTPIniColor(_T("PropertyGrid"), _T("AreaFill"), RGB(255, 255, 255)));
	// color of the background groups and lines between items.
	m_pMetrics->m_clrLine.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("Grid"), RGB(238, 238, 242)));
	// color the static edge in xtpPRopertyGridBorderClientEdge mode.
	m_clrShadow.SetStandardValue(XTPIniColor(_T("PropertyGrid"), _T("Border"), RGB(142, 155, 188)));
	// color of the separator between groups.
	m_clrCategorySeparator.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("Separator"), RGB(238, 238, 242)));
	// color of the splitter border.
	m_clrSplitter.SetStandardValue(
		XTPIniColor(_T("PropertyGrid"), _T("Splitter"), RGB(238, 238, 242)));

#ifdef _XTP_INCLUDE_CONTROLS
	if (m_bUseThemedControls)
	{
		SetControlsTheme(xtpControlThemeVisualStudio2017);
	}
#endif
}

void CXTPPropertyGridVisualStudio2017Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem,
															 CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcSign(CPoint(XTP_DPI_X(XTP_PGI_EXPAND_BORDER) / 2 - XTP_DPI_X(5),
						rcCaption.CenterPoint().y - XTP_DPI_Y(4)),
				 XTP_DPI(CSize(9, 9)));

	if (pItem->GetIndent() > 0)
		rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1))
							  * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
						  0);

	CPoint pt(rcSign.right - XTP_DPI_X(6), rcSign.top);

	if (pItem->IsExpanded())
	{
		XTPDrawHelpers()->DrawExpandTriangle(&dc, pt, TRUE, m_pMetrics->m_clrFore);
	}
	else
	{
		XTPDrawHelpers()->DrawExpandTriangle(&dc, pt, FALSE, m_pMetrics->m_clrFore,
											 m_pMetrics->m_clrBack);
	}
}

XTPScrollBarTheme CXTPPropertyGridVisualStudio2017Theme::GetScrollBarTheme() const
{
	return xtpScrollBarThemeVisualStudio2017;
}
