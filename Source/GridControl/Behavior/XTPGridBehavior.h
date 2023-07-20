// XTPGridBehavior.h: Declaration of the CXTPGridBehavior class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPGRIDBEHAVIOR_H__)
#	define __XTPGRIDBEHAVIOR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

enum XTPGridMouseButton
{
	xtpGridMouseButtonNone,
	xtpGridMouseButtonLeft,
	xtpGridMouseButtonMiddle,
	xtpGridMouseButtonRight
};

enum XTPGridMouseEvent
{
	xtpGridMouseEventButtonDown,
	xtpGridMouseEventButtonUp
};

enum XTPGridBehavior
{
	xtpGridBehaviorOutlook2003 = 0x00001100, // 11.0
	xtpGridBehaviorOutlook2007 = 0x00001200, // 12.0
	xtpGridBehaviorOutlook2010 = 0x00001400, // 14.0

	xtpGridBehaviorExcel2003 = 0x10001100, // 11.0
	xtpGridBehaviorExcel2007 = 0x10001200, // 12.0
	xtpGridBehaviorExcel2010 = 0x10001400, // 14.0

	// xtpGridBehaviorCodejock1300    = 0x20001300, // 13.0.0
	// xtpGridBehaviorCodejock1321    = 0x20001321, // 13.2.1
	// xtpGridBehaviorCodejock1330    = 0x20001330, // 13.3.0
	// xtpGridBehaviorCodejock1331    = 0x20001331, // 13.3.1
	xtpGridBehaviorCodejock1340	= 0x20001340, // 13.4.0
	xtpGridBehaviorCodejock1341	= 0x20001341, // 13.4.1
	xtpGridBehaviorCodejock1342	= 0x20001342, // 13.4.2
	xtpGridBehaviorCodejock1501	= 0x20001501, // 15.0.1
	xtpGridBehaviorCodejock1502	= 0x20001502, // 15.0.2
	xtpGridBehaviorCodejockDefault = 0x20000000
};

/////////////////////////////////////////////////////////////////////////////
// Key events
/////////////////////////////////////////////////////////////////////////////

class _XTP_EXT_CLASS CXTPGridBehaviorRowKeyDown
{
};

class _XTP_EXT_CLASS CXTPGridBehaviorRowKeyUp
{
};

/////////////////////////////////////////////////////////////////////////////
// Mouse events
/////////////////////////////////////////////////////////////////////////////

class _XTP_EXT_CLASS CXTPGridBehaviorRowModifier : public CXTPCmdTarget
{
public:
	CXTPGridBehaviorRowModifier();
	void Reset();

	BOOL bFocusRow;			   // TRUE if the row gets focus.
	BOOL bFocusRowTemporarily; // TRUE if the row gets focus temporarily.

	BOOL bSelectRow;			// TRUE if the row gets selected.
	BOOL bSelectRowTemporarily; // TRUE if the row gets selected temporarily.
	BOOL bMultipleSelection;	// TRUE to enable multiple selection.

	BOOL bKeepSelection; // TRUE if multiple rows stay selected.
	BOOL bKeepFocus;	 // TRUE if rows remain drawn as focused.

	BOOL bFocusCancelsSelect; // bFocusCancelsSelect.

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_OLETYPELIB_EX(CXTPGridBehaviorRowModifier);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	XTP_DECLARE_PROPERTY(FocusRow, BOOL);
	XTP_DECLARE_PROPERTY(FocusRowTemporarily, BOOL);
	XTP_DECLARE_PROPERTY(SelectRow, BOOL);
	XTP_DECLARE_PROPERTY(SelectRowTemporarily, BOOL);
	XTP_DECLARE_PROPERTY(MultipleSelection, BOOL);
	XTP_DECLARE_PROPERTY(KeepSelection, BOOL);
	XTP_DECLARE_PROPERTY(KeepFocus, BOOL);
//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridBehaviorRowMouseButton : public CXTPCmdTarget
{
public:
	CXTPGridBehaviorRowMouseButton(XTPGridMouseButton button, XTPGridMouseEvent event);
	virtual ~CXTPGridBehaviorRowMouseButton();

	void Reset();

public:
	CXTPGridBehaviorRowModifier* None;
	CXTPGridBehaviorRowModifier* Control;
	CXTPGridBehaviorRowModifier* Shift;

	BOOL bFocusColumn;   // TRUE if the column gets the focus.
	BOOL bEnsureVisible; // TRUE if the row gets visible.

	BOOL bClick;			 // TRUE if a click event is to be triggered for the mouse button.
	BOOL bContextMenu;		 // TRUE if OnContextMenu/NM_RCLICK gets called.
	BOOL bCheckSelectedRows; // TRUE to check selected rows by using the mouse button.

	BOOL bDragBegin;	   // TRUE to begin drag.
	BOOL bDragEnd;		   // TRUE to end drag.
	BOOL bDragSelectGroup; // TRUE to select the entire group on drag.

public:
	const XTPGridMouseButton m_button;
	const XTPGridMouseEvent m_event;

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridBehaviorRowMouseButton);

	afx_msg LPDISPATCH OleGetNone();
	afx_msg LPDISPATCH OleGetControl();
	afx_msg LPDISPATCH OleGetShift();

	XTP_DECLARE_PROPERTY(FocusColumn, BOOL);
	XTP_DECLARE_PROPERTY(EnsureVisible, BOOL);

	XTP_DECLARE_PROPERTY(Click, BOOL);
	XTP_DECLARE_PROPERTY(ContextMenu, BOOL);
	XTP_DECLARE_PROPERTY(CheckSelectedRows, BOOL);

	XTP_DECLARE_PROPERTY(DragBegin, BOOL);
	XTP_DECLARE_PROPERTY(DragEnd, BOOL);
	XTP_DECLARE_PROPERTY(DragSelectGroup, BOOL);
//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridBehaviorRowMouse : public CXTPCmdTarget
{
public:
	CXTPGridBehaviorRowMouse(XTPGridMouseEvent event);
	virtual ~CXTPGridBehaviorRowMouse();

	CXTPGridBehaviorRowMouseButton* Left;
	CXTPGridBehaviorRowMouseButton* Right;

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridBehaviorRowMouse);

	afx_msg LPDISPATCH OleGetLeft();
	afx_msg LPDISPATCH OleGetRight();
//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridBehaviorRow : public CXTPCmdTarget
{
public:
	CXTPGridBehaviorRow();
	virtual ~CXTPGridBehaviorRow();

public:
	CXTPGridBehaviorRowMouse* MouseDown;
	CXTPGridBehaviorRowMouse* MouseUp;

#	ifdef _XTP_ACTIVEX
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridBehaviorRow);

	afx_msg LPDISPATCH OleGetMouseDown();
	afx_msg LPDISPATCH OleGetMouseUp();
//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

class _XTP_EXT_CLASS CXTPGridNotificationsPopulate : public CXTPCmdTarget
{
public:
	CXTPGridNotificationsPopulate();
	virtual ~CXTPGridNotificationsPopulate();

public:
	BOOL bFocusChanging;
	BOOL bSelectionChanging;
	BOOL bSelectionChanged;

#	ifdef _XTP_ACTIVEX
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridNotificationsPopulate);

	XTP_DECLARE_PROPERTY(FocusChangingNotification, BOOL);
	XTP_DECLARE_PROPERTY(SelectionChangingNotification, BOOL);
	XTP_DECLARE_PROPERTY(SelectionChangedNotification, BOOL);
	//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridNotificationsSelectedRows : public CXTPCmdTarget
{
public:
	CXTPGridNotificationsSelectedRows();
	virtual ~CXTPGridNotificationsSelectedRows();

public:
	BOOL bClear;
	BOOL bAdd;
	BOOL bRemove;

#	ifdef _XTP_ACTIVEX
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridNotificationsSelectedRows);

	XTP_DECLARE_PROPERTY(ClearNotification, BOOL);
	XTP_DECLARE_PROPERTY(AddNotification, BOOL);
	XTP_DECLARE_PROPERTY(RemoveNotification, BOOL);
	//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridNotificationsRow : public CXTPCmdTarget
{
public:
	CXTPGridNotificationsRow();
	virtual ~CXTPGridNotificationsRow();

public:
	BOOL bRowExpanded;

#	ifdef _XTP_ACTIVEX
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridNotificationsRow);

	XTP_DECLARE_PROPERTY(RowExpandedNotification, BOOL);
	//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridNotifications : public CXTPCmdTarget
{
public:
	CXTPGridNotifications();
	virtual ~CXTPGridNotifications();

public:
	CXTPGridNotificationsPopulate* Populate;
	CXTPGridNotificationsSelectedRows* SelectedRows;
	CXTPGridNotificationsRow* Row;

#	ifdef _XTP_ACTIVEX
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridNotifications);

	afx_msg LPDISPATCH OleGetPopulate();
	afx_msg LPDISPATCH OleGetSelectedRows();
	afx_msg LPDISPATCH OleGetRow();
	//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPGridBehavior : public CXTPCmdTarget
{
public:
	CXTPGridBehavior(XTPGridBehavior behavior);
	virtual ~CXTPGridBehavior();

	void SetScheme(XTPGridBehavior behavior);

public:
	CXTPGridBehaviorRow* Row;
	CXTPGridBehaviorRow* GroupRow;

	CXTPGridNotifications* Notifications;

private:
	void SetSchemeOutlook();
	void SetSchemeExcel();
	void SetSchemeCodejock134x();
	void SetSchemeCodejock1501();
	void SetSchemeCodejock1502();
	void SetSchemeCodejockDefault();

	XTPGridBehavior m_scheme; // The last set scheme.

#	ifdef _XTP_ACTIVEX
protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridBehavior);

	XTP_DECLARE_PROPERTY(Scheme, XTPGridBehavior);

	afx_msg LPDISPATCH OleGetRow();
	afx_msg LPDISPATCH OleGetGroupRow();
	afx_msg LPDISPATCH OleGetNotifications();
//}}AFX_CODEJOCK_PRIVATE
#	endif // _XTP_ACTIVEX
};

//{{AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDBEHAVIOR_H__)
//}}AFX_CODEJOCK_PRIVATE
