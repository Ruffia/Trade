// XTPRibbonMarkupTheme.cpp: implementation of the CXTPRibbonMarkupTheme class.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"

#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonTab.h"
#include "Ribbon/XTPRibbonControlTab.h"
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonSystemButton.h"
#include "Ribbon/XTPRibbonMarkupTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

static const LPCTSTR lpszRibbonThemePart[xtpRibbonThemePartCount] = {
	_T("RibbonFrameCaptionActive.xaml"),
	_T("RibbonFrameCaptionInactive.xaml"),
	_T("RibbonFrameSystemButtonNormal.xaml"),
	_T("RibbonFrameSystemButtonPopuped.xaml"),
	_T("RibbonFrameSystemButtonSelected.xaml"),
	_T("RibbonGroupControlEntry.xaml"),
	_T("RibbonGroupControlEntryLeft.xaml"),
	_T("RibbonGroupControlEntryLeftRight.xaml"),
	_T("RibbonGroupControlEntryRight.xaml"),
	_T("RibbonGroupRect.xaml"),
	_T("RibbonGroupRectBlue.xaml"),
	_T("RibbonGroupRectCyan.xaml"),
	_T("RibbonGroupRectGreen.xaml"),
	_T("RibbonGroupRectOrange.xaml"),
	_T("RibbonGroupRectPurple.xaml"),
	_T("RibbonGroupRectRed.xaml"),
	_T("RibbonGroupRectYellow.xaml"),
	_T("RibbonScrollButtonLeft.xaml"),
	_T("RibbonScrollButtonRight.xaml"),
	_T("RibbonTabHighlighted.xaml"),
	_T("RibbonTabSelected.xaml"),
	_T("RibbonTabSelectedFocused.xaml"),
	_T("RibbonTabSelectedHighlighted.xaml"),
	_T("RibbonContextTabBlue.xaml"),
	_T("RibbonContextTabCyan.xaml"),
	_T("RibbonContextTabGreen.xaml"),
	_T("RibbonContextTabOrange.xaml"),
	_T("RibbonContextTabPurple.xaml"),
	_T("RibbonContextTabRed.xaml"),
	_T("RibbonContextTabYellow.xaml"),
	_T("RibbonTabSelectedBlue.xaml"),
	_T("RibbonTabSelectedFocusedBlue.xaml"),
	_T("RibbonTabSelectedHighlightedBlue.xaml"),
	_T("RibbonTabSelectedCyan.xaml"),
	_T("RibbonTabSelectedFocusedCyan.xaml"),
	_T("RibbonTabSelectedHighlightedCyan.xaml"),
	_T("RibbonTabSelectedGreen.xaml"),
	_T("RibbonTabSelectedFocusedGreen.xaml"),
	_T("RibbonTabSelectedHighlightedGreen.xaml"),
	_T("RibbonTabSelectedOrange.xaml"),
	_T("RibbonTabSelectedFocusedOrange.xaml"),
	_T("RibbonTabSelectedHighlightedOrange.xaml"),
	_T("RibbonTabSelectedPurple.xaml"),
	_T("RibbonTabSelectedFocusedPurple.xaml"),
	_T("RibbonTabSelectedHighlightedPurple.xaml"),
	_T("RibbonTabSelectedRed.xaml"),
	_T("RibbonTabSelectedFocusedRed.xaml"),
	_T("RibbonTabSelectedHighlightedRed.xaml"),
	_T("RibbonTabSelectedYellow.xaml"),
	_T("RibbonTabSelectedFocusedYellow.xaml"),
	_T("RibbonTabSelectedHighlightedYellow.xaml"),
	_T("RibbonFrameCaptionBackstageLeft.xaml"),
	_T("RibbonFrameCaptionBackstageRight.xaml"),
};

static XTPRibbonThemePart GetContextTabThemePartIdFromColor(
	XTPRibbonTabContextColor color, XTPRibbonThemePart defaultPart = xtpRibbonThemePartCount)
{
	XTPRibbonThemePart part = defaultPart;
	switch (color)
	{
		case xtpRibbonTabContextColorBlue: // Blue context color
			part = xtpRibbonContextTabBlue;
			break;
		case xtpRibbonTabContextColorYellow: // Yellow context color
			part = xtpRibbonContextTabYellow;
			break;
		case xtpRibbonTabContextColorGreen: // Green context color
			part = xtpRibbonContextTabGreen;
			break;
		case xtpRibbonTabContextColorRed: // Red context color
			part = xtpRibbonContextTabRed;
			break;
		case xtpRibbonTabContextColorPurple: // Purple context color
			part = xtpRibbonContextTabPurple;
			break;
		case xtpRibbonTabContextColorCyan: // Cyan context color
			part = xtpRibbonContextTabCyan;
			break;
		case xtpRibbonTabContextColorOrange: // Orange context color
			part = xtpRibbonContextTabOrange;
			break;
	}
	return part;
}

static XTPRibbonThemePart GetRibbonTabThemePartIdFromColor(XTPRibbonThemePart part,
														   XTPRibbonTabContextColor color)
{
	ASSERT(xtpRibbonTabSelected == part || xtpRibbonTabSelectedFocused == part
		   || xtpRibbonTabSelectedHighlighted == part || xtpRibbonTabHighlighted == part);

	switch (color)
	{
		case xtpRibbonTabContextColorBlue: // Blue context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedBlue; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedBlue; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedBlue;
					break;
			}
			break;
		case xtpRibbonTabContextColorYellow: // Yellow context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedYellow; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedYellow; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedYellow;
					break;
			}
			break;
		case xtpRibbonTabContextColorGreen: // Green context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedGreen; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedGreen; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedGreen;
					break;
			}
			break;
		case xtpRibbonTabContextColorRed: // Red context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedRed; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedRed; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedRed;
					break;
			}
			break;
		case xtpRibbonTabContextColorPurple: // Purple context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedPurple; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedPurple; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedPurple;
					break;
			}
			break;
		case xtpRibbonTabContextColorCyan: // Cyan context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedCyan; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedCyan; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedCyan;
					break;
			}
			break;
		case xtpRibbonTabContextColorOrange: // Orange context color
			switch (part)
			{
				case xtpRibbonTabSelected: part = xtpRibbonTabSelectedOrange; break;
				case xtpRibbonTabSelectedFocused: part = xtpRibbonTabSelectedFocusedOrange; break;
				case xtpRibbonTabSelectedHighlighted:
					part = xtpRibbonTabSelectedHighlightedOrange;
					break;
			}
			break;
	}
	return part;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPRibbonMarkupTheme::CXTPRibbonMarkupTheme(CXTPPaintManager* pPaintManager)
	: CXTPRibbonPaintManager(pPaintManager)
	// System menu
	, m_pSystemMenuNormal(NULL)
	, m_pSystemMenuSelected(NULL)
	, m_pSystemMenuPopuped(NULL)
	// Group
	, m_pGroupNormal(NULL)
	, m_pGroupHighlighted(NULL)
	// Group pop-up
	, m_pGroupPopupNormal(NULL)
	, m_pGroupPopupPopuped(NULL)
	, m_pGroupPopupSelected(NULL)
	, m_pGroupPopupToolBar(NULL)
	, m_pBackstageBackButton(NULL)
{
	m_bLoaded			   = FALSE;
	m_strHighlightedPrefix = _T("");
	m_strNormalPrefix	  = _T("");

	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);

	m_clrControlGroupOption			  = RGB(0, 0, 0);
	m_clrGroupCaption				  = RGB(0, 0, 0);
	m_clrQuickAccessBelow			  = RGB(0, 0, 0);
	m_clrQuickAccess				  = RGB(0, 0, 0);
	m_clrQuickAccessDisabled		  = RGB(0, 0, 0);
	m_clrQuickAccessButtonHotBack	 = COLORREF_NULL;
	m_clrQuickAccessButtonPressedBack = COLORREF_NULL;
}

CXTPRibbonMarkupTheme::~CXTPRibbonMarkupTheme()
{
	SAFE_DELETE(m_pSystemMenuNormal);
	SAFE_DELETE(m_pSystemMenuSelected);
	SAFE_DELETE(m_pSystemMenuPopuped);
	//
	SAFE_DELETE(m_pGroupNormal);
	SAFE_DELETE(m_pGroupHighlighted);
	//
	SAFE_DELETE(m_pGroupPopupNormal);
	SAFE_DELETE(m_pGroupPopupSelected);
	SAFE_DELETE(m_pGroupPopupPopuped);
	SAFE_DELETE(m_pGroupPopupToolBar);

	SAFE_DELETE(m_pBackstageBackButton);

	XTPMarkupReleaseContext(m_pMarkupContext);
}

IMPLEMENT_DYNAMIC(CXTPRibbonMarkupTheme, CXTPRibbonPaintManager)

void CXTPRibbonMarkupTheme::LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext /*= NULL*/)
{
	if (m_bLoaded)
		return;

	if (NULL == pMarkupContext)
		pMarkupContext = m_pMarkupContext;

	ASSERT(NULL != pMarkupContext);

	BOOL bMarkupLoaded = FALSE;

	for (int iPart = 0; iPart < xtpRibbonThemePartCount; iPart++)
	{
		bMarkupLoaded = m_parts[iPart].LoadPart(hZip, lpszRibbonThemePart[iPart], pMarkupContext);
		ASSERT(bMarkupLoaded);
	}

	if (NULL == m_pSystemMenuNormal)
		m_pSystemMenuNormal = new CXTPRibbonMarkupThemePartSystemMenu();

	if (NULL == m_pSystemMenuSelected)
		m_pSystemMenuSelected = new CXTPRibbonMarkupThemePartSystemMenu();

	if (NULL == m_pSystemMenuPopuped)
		m_pSystemMenuPopuped = new CXTPRibbonMarkupThemePartSystemMenu();

	if (NULL == m_pGroupNormal)
		m_pGroupNormal = new CXTPRibbonMarkupThemePartGroup();

	if (NULL == m_pGroupHighlighted)
		m_pGroupHighlighted = new CXTPRibbonMarkupThemePartGroup();

	if (NULL == m_pGroupPopupNormal)
		m_pGroupPopupNormal = new CXTPRibbonMarkupThemePartGroup();

	if (NULL == m_pGroupPopupPopuped)
		m_pGroupPopupPopuped = new CXTPRibbonMarkupThemePartGroup();

	if (NULL == m_pGroupPopupSelected)
		m_pGroupPopupSelected = new CXTPRibbonMarkupThemePartGroup();

	if (NULL == m_pGroupPopupToolBar)
		m_pGroupPopupToolBar = new CXTPRibbonMarkupThemePartGroup();

	if (NULL == m_pBackstageBackButton)
		m_pBackstageBackButton = new CXTPMarkupThemePart();

	// System pop-up bar
	bMarkupLoaded = m_systemPopupBarEntry.LoadPart(hZip, _T("SystemPopupBarEntry.xaml"),
												   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_systemPopupBarButtonNormal.LoadPart(hZip,
														  _T("SystemPopupBarButtonNormal.xaml"),
														  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_systemPopupBarButtonSelected.LoadPart(hZip,
															_T("SystemPopupBarButtonSelected.xaml"),
															pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pSystemMenuNormal->LoadPart(hZip, _T("RibbonFrameSystemMenuNormal.xaml"),
												  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pSystemMenuSelected->LoadPart(hZip, _T("RibbonFrameSystemMenuSelected.xaml"),
													pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pSystemMenuPopuped->LoadPart(hZip, _T("RibbonFrameSystemMenuPopuped.xaml"),
												   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGroupNormal->LoadPart(hZip, _T("RibbonGroupNormal.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGroupHighlighted->LoadPart(hZip, _T("RibbonGroupHighlighted.xaml"),
												  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGroupPopupNormal->LoadPart(hZip, _T("RibbonGroupPopupNormal.xaml"),
												  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGroupPopupPopuped->LoadPart(hZip, _T("RibbonGroupPopupPopuped.xaml"),
												   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGroupPopupSelected->LoadPart(hZip, _T("RibbonGroupPopupSelected.xaml"),
													pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGroupPopupToolBar->LoadPart(hZip, _T("RibbonGroupPopupToolBar.xaml"),
												   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBackstageBackButton->LoadPart(hZip, _T("RibbonBackstageBackButton.xaml"),
													 pMarkupContext);
	ASSERT(bMarkupLoaded);

	m_bLoaded = TRUE;
	UNREFERENCED_PARAMETER(bMarkupLoaded);
}

void CXTPRibbonMarkupTheme::RenderMarkup(CDC* pDC, CRect rc, LPCTSTR lpszMarkupText)
{
	CXTPMarkupUIElement* pMarkupUIElement = XTPMarkupParseText(m_pMarkupContext, lpszMarkupText);
	if (NULL != pMarkupUIElement)
	{
		ASSERT(pDC);
		XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), rc);
		XTPMarkupReleaseElement(pMarkupUIElement);
	}
}

void CXTPRibbonMarkupTheme::RenderMarkup(CDC* pDC, CRect rc, XTPRibbonThemePart part)
{
	m_parts[part].RenderMarkup(pDC, rc);
}

void CXTPRibbonMarkupTheme::RefreshMetrics()
{
	CXTPRibbonPaintManager::RefreshMetrics();

	m_clrRibbonFace					 = GetSysColor(COLOR_3DFACE);
	m_clrMinimizedFrameEdgeShadow	= GetSysColor(COLOR_3DSHADOW);
	m_clrMinimizedFrameEdgeHighLight = GetSysColor(COLOR_3DHIGHLIGHT);
	m_clrRibbonGroupCaptionText		 = GetSysColor(COLOR_BTNTEXT);
	m_clrQuickAccess				 = GetSysColor(COLOR_BTNTEXT);
	m_clrQuickAccessBelow			 = GetSysColor(COLOR_BTNTEXT);

	m_pTabPaintManager->GetColorSet()->m_clrTextHighlight.SetStandardValue(
		m_clrRibbonGroupCaptionText);
	m_pTabPaintManager->GetColorSet()->m_clrTextNormal.SetStandardValue(
		m_clrRibbonGroupCaptionText);
	m_pTabPaintManager->GetColorSet()->m_clrTextSelected.SetStandardValue(
		m_clrRibbonGroupCaptionText);

	m_clrRecentFileListEdgeShadow	= GetSysColor(COLOR_3DSHADOW);
	m_clrRecentFileListEdgeHighLight = GetSysColor(COLOR_3DHIGHLIGHT);

	m_nGroupSpacing = XTP_DPI_X(2);
	m_bFlatFrame	= FALSE; // Don't extend glass
}

void CXTPRibbonMarkupTheme::DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem)
{
	ASSERT(pItem);
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcEntry(pItem->GetRect());
	CXTPTabPaintManagerTheme::InflateRectEx(rcEntry, CRect(0, XTP_DPI_Y(1), 0, -XTP_DPI_Y(1)),
											tabPosition);

	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();
	UNREFERENCED_PARAMETER(tabColor);

	CXTPRibbonBar* pRibbonBar = pItem->GetRibbonBar();
	ASSERT(pRibbonBar);

	BOOL bSelected	= pItem->IsSelected();
	BOOL bHighlighted = pItem->IsHighlighted();
	BOOL bFocused	 = pItem->IsFocused();

	if (bSelected && pRibbonBar->IsRibbonMinimized())
	{
		CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pItem->GetTabManager();
		if (pControlTab->GetCommandBar() && pControlTab->GetCommandBar()->IsTrackingMode())
		{
			bFocused = bHighlighted = FALSE;
		}
		else if (pControlTab->GetSelected() && bFocused)
		{
			bFocused = bSelected = FALSE;
			bHighlighted		 = TRUE;
		}
		else
		{
			bFocused = bSelected = FALSE;
		}
	}

	if (bSelected || bHighlighted)
	{
		if (bSelected && bFocused)
		{
			const XTPRibbonThemePart part = GetRibbonTabThemePartIdFromColor(
				xtpRibbonTabSelectedFocused, pItem->GetContextColor());
			RenderMarkup(pDC, rcEntry, part);
		}
		else if (bSelected && bHighlighted)
		{
			const XTPRibbonThemePart part = GetRibbonTabThemePartIdFromColor(
				xtpRibbonTabSelectedHighlighted, pItem->GetContextColor());
			RenderMarkup(pDC, rcEntry, part);
		}
		else if (bSelected)
		{
			const XTPRibbonThemePart part =
				GetRibbonTabThemePartIdFromColor(xtpRibbonTabSelected, pItem->GetContextColor());
			RenderMarkup(pDC, rcEntry, part);
		}
		else if (bHighlighted)
		{
			RenderMarkup(pDC, rcEntry, xtpRibbonTabHighlighted);
		}
	}
}

void CXTPRibbonMarkupTheme::FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	ASSERT(pDC);
	ASSERT(pRibbonBar);

	CXTPClientRect rcRibbonClient(pRibbonBar);

	if (pRibbonBar->IsFrameThemeEnabled())
	{
		CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();
		ASSERT(NULL != pFrameHook);
		if (pFrameHook->IsDwmEnabled())
		{
			CRect rcBorder = GetFramePaintManager()->CalcFrameBorder(pFrameHook);
			rcRibbonClient.top += rcBorder.top;
		}

		rcRibbonClient.top += GetRibbonCaptionHeight(pRibbonBar);
	}

	CRect rcRibbonTabs(rcRibbonClient.left, rcRibbonClient.top, rcRibbonClient.right,
					   rcRibbonClient.top + m_nTabsHeight);
	CRect rcRibbonGroups(rcRibbonClient.left, rcRibbonTabs.bottom, rcRibbonClient.right,
						 rcRibbonClient.bottom);

	pDC->FillSolidRect(rcRibbonTabs, m_clrRibbonFace);
	pDC->FillSolidRect(rcRibbonGroups, m_clrRibbonFace);

	if (pRibbonBar->IsQuickAccessBelowRibbon())
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
		pDC->Draw3dRect(rcQuickAccess, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));
	}

	if (!pRibbonBar->IsGroupsVisible() && !pRibbonBar->IsQuickAccessBelowRibbon())
	{
		m_pPaintManager->HorizontalLine(pDC, rcRibbonClient.left,
										rcRibbonClient.bottom - XTP_DPI_Y(2), rcRibbonClient.right,
										m_clrMinimizedFrameEdgeShadow);
		m_pPaintManager->HorizontalLine(pDC, rcRibbonClient.left,
										rcRibbonClient.bottom - XTP_DPI_Y(1), rcRibbonClient.right,
										m_clrMinimizedFrameEdgeHighLight);
	}
}

void CXTPRibbonMarkupTheme::FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups)
{
	ASSERT(pDC);
	UNUSED_ALWAYS(pActiveTab);
	RenderMarkup(pDC, rcGroups, xtpRibbonGroupRect);
}

void CXTPRibbonMarkupTheme::FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup,
												  CXTPCommandBar* pCommandBar)
{
	ASSERT(pDC);
	ASSERT(pGroup);
	UNUSED_ALWAYS(pCommandBar);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(pGroup->GetRect(), m_clrRibbonFace);
}

void CXTPRibbonMarkupTheme::DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup)
{
	ASSERT(pDC);
	ASSERT(pGroup);
	CRect rcGroup(pGroup->GetRect());

	if (!pGroup->IsReduced())
	{
		CString strCaption = pGroup->GetCaption();
		XTPDrawHelpers()->StripMnemonics(strCaption);

		LOGFONT lf;
		m_fontGroupCaption.GetLogFont(&lf);

		if (pGroup->IsHighlighted())
		{
			ASSERT(NULL != m_pGroupHighlighted);
			m_pGroupHighlighted->SetFontFamily(lf.lfFaceName);
			m_pGroupHighlighted->SetFontSize(-lf.lfHeight);
			m_pGroupHighlighted->SetCaption(strCaption);
			m_pGroupHighlighted->SetTextColor(m_clrGroupCaption);
			m_pGroupHighlighted->RenderMarkup(pDC, rcGroup);
		}
		else
		{
			ASSERT(NULL != m_pGroupNormal);
			m_pGroupNormal->SetFontFamily(lf.lfFaceName);
			m_pGroupNormal->SetFontSize(-lf.lfHeight);
			m_pGroupNormal->SetCaption(strCaption);
			m_pGroupNormal->SetTextColor(m_clrGroupCaption);
			m_pGroupNormal->RenderMarkup(pDC, rcGroup);
		}
	}
}

// This method is called to draw ribbon caption if EnableFrameTheme was called
void CXTPRibbonMarkupTheme::DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar,
												   BOOL bActive)
{
	ASSERT(pDC);
	ASSERT(pRibbonBar);
	CRect rcCaption = pRibbonBar->GetCaptionRect();

	if (!pRibbonBar->IsDwmEnabled())
	{
		if (bActive)
		{
			RenderMarkup(pDC, rcCaption, xtpRibbonFrameCaptionActive);
		}
		else
		{
			RenderMarkup(pDC, rcCaption, xtpRibbonFrameCaptionInactive);
		}
	}

	// Frame icon
	CXTPRibbonControlSystemButton* pSystemButton = pRibbonBar->GetSystemButton();

	if ((NULL == pSystemButton) || (xtpButtonCaption == pSystemButton->GetStyle()))
	{
		m_pPaintManager->GetFramePaintManager()->DrawFrameIcon(pDC, pRibbonBar->GetFrameHook(),
															   rcCaption);
	}

	DrawRibbonQuickAccessControls(pDC, pRibbonBar);

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	ASSERT(NULL != pContextHeaders);
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}

	CRect rcCaptionText = pRibbonBar->GetCaptionTextRect();

	CXTPFontDC font(pDC, &m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption);

	CWnd* pSite = pRibbonBar->GetSite();
	ASSERT(pSite);

	CString strWindowText;
	pSite->GetWindowText(strWindowText);

	if (pSite->GetStyle() & WS_MAXIMIZE)
	{
		rcCaptionText.top += pRibbonBar->GetFrameHook()->CalcFrameBorder().top / 2;
	}

	if (!pRibbonBar->IsDwmEnabled())
	{
		m_pPaintManager->GetFramePaintManager()->DrawCaptionText(pDC, rcCaptionText, pSite, bActive,
																 pRibbonBar);
	}
	else
	{
		DrawDwmCaptionText(pDC, rcCaptionText, strWindowText, pSite, bActive);
	}
}

// This method is called to draw ribbon caption if EnableFrameTheme was not called
void CXTPRibbonMarkupTheme::DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar,
													  BOOL bActive)
{
	ASSERT(pDC);
	ASSERT(pRibbonBar);

	CRect rcCaption = pRibbonBar->GetCaptionRect();
	rcCaption.top--;

	if (!pRibbonBar->IsDwmEnabled())
	{
		if (bActive)
		{
			RenderMarkup(pDC, rcCaption, xtpRibbonFrameCaptionActive);
		}
		else
		{
			RenderMarkup(pDC, rcCaption, xtpRibbonFrameCaptionInactive);
		}
	}

	DrawRibbonQuickAccessControls(pDC, pRibbonBar);

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	ASSERT(NULL != pContextHeaders);
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}
}

CSize CXTPRibbonMarkupTheme::DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption,
													BOOL bDraw)
{
	ASSERT(pDC);
	ASSERT(pControlGroupOption);

	if (bDraw)
	{
		CRect rc = pControlGroupOption->GetRect();

		m_pPaintManager->DrawRectangle(pDC, rc, pControlGroupOption->GetSelected(),
									   pControlGroupOption->GetPressed(), TRUE, FALSE, FALSE,
									   xtpBarTypeNormal, xtpBarTop);

		rc = CRect(CPoint((rc.left + rc.right - XTP_DPI_X(6)) / 2,
						  (rc.bottom + rc.top - XTP_DPI_Y(8)) / 2),
				   XTP_DPI(CSize(6, 8)));

		CXTPPenDC pen(*pDC, m_clrControlGroupOption);
		CXTPBrushDC brush(*pDC, m_clrControlGroupOption);
		pDC->MoveTo(rc.left, rc.top + XTP_DPI_Y(5));
		pDC->LineTo(rc.left, rc.top);
		pDC->LineTo(rc.left + XTP_DPI_X(6), rc.top);
		pDC->MoveTo(rc.left + XTP_DPI_X(3), rc.top + XTP_DPI_Y(3));
		pDC->LineTo(rc.left + XTP_DPI_X(6), rc.top + XTP_DPI_Y(6));

		XTPDrawHelpers()->Triangle(pDC, CPoint(rc.left + XTP_DPI_X(3), rc.top + XTP_DPI_Y(6)),
								   CPoint(rc.left + XTP_DPI_X(6), rc.top + XTP_DPI_Y(3)),
								   CPoint(rc.left + XTP_DPI_X(6), rc.top + XTP_DPI_Y(6)));

		pen.Color(0xFFFFFF);
		pDC->MoveTo(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(5));
		pDC->LineTo(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1));
		pDC->LineTo(rc.left + XTP_DPI_X(6), rc.top + XTP_DPI_Y(1));

		pDC->MoveTo(rc.left + XTP_DPI_X(4), rc.top + XTP_DPI_Y(7));
		pDC->LineTo(rc.left + XTP_DPI_X(7), rc.top + XTP_DPI_Y(7));
		pDC->LineTo(rc.left + XTP_DPI_X(7), rc.top + XTP_DPI_Y(2));
		pDC->SetPixel(rc.left + XTP_DPI_X(4), rc.top + XTP_DPI_Y(3), 0xFFFFFF);
	}

	return XTP_DPI(CSize(15, 14));
}

CSize CXTPRibbonMarkupTheme::GetControlGroupOptionSize() const
{
	return XTP_DPI(CSize(8, 8));
}

CSize CXTPRibbonMarkupTheme::DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup,
												   BOOL bDraw)
{
	ASSERT(pDC);
	ASSERT(pControlGroupPopup);
	CXTPRibbonGroup* pRibbonGroup = pControlGroupPopup->GetRibbonGroup();

	if (!pRibbonGroup)
	{
		if (bDraw)
		{
			m_pPaintManager->DrawControlEntry(pDC, pControlGroupPopup);
		}
		return m_pPaintManager->DrawControlToolBarParent(pDC, pControlGroupPopup, bDraw);
	}

	if (bDraw)
	{
		CRect rc = pControlGroupPopup->GetRect();

		if (pControlGroupPopup->GetPopuped())
		{
			ASSERT(NULL != m_pGroupPopupPopuped);
			m_pGroupPopupPopuped->RenderMarkup(pDC, rc);
		}
		else if (pControlGroupPopup->GetSelected())
		{
			ASSERT(NULL != m_pGroupPopupSelected);
			m_pGroupPopupSelected->RenderMarkup(pDC, rc);
		}
		else
		{
			ASSERT(NULL != m_pGroupPopupNormal);
			m_pGroupPopupNormal->RenderMarkup(pDC, rc);
		}
	}
	CSize szButton(max(XTP_DPI_X(42), m_pPaintManager->m_nTextHeight * 2 - XTP_DPI_X(3)),
				   GetEditHeight() * 3);

	CXTPFontDC font(pDC, m_pPaintManager->GetControlFont(pControlGroupPopup));

	if (bDraw)
	{
		CXTPImageManagerIcon* pImageIcon = pControlGroupPopup->GetImage(
			pControlGroupPopup->GetIconSize().cx);
		BOOL bDrawImage = (pImageIcon != NULL);

		CRect rcButton = pControlGroupPopup->GetRect();
		if (bDrawImage)
		{
			CSize sz;
			if (pImageIcon->IsVectorIcon())
			{
				sz = pControlGroupPopup->GetIconSize();
			}
			else
			{
				sz = m_pPaintManager->m_bAutoResizeIcons ? m_pPaintManager->GetAutoIconSize(TRUE)
														 : pImageIcon->GetExtent();
			}

			sz = m_pPaintManager->AdjustDpiIconSize(pImageIcon, sz, pControlGroupPopup,
													rcButton.Size());

			CPoint pt = CPoint(rcButton.CenterPoint().x - sz.cx / 2, rcButton.top + XTP_DPI_Y(4));

			if (sz.cx < XTP_DPI_X(20))
			{
				pt.y = rcButton.top + XTP_DPI_Y(9);
			}

			m_pPaintManager->DrawImage(pDC, pt, sz, pImageIcon, pControlGroupPopup->GetSelected(),
									   pControlGroupPopup->GetPressed(),
									   pControlGroupPopup->GetEnabled(), FALSE,
									   pControlGroupPopup->GetPopuped());
		}

		CSize szText = m_pPaintManager->DrawControlText2(pDC, pControlGroupPopup, CXTPEmptyRect(),
														 FALSE, FALSE, TRUE);

		CRect rcText = rcButton;
		rcText.bottom -= m_nGroupCaptionHeight + XTP_DPI_Y(2);
		rcText.top = rcText.bottom - szText.cy;

		pDC->SetTextColor(m_pPaintManager->GetControlTextColor(pControlGroupPopup));
		m_pPaintManager->DrawControlText2(pDC, pControlGroupPopup, rcText, TRUE, FALSE, TRUE);
	}
	else
	{
		CSize szText = m_pPaintManager->DrawControlText2(pDC, pControlGroupPopup, CXTPEmptyRect(),
														 FALSE, FALSE, TRUE);

		szButton.cx = max(szButton.cx, szText.cx + XTP_DPI_X(5));
	}

	return szButton;
}

void CXTPRibbonMarkupTheme::DrawRibbonQuickAccessControls(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pRibbonBar);
}

void CXTPRibbonMarkupTheme::DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl)
{
	ASSERT(pControl);

	CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControl->GetParent());

	if ((NULL != pRibbon) && (!pRibbon->IsQuickAccessBelowRibbon()))
	{
		if (pControl->GetEnabled())
		{
			if (pControl->GetSelected())
			{
				if (pControl->GetPressed() || pControl->GetPopuped())
					pDC->FillSolidRect(pControl->GetRect(), m_clrQuickAccessButtonPressedBack);
				else
					pDC->FillSolidRect(pControl->GetRect(), m_clrQuickAccessButtonHotBack);
			}
			else if (pControl->GetPressed() || pControl->GetPopuped())
				pDC->FillSolidRect(pControl->GetRect(), m_clrQuickAccessButtonPressedBack);
		}
	}
	else
		m_pPaintManager->DrawRectangle(pDC, pControl->GetRect(), pControl->GetSelected(),
									   pControl->GetPressed(), pControl->GetEnabled(), FALSE,
									   pControl->GetPopuped(), xtpBarTypeNormal, xtpBarTop);

	CRect rc = pControl->GetRect();
	rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	COLORREF color = m_clrQuickAccess;
	if (NULL != pRibbon)
	{
		if (pRibbon->IsQuickAccessBelowRibbon())
			color = m_clrQuickAccessBelow;
	}
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEjectDown, rc, color);
}

void CXTPRibbonMarkupTheme::DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl)
{
	ASSERT(pControl);

	m_pPaintManager->DrawRectangle(pDC, pControl->GetRect(), pControl->GetSelected(),
								   pControl->GetPressed(), pControl->GetEnabled(), FALSE,
								   pControl->GetPopuped(), xtpBarTypeNormal, xtpBarTop);

	CRect rc = pControl->GetRect();
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolRewindRight, rc,
									 GetSysColor(COLOR_3DHIGHLIGHT));

	rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolRewindRight, rc,
									 GetSysColor(COLOR_BTNTEXT));
}

void CXTPRibbonMarkupTheme::FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	ASSERT(pDC);
	ASSERT(pBar);

	CRect rc;
	pBar->GetClientRect(&rc);

	pDC->FillSolidRect(rc, m_clrRibbonFace);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPRibbonMarkupTheme::FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar)
{
	ASSERT(pDC);
	ASSERT(pBar);

	CXTPClientRect rc(pBar);

	m_systemPopupBarEntry.RenderMarkup(pDC, rc);

	CXTPRibbonControlSystemButton* pControlPopup = DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton,
																	pBar->GetControlPopup());
	if (!pControlPopup)
		return;

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControlPopup->GetParent());
	if (!pRibbonBar)
		return;

	CRect rcPopup = pControlPopup->GetRect();
	pControlPopup->GetParent()->ClientToScreen(rcPopup);
	pBar->ScreenToClient(rcPopup);

	if (pRibbonBar->IsCaptionVisible() && CRect().IntersectRect(rcPopup, rc))
	{
		DrawRibbonFrameSystemButton(pDC, pControlPopup, rcPopup);
	}

	for (int i = 0; i < pBar->GetControlCount(); i++)
	{
		CXTPControl* pControl = pBar->GetControl(i);
		if (pControl && pControl->GetWrap())
		{
			CRect rcRow = pControl->GetRowRect();
			pDC->FillSolidRect(rcRow, m_clrRibbonFace);
			break;
		}
	}
}

void CXTPRibbonMarkupTheme::DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton)
{
	ASSERT(pButton);

	if (pButton->GetSelected())
	{
		m_systemPopupBarButtonSelected.RenderMarkup(pDC, pButton->GetRect());
	}
	else
	{
		m_systemPopupBarButtonNormal.RenderMarkup(pDC, pButton->GetRect());
	}

	m_pPaintManager->DrawControlToolBarParent(pDC, pButton, TRUE);
}

void CXTPRibbonMarkupTheme::DrawRibbonFrameSystemButton(CDC* pDC,
														CXTPRibbonControlSystemButton* pControl,
														CRect rc)
{
	ASSERT(pDC);
	ASSERT(pControl);

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControl->GetParent());
	ASSERT(pRibbonBar);

	if ((!pRibbonBar->IsCaptionVisible() && pRibbonBar->GetShowCaptionAlways())
		|| !pRibbonBar->IsTabsVisible())
	{
		m_pPaintManager->DrawControlToolBarParent(pDC, pControl, TRUE);
		return;
	}

	XTPImageState nState = xtpImageNormal;

	if (pControl->GetPopuped())
		nState = xtpImagePressed;
	else if (pControl->GetSelected())
		nState = xtpImageHot;

	CXTPImageManagerIcon* pIcon = pControl->GetImage(32);

	int nOffset = 0;
	if (rc.top < 0)
	{
		nOffset = rc.top;
		rc.OffsetRect(0, -nOffset);
	}

	if (xtpButtonCaption == pControl->GetStyle())
	{
		if (!pRibbonBar->IsCaptionBarInBackstageMode())
		{
			CString strCaption = pControl->GetCaption();
			XTPDrawHelpers()->StripMnemonics(strCaption);

			if (pControl->GetPopuped())
			{
				ASSERT(NULL != m_pSystemMenuPopuped);
				m_pSystemMenuPopuped->SetCaption(strCaption);
				m_pSystemMenuPopuped->RenderMarkup(pDC, rc);
			}
			else if (pControl->GetSelected())
			{
				ASSERT(NULL != m_pSystemMenuSelected);
				m_pSystemMenuSelected->SetCaption(strCaption);
				m_pSystemMenuSelected->RenderMarkup(pDC, rc);
			}
			else
			{
				ASSERT(NULL != m_pSystemMenuNormal);
				m_pSystemMenuNormal->SetCaption(strCaption);
				m_pSystemMenuNormal->RenderMarkup(pDC, rc);
			}
		}
		else
		{
			COLORREF clrBack;
			if (pControl->GetSelected())
				clrBack = m_clrBackstageSysButtonBack2013Highlighted;
			else
				clrBack = m_clrBackstageSysButtonBack2013Normal;

			pDC->FillSolidRect(rc, clrBack);
			CRect rcImage(rc);
			rcImage.DeflateRect(26, XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));

			int h = XTP_DPI_Y(35);

			rcImage.SetRect(rcImage.left - 3, rc.top + XTP_DPI_Y(7), rcImage.left + h - 3,
							rc.top + XTP_DPI_Y(7) + h);

			ASSERT(NULL != m_pBackstageBackButton);
			m_pBackstageBackButton->RenderMarkup(pDC, rcImage);
		}
	}
	else
	{
		CSize szDest(XTP_DPI_X(46), XTP_DPI_Y(45));
		CPoint ptDest((rc.left + rc.right - szDest.cx) / 2,
					  nOffset + (rc.top + rc.bottom - szDest.cy) / 2);

		if (NULL != pIcon)
		{
			if (pControl->GetPopuped())
				RenderMarkup(pDC, CRect(ptDest, szDest), xtpRibbonFrameSystemButtonPopuped);
			else if (pControl->GetSelected())
				RenderMarkup(pDC, CRect(ptDest, szDest), xtpRibbonFrameSystemButtonSelected);
			else
				RenderMarkup(pDC, CRect(ptDest, szDest), xtpRibbonFrameSystemButtonNormal);

			CSize sz;
			if (pIcon->IsVectorIcon())
			{
				sz = XTPSystemMetrics()->GetIconSize();
			}
			else
			{
				sz = pIcon->GetExtent();
				if (CXTPPaintManager::IsDpiBitmapScalingEnabled(pRibbonBar))
					sz = XTP_DPI(sz);
			}

			CPoint ptIcon((rc.left + rc.right - sz.cx) / 2,
						  nOffset + (rc.top + rc.bottom + XTP_DPI_Y(1) - sz.cy) / 2);
			pIcon->m_bDrawComposited = pControl->HasDwmCompositedRect();
			pIcon->Draw(pDC, ptIcon, nState, sz);
			pIcon->m_bDrawComposited = FALSE;
		}
		else
		{
			CWnd* pSite = pControl->GetParent()->GetSite();
			HICON hIcon = m_pPaintManager->GetFramePaintManager()->GetFrameLargeIcon(pSite);
			if (hIcon)
			{
				CSize size = XTPSystemMetrics()->GetIconSize();
				CPoint ptIcon((rc.left + rc.right - size.cx - 1) / 2,
							  nOffset + (rc.top + rc.bottom - size.cy + 1) / 2);

#ifdef _XTP_COMMANDBARS_ACTIVEX
				if (pControl->HasDwmCompositedRect())
					CXTPImageManagerIcon::DrawIconComposited(pDC, ptIcon, size, hIcon);
				else
#endif
					DrawIconEx(pDC->GetSafeHdc(), ptIcon.x, ptIcon.y, hIcon, size.cx, size.cy, NULL,
							   NULL, DI_NORMAL);
			}
		}
	}
}

void CXTPRibbonMarkupTheme::DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl,
												   BOOL bScrollLeft)
{
	ASSERT(pControl);

	CRect rc(pControl->GetRect());
	COLORREF clr = GetSysColor(COLOR_BTNTEXT);

	if (bScrollLeft)
	{
		RenderMarkup(pDC, rc, xtpRibbonScrollButtonLeft);
		XTPPrimitiveDrawer()->DrawSymbol(pDC,
										 CXTPDrawHelpers::IsContextRTL(pDC)
											 ? xtpPrimitiveSymbolPlayRight
											 : xtpPrimitiveSymbolPlayLeft,
										 rc, clr);
	}
	else
	{
		RenderMarkup(pDC, rc, xtpRibbonScrollButtonRight);
		XTPPrimitiveDrawer()->DrawSymbol(pDC,
										 CXTPDrawHelpers::IsContextRTL(pDC)
											 ? xtpPrimitiveSymbolPlayLeft
											 : xtpPrimitiveSymbolPlayRight,
										 rc, clr);
	}
}

BOOL CXTPRibbonMarkupTheme::DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton)
{
	ASSERT(pButton);

	if (pButton->GetRibbonGroup() && pButton->GetRibbonGroup()->IsControlsGrouping())
	{
		if (!pButton->GetSelected())
		{
			BOOL bLeft = pButton->GetBeginGroup() || pButton->GetWrap();

			int nIndexNext = pButton->GetControls()->GetNext(pButton->GetIndex());

			BOOL bRight = nIndexNext <= pButton->GetIndex()
						  || pButton->GetControls()->GetAt(nIndexNext)->GetBeginGroup()
						  || pButton->GetControls()->GetAt(nIndexNext)->GetWrap();

			if (bLeft && bRight)
			{
				RenderMarkup(pDC, pButton->GetRect(), xtpRibbonGroupControlEntryLeftRight);
			}
			else if (bLeft)
			{
				RenderMarkup(pDC, pButton->GetRect(), xtpRibbonGroupControlEntryLeft);
			}
			else if (bRight)
			{
				RenderMarkup(pDC, pButton->GetRect(), xtpRibbonGroupControlEntryRight);
			}
			else
			{
				RenderMarkup(pDC, pButton->GetRect(), xtpRibbonGroupControlEntry);
			}
		}
	}

	return FALSE;
}

void CXTPRibbonMarkupTheme::DrawRibbonGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pRibbonGroup)
{
	ASSERT(pDC);
	ASSERT(pRibbonGroup);

	CXTPRibbonBar* pRibbonBar = pRibbonGroup->GetRibbonBar();
	pRibbonBar->GetRibbonPaintManager()->FillGroupPopupToolBar(pDC, pRibbonGroup, pRibbonBar);
	CRect rcGroup = pRibbonGroup->GetRect();
	m_pGroupPopupToolBar->RenderMarkup(pDC, rcGroup);
	pRibbonGroup->Draw(pDC, rcGroup);
}

CXTPMarkupUIElement* CXTPRibbonMarkupTheme::GetTabContextHeader(CXTPRibbonTabContextHeader* pHeader)
{
	UNUSED_ALWAYS(pHeader);
	return NULL;
}

void CXTPRibbonMarkupTheme::DrawRibbonFrameContextHeaders(
	CDC* pDC, CXTPRibbonBar* pRibbonBar, CXTPRibbonTabContextHeaders* pContextHeaders)
{
	ASSERT(pDC);
	ASSERT(pRibbonBar);
	ASSERT(NULL != pContextHeaders);

	int nCount = pContextHeaders->GetCount();

	CXTPFontDC font(pDC, m_pTabPaintManager->GetFont());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pTabPaintManager->GetColorSet()->m_clrTextNormal);

	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pContextHeaders->GetHeader(i);
		CRect rcHeader(pHeader->m_rcRect);

		const XTPRibbonThemePart part = GetContextTabThemePartIdFromColor(pHeader->m_color);
		if (xtpRibbonThemePartCount != part)
		{
			RenderMarkup(pDC, rcHeader, part);
		}

		int nFrameBorder = pRibbonBar->IsFrameThemeEnabled()
							   ? pRibbonBar->GetFrameHook()->CalcFrameBorder().top
							   : 0;

		rcHeader.top += nFrameBorder - XTP_DPI_Y(2);

		if (pRibbonBar->IsDwmEnabled())
		{
			CSize szText = pDC->GetTextExtent(pHeader->m_strCaption);
			if ((rcHeader.Width() - XTP_DPI_X(10)) < szText.cx)
			{
				szText.cx = rcHeader.Width() - XTP_DPI_X(10);
			}

			rcHeader.left += ((rcHeader.Width() / 2) - (szText.cx / 2));

			DrawDwmCaptionText(pDC, rcHeader, pHeader->m_strCaption, pRibbonBar->GetSite(),
							   pRibbonBar->GetFrameHook()->IsFrameActive());
		}
		else
		{
			pDC->DrawText(pHeader->m_strCaption, rcHeader,
						  DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS
							  | DT_LEFT);
		}
	}
}

void CXTPRibbonMarkupTheme::FillRibbonTabControl(CDC* pDC, CXTPRibbonBar* pRibbonBar,
												 CRect rcControl)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pRibbonBar);
	UNREFERENCED_PARAMETER(rcControl);
}
