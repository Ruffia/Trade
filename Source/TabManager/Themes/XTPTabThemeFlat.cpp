// XTPTabThemeFlat.cpp
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
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerTheme.h"
#include "TabManager/Themes/XTPTabThemeFlat.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeFlat

void CXTPTabThemeFlat::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());

	InflateRectEx(rcItem, CRect(0, 0, 1, 0), pItem->GetTabManager()->GetPosition());

	GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
		InflateRectEx(rcItem, CRect(0, 0, 0, 1), pItem->GetTabManager()->GetPosition());

	pDC->Draw3dRect(rcItem, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}

CRect CXTPTabThemeFlat::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(),
							 GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}

	return rcHeader;
}
