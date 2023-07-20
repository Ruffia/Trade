// XTPSyntaxEditDrawTextProcessor.h: interface for the CXTPSyntaxEditDrawTextProcessor class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITDRAWTEXTPROCESSOR_H__)
#	define __XTPSYNTAXEDITDRAWTEXTPROCESSOR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE
template<class TYPE, class ARG_TYPE>
class CXTPReserveArray : protected CArray<TYPE, ARG_TYPE>
{
	typedef CArray<TYPE, ARG_TYPE> Parent;

protected:
	int m_nDataSize;

public:
	CXTPReserveArray()
	{
		m_nDataSize = 0;
	}

	int GetDataSize() const
	{
		return m_nDataSize;
	}

	void SetDataSize(int nDataSize, int nReservedSize = -1, int nGrowBy = -1)
	{
		ASSERT(nReservedSize == -1 || nReservedSize >= nDataSize);
		nReservedSize = max(nReservedSize, nDataSize);

		m_nDataSize = nDataSize;
		CArray<TYPE, ARG_TYPE>::SetSize(nReservedSize, nGrowBy);
	}

	void AddData(ARG_TYPE newElement)
	{
		CArray<TYPE, ARG_TYPE>::SetAtGrow(m_nDataSize, newElement);
		m_nDataSize++;
	}

	void RemoveAll()
	{
		m_nDataSize = 0;
	}

	TYPE operator[](int nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < GetDataSize());
		return CArray<TYPE, ARG_TYPE>::GetAt(nIndex);
	}
	TYPE& operator[](int nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < GetDataSize());
		return CArray<TYPE, ARG_TYPE>::ElementAt(nIndex);
	}

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const
	{
		return CArray<TYPE, ARG_TYPE>::GetData();
	}

	TYPE* GetData()
	{
		return CArray<TYPE, ARG_TYPE>::GetData();
	}
};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//      This helper class is used by CXTPSyntaxEditCtrl as the Draw Text Processor.
//      It is responsible for both drawing chars and remembering each char position
//      and other text properties.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditDrawTextProcessor
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDrawTextProcessor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text rectangle.
	// Returns:
	//     The text rectangle.
	// See Also:
	//     SetTextRect
	//-----------------------------------------------------------------------
	CRect GetTextRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text rectangle.
	// Parameters:
	//     rcRect - [in] Rectangle to be set.
	// See Also:
	//     GetTextRect
	//-----------------------------------------------------------------------
	void SetTextRect(const CRect& rcRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of a single row.
	// Returns:
	//     The height of a single row.
	// See Also:
	//     RecalcRowHeight
	//-----------------------------------------------------------------------
	int GetRowHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the height of a single row.
	// Parameters:
	//     pDC   - [in] Pointer to the device context.
	//     pFont - [in] Pointer to the font to calculate.
	// Returns:
	//     The height of a single row.
	// See Also:
	//     GetRowHeight
	//-----------------------------------------------------------------------
	int RecalcRowHeight(CDC* pDC, CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of visible rows for the text rectangle.
	// Parameters:
	//     bWithPartlyVisible - TRUE to count both partly and fully visible rows,
	//                          FALSE to only count fully visible rows.
	// Returns:
	//     The number of visible rows for the text rectangle.
	// See Also:
	//     SetTextRect
	//-----------------------------------------------------------------------
	int GetRowsCount(BOOL bWithPartlyVisible) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab size.
	// Returns:
	//     The tab size.
	// See Also:
	//     SetTabSize
	//-----------------------------------------------------------------------
	int GetTabSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab size.
	// Parameters:
	//     nTabSize - [in] Tab size to be set.
	// Remarks:
	//     Size is measured in space characters and should be between 2 to 10.
	// See also:
	//     GetTabSize
	//-----------------------------------------------------------------------
	void SetTabSize(int nTabSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scroll offset for the x- coordinate.
	// Returns:
	//     The scroll offset for the x- coordinate.
	// See Also:
	//     SetScrollXOffset
	//-----------------------------------------------------------------------
	int GetScrollXOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the scroll offset for the x- coordinate.
	// Parameters:
	//     nOffsetX - [in] Scroll offset to be set.
	// See Also:
	//     GetScrollXOffset
	//-----------------------------------------------------------------------
	void SetScrollXOffset(int nOffsetX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current text metrics.
	// Returns:
	//     A reference to the current text metrics.
	// See Also:
	//     RecalcRowHeight
	//-----------------------------------------------------------------------
	const TEXTMETRIC& GetTextMetrics() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the space character width, in pixels.
	// Returns:
	//     The space character width, in pixels.
	// See Also:
	//     RecalcRowHeight
	//-----------------------------------------------------------------------
	int GetSpaceWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Aligns a specified column index to tab borders.
	// Parameters:
	//     nRow          - Zero-based row index.
	//     nCol          - Zero-based column index.
	//     bVirtualSpace - TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     The aligned column index.
	// See Also:
	//     SetTabSize, GetTabSize
	//-----------------------------------------------------------------------
	int AlignColIdxToTabs(int nRow, int nCol, BOOL bVirtualSpace = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the starting x- position of a specified column.
	// Parameters:
	//     nRow          - [in] Zero-based row index.
	//     nCol          - [in] Zero-based column index.
	//     pnCharWidth   - [out] Pointer to an integer to receive the
	//                           character width of the column (in pixels).
	//                           The value of this parameter may be NULL.
	//     bVirtualSpace - [in] TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     The starting x-position (in pixels) of the specified column.
	// See Also:
	//     ColFromXPos
	//-----------------------------------------------------------------------
	int GetColPosX(int nRow, int nCol, int* pnCharWidth = NULL, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the column at a specified x- position.
	// Parameters:
	//     nRow          - [in] Zero-based row index.
	//     nX            - [in] x- position (in pixels).
	//     rnCol         - [out] Reference to an integer to receive the
	//                           index of the column at the specified x- position.
	//     bVirtualSpace - [in] TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetColPosX, AlignColIdxToTabs
	//-----------------------------------------------------------------------
	BOOL ColFromXPos(int nRow, int nX, int& rnCol, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the width (in pixels) of a specified row.
	// Parameters:
	//     nRow - Zero-based index of the row with the width to retrieve.
	// Returns:
	//     The width (in pixels) of the specified row.
	// See Also:
	//     GetRowsMaxWidth
	//-----------------------------------------------------------------------
	int GetRowWidth(int nRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum width (in pixels) of all visible rows.
	// Returns:
	//     The maximum width (in pixels) of all visible rows.
	// See Also:
	//     GetRowsWidth
	//-----------------------------------------------------------------------
	int GetRowsMaxWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row and column, if any, at a specified point.
	// Parameters:
	//     pt            - [in] Point to be tested.
	//     rnRow         - [out] Reference to an integer to receive
	//                           the index of the row at the specified point.
	//     rnCol         - [out] Reference to an integer to receive
	//                           the index of the column at the specified point.
	//     bVirtualSpace - [in] TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetColPosX, HitTestRow.
	//-----------------------------------------------------------------------
	BOOL HitTest(const CPoint& pt, int& rnRow, int& rnCol, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row, if any, at a specified y- position.
	// Parameters:
	//     nY    - [in] y- position (in pixels).
	//     rnRow - [out] Reference to an integer to receive
	//                   the index of the row at the specified y- position.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetColPosX, HitTest.
	//-----------------------------------------------------------------------
	BOOL HitTestRow(int nY, int& rnRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears the cached row information of a specified row.
	// Parameters:
	//     nRow - Zero-based row index.
	// See Also:
	//     DrawRowPart, PrintRowPart.
	//-----------------------------------------------------------------------
	void ResetRowInfo(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the row part and stores character positions.
	// Parameters:
	//     pDC      - [in] Pointer to a valid device context.
	//     nRow     - [in] Zero-based row index (or a value of -1 to
	//                     switch to finishing the current row drawing).
	//     pcszText - [in] String to draw.
	//     nchCount - [in] Character count to draw.
	// Returns:
	//      The x- offset of the next row part.
	// See Also:
	//      PrintRowPart
	//-----------------------------------------------------------------------
	int DrawRowPart(CDC* pDC, int nRow, LPCTSTR pcszText, int nchCount = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to draw row part and store characters positions.
	// Parameters:
	//     pDC              - [in] Pointer to a valid device context.
	//     nRow             - [in] Zero-based row index (or a value of -1 to
	//                             switch to finishing the current row drawing).
	//     nPosY            - [in] y- offset to print the specified row.
	//     nFlags           - [in] Additional printing options.
	//                             The following values are supported:
	//                             DT_CALCRECT, DT_SINGLELINE, DT_WORDBREAK
	//     pcszText         - [in] String to draw.
	//     nchCount         - [in] Character count to draw.
	//     pnPrintedTextLen - [out] Pointer to an integer to receive
	//                              the printed text length.
	// Returns:
	//     The printed row height.
	// See Also:
	//     DrawRowPart
	//-----------------------------------------------------------------------
	int PrintRowPart(CDC* pDC, int nRow, int nPosY, UINT nFlags, LPCTSTR pcszText,
					 int nchCount = -1, int* pnPrintedTextLen = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands the character set by putting spaces in the position of tabs.
	// Parameters:
	//     pszChars          - [in] Text to be processed.
	//     strBuffer         - [out] Buffer for text to be created after expansion.
	//     nDispPos          - [in] Start display position.
	//     bEnableWhiteSpace - [in] TRUE to show white space, FALSE otherwise.
	// Returns:
	//     The end display position after expansion.
	//-----------------------------------------------------------------------
	int ExpandChars(LPCTSTR pszChars, CString& strBuffer, int nDispPos = 0,
					BOOL bEnableWhiteSpace = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab positions for a specified row.
	// Parameters:
	//     nRow            - [in] Zero-based row index.
	//     pcszOrigRowText - [in] Row text (not expanded).
	//-----------------------------------------------------------------------
	void SetRowTabPositions(int nRow, LPCTSTR pcszOrigRowText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts display position to string position.
	// Parameters:
	//     nRow          - Zero-based row index.
	//     nDispPos      - Zero-based display column index.
	//     bVirtualSpace - TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     The converted string position.
	//-----------------------------------------------------------------------
	int DispPosToStrPos(int nRow, int nDispPos, BOOL bVirtualSpace) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts string position to display position.
	// Parameters:
	//     nRow          - Zero-based row index.
	//     nStrPos       - Zero-based string position.
	//     bVirtualSpace - TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     The converted display position.
	//-----------------------------------------------------------------------
	int StrPosToDispPos(int nRow, int nStrPos, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caret position.
	// Parameters:
	//     pWnd          - [in] Pointer to the caret owner window.
	//     pt            - [in] Reference to the point to set the caret.
	//     szSize        - [in] Reference to the caret size.
	//     nRow          - [in] Zero-based row index.
	//     rnRow         - [in, out] Reference to the zero-based row index.
	//     rnCol         - [in, out] Reference to the zero-based column index.
	//     bHideCaret    - [in] TRUE to hide the caret, FALSE to show the caret.
	//     bVirtualSpace - [in] TRUE if virtual space is enabled, otherwise FALSE.
	// Remarks:
	//     The [in, out] parameters rnCol and rnRow may be adjusted to valid values.
	// Returns:
	//     The point of the caret position.
	//-----------------------------------------------------------------------
	CPoint SetCaretPos(CWnd* pWnd, const CSize& szSize, int nRow, int& rnCol,
					   BOOL bHideCaret = FALSE, BOOL bVirtualSpace = FALSE);
	CPoint SetCaretByPoint(
		CWnd* pWnd, const CPoint& pt, const CSize& szSize, int& rnRow, int& rnCol,
		BOOL bVirtualSpace = FALSE); // <combine
									 // CXTPSyntaxEditDrawTextProcessor::SetCaretPos@CWnd*@const
									 // CSize&@int@int&@BOOL@BOOL>

public:
	//{{AFX_CODEJOCK_PRIVATE
	class _XTP_EXT_CLASS CXTPRowInfo : public CXTPCmdTarget
	{
	public:
		CXTPRowInfo()
		{
			nMaxWidth				 = 0;
			const int cnReservedSize = 4096;
			arCharsEnds.SetDataSize(0, cnReservedSize, cnReservedSize);

			arTabs.SetDataSize(0, cnReservedSize, cnReservedSize);
			arDispCol2StrPos.SetDataSize(0, cnReservedSize, cnReservedSize);
			arStrPos2DispCol.SetDataSize(0, cnReservedSize, cnReservedSize);
		}

		void Reset()
		{
			nMaxWidth = 0;
			arCharsEnds.RemoveAll();
		}

		int nMaxWidth;

		CXTPReserveArray<int, int> arCharsEnds;

		CXTPReserveArray<BYTE, BYTE> arTabs;
		CXTPReserveArray<int, int> arDispCol2StrPos;
		CXTPReserveArray<int, int> arStrPos2DispCol;
	};

	CXTPRowInfo* GetRowInfo(int nRow) const;
	//}}AFX_CODEJOCK_PRIVATE

private:
	void DrawTextOfSize(CDC* pDC, int nX, int nY, const SIZE& textSize, const RECT& rcText,
						LPCTSTR pcszText, int nchCount) const;

protected:
	int m_nTabSize;		 // Tab size.
	TEXTMETRIC m_tmText; // Text metrics.
	int m_nSpaceWidth;   // Space character width.

	CRect m_rcTextRect;   // Text rectangle.
	int m_nRowHeight;	 // Row height.
	int m_nScrollXOffset; // Scroll offset for the x- coordinate.

	int m_nDrawingRow;  // Currently drawing row.
	int m_nNextRowPosX; // x- offset to draw the next row part.

	int m_nPrintingRow;		 // Currently printing row.
	CPoint m_ptNextPrintPos; // x- and y- offset to print the next row part.
	BOOL m_bUseOutputDC;	 // TRUE to use the output DC for calculations.

	//{{AFX_CODEJOCK_PRIVATE
	typedef CXTPInternalCollectionT<CXTPRowInfo> CXTPRowsInfoArray; //
	mutable CXTPRowsInfoArray m_arRows;
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// temporary buffers
	CArray<int, int> m_arBuf_aDx;
	CXTPReserveArray<TCHAR, TCHAR> m_arExpandCharsBuffer;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE const TEXTMETRIC& CXTPSyntaxEditDrawTextProcessor::GetTextMetrics() const
{
	return m_tmText;
}
AFX_INLINE CRect CXTPSyntaxEditDrawTextProcessor::GetTextRect() const
{
	return m_rcTextRect;
}
AFX_INLINE void CXTPSyntaxEditDrawTextProcessor::SetTextRect(const CRect& rcRect)
{
	m_rcTextRect = rcRect;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetRowHeight() const
{
	return m_nRowHeight;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetTabSize() const
{
	return m_nTabSize;
}
AFX_INLINE void CXTPSyntaxEditDrawTextProcessor::SetTabSize(int nTabSize)
{
	m_nTabSize = nTabSize;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetScrollXOffset() const
{
	return m_nScrollXOffset;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetRowsCount(BOOL bWithPartlyVisible) const
{
	int nHeight = m_rcTextRect.Height();
	int nRowH   = max(1, m_nRowHeight);
	int nPartly = bWithPartlyVisible ? 1 : 0;

	return nHeight / nRowH + ((nHeight % nRowH) ? nPartly : 0);
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetSpaceWidth() const
{
	return m_nSpaceWidth;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITDRAWTEXTPROCESSOR_H__)
