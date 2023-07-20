// XTPGridRecordItemPreview.h: interface for the CXTPGridRecordItemPreview class.
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
#if !defined(__XTPGRIDRECORDITEMPREVIEW_H__)
#	define __XTPGRIDRECORDITEMPREVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     This represents a preview cell and inherits basic functionality
//     from the CXTPGridRecordItem class.
// Remarks:
//     You create a CXTPGridRecordItemPreview record item simply by
//     calling its constructor with one parameter - its preview text.
// See Also:
//     CXTPGridRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemPreview : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPGridRecordItemPreview)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a new preview record item.
	// Parameters:
	//     szPreviewText - Pointer to the preview text to set.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemPreview(LPCTSTR szPreviewText = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Set the preview text for the item.
	// Parameters:
	//     strPreviewText - New preview text to be set.
	//-----------------------------------------------------------------------
	void SetPreviewText(LPCTSTR strPreviewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the preview text of the item.
	// Returns:
	//     The preview text of the item.
	//-----------------------------------------------------------------------
	virtual CString GetPreviewText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the height of the preview area.
	// Parameters:
	//     pDC    - Pointer to the control drawing context.
	//     pRow   - Pointer to the corresponding row.
	//     nWidth - Width of the row.
	// Returns:
	//     The height of the preview area.
	//-----------------------------------------------------------------------
	virtual int GetPreviewHeight(CDC* pDC, CXTPGridRow* pRow, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the item.
	// Returns:
	//     The caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption(CXTPGridColumn* pColumn);

	using CXTPGridRecordItem::GetCaption;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the item.
	// Parameters:
	//     strCaption - New caption to be set.
	//-----------------------------------------------------------------------
	virtual void SetCaption(LPCTSTR strCaption);

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
	//     Fills an XTP_GRIDRECORDITEM_METRICS structure.
	// Parameters:
	//     pDrawArgs    - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                    structure containing drawing arguments used
	//                    for calculating item metrics.
	//     pItemMetrics - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                    structure to fill with item metrics.
	// Remarks:
	//     Calculates preview item metrics based on provided draw arguments.
	//     Overwrites parents' member function
	// See Also:
	//     XTP_GRIDRECORDITEM_DRAWARGS, XTP_GRIDRECORDITEM_METRICS
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								XTP_GRIDRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is a preview item.
	// Returns:
	//     TRUE if the item is a preview item, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPreviewItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the in-place edit control is changed.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption text of the bounding rectangle.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcItem    - Reference to a CRect object that receives
	//                 the caption text bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_GRIDRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	virtual void OnBeginEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

protected:
	CString m_strPreviewText; // Preview content.
};

AFX_INLINE CString CXTPGridRecordItemPreview::GetPreviewText()
{
	return m_strPreviewText;
}

AFX_INLINE BOOL CXTPGridRecordItemPreview::IsPreviewItem() const
{
	return TRUE;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDRECORDITEMPREVIEW_H__)
