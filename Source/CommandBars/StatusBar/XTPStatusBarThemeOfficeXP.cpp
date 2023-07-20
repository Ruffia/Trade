// XTPStatusBarThemeOfficeXP.cpp
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
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOfficeXP.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeOfficeXP::CXTPStatusBarThemeOfficeXP(CXTPPaintManager* pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeOfficeXP::DrawStatusBarPaneBorder(CDC* pDC, CRect rc,
														 CXTPStatusBarPane* /*pPane*/,
														 BOOL /*bGripperPane*/)
{
	m_pPaintManager->Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_3DSHADOW);
}

void CXTPStatusBarThemeOfficeXP::DrawStatusBarPaneSeparator(CDC* pDC, CRect rc,
															CXTPStatusBarPane* /*pPane*/)
{
	m_pPaintManager->VerticalLine(pDC, rc.left - XTP_DPI_X(2), rc.top, rc.bottom,
								  GetXtremeColor(XPCOLOR_SEPARATOR));
}
