// XTPSyntaxEditStruct.h
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
#if !defined(__XTPSYNTAXEDITSTRUCT_H__)
#	define __XTPSYNTAXEDITSTRUCT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace XTPSyntaxEditLexAnalyser
{
class CXTPSyntaxEditLexTextBlock;
}

class CXTPSyntaxEditCtrl;

//===========================================================================
// max line: 1 048 575
// max col:  4 095
//===========================================================================
#	define XTP_EDIT_XLC(nL, nC) ((((DWORD)nL) << 12) | (((DWORD)nC) & 0x00000FFF))

//===========================================================================
// Summary:
//     The XTP_EDIT_LINECOL structure represents the coordinates of text items
//     on the displaying context in terms of line/column.
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_LINECOL
{
	int nLine; // Line identifier.
	int nCol;  // Column identifier.

	static const XTP_EDIT_LINECOL MAXPOS; // Maximum position value.
	static const XTP_EDIT_LINECOL MINPOS; // Minimum position value.
	static const XTP_EDIT_LINECOL Pos1;   // {1,0} position value.

	//----------------------------------------------------------------------
	// Summary:
	//     Constructs an XTP_EDIT_LINECOL object from
	//     a pair of coordinates (line and column).
	// Parameters:
	//     nParamLine - [in] Line identifier.
	//     nParamCol  - [in] Column identifier.
	//----------------------------------------------------------------------
	static const XTP_EDIT_LINECOL AFX_CDECL MakeLineCol(int nParamLine, int nParamCol);

	//----------------------------------------------------------------------
	// Summary:
	//     Gets a minimum/maximum of two XTP_EDIT_LINECOL values.
	// Parameters:
	//     pos1 - [in] Reference to the first value.
	//     pos2 - [in] Reference to the second value.
	//----------------------------------------------------------------------
	static const XTP_EDIT_LINECOL AFX_CDECL min2(const XTP_EDIT_LINECOL& pos1,
												 const XTP_EDIT_LINECOL& pos2);
	static const XTP_EDIT_LINECOL AFX_CDECL
		max2(const XTP_EDIT_LINECOL& pos1,
			 const XTP_EDIT_LINECOL& pos2); // <COMBINE XTP_EDIT_LINECOL::min2@const
											// XTP_EDIT_LINECOL&@const XTP_EDIT_LINECOL&>

	//----------------------------------------------------------------------
	// Summary:
	//     Gets the coordinates in packed form.
	// Returns:
	//     The coordinates in packed form.
	//----------------------------------------------------------------------
	DWORD GetXLC() const;

	//----------------------------------------------------------------------
	// Summary:
	//     Determines if the data is valid.
	// Returns:
	//     TRUE if the data is valid, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL IsValidData() const;

	//----------------------------------------------------------------------
	// Summary:
	//     Clears data members.
	// Remarks:
	//     This function sets line/column coordinates to 0.
	//----------------------------------------------------------------------
	void Clear();

	//----------------------------------------------------------------------
	// Summary:
	//     Less than operator.
	//     Compares a specified struct with the current struct.
	// Parameters:
	//     pos2 - [in] Reference to the struct to compare
	//                 with the current struct.
	// Returns:
	//     TRUE if the current struct is less than
	//     the specified struct, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL operator<(const XTP_EDIT_LINECOL& pos2) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Less than or equal operator.
	//     Compares a specified struct with the current struct.
	// Parameters:
	//     pos2 - [in] Reference to the struct to compare
	//                 with the current struct.
	// Returns:
	//     TRUE if the current struct is less than or equal to
	//     the specified struct, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL operator<=(const XTP_EDIT_LINECOL& pos2) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Greater than operator.
	//     Compares a specified struct with the current struct.
	// Parameters:
	//     pos2 - [in] Reference to the struct to compare
	//                 with the current struct.
	// Returns:
	//     TRUE if the current struct is greater than
	//     the specified struct, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL operator>(const XTP_EDIT_LINECOL& pos2) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Greater or equal operator.
	//     Compares a specified struct with the current struct.
	// Parameters:
	//     pos2 - [in] Reference to the struct to compare
	//                 with the current struct.
	// Returns:
	//     TRUE if the current struct is greater than or equal to
	//     the specified struct, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL operator>=(const XTP_EDIT_LINECOL& pos2) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Equal operator.
	//     Compares a specified struct with the current struct.
	// Parameters:
	//     pos2 - [in] Reference to the struct to compare
	//                 with the current struct.
	// Returns:
	//     TRUE if the current struct is equal to
	//     the specified struct, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL operator==(const XTP_EDIT_LINECOL& pos2) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Not Equal operator.
	//     Compares a specified struct with the current struct.
	// Parameters:
	//     pos2 - [in] Reference to the struct to compare
	//                 with the current struct.
	// Returns:
	//     TRUE if the current struct is not equal to
	//     the specified struct, otherwise FALSE.
	//----------------------------------------------------------------------
	BOOL operator!=(const XTP_EDIT_LINECOL& pos2) const
	{
		return !operator==(pos2);
	}
};

//===========================================================================
// Summary:
//     Stores the colors for different phrases.
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_COLORVALUES
{
	CXTPPaintManagerColor crText;				// Text color.
	CXTPPaintManagerColor crBack;				// Back color.
	CXTPPaintManagerColor crReadOnlyBack;		// Back color for read only mode.
	CXTPPaintManagerColor crHiliteText;			// Text color of selection.
	CXTPPaintManagerColor crHiliteBack;			// Text color of selection back.
	CXTPPaintManagerColor crInactiveHiliteText; // Inactive text color of selection.
	CXTPPaintManagerColor crInactiveHiliteBack; // Inactive text color of selection back.
	CXTPPaintManagerColor crLineNumberText;		// Line numbers text and line color.
	CXTPPaintManagerColor crLineNumberBack;		// Line numbers text and line color.
	CXTPPaintManagerColor crBreakpointText;		// Breakpoint Text color.
	CXTPPaintManagerColor crBreakpointBack;		// Breakpoint Back color.

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the corresponding background color for either
	//     the edit or read-only mode of a specified CXTPSyntaxEditCtrl object.
	// Parameters:
	//     pEditCtrl - Pointer to a CXTPSyntaxEditCtrl object.
	// Returns:
	//     The corresponding background color.
	// See Also:
	//     crBack, crReadOnlyBack
	//-----------------------------------------------------------------------
	COLORREF GetBackColorEx(CXTPSyntaxEditCtrl* pEditCtrl);
};

//===========================================================================
// Summary:
//     XTP_EDIT_ROWSBLOCK structure.
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_ROWSBLOCK
{
	XTP_EDIT_LINECOL lcStart; // Start position.
	XTP_EDIT_LINECOL lcEnd;   // End Position.

	CString strCollapsedText; // Collapsed text.
};

typedef CArray<XTP_EDIT_ROWSBLOCK, const XTP_EDIT_ROWSBLOCK&> CXTPSyntaxEditRowsBlockArray;

//===========================================================================
// Summary:
//     Stores collapsed block parameters.
// See Also:
//     XTP_EDIT_ROWSBLOCK
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_COLLAPSEDBLOCK
{
	XTP_EDIT_ROWSBLOCK collBlock; // Collapsed block parameters.
	CRect rcCollMark;			  // Rectangle area to display collapsed block.
};

const UINT XTP_EDIT_ROWNODE_NOTHING   = 0x00; // Define that row has no node mark.
const UINT XTP_EDIT_ROWNODE_COLLAPSED = 0x01; // Define that row has collapsed node mark.
const UINT XTP_EDIT_ROWNODE_EXPANDED  = 0x02; // Define that row has expanded node mark.
const UINT XTP_EDIT_ROWNODE_ENDMARK   = 0x04; // Define that row has end node mark.
const UINT XTP_EDIT_ROWNODE_NODEUP	= 0x10; // Define that row has up-line node mark.
const UINT XTP_EDIT_ROWNODE_NODEDOWN  = 0x20; // Define that row has down-line node mark.

//===========================================================================
// Summary:
//     Used with XTP_EDIT_FONTOPTIONS struct and define unspecified member value.
// See Also:
//     XTP_EDIT_FONTOPTIONS
//===========================================================================
#	define XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION (BYTE) - 1

//===========================================================================
// Summary:
//     The XTP_EDIT_FONTOPTIONS structure is a self-initializing LOGFONT structure
//     that initializes all values equal to XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION
//     except for lfFaceName which is set to zero length.
// See Also:
//     XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_FONTOPTIONS : public LOGFONT
{
	//-----------------------------------------------------------------------
	// Summary:
	//      Default constructor.
	//-----------------------------------------------------------------------
	XTP_EDIT_FONTOPTIONS();
};

//===========================================================================
// Summary
//     The XTP_EDIT_TEXTBLOCK structure stores the text blocks after parsing.
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_TEXTBLOCK
{
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	XTP_EDIT_TEXTBLOCK();

	int nPos;					   // Position.
	int nNextBlockPos;			   // Position for the next block.
	XTP_EDIT_COLORVALUES clrBlock; // Color for this block.
	XTP_EDIT_FONTOPTIONS lf;	   // Font options for this block.
};

//===========================================================================
// Summary:
//     The XTP_EDIT_ROWCOLRECT structure is used to
//     define a row and column selection rectangle.
//===========================================================================
struct XTP_EDIT_ROWCOLRECT
{
	int nRow1; // First row index for a text block.
	int nCol1; // First column index for a text block.
	int nRow2; // Last row index for a text block.
	int nCol2; // Last column index for a text block.
};

//////////////////////////////
// Notification structures
//////////////////////////////

//===========================================================================
// Summary:
//     Bookmark notification structure.
//     Provided as LPARAM with WM_NOTIFY while bookmarks must be drawn.
//===========================================================================
struct XTP_EDIT_SENMBOOKMARK
{
	NMHDR nmhdr;	 // First param should be NMHDR.
	HDC hDC;		 // HDC for drawing.
	RECT rcBookmark; // RECT for bookmark.
	int nRow;		 // The row number.
};

//===========================================================================
// Summary:
//     Row column notification structure.
//     Provided as LPARAM with WM_NOTIFY.
//     This is used to display the current row and column.
//===========================================================================
struct XTP_EDIT_NMHDR_ROWCOLCHANGED
{
	NMHDR nmhdr; // First param should be NMHDR.
	int nRow;	// Current document row.
	int nCol;	// Current document column.
};

//===========================================================================
// Summary:
//     Document notification structure.
//     Provided as LPARAM with WM_NOTIFY.
//     This is used to notify of a change in the document's modified state.
//===========================================================================
struct XTP_EDIT_NMHDR_DOCMODIFIED
{
	NMHDR nmhdr;	// First param should be NMHDR.
	BOOL bModified; // TRUE if the document was modified.
};

//===========================================================================
// Summary:
//     Structure to notify "Edit changed" event.
//===========================================================================
struct XTP_EDIT_NMHDR_EDITCHANGED
{
	NMHDR nmhdr;  // First param should be NMHDR.
	int nRowFrom; // The start row for the action.
	int nRowTo;   // The end row for the action.
	int nAction;  // The actual action.
};

//===========================================================================
// Summary:
//     Structure to notify XTP_EDIT_NM_MARGINCLICKED event.
//===========================================================================
struct XTP_EDIT_NMHDR_MARGINCLICKED
{
	NMHDR nmhdr;  // First param should be NMHDR.
	int nRow;	 // Document row number.
	int nDispRow; // Visible row number (start from 1).
};

//===========================================================================
// Summary:
//     Structure to notify XTP_EDIT_NM_UPDATESCROLLPOS event.
//===========================================================================
struct XTP_EDIT_NMHDR_SETSCROLLPOS
{
	NMHDR nmhdr;	// First param should be NMHDR.
	DWORD dwUpdate; // Scroll position update flags can be any combination of XTP_EDIT_UPDATE_HORZ,
					// XTP_EDIT_UPDATE_VERT or XTP_EDIT_UPDATE_DIAG.
};

//===========================================================================
// Summary:
//     Structure to notify XTP_EDIT_NM_ENABLESCROLLBAR event.
//===========================================================================
struct XTP_EDIT_NMHDR_ENABLESCROLLBAR
{
	NMHDR nmhdr;	   // First param should be NMHDR.
	DWORD dwScrollBar; // Define the scroll bar(s) to change state as WS_HSCROLL and WS_VSCROLL
					   // flags.
	DWORD dwState;	 // Define the scroll bar(s) state as WS_HSCROLL and WS_VSCROLL flags.
};

//===========================================================================
// Summary:
//     Structure to notify XTP_EDIT_NM_PARSEEVENT event.
//===========================================================================
struct XTP_EDIT_NMHDR_PARSEEVENT
{
	NMHDR nmhdr;   // First param should be NMHDR.
	DWORD code;	// Parser event. A value from the XTPSyntaxEditOnParseEvent enum.
	WPARAM wParam; // First event parameter.
	LPARAM lParam; // Second event parameter.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITSTRUCT_H__)
