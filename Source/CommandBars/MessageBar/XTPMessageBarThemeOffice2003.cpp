// XTPMessageBarThemeOffice2003.cpp
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"

#include "CommandBars/MessageBar/XTPMessageBar.h"
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPMessageBarThemeOffice2003::CXTPMessageBarThemeOffice2003(CXTPCommandBarsOffice2003Theme* pTheme)
	: CXTPMessageBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPMessageBarThemeOffice2003::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rc((CWnd*)pBar);
	CRect rcFill(rc.left, rc.top, max(rc.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2),
				 rc.bottom);

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_pTheme->m_clrDockBar, TRUE, rc);

	CRect rcMessage = pBar->GetMessageRect();
	pDC->Draw3dRect(rcMessage, m_pTheme->m_clrToolbarShadow, m_pTheme->m_clrToolbarShadow);

	rcMessage.DeflateRect(1, 1);
	pDC->FillSolidRect(rcMessage, GetXtremeColor(COLOR_INFOBK));
}
