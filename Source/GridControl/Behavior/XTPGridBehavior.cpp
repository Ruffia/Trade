// XTPGridBehavior.cpp : implementation of the CXTPGridBehavior class.
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

CXTPGridBehaviorRow::CXTPGridBehaviorRow()
	: MouseDown(NULL)
	, MouseUp(NULL)
{
	MouseDown = new CXTPGridBehaviorRowMouse(xtpGridMouseEventButtonDown);
	MouseUp   = new CXTPGridBehaviorRowMouse(xtpGridMouseEventButtonUp);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridBehaviorRow::~CXTPGridBehaviorRow()
{
	SAFE_DELETE(MouseDown);
	SAFE_DELETE(MouseUp);
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridBehaviorRow, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridBehaviorRow, XTPDIID_IGridBehaviorRow, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridBehaviorRow, XTPDIID_IGridBehaviorRow)

BEGIN_DISPATCH_MAP(CXTPGridBehaviorRow, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridBehaviorRow, "MouseDown", 1, OleGetMouseDown, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridBehaviorRow, "MouseUp", 2, OleGetMouseUp, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

LPDISPATCH CXTPGridBehaviorRow::OleGetMouseDown()
{
	return XTPGetDispatch(MouseDown);
}

LPDISPATCH CXTPGridBehaviorRow::OleGetMouseUp()
{
	return XTPGetDispatch(MouseUp);
}

#endif // _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////////

CXTPGridNotifications::CXTPGridNotifications()
	: Populate(NULL)
	, SelectedRows(NULL)
	, Row(NULL)
{
	Populate	 = new CXTPGridNotificationsPopulate();
	SelectedRows = new CXTPGridNotificationsSelectedRows();
	Row			 = new CXTPGridNotificationsRow();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridNotifications::~CXTPGridNotifications()
{
	SAFE_DELETE(Populate);
	SAFE_DELETE(SelectedRows);
	SAFE_DELETE(Row);
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridNotifications, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridNotifications, XTPDIID_IGridNotifications, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridNotifications, XTPDIID_IGridNotifications)

BEGIN_DISPATCH_MAP(CXTPGridNotifications, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridNotifications, "Row", 1, OleGetRow, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridNotifications, "SelectedRows", 2, OleGetSelectedRows, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridNotifications, "Populate", 3, OleGetPopulate, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

LPDISPATCH CXTPGridNotifications::OleGetRow()
{
	return XTPGetDispatch(Row);
}

LPDISPATCH CXTPGridNotifications::OleGetSelectedRows()
{
	return XTPGetDispatch(SelectedRows);
}

LPDISPATCH CXTPGridNotifications::OleGetPopulate()
{
	return XTPGetDispatch(Populate);
}

#endif // _XTP_ACTIVEX

CXTPGridNotificationsPopulate::CXTPGridNotificationsPopulate()
	: bFocusChanging(FALSE)
	, bSelectionChanging(FALSE)
	, bSelectionChanged(FALSE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridNotificationsPopulate::~CXTPGridNotificationsPopulate()
{
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridNotificationsPopulate, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridNotificationsPopulate, XTPDIID_IGridNotificationsPopulate, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridNotificationsPopulate, XTPDIID_IGridNotificationsPopulate)

BEGIN_DISPATCH_MAP(CXTPGridNotificationsPopulate, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsPopulate, FocusChangingNotification, 1, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsPopulate, SelectionChangingNotification, 2,
							VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsPopulate, SelectionChangedNotification, 3, VT_BOOL)
END_DISPATCH_MAP()

XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsPopulate, FocusChangingNotification, bFocusChanging,
					   BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsPopulate, SelectionChangingNotification,
					   bSelectionChanging, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsPopulate, SelectionChangedNotification,
					   bSelectionChanged, BOOL)

#endif // _XTP_ACTIVEX

CXTPGridNotificationsSelectedRows::CXTPGridNotificationsSelectedRows()
	: bClear(FALSE)
	, bAdd(FALSE)
	, bRemove(FALSE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridNotificationsSelectedRows::~CXTPGridNotificationsSelectedRows()
{
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridNotificationsSelectedRows, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridNotificationsSelectedRows, XTPDIID_IGridNotificationsSelectedRows,
				   Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridNotificationsSelectedRows, XTPDIID_IGridNotificationsSelectedRows)

BEGIN_DISPATCH_MAP(CXTPGridNotificationsSelectedRows, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsSelectedRows, ClearNotification, 1, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsSelectedRows, AddNotification, 2, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsSelectedRows, RemoveNotification, 3, VT_BOOL)
END_DISPATCH_MAP()

XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsSelectedRows, ClearNotification, bClear, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsSelectedRows, AddNotification, bAdd, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsSelectedRows, RemoveNotification, bRemove, BOOL)

#endif // _XTP_ACTIVEX

CXTPGridNotificationsRow::CXTPGridNotificationsRow()
	: bRowExpanded(TRUE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridNotificationsRow::~CXTPGridNotificationsRow()
{
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridNotificationsRow, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridNotificationsRow, XTPDIID_IGridNotificationsRow, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridNotificationsRow, XTPDIID_IGridNotificationsRow)

BEGIN_DISPATCH_MAP(CXTPGridNotificationsRow, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridNotificationsRow, RowExpandedNotification, 1, VT_BOOL)
END_DISPATCH_MAP()

XTP_IMPLEMENT_PROPERTY(CXTPGridNotificationsRow, RowExpandedNotification, bRowExpanded, BOOL)

#endif // _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////////

CXTPGridBehavior::CXTPGridBehavior(XTPGridBehavior scheme)
	: Row(NULL)
	, GroupRow(NULL)
	, Notifications(NULL)
	, m_scheme(scheme)
{
	Row			  = new CXTPGridBehaviorRow();
	GroupRow	  = new CXTPGridBehaviorRow();
	Notifications = new CXTPGridNotifications();

	SetScheme(scheme);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridBehavior::~CXTPGridBehavior()
{
	SAFE_DELETE(Row);
	SAFE_DELETE(GroupRow);
	SAFE_DELETE(Notifications);
}

void CXTPGridBehavior::SetScheme(XTPGridBehavior behavior)
{
	m_scheme = behavior;

	Row->MouseDown->Left->Reset();
	Row->MouseDown->Right->Reset();
	Row->MouseUp->Left->Reset();
	Row->MouseUp->Right->Reset();

	switch (behavior)
	{
		case xtpGridBehaviorOutlook2003:
		case xtpGridBehaviorOutlook2007:
		case xtpGridBehaviorOutlook2010: SetSchemeOutlook(); break;

		case xtpGridBehaviorExcel2003:
		case xtpGridBehaviorExcel2007:
		case xtpGridBehaviorExcel2010: SetSchemeExcel(); break;

		case xtpGridBehaviorCodejock1340:
		case xtpGridBehaviorCodejock1341:
		case xtpGridBehaviorCodejock1342: SetSchemeCodejock134x(); break;

		case xtpGridBehaviorCodejock1501: SetSchemeCodejock1501(); break;

		case xtpGridBehaviorCodejock1502: SetSchemeCodejock1502(); break;

		case xtpGridBehaviorCodejockDefault: SetSchemeCodejockDefault(); break;

		default:
			ASSERT(FALSE); // Invalid scheme
			break;
	}
}

void CXTPGridBehavior::SetSchemeExcel()
{
	//////////////////////////////////////////////////////////////////////////
	// Left mouse button
	//////////////////////////////////////////////////////////////////////////

	Row->MouseDown->Left->None->bFocusRow			  = TRUE;
	Row->MouseDown->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->None->bSelectRow			  = TRUE;
	Row->MouseDown->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->bFocusColumn				  = TRUE;
	Row->MouseDown->Left->bEnsureVisible			  = FALSE;
	Row->MouseDown->Left->bClick					  = TRUE;
	Row->MouseDown->Left->bContextMenu				  = FALSE;
	Row->MouseDown->Left->bCheckSelectedRows		  = FALSE;
	Row->MouseDown->Left->bDragBegin				  = FALSE;
	Row->MouseDown->Left->bDragEnd					  = FALSE;

	Row->MouseUp->Left->None->bFocusRow				= FALSE; // Set on mouse down
	Row->MouseUp->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->None->bSelectRow			= FALSE; // Set on mouse down
	Row->MouseUp->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->bFocusColumn				= FALSE; // Set on mouse down
	Row->MouseUp->Left->bEnsureVisible				= FALSE;
	Row->MouseUp->Left->bClick						= FALSE; //
	Row->MouseUp->Left->bContextMenu				= FALSE;
	Row->MouseUp->Left->bCheckSelectedRows			= FALSE;
	Row->MouseUp->Left->bDragBegin					= FALSE;
	Row->MouseUp->Left->bDragEnd					= TRUE;

	//////////////////////////////////////////////////////////////////////////
	// Right mouse button
	//////////////////////////////////////////////////////////////////////////

	Row->MouseDown->Right->None->bFocusRow			   = TRUE;
	Row->MouseDown->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->None->bSelectRow			   = TRUE;
	Row->MouseDown->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->bFocusColumn				   = TRUE;
	Row->MouseDown->Right->bEnsureVisible			   = FALSE;
	Row->MouseDown->Right->bClick					   = FALSE;
	Row->MouseDown->Right->bContextMenu				   = TRUE;
	Row->MouseDown->Right->bCheckSelectedRows		   = FALSE;
	Row->MouseDown->Right->bDragBegin				   = FALSE;
	Row->MouseDown->Right->bDragEnd					   = FALSE;

	Row->MouseUp->Right->None->bFocusRow			 = FALSE;
	Row->MouseUp->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->None->bSelectRow			 = FALSE;
	Row->MouseUp->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->bFocusColumn				 = FALSE;
	Row->MouseUp->Right->bEnsureVisible				 = FALSE;
	Row->MouseUp->Right->bClick						 = FALSE;
	Row->MouseUp->Right->bContextMenu				 = FALSE;
	Row->MouseUp->Right->bCheckSelectedRows			 = FALSE;
	Row->MouseUp->Right->bDragBegin					 = FALSE;
	Row->MouseUp->Right->bDragEnd					 = TRUE;
}

void CXTPGridBehavior::SetSchemeCodejock134x()
{
	//////////////////////////////////////////////////////////////////////////
	// Left mouse button
	//////////////////////////////////////////////////////////////////////////

	Row->MouseDown->Left->None->bFocusRow			  = TRUE;
	Row->MouseDown->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->None->bSelectRow			  = TRUE;
	Row->MouseDown->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->None->bMultipleSelection	= FALSE;
	Row->MouseDown->Left->None->bKeepFocus			  = TRUE; // Drag multi selection
	Row->MouseDown->Left->None->bKeepSelection		  = TRUE; // Drag multi selection

	Row->MouseDown->Left->Shift->bFocusRow			   = TRUE;
	Row->MouseDown->Left->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->Shift->bSelectRow			   = TRUE;
	Row->MouseDown->Left->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->Shift->bMultipleSelection	= TRUE;
	Row->MouseDown->Left->Shift->bKeepFocus			   = FALSE; // Block selection
	Row->MouseDown->Left->Shift->bKeepSelection		   = FALSE; // Block selection

	Row->MouseDown->Left->Control->bFocusRow			 = FALSE;
	Row->MouseDown->Left->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->Control->bSelectRow			 = FALSE;
	Row->MouseDown->Left->Control->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->Control->bMultipleSelection	= TRUE;
	Row->MouseDown->Left->Control->bKeepFocus			 = TRUE; // Copy multi selection
	Row->MouseDown->Left->Control->bKeepSelection		 = TRUE; // Copy multi selection

	Row->MouseDown->Left->bFocusColumn		 = TRUE;
	Row->MouseDown->Left->bEnsureVisible	 = FALSE;
	Row->MouseDown->Left->bClick			 = FALSE;
	Row->MouseDown->Left->bContextMenu		 = FALSE;
	Row->MouseDown->Left->bCheckSelectedRows = FALSE;
	Row->MouseDown->Left->bDragBegin		 = TRUE;
	Row->MouseDown->Left->bDragEnd			 = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Left mouse button up
	//////////////////////////////////////////////////////////////////////////

	Row->MouseUp->Left->None->bFocusRow				= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->None->bSelectRow			= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->None->bMultipleSelection	= FALSE;
	Row->MouseUp->Left->None->bKeepFocus			= FALSE;
	Row->MouseUp->Left->None->bKeepSelection		= FALSE;

	Row->MouseUp->Left->Shift->bFocusRow			 = FALSE;
	Row->MouseUp->Left->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->Shift->bSelectRow			 = FALSE;
	Row->MouseUp->Left->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->Shift->bMultipleSelection	= FALSE;
	Row->MouseUp->Left->Shift->bKeepFocus			 = FALSE;
	Row->MouseUp->Left->Shift->bKeepSelection		 = FALSE;

	Row->MouseUp->Left->Control->bFocusRow			   = TRUE;
	Row->MouseUp->Left->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->Control->bSelectRow			   = TRUE;
	Row->MouseUp->Left->Control->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->Control->bMultipleSelection	= TRUE;
	Row->MouseUp->Left->Control->bKeepFocus			   = TRUE; // Ctrl/Shift
	Row->MouseUp->Left->Control->bKeepSelection		   = TRUE; // Ctrl/Shift

	Row->MouseUp->Left->bFocusColumn	   = FALSE;
	Row->MouseUp->Left->bEnsureVisible	 = TRUE; // Ensure visible on mouse up
	Row->MouseUp->Left->bClick			   = TRUE;
	Row->MouseUp->Left->bContextMenu	   = FALSE;
	Row->MouseUp->Left->bCheckSelectedRows = FALSE;
	Row->MouseUp->Left->bDragBegin		   = FALSE;
	Row->MouseUp->Left->bDragEnd		   = TRUE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button down
	///////////////////////////////////////////////////////////////////////

	Row->MouseDown->Right->None->bFocusRow			   = TRUE;
	Row->MouseDown->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->None->bSelectRow			   = TRUE;
	Row->MouseDown->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->None->bMultipleSelection	= FALSE;
	Row->MouseDown->Right->None->bKeepSelection		   = TRUE;
	Row->MouseDown->Right->None->bKeepFocus			   = FALSE;

	Row->MouseDown->Right->Shift->bFocusRow				= TRUE;
	Row->MouseDown->Right->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->Shift->bSelectRow			= TRUE;
	Row->MouseDown->Right->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->Shift->bMultipleSelection	= FALSE;
	Row->MouseDown->Right->Shift->bKeepSelection		= TRUE;
	Row->MouseDown->Right->Shift->bKeepFocus			= FALSE;

	Row->MouseDown->Right->Control->bFocusRow			  = TRUE;
	Row->MouseDown->Right->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->Control->bSelectRow			  = TRUE;
	Row->MouseDown->Right->Control->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->Control->bMultipleSelection	= FALSE;
	Row->MouseDown->Right->Control->bKeepSelection		  = TRUE;
	Row->MouseDown->Right->Control->bKeepFocus			  = FALSE;

	Row->MouseDown->Right->bFocusColumn		  = FALSE;
	Row->MouseDown->Right->bEnsureVisible	 = TRUE;
	Row->MouseDown->Right->bClick			  = FALSE;
	Row->MouseDown->Right->bContextMenu		  = FALSE;
	Row->MouseDown->Right->bCheckSelectedRows = FALSE;
	Row->MouseDown->Right->bDragBegin		  = FALSE;
	Row->MouseDown->Right->bDragEnd			  = FALSE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button up
	///////////////////////////////////////////////////////////////////////

	Row->MouseUp->Right->None->bFocusRow	  = TRUE;
	Row->MouseUp->Right->None->bSelectRow	 = TRUE;
	Row->MouseUp->Right->None->bKeepSelection = TRUE;

	Row->MouseUp->Right->bEnsureVisible = TRUE;
	Row->MouseUp->Right->bClick			= FALSE;
	Row->MouseUp->Right->bContextMenu   = TRUE;
	Row->MouseUp->Right->bDragEnd		= TRUE;
}

void CXTPGridBehavior::SetSchemeCodejock1501()
{
	//////////////////////////////////////////////////////////////////////////
	// Left mouse button down
	//////////////////////////////////////////////////////////////////////////

	Row->MouseDown->Left->None->bFocusRow			  = TRUE;
	Row->MouseDown->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->None->bSelectRow			  = TRUE;
	Row->MouseDown->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->None->bKeepFocus			  = TRUE; // Drag multi selection
	Row->MouseDown->Left->None->bKeepSelection		  = TRUE; // Drag multi selection

	Row->MouseDown->Left->bFocusColumn		 = TRUE;
	Row->MouseDown->Left->bEnsureVisible	 = FALSE;
	Row->MouseDown->Left->bClick			 = FALSE;
	Row->MouseDown->Left->bContextMenu		 = FALSE;
	Row->MouseDown->Left->bCheckSelectedRows = FALSE;
	Row->MouseDown->Left->bDragBegin		 = TRUE;
	Row->MouseDown->Left->bDragEnd			 = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Left mouse button up
	//////////////////////////////////////////////////////////////////////////

	Row->MouseUp->Left->None->bFocusRow				= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->None->bSelectRow			= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bSelectRowTemporarily = FALSE;

	Row->MouseUp->Left->bFocusColumn   = FALSE;
	Row->MouseUp->Left->bEnsureVisible = TRUE; // Ensure visible on mouse up
	Row->MouseUp->Left->bClick		   = TRUE;
	Row->MouseUp->Left->bDragBegin	 = FALSE;
	Row->MouseUp->Left->bDragEnd	   = TRUE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button
	///////////////////////////////////////////////////////////////////////

	Row->MouseUp->Right->bClick		  = TRUE;
	Row->MouseUp->Right->bContextMenu = TRUE;
}

void CXTPGridBehavior::SetSchemeCodejock1502()
{
	//////////////////////////////////////////////////////////////////////////
	// Left mouse button down
	//////////////////////////////////////////////////////////////////////////

	Row->MouseDown->Left->None->bFocusRow			  = TRUE;
	Row->MouseDown->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->None->bSelectRow			  = TRUE;
	Row->MouseDown->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->None->bMultipleSelection	= FALSE;
	Row->MouseDown->Left->None->bKeepFocus			  = TRUE; // Drag multi selection
	Row->MouseDown->Left->None->bKeepSelection		  = TRUE; // Drag multi selection

	Row->MouseDown->Left->Control->bFocusRow			 = FALSE; // 15.0.2
	Row->MouseDown->Left->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->Control->bSelectRow			 = FALSE; // 15.0.2
	Row->MouseDown->Left->Control->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->Control->bMultipleSelection	= FALSE;
	Row->MouseDown->Left->Control->bKeepFocus			 = TRUE; // Copy multi selection
	Row->MouseDown->Left->Control->bKeepSelection		 = TRUE; // Copy multi selection

	Row->MouseDown->Left->Shift->bFocusRow			   = TRUE;
	Row->MouseDown->Left->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->Shift->bSelectRow			   = TRUE;
	Row->MouseDown->Left->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->Shift->bMultipleSelection	= TRUE;
	Row->MouseDown->Left->Shift->bKeepFocus			   = FALSE; // Block selection
	Row->MouseDown->Left->Shift->bKeepSelection		   = FALSE; // Block selection

	Row->MouseDown->Left->bFocusColumn		 = TRUE;
	Row->MouseDown->Left->bEnsureVisible	 = FALSE;
	Row->MouseDown->Left->bClick			 = FALSE;
	Row->MouseDown->Left->bContextMenu		 = FALSE;
	Row->MouseDown->Left->bCheckSelectedRows = FALSE;
	Row->MouseDown->Left->bDragBegin		 = TRUE;
	Row->MouseDown->Left->bDragEnd			 = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Left mouse button up
	//////////////////////////////////////////////////////////////////////////

	Row->MouseUp->Left->None->bFocusRow				= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->None->bSelectRow			= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->None->bMultipleSelection	= FALSE;
	Row->MouseUp->Left->None->bKeepFocus			= FALSE;
	Row->MouseUp->Left->None->bKeepSelection		= FALSE;

	Row->MouseUp->Left->Control->bFocusRow			   = TRUE; // 15.0.2
	Row->MouseUp->Left->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->Control->bSelectRow			   = TRUE; // 15.0.2
	Row->MouseUp->Left->Control->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->Control->bMultipleSelection	= TRUE;
	Row->MouseUp->Left->Control->bKeepFocus			   = FALSE;
	Row->MouseUp->Left->Control->bKeepSelection		   = FALSE;

	Row->MouseUp->Left->Shift->bFocusRow			 = FALSE;
	Row->MouseUp->Left->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->Shift->bSelectRow			 = FALSE;
	Row->MouseUp->Left->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->Shift->bKeepFocus			 = FALSE;
	Row->MouseUp->Left->Shift->bKeepSelection		 = FALSE;

	Row->MouseUp->Left->bFocusColumn	   = FALSE;
	Row->MouseUp->Left->bEnsureVisible	 = TRUE; // Ensure visible on mouse up
	Row->MouseUp->Left->bClick			   = TRUE;
	Row->MouseUp->Left->bContextMenu	   = FALSE;
	Row->MouseUp->Left->bCheckSelectedRows = FALSE;
	Row->MouseUp->Left->bDragBegin		   = FALSE;
	Row->MouseUp->Left->bDragEnd		   = TRUE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button down
	///////////////////////////////////////////////////////////////////////

	Row->MouseDown->Right->None->bFocusRow			   = TRUE;
	Row->MouseDown->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->None->bSelectRow			   = TRUE;
	Row->MouseDown->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->None->bMultipleSelection	= FALSE;
	Row->MouseDown->Right->None->bKeepFocus			   = FALSE;
	Row->MouseDown->Right->None->bKeepSelection		   = TRUE;

	Row->MouseDown->Right->Control->bFocusRow			  = TRUE; // 15.0.2
	Row->MouseDown->Right->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->Control->bSelectRow			  = TRUE; // 15.0.2
	Row->MouseDown->Right->Control->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->Control->bMultipleSelection	= FALSE; // 15.0.2
	Row->MouseDown->Right->Control->bKeepFocus			  = FALSE;
	Row->MouseDown->Right->Control->bKeepSelection		  = TRUE;

	Row->MouseDown->Right->Shift->bFocusRow				= TRUE; // 15.0.2
	Row->MouseDown->Right->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->Shift->bSelectRow			= TRUE; // 15.0.2
	Row->MouseDown->Right->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->Shift->bMultipleSelection	= FALSE; // 15.0.2
	Row->MouseDown->Right->Shift->bKeepFocus			= FALSE;
	Row->MouseDown->Right->Shift->bKeepSelection		= TRUE;

	Row->MouseDown->Right->bFocusColumn		  = FALSE;
	Row->MouseDown->Right->bEnsureVisible	 = TRUE; // 15.0.2
	Row->MouseDown->Right->bClick			  = FALSE;
	Row->MouseDown->Right->bContextMenu		  = FALSE;
	Row->MouseDown->Right->bCheckSelectedRows = FALSE;
	Row->MouseDown->Right->bDragBegin		  = FALSE;
	Row->MouseDown->Right->bDragEnd			  = FALSE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button up
	///////////////////////////////////////////////////////////////////////

	Row->MouseUp->Right->None->bFocusRow			 = TRUE; // 15.0.2 bug
	Row->MouseUp->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->None->bSelectRow			 = TRUE; // 15.0.2 bug
	Row->MouseUp->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->None->bMultipleSelection	= FALSE;
	Row->MouseUp->Right->None->bKeepFocus			 = FALSE;
	Row->MouseUp->Right->None->bKeepSelection		 = TRUE;

	Row->MouseUp->Right->Control->bFocusRow				= TRUE; // 15.0.2 bug
	Row->MouseUp->Right->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->Control->bSelectRow			= TRUE; // 15.0.2 bug
	Row->MouseUp->Right->Control->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->Control->bMultipleSelection	= FALSE;
	Row->MouseUp->Right->Control->bKeepFocus			= FALSE;
	Row->MouseUp->Right->Control->bKeepSelection		= TRUE;

	Row->MouseUp->Right->Shift->bFocusRow			  = TRUE; // 15.0.2 bug
	Row->MouseUp->Right->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->Shift->bSelectRow			  = TRUE; // 15.0.2 bug
	Row->MouseUp->Right->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->Shift->bMultipleSelection	= FALSE;
	Row->MouseUp->Right->Shift->bKeepFocus			  = FALSE;
	Row->MouseUp->Right->Shift->bKeepSelection		  = TRUE;

	Row->MouseUp->Right->bFocusColumn		= FALSE;
	Row->MouseUp->Right->bEnsureVisible		= TRUE; // 15.0.2 bug (block scrolling only)
	Row->MouseUp->Right->bClick				= FALSE;
	Row->MouseUp->Right->bContextMenu		= TRUE;
	Row->MouseUp->Right->bCheckSelectedRows = FALSE;
	Row->MouseUp->Right->bDragBegin			= FALSE;
	Row->MouseUp->Right->bDragEnd			= FALSE;
}

void CXTPGridBehavior::SetSchemeCodejockDefault()
{
	Notifications->Populate->bFocusChanging		= TRUE;
	Notifications->Populate->bSelectionChanging = TRUE;
	Notifications->Populate->bSelectionChanged  = TRUE;

	//////////////////////////////////////////////////////////////////////////
	// Left mouse button down
	//////////////////////////////////////////////////////////////////////////

	Row->MouseDown->Left->None->bFocusRow			  = TRUE;
	Row->MouseDown->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->None->bSelectRow			  = TRUE;
	Row->MouseDown->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->None->bMultipleSelection	= FALSE;
	Row->MouseDown->Left->None->bKeepFocus			  = TRUE; // Drag multi selection
	Row->MouseDown->Left->None->bKeepSelection		  = TRUE; // Drag multi selection

	Row->MouseDown->Left->Control->bFocusRow			 = TRUE;
	Row->MouseDown->Left->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->Control->bSelectRow			 = TRUE;
	Row->MouseDown->Left->Control->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->Control->bMultipleSelection	= TRUE;
	Row->MouseDown->Left->Control->bKeepFocus			 = TRUE; // Copy multi selection
	Row->MouseDown->Left->Control->bKeepSelection		 = TRUE; // Copy multi selection

	Row->MouseDown->Left->Shift->bFocusRow			   = TRUE;
	Row->MouseDown->Left->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Left->Shift->bSelectRow			   = TRUE;
	Row->MouseDown->Left->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Left->Shift->bMultipleSelection	= TRUE;
	Row->MouseDown->Left->Shift->bKeepFocus			   = FALSE; // Block selection
	Row->MouseDown->Left->Shift->bKeepSelection		   = FALSE; // Block selection

	Row->MouseDown->Left->bFocusColumn		 = TRUE;
	Row->MouseDown->Left->bEnsureVisible	 = FALSE;
	Row->MouseDown->Left->bClick			 = FALSE;
	Row->MouseDown->Left->bContextMenu		 = FALSE;
	Row->MouseDown->Left->bCheckSelectedRows = FALSE;
	Row->MouseDown->Left->bDragBegin		 = TRUE;
	Row->MouseDown->Left->bDragEnd			 = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Left mouse button up
	//////////////////////////////////////////////////////////////////////////

	Row->MouseUp->Left->None->bFocusRow				= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->None->bSelectRow			= TRUE; // Drag multi selection
	Row->MouseUp->Left->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->None->bMultipleSelection	= FALSE;
	Row->MouseUp->Left->None->bKeepFocus			= FALSE;
	Row->MouseUp->Left->None->bKeepSelection		= FALSE;

	Row->MouseUp->Left->Shift->bFocusRow			 = FALSE;
	Row->MouseUp->Left->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->Shift->bSelectRow			 = FALSE;
	Row->MouseUp->Left->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->Shift->bMultipleSelection	= TRUE;
	Row->MouseUp->Left->Shift->bKeepFocus			 = FALSE;
	Row->MouseUp->Left->Shift->bKeepSelection		 = FALSE;

	Row->MouseUp->Left->Control->bFocusRow			   = FALSE;
	Row->MouseUp->Left->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Left->Control->bSelectRow			   = FALSE;
	Row->MouseUp->Left->Control->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Left->Control->bMultipleSelection	= TRUE;
	Row->MouseUp->Left->Control->bKeepFocus			   = TRUE; // Ctrl/Shift
	Row->MouseUp->Left->Control->bKeepSelection		   = TRUE; // Ctrl/Shift

	Row->MouseUp->Left->bFocusColumn	   = FALSE; // On mouse down
	Row->MouseUp->Left->bEnsureVisible	 = TRUE;  // Ensure visible on mouse up
	Row->MouseUp->Left->bClick			   = TRUE;
	Row->MouseUp->Left->bContextMenu	   = FALSE;
	Row->MouseUp->Left->bCheckSelectedRows = FALSE;
	Row->MouseUp->Left->bDragBegin		   = FALSE;
	Row->MouseUp->Left->bDragEnd		   = TRUE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button down
	///////////////////////////////////////////////////////////////////////

	Row->MouseDown->Right->None->bFocusRow			   = TRUE;
	Row->MouseDown->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->None->bSelectRow			   = TRUE;
	Row->MouseDown->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->None->bMultipleSelection	= FALSE;
	Row->MouseDown->Right->None->bKeepFocus			   = TRUE;
	Row->MouseDown->Right->None->bKeepSelection		   = TRUE;

	Row->MouseDown->Right->Control->bFocusRow			  = TRUE;
	Row->MouseDown->Right->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->Control->bSelectRow			  = TRUE;
	Row->MouseDown->Right->Control->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->Control->bMultipleSelection	= TRUE;
	Row->MouseDown->Right->Control->bKeepFocus			  = TRUE;
	Row->MouseDown->Right->Control->bKeepSelection		  = TRUE;

	Row->MouseDown->Right->Shift->bFocusRow				= TRUE;
	Row->MouseDown->Right->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->Shift->bSelectRow			= TRUE;
	Row->MouseDown->Right->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->Shift->bMultipleSelection	= FALSE;
	Row->MouseDown->Right->Shift->bKeepFocus			= TRUE;
	Row->MouseDown->Right->Shift->bKeepSelection		= TRUE;

	Row->MouseDown->Right->bFocusColumn		  = TRUE;
	Row->MouseDown->Right->bEnsureVisible	 = TRUE;
	Row->MouseDown->Right->bClick			  = FALSE;
	Row->MouseDown->Right->bContextMenu		  = FALSE;
	Row->MouseDown->Right->bCheckSelectedRows = FALSE;
	Row->MouseDown->Right->bDragBegin		  = FALSE;
	Row->MouseDown->Right->bDragEnd			  = FALSE;

	///////////////////////////////////////////////////////////////////////
	// Right mouse button up
	///////////////////////////////////////////////////////////////////////

	Row->MouseUp->Right->None->bFocusRow			 = FALSE;
	Row->MouseUp->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->None->bSelectRow			 = FALSE;
	Row->MouseUp->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->None->bMultipleSelection	= FALSE;
	Row->MouseUp->Right->None->bKeepSelection		 = FALSE;
	Row->MouseUp->Right->None->bKeepFocus			 = FALSE;

	Row->MouseUp->Right->Control->bFocusRow				= FALSE;
	Row->MouseUp->Right->Control->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->Control->bSelectRow			= FALSE;
	Row->MouseUp->Right->Control->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->Control->bMultipleSelection	= FALSE;
	Row->MouseUp->Right->Control->bKeepSelection		= FALSE;
	Row->MouseUp->Right->Control->bKeepFocus			= FALSE;

	Row->MouseUp->Right->Shift->bFocusRow			  = FALSE;
	Row->MouseUp->Right->Shift->bFocusRowTemporarily  = FALSE;
	Row->MouseUp->Right->Shift->bSelectRow			  = FALSE;
	Row->MouseUp->Right->Shift->bSelectRowTemporarily = FALSE;
	Row->MouseUp->Right->Shift->bMultipleSelection	= FALSE;
	Row->MouseUp->Right->Shift->bKeepSelection		  = FALSE;
	Row->MouseUp->Right->Shift->bKeepFocus			  = FALSE;

	Row->MouseUp->Right->bFocusColumn		= FALSE;
	Row->MouseUp->Right->bEnsureVisible		= FALSE;
	Row->MouseUp->Right->bClick				= FALSE;
	Row->MouseUp->Right->bContextMenu		= TRUE;
	Row->MouseUp->Right->bCheckSelectedRows = FALSE;
	Row->MouseUp->Right->bDragBegin			= FALSE;
	Row->MouseUp->Right->bDragEnd			= FALSE;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridBehavior, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehavior, Scheme, 100, VT_I4)

	DISP_FUNCTION_ID(CXTPGridBehavior, "Row", 1, OleGetRow, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridBehavior, "GroupRow", 2, OleGetGroupRow, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridNotifications, "Notifications", 3, OleGetNotifications, VT_DISPATCH,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridBehavior, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridBehavior, XTPDIID_IGridBehavior, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridBehavior, XTPDIID_IGridBehavior)

XTPGridBehavior CXTPGridBehavior::OleGetScheme()
{
	return m_scheme;
}

void CXTPGridBehavior::OleSetScheme(XTPGridBehavior scheme)
{
	SetScheme(scheme);
}

LPDISPATCH CXTPGridBehavior::OleGetRow()
{
	return XTPGetDispatch(Row);
}

LPDISPATCH CXTPGridBehavior::OleGetGroupRow()
{
	return XTPGetDispatch(GroupRow);
}

LPDISPATCH CXTPGridBehavior::OleGetNotifications()
{
	return XTPGetDispatch(Notifications);
}

#endif // _XTP_ACTIVEX
