// XTPGridHitTestInfo.cpp : implementation of the CXTPGridHitTestInfo class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridHitTestInfo.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridColumn.h"
#include "TrackControl/XTPTrackBlock.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CXTPGridHitTestInfo, CXTPCmdTarget)

CXTPGridHitTestInfo::CXTPGridHitTestInfo()
	: m_htCode(xtpGridHitTestUnknown)
	, m_pColumn(NULL)
	, m_pRow(NULL)
	, m_pItem(NULL)
	, m_pBlock(NULL)
	, m_iTrackPosition(0)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridHitTestInfo::~CXTPGridHitTestInfo()
{
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridHitTestInfo, CXTPCmdTarget)
	//{{AFX_DISPATCH_MAP(CGridHitTestInfo)
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX_ID(CXTPGridHitTestInfo, "Column", 1, OleGetColumn, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPGridHitTestInfo, "Row", 2, OleGetRow, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPGridHitTestInfo, "Item", 3, OleGetItem, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPGridHitTestInfo, "ht", 4, OleGetHt, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridHitTestInfo, "Block", 5, OleGetBlock, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPGridHitTestInfo, "TrackPosition", 6, OleGetTrackPosition,
						SetNotSupported, VT_I4)
END_DISPATCH_MAP()

// Note: we add support for DIID_IGridHitTestInfo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

BEGIN_INTERFACE_MAP(CXTPGridHitTestInfo, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridHitTestInfo, XTPDIID_IGridHitTestInfo, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridHitTestInfo, XTPDIID_IGridHitTestInfo)
/////////////////////////////////////////////////////////////////////////////
// CGridHitTestInfo message handlers

LPDISPATCH CXTPGridHitTestInfo::OleGetColumn()
{
	return m_pColumn ? m_pColumn->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPGridHitTestInfo::OleGetRow()
{
	return m_pRow ? m_pRow->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPGridHitTestInfo::OleGetItem()
{
	return m_pItem ? m_pItem->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPGridHitTestInfo::OleGetBlock()
{
	return m_pBlock ? m_pBlock->GetIDispatch(TRUE) : NULL;
}

long CXTPGridHitTestInfo::OleGetTrackPosition()
{
	return m_iTrackPosition;
}

long CXTPGridHitTestInfo::OleGetHt()
{
	return m_htCode;
}

#endif
