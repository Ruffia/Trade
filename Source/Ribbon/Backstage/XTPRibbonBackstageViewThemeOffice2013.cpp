// XTPRibbonBackstageViewThemeOffice2013.cpp : implementation file
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
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/XTPHookManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"

#include "Ribbon/Resource.h"
#include "Ribbon/Backstage/XTPRibbonBackstageView.h"
#include "Ribbon/Backstage/XTPRibbonBackstagePaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPRibbonBackstageViewThemeOffice2013::CXTPRibbonBackstageViewThemeOffice2013()
{
	m_nCommandGap			 = 0;
	m_nCommandGapY			 = 0;
	m_nMenuItemCommandHeight = 30 + 7;
	m_nMenuItemPageHeight	= 30 + 7;
	m_nMenuItemPageGapY		 = 0;
	m_nMenuSeparatorHeight   = 21;

	m_clrButtonBackNormal = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("ButtonBackNormal"),
										RGB(43, 87, 154));
	m_clrButtonBackSelected	= XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonBackSelected"), RGB(62, 109, 181));
	m_clrButtonBackHighlighted = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonBackHighlighted"), RGB(25, 71, 138));
	m_clrButtonTextNormal = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("ButtonTextNormal"),
										RGB(255, 255, 255));
	m_clrButtonTextDisabled	= XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonTextDisabled"), RGB(62, 109, 181));
	m_clrButtonTextSelected	= XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonTextSelected"), RGB(255, 255, 255));
	m_clrButtonTextHighlighted = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonTextHighlighted"), RGB(255, 255, 255));
}

CXTPRibbonBackstageViewThemeOffice2013::~CXTPRibbonBackstageViewThemeOffice2013()
{
}

void CXTPRibbonBackstageViewThemeOffice2013::DrawBackground(CDC* pDC,
															CXTPRibbonBackstageView* pView)
{
	ASSERT(pDC);
	ASSERT(pView);
	CXTPClientRect rc(pView);

	pDC->FillSolidRect(rc, m_clrButtonBackNormal);
	if (pView->GetBackstageStyle() > xtpRibbonBackstageOffice2010Style)
	{
		rc.left = pView->m_nMenuWidth;
		pDC->FillSolidRect(rc, pView->m_clrBack);
	}
}

void CXTPRibbonBackstageViewThemeOffice2013::DrawControl(CDC* pDC, CXTPRibbonBackstageView* pView,
														 CXTPControl* pControl)
{
	if (!pControl->IsVisible())
		return;

	if (pView->IsMenuSeparator(pControl))
	{
		CRect rcControl(pControl->GetRect());
		rcControl.left += GetMenuItemTextGap() + GetMenuItemCommandGapX();
		rcControl.right -= GetMenuItemTextGap() + GetMenuItemCommandGapX();
		rcControl.top += rcControl.Height() / 2;
		rcControl.bottom = rcControl.top + XTP_DPI_Y(1);

		pDC->FillSolidRect(rcControl, m_clrButtonTextDisabled);
		return;
	}

	BOOL bEnabled	 = pControl->GetEnabled();
	BOOL bActiveTab   = pView->IsActiveTab(pControl);
	BOOL bHighlighted = pView->IsHighlightedControl(pControl);
	BOOL bFocused	 = pView->IsFocusedControl(pControl);

	if (pView->IsCommand(pControl))
	{
		bHighlighted = bFocused || bHighlighted;
	}

	CRect rcControl(pControl->GetRect());
	COLORREF clrText, clrBack;

	if (bEnabled)
	{
		clrText = bHighlighted ? m_clrButtonTextHighlighted
							   : (bActiveTab ? m_clrButtonTextSelected : m_clrButtonTextNormal);
		clrBack = bHighlighted ? m_clrButtonBackHighlighted
							   : (bActiveTab ? m_clrButtonBackSelected : m_clrButtonBackNormal);
	}
	else
	{
		clrText = m_clrButtonTextDisabled;
		clrBack = bHighlighted || bFocused ? m_clrButtonBackHighlighted : m_clrButtonBackNormal;
	}

	pDC->FillSolidRect(rcControl, clrBack);

	CRect rcText(rcControl);
	if (pView->GetBackstageStyle() > xtpRibbonBackstageOffice2010Style)
		rcText.DeflateRect(26, XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
	else
		rcText.DeflateRect(GetMenuItemTextGap() + GetMenuItemCommandGapX(), XTP_DPI_Y(2),
						   XTP_DPI_X(2), XTP_DPI_Y(2));

	CXTPFontDC font(pDC, &pView->m_xtpFontCommand);
	pDC->SetTextColor(clrText);
	pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
}
