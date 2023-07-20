// XTPGridBehaviorRowMouseButton.cpp : implementation of the CXTPGridBehaviorRowMouseButton
// class.
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

CXTPGridBehaviorRowMouseButton::CXTPGridBehaviorRowMouseButton(XTPGridMouseButton button,
															   XTPGridMouseEvent event)
	: bFocusColumn(FALSE)
	, bEnsureVisible(FALSE)
	, bClick(FALSE)
	, bContextMenu(FALSE)
	, bCheckSelectedRows(FALSE)
	, bDragBegin(FALSE)
	, bDragEnd(FALSE)
	, bDragSelectGroup(FALSE)

	, m_button(button)
	, m_event(event)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	None	= new CXTPGridBehaviorRowModifier();
	Control = new CXTPGridBehaviorRowModifier();
	Shift   = new CXTPGridBehaviorRowModifier();
}

CXTPGridBehaviorRowMouseButton::~CXTPGridBehaviorRowMouseButton()
{
	SAFE_DELETE(None);
	SAFE_DELETE(Control);
	SAFE_DELETE(Shift);
}

void CXTPGridBehaviorRowMouseButton::Reset()
{
	None->Reset();
	Control->Reset();
	Shift->Reset();

	bFocusColumn	   = FALSE;
	bEnsureVisible	 = FALSE;
	bClick			   = FALSE;
	bContextMenu	   = FALSE;
	bCheckSelectedRows = FALSE;
	bDragBegin		   = FALSE;
	bDragEnd		   = FALSE;
	bDragSelectGroup   = FALSE;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridBehaviorRowMouseButton, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, FocusColumn, 1, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, EnsureVisible, 2, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, Click, 3, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, ContextMenu, 4, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, CheckSelectedRows, 5, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, DragBegin, 6, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, DragEnd, 7, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowMouseButton, DragSelectGroup, 8, VT_BOOL)

	DISP_FUNCTION_ID(CXTPGridBehaviorRowMouseButton, "None", 9, OleGetNone, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridBehaviorRowMouseButton, "Control", 10, OleGetControl, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridBehaviorRowMouseButton, "Shift", 11, OleGetShift, VT_DISPATCH,
					 VTS_NONE)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridBehaviorRowMouseButton, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridBehaviorRowMouseButton, XTPDIID_IGridBehaviorRowMouseButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridBehaviorRowMouseButton, XTPDIID_IGridBehaviorRowMouseButton)

XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, FocusColumn, bFocusColumn, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, EnsureVisible, bEnsureVisible, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, Click, bClick, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, ContextMenu, bContextMenu, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, CheckSelectedRows, bCheckSelectedRows, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, DragBegin, bDragBegin, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, DragEnd, bDragEnd, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowMouseButton, DragSelectGroup, bDragSelectGroup, BOOL)

LPDISPATCH CXTPGridBehaviorRowMouseButton::OleGetNone()
{
	return XTPGetDispatch(None);
}

LPDISPATCH CXTPGridBehaviorRowMouseButton::OleGetControl()
{
	return XTPGetDispatch(Control);
}

LPDISPATCH CXTPGridBehaviorRowMouseButton::OleGetShift()
{
	return XTPGetDispatch(Shift);
}

#endif // _XTP_ACTIVEX
