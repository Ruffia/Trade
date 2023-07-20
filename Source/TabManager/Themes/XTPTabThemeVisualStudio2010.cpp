// XTPTabThemeVisualStudio2010.cpp
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

#include "TabManager/Includes.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2010.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeVisualStudio2010

void CXTPTabThemeVisualStudio2010::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcTab(rcItem);

	InflateRectEx(rcTab, CRect(0, 0, 0, 1), tabPosition);

	XTPDrawHelpers()->ExcludeCorners(pDC, rcTab, TRUE);

	if (!m_bDrawNormalTab && !pItem->IsSelected() && !pItem->IsHighlighted())
	{
		m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
		return;
	}

	CXTPTabPaintManagerColorSet* pColorSet = GetColorSet();
	COLORREF clrFrameBorder				   = pItem->IsHighlighted() && !pItem->IsSelected()
								  ? pColorSet->m_clrFrameHighlightBorder.operator COLORREF()
								  : COLORREF_NULL;
	if (clrFrameBorder == COLORREF_NULL)
		clrFrameBorder = pColorSet->m_clrFrameBorder;

	pColorSet->FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
	{
		if (!pColorSet->m_clrFrameHighlightBorder.IsDefaultValue())
			InflateRectEx(rcTab, CRect(1, 1, 1, 0), tabPosition);
		else
			pDC->Draw3dRect(rcTab, clrFrameBorder, clrFrameBorder);
	}
	else
	{
		pDC->Draw3dRect(rcTab, clrFrameBorder, clrFrameBorder);
	}

	if (m_bBlurPoints)
	{
		BlurTab(pDC, rcTab, pColorSet->m_clrHeaderFace, clrFrameBorder, tabPosition);
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
