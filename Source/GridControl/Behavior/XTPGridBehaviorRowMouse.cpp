// XTPGridBehaviorRowMouse.cpp : implementation of the CXTPGridBehaviorRowMouse class.
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

#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/Behavior/XTPGridBehavior.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPGridBehaviorRowMouse::CXTPGridBehaviorRowMouse(XTPGridMouseEvent event)

	: Left(NULL)
	, Right(NULL)
{
	Left  = new CXTPGridBehaviorRowMouseButton(xtpGridMouseButtonLeft, event);
	Right = new CXTPGridBehaviorRowMouseButton(xtpGridMouseButtonRight, event);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridBehaviorRowMouse::~CXTPGridBehaviorRowMouse()
{
	SAFE_DELETE(Left);
	SAFE_DELETE(Right);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridBehaviorRowMouse, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridBehaviorRowMouse, "LeftButton", 1, OleGetLeft, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridBehaviorRowMouse, "RightButton", 2, OleGetRight, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridBehaviorRowMouse, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridBehaviorRowMouse, XTPDIID_IGridBehaviorRowMouse, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridBehaviorRowMouse, XTPDIID_IGridBehaviorRowMouse)

LPDISPATCH CXTPGridBehaviorRowMouse::OleGetLeft()
{
	return XTPGetDispatch(Left);
}

LPDISPATCH CXTPGridBehaviorRowMouse::OleGetRight()
{
	return XTPGetDispatch(Right);
}

#endif // _XTP_ACTIVEX
