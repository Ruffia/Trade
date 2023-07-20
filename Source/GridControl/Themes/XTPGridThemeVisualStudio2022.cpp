// XTPGridThemeVisualStudio2022.cpp : implementation of the CXTPGridPaintManager class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#endif

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridHyperlink.h"

#include "GridControl/Themes/XTPGridThemeOffice2013.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2022.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeVisualStudio2022
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeVisualStudio2022::CXTPGridThemeVisualStudio2022()
{
#ifdef _XTP_INCLUDE_CONTROLS
	m_bUseThemedControls = TRUE;
#endif
}

void CXTPGridThemeVisualStudio2022::RefreshMetrics()
{
#ifdef _XTP_INCLUDE_CONTROLS
	CXTPGridPaintManager::RefreshMetrics();
#endif

	// default colors located in VISUALSTUDIO2022BLUE_INI.

	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPGridHyperlinkStyle(XTPIniColor(_T("GridControl"), _T("HyperLink"),
															   RGB(19, 100, 196)),
												   xtpGridTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPGridHyperlinkStyle(XTPIniColor(_T("GridControl"),
																	_T("HyperLinkHover"),
																	RGB(51, 153, 255)),
														xtpGridTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BtnFace"),
					RGB(255, 255, 255))); /* grid and separator color between items */
	m_clrBtnFacePressed.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BtnFacePressed"),
					RGB(255, 240, 208))); /* header column pushed color */
	m_clrBtnText.SetStandardValue(XTPIniColor(_T("GridControl"), _T("BtnText"), RGB(30, 30, 30)));
	m_clrControlDark.SetStandardValue(XTPIniColor(_T("GridControl"), _T("ControlDark"),
												  RGB(41, 57, 85))); /* header border color */
	m_clrControlLightLight.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("ControlLightLight"), RGB(255, 255, 255)));
	m_clrHighlight.SetStandardValue(XTPIniColor(_T("GridControl"), _T("Highlight"),
												RGB(51, 153, 255))); /* row back selected color */
	m_clrHighlightText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("HighlightText"),
					RGB(255, 255, 255))); /* row text selected color */
	m_clrBoldText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BoldText"), RGB(30, 30, 30))); /* row text bold color */
	m_clrBoldTextHighlight.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("BoldTextHighlight"),
					RGB(255, 255, 255))); /* row text bold selected color */
	m_clrGroupText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupText"), RGB(30, 30, 30))); /* group text color */
	m_clrGroupTextHilite.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupTextHilite"),
					RGB(255, 255, 255))); /* group text selected color */
	m_clrGroupBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupBack"), RGB(239, 239, 242))); /* group back color */
	m_clrGroupBackHilite.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupBackHilite"),
					RGB(51, 153, 255))); /* group back selected color */
	m_clrSelectedRow.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("SelectedRow"), RGB(239, 239, 242)));
	m_clrSelectedRowText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("SelectedRowText"), RGB(30, 30, 30)));
	m_clrItemShade.SetStandardValue(XTPIniColor(_T("GridControl"), _T("ItemShade"),
												RGB(241, 243, 248))); /* sorted column back color */
	m_clrHotDivider.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("HotDivider"),
					RGB(255, 0, 0))); /* red arrow indicator when moving columns */
	m_clrFreezeColsDivider.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("FreezeColsDivider"), RGB(43, 59, 88)));
	m_clrWindowText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("WindowText"), RGB(30, 30, 30))); /* row text color */
	m_clrControlBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("ControlBack"), RGB(255, 255, 255))); /* row back color */
	m_clrConnector.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("Connector"),
					RGB(77, 96, 130))); /* group-by connector line color */
	m_clrIndentControl.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("IndentControl"), RGB(255, 255, 255)));
	m_clrCaptionText.SetStandardValue(XTPIniColor(_T("GridControl"), _T("CaptionText"),
												  RGB(255, 255, 255))); /* header text color */
	m_clrCaptionTextPressed.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("CaptionTextPressed"),
					RGB(117, 99, 61))); /* header text pushed color */
	m_clrHeaderControl.SetStandardValue(XTPIniColor(_T("GridControl"), _T("HeaderControl"),
													RGB(77, 96, 130))); /* header back color */
	m_clrGroupShadeBorder.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupShadeBorder"), RGB(41, 57, 85)));
	m_clrGroupShadeBack.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupShadeBack"), RGB(255, 255, 255)));
	m_clrGroupShadeText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupShadeText"), RGB(30, 30, 30)));
	m_clrGroupRowText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GroupRowText"), RGB(41, 57, 85)));
	m_clrGroupBoxBack.SetStandardValue(XTPIniColor(_T("GridControl"), _T("GroupBoxBack"),
												   RGB(41, 57, 85))); /* group-by area back color */
	m_crlNoGroupByText.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("NoGroupByText"),
					RGB(41, 57, 85))); /* group-by area text color */
	m_clrGradientColumnSeparator.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GradientColumnSeparator"), RGB(206, 212, 221)));
	m_clrGlyph.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("Glyph"), RGB(206, 212, 221))); /* sort arrow color */
	m_clrGlyphHighlight.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GlyphHighlight"),
					RGB(117, 99, 61))); /* sort arrow highlight color */
	m_clrGlyphExpand.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GlyphExpand"), m_clrControlBack));
	m_clrGlyphBorder.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GlyphBorder"), m_clrConnector));
	m_clrGlyphContract.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("GlyphContract"), m_clrBtnText));
	m_clrDisableBitmapLightest.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("DisableBitmapLightest"), RGB(250, 250, 250)));
	m_clrDisableBitmapDarkest.SetStandardValue(
		XTPIniColor(_T("GridControl"), _T("DisableBitmapDarkest"), RGB(128, 128, 128)));

	SetGridColor(m_clrBtnFace);

	m_nGroupGridLineStyle = xtpGridGridSolid;

	m_xtpBrushVeriticalGrid.DeleteObject();
	m_xtpBrushHorizontalGrid.DeleteObject();
	m_xtpBrushTreeStructure.DeleteObject();
	m_xtpBrushGroupGridLineStyle.DeleteObject();

	LOGFONT lf;
	m_xtpFontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);

#ifdef _XTP_INCLUDE_CONTROLS
	if (m_bUseThemedControls)
	{
		SetControlsTheme(xtpControlThemeVisualStudio2022);
	}
#endif
}
