// XTPDockingPaneThemeVisualStudio2008.cpp : implementation of the
// CXTPDockingPaneVisualStudio2008Theme class.
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
#include "Common/XTPWinThemeWrapper.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPanePaintManager.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2008.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2008Theme

CXTPDockingPaneVisualStudio2008Theme::CXTPDockingPaneVisualStudio2008Theme()
{
}

void CXTPDockingPaneVisualStudio2008Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Theme::RefreshMetrics();

	if (m_themeRebar->IsAppThemed())
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
		m_pPanelPaintManager->m_bHotTracking = TRUE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pTabPaintManager->SetColor(xtpTabColorWinNative);
		m_pTabPaintManager->m_bHotTracking = TRUE;
	}
}
