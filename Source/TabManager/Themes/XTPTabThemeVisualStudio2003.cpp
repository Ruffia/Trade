// XTPTabThemeVisualStudio2003.cpp
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
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManager.inl"
#include "TabManager/XTPTabPaintManagerTheme.h"

#include "TabManager/Themes/XTPTabThemeVisualStudio2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeVisualStudio2003

void CXTPTabThemeVisualStudio2003::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	if (!pItem->IsSelected())
	{
		if (!IsHorizontalTabPosition(pItem->GetTabManager()->GetPosition()))
		{
			HorizontalLine(pDC, rcItem.left + 2, rcItem.bottom, rcItem.Width() - 4,
						   GetColorSet()->m_clrFrameBorder);
		}
		else
		{
			VerticalLine(pDC, rcItem.right, rcItem.top + 2, rcItem.Height() - 4,
						 GetColorSet()->m_clrFrameBorder);
		}
	}
	else
	{
		GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

		InflateRectEx(rcItem, CRect(0, 0, 0, 1), pItem->GetTabManager()->GetPosition());
		pDC->Draw3dRect(rcItem, GetColorSet()->m_clrBorderHighlight,
						GetColorSet()->m_clrTextSelected);
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}

CRect CXTPTabThemeVisualStudio2003::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC,
												   CRect rcControl)
{
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrBorderHighlight,
						GetColorSet()->m_clrTextSelected);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(),
							 GetColorSet()->m_clrBorderHighlight, GetColorSet()->m_clrTextSelected);
	}

	return rcHeader;
}
