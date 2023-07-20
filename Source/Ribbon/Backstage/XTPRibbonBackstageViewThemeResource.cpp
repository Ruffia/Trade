// XTPRibbonBackstageViewThemeResource.cpp : implementation file
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"
#include "CommandBars/XTPCommandBar.h"

#include "Ribbon/Backstage/XTPRibbonBackstageView.h"
#include "Ribbon/Backstage/XTPRibbonBackstagePaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPRibbonBackstageViewThemeResource::DrawBackground(CDC* pDC, CXTPRibbonBackstageView* pView)
{
	CXTPClientRect rc(pView);

	pDC->FillSolidRect(rc, RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);

	COLORREF clrText = XTPResourceImages()->GetImageColor(_T("Ribbon"),
														  _T("BackstageMenuTextColor"), 0);
	pDC->SetTextColor(clrText);

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageTopBorder"));
	if (pImage)
	{
		pImage->DrawImage(pDC, CRect(0, 0, rc.Width(), XTP_DPI_Y(2)), pImage->GetSource());
	}

	pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuBackground"));

	if (pImage)
	{
		pImage->DrawImage(pDC, CRect(0, XTP_DPI_Y(2), pView->GetMenuWidth(), rc.bottom),
						  pImage->GetSource());
	}
}

void CXTPRibbonBackstageViewThemeResource::DrawControl(CDC* pDC, CXTPRibbonBackstageView* pView,
													   CXTPControl* pControl)
{
	if (!pControl->IsVisible())
		return;

	BOOL bEnabled	 = pControl->GetEnabled();
	BOOL bActiveTab   = pView->IsActiveTab(pControl);
	BOOL bHighlighted = pView->IsHighlightedControl(pControl);
	BOOL bFocused	 = pView->IsFocusedControl(pControl);

	COLORREF clrText = XTPResourceImages()->GetImageColor(_T("Ribbon"),
														  _T("BackstageMenuTextColor"), 0);

	if (!clrText)
		clrText = pDC->GetTextColor();

	if (bActiveTab)
		clrText = RGB(255, 255, 255);

	if (!bEnabled)
		clrText = GetXtremeColor(XPCOLOR_GRAYTEXT);

	pDC->SetTextColor(clrText);

	if (pView->IsCommand(pControl))
	{
		if (bHighlighted || bFocused)
		{
			CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuCommand"));
			if (pImage)
			{
				pImage->DrawImage(pDC, pControl->GetRect(),
								  pImage->GetSource(!pControl->GetEnabled() ? 2 : 0, 3),
								  CRect(2, 2, 2, 2));
			}
		}

		CXTPFontDC font(pDC, &pView->m_xtpFontCommand);

		CRect rcText(pControl->GetRect());
		rcText.DeflateRect(GetMenuItemTextGap(), 2, XTP_DPI_X(2), XTP_DPI_Y(2));

		CXTPImageManagerIcon* pIcon =
			pView->GetImageManager()->GetImage(XTPToUInt(pControl->GetIconId()), ICON_SMALL);
		if (pIcon)
		{
			// Office 2010 respects this rule of 120 DPI
			CSize szIconScalled(XTPDpiHelper()->ScaleX(pIcon->GetWidth(), 120),
								XTPDpiHelper()->ScaleY(pIcon->GetHeight(), 120));

			CPoint ptIcon = CPoint(rcText.left,
								   (rcText.top + rcText.bottom - szIconScalled.cy) / 2);

			CXTPImageManagerIcon* pBestIcon = pView->GetImageManager()->GetImage(
				XTPToUInt(pControl->GetIconId()), szIconScalled.cx);
			if (NULL != pBestIcon)
			{
				pBestIcon->Draw(pDC, ptIcon, (bEnabled ? xtpImageNormal : xtpImageDisabled),
								szIconScalled);
			}

			rcText.left += XTPDpiHelper()->ScaleX(pIcon->GetWidth(), 120) + GetMenuImageMargin();
		}

		pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
	}
	else if (pView->IsMenuSeparator(pControl))
	{
		// do nothing
	}
	else
	{
		if (bHighlighted || bActiveTab || bFocused)
		{
			CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuPage"));
			if (pImage)
			{
				pImage->DrawImage(pDC, pControl->GetRect(),
								  pImage->GetSource(!pControl->GetEnabled() ? 3
																			: bActiveTab ? 1 : 0,
													4),
								  CRect(2, 2, 2, 2));
			}

			if (bActiveTab)
			{
				pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuPageGlyph"));
				if (pImage)
				{
					CRect rcSrc(pImage->GetSource());
					CRect rcDst(XTP_DPI(rcSrc));
					CRect rc(pControl->GetRect());
					CRect rcGlyph(CPoint(rc.right - rcDst.Width(),
										 (rc.top + rc.bottom - rcDst.Height()) / 2),
								  rcDst.Size());

					pImage->DrawImage(pDC, rcGlyph, rcSrc);
				}
			}
		}

		CXTPFontDC font(pDC, &pView->m_xtpFontTab);

		CRect rcText(pControl->GetRect());
		rcText.DeflateRect(GetMenuItemTextGap() + GetMenuItemCommandGapX(), XTP_DPI_Y(2),
						   XTP_DPI_X(2), XTP_DPI_Y(2));

		CSize sz(0, 0);
		CXTPImageManagerIcon* pIcon =
			pView->GetImageManager()->GetImage(XTPToUInt(pControl->GetIconId()), sz.cx);
		if (pIcon)
		{
			if (pIcon->IsVectorIcon())
			{
				sz = XTPSystemMetrics()->GetSmallIconSize();
			}
			else
			{
				sz = pIcon->GetExtent();
				if (pView->IsDpiIconsScalingEnabled())
					sz = XTP_DPI(sz);
			}

			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - sz.cy) / 2),
						(bEnabled ? xtpImageNormal : xtpImageDisabled), sz);

			rcText.left += sz.cx + GetMenuImageMargin();
		}

		pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
	}
}
