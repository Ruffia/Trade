// XTPGridHitTestInfo.h: interface for the CXTPGridHitTestInfo class.
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
#if !defined(__XTPGRIDHITTESTINFO_H__)
#	define __XTPGRIDHITTESTINFO_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// Forward declarations
class CXTPGridColumn;
class CXTPGridRow;
class CXTPGridRecordItem;
class CXTPTrackBlock;

enum XTPGridHitTestInfoCode
{
	xtpGridHitTestUnknown	= 0,
	xtpGridHitTestHeader	 = 1,
	xtpGridHitTestGroupBox   = 2,
	xtpGridHitTestBodyRows   = 3,
	xtpGridHitTestBlock		 = 4,
	xtpGridHitTestHeaderRows = 5,
	xtpGridHitTestFooterRows = 6
};

class _XTP_EXT_CLASS CXTPGridHitTestInfo : public CXTPCmdTarget
{
	DECLARE_DYNCREATE(CXTPGridHitTestInfo)

public:
	CXTPGridHitTestInfo();
	virtual ~CXTPGridHitTestInfo();

	// Attributes
public:
	XTPGridHitTestInfoCode m_htCode;
	CXTPGridColumn* m_pColumn;
	CXTPGridRow* m_pRow;
	CXTPGridRecordItem* m_pItem;
	CXTPTrackBlock* m_pBlock;
	int m_iTrackPosition;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
public:
	LPDISPATCH OleGetColumn();
	LPDISPATCH OleGetRow();
	LPDISPATCH OleGetItem();
	LPDISPATCH OleGetBlock();
	long OleGetTrackPosition();
	long OleGetHt();

protected:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridHitTestInfo);
	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDHITTESTINFO_H__)
