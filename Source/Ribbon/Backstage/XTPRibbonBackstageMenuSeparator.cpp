// XTPRibbonBackstageMenuSeparator.cpp : implementation file
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

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/Backstage/XTPRibbonBackstageMenuSeparator.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_XTP_CONTROL(CXTPRibbonBackstageMenuSeparator, CXTPControl)

CXTPRibbonBackstageMenuSeparator::CXTPRibbonBackstageMenuSeparator()
{
	SetFlags(GetFlags() | xtpFlagSkipFocus);
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonBackstageMenuSeparator, CXTPControl)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPRibbonBackstageMenuSeparator, CXTPControl)
	INTERFACE_PART(CXTPRibbonBackstageMenuSeparator, XTPDIID_RibbonBackstageMenuSeparator, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonBackstageMenuSeparator, XTPDIID_RibbonBackstageMenuSeparator)

#endif
