// XTPDockingPaneTabTheme2019.cpp
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

#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2012.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2019.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneTabTheme2019::CXTPDockingPaneTabTheme2019()
	: CXTPDockingPaneTabTheme2012()
{
}

void CXTPDockingPaneTabTheme2019::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CXTPTabPaintManagerColorSet* pColorSet = GetColorSet();

	CRect rcItem(pItem->GetRect());
	pDC->FillSolidRect(rcItem, pColorSet->GetItemColor(pItem));

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
			if (pItem->IsSelected())
			{
				pDC->FillSolidRect(rcItem.left, rcItem.top, XTP_DPI_X(1), rcItem.Height(),
								   pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.right - XTP_DPI_X(1), rcItem.top, XTP_DPI_X(1),
								   rcItem.Height(), pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.left, rcItem.top, rcItem.Width(), XTP_DPI_Y(1),
								   pColorSet->m_clrButtonBorder);
			}
			else
			{
				pDC->FillSolidRect(rcItem.left, rcItem.bottom - XTP_DPI_Y(1), rcItem.Width(),
								   XTP_DPI_Y(1), pColorSet->m_clrButtonBorder);
			}
			break;
		case xtpTabPositionBottom:
			if (pItem->IsSelected())
			{
				pDC->FillSolidRect(rcItem.left, rcItem.top, XTP_DPI_X(1), rcItem.Height(),
								   pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.right - XTP_DPI_X(1), rcItem.top, XTP_DPI_X(1),
								   rcItem.Height(), pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.left, rcItem.bottom - XTP_DPI_Y(1), rcItem.Width(),
								   XTP_DPI_Y(1), pColorSet->m_clrButtonBorder);
			}
			else
			{
				pDC->FillSolidRect(rcItem.left, rcItem.top + XTP_DPI_Y(1), rcItem.Width(),
								   XTP_DPI_Y(1), pColorSet->m_clrButtonBorder);
			}
			break;
		case xtpTabPositionLeft:
			if (pItem->IsSelected())
			{
				pDC->FillSolidRect(rcItem.left, rcItem.top, rcItem.Width(), XTP_DPI_Y(1),
								   pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.left, rcItem.bottom - XTP_DPI_Y(1), rcItem.Width(),
								   XTP_DPI_X(1), pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.left, rcItem.top, XTP_DPI_X(1), rcItem.Height(),
								   pColorSet->m_clrButtonBorder);
			}
			else
			{
				pDC->FillSolidRect(rcItem.right - XTP_DPI_X(1), rcItem.top, XTP_DPI_X(1),
								   rcItem.Height(), pColorSet->m_clrButtonBorder);
			}
			break;
		case xtpTabPositionRight:
			if (pItem->IsSelected())
			{
				pDC->FillSolidRect(rcItem.left, rcItem.top, rcItem.Width(), XTP_DPI_X(1),
								   pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.left, rcItem.bottom - XTP_DPI_Y(1), rcItem.Width(),
								   XTP_DPI_Y(1), pColorSet->m_clrButtonBorder);
				pDC->FillSolidRect(rcItem.right - XTP_DPI_X(1), rcItem.top, XTP_DPI_X(1),
								   rcItem.Height(), pColorSet->m_clrButtonBorder);
			}
			else
			{
				pDC->FillSolidRect(rcItem.left, rcItem.top, XTP_DPI_X(1), rcItem.Height(),
								   pColorSet->m_clrButtonBorder);
			}
			break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rcItem, TRUE);
}
