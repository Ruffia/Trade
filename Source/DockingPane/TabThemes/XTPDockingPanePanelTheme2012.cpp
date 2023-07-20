// XTPDockingPanePanelTheme2012.cpp
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
#include "Common/XTPResourceImage.h"

#include "TabManager/Includes.h"

#include "DockingPane/TabThemes/XTPDockingPanePanelTheme2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPanePanelTheme2012::CXTPDockingPanePanelTheme2012()
	: CXTPTabThemeFlat()
{
}

void CXTPDockingPanePanelTheme2012::RefreshMetrics()
{
	CXTPTabThemeFlat::RefreshMetrics();

	m_nGripperWidth  = XTP_DPI_X(XTPIniInt(_T("DockingPane.Panel"), _T("GripperWidth"), 6));
	m_nButtonHeight  = XTP_DPI_Y(XTPIniInt(_T("DockingPane.Panel"), _T("ButtonHeight"), 29));
	m_rcHeaderMargin = XTP_DPI(
		XTPIniRect(_T("DockingPane.Panel"), _T("HeaderMargin"), CRect(0, 0, 0, 0)));
}

void CXTPDockingPanePanelTheme2012::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CXTPTabPaintManagerColorSet* pColorSet = GetColorSet();

	CRect rcItem(pItem->GetRect());

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.left, rcItem.bottom - XTP_DPI_Y(m_nGripperWidth), rcItem.right,
							rcItem.bottom);
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.right += XTP_DPI_X(5);
			rcItem.left -= XTP_DPI_X(5);
			rcItem.bottom -= XTP_DPI_Y(m_nGripperWidth);
		}
		break;

		case xtpTabPositionBottom:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.left, rcItem.top, rcItem.right,
							rcItem.top + XTP_DPI_Y(m_nGripperWidth));
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.right += XTP_DPI_X(5);
			rcItem.left -= XTP_DPI_X(5);
			rcItem.top += XTP_DPI_Y(m_nGripperWidth);
		}
		break;

		case xtpTabPositionLeft:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.right - XTP_DPI_X(m_nGripperWidth), rcItem.top, rcItem.right,
							rcItem.bottom - XTP_DPI_Y(10));
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.right -= XTP_DPI_X(m_nGripperWidth);
			rcItem.top -= XTP_DPI_Y(5);
			rcItem.bottom -= XTP_DPI_Y(5);
		}
		break;

		case xtpTabPositionRight:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.left, rcItem.top, rcItem.left + XTP_DPI_X(m_nGripperWidth),
							rcItem.bottom - XTP_DPI_Y(10));
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.left += XTP_DPI_X(m_nGripperWidth);
			rcItem.top -= XTP_DPI_Y(5);
			rcItem.bottom -= XTP_DPI_Y(5);
		}
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rcItem, TRUE);
}
