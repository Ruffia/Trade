// XTPRibbonTabTheme.cpp: implementation of the CXTPRibbonTabTheme class.
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
#include "Common/XTPDrawHelpers.h"
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
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonTabTheme.h"
#include "Ribbon/XTPRibbonControlTab.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPRibbonTabTheme::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	m_pRibbonTheme->FillRibbonTabControl(pDC, ((CXTPRibbonControlTab*)pTabManager)->GetRibbonBar(),
										 rcControl);

	CAppearanceSetPropertyPageFlat::DrawTabControl(pTabManager, pDC, rcControl);
}

void CXTPRibbonTabTheme::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	m_pRibbonTheme->DrawRibbonTab(pDC, (CXTPRibbonTab*)pItem);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}

void CXTPRibbonTabTheme::DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str,
								  LPRECT lpRect, UINT nFormat)
{
	CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pManager;
	CXTPRibbonBar* pRibbonBar		  = pControlTab->GetRibbonBar();

	if (nFormat & DT_CALCRECT)
		pDC->DrawText(str, lpRect, nFormat);
	else
	{
		BOOL bComposite = IsCompositeRect(pRibbonBar, lpRect);
		CXTPPaintManager::DrawTextApi(pDC, str, lpRect, nFormat, bComposite);
	}
}

int CXTPRibbonTabTheme::GetButtonHeight(const CXTPTabManager* pTabManager)
{
	UNREFERENCED_PARAMETER(pTabManager);
	return m_pRibbonTheme->m_nTabsHeight + XTP_DPI_Y(1);
}
