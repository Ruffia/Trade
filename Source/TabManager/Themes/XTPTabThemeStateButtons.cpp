// XTPTabThemeStateButtons.cpp
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
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerTheme.h"
#include "TabManager/Themes/XTPTabThemeStateButtons.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CAppearanceSetStateButtons

CRect CAppearanceSetStateButtons::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC,
												 CRect rcControl)
{
	CRect rcTab = GetHeaderRect(rcControl, pTabManager);
	pDC->FillSolidRect(rcTab, GetColorSet()->m_clrButtonNormal);
	return rcTab;
}

void CAppearanceSetStateButtons::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	GetColorSet()->FillStateButton(pDC, rcItem, pItem);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
