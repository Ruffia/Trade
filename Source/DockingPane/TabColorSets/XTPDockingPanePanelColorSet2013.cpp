// XTPDockingPanePanelColorSet2013.cpp
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
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPanePanelColorSet2013::CXTPDockingPanePanelColorSet2013()
	: CXTPDockingPanePanelColorSet2012()
{
}

void CXTPDockingPanePanelColorSet2013::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	static LPCTSTR lpszSection = _T("DockingPane.Panel");

	m_clrButtonNormal	  = XTPIniColor(lpszSection, _T("ButtonNormal"), RGB(212, 212, 212));
	m_clrButtonPressed	 = XTPIniColor(lpszSection, _T("ButtonPressed"), RGB(43, 87, 154));
	m_clrButtonSelected	= XTPIniColor(lpszSection, _T("ButtonSelected"), RGB(43, 87, 154));
	m_clrButtonHighlighted = XTPIniColor(lpszSection, _T("ButtonHighlighted"), RGB(62, 109, 181));
	m_clrButtonBorder	  = XTPIniColor(lpszSection, _T("ButtonBorder"), RGB(212, 212, 212));
	m_clrTextNormal		   = XTPIniColor(lpszSection, _T("TextNormal"), RGB(152, 152, 152));
	m_clrTextSelected	  = XTPIniColor(lpszSection, _T("TextSelected"), RGB(43, 87, 154));
	m_clrTextHighlight	 = XTPIniColor(lpszSection, _T("TextHighlight"), RGB(62, 109, 181));
	m_clrTextDisabled	  = XTPIniColor(lpszSection, _T("TextDisabled"), RGB(212, 212, 212));
	m_clrAutoHideFace	  = XTPIniColor(lpszSection, _T("AutoHideFace"), RGB(245, 245, 245));
}
