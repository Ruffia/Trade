// XTPGridBehaviorRowModifier.cpp : implementation of the CXTPGridBehaviorRowModifier class.
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

CXTPGridBehaviorRowModifier::CXTPGridBehaviorRowModifier()
	: bFocusRow(FALSE)
	, bFocusRowTemporarily(FALSE)
	, bSelectRow(FALSE)
	, bSelectRowTemporarily(FALSE)
	, bMultipleSelection(FALSE)
	, bKeepSelection(FALSE)
	, bKeepFocus(FALSE)
	, bFocusCancelsSelect(FALSE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPGridBehaviorRowModifier::Reset()
{
	bFocusRow			  = FALSE;
	bFocusRowTemporarily  = FALSE;
	bSelectRow			  = FALSE;
	bSelectRowTemporarily = FALSE;
	bMultipleSelection	= FALSE;
	bKeepSelection		  = FALSE;
	bKeepFocus			  = FALSE;
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridBehaviorRowModifier, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridBehaviorRowModifier, XTPDIID_IGridBehaviorRowModifier, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridBehaviorRowModifier, XTPDIID_IGridBehaviorRowModifier)

BEGIN_DISPATCH_MAP(CXTPGridBehaviorRowModifier, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, FocusRow, 1, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, FocusRowTemporarily, 2, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, SelectRow, 3, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, SelectRowTemporarily, 4, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, MultipleSelection, 5, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, KeepSelection, 6, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPGridBehaviorRowModifier, KeepFocus, 7, VT_BOOL)
END_DISPATCH_MAP()

XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, FocusRow, bFocusRow, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, FocusRowTemporarily, bFocusRowTemporarily, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, SelectRow, bSelectRow, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, SelectRowTemporarily, bSelectRowTemporarily,
					   BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, MultipleSelection, bMultipleSelection, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, KeepSelection, bKeepSelection, BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPGridBehaviorRowModifier, KeepFocus, bKeepFocus, BOOL)

#endif
