// XTPDockingPanePanelColorSet2012.cpp
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
#include "Common/XTPResourceImage.h"

#include "TabManager/Includes.h"

#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPanePanelColorSet2012::CXTPDockingPanePanelColorSet2012()
	: CXTPTabPaintManagerColorSet()
{
}

void CXTPDockingPanePanelColorSet2012::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	static LPCTSTR lpszSection = _T("DockingPane.Panel");

	m_clrButtonNormal	  = XTPIniColor(lpszSection, _T("ButtonNormal"), RGB(204, 206, 219));
	m_clrButtonSelected	= XTPIniColor(lpszSection, _T("ButtonSelected"), RGB(246, 246, 246));
	m_clrButtonHighlighted = XTPIniColor(lpszSection, _T("ButtonHighlighted"), RGB(28, 151, 234));
	m_clrButtonPressed	 = XTPIniColor(lpszSection, _T("ButtonPressed"), RGB(0, 122, 204));
	m_clrTextNormal		   = XTPIniColor(lpszSection, _T("TextNormal"), RGB(68, 68, 68));
	m_clrTextHighlight	 = XTPIniColor(lpszSection, _T("TextHighlight"), RGB(0, 122, 204));
	m_clrTextSelected	  = XTPIniColor(lpszSection, _T("TextSelected"), RGB(0, 122, 204));
	m_clrTextDisabled	  = XTPIniColor(lpszSection, _T("TextDisabled"), RGB(0, 0, 0));
	m_clrAutoHideFace	  = XTPIniColor(lpszSection, _T("AutoHideFace"), RGB(239, 239, 242));
}

COLORREF CXTPDockingPanePanelColorSet2012::GetItemColor(CXTPTabManagerItem* pItem)
{
	if (!pItem)
	{
		ASSERT(0);
		return COLORREF_NULL;
	}

	COLORREF clrItem;

	if (pItem->IsSelected())
	{
		clrItem = m_clrButtonSelected;
	}
	else if (pItem->IsPressed())
	{
		clrItem = m_clrButtonPressed;
	}
	else if (pItem->IsHighlighted())
	{
		clrItem = m_clrButtonHighlighted;
	}
	else
	{
		clrItem = m_clrButtonNormal;
	}

	return clrItem;
}

void CXTPDockingPanePanelColorSet2012::FillNavigateButton(CDC* pDC,
														  CXTPTabManagerNavigateButton* pButton,
														  CRect& rc)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pButton);
	UNREFERENCED_PARAMETER(rc);
}
