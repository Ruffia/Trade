// XTPTabCtrl.cpp : implementation of the CXTPTabCtrl class.
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

#include "Controls/Util/XTPControlTheme.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/Tab/XTPTabBase.h"
#include "Controls/Tab/XTPTabCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTabCtrl

IMPLEMENT_DYNAMIC(CXTPTabCtrl, CTabCtrl)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CXTPTabCtrl)
	ON_TABCTRLEX_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPTabCtrl construction/destruction

CXTPTabCtrl::CXTPTabCtrl()
{
	ImplAttach(this);
}

CXTPTabCtrl::~CXTPTabCtrl()
{
}

BOOL CXTPTabCtrl::EnableToolTips(BOOL bEnable)
{
	return CXTPTabCtrlBase::EnableToolTipsEx(bEnable);
}
