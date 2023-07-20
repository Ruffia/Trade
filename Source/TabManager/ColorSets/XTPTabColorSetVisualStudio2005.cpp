// XTPTabColorSetVisualStudio2005.cpp
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
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetDefault.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2003.h"
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2005.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// void CColorSetVisualStudio2005

CColorSetVisualStudio2005::CColorSetVisualStudio2005()
{
	m_bGradientButton = FALSE;
}

void CColorSetVisualStudio2005::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrTextSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_bLunaTheme	  = FALSE;
	m_bGradientButton = FALSE;

	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrTextHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	if (!m_pPaintManager->IsLunaColorsDisabled())
	{
		CXTPTabColorSetOffice2003::RefreshMetrics();
	}
	else
	{
		switch (XTPColorManager()->GetCurrentSystemTheme())
		{
			case xtpSystemThemeBlue:
			case xtpSystemThemeOlive:
				m_clrHeaderFace.SetStandardValue(RGB(228, 226, 213));
				m_bGradientButton = TRUE;
				break;

			case xtpSystemThemeSilver:
				m_clrButtonNormal.SetStandardValue(RGB(245, 245, 248));
				m_clrHeaderFace.SetStandardValue(RGB(238, 238, 238));
				m_bGradientButton = TRUE;
				break;
		}
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
		m_clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
	}
	m_clrTextSelectedHighlighted   = m_clrTextNormal;
	m_clrButtonSelectedHighlighted = m_clrButtonNormal;
}

COLORREF CColorSetVisualStudio2005::FillPropertyButton(CDC* pDC, CRect rcItem,
													   CXTPTabManagerItem* pItem)
{
	COLORREF clrItem = GetItemColor(pItem);

	if (pItem->IsHighlighted())
	{
		clrItem = m_clrButtonHighlighted;
	}

	if (m_bGradientButton)
	{
		GradientFill(pDC, rcItem, XTPColorManager()->LightColor(clrItem, RGB(255, 255, 255), 300),
					 clrItem, pItem->GetTabManager()->GetPosition());
	}
	else
	{
		pDC->FillSolidRect(rcItem, clrItem);
	}

	return clrItem;
}
