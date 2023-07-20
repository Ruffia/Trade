// XTPDockingPaneThemeVisualStudio2005.cpp : implementation of the
// CXTPDockingPaneVisualStudio2005Theme class.
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

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPanePaintManager.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005.h"

#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2005.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2005Theme

CXTPDockingPaneVisualStudio2005Theme::CXTPDockingPaneVisualStudio2005Theme()
{
	CAppearanceSetPropertyPageFlat* pAppearance = new CAppearanceSetPropertyPageFlat;
	pAppearance->m_bBlurPoints					= FALSE;

	m_pPanelPaintManager->SetAppearanceSet(pAppearance);
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(FALSE));
	m_pPanelPaintManager->m_bRotateImageOnVerticalDraw = TRUE;

	pAppearance							  = new CAppearanceSetPropertyPageFlat;
	pAppearance->m_bBlurPoints			  = FALSE;
	pAppearance->m_bVisualStudio2005Style = TRUE;

	m_pTabPaintManager->SetAppearanceSet(pAppearance);
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(TRUE));

	m_pTabPaintManager->m_clientFrame		 = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_rcClientMargin.top = XTP_DPI_X(1);
}
