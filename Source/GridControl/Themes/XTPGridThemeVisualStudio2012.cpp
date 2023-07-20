// XTPGridThemeVisualStudio2012.cpp : implementation of the CXTPGridThemeOffice2013 class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/Resource.h"
#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridHyperlink.h"

#include "GridControl/Themes/XTPGridThemeOffice2013.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeVisualStudio2012Light
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeVisualStudio2012Light::CXTPGridThemeVisualStudio2012Light()
{
	m_bMakeUpper = FALSE;
	LoadGlyphs();
}

void CXTPGridThemeVisualStudio2012Light::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	int nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_GRID_GLYPHS_VS2012_LIGHT_12,
												   XTP_IDB_GRID_GLYPHS_VS2012_LIGHT_15,
												   XTP_IDB_GRID_GLYPHS_VS2012_LIGHT_18,
												   XTP_IDB_GRID_GLYPHS_VS2012_LIGHT_24);
	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTPToUInt(nBmpID)));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpGridGlyphCount, CSize(0, 0)); // 11
}

void CXTPGridThemeVisualStudio2012Light::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPGridHyperlinkStyle(RGB(19, 100, 196), xtpGridTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPGridHyperlinkStyle(RGB(51, 153, 255),
														xtpGridTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(246, 246, 246));
	m_clrBtnFacePressed.SetStandardValue(RGB(0, 122, 204));
	m_clrBtnText.SetStandardValue(RGB(30, 30, 30));
	m_clrControlDark.SetStandardValue(RGB(204, 206, 219));
	m_clrControlLightLight.SetStandardValue(m_clrBtnFace);
	m_clrHighlight.SetStandardValue(RGB(51, 153, 255));
	m_clrHighlightText.SetStandardValue(RGB(255, 255, 255));
	m_clrBoldText.SetStandardValue(m_clrBtnText);
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(m_clrBtnText);
	m_clrGroupTextHilite.SetStandardValue(m_clrHighlightText);
	m_clrGroupBack.SetStandardValue(RGB(239, 239, 242));
	m_clrGroupBackHilite.SetStandardValue(m_clrHighlight);
	m_clrSelectedRow.SetStandardValue(m_clrGroupBack);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(230, 231, 232));
	m_clrHotDivider.SetStandardValue(m_clrHighlight);
	m_clrFreezeColsDivider.SetStandardValue(RGB(134, 137, 153));

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnText);
	m_clrCaptionTextPressed.SetStandardValue(m_clrHighlightText);
	m_clrHeaderControl.SetStandardValue(m_clrGroupBack);
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrGroupBack);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);
	m_clrGradientColumnSeparator.SetStandardValue(m_clrBtnText);
	m_clrGlyph.SetStandardValue(m_clrBtnText);
	m_clrGlyphHighlight.SetStandardValue(m_clrHighlightText);
	SetGridColor(m_clrBtnFace);

	m_clrDisableBitmapLightest.SetStandardValue(RGB(250, 250, 250));
	m_clrDisableBitmapDarkest.SetStandardValue(RGB(128, 128, 128));

	m_nGroupGridLineStyle = xtpGridGridSolid;

	m_xtpBrushVeriticalGrid.DeleteObject();
	m_xtpBrushHorizontalGrid.DeleteObject();
	m_xtpBrushTreeStructure.DeleteObject();
	m_xtpBrushGroupGridLineStyle.DeleteObject();

	LOGFONT lf;
	m_xtpFontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);
}

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeVisualStudio2012Dark
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeVisualStudio2012Dark::CXTPGridThemeVisualStudio2012Dark()
{
	m_bMakeUpper = FALSE;
	LoadGlyphs();
}

void CXTPGridThemeVisualStudio2012Dark::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	int nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_GRID_GLYPHS_VS2012_DARK_12,
												   XTP_IDB_GRID_GLYPHS_VS2012_DARK_15,
												   XTP_IDB_GRID_GLYPHS_VS2012_DARK_18,
												   XTP_IDB_GRID_GLYPHS_VS2012_DARK_24);
	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTPToUInt(nBmpID)));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpGridGlyphCount, CSize(0, 0)); // 11
}

void CXTPGridThemeVisualStudio2012Dark::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPGridHyperlinkStyle(RGB(51, 153, 255), xtpGridTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPGridHyperlinkStyle(RGB(255, 255, 255),
														xtpGridTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(37, 37, 38));
	m_clrBtnFacePressed.SetStandardValue(RGB(0, 122, 204));
	m_clrBtnText.SetStandardValue(RGB(153, 153, 153));
	m_clrControlDark.SetStandardValue(RGB(45, 45, 48));
	m_clrControlLightLight.SetStandardValue(m_clrBtnFace);
	m_clrHighlight.SetStandardValue(RGB(51, 153, 255));
	m_clrHighlightText.SetStandardValue(RGB(255, 255, 255));
	m_clrBoldText.SetStandardValue(m_clrBtnText);
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(m_clrBtnText);
	m_clrGroupTextHilite.SetStandardValue(m_clrHighlightText);
	m_clrGroupBack.SetStandardValue(m_clrControlDark);
	m_clrGroupBackHilite.SetStandardValue(m_clrHighlight);
	m_clrSelectedRow.SetStandardValue(m_clrControlDark);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(51, 51, 51));
	m_clrHotDivider.SetStandardValue(m_clrHighlight);
	m_clrFreezeColsDivider.SetStandardValue(m_clrBtnText);

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(RGB(208, 208, 208));
	m_clrCaptionTextPressed.SetStandardValue(m_clrHighlightText);
	m_clrHeaderControl.SetStandardValue(RGB(62, 62, 66));
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);
	m_clrGradientColumnSeparator.SetStandardValue(RGB(241, 241, 241));
	m_clrGlyph.SetStandardValue(m_clrGradientColumnSeparator);
	m_clrGlyphHighlight.SetStandardValue(m_clrHighlightText);
	SetGridColor(m_clrBtnFace);

	m_clrDisableBitmapLightest.SetStandardValue(RGB(100, 100, 100));
	m_clrDisableBitmapDarkest.SetStandardValue(RGB(30, 30, 30));

	m_nGroupGridLineStyle = xtpGridGridSolid;

	m_xtpBrushVeriticalGrid.DeleteObject();
	m_xtpBrushHorizontalGrid.DeleteObject();
	m_xtpBrushTreeStructure.DeleteObject();
	m_xtpBrushGroupGridLineStyle.DeleteObject();

	LOGFONT lf;
	m_xtpFontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);
}

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeVisualStudio2012Blue
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeVisualStudio2012Blue::CXTPGridThemeVisualStudio2012Blue()
{
	m_bMakeUpper = FALSE;
	LoadGlyphs();
}

void CXTPGridThemeVisualStudio2012Blue::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	int nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_GRID_GLYPHS_VS2012_BLUE_12,
												   XTP_IDB_GRID_GLYPHS_VS2012_BLUE_15,
												   XTP_IDB_GRID_GLYPHS_VS2012_BLUE_18,
												   XTP_IDB_GRID_GLYPHS_VS2012_BLUE_24);
	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTPToUInt(nBmpID)));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpGridGlyphCount, CSize(0, 0)); // 11
}

void CXTPGridThemeVisualStudio2012Blue::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPGridHyperlinkStyle(RGB(19, 100, 196), xtpGridTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPGridHyperlinkStyle(RGB(51, 153, 255),
														xtpGridTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(255, 255, 255));
	m_clrBtnFacePressed.SetStandardValue(RGB(255, 240, 208));
	m_clrBtnText.SetStandardValue(RGB(30, 30, 30));
	m_clrControlDark.SetStandardValue(RGB(41, 57, 85));
	m_clrControlLightLight.SetStandardValue(m_clrBtnFace);
	m_clrHighlight.SetStandardValue(RGB(51, 153, 255));
	m_clrHighlightText.SetStandardValue(m_clrBtnFace);
	m_clrBoldText.SetStandardValue(m_clrBtnText);
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(m_clrBtnText);
	m_clrGroupTextHilite.SetStandardValue(m_clrHighlightText);
	m_clrGroupBack.SetStandardValue(RGB(239, 239, 242));
	m_clrGroupBackHilite.SetStandardValue(m_clrHighlight);
	m_clrSelectedRow.SetStandardValue(m_clrGroupBack);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(241, 243, 248));
	m_clrHotDivider.SetStandardValue(m_clrHighlight);
	m_clrFreezeColsDivider.SetStandardValue(RGB(43, 59, 88));

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnFace);
	m_clrCaptionTextPressed.SetStandardValue(RGB(117, 99, 61));
	m_clrHeaderControl.SetStandardValue(RGB(77, 96, 130));
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);
	m_clrGradientColumnSeparator.SetStandardValue(RGB(206, 212, 221));
	m_clrGlyph.SetStandardValue(m_clrGradientColumnSeparator);
	m_clrGlyphHighlight.SetStandardValue(m_clrCaptionTextPressed);
	SetGridColor(m_clrBtnFace);

	m_clrDisableBitmapLightest.SetStandardValue(RGB(250, 250, 250));
	m_clrDisableBitmapDarkest.SetStandardValue(RGB(128, 128, 128));

	m_nGroupGridLineStyle = xtpGridGridSolid;

	m_xtpBrushVeriticalGrid.DeleteObject();
	m_xtpBrushHorizontalGrid.DeleteObject();
	m_xtpBrushTreeStructure.DeleteObject();
	m_xtpBrushGroupGridLineStyle.DeleteObject();

	LOGFONT lf;
	m_xtpFontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);
}
