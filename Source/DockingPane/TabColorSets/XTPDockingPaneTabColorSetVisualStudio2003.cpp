// XTPDockingPaneTabColorSetVisualStudio2003.cpp
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

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneTabColorSetVisualStudio2003::CXTPDockingPaneTabColorSetVisualStudio2003(
	BOOL bHighlightSelected)
{
	m_bHighlightSelected = bHighlightSelected;
	m_bTransparentButton = FALSE;
	m_bGradientButton	= FALSE;
}

void CXTPDockingPaneTabColorSetVisualStudio2003::RefreshMetrics()
{
	CXTPTabColorSetOffice2003::RefreshMetrics();

	if (m_bHighlightSelected)
	{
		m_clrButtonSelected.SetStandardValue(
			XTPColorManager()->LightColor(GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_WINDOW),
										  0x02));

		m_clrTextSelected.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
	}
	else
	{
		m_clrTextSelected.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	}
	m_clrHeaderFace.SetStandardValue(GetSysColor(COLOR_3DFACE),
									 XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE),
																   GetSysColor(COLOR_WINDOW),
																   0xcd));
	m_clrTextNormal.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	m_clrButtonNormal.SetStandardValue(RGB(242, 242, 247));

	m_bGradientButton = FALSE;
	m_bLunaTheme	  = FALSE;

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
		case xtpSystemThemeSilver:
		case xtpSystemThemeBlue:
		case xtpSystemThemeOlive:
			m_clrTextNormal.SetStandardValue(RGB(113, 111, 100));
			if (!m_bHighlightSelected)
				m_clrTextSelected.SetStandardValue(RGB(113, 111, 100));
			break;
	}

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);
}

void CXTPDockingPaneTabColorSetVisualStudio2003::FillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd,
																   CWnd* pSite)
{
	::XTPFillFramePartRect(pDC, rc, pWnd, pSite, m_clrHeaderFace, TRUE);
}

void CXTPDockingPaneTabColorSetVisualStudio2003::FillHeader(CDC* pDC, CRect rc,
															CXTPTabManager* pTabManager)
{
	FillFramePartRect(pDC, rc, (CXTPDockingPaneTabbedContainer*)pTabManager,
					  ((CXTPDockingPaneTabbedContainer*)pTabManager)->GetDockingSite());
}

COLORREF CXTPDockingPaneTabColorSetVisualStudio2003::FillPropertyButton(CDC* pDC, CRect rcItem,
																		CXTPTabManagerItem* pItem)
{
	if (m_bTransparentButton && (!m_bHighlightSelected || !pItem->IsSelected()))
		return COLORREF_NULL;

	if (m_bGradientButton)
		return CXTPTabColorSetOffice2003::FillPropertyButton(pDC, rcItem, pItem);

	return CXTPTabColorSetOffice2003::FillPropertyButton(pDC, rcItem, pItem);
}
