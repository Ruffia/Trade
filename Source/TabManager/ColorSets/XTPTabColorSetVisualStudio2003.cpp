// XTPTabColorSetVisualStudio2003.cpp
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetDefault.h"
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CColorSetVisualStudio2003

void CColorSetVisualStudio2003::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	if (m_pPaintManager->GetAppearance() != xtpTabAppearanceExcel2003
		&& m_pPaintManager->GetAppearance() != xtpTabAppearanceStateButtons)
		m_clrHeaderFace.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceVisualStudio2003)
	{
		m_clrTextSelectedHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
		m_clrTextNormal.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
	}

	m_clrAutoHideFace.SetStandardValue(XTPDrawHelpers()->IsLowResolution()
										   ? GetXtremeColor(COLOR_3DFACE)
										   : m_clrHeaderFace.operator COLORREF());
	m_clrButtonSelectedHighlighted = GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK);
}

void CColorSetVisualStudio2003::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton,
												   CRect& rc)
{
	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceExcel2003
		|| m_pPaintManager->GetAppearance() == xtpTabAppearanceStateButtons)
	{
		CXTPTabColorSetDefault::FillNavigateButton(pDC, pButton, rc);
		return;
	}

	COLORREF clrFrameTopLeft	 = GetXtremeColor(COLOR_BTNTEXT),
			 clrFrameBottomRight = GetXtremeColor(COLOR_3DFACE);

	if (pButton->IsEnabled())
	{
		if (pButton->IsPressed())
		{
			pDC->Draw3dRect(rc, clrFrameTopLeft, clrFrameBottomRight);
			rc.OffsetRect(1, 1);
		}
		else if (pButton->IsHighlighted())
			pDC->Draw3dRect(rc, clrFrameBottomRight, clrFrameTopLeft);
	}
	pDC->SetTextColor(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));

	pButton->DrawEntry(pDC, rc, m_clrActiveFilesGlyph, pButton->IsEnabled());
}
