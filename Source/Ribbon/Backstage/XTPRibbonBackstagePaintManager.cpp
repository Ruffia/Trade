// XTPRibbonBackstagePaintManager.cpp : implementation file
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
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"

#include "Ribbon/Backstage/XTPRibbonBackstagePaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int COMMAND_GAP  = 6;
const int TEXT_GAP	 = 15;
const int IMAGE_MARGIN = 7;

CXTPRibbonBackstagePaintManager::CXTPRibbonBackstagePaintManager()
	: m_nCommandGap(COMMAND_GAP)
	, m_nCommandGapY(2)
	, m_nMenuItemCommandHeight(16 + 10)
	, m_nMenuItemPageHeight(30 + 7)
	, m_nMenuItemPageGapY(1)
	, m_nTextGap(TEXT_GAP)
	, m_nImageMargin(IMAGE_MARGIN)
	, m_nMenuSeparatorHeight(0)
{
}

CXTPRibbonBackstagePaintManager::~CXTPRibbonBackstagePaintManager()
{
}

void CXTPRibbonBackstagePaintManager::DrawBackground(CDC* pDC, CXTPRibbonBackstageView* pView)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pView);
}

void CXTPRibbonBackstagePaintManager::DrawControl(CDC* pDC, CXTPRibbonBackstageView* pView,
												  CXTPControl* pControl)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pView);
	UNUSED_ALWAYS(pControl);
}

int CXTPRibbonBackstagePaintManager::GetMenuItemCommandHeight() const
{
	return XTP_DPI_Y(m_nMenuItemCommandHeight);
}

int CXTPRibbonBackstagePaintManager::GetMenuItemPageHeight() const
{
	return XTP_DPI_Y(m_nMenuItemPageHeight);
}

int CXTPRibbonBackstagePaintManager::GetMenuItemCommandGapX() const
{
	return XTP_DPI_X(m_nCommandGap);
}

int CXTPRibbonBackstagePaintManager::GetMenuItemCommandGapY() const
{
	return XTP_DPI_Y(m_nCommandGapY);
}

int CXTPRibbonBackstagePaintManager::GetMenuItemPageGapY() const
{
	return XTP_DPI_Y(m_nMenuItemPageGapY);
}

int CXTPRibbonBackstagePaintManager::GetMenuItemTextGap() const
{
	return XTP_DPI_X(m_nTextGap);
}

int CXTPRibbonBackstagePaintManager::GetMenuImageMargin() const
{
	return XTP_DPI_X(m_nImageMargin);
}

int CXTPRibbonBackstagePaintManager::GetMenuSeparatorHeight() const
{
	return XTP_DPI_Y(m_nMenuSeparatorHeight);
}

void CXTPRibbonBackstagePaintManager::DrawKeyboardTip(CDC* pDC, CXTPPaintManager* pPaintManager,
													  CXTPControl* pControl)
{
	ASSERT(pDC);
	ASSERT(pPaintManager);
	ASSERT(pControl);
	if ((pDC == NULL) || (pPaintManager == NULL) || (pControl == NULL))
		return;
	if (!pControl->IsVisible())
		return;

	CString strTipText = pControl->GetKeyboardTip();
	CXTPFontDC font(pDC, &pPaintManager->m_fontToolTip);
	CSize sz = pDC->GetTextExtent(strTipText);
	sz = CSize(max(sz.cx + XTP_DPI_X(6), XTP_DPI_X(16)), max(sz.cy + XTP_DPI_Y(2), XTP_DPI_Y(15)));
	CRect rc(pControl->GetRect());
	rc.right  = rc.left + sz.cx;
	rc.bottom = rc.top + sz.cy;

	pDC->FillSolidRect(rc, pPaintManager->GetXtremeColor(COLOR_INFOBK));
	pDC->Draw3dRect(rc, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(
		GetXtremeColor(UINT(pControl->GetEnabled() ? COLOR_INFOTEXT : COLOR_GRAYTEXT)));
	pDC->DrawText(strTipText, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}
