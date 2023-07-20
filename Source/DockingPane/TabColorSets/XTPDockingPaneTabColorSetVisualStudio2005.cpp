// XTPDockingPaneTabColorSetVisualStudio2005.cpp
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

#include "TabManager/Includes.h"

#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2005.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneTabColorSetVisualStudio2005::CXTPDockingPaneTabColorSetVisualStudio2005(
	BOOL bHighlightSelected)
	: CXTPDockingPaneTabColorSetVisualStudio2003(bHighlightSelected)
{
	m_bTransparentButton = FALSE;
}

void CXTPDockingPaneTabColorSetVisualStudio2005::RefreshMetrics()
{
	CXTPDockingPaneTabColorSetVisualStudio2003 ::RefreshMetrics();

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
		case xtpSystemThemeSilver:
			m_bGradientButton = TRUE;
			m_clrButtonNormal.SetStandardValue(RGB(242, 242, 247));
			m_clrHeaderFace.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));

			m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
			m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(145, 155, 156));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(145, 155, 156));
			m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(190, 190, 216));
			break;

		case xtpSystemThemeBlue:
			m_bGradientButton = TRUE;
			m_clrHeaderFace.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));

			m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
			break;

		case xtpSystemThemeOlive:
			m_bGradientButton = TRUE;
			m_clrHeaderFace.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));

			m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
			break;
	}

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);
}
