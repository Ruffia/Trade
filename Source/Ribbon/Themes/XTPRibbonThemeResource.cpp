// XTPRibbonThemeResource.cpp : implementation file
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
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

#include "Ribbon/Themes/XTPRibbonThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//

AFX_INLINE LPCTSTR FormatName(LPCTSTR lpszName)
{
	return lpszName;
}

AFX_INLINE CString FormatName(LPCTSTR lpszFormat, LPCTSTR lpszParameter)
{
	CString strResult;
	strResult.Format(lpszFormat, lpszParameter);
	return strResult;
}

AFX_INLINE LPCTSTR GetColorName(XTPRibbonTabContextColor color)
{
	switch (color)
	{
		case xtpRibbonTabContextColorBlue: return _T("BLUE");
		case xtpRibbonTabContextColorYellow: return _T("YELLOW");
		case xtpRibbonTabContextColorGreen: return _T("GREEN");
		case xtpRibbonTabContextColorRed: return _T("RED");
		case xtpRibbonTabContextColorPurple: return _T("PURPLE");
		case xtpRibbonTabContextColorCyan: return _T("CYAN");
		case xtpRibbonTabContextColorOrange: return _T("ORANGE");
	}

	return _T("");
}

AFX_INLINE DWORD GetDrawImageFlags(CXTPControl* pControl, const CRect& rc)
{
	return DWORD(IsCompositeRect(pControl->GetParent(), rc) ? XTP_DI_COMPOSITE : 0);
}

AFX_INLINE DWORD GetDrawImageFlags(CXTPControl* pControl)
{
	return DWORD(IsCompositeRect(pControl->GetParent(), pControl->GetRect()) ? XTP_DI_COMPOSITE
																			 : 0);
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonThemeResource

CXTPRibbonThemeResource::CXTPRibbonThemeResource(CXTPPaintManager* pPaintManager)
	: CXTPRibbonPaintManager(pPaintManager)
{
}

CXTPRibbonThemeResource::~CXTPRibbonThemeResource()
{
}

void CXTPRibbonThemeResource::RefreshMetrics()
{
	CXTPRibbonPaintManager::RefreshMetrics();

	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImages* pImages = GetImages();

	if (!pImages->IsValid()) // Resources not loaded
		return;

	m_clrRibbonFace			= GetImages()->GetImageColor(_T("Ribbon"), _T("RibbonFace"));
	m_clrRibbonInactiveFace = GetImages()->GetImageColor(_T("Ribbon"), _T("RibbonInactiveFace"),
														 m_clrRibbonFace);

	COLORREF clrTabNormalText = pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText"));

	m_pTabPaintManager->GetColorSet()->m_clrTextHighlight.SetStandardValue(
		pImages->GetImageColor(_T("Ribbon"), _T("TabHighlightText"), clrTabNormalText));
	m_pTabPaintManager->GetColorSet()->m_clrTextNormal.SetStandardValue(clrTabNormalText);

	m_pTabPaintManager->GetColorSet()->m_clrTextSelected.SetStandardValue(
		pImages->GetImageColor(_T("Ribbon"), _T("TabSelectedText")));

	m_clrRibbonGroupCaptionText = pImages->GetImageColor(_T("Ribbon"), _T("GroupCaptionText"));

	m_clrMinimizedFrameEdgeHighLight = pImages->GetImageColor(_T("Ribbon"),
															  _T("MinimizedFrameEdgeHighLight"));
	m_clrMinimizedFrameEdgeShadow	= pImages->GetImageColor(_T("Ribbon"),
															  _T("MinimizedFrameEdgeShadow"));

	m_clrRecentFileListEdgeShadow = GetImages()->GetImageColor(_T("Ribbon"),
															   _T("RecentFileListEdgeShadow"));
	m_clrRecentFileListEdgeHighLight =
		GetImages()->GetImageColor(_T("Ribbon"), _T("RecentFileListEdgeHighLight"));

	m_nGroupSpacing = XTP_DPI_X(GetImages()->GetImageInt(_T("Ribbon"), _T("GroupSpacing"), 2));

	m_bFlatFrame = GetImages()->GetImageInt(_T("Window"), _T("FlatFrame"), 0);

	m_clrFileButtonText = GetImages()->GetImageColor(_T("Ribbon"), _T("FileButtonText"));

	m_rcFileButtonMargin = XTP_DPI(GetImages()->GetImageRect(_T("Ribbon"), _T("FileButtonMargin")));

	m_nFileButtonImageCount = GetImages()->GetImageInt(_T("Ribbon"), _T("FileButtonImageCount"), 3);
}

void CXTPRibbonThemeResource::FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup,
													CXTPCommandBar* pCommandBar)
{
	ASSERT(NULL != pDC);
	ASSERT(NULL != pGroup);
	UNUSED_ALWAYS(pCommandBar);

	if (!XTPResourceImages()->IsValid())
		return;

	pDC->SetBkMode(TRANSPARENT);

	CRect rcGroup = pGroup->GetRect();
	pDC->FillSolidRect(rcGroup, m_clrRibbonFace);

	XTPRibbonTabContextColor tabColor = pGroup->GetParentTab()->GetContextColor();

	if (tabColor != xtpRibbonTabContextColorNone)
	{
		CXTPResourceImage* pImage = LoadImage(
			FormatName(_T("CONTEXTTAB%sGROUPPOPUPBACKGROUND"), GetColorName(tabColor)));

		if (!pImage)
		{
			pImage = LoadImage(FormatName(_T("CONTEXTTAB%sCLIENT"), GetColorName(tabColor)));
			rcGroup.InflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
		}

		if (pImage)
			pImage->DrawImage(pDC, rcGroup, pImage->GetSource(0, 1),
							  pImage->GetWidth() < 8 ? CRect(2, 8, 2, 8) : CRect(8, 8, 8, 8));
	}
	else
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONGROUPPOPUPBACKGROUND")));

		if (!pImage)
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPS")));
			rcGroup.InflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
		}

		if (pImage)
			pImage->DrawImage(pDC, rcGroup, pImage->GetSource(0, 1),
							  pImage->GetWidth() < 8 ? CRect(2, 8, 2, 8) : CRect(8, 8, 8, 8));
	}
}

void CXTPRibbonThemeResource::DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CRect rcGroup(pGroup->GetRect());

	CRect rcGroupClient(rcGroup.left, rcGroup.top, rcGroup.right,
						rcGroup.bottom - m_nGroupCaptionHeight);
	CRect rcGroupCaption(rcGroup.left, rcGroupClient.bottom, rcGroup.right, rcGroup.bottom);

	if (!pGroup->IsReduced())
	{
		CXTPResourceImage *pImage = 0, *pImageCaption = 0;

		if (pGroup->GetParentTab()->GetContextColor() == xtpRibbonTabContextColorNone)
		{
			pImage		  = LoadImage(FormatName(_T("RIBBONGROUPCLIENT")));
			pImageCaption = LoadImage(FormatName(_T("RIBBONGROUPCAPTION")));
		}
		else
		{
			XTPRibbonTabContextColor tabColor = pGroup->GetParentTab()->GetContextColor();

			pImage = LoadImage(FormatName(_T("CONTEXTTAB%sGROUPCLIENT"), GetColorName(tabColor)));
			if (!pImage)
			{
				pImage		  = LoadImage(FormatName(_T("CONTEXTTABGROUPCLIENT")));
				pImageCaption = LoadImage(FormatName(_T("CONTEXTTABGROUPCAPTION")));
			}
		}
		if (!pImageCaption)
		{
			rcGroupClient = rcGroup;
		}

		int nState = pGroup->IsHighlighted() ? 1 : 0;

		if (pImage)
			pImage->DrawImage(pDC, rcGroupClient, pImage->GetSource(nState, 2), CRect(5, 5, 5, 5));
		if (pImageCaption)
			pImageCaption->DrawImage(pDC, rcGroupCaption, pImageCaption->GetSource(nState, 2),
									 CRect(5, 5, 5, 5));

		pImage = LoadImage(FormatName(_T("RIBBONGROUPSEPARATOR")));
		if (pImage)
		{
			pImage->DrawImage(pDC,
							  CRect(rcGroup.right - XTP_DPI_X(1), rcGroup.top + XTP_DPI_Y(2),
									rcGroup.right + XTP_DPI_X(2), rcGroup.bottom - XTP_DPI_Y(5)),
							  pImage->GetSource());
		}

		CString strCaption = pGroup->GetCaption();
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPFontDC font(pDC, &m_fontGroupCaption);

		CRect rcGroupCaptionText(rcGroupCaption);
		rcGroupCaptionText.OffsetRect(0, -XTP_DPI_Y(1));
		if (pGroup->IsOptionButtonVisible())
			rcGroupCaptionText.right -= m_nGroupCaptionHeight + XTP_DPI_X(1);
		rcGroupCaptionText.DeflateRect(XTP_DPI_X(2), 0);

		if (!rcGroupCaptionText.IsRectEmpty())
		{
			pDC->SetTextColor(m_clrRibbonGroupCaptionText);
			pDC->DrawText(strCaption, rcGroupCaptionText,
						  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_CENTER);
		}
	}
}

void CXTPRibbonThemeResource::FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPClientRect rcRibbonClient(pRibbonBar);

	if (pRibbonBar->IsFrameThemeEnabled())
	{
		rcRibbonClient.top += (::GetSystemMetrics(SM_CYSIZEFRAME)
							   + ::GetSystemMetrics(SM_CYCAPTION));
	}

	CRect rcRibbonTabs(pRibbonBar->GetTabControlRect());
	rcRibbonTabs.left  = rcRibbonClient.left;
	rcRibbonTabs.right = rcRibbonClient.right;

	CRect rcRibbonGroups(rcRibbonClient.left, rcRibbonTabs.bottom, rcRibbonClient.right,
						 rcRibbonClient.bottom);

	pDC->FillSolidRect(rcRibbonTabs, m_clrRibbonFace);
	pDC->FillSolidRect(rcRibbonGroups, m_clrRibbonFace);

	if (m_bFlatFrame)
	{
		if (!pRibbonBar->IsDwmEnabled())
		{
			CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();
			if (pFrameHook && !pFrameHook->IsFrameActive() && pRibbonBar->IsFrameThemeEnabled())
			{
				pDC->FillSolidRect(rcRibbonTabs, m_clrRibbonInactiveFace);
			}
		}
		else
		{
			pDC->FillSolidRect(rcRibbonTabs, 0);

			CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTABBACKGROUNDDWM")));
			if (pImage)
			{
				CRect rcSrc(pImage->GetSource(0, 1));
				CXTPClientRect rcClient(pRibbonBar);
				CRect rcTabs(pRibbonBar->GetTabControlRect());

				int nHeight = min(rcTabs.Height(), rcSrc.Height());
				rcSrc.top   = rcSrc.bottom - nHeight;
				CRect rcArea(rcClient.left, rcTabs.bottom - nHeight, rcClient.right, rcTabs.bottom);

				pImage->DrawImage(pDC, rcArea, rcSrc, CRect(10, 0, 10, 0));
			}
		}
	}

	if (pRibbonBar->IsQuickAccessBelowRibbon())
	{
		CRect rcQuickAccess		  = pRibbonBar->GetQuickAccessRect();
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONQUICKACCESSFRAME")));
		if (pImage)
			pImage->DrawImage(pDC, rcQuickAccess, pImage->GetSource(0, 1), CRect(3, 3, 3, 3));
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

void CXTPRibbonThemeResource::FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups)
{
	if (!XTPResourceImages()->IsValid())
		return;

	XTPRibbonTabContextColor tabColor = pActiveTab ? pActiveTab->GetContextColor()
												   : xtpRibbonTabContextColorNone;

	if (tabColor != xtpRibbonTabContextColorNone)
	{
		CXTPResourceImage* pImage = LoadImage(
			FormatName(_T("CONTEXTTAB%sCLIENT"), GetColorName(tabColor)));
		if (pImage)
			pImage->DrawImage(pDC, rcGroups, pImage->GetSource(0, 1),
							  pImage->GetWidth() < 5 ? CRect(1, 8, 1, 8) : CRect(8, 8, 8, 8));
	}
	else
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONGROUPS")));
		if (pImage)
			pImage->DrawImage(pDC, rcGroups, pImage->GetSource(0, 1), CRect(8, 8, 8, 8));
	}
}

CSize CXTPRibbonThemeResource::DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption,
													  BOOL bDraw)
{
	if (XTPResourceImages()->IsValid() && bDraw)
	{
		CRect rc = pControlGroupOption->GetRect();

		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONOPTIONBUTTON")));

		int nState = 0;
		if (pControlGroupOption->GetSelected())
			nState = 1;
		if (pControlGroupOption->GetPressed())
			nState = 2;

		CRect rcImage = pImage->GetSource(nState, 4);
		rc.left += rc.Width() / 2 - XTP_DPI_X(rcImage.Width() / 2);
		rc.right = rc.left + XTP_DPI_X(rcImage.Width());
		rc.bottom -= rc.Height() / 2 - XTP_DPI_Y(rcImage.Height() / 2);
		rc.top = rc.bottom - XTP_DPI_Y(rcImage.Height());
		if (pImage)
			pImage->DrawImage(pDC, rc, rcImage, CXTPEmptyRect(), 0xFF00FF);
	}

	return XTP_DPI(CSize(15, 14));
}

CSize CXTPRibbonThemeResource::DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup,
													 BOOL bDraw)
{
	if (!XTPResourceImages()->IsValid())
		return CSize(0, 0);

	CXTPRibbonGroup* pRibbonGroup = pControlGroupPopup->GetRibbonGroup();
	CXTPResourceImage* pImage	 = NULL;

	int nOffset = 0;
	if (pControlGroupPopup->GetSelected())
		nOffset = 1;
	if (pControlGroupPopup->GetPopuped())
		nOffset = 2;

	if (!pRibbonGroup)
	{
		if (bDraw)
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPQUICKACCESS")));
			ASSERT(pImage);

			if (pImage)
				pImage->DrawImage(pDC, pControlGroupPopup->GetRect(), pImage->GetSource(nOffset, 3),
								  CRect(2, 2, 2, 2), 0xFF00FF,
								  GetDrawImageFlags(pControlGroupPopup));
		}
		return m_pPaintManager->DrawControlToolBarParent(pDC, pControlGroupPopup, bDraw);
	}

	if (bDraw)
	{
		CRect rc = pControlGroupPopup->GetRect();

		XTPRibbonTabContextColor tabColor = pRibbonGroup
												? pRibbonGroup->GetParentTab()->GetContextColor()
												: xtpRibbonTabContextColorNone;

		if (tabColor != xtpRibbonTabContextColorNone)
		{
			pImage = LoadImage(FormatName(_T("CONTEXTTAB%sGROUPBUTTON"), GetColorName(tabColor)));
		}
		else
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPBUTTON")));
		}

		if (pImage)
			pImage->DrawImage(pDC, rc, pImage->GetSource(nOffset, 3), CRect(5, 5, 5, 5));
	}

	CSize szButton(max(XTP_DPI_X(42), m_pPaintManager->m_nTextHeight * 2 - XTP_DPI_X(3)),
				   GetEditHeight() * 3);

	CXTPFontDC font(pDC, m_pPaintManager->GetControlFont(pControlGroupPopup));

	int nSplitDropDownHeight = m_pPaintManager->GetSplitDropDownHeight();

	if (bDraw)
	{
		CXTPImageManagerIcon* pImageIcon = pControlGroupPopup->GetImage(
			pControlGroupPopup->GetIconSize().cx);
		BOOL bDrawImage = (pImageIcon != NULL);

		CRect rcButton = pControlGroupPopup->GetRect();
		if (bDrawImage)
		{
			CSize sz(m_pPaintManager->m_bAutoResizeIcons ? m_pPaintManager->GetAutoIconSize(TRUE)
														 : pImageIcon->GetExtent());

			sz = m_pPaintManager->AdjustDpiIconSize(pImageIcon, sz, pControlGroupPopup,
													rcButton.Size());

			CPoint pt = CPoint(rcButton.CenterPoint().x - sz.cx / 2, rcButton.top + XTP_DPI_Y(4));

			if (sz.cx < XTP_DPI_X(20))
			{
				CXTPResourceImage* pImageButton = LoadImage(
					FormatName(_T("RIBBONGROUPBUTTONICON")));
				CSize szImageButton(pImageButton != NULL ? pImageButton->GetExtent() : sz);

				// Increase ImageButton if it is smaller than icon
				const int nImageButtonMarginX = XTP_DPI_X(5) * 2;
				if ((szImageButton.cx - sz.cx) < nImageButtonMarginX)
					szImageButton.cx = sz.cx + nImageButtonMarginX;

				const int nImageButtonMarginY = XTP_DPI_Y(5) * 2;
				if ((szImageButton.cy - sz.cy) < nImageButtonMarginY)
					szImageButton.cy = sz.cy + nImageButtonMarginY;

				CRect rc(CPoint(rcButton.CenterPoint().x - szImageButton.cx / 2,
								rcButton.top + XTP_DPI_Y(2)),
						 szImageButton);

				if (pImageButton)
					pImageButton->DrawImage(pDC, rc, pImageButton->GetSource(0, 1),
											CXTPEmptyRect());

				pt.x = rc.CenterPoint().x - sz.cx / 2;
				pt.y = rc.CenterPoint().y - sz.cy / 2;
			}

			m_pPaintManager->DrawImage(pDC, pt, sz, pImageIcon, pControlGroupPopup->GetSelected(),
									   pControlGroupPopup->GetPressed(),
									   pControlGroupPopup->GetEnabled(), FALSE,
									   pControlGroupPopup->GetPopuped());
		}

		CRect rcText = rcButton;
		rcText.bottom -= m_nGroupCaptionHeight + XTP_DPI_Y(2);
		rcText.top = rcText.bottom - nSplitDropDownHeight;

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

void CXTPRibbonThemeResource::DrawRibbonFrameContextHeaders(
	CDC* pDC, CXTPRibbonBar* pRibbonBar, CXTPRibbonTabContextHeaders* pContextHeaders)
{
	if (!XTPResourceImages()->IsValid())
		return;

	int nCount = pContextHeaders->GetCount();
	if (nCount == 0)
		return;

	CXTPFontDC font(pDC, m_pTabPaintManager->GetFont());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pTabPaintManager->GetColorSet()->m_clrTextNormal);

	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pContextHeaders->GetHeader(i);

		CXTPResourceImage* pImage = LoadImage(
			FormatName(_T("CONTEXTTAB%sHEADER"), GetColorName(pHeader->m_color)));
		if (!pImage)
			return;

		CRect rc(pHeader->m_rcRect);
		pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(2, 4, 2, 2));

		int nFrameBorder = pRibbonBar->IsFrameThemeEnabled()
							   ? pRibbonBar->GetFrameHook()->CalcFrameBorder().top
							   : 0;

		rc.top += nFrameBorder - XTP_DPI_Y(2);

		if (pRibbonBar->IsDwmEnabled())
		{
			CSize szText = pDC->GetTextExtent(pHeader->m_strCaption);
			if ((rc.Width() - XTP_DPI_X(10)) < szText.cx)
			{
				szText.cx = rc.Width() - XTP_DPI_X(10);
			}

			rc.left += ((rc.Width() / 2) - (szText.cx / 2));

			DrawDwmCaptionText(pDC, rc, pHeader->m_strCaption, pRibbonBar->GetSite(),
							   pRibbonBar->GetFrameHook()->IsFrameActive());
		}
		else
		{
			pDC->DrawText(pHeader->m_strCaption, rc,
						  DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS
							  | DT_LEFT);
		}
	}
}

void CXTPRibbonThemeResource::FillRibbonTabControl(CDC* pDC, CXTPRibbonBar* pRibbonBar,
												   CRect rcControl)
{
	if (!XTPResourceImages()->IsValid())
		return;

	int nCount = pRibbonBar->GetContextHeaders()->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pRibbonBar->GetContextHeaders()->GetHeader(i);
		if (!pHeader || !pHeader->m_pFirstTab || !pHeader->m_pLastTab
			|| pHeader->m_color == xtpRibbonTabContextColorNone)
			continue;

		CRect rcFirst(pHeader->m_pFirstTab->GetRect());
		CRect rcLast(pHeader->m_pLastTab->GetRect());

		CXTPResourceImage* pImage = LoadImage(
			FormatName(_T("CONTEXTTAB%sBACKGROUND"), GetColorName(pHeader->m_color)));
		if (pImage)
			pImage->DrawImage(pDC, CRect(rcFirst.left, rcControl.top, rcLast.right, rcLast.bottom),
							  pImage->GetSource(), CRect(2, 2, 2, 2));
	}
}

void CXTPRibbonThemeResource::DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem)
{
	if (!XTPResourceImages()->IsValid())
		return;

	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcEntry(pItem->GetRect());
	CXTPTabPaintManagerTheme::InflateRectEx(rcEntry, CRect(0, 1, -1, -1), tabPosition);

	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();

	CXTPRibbonBar* pRibbonBar = pItem->GetRibbonBar();

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

	if (bSelected && pRibbonBar->IsBackstageViewVisible())
		bSelected = FALSE;

	if (tabColor != xtpRibbonTabContextColorNone)
	{
		if (bSelected || bHighlighted)
		{
			CXTPResourceImage* pImage = LoadImage(
				FormatName(_T("CONTEXTTAB%s"), GetColorName(tabColor)));

			if (pImage && pImage->GetHeight() < 70)
			{
				int nState = 0;
				if (bSelected)
				{
					nState = bFocused ? 2 : 1;
				}
				else if (bHighlighted)
				{
					nState = 0;
				}

				pImage->DrawImage(pDC, rcEntry, pImage->GetSource(nState, 3), CRect(5, 5, 5, 5));
			}
			else
			{
				int nState = 0;
				if (bSelected)
				{
					nState = bFocused || bHighlighted ? 4 : 3;
				}
				else if (bHighlighted)
				{
					nState = 1;
				}

				if (nState == 4)
				{
					pImage = LoadImage(FormatName(_T("RIBBONTAB")));
					if (pImage)
						pImage->DrawImage(pDC, rcEntry, pImage->GetSource(5, 6), CRect(5, 5, 5, 5),
										  0xFF00FF);
				}
				else
				{
					if (pImage)
						pImage->DrawImage(pDC, rcEntry, pImage->GetSource(nState, 5),
										  CRect(5, 5, 5, 5));
				}
			}
		}
	}
	else
	{
		if (bSelected || bHighlighted)
		{
			CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTAB")));

			int nState = 0;
			if (bSelected && bFocused)
			{
				nState = 5;
			}
			else if (bSelected && bHighlighted)
			{
				nState = 4;
			}
			else if (bSelected)
			{
				nState = 3;
			}
			else if (pItem->IsPressed())
			{
				nState = 2;
			}
			else if (bHighlighted)
			{
				nState = 1;
			}

			if (pImage)
				pImage->DrawImage(pDC, rcEntry, pImage->GetSource(nState, 6), CRect(5, 5, 5, 5),
								  0xFF00FF);
		}
	}

	if (pItem->GetButtonLength() < pItem->GetContentLength())
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTABSEPARATOR")));
		CRect rcSeparator(rcEntry.right, rcEntry.top, rcEntry.right + XTP_DPI_Y(pImage->GetWidth()),
						  rcEntry.bottom - XTP_DPI_Y(1));
		if (pImage)
			pImage->DrawImage(pDC, rcSeparator, pImage->GetSource(), CXTPEmptyRect());
	}

	if (tabColor != xtpRibbonTabContextColorNone && pItem->GetContextHeader()
		&& (pItem->GetContextHeader()->m_pFirstTab == pItem))
	{
		CRect rcSeparator(rcEntry.left, rcEntry.top, rcEntry.left + XTP_DPI_X(1),
						  rcEntry.bottom - XTP_DPI_Y(1));
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTABSEPARATOR")));
		if (pImage)
			pImage->DrawImage(pDC, rcSeparator, pImage->GetSource(), CXTPEmptyRect());
	}
	if (tabColor != xtpRibbonTabContextColorNone && pItem->GetContextHeader()
		&& (pItem->GetContextHeader()->m_pLastTab == pItem))
	{
		CRect rcSeparator(rcEntry.right, rcEntry.top, rcEntry.right + XTP_DPI_X(1),
						  rcEntry.bottom - XTP_DPI_Y(1));
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTABSEPARATOR")));
		if (pImage)
			pImage->DrawImage(pDC, rcSeparator, pImage->GetSource(), CXTPEmptyRect());
	}
}

void CXTPRibbonThemeResource::DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar,
														BOOL bActive)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CRect rcCaption = pRibbonBar->GetCaptionRect();
	rcCaption.top--;

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("FRAMETOPCENTER")));
	pImage->DrawImage(pDC, rcCaption, pImage->GetSource(bActive ? 0 : 1, 2), CRect(0, 5, 0, 3));

	DrawRibbonQuickAccessControls(pDC, pRibbonBar);

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}
}

void CXTPRibbonThemeResource::DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar,
													 BOOL bActive)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CFrameWnd* pSite = (CFrameWnd*)pRibbonBar->GetSite();
	CRect rcCaption  = pRibbonBar->GetCaptionRect();
	CXTPResourceImage* pImage;

	if (!pRibbonBar->IsDwmEnabled())
	{
		pImage = LoadImage(FormatName(_T("FRAMETOPLEFT")));

		ASSERT(pImage);
		if (!pImage)
			return;

		CRect rcSrc(pImage->GetSource(bActive ? 0 : 1, 2));

		CRect rcTopLeft(rcCaption.left, rcCaption.top, rcCaption.left + XTP_DPI_X(rcSrc.Width()),
						rcCaption.bottom);
		pImage->DrawImage(pDC, rcTopLeft, rcSrc, CRect(0, 5, 0, 3), 0xFF00FF);

		//////////////////////////////////////////////////////////////////////////

		pImage = LoadImage(FormatName(_T("FRAMETOPRIGHT")));

		rcSrc = pImage->GetSource(bActive ? 0 : 1, 2);
		CRect rcTopRight(rcCaption.right - XTP_DPI_X(rcSrc.Width()), rcCaption.top, rcCaption.right,
						 rcCaption.bottom);
		pImage->DrawImage(pDC, rcTopRight, rcSrc, CRect(0, 5, 0, 3));

		//////////////////////////////////////////////////////////////////////////

		pImage = LoadImage(FormatName(_T("FRAMETOPCENTER")));

		CRect rcTopCenter(rcTopLeft.right, rcCaption.top, rcTopRight.left, rcCaption.bottom);
		pImage->DrawImage(pDC, rcTopCenter, pImage->GetSource(bActive ? 0 : 1, 2),
						  CRect(0, 5, 0, 3));
	}
	else
	{
		pDC->FillSolidRect(rcCaption, 0);

		if (m_bFlatFrame)
		{
			pImage = LoadImage(FormatName(_T("RIBBONTABBACKGROUNDDWM")));
			if (pImage)
			{
				CRect rcSrc(pImage->GetSource(0, 1));
				CXTPClientRect rcRibbonClient(pRibbonBar);
				CRect rcRibbonTabs(pRibbonBar->GetTabControlRect());
				rcSrc.bottom = rcSrc.bottom - rcRibbonTabs.Height();
				if (rcSrc.Height() > 0)
				{
					CRect rcArea(rcRibbonClient.left, rcRibbonTabs.top - rcSrc.Height(),
								 rcRibbonClient.right, rcRibbonTabs.top);
					pImage->DrawImage(pDC, rcArea, rcSrc, CRect(10, 0, 10, 0));
				}
			}
		}
	}

	DrawRibbonQuickAccessControls(pDC, pRibbonBar);

	if ((pRibbonBar->GetSystemButton() == NULL || pRibbonBar->GetSystemButton()->IsSimpleButton())
		&& pRibbonBar->GetFrameHook())
	{
		HICON hIcon = m_pPaintManager->GetFramePaintManager()->GetFrameSmallIcon(pSite);
		if (hIcon)
		{
			int nFrameBorder = pRibbonBar->GetFrameHook()->CalcFrameBorder().top;
			int nTopBorder   = pRibbonBar->GetRibbonTopBorder();

			CSize szIcon(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));

			int nTop  = nTopBorder / 2 - XTP_DPI_Y(1) + (rcCaption.bottom - szIcon.cy) / 2;
			int nLeft = rcCaption.left + nFrameBorder + XTP_DPI_X(1);

			DrawRibbonApplicationIcon(pDC, pRibbonBar,
									  CRect(nLeft, nTop, nLeft + szIcon.cx, nTop + szIcon.cy),
									  hIcon);

			if (pRibbonBar->IsQuickAccessVisible())
			{
				pImage = LoadImage(FormatName(_T("SYSTEMICONSEPARATOR")));
				if (pImage)
				{
					CRect rcSrc(pImage->GetSource());
					CRect rcDest(CPoint(nLeft + szIcon.cy + XTP_DPI_X(6),
										nTop + szIcon.cy / 2 - XTP_DPI_Y(rcSrc.Height()) / 2),
								 XTP_DPI(rcSrc.Size()));
					pImage->DrawImage(pDC, rcDest, rcSrc);
				}
			}
		}
	}

	CRect rcCaptionText = pRibbonBar->GetCaptionTextRect();

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}

	CXTPFontDC font(pDC, &m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption);

	CString strWindowText;
	pSite->GetWindowText(strWindowText);
	rcCaptionText.left += max(0,
							  (rcCaptionText.Width() - pDC->GetTextExtent(strWindowText).cx) / 2);

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

void CXTPRibbonThemeResource::DrawRibbonQuickAccessControls(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImage* pImage;

	if (!pRibbonBar->IsQuickAccessBelowRibbon() && pRibbonBar->IsQuickAccessVisible()
		&& ((CXTPControls*)pRibbonBar->GetQuickAccessControls())->GetVisibleCount() > 0)
	{
		if (pRibbonBar->GetSystemButton() && !pRibbonBar->GetSystemButton()->IsSimpleButton())
		{
			CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
			CRect rcQuickAccessArea(rcQuickAccess.left - XTP_DPI_X(15),
									rcQuickAccess.top + XTP_DPI_Y(2), rcQuickAccess.right,
									rcQuickAccess.top + XTP_DPI_Y(2)
										+ pRibbonBar->GetQuickAccessHeight());

			BOOL bDwm = IsCompositeRect(pRibbonBar, rcQuickAccess);
			pImage	= LoadImage(
				   FormatName(bDwm ? _T("RIBBONQUICKACCESSAREADWM") : _T("RIBBONQUICKACCESSAREA")));
			if (pImage)
			{
				pImage->PreserveOrientation();
				pImage->DrawImage(pDC, rcQuickAccessArea, pImage->GetSource(), CRect(16, 3, 16, 3));
			}
		}
	}
}

void CXTPRibbonThemeResource::DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONQUICKACCESSBUTTON")));

	ASSERT(pImage);
	if (!pImage)
		return;

	pImage->DrawImage(pDC, pControl->GetRect(),
					  pImage->GetSource(!pControl->GetEnabled()
											? 4
											: pControl->GetPopuped()
												  ? 2
												  : pControl->GetSelected() ? 1 : 0,
										5),
					  CRect(2, 2, 2, 2), 0xFF00FF, GetDrawImageFlags(pControl));
}

void CXTPRibbonThemeResource::DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONQUICKACCESSMORE")));

	ASSERT(pImage);
	if (!pImage)
		return;

	pImage->DrawImage(pDC, pControl->GetRect(),
					  pImage->GetSource(pControl->GetPopuped() ? 2
															   : pControl->GetSelected() ? 1 : 0,
										4),
					  CRect(2, 2, 2, 2), 0xFF00FF, GetDrawImageFlags(pControl));
}

void CXTPRibbonThemeResource::DrawRibbonFrameSystemButton(CDC* pDC,
														  CXTPRibbonControlSystemButton* pControl,
														  CRect rc)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pControl->GetParent();

	if (!pRibbonBar->IsCaptionVisible() || !pRibbonBar->IsTabsVisible()
		|| pControl->IsSimpleButton())
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("FILEBUTTON")));

		if (pImage)
		{
			if (m_nFileButtonImageCount == 7)
			{
				int nState = !pControl->GetEnabled()
								 ? 5
								 : pControl->GetPopuped()
									   ? (pControl->GetSelected() ? 3 : 2)
									   : IsKeyboardSelected(pControl->GetSelected())
											 ? 4
											 : pControl->GetSelected() ? 1 : 0;

				CRect rcSrc(pImage->GetSource(nState, 7));

				CRect rcDest(rc);

				if (pControl->GetPopuped())
				{
					++rcDest.bottom;
				}
				else
				{
					--rcSrc.bottom;
				}

				pImage->DrawImage(pDC, rcDest, rcSrc, CRect(3, 3, 3, 3));
			}
			else
			{
				int nState = pControl->GetPopuped() ? 2 : pControl->GetSelected() ? 1 : 0;

				pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 3), CRect(3, 3, 3, 3));
			}

			if (pControl->GetStyle() == xtpButtonIcon)
			{
				CXTPImageManagerIcon* pButtonIcon = pControl->GetImage();
				if (pButtonIcon)
				{
					CSize sz;
					if (pButtonIcon->IsVectorIcon())
					{
						sz = XTPSystemMetrics()->GetSmallIconSize();
					}
					else
					{
						sz = pButtonIcon->GetExtent();
						if (CXTPPaintManager::IsDpiBitmapScalingEnabled(pRibbonBar))
							sz = XTP_DPI(sz);
					}

					CPoint pt((rc.left + rc.right - sz.cx) / 2, (rc.top + rc.bottom - sz.cy) / 2);
					pButtonIcon->Draw(pDC, pt, sz);
				}
			}
			else
			{
				pDC->SetTextColor(m_clrFileButtonText);

				BOOL bComposite = IsCompositeRect(pRibbonBar, rc);

				CString strCaption(pControl->GetCaption());
				CXTPPaintManager::StripMnemonics(strCaption);

				CXTPPaintManager::DrawTextApi(pDC, strCaption, rc,
											  DT_NOPREFIX | DT_VCENTER | DT_CENTER | DT_SINGLELINE,
											  bComposite);
			}
		}
		else
		{
			m_pPaintManager->DrawControlToolBarParent(pDC, pControl, TRUE);
		}
		return;
	}

	XTPImageState nState = xtpImageNormal;
	if (pControl->GetPopuped())
		nState = xtpImagePressed;
	else if (pControl->GetSelected())
		nState = xtpImageHot;
	else if (!pControl->GetEnabled())
		nState = xtpImageDisabled;

	CXTPImageManagerIcon* pIcon = pControl->GetImage(32);

	if (pIcon && pIcon->GetWidth() > 32)
	{
		CSize sz;
		if (pIcon->IsVectorIcon())
			sz = XTPSystemMetrics()->GetIconSize();
		else
			sz = CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), pControl,
													 rc.Size());

		CPoint ptIcon((rc.left + rc.right - sz.cx) / 2, (rc.top + rc.bottom - sz.cy) / 2);
		pIcon->m_bDrawComposited = pControl->HasDwmCompositedRect();
		pIcon->Draw(pDC, ptIcon, nState, sz);
		pIcon->m_bDrawComposited = FALSE;
		return;
	}

	CXTPResourceImage* pImage = NULL;

	if (rc.Width() > XTP_DPI_X(54) && rc.Height() > XTP_DPI_Y(54))
		pImage = LoadImage(FormatName(_T("FRAMESYSTEMBUTTON52")));

	if (pImage == NULL)
		pImage = LoadImage(FormatName(_T("FRAMESYSTEMBUTTON")));

	ASSERT(pImage);
	if (!pImage)
		return;

	int nOffset = 0;
	if (rc.top < 0)
	{
		nOffset = rc.top;
		rc.OffsetRect(0, -nOffset);
	}

	CRect rcSrc(pImage->GetSource(nState == xtpImageDisabled || nState == xtpImageNormal
									  ? 0
									  : nState == xtpImageHot ? 1 : 2,
								  3));
	CSize szDest(XTP_DPI(rcSrc.Size()));
	CPoint ptDest((rc.left + rc.right - szDest.cx) / 2,
				  nOffset + (rc.top + rc.bottom - szDest.cy) / 2);

	pImage->DrawImage(pDC, CRect(ptDest, szDest), rcSrc, CXTPEmptyRect());

	if (pIcon)
	{
		CSize sz;
		if (pIcon->IsVectorIcon())
			sz = XTPSystemMetrics()->GetIconSize();
		else
			sz = CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), pControl,
													 rc.Size());

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

void CXTPRibbonThemeResource::DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl,
													 BOOL bScrollLeft)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImage* pImage = LoadImage(
		FormatName(bScrollLeft ? _T("RIBBONGROUPSSCROLLLEFT") : _T("RIBBONGROUPSSCROLLRIGHT")));

	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rc(pControl->GetRect());

	int nState = pControl->GetPressed() ? 2 : pControl->GetSelected() ? 1 : 0;

	CXTPResourceImage* pImageGlyph = LoadImage(FormatName(
		bScrollLeft ? _T("RIBBONGROUPSSCROLLLEFTGLYPH") : _T("RIBBONGROUPSSCROLLRIGHTGLYPH")));

	pImage->PreserveOrientation();
	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 3), CRect(4, 6, 3, 6), 0xFF00FF);

	CSize szGlyph = XTP_DPI(pImageGlyph->GetSource(0, 3).Size());
	CRect rcGlyph(CPoint((rc.right + rc.left - szGlyph.cx) / 2,
						 (rc.top + rc.bottom - szGlyph.cy) / 2),
				  szGlyph);

	pImageGlyph->DrawImage(pDC, rcGlyph, pImageGlyph->GetSource(nState, 3), CXTPEmptyRect(),
						   0xFF00FF);
}

void CXTPRibbonThemeResource::FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CRect rc;
	pBar->GetClientRect(&rc);

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("MOREPOPUPBARFRAME")));
	if (!pImage)
		pImage = LoadImage(FormatName(_T("POPUPBARFRAME")));

	ASSERT(pImage);
	if (!pImage)
		return;

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(4, 4, 4, 4));
}

void CXTPRibbonThemeResource::FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPClientRect rc(pBar);

	CXTPResourceImage* pImage = LoadImage(_T("RIBBONSYSTEMMENU"));
	if (!pImage)
		return;

	pImage->DrawImage(pDC, rc, pImage->GetSource(), XTP_UNDPI(pBar->GetBorders()), COLORREF_NULL);

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
			pDC->FillSolidRect(rcRow, GetImages()->GetImageColor(_T("Ribbon"),
																 _T("RecentFileListBackground")));
			break;
		}
	}
}

void CXTPRibbonThemeResource::DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImage* pImage = LoadImage(_T("RIBBONSYSTEMMENUBUTTON"));

	if (pImage)
	{
		pImage->DrawImage(pDC, pButton->GetRect(),
						  pImage->GetSource(pButton->GetSelected() ? 1 : 0, 2), CRect(2, 2, 2, 2),
						  0xFF00FF);
	}

	m_pPaintManager->DrawControlToolBarParent(pDC, pButton, TRUE);
}

BOOL CXTPRibbonThemeResource::DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton)
{
	if (!XTPResourceImages()->IsValid())
		return FALSE;

	if (pButton->GetRibbonGroup() && pButton->GetRibbonGroup()->IsControlsGrouping())
	{
		if (pButton->GetType() == xtpControlSplitButtonPopup)
			return FALSE;

		BOOL bLeft = pButton->GetBeginGroup() || pButton->GetWrap();

		int nIndexNext = pButton->GetControls()->GetNext(pButton->GetIndex());

		BOOL bRight = nIndexNext <= pButton->GetIndex()
					  || pButton->GetControls()->GetAt(nIndexNext)->GetBeginGroup()
					  || pButton->GetControls()->GetAt(nIndexNext)->GetWrap();

		CString strImage = FormatName(bLeft && bRight
										  ? _T("TOOLBARBUTTONSSPECIALSINGLE")
										  : bLeft ? _T("TOOLBARBUTTONSSPECIALLEFT")
												  : bRight ? _T("TOOLBARBUTTONSSPECIALRIGHT")
														   : _T("TOOLBARBUTTONSSPECIALCENTER"));

		CXTPResourceImage* pImage = LoadImage(strImage);
		ASSERT(pImage);
		if (!pImage)
			return FALSE;

		BOOL bPopuped  = pButton->GetPopuped();
		BOOL bEnabled  = pButton->GetEnabled();
		BOOL bChecked  = pButton->GetChecked();
		BOOL bSelected = pButton->GetSelected();
		BOOL bPressed  = pButton->GetPressed();
		CRect rc	   = pButton->GetRect();

		int nState = 0;

		if (!bEnabled)
		{
			if (IsKeyboardSelected(bSelected))
				nState = 0 + 1;
		}
		else if (bPopuped)
			nState = 2 + 1;
		else if (bChecked && !bSelected && !bPressed)
			nState = 2 + 1;
		else if (bChecked && bSelected && !bPressed)
			nState = 3 + 1;
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed))
			nState = 1 + 1;
		else if (bSelected || bPressed)
			nState = 0 + 1;

		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 5), CRect(2, 2, 2, 2), 0xFF00FF);
		return TRUE;
	}

	return FALSE;
}

int CXTPRibbonThemeResource::GetRibbonCaptionHeight(const CXTPRibbonBar* pRibbonBar) const
{
	int nRibbonCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION);

	if (XTPSystemVersion()->IsWin10OrGreater() && GetFramePaintManager()->m_bFlatFrame
		&& pRibbonBar->IsDwmEnabled())
	{
		if (AfxGetMainWnd() != NULL && (AfxGetMainWnd()->GetStyle() & WS_MAXIMIZE))
		{
			nRibbonCaptionHeight += XTP_DPI_Y(6);
		}
		else
		{
			//
		}
	}
	else
	{
		if (!pRibbonBar->IsQuickAccessVisible() || pRibbonBar->IsQuickAccessBelowRibbon())
		{
			nRibbonCaptionHeight -= XTP_DPI_Y(3);
		}
	}

	return nRibbonCaptionHeight;
}

CSize CXTPRibbonThemeResource::GetControlGroupOptionSize() const
{
	CSize sz(0, 0);
	if (XTPResourceImages()->IsValid())
	{
		CXTPResourceImage* pImage = const_cast<CXTPRibbonThemeResource*>(this)->LoadImage(
			FormatName(_T("RIBBONOPTIONBUTTON")));
		if (NULL != pImage)
		{
			sz = pImage->GetSource(0, 4).Size();
		}
	}

	return sz;
}
