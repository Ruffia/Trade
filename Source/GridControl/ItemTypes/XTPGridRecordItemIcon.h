// XTPGridRecordItemIcon.h: interface for the CXTPGridRecordItemIcon class.
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
#if !defined(__XTPGRIDRECORDITEMICON_H__)
#	define __XTPGRIDRECORDITEMICON_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPGridRecordItemIcon : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPGridRecordItemIcon)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecordItemIcon object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	// Returns:
	//     The item text width.
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     pMetrics  - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                 structure containing item metrics.
	//-----------------------------------------------------------------------
	virtual void OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
							   XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption text bounding rectangle.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcItem    - Reference to a CRect object that receives
	//                 the caption text bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_GRIDRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the in-place edit control is changed.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	void OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDRECORDITEMICON_H__)
