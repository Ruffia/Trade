// XTPDockingPaneTabColorSet2022.cpp
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

#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2022.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneTabColorSet2022::CXTPDockingPaneTabColorSet2022()
	: CXTPDockingPaneTabColorSet2012()
{
}

void CXTPDockingPaneTabColorSet2022::FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	UNREFERENCED_PARAMETER(pTabManager);
	pDC->FillSolidRect(rc, m_clrHeaderFace);
	pDC->FillSolidRect(rc.left, rc.top + 1, rc.Width(), 1, m_clrButtonBorder);
}
