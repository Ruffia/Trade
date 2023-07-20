// XTPRibbonTabColorSet.cpp: implementation of the CXTPRibbonTabColorSet class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonTab.h"
#include "Ribbon/XTPRibbonControlTab.h"
#include "Ribbon/XTPRibbonTabColorSet.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPRibbonTabColorSet::CXTPRibbonTabColorSet()
{
	m_bIsInactiveSelectedTextVisible = TRUE;
}

void CXTPRibbonTabColorSet::RefreshMetrics()
{
	CXTPTabColorSetOffice2003::RefreshMetrics();
}

void CXTPRibbonTabColorSet::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CXTPRibbonTab* pRibbonTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, pItem);
	CXTPRibbonBar* pRibbonBar = pRibbonTab->GetRibbonBar();

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

	// If m_bIsInactiveSelectedTextVisible is not true the color of normal (inactive) button text
	// won't be visible on the selected button
	if (bSelected && pRibbonBar->IsBackstageViewVisible() && m_bIsInactiveSelectedTextVisible)
		bSelected = FALSE;

	pDC->SetTextColor(!pItem->IsEnabled()
						  ? m_clrTextDisabled
						  : bSelected ? m_clrTextSelected
									  : bHighlighted ? m_clrTextHighlight : m_clrTextNormal);
}
