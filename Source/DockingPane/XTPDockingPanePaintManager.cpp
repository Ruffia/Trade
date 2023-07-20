// XTPDockingPanePaintManager.cpp : implementation of the CXTPDockingPanePaintManager class.
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

#include "stdafx.h"

#include "GraphicLibrary/unzip/unzip.h"

#include "DockingPane/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPImageManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPane.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPaneSplitterContainer.h"
#include "DockingPane/XTPDockingPaneSidePanel.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"

#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2012.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2013.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2015.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2017.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2019.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2022.h"
#include "DockingPane/TabThemes/XTPDockingPanePanelTheme2012.h"

#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2005.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2013.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2015.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2017.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2019.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2022.h"
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetOfficePanel.h"

#include "DockingPane/XTPDockingPaneIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFX_CDECL XTPDeflateRectDirection(CRect& rc, int left, int top, int right, int bottom,
									   BOOL bVertical)
{
	if (bVertical)
	{
		rc.DeflateRect(top, left, bottom, right);
	}
	else
	{
		rc.DeflateRect(left, top, right, bottom);
	}
}

int AFX_CDECL XTPGetRectDirectionHeight(const CRect& rc, BOOL bVertical)
{
	return bVertical ? rc.Width() : rc.Height();
}

void AFX_CDECL XTPFillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd, CWnd* pParentFrame,
									const CXTPPaintManagerColorGradient& clrHeaderFace, BOOL bHoriz)
{
	ASSERT(NULL != pWnd);
	ASSERT(NULL != pParentFrame);

	COLORREF clrLight = clrHeaderFace.clrDark, clrDark = clrHeaderFace.clrLight;

	CWnd* pFrame = pParentFrame;

	if (XTPColorManager()->IsLowResolution())
	{
		pDC->FillSolidRect(rc, clrDark);
		return;
	}

	if (pFrame)
	{
		CRect rcFrame;
		pFrame->GetWindowRect(&rcFrame);
		pWnd->ScreenToClient(&rcFrame);

		if (bHoriz)
		{
			float fWidth = (float)max(rcFrame.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);

			COLORREF clr1 = XTPDrawHelpers()->BlendColors(clrLight, clrDark,
														  float(rc.left - rcFrame.left) / fWidth);
			COLORREF clr2 = XTPDrawHelpers()->BlendColors(clrLight, clrDark,
														  float(rc.right - rcFrame.left) / fWidth);

			XTPDrawHelpers()->GradientFill(pDC, rc, clr1, clr2, TRUE);
		}
		else
		{
			float fHeight = (float)rcFrame.Height();

			COLORREF clr1 = XTPDrawHelpers()->BlendColors(clrLight, clrDark,
														  float(rc.top - rcFrame.top) / fHeight);
			COLORREF clr2 = XTPDrawHelpers()->BlendColors(clrLight, clrDark,
														  float(rc.bottom - rcFrame.top) / fHeight);

			XTPDrawHelpers()->GradientFill(pDC, rc, clr1, clr2, FALSE);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneTabPaintManager

CXTPTabPaintManagerColorSet* CXTPDockingPaneTabPaintManager::SetColor(XTPTabColorStyle tabColor)
{
	m_tabColor = tabColor;

	switch (tabColor)
	{
		case xtpTabColorVisualStudio2005:
			return SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(TRUE));

		case xtpTabColorOffice2013: return SetColorSet(new CXTPDockingPaneTabColorSet2013());

		case xtpTabColorVisualStudio2012:
		case xtpTabColorVisualStudio2012Dark:
			return SetColorSet(new CXTPDockingPaneTabColorSet2012());

		case xtpTabColorVisualStudio2015: return SetColorSet(new CXTPDockingPaneTabColorSet2015());

		case xtpTabColorVisualStudio2017: return SetColorSet(new CXTPDockingPaneTabColorSet2017());

		case xtpTabColorVisualStudio2019: return SetColorSet(new CXTPDockingPaneTabColorSet2019());

		case xtpTabColorVisualStudio2022: return SetColorSet(new CXTPDockingPaneTabColorSet2022());
	}

	return CXTPTabPaintManager::SetColor(tabColor);
}

CXTPTabPaintManagerTheme*
	CXTPDockingPaneTabPaintManager::SetAppearance(XTPTabAppearanceStyle tabAppearance)
{
	m_tabAppearance = tabAppearance;

	switch (m_tabAppearance)
	{
		case xtpTabAppearanceOffice2013: return SetAppearanceSet(new CXTPDockingPaneTabTheme2013());

		case xtpTabAppearanceVisualStudio2012:
			return SetAppearanceSet(new CXTPDockingPaneTabTheme2012());

		case xtpTabAppearanceVisualStudio2015:
			return SetAppearanceSet(new CXTPDockingPaneTabTheme2015());

		case xtpTabAppearanceVisualStudio2017:
			return SetAppearanceSet(new CXTPDockingPaneTabTheme2017());

		case xtpTabAppearanceVisualStudio2019:
			return SetAppearanceSet(new CXTPDockingPaneTabTheme2019());

		case xtpTabAppearanceVisualStudio2022:
			return SetAppearanceSet(new CXTPDockingPaneTabTheme2022());
	}

	return CXTPTabPaintManager::SetAppearance(tabAppearance);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePanelPaintManager

CXTPTabPaintManagerColorSet* CXTPDockingPanePanelPaintManager::SetColor(XTPTabColorStyle tabColor)
{
	m_tabColor = tabColor;

	switch (tabColor)
	{
		case xtpTabColorVisualStudio2003:
			return SetColorSet(new CXTPDockingPaneTabColorSetOfficePanel());

		case xtpTabColorVisualStudio2005:
			return SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(FALSE));

		case xtpTabColorOffice2013:
		case xtpTabColorVisualStudio2012:
		case xtpTabColorVisualStudio2012Dark:
		case xtpTabColorVisualStudio2015:
		case xtpTabColorVisualStudio2017:
		case xtpTabColorVisualStudio2019:
		case xtpTabColorVisualStudio2022:
			return SetColorSet(new CXTPDockingPanePanelColorSet2012());
	}

	return CXTPTabPaintManager::SetColor(tabColor);
}

CXTPTabPaintManagerTheme*
	CXTPDockingPanePanelPaintManager::SetAppearance(XTPTabAppearanceStyle tabAppearance)
{
	m_tabAppearance = tabAppearance;

	switch (m_tabAppearance)
	{
		case xtpTabAppearanceOffice2013:
		case xtpTabAppearanceVisualStudio2012:
		case xtpTabAppearanceVisualStudio2015:
		case xtpTabAppearanceVisualStudio2017:
		case xtpTabAppearanceVisualStudio2019:
		case xtpTabAppearanceVisualStudio2022:
			return SetAppearanceSet(new CXTPDockingPanePanelTheme2012());
	}

	return CXTPTabPaintManager::SetAppearance(tabAppearance);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager

CXTPDockingPanePaintManager::CXTPDockingPanePaintManager()
	: m_bUseOfficeFont(FALSE)
	, m_bDestroyCustomTitleBrushes(FALSE)
	, m_hbrActiveTitleHorz(NULL)
	, m_hbrInactiveTitleHorz(NULL)
	, m_hbrActiveTitleVert(NULL)
	, m_hbrInactiveTitleVert(NULL)
	, m_strOfficeFont(_T("Tahoma"))
{
	m_themeToolbar  = new CXTPWinThemeWrapper();
	m_themeExplorer = new CXTPWinThemeWrapper();
	m_themeButton   = new CXTPWinThemeWrapper();
	m_themeWindow   = new CXTPWinThemeWrapper();
	m_themeRebar	= new CXTPWinThemeWrapper();

	m_pTabPaintManager = new CXTPDockingPaneTabPaintManager();
	m_pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);
	m_pTabPaintManager->SetPosition(xtpTabPositionBottom);
	m_pTabPaintManager->m_clientFrame   = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_bBoldSelected = FALSE;
	m_pTabPaintManager->EnableToolTips();

	m_pPanelPaintManager = new CXTPDockingPanePanelPaintManager();
	m_pPanelPaintManager->SetLayout(xtpTabLayoutCompressed);
	m_pPanelPaintManager->m_clientFrame		= xtpTabFrameNone;
	m_pPanelPaintManager->m_bBoldSelected   = FALSE;
	m_pPanelPaintManager->m_bInvertGradient = TRUE;
	m_pPanelPaintManager->m_bFillBackground = FALSE;

	m_bAutoHidePanelHighlightSelected = TRUE;

	m_bDrawSingleTab   = FALSE;
	m_nSplitterSize	= XTP_DPI_X(4);
	m_nSplitterIndent  = 0;
	m_bDrawCaptionIcon = FALSE;

	m_rcCaptionMargin.SetRect(0, XTP_DPI_Y(2), 0, XTP_DPI_Y(1));
	m_nCaptionFontGap = XTP_DPI_X(5);

	m_bShowCaption	 = TRUE;
	m_bUseStandardFont = TRUE;

	m_bHighlightActiveCaption = TRUE;
	m_systemTheme			  = xtpSystemThemeDefault;
	m_bUseBoldCaptionFont	 = TRUE;
	m_dwSplitterStyle		  = xtpPaneSplitterFlat;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
	m_nTitleHeight		  = 0;
	m_nTabsHeight		  = 0;
	m_bLunaTheme		  = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();

	m_bAxUseCustomTitleColors = FALSE;
	m_clrAxTitleActiveHorz	= 0;
	m_clrAxTitleInactiveHorz  = 0;
	m_clrAxTitleActiveVert	= 0;
	m_clrAxTitleInactiveVert  = 0;

#endif
}

CXTPDockingPanePaintManager::~CXTPDockingPanePaintManager()
{
	SetCustomTitleBrushes(NULL);

	m_pTabPaintManager->InternalRelease();
	m_pPanelPaintManager->InternalRelease();

	SAFE_DELETE(m_themeToolbar);
	SAFE_DELETE(m_themeExplorer);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeWindow);
	SAFE_DELETE(m_themeRebar);
}

DWORD CXTPDockingPanePaintManager::GetSplitterStyle() const
{
	return m_dwSplitterStyle;
}

void CXTPDockingPanePaintManager::SetSplitterStyle(DWORD dwStyle)
{
	m_dwSplitterStyle = dwStyle;

	m_nSplitterSize = XTP_DPI_X(dwStyle == xtpPaneSplitterFlat ? 4 : 7);
}

XTPCurrentSystemTheme CXTPDockingPanePaintManager::GetCurrentSystemTheme() const
{
	if (m_systemTheme == xtpSystemThemeDefault)
		return XTPColorManager()->IsLunaColorsDisabled()
				   ? xtpSystemThemeUnknown
				   : XTPColorManager()->GetCurrentSystemTheme();

	if (m_systemTheme == xtpSystemThemeAuto)
		return XTPColorManager()->GetWinThemeWrapperTheme();

	return m_systemTheme;
}

void CXTPDockingPanePaintManager::SetLunaTheme(XTPCurrentSystemTheme systemTheme)
{
	m_systemTheme = systemTheme;
	RefreshMetrics();
}

COLORREF CXTPDockingPanePaintManager::GetXtremeColor(UINT nIndex)
{
	if (nIndex > XPCOLOR_LAST)
		return nIndex;
	return m_arrColor[nIndex];
}

void CXTPDockingPanePaintManager::SetColors(int cElements, CONST INT* lpaElements,
											CONST COLORREF* lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrColor[lpaElements[i]] = lpaRgbValues[i];
}

CXTPTabPaintManager* CXTPDockingPanePaintManager::SetTabPaintManager(CXTPTabPaintManager* pManager)
{
	m_pTabPaintManager->InternalRelease();
	m_pTabPaintManager = pManager;

	m_pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);
	m_pTabPaintManager->SetPosition(xtpTabPositionBottom);
	m_pTabPaintManager->m_bBoldSelected = FALSE;
	m_pTabPaintManager->EnableToolTips();

	pManager->RefreshMetrics();
	return pManager;
}

CXTPTabPaintManager* CXTPDockingPanePaintManager::SetPanelPaintManager(CXTPTabPaintManager* pManager)
{
	SAFE_DELETE(m_pPanelPaintManager);
	m_pPanelPaintManager = pManager;

	m_pPanelPaintManager->SetLayout(xtpTabLayoutCompressed);
	m_pPanelPaintManager->m_clientFrame		= xtpTabFrameNone;
	m_pPanelPaintManager->m_bBoldSelected   = FALSE;
	m_pPanelPaintManager->m_bInvertGradient = TRUE;
	m_pPanelPaintManager->m_bFillBackground = FALSE;

	pManager->RefreshMetrics();
	return pManager;
}

void CXTPDockingPanePaintManager::UseOfficeFont(BOOL bUseOfficeFont)
{
	m_bUseOfficeFont = bUseOfficeFont;
	RefreshMetrics();
}

void CXTPDockingPanePaintManager::SetCaptionFontIndirect(LOGFONT* pLogFont,
														 BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	BOOL bUseSystemFont = (pLogFont->lfCharSet > SYMBOL_CHARSET);
	if (m_bUseOfficeFont && !bUseSystemFont && bUseStandardFont
		&& XTPDrawHelpers()->FontExists(_T("Tahoma")))
	{
		STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, _T("Tahoma"));
	}

	m_xtpFontTitle.DeleteObject();
	VERIFY(m_xtpFontTitle.CreateFontIndirect(pLogFont));

	pLogFont->lfOrientation = 900;
	pLogFont->lfEscapement  = 2700;
	pLogFont->lfHeight		= pLogFont->lfHeight < 0 ? __min(-11, pLogFont->lfHeight)
												: pLogFont->lfHeight;
	STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE,
			 CXTPDrawHelpers::GetVerticalFontName(m_bUseOfficeFont));

	m_xtpFontTitleVertical.DeleteObject();
	m_xtpFontTitleVertical.CreateFontIndirect(pLogFont);

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_xtpFontTitle);
	m_nTitleHeight = max(XTP_DPI_Y(13), dc.GetTextExtent(_T(" "), 1).cy) + m_nCaptionFontGap;
}

void CXTPDockingPanePaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	CXTPColorManager* pColorManager = XTPColorManager();

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrColor[i] = pColorManager->GetColor(i);

	m_clrBackground.SetStandardValue(GetSysColor(COLOR_3DFACE));
	m_clrSplitterGripper.SetStandardValue(m_arrColor[COLOR_3DSHADOW]);
	m_clrSplitter.SetStandardValue(m_arrColor[COLOR_3DFACE]);
	m_clrSplitterGradient.SetStandardValue(GetSysColor(COLOR_3DFACE),
										   XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE),
																		 GetSysColor(COLOR_WINDOW),
																		 0xcd));
	m_clrDisabledCaptionText.SetStandardValue(m_arrColor[COLOR_GRAYTEXT]);

	if (m_bUseStandardFont)
	{
		CXTPNonClientMetrics ncm;
		ncm.lfSmCaptionFont.lfWeight  = m_bUseBoldCaptionFont ? FW_BOLD : FW_NORMAL;
		ncm.lfSmCaptionFont.lfCharSet = XTPResourceManager()->GetFontCharset();
		SetCaptionFontIndirect(&ncm.lfSmCaptionFont, TRUE);
	}

	m_pTabPaintManager->RefreshMetrics();
	m_pPanelPaintManager->RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, m_pTabPaintManager->GetFont(FALSE));
	m_nTabsHeight = max(XTP_DPI_Y(24), dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(10));

	m_themeExplorer->OpenThemeData(NULL, L"EXPLORERBAR");
	m_themeButton->OpenThemeData(NULL, L"BUTTON");
	m_themeToolbar->OpenThemeData(NULL, L"TOOLBAR");
	m_themeWindow->OpenThemeData(NULL, L"WINDOW");
	m_themeRebar->OpenThemeData(NULL, L"REBAR");

	m_bLunaTheme = FALSE;
}

void CXTPDockingPanePaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(XTPToUIntChecked(nPen)));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}

void CXTPDockingPanePaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1)
{
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}

void CXTPDockingPanePaintManager::Rectangle(CDC* pDC, CRect rc, int nBrush, int nPen)
{
	pDC->FillSolidRect(rc, GetXtremeColor(XTPToUIntChecked(nBrush)));
	pDC->Draw3dRect(rc, GetXtremeColor(XTPToUIntChecked(nPen)),
					GetXtremeColor(XTPToUIntChecked(nPen)));
}

void CXTPDockingPanePaintManager::Pixel(CDC* pDC, int x, int y, int nPen)
{
	pDC->SetPixel(x, y, GetXtremeColor(XTPToUIntChecked(nPen)));
}

CRect CXTPDockingPanePaintManager::GetCaptionGripperRect(
	const CXTPDockingPaneTabbedContainer* /*pPane*/)
{
	return CRect(0, 0, 0, 0);
}

void CXTPDockingPanePaintManager::DrawCaptionButtons(CDC* pDC,
													 CXTPDockingPaneCaptionButtons* pButtons,
													 CRect& rcCaption, COLORREF clr,
													 int nButtonSize, int nButtonGap,
													 BOOL bVertical)
{
	if (!pButtons)
		return;

	CRect rcButton(bVertical ? CPoint((rcCaption.left + rcCaption.right - nButtonSize) / 2,
									  rcCaption.bottom - nButtonSize)
							 : CPoint(rcCaption.right - nButtonSize,
									  (rcCaption.top + rcCaption.bottom - nButtonSize) / 2),
				   CSize(nButtonSize, nButtonSize));

	for (int i = 0; i < pButtons->GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = pButtons->GetAt(i);

		if (i == 0)
		{
			CXTPDockingPane* pPane = GetSelectedPane(pButton->GetPane());

			if (pPane && ((pPane->GetEnabled() & xtpPaneEnableActions) == 0))
				clr = m_clrDisabledCaptionText;
		}

		if (pButton->IsVisible())
		{
			pButton->SetRect(rcButton);
			DrawCaptionButton(pDC, pButton, clr);

			if (bVertical)
			{
				rcButton.OffsetRect(0, -nButtonSize - nButtonGap);
			}
			else
			{
				rcButton.OffsetRect(-nButtonSize - nButtonGap, 0);
			}
		}
		else
		{
			pButton->SetRect(CXTPEmptyRect());
		}
	}
	if (bVertical)
	{
		rcCaption.bottom = rcButton.bottom;
	}
	else
	{
		rcCaption.right = rcButton.right;
	}
}

void CXTPDockingPanePaintManager::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton,
													COLORREF clrButton)
{
	CRect rc = pButton->GetRect();
	CSize szOffset(0, 0);

	XTPDockingPaneCaptionButtonStyle nCaptionButtonStyle = m_nCaptionButtonStyle;

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedToolBar)
	{
		if (!m_themeToolbar->IsAppThemeActive())
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			if (pButton->IsPressed())
			{
				m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_PRESSED, &rc,
													0);
			}
			else if (pButton->IsSelected())
			{
				m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_HOT, &rc, 0);
			}
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedToolWindow)
	{
		if (!m_themeWindow->IsAppThemeActive() || pButton->GetID() != XTP_IDS_DOCKINGPANE_CLOSE)
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			BOOL bActive = TRUE;
			if (pButton->GetPane()->GetType() == xtpPaneTypeTabbedContainer)
				bActive = ((CXTPDockingPaneTabbedContainer*)pButton->GetPane())->IsActive();
			if (pButton->GetPane()->GetType() == xtpPaneTypeMiniWnd)
				bActive = ((CXTPDockingPaneMiniWnd*)pButton->GetPane())->IsActive();

			int nState = pButton->IsPressed()
							 ? CBS_PUSHED
							 : pButton->IsSelected() ? CBS_HOT : !bActive ? 5 : CBS_NORMAL;
			m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLCLOSEBUTTON, nState, &rc,
											   0);
			return;
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedExplorerBar)
	{
		if (!m_themeExplorer->IsAppThemeActive())
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE
				|| pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
			{
				BOOL bActive = TRUE;
				if (pButton->GetPane()->GetType() == xtpPaneTypeTabbedContainer)
					bActive = ((CXTPDockingPaneTabbedContainer*)pButton->GetPane())->IsActive();
				if (pButton->GetPane()->GetType() == xtpPaneTypeMiniWnd)
					bActive = ((CXTPDockingPaneMiniWnd*)pButton->GetPane())->IsActive();

				bActive = m_bHighlightActiveCaption && bActive;

				if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
				{
					int nState = pButton->IsPressed()
									 ? EBHC_PRESSED
									 : bActive || pButton->IsSelected() ? EBHC_HOT : EBHC_NORMAL;
					if (SUCCEEDED(m_themeExplorer->DrawThemeBackground(*pDC, EBP_HEADERCLOSE,
																	   nState, pButton->GetRect(),
																	   0)))
						return;
				}
				else
				{
					int nState = pButton->IsPressed()
									 ? EBHP_PRESSED
									 : pButton->IsSelected() ? EBHP_HOT : EBHP_NORMAL;
					if (SUCCEEDED(m_themeExplorer->DrawThemeBackground(
							*pDC, EBP_HEADERPIN,
							(pButton->GetState() & xtpPanePinPushed ? 0 : 3) + nState,
							pButton->GetRect(), 0)))
						return;
				}
			}

			nCaptionButtonStyle = xtpPaneCaptionButtonThemedButton;
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedButton)
	{
		if (!m_themeButton->IsAppThemeActive())
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			int nState = pButton->IsPressed() ? PBS_PRESSED
											  : pButton->IsSelected() ? PBS_HOT : PBS_NORMAL;
			if (nState != PBS_NORMAL)
			{
				m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON, nState, pButton->GetRect(),
												   0);
				m_themeButton->GetThemeColor(BP_PUSHBUTTON, nState, TMT_TEXTCOLOR, &clrButton);
			}
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonDefault)
	{
		pDC->DrawFrameControl(&rc, DFC_BUTTON,
							  UINT(DFCS_ADJUSTRECT | DFCS_BUTTONPUSH
								   | (pButton->IsPressed() ? DFCS_PUSHED : 0)));

		if (pButton->IsPressed())
		{
			szOffset = CSize(XTP_DPI_X(1), XTP_DPI_Y(1));
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonOffice)
	{
		if (pButton->IsPressed())
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_BTNHILIGHT));
			szOffset = CSize(XTP_DPI_X(1), XTP_DPI_Y(1));
		}
		else if (pButton->IsSelected())
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_BTNTEXT));
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonOffice2003)
	{
		if (pButton->IsSelected())
		{
			if (!m_bLunaTheme)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER);
				clrButton = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 242, 200), RGB(255, 212, 151),
											   FALSE);
				pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
								GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
				clrButton = RGB(0, 0, 0);
			}
		}
		if (pButton->IsPressed())
		{
			if (!m_bLunaTheme)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_BORDER);
				clrButton = GetXtremeColor(XPCOLOR_PUSHED_TEXT);
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(254, 142, 75), RGB(255, 207, 139),
											   FALSE);
				pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
								GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
				clrButton = RGB(0, 0, 0);
			}
		}
	}

	DrawCaptionButtonEntry(pDC, pButton, clrButton, szOffset);
}

void CXTPDockingPanePaintManager::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
										   CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	pDC->SetBkMode(TRANSPARENT);

	CRect rcCaption(rc);

	if (pPane->IsTabsVisible())
	{
		AdjustCaptionRect(pPane, rc);
		m_pTabPaintManager->DrawTabControl(pPane, pDC, rc);
	}

	if (pPane->IsTitleVisible())
	{
		DrawCaption(pDC, pPane, rcCaption);
	}
}

void CXTPDockingPanePaintManager::GetCaptionRect(const CXTPDockingPaneTabbedContainer* pPane,
												 CRect& rc)
{
	if (pPane->IsTitleVisible())
	{
		GetClientRect(pPane->GetSafeHwnd(), rc);

		if (pPane->IsCaptionVertical())
		{
			rc.right = rc.left + m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
		else
		{
			rc.bottom = rc.top + m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
	}
	else
	{
		rc.SetRectEmpty();
	}
}

void CXTPDockingPanePaintManager::AdjustCaptionRect(const CXTPDockingPaneTabbedContainer* pPane,
													CRect& rc)
{
	if (pPane->IsTitleVisible())
	{
		if (pPane->IsCaptionVertical())
		{
			rc.left += m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
		else
		{
			rc.top += m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
	}
}

void CXTPDockingPanePaintManager::AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane,
												   CRect& rect, BOOL bApply)
{
	CPoint pt = rect.TopLeft();
	AdjustCaptionRect(pPane, rect);

	if (pPane->IsTabsVisible())
	{
		if (bApply)
		{
			CClientDC dc(pPane);

			CRect rcTabs(rect);
			rcTabs.OffsetRect(-pt);

			m_pTabPaintManager->RepositionTabControl(pPane, &dc, rcTabs);
		}
		m_pTabPaintManager->AdjustClientRect(pPane, rect);
	}
}

void CXTPDockingPanePaintManager::AdjustClientRect(CXTPDockingPaneSidePanel* pPane, CRect& rect)
{
	rect.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
	AdjustCaptionRect(pPane, rect);
}

void CXTPDockingPanePaintManager::AdjustCaptionRect(const CXTPDockingPaneSidePanel* pPane,
													CRect& rect)
{
	int nTitleFont					  = GetCaptionHeight();
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft)
		rect.left += nTitleFont;
	else if (direction == xtpPaneDockRight)
		rect.right -= nTitleFont;
	else if (direction == xtpPaneDockTop)
		rect.top += nTitleFont;
	else if (direction == xtpPaneDockBottom)
		rect.bottom -= nTitleFont;
}

CRect CXTPDockingPanePaintManager::GetPaneCaptionRect(const CXTPDockingPaneSidePanel* pPane)
{
	CXTPClientRect rect(pPane);
	rect.DeflateRect(1, 1);

	int nCaptionHeight				  = GetCaptionHeight();
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (xtpPaneDockLeft == direction)
		rect.right = rect.left + nCaptionHeight;
	else if (xtpPaneDockRight == direction)
		rect.left = rect.right - nCaptionHeight;
	else if (xtpPaneDockTop == direction)
		rect.bottom = rect.top + nCaptionHeight;
	else if (xtpPaneDockBottom == direction)
		rect.top = rect.bottom - nCaptionHeight;

	return rect;
}

void CXTPDockingPanePaintManager::DrawSplitter(CDC* pDC, CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc(pSplitter);

	if (m_dwSplitterStyle & xtpPaneSplitter3D)
	{
		pDC->FillSolidRect(rc, m_clrSplitter);

		if (pSplitter->IsHorizontal())
		{
			pDC->FillSolidRect(0, 0, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->FillSolidRect(0, rc.bottom - XTP_DPI_Y(2), rc.Width(), XTP_DPI_Y(1),
							   GetXtremeColor(XPCOLOR_3DSHADOW));
			pDC->FillSolidRect(0, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   GetXtremeColor(COLOR_BTNTEXT));
		}
		else
		{
			pDC->FillSolidRect(0, 0, 1, rc.Height(), GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->FillSolidRect(rc.right - XTP_DPI_X(2), 0, XTP_DPI_X(1), rc.Height(),
							   GetXtremeColor(XPCOLOR_3DSHADOW));
			pDC->FillSolidRect(rc.right - XTP_DPI_X(1), 0, XTP_DPI_X(1), rc.Height(),
							   GetXtremeColor(COLOR_BTNTEXT));
		}
	}
	else if (m_dwSplitterStyle & xtpPaneSplitterSoft3D)
	{
		int nCenter = m_nSplitterSize / 2;

		if (pSplitter->IsHorizontal())
		{
			pDC->FillSolidRect(0, 0, rc.Width(), XTP_DPI_Y(1), GetXtremeColor(COLOR_3DDKSHADOW));
			pDC->FillSolidRect(0, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   GetXtremeColor(COLOR_3DDKSHADOW));

			pDC->FillSolidRect(0, XTP_DPI_Y(1), rc.Width(), nCenter - XTP_DPI_Y(1),
							   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(0, nCenter, rc.Width(), rc.Height() - nCenter - XTP_DPI_Y(1),
							   m_clrSplitter);
		}
		else
		{
			pDC->FillSolidRect(0, 0, XTP_DPI_X(1), rc.Height(), GetXtremeColor(COLOR_3DDKSHADOW));
			pDC->FillSolidRect(rc.right - XTP_DPI_X(1), 0, XTP_DPI_X(1), rc.Height(),
							   GetXtremeColor(COLOR_3DDKSHADOW));

			pDC->FillSolidRect(XTP_DPI_X(1), 0, nCenter - XTP_DPI_X(1), rc.Height(),
							   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(nCenter, 0, rc.Width() - nCenter - XTP_DPI_X(1), rc.Height(),
							   m_clrSplitter);
		}
	}
	else if (m_dwSplitterStyle & xtpPaneSplitterGradient)
	{
		XTPFillFramePartRect(pDC, rc, (CWnd*)pSplitter, pSplitter->GetParent(),
							 m_clrSplitterGradient, TRUE);
	}
	else if (m_dwSplitterStyle & xtpPaneSplitterGradientVert)
	{
		XTPFillFramePartRect(pDC, rc, (CWnd*)pSplitter, pSplitter->GetParent(),
							 m_clrSplitterGradient, FALSE);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrSplitter);
	}

	if (m_dwSplitterStyle & xtpPaneSplitterGripperOfficeXP)
	{
		if (pSplitter->IsHorizontal())
		{
			pDC->FillSolidRect(rc.CenterPoint().x, 0 + XTP_DPI_Y(2), XTP_DPI_X(11), XTP_DPI_Y(1),
							   m_clrSplitterGripper);
			pDC->FillSolidRect(rc.CenterPoint().x, 0 + XTP_DPI_Y(4), XTP_DPI_X(11), XTP_DPI_Y(1),
							   m_clrSplitterGripper);
		}
		else
		{
			pDC->FillSolidRect(XTP_DPI_X(2), rc.CenterPoint().y, XTP_DPI_X(1), XTP_DPI_Y(11),
							   m_clrSplitterGripper);
			pDC->FillSolidRect(XTP_DPI_X(4), rc.CenterPoint().y, XTP_DPI_X(1), XTP_DPI_Y(11),
							   m_clrSplitterGripper);
		}
	}
	if (m_dwSplitterStyle & xtpPaneSplitterGripperOffice2003)
	{
		if (pSplitter->IsHorizontal())
		{
			int nLeft  = rc.CenterPoint().x - min(XTP_DPI_X(32), rc.Width() / 2 - XTP_DPI_X(10));
			int nRight = rc.CenterPoint().x + min(XTP_DPI_X(32), rc.Width() / 2 - XTP_DPI_X(10));
			int y	  = rc.CenterPoint().y - XTP_DPI_Y(1);

			for (int x = nLeft; x < nRight; x += XTP_DPI_X(4))
			{
				pDC->FillSolidRect(CRect(x + 1, y + 1, x + 3, y + 3),
								   GetXtremeColor(COLOR_BTNHIGHLIGHT));
				pDC->FillSolidRect(CRect(x, y, x + 2, y + 2), m_clrSplitterGripper);
			}
		}
		else
		{
			int nTop	= rc.CenterPoint().y - min(XTP_DPI_Y(32), rc.Height() / 2 - XTP_DPI_Y(10));
			int nBottom = rc.CenterPoint().y + min(XTP_DPI_Y(32), rc.Height() / 2 - XTP_DPI_Y(10));
			int x		= rc.CenterPoint().x - XTP_DPI_X(1);

			for (int y = nTop; y < nBottom; y += XTP_DPI_Y(4))
			{
				pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), y + XTP_DPI_Y(1), x + XTP_DPI_X(3),
										 y + XTP_DPI_Y(3)),
								   GetXtremeColor(COLOR_BTNHIGHLIGHT));
				pDC->FillSolidRect(CRect(x, y, x + XTP_DPI_X(2), y + XTP_DPI_Y(2)),
								   m_clrSplitterGripper);
			}
		}
	}
}

void CXTPDockingPanePaintManager::DrawCaptionButtonEntry(CDC* pDC,
														 CXTPDockingPaneCaptionButton* pButton,
														 CPoint pt, COLORREF clrButton)
{
	ASSERT(NULL != pButton);

	CPoint ptCenter = pButton->GetRect().CenterPoint();
	CSize szOffset(pt.x - ptCenter.x, pt.y - ptCenter.y);
	DrawCaptionButtonEntry(pDC, pButton, clrButton, szOffset);
}

void CXTPDockingPanePaintManager::DrawCaptionButtonEntry(CDC* pDC,
														 CXTPDockingPaneCaptionButton* pButton,
														 COLORREF clrButton, CSize szOffset)
{
	CRect rc = pButton->GetRect();
	rc.OffsetRect(szOffset);

	BOOL bPinned = (pButton->GetState() & xtpPanePinPushed) == xtpPanePinPushed;

	switch (pButton->GetID())
	{
		case XTP_IDS_DOCKINGPANE_CLOSE:
			rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));
			CXTPDockingPaneCaptionButton::DrawCloseButton(pDC, rc, clrButton);
			break;
		case XTP_IDS_DOCKINGPANE_AUTOHIDE:
			bPinned ? rc.DeflateRect(0, XTP_DPI_Y(1))
					: rc.DeflateRect(XTP_DPI_X(-1), XTP_DPI_Y(1), 0, 0);
			CXTPDockingPaneCaptionButton::DrawPinButton(pDC, rc, bPinned, clrButton);
			break;
		case XTP_IDS_DOCKINGPANE_MAXIMIZE:
			rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(pDC, rc, TRUE, clrButton);
			break;
		case XTP_IDS_DOCKINGPANE_RESTORE:
			rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(pDC, rc, FALSE, clrButton);
			break;
		case XTP_IDS_DOCKINGPANE_MENU:
			rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			CXTPDockingPaneCaptionButton::DrawMenuButton(pDC, rc, clrButton);
			break;
		default: ASSERT(!"Unexpected button identifier"); break;
	}
}

int CXTPDockingPanePaintManager::DrawCaptionText(CDC* pDC, const CString& strTitle, CRect rcText,
												 BOOL bVertical, BOOL bCalcWidth)
{
	if (strTitle.IsEmpty())
	{
		return 0;
	}

	CXTPFontDC font(pDC, bVertical ? &m_xtpFontTitleVertical : &m_xtpFontTitle);

	CString strTitle2(strTitle);
	CSize szText = pDC->GetTextExtent(strTitle);

	UINT uFormat = DT_LEFT | DT_SINGLELINE | DT_NOPREFIX;

	if (bVertical)
	{
		if (rcText.top > rcText.bottom)
		{
			return 0;
		}

		//	uFormat |= DT_END_ELLIPSIS;	//doesn't work with rotated text

		ClipAndAddEllipsisIfNeeded(pDC, strTitle2, rcText.Height());

		// R and L are exchanged to get correct Rect for drawing rotated text
		int r = rcText.left + ((rcText.Width() - szText.cy) / 2);
		int l = r + szText.cy;
		int t = rcText.top;
		int b = min(t + szText.cx + XTP_DPI_Y(1), rcText.bottom);

		rcText.CopyRect(CRect(l, t, r, b));
	}
	else
	{
		if (rcText.left > rcText.right)
		{
			return 0;
		}

		uFormat |= DT_VCENTER | DT_END_ELLIPSIS;

		int t = rcText.top + ((rcText.Height() - szText.cy) / 2);
		int b = t + szText.cy;
		int l = rcText.left;
		int r = min(l + szText.cx + XTP_DPI_X(1), rcText.right);

		rcText.CopyRect(CRect(l, t, r, b));
	}

	CXTPDCSaveState dcState(pDC);

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strTitle2, rcText, uFormat);

	if (bCalcWidth)
	{
		pDC->DrawText(strTitle2, rcText, uFormat | DT_CALCRECT);
		return rcText.Width();
	}

	return 0;
}

void CXTPDockingPanePaintManager::ClipAndAddEllipsisIfNeeded(CDC* pDC, CString& strTitle,
															 int nAvailableWidth) const
{
	CSize szText = pDC->GetTextExtent(strTitle);
	if (szText.cx <= nAvailableWidth)
		return;

	const CString strEllipsis(_T("..."));
	const CSize szEll = pDC->GetTextExtent(strEllipsis);
	nAvailableWidth -= szEll.cx;

	if (nAvailableWidth <= 0)
		return;

	float fAverageSymbolWidth = float(szText.cx) / float(strTitle.GetLength());
	int nRestSymbols		  = int(static_cast<float>(nAvailableWidth) / fAverageSymbolWidth);
	szText					  = pDC->GetTextExtent(strTitle.Left(nRestSymbols));

	int nDiff = szText.cx - nAvailableWidth;
	if (nDiff != 0)
	{
		if (nDiff > 0) // text need to decrease
		{
			do
			{
				--nRestSymbols;
				if (nRestSymbols == 1)
					break;

				szText = pDC->GetTextExtent(strTitle.Left(nRestSymbols));
				nDiff  = szText.cx - nAvailableWidth;
			} while (nDiff > 0);
		}
		else // text need to increase
		{
			do
			{
				++nRestSymbols;
				if (nRestSymbols > strTitle.GetLength())
					break;

				szText = pDC->GetTextExtent(strTitle.Left(nRestSymbols));
				nDiff  = szText.cx - nAvailableWidth;
			} while (nDiff < 0);
			--nRestSymbols;
		}
	}

	if (nRestSymbols < 1)
		nRestSymbols = 1;
	strTitle = strTitle.Left(nRestSymbols) + strEllipsis;
}

CXTPDockingPane* CXTPDockingPanePaintManager::GetSelectedPane(CXTPDockingPaneBase* pPane)
{
	if (pPane->GetType() == xtpPaneTypeTabbedContainer)
	{
		return ((CXTPDockingPaneTabbedContainer*)pPane)->GetSelected();
	}
	if (pPane->GetType() == xtpPaneTypeSidePanel)
	{
		return ((CXTPDockingPaneSidePanel*)pPane)->GetSelectedPane();
	}
	else if (pPane->GetType() == xtpPaneTypeMiniWnd)
	{
		CXTPDockingPaneBaseList lst;
		pPane->FindPane(xtpPaneTypeTabbedContainer, &lst);

		if (lst.GetCount() == 1)
		{
			return ((CXTPDockingPaneTabbedContainer*)lst.GetHead())->GetSelected();
		}
	}
	return NULL;
}

BOOL CXTPDockingPanePaintManager::IsCaptionEnabled(CXTPDockingPaneBase* pPane)
{
	CXTPDockingPane* pSelectedPane = GetSelectedPane(pPane);
	if (!pSelectedPane)
		return TRUE;
	return pSelectedPane->GetEnabled() & xtpPaneEnableClient;
}

void CXTPDockingPanePaintManager::DrawCaptionIcon(CDC* pDC, CXTPDockingPaneBase* pPane,
												  CRect& rcCaption, BOOL bVertical)
{
	if (!m_bDrawCaptionIcon)
		return;

	CSize szIcon = GetTabPaintManager()->m_szIcon;

	CXTPDockingPane* pSelectedPane = GetSelectedPane(pPane);
	CXTPImageManagerIcon* pImage   = pSelectedPane ? pSelectedPane->GetIcon(szIcon.cx) : NULL;

	if (!pImage)
		return;

	XTPImageState imageState = (IsCaptionEnabled(pPane) ? xtpImageNormal : xtpImageDisabled);
	if (!bVertical && rcCaption.Width() >= szIcon.cx)
	{
		pImage->Draw(pDC, CPoint(rcCaption.left, rcCaption.CenterPoint().y - szIcon.cy / 2),
					 imageState, szIcon);

		rcCaption.left += szIcon.cx + 3;
	}
	else if (bVertical && rcCaption.Height() >= szIcon.cy)
	{
		pImage->Draw(pDC, CPoint(rcCaption.CenterPoint().x - szIcon.cy / 2, rcCaption.top),
					 imageState, szIcon);

		rcCaption.top += szIcon.cy + 3;
	}
}

void CXTPDockingPanePaintManager::DrawAutoHidePanel(CDC* pDC, CRect rc,
													XTPDockingPaneDirection direction)
{
	pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_3DFACE));

	switch (direction)
	{
		case xtpPaneDockLeft:
			pDC->Draw3dRect(rc.right - 1, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_BTNTEXT),
							GetXtremeColor(COLOR_BTNTEXT));
			pDC->Draw3dRect(rc.right - 2, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_3DSHADOW),
							GetXtremeColor(COLOR_3DSHADOW));
			break;

		case xtpPaneDockTop:
			pDC->Draw3dRect(rc.left, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_BTNTEXT),
							GetXtremeColor(COLOR_BTNTEXT));
			pDC->Draw3dRect(rc.left, rc.bottom - 2, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW),
							GetXtremeColor(COLOR_3DSHADOW));
			break;

		case xtpPaneDockRight:
			pDC->Draw3dRect(rc.left + 1, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_3DHIGHLIGHT),
							GetXtremeColor(COLOR_3DHIGHLIGHT));
			break;

		case xtpPaneDockBottom:
			pDC->Draw3dRect(rc.left, rc.top + 1, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT),
							GetXtremeColor(COLOR_3DHIGHLIGHT));
			break;
	}
}

void CXTPDockingPanePaintManager::SetCustomTitleBrushes(HBRUSH hbrActiveTitleHorz,
														HBRUSH hbrInactiveTitleHorz /*= NULL*/,
														HBRUSH hbrActiveTitleVert /*= NULL*/,
														HBRUSH hbrInactiveTitleVert /*= NULL*/,
														BOOL bDestroyOnCleanup /*= FALSE*/)
{
	// Cleanup
	if (m_bDestroyCustomTitleBrushes)
	{
		if (NULL != m_hbrActiveTitleHorz)
		{
			DeleteObject(m_hbrActiveTitleHorz);
			if (m_hbrInactiveTitleHorz == m_hbrActiveTitleHorz)
				m_hbrInactiveTitleHorz = NULL;
			if (m_hbrActiveTitleVert == m_hbrActiveTitleHorz)
				m_hbrActiveTitleVert = NULL;
			if (m_hbrInactiveTitleVert == m_hbrActiveTitleHorz)
				m_hbrInactiveTitleVert = NULL;
			m_hbrActiveTitleHorz = NULL;
		}

		if (NULL != m_hbrInactiveTitleHorz)
		{
			DeleteObject(m_hbrInactiveTitleHorz);
			if (m_hbrActiveTitleVert == m_hbrInactiveTitleHorz)
				m_hbrActiveTitleVert = NULL;
			if (m_hbrInactiveTitleVert == m_hbrInactiveTitleHorz)
				m_hbrInactiveTitleVert = NULL;
			m_hbrInactiveTitleHorz = NULL;
		}

		if (NULL != m_hbrActiveTitleVert)
		{
			DeleteObject(m_hbrActiveTitleVert);
			if (m_hbrInactiveTitleVert == m_hbrActiveTitleVert)
				m_hbrInactiveTitleVert = NULL;
			m_hbrActiveTitleVert = NULL;
		}

		if (NULL != m_hbrInactiveTitleVert)
		{
			DeleteObject(m_hbrInactiveTitleVert);
			m_hbrInactiveTitleVert = NULL;
		}
	}

	// Assign new brushes.
	m_hbrActiveTitleHorz   = hbrActiveTitleHorz;
	m_hbrInactiveTitleHorz = hbrInactiveTitleHorz;
	m_hbrActiveTitleVert   = hbrActiveTitleVert;
	m_hbrInactiveTitleVert = hbrInactiveTitleVert;

	if (NULL == m_hbrInactiveTitleHorz)
		m_hbrInactiveTitleHorz = m_hbrActiveTitleHorz;
	if (NULL == m_hbrActiveTitleVert)
		m_hbrActiveTitleVert = m_hbrActiveTitleHorz;
	if (NULL == m_hbrInactiveTitleVert)
		m_hbrInactiveTitleVert = m_hbrInactiveTitleHorz;

	m_bDestroyCustomTitleBrushes = bDestroyOnCleanup;
}

HBRUSH CXTPDockingPanePaintManager::GetCustomTitleBrush(BOOL bActive, BOOL bVertical) const
{
	HBRUSH hBrush = (bVertical ? (bActive ? m_hbrActiveTitleVert : m_hbrInactiveTitleVert)
							   : (bActive ? m_hbrActiveTitleHorz : m_hbrInactiveTitleHorz));
	return hBrush;
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPDockingPanePaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPDockingPanePaintManager, XTPDIID_IDockingPanePaintManager, Dispatch)
END_INTERFACE_MAP()

BEGIN_DISPATCH_MAP(CXTPDockingPanePaintManager, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "ShowCaption", 1, m_bShowCaption, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "CaptionFont", DISPID_FONT, OleGetFont,
						OleSetFont, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "HighlightActiveCaption", 2,
					 m_bHighlightActiveCaption, VT_BOOL)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "DrawSingleTab", 3, m_bDrawSingleTab, VT_BOOL)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "SplitterSize", 4, m_nSplitterSize, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "SplitterStyle", 5, GetSplitterStyle,
						SetSplitterStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "SplitterColor", 6, OleGetSplitterColor,
						OleSetSplitterColor, VT_COLOR)

	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "CaptionButtonStyle", 7, m_nCaptionButtonStyle,
					 VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPDockingPanePaintManager, "CaptionMargin", 8, m_nCaptionFontGap,
							OleRefreshMetrics, VT_I4)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "DrawCaptionIcon", 9, m_bDrawCaptionIcon, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "UseCustomTitleColors", 10,
						OleGetUseCustomTitleColors, OleSetUseCustomTitleColors, VT_BOOL)
	DISP_FUNCTION_ID(CXTPDockingPanePaintManager, "SetCustomTitleColors", 11,
					 OleSetCustomTitleColors, VT_EMPTY, VTS_COLOR VTS_COLOR VTS_COLOR VTS_COLOR)
	DISP_FUNCTION_ID(CXTPDockingPanePaintManager, "GetCustomTitleColor", 12, OleGetCustomTitleColor,
					 VT_COLOR, VTS_BOOL VTS_BOOL)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDockingPanePaintManager, XTPDIID_IDockingPanePaintManager)

void CXTPDockingPanePaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPDockingPanePaintManager::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		SetCaptionFontIndirect(&lf);
	}
	else
	{
		SetCaptionFontIndirect(NULL, TRUE);
	}
}

LPFONTDISP CXTPDockingPanePaintManager::OleGetFont()
{
	return AxCreateOleFont(&m_xtpFontTitle, this,
						   (LPFNFONTCHANGED)&CXTPDockingPanePaintManager::OleSetFont);
}

BOOL CXTPDockingPanePaintManager::OleGetUseCustomTitleColors()
{
	return m_bAxUseCustomTitleColors;
}

void CXTPDockingPanePaintManager::OleSetUseCustomTitleColors(BOOL fUse)
{
	if (fUse != m_bAxUseCustomTitleColors)
	{
		m_bAxUseCustomTitleColors = fUse;
		if (m_bAxUseCustomTitleColors)
		{
			OleSetCustomTitleColors(m_clrAxTitleActiveHorz, m_clrAxTitleInactiveHorz,
									m_clrAxTitleActiveVert, m_clrAxTitleInactiveVert);
		}
		else
		{
			SetCustomTitleBrushes(NULL);
		}
	}
}

void CXTPDockingPanePaintManager::OleSetCustomTitleColors(OLE_COLOR clrActiveHorz,
														  OLE_COLOR clrInactiveHorz,
														  OLE_COLOR clrActiveVert,
														  OLE_COLOR clrInactiveVert)
{
	m_clrAxTitleActiveHorz   = clrActiveHorz;
	m_clrAxTitleInactiveHorz = clrInactiveHorz;
	m_clrAxTitleActiveVert   = clrActiveVert;
	m_clrAxTitleInactiveVert = clrInactiveVert;

	SetCustomTitleBrushes(CreateSolidBrush(m_clrAxTitleActiveHorz),
						  CreateSolidBrush(m_clrAxTitleInactiveHorz),
						  CreateSolidBrush(m_clrAxTitleActiveVert),
						  CreateSolidBrush(m_clrAxTitleInactiveVert), TRUE);
}

OLE_COLOR CXTPDockingPanePaintManager::OleGetCustomTitleColor(BOOL fActive, BOOL fVert)
{
	OLE_COLOR clr;
	if (fActive)
	{
		if (fVert)
			clr = m_clrAxTitleActiveVert;
		else
			clr = m_clrAxTitleActiveHorz;
	}
	else
	{
		if (fVert)
			clr = m_clrAxTitleInactiveVert;
		else
			clr = m_clrAxTitleInactiveHorz;
	}

	return clr;
}

COLORREF CXTPDockingPanePaintManager::OleGetSplitterColor()
{
	return m_clrSplitter;
}

void CXTPDockingPanePaintManager::OleSetSplitterColor(OLE_COLOR clr)
{
	m_clrSplitter.SetCustomValue(AxTranslateColor(clr));
}

#endif
