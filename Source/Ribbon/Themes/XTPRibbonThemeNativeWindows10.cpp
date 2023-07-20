// XTPRibbonThemeNativeWindows10.cpp : implementation file
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
#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPCasting.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPResource.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "TabManager/Includes.h"

#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2013Theme.h"

#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonTab.h"
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonSystemButton.h"
#include "Ribbon/XTPRibbonMarkupTheme.h"
#include "Ribbon/XTPRibbonControlTab.h"
#include "Ribbon/XTPRibbonTabColorSet.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"
#include "Markup/Extensions/XTPMarkupSystemColorsStaticExtension.h"
#include "Markup/Extensions/XTPMarkupAmbientColorsStaticExtension.h"

#include "Ribbon/Themes/XTPRibbonThemeOffice2013.h"
#include "Ribbon/Themes/XTPRibbonThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonThemeNativeWindows10

CXTPRibbonThemeNativeWindows10::CXTPRibbonThemeNativeWindows10(CXTPPaintManager* pPaintManager)
	: CXTPRibbonOffice2013Theme(pPaintManager)
{
	if (m_pMarkupContext)
	{
		CXTPMarkupStaticExtension* pRoot = m_pMarkupContext->GetExtensionRoot();
		if (pRoot)
		{
			CXTPMarkupStaticExtension* pTheme = pRoot->FindExtension(L"Theme");
			if (pTheme)
			{
				CXTPMarkupStaticExtension* pColors = pTheme->FindExtension(L"Colors");
				if (pColors)
				{
					// Title bar color
					ASSERT(NULL == pColors->FindExtension(L"FrameCaptionActiveBackAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameCaptionActiveBackAccentBrush", xtpPaintManagerAccentColor));

					// File menu button back color
					ASSERT(NULL == pColors->FindExtension(L"FrameSystemMenuNormalBackAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameSystemMenuNormalBackAccentBrush",
						xtpPaintManagerNormalBackAccentColor));

					ASSERT(NULL
						   == pColors->FindExtension(L"FrameSystemMenuSelectedBackAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameSystemMenuSelectedBackAccentBrush",
						xtpPaintManagerPressedBackAccentColor));

					ASSERT(NULL
						   == pColors->FindExtension(L"FrameSystemMenuPopupedBackAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameSystemMenuPopupedBackAccentBrush",
						xtpPaintManagerHighlightBackAccentColor));

					// File text color
					ASSERT(NULL == pColors->FindExtension(L"FrameSystemMenuNormalTextAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameSystemMenuNormalTextAccentBrush",
						xtpPaintManagerNormalTextAccentColor));

					ASSERT(NULL
						   == pColors->FindExtension(L"FrameSystemMenuSelectedTextAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameSystemMenuSelectedTextAccentBrush",
						xtpPaintManagerHighlightTextAccentColor));

					ASSERT(NULL
						   == pColors->FindExtension(L"FrameSystemMenuPopupedTextAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameSystemMenuPopupedTextAccentBrush",
						xtpPaintManagerPressedTextAccentColor));

					// Backstage title bar color
					ASSERT(NULL == pColors->FindExtension(L"FrameCaptionBackstageLeftAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameCaptionBackstageLeftAccentBrush", xtpPaintManagerAccentColor));

					ASSERT(NULL
						   == pColors->FindExtension(L"FrameCaptionBackstageRightAccentBrush"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"FrameCaptionBackstageRightAccentBrush", xtpPaintManagerAccentColor));

					// Backstage back button text color (same text color as other buttons)
					ASSERT(NULL
						   == pColors->FindExtension(L"BackstageSysButtonBackTextAccentNormal"));
					pColors->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
						L"BackstageSysButtonBackTextAccentNormal",
						xtpPaintManagerNormalTextAccentColor));
				}
			}
		}
	}
}

CXTPRibbonThemeNativeWindows10::~CXTPRibbonThemeNativeWindows10()
{
}

void CXTPRibbonThemeNativeWindows10::RefreshMetrics()
{
	CXTPRibbonOffice2013Theme::RefreshMetrics();

	CXTPTabPaintManagerColorSet* pColorSet = m_pTabPaintManager->GetColorSet();
	if (pColorSet)
	{
		pColorSet->m_clrButtonBorderHighlighted = XTPIniColor(_T("CommandBars.Ribbon"),
															  _T("TabBorderHighlighted"),
															  RGB(255, 255, 255));
		pColorSet->m_clrButtonBorderTabContext1 = XTPIniColor(_T("CommandBars.Ribbon"),
															  _T("TabBorderTabContext1"),
															  RGB(193, 193, 193));
		pColorSet->m_clrButtonBorderTabContext2 = XTPIniColor(_T("CommandBars.Ribbon"),
															  _T("TabBorderTabContext2"),
															  RGB(255, 255, 255));
		pColorSet->m_clrButtonBorderKeyTips		= XTPIniColor(_T("CommandBars.Ribbon"),
														  _T("TabBorderKeyTips"),
														  RGB(255, 162, 111));
	}

	// backstage button '<-' back color
	m_clrBackstageSysButtonBack2013Normal	  = XTPGetAccentColor(xtpPaintManagerAccentColor);
	m_clrBackstageSysButtonBack2013Highlighted = XTPGetAccentColor(
		xtpPaintManagerPressedBackAccentColor);
}

void CXTPRibbonThemeNativeWindows10::FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab,
												   CRect rcGroups)
{
	ASSERT(pDC);
	UNUSED_ALWAYS(pActiveTab);

	switch (pActiveTab->GetContextColor())
	{
		case xtpRibbonTabContextColorBlue:
			RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectBlue);
			break;
		case xtpRibbonTabContextColorYellow:
			RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectYellow);
			break;
		case xtpRibbonTabContextColorGreen:
			RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectGreen);
			break;
		case xtpRibbonTabContextColorRed: RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectRed); break;
		case xtpRibbonTabContextColorPurple:
			RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectPurple);
			break;
		case xtpRibbonTabContextColorCyan:
			RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectCyan);
			break;
		case xtpRibbonTabContextColorOrange:
			RenderMarkup(pDC, rcGroups, xtpRibbonGroupRectOrange);
			break;
		case xtpRibbonTabContextColorNone: RenderMarkup(pDC, rcGroups, xtpRibbonGroupRect); break;
		default: RenderMarkup(pDC, rcGroups, xtpRibbonGroupRect); break;
	}
}

void CXTPRibbonThemeNativeWindows10::DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem)
{
	// must be valid.
	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	// must be valid.
	CXTPRibbonBar* pRibbon = pItem->GetRibbonBar();
	ASSERT(pRibbon != NULL);
	if (!pRibbon)
		return;

	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();
	XTPTabPosition tabPosition		  = pItem->GetTabManager()->GetPosition();

	CRect r(pItem->GetRect());
	CXTPTabPaintManagerTheme::InflateRectEx(r, CRect(-1, 1, -1, -1), tabPosition);
	BOOL bSelected = pItem->IsSelected();
	if (pRibbon->IsRibbonMinimized() && bSelected)
	{
		CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pItem->GetTabManager();
		if (pControlTab)
		{
			if (!pControlTab->GetCommandBar() || !pControlTab->GetCommandBar()->IsTrackingMode())
				bSelected = FALSE;
		}
	}

	if (bSelected)
	{
		COLORREF clrBorder = m_clrRibbonBorder;

		if (pItem->GetContextHeader())
		{
			clrBorder = m_clrHeader[tabColor].clrEdge;
		}
		else
		{
			if (pRibbon->GetCommandBars()->m_keyboardTips.pBar)
			{
				clrBorder = XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
										  m_clrButtonBorderKeyTips, tabColor);
			}
		}

		pDC->FillSolidRect(r, m_clrHeader[xtpRibbonTabContextColorNone].clrBack);

		XTPDrawHelpers()->DrawLine(pDC, r.left, r.top, r.left, r.bottom, clrBorder);
		XTPDrawHelpers()->DrawLine(pDC, r.right, r.top, r.right, r.bottom, clrBorder);
		XTPDrawHelpers()->DrawLine(pDC, r.left, r.top, r.right, r.top, clrBorder);
	}
	else
	{
		r.DeflateRect(0, 0, 0, XTP_DPI_Y(1));

		if (pRibbon->IsActive())
		{
			if (pItem->IsHighlighted())
			{
				CRect rc(r);

				if (pItem->GetContextHeader())
				{
					pDC->FillSolidRect(rc, m_clrHeader[tabColor].clrEdge);
				}
				else
				{
					pDC->FillSolidRect(rc, XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
														 m_clrButtonBorderHighlighted, tabColor));
				}

				rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), 0);

				pDC->FillSolidRect(rc, XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
													 m_clrButtonHighlighted, tabColor));
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, r,
											   XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
															 m_clrButtonBorderTabContext1,
															 tabColor),
											   XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
															 m_clrButtonBorderTabContext2,
															 tabColor),
											   FALSE);

				CXTPRibbonTab* pFirstContextTab = XTP_SAFE_GET2(pItem, GetContextHeader(),
																m_pFirstTab, NULL);
				CXTPRibbonTab* pLastContextTab  = XTP_SAFE_GET2(pItem, GetContextHeader(),
																m_pLastTab, NULL);

				r.DeflateRect(pItem == pFirstContextTab ? XTP_DPI_Y(1) : 0, 0,
							  pItem == pLastContextTab ? XTP_DPI_Y(1) : 0, 0);

				pDC->FillSolidRect(r, XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
													m_clrButtonNormal, tabColor));
			}
		}
		else
		{
			pDC->FillSolidRect(r, m_clrTabBackInactiveState);
		}
	}

	for (int i = 0; i < XTP_SAFE_GET2(pItem, GetTabManager(), GetItemCount(), 0); i++)
	{
		if (XTP_SAFE_GET3(pItem, GetTabManager(), GetItem(i), IsItemShrinked(), FALSE))
		{
			CRect rc(r);
			rc.left  = bSelected ? rc.right + 1 : rc.right;
			rc.right = rc.left + XTP_DPI_X(1);
			rc.top += XTP_DPI_Y(3);
			rc.bottom -= XTP_DPI_Y(3);

			pDC->FillSolidRect(rc, m_clrRibbonTabDelimeter);
			break;
		}
	}
}

void CXTPRibbonThemeNativeWindows10::DrawRibbonFrameContextHeaders(
	CDC* pDC, CXTPRibbonBar* pRibbonBar, CXTPRibbonTabContextHeaders* pContextHeaders)
{
	int nCount = pContextHeaders->GetCount();

	CXTPWinThemeWrapper wrapper(FALSE);
	wrapper.OpenThemeData(NULL, L"WINDOW");

	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pContextHeaders->GetHeader(i);
		XTPRibbonTabContextColor color		= pHeader->m_color;

		CString strCaption(pHeader->m_strCaption);
		if (pRibbonBar->GetCommandBars()->IsAllCaps())
		{
			strCaption.MakeUpper();
		}

		CRect rcHeader(pHeader->m_rcRect);
		if (LAYOUT_RTL == pDC->GetLayout())
		{
			// GDI+ has issues with RTL renderring, so this is to emulate RTL.
			CPoint ptHeaderTopLeft = rcHeader.TopLeft();
			pRibbonBar->ClientToScreen(&ptHeaderTopLeft);

			CXTPClientRect rcClient(pRibbonBar);
			CPoint ptWindowTopLeft = rcClient.TopLeft();
			CPoint ptWindowTopRight(rcClient.right, ptWindowTopLeft.y);
			pRibbonBar->ClientToScreen(&ptWindowTopLeft);
			pRibbonBar->ClientToScreen(&ptWindowTopRight);
			ASSERT(ptWindowTopRight.x <= ptWindowTopLeft.x);
			ASSERT(ptHeaderTopLeft.x < ptWindowTopLeft.x && ptWindowTopRight.x < ptHeaderTopLeft.x);

			int nWidth	= rcHeader.Width();
			rcHeader.left = rcClient.Width() - (ptWindowTopLeft.x - ptHeaderTopLeft.x) - nWidth
							- XTP_DPI_X(1);
			rcHeader.right = rcHeader.left + nWidth;
		}

		rcHeader.DeflateRect(XTP_DPI_X(1), 0, XTP_DPI_X(1), 0);

		CRect rcBackground(rcHeader);
		CRect rcText(rcBackground);
		CSize szText = pDC->GetTextExtent(strCaption);

		int nMaxTextWidth = rcText.Width() - XTP_DPI_X(10);
		if (nMaxTextWidth < szText.cx)
		{
			szText.cx = nMaxTextWidth;
		}

		rcText.left += (rcText.Width() - szText.cx) / 2;

		// Draw header using GDI+
		Gdiplus::Graphics graphics(pDC->GetSafeHdc());
		graphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
		graphics.SetCompositingQuality(Gdiplus::CompositingQualityDefault);

		// Draw background
		DWORD clrBackground = m_clrHeader[color].clrBack;
		Gdiplus::SolidBrush brBackground(
			Gdiplus::Color(0xfe, // Not 0xff because GDI+ the applies color blending.
						   GetRValue(clrBackground), GetGValue(clrBackground),
						   GetBValue(clrBackground)));
		Gdiplus::RectF rcfBackground(static_cast<Gdiplus::REAL>(rcBackground.left),
									 static_cast<Gdiplus::REAL>(rcBackground.top),
									 static_cast<Gdiplus::REAL>(rcBackground.Width()),
									 static_cast<Gdiplus::REAL>(rcBackground.Height()));
		graphics.FillRectangle(&brBackground, rcfBackground);

		// Draw text
		Gdiplus::Font font(pDC->GetSafeHdc(), *m_pTabPaintManager->GetFont());
		Gdiplus::RectF rcfText(static_cast<Gdiplus::REAL>(rcText.left),
							   static_cast<Gdiplus::REAL>(rcText.top),
							   static_cast<Gdiplus::REAL>(rcText.Width()),
							   static_cast<Gdiplus::REAL>(rcText.Height()));

		DWORD clrText = m_clrHeader[color].clrText;
		Gdiplus::SolidBrush brText(
			Gdiplus::Color(0xfe, // Not 0xff because GDI+ the applies color blending.
						   GetRValue(clrText), GetGValue(clrText), GetBValue(clrText)));

		Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap, ::GetUserDefaultLangID());
		format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		graphics.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
#ifdef _UNICODE
		graphics.DrawString(strCaption, strCaption.GetLength(), &font, rcfText, &format, &brText);
#else
		WCHAR* pUnicodeCaption = new WCHAR[XTPToSizeT(strCaption.GetLength() + 1)];
		int cchCaption		   = MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, strCaption,
											 strCaption.GetLength(), pUnicodeCaption,
											 XTPToInt((strCaption.GetLength() + 1)
													  * sizeof(WCHAR)));
		if (0 < cchCaption)
		{
			graphics.DrawString(pUnicodeCaption, cchCaption, &font, rcfText, &format, &brText);
		}
		delete[] pUnicodeCaption;
#endif
	}

	wrapper.CloseThemeData();
}
