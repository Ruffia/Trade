// XTPTabColorSetDefault.cpp
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

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetDefault.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTabColorSetDefault

void CXTPTabColorSetDefault::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_csStateButton.clrFace.SetStandardValue(XTP_TABMANAGER_COLOR_SHADED);
	m_csStateButton.clrBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW),
											   GetXtremeColor(COLOR_3DHIGHLIGHT));

	m_csExcelSelected.clrRightInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrLeftInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrTopInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrLeftOuterBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_csExcelSelected.clrTopOuterBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csExcelSelected.clrRightOuterBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csExcelNormal = m_csExcelSelected;
	m_csExcelNormal.clrLeftOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal.clrRightOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal.clrTopOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceExcel2003)
	{
		m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrButtonNormal.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
		m_clrFrameBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	}
}

void CXTPTabColorSetDefault::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton,
												CRect& rc)
{
	COLORREF clrFrameTopLeft	 = GetXtremeColor(COLOR_3DSHADOW),
			 clrFrameBottomRight = GetXtremeColor(COLOR_BTNHIGHLIGHT);

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
	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

	pButton->DrawEntry(pDC, rc, m_clrActiveFilesGlyph, pButton->IsEnabled());
}
