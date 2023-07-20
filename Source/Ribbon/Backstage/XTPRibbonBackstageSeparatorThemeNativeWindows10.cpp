// XTPRibbonBackstageSeparatorThemeNativeWindows10.cpp : implementation file
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

#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorPaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeOffice2013.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparatorThemeNativeWindows10

CXTPRibbonBackstageSeparatorThemeNativeWindows10::CXTPRibbonBackstageSeparatorThemeNativeWindows10()
{
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
