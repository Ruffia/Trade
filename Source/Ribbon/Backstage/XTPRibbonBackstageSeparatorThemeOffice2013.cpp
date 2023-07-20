// XTPRibbonBackstageSeparatorThemeOffice2013.cpp : implementation file
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"

#include "Controls/Util/XTPControlTheme.h"

#include "Ribbon/Backstage/XTPRibbonBackstageSeparator.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorPaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparatorThemeOffice2013

CXTPRibbonBackstageSeparatorThemeOffice2013::CXTPRibbonBackstageSeparatorThemeOffice2013()
{
	m_clrSeparator = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageSeparator"),
								 RGB(225, 225, 225));
	m_clrBack	  = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageBackground"),
							 RGB(255, 255, 255));
}

void CXTPRibbonBackstageSeparatorThemeOffice2013::DrawSeparator(
	CDC* pDC, CXTPRibbonBackstageSeparator* pSeparator)
{
	CXTPClientRect rc(pSeparator);

	if (pSeparator->IsVerticalStyle())
	{
		pDC->FillSolidRect(CRect(rc.right - XTP_DPI_X(1), rc.top, rc.right, rc.bottom),
						   m_clrSeparator);
	}
	else
	{
		pDC->FillSolidRect(CRect(rc.left, rc.Height() / 2, rc.right,
								 rc.Height() / 2 + XTP_DPI_Y(1)),
						   m_clrSeparator);
	}
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
