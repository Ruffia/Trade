// XTPSyntaxEditCtrl.h : header file
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
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITSYNTAXEDITCTRL_H__)
#	define __XTPSYNTAXEDITSYNTAXEDITCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE
typedef CMap<int, int, COLORREF, COLORREF> CXTPSyntaxEditRowColorMap;
//}}AFX_CODEJOCK_PRIVATE

// forwards
class CXTPNotifySink;
class CXTPSyntaxEditBufferManager;
class CXTPSyntaxEditConfigurationManager;
class CXTPSyntaxEditToolTipCtrl;
class CXTPSyntaxEditAutoCompleteWnd;
class CXTPSyntaxEditLineMarksManager;
class CXTPSyntaxEditDrawTextProcessor;
class CXTPSyntaxEditSelection;
typedef CList<XTP_EDIT_TEXTBLOCK, XTP_EDIT_TEXTBLOCK> CXTPSyntaxEditTextBlockList;
typedef LPCTSTR XTP_EDIT_LINEMARKTYPE;
struct XTP_EDIT_LMPARAM;

//===========================================================================
// Summary:
//     Shared Options of SyntaxEdit.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditOptions : public CXTPCmdTarget
{
public:
	CXTPSyntaxEditOptions();

protected:
	BOOL m_bSyntaxColor;			// TRUE if syntax colorization is enabled, FALSE if disabled.
	BOOL m_bAutoIndent;				// TRUE if auto-indentation is enabled, FALSE if disabled.
	BOOL m_bSelMargin;				// TRUE if selection margin is enabled, FALSE if disabled.
	BOOL m_bLineNumbers;			// TRUE if line numbers are to be printed, FALSE otherwise.
	BOOL m_bWideCaret;				// Stored Overwrite Caret Style: Thin or Thick.
	BOOL m_bTabWithSpace;			// TRUE if tabs are replaced with spaces, FALSE otherwise.
	BOOL m_bVirtualSpace;			// TRUE if virtual space is enabled (i.e. the cursor can
									// be placed anywhere after the line end), otherwise FALSE.
	BOOL m_bDrawNodes;				// TRUE if collapsible nodes signs are printed, otherwise FALSE.
	BOOL m_bEnableWhiteSpace;		// TRUE if white space is shown, otherwise FALSE.
	BOOL m_bReadOnly;				// TRUE if in read-only mode, otherwise FALSE.
	BOOL m_bViewOnly;				// TRUE if in view-only mode, otherwise FALSE.
	BOOL m_bHideCaret;				// TRUE if the caret is hidden, otherwise FALSE.
	BOOL m_bEnableEditAccelerators; // TRUE if the accelerators for the Cut, Copy, Paste, Undo,
									// Redo, and Select All operations are enabled, FALSE if
									// disabled.

	CXTPSyntaxEditRowColorMap m_mapRowBkColor;   // Map for row back colors.
	CXTPSyntaxEditRowColorMap m_mapRowForeColor; // Map for row colors.

	friend class CXTPSyntaxEditCtrl;
};

//===========================================================================
// Summary:
//     This class is the main CWnd-based editor class. It is a rectangular
//     child window in which the user can enter and edit text.
// Remarks:
//     The control is also capable of enhancing text coloring for a number
//     of specific input file types (e.g. C/C++ source files, HTML files, etc.).
//     The user can create and customize their own text files schemes
//     (they are independent of source files).
//     The control contains the main configuration file, which can contain
//     references to specific configuration files. By default, this file
//     is placed in the same folder as the application executable and
//     is called <i>EditControl.ini</i>.
//     The main configuration file contains 2 blocks with references to
//     input file lexicographic schemes in a specific format (see below)
//     and color themes for the editor, which could be applied to the
//     control editing any file type. The format of the main configuration
//     file is as follows:
//<code>
//[Schemas]
// FileType1=SchemaFileName1.schclass
// FileType2=SchemaFileName2.schclass
//...
//
//[Themes]
// ThemeName1=ThemeFileName1.ini
// ThemeName2=ThemeFileName2.ini
//...
//</code>
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditCtrl : public CWnd
{
	DECLARE_DYNAMIC(CXTPSyntaxEditCtrl)

	friend class CXTPSyntaxEditAutoCompleteWnd;
	friend class CXTPSyntaxEditView;
	friend class CXTPSyntaxEditCommand;
	friend class CXTPSyntaxEditPaintManager;
	friend class CXTPSyntaxEditSelection;
	friend class CSyntaxEditCtrl; // for ActiveX implementation
	friend class CSyntaxEditBaseCtrl;

	class CImmWrapper;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditCtrl object, handles cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if line numbering is enabled/disabled.
	// Returns:
	//     TRUE if line numbering is enabled,
	//     FALSE if line numbering is disabled.
	//-----------------------------------------------------------------------
	BOOL GetLineNumbers() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables line numbering.
	// Parameters:
	//     bLineNumbers - [in] TRUE to enable line numbering,
	//                         FALSE to disable line numbering.
	//     bUpdateReg   - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                         By default, this parameter is FALSE.
	//-----------------------------------------------------------------------
	BOOL SetLineNumbers(BOOL bLineNumbers, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the collapsible nodes area is enabled/disabled.
	// Returns:
	//     TRUE if the collapsible nodes area is enabled,
	//     FALSE if the collapsible nodes area is disabled.
	//-----------------------------------------------------------------------
	BOOL GetCollapsibleNodes() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the collapsible nodes area.
	// Parameters:
	//     bDrawNodes - [in] TRUE to enable the collapsible nodes area,
	//                       FALSE to disable the collapsible nodes area.
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	//-----------------------------------------------------------------------
	void SetCollapsibleNodes(BOOL bDrawNodes, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Replaces all tabs with blank spaces.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL UnTabifySelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Replaces all blank spaces with tabs.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL TabifySelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the selected text to lowercase.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL LowercaseSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the selected text to uppercase.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL UppercaseSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current selection.
	// Returns:
	//     A reference to the current selection.
	// See Also:
	//     CXTPSyntaxEditSelection
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSelection& GetSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the auto-complete pop-up window.
	// Returns:
	//     A pointer to the auto-complete pop-up window.
	// See Also:
	//     CXTPSyntaxEditAutoCompleteWnd
	//-----------------------------------------------------------------------
	CXTPSyntaxEditAutoCompleteWnd* GetAutoCompleteWnd() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows that can be printed per page when
	//     using the current settings.
	// Returns:
	//     The number of rows that can be printed per page when
	//     using the current settings.
	//-----------------------------------------------------------------------
	int GetRowPerPage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified row is visible.
	// Parameters:
	//     iRow - [in] Document row number to be tested.
	// Returns:
	//     TRUE if the specified row is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsRowVisible(int iRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets right mouse button for dragging.
	// Parameters:
	//     bRightButtonDrag - [in] Boolean state flag of right button drag mode.
	// Remarks:
	//     Mainly used by <b>CXTPSyntaxEditView</b> for setting right button
	//     drag mode. If parameter is TRUE function sets right button for
	//     dragging, otherwise dragging mode is disabled for right mouse
	//     button.
	// See also:
	//     void CancelRightButtonDrag(), BOOL IsRightButtonDrag()
	//-----------------------------------------------------------------------
	void SetRightButtonDrag(BOOL bRightButtonDrag = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables scrollbars.
	// Parameters:
	//     bHorzSBar   - [in] TRUE to enable the horizontal scrollbar,
	//                        FALSE to disable the horizontal scrollbar.
	//     bVertSBar   - [in] TRUE to enable the vertical scrollbar,
	//                        FALSE to disable the vertical scrollbar.
	//     bUpdateReg  - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                        By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the scrollbars were updated, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetScrollBars(BOOL bHorzSBar, BOOL bVertSBar, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the horizontal scrollbar is enabled/disabled.
	// Returns:
	//     TRUE if the horizontal scrollbar is enabled,
	//     FALSE if the horizontal scrollbar is disabled.
	//-----------------------------------------------------------------------
	BOOL GetHorzScrollBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the vertical scrollbar is enabled/disabled.
	// Returns:
	//     TRUE if the vertical scrollbar is enabled,
	//     FALSE if the vertical scrollbar is disabled.
	//-----------------------------------------------------------------------
	BOOL GetVertScrollBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the gutter.
	// Parameters:
	//     bSelMargin - [in] TRUE to enable the gutter,
	//                       FALSE to disable the gutter.
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	// Remarks:
	//     The gutter is not visible if it is disabled.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     GetSelMargin
	//-----------------------------------------------------------------------
	BOOL SetSelMargin(BOOL bSelMargin, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the gutter is enabled/disabled.
	// Returns:
	//     TRUE if the gutter is enabled,
	//     FALSE if the gutter is disabled.
	// See Also:
	//     SetSelMargin
	//-----------------------------------------------------------------------
	BOOL GetSelMargin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables syntax colorization.
	// Parameters:
	//     bSyntaxColor - [in] TRUE to enable syntax colorization,
	//                         FALSE to disable syntax colorization.
	//     bUpdateReg   - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                         By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     GetSyntaxColor
	//-----------------------------------------------------------------------
	BOOL SetSyntaxColor(BOOL bSyntaxColor, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if syntax colorization is enabled/disabled.
	// Returns:
	//     TRUE if syntax colorization is enabled, otherwise FALSE.
	// See Also:
	//     SetSyntaxColor
	//-----------------------------------------------------------------------
	BOOL GetSyntaxColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables auto-indentation.
	// Parameters:
	//     bAutoIndent - [in] TRUE to enable auto-indentation,
	//                        FALSE to disable auto-indentation.
	//     bUpdateReg  - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                        By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     GetAutoIndent
	//-----------------------------------------------------------------------
	BOOL SetAutoIndent(BOOL bAutoIndent, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if auto-indentation is enabled/disabled.
	// Returns:
	//     TRUE if auto-indentation is enabled,
	//     FALSE if auto-indentation is disabled.
	// See Also:
	//     SetAutoIndent
	//-----------------------------------------------------------------------
	BOOL GetAutoIndent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab size.
	// Parameters:
	//     nTabSize   - [in] Tab size to be set (should be between 2 to 10).
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     GetTabSize
	//-----------------------------------------------------------------------
	BOOL SetTabSize(int nTabSize, BOOL bUpdateReg = FALSE);

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
	//     Determines if replacing tabs with spaces is enabled/disabled.
	// Returns:
	//     TRUE if replacing tabs with spaces is enabled,
	//     FALSE if replacing tabs with spaces is disabled.
	// See Also:
	//     SetTabWithSpace
	//-----------------------------------------------------------------------
	BOOL GetTabWithSpace() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables replacing tabs with spaces.
	// Parameters:
	//     bTabWithSpace - [in] TRUE to enable replacing tabs with spaces,
	//                          FALSE to disable replacing tabs with spaces.
	//     bUpdateReg    - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                          By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     GetTabWithSpace
	//-----------------------------------------------------------------------
	BOOL SetTabWithSpace(BOOL bTabWithSpace, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text color foreground for a specified row in the colors cache.
	// Parameters:
	//     nRow     - [in] Row identifier.
	//     clrFront - [in] Text color foreground to be set.
	// See Also:
	//     GetRowColor
	//-----------------------------------------------------------------------
	void SetRowColor(int nRow, COLORREF clrFront);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text color foreground of a specified row in the colors cache.
	// Parameters:
	//     nRow - [in] Row identifier.
	// Returns:
	//     The text color foreground of the specified row in the colors cache.
	// See Also:
	//     SetRowColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetRowColor(int nRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the background color of a specified row in the colors cache.
	// Parameters:
	//     nRow   - [in] Row identifier.
	//     crBack - [in] Background color to be set.
	// See Also:
	//     GetRowBkColor
	//-----------------------------------------------------------------------
	void SetRowBkColor(int nRow, COLORREF crBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the background color of a specified row in the colors cache.
	// Parameters:
	//     nRow - [in] Row identifier.
	// Returns:
	//     The background color of the specified row in the colors cache.
	// See Also:
	//     SetRowBkColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetRowBkColor(int nRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if showing white space is enabled/disabled.
	// Returns:
	//     TRUE if showing white space is enabled,
	//     FALSE if showing white space is disabled.
	// See Also:
	//     EnableWhiteSpace
	//-----------------------------------------------------------------------
	BOOL IsEnabledWhiteSpace() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables showing white space.
	// Parameters:
	//     bShow - [in] TRUE to enable showing white space,
	//                  FALSE to disable showing white space.
	// See Also:
	//     IsEnabledWhiteSpace
	//-----------------------------------------------------------------------
	void EnableWhiteSpace(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if virtual space mode is enabled/disabled.
	// Returns:
	//     TRUE if virtual space mode is enabled,
	//     FALSE if virtual space mode is disabled.
	// See Also:
	//     EnableVirtualSpace
	//-----------------------------------------------------------------------
	BOOL IsEnabledVirtualSpace() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables virtual space mode.
	// Parameters:
	//     bEnable    - [in] TRUE to enable virtual space mode,
	//                       FALSE to disable virtual space mode.
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	// See Also:
	//     IsEnabledWhiteSpace
	//-----------------------------------------------------------------------
	void EnableVirtualSpace(BOOL bEnable, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if mouse right button drag is enabled/disabled.
	// Returns:
	//     TRUE if mouse right button drag is enabled,
	//     FALSE if mouse right button drag is disabled.
	// See Also:
	//     SetRightButtonDrag, CancelRightButtonDrag
	//-----------------------------------------------------------------------
	BOOL IsRightButtonDrag() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Disables mouse right button drag.
	// See Also:
	//     SetRightButtonDrag, IsRightButtonDrag, CXTPSyntaxEditView
	//-----------------------------------------------------------------------
	void CancelRightButtonDrag();

	//-----------------------------------------------------------------------
	// Summary:
	//     Displays the default context menu.
	// Remarks:
	//     This member function calls OnContextMenu.
	// See Also:
	//     OnContextMenu
	//-----------------------------------------------------------------------
	void ShowDefaultContextMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles the drop operation within the same edit buffer.
	// Parameters:
	//     bCopy - [in] TRUE if drag is to copy,
	//                  FALSE to specify a move.
	//-----------------------------------------------------------------------
	void HandleDrop(BOOL bCopy);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves to a specified row.
	// Parameters:
	//     iRow       - [in] Row index.
	//     bSelectRow - [in] TRUE to select row, FALSE otherwise.
	//-----------------------------------------------------------------------
	void GoToRow(int iRow, BOOL bSelectRow = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Replaces the selected text with specified text.
	// Parameters:
	//     szNewText - [in] Text to be used to replace the selected text.
	// Returns:
	//     TRUE if the selected text was replaced, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ReplaceSel(LPCTSTR szNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Replaces all occurrences of szFindText with szReplaceText.
	// Parameters:
	//     szFindText      - [in] Pointer to the text to find.
	//     szReplaceText   - [in] Pointer to the text to be replaced.
	//     bMatchWholeWord - [in] TRUE to match whole word, FALSE otherwise.
	//     bMatchCase      - [in] TRUE to match case, FALSE otherwise.
	// Returns:
	//     The number of found matches.
	//-----------------------------------------------------------------------
	int ReplaceAll(LPCTSTR szFindText, LPCTSTR szReplaceText, BOOL bMatchWholeWord,
				   BOOL bMatchCase);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected text.
	// Parameters:
	//     strText - [out] Reference to a CString object to receive
	//                     the selected text.
	// Returns:
	//     The number of characters in the selected text.
	//-----------------------------------------------------------------------
	int GetSelectionText(CString& strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the drop position.
	// Parameters:
	//     iRow - [in] Row of the drop position.
	//     iCol - [in] Column of the drop position.
	// See Also:
	//     CXTPSyntaxEditView
	//-----------------------------------------------------------------------
	void SetDropPos(int iRow, int iCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the absolute column position.
	// Returns:
	//     The absolute column position.
	//-----------------------------------------------------------------------
	int GetCurAbsCol() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the handle of the selection buffer in global memory for
	//     a specified clipboard format (either CF_TEXT or CF_UNICODETEXT).
	// Parameters:
	//     nFormat - [in] Clipboard format (either CF_TEXT or CF_UNICODETEXT).
	// Remarks:
	//     This method is used mainly in OLE drag drop operations.
	// Returns:
	//     The handle of the selection buffer in global memory for
	//     the specified clipboard format (either CF_TEXT or CF_UNICODETEXT).
	//-----------------------------------------------------------------------
	HGLOBAL GetSelectionBuffer(UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables OLE drag drop mode.
	// Parameters:
	//     bEnableDrag - [in] TRUE to enable OLE drag drop mode,
	//                        FALSE to disable OLE drag drop mode.
	//-----------------------------------------------------------------------
	void EnableOleDrag(BOOL bEnableDrag = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Validates and sets the row column to a valid one.
	// Parameters:
	//     nRow    - [in] Document row to be validated.
	//     nCol    - [in, out] Column to be validated.
	//     nAbsCol - [in, out] Absolute column to be validated.
	//-----------------------------------------------------------------------
	void ValidateCol(const int nRow, int& nCol, int& nAbsCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row and column (not validated) at a specified point.
	//     Use CalcValidDispCol for validation.
	// Parameters:
	//     pt            - [in] Point in client coordinates.
	//     pRow          - [out] Pointer to an int to receive
	//                           the calculated row.
	//     pCol          - [out] Pointer to an int to receive
	//                           the calculated column.
	//     pDispRow      - [out] Pointer to an int to receive
	//                           the visible row.
	//     pDispCol      - [out] Pointer to an int to receive
	//                           the visible column.
	//     bVirtualSpace - [in] TRUE if virtual space is enabled, otherwise FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL RowColFromPoint(CPoint pt, int* pRow, int* pCol, int* pDispRow = NULL,
						 int* pDispCol = NULL, BOOL bVirtualSpace = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes a tab from the front of the selection.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DecreaseIndent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a tab to the front of the selection.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IncreaseIndent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Either deletes all lines in the selection or a specified line
	//     in the case that no selection exists.
	// Parameters:
	//     iForceDeleteRow - [in] Line to delete in the case that
	//                            no selection exists.
	//-----------------------------------------------------------------------
	void DeleteSelectedLines(int iForceDeleteRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the horizontal scrollbar.
	// Parameters:
	//     bShow - [in] TRUE to show the horizontal scrollbar,
	//                  FALSE to hide the horizontal scrollbar.
	//                  The default value for this parameter is TRUE.
	//-----------------------------------------------------------------------
	void ShowHScrollBar(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies or moves text after dragging.
	// Parameters:
	//     bCopy - [in] TRUE to copy text, FALSE to move text.
	//-----------------------------------------------------------------------
	void CopyOrMoveText(BOOL bCopy);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds specified text.
	// Parameters:
	//     szText          - [in] Pointer to the text to be found.
	//     bMatchWholeWord - [in] TRUE to find text matching the whole word,
	//                            FALSE otherwise.
	//     bMatchCase      - [in] TRUE to find text matching the case,
	//                            FALSE otherwise.
	//     bSearchDown     - [in] TRUE to search downward,
	//                            FALSE otherwise.
	//     bRedraw         - [in] TRUE to redraw the control,
	//                            FALSE otherwise.
	//                            The default value of this parameter is TRUE.
	//     nStartRow       - [in] Row number to start search. Set this
	//                            parameter to -1 to start from the current row.
	//     nStartCol       - [in] Column number to start search. Set this
	//                            parameter to -1 to start from the current column.
	// Returns:
	//     TRUE if the text was found, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Find(LPCTSTR szText, BOOL bMatchWholeWord, BOOL bMatchCase, BOOL bSearchDown,
			  BOOL bRedraw = TRUE, int nStartRow = -1, int nStartCol = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects the text bounded by a specified rectangle area.
	// Parameters:
	//     nRow1     - Start row of the text.
	//     nDispCol1 - Start visible column (character) of the text.
	//     nRow2     - End row of the text.
	//     nDispCol2 - End visible column (character) of the text.
	//     bRedraw   - TRUE to redraw the control,
	//                 FALSE otherwise.
	//                 The default value of this parameter is TRUE.
	// Returns:
	//     TRUE if the text was found, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Select(int nRow1, int nDispCol1, int nRow2, int nDispCol2, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deselects the previously selected rectangle area.
	// See Also:
	//     SetCurSel
	//-----------------------------------------------------------------------
	void Unselect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the control settings from a specified control.
	// Parameters:
	//     src - Reference to a CXTPSyntaxEditCtrl object to copy from.
	// Returns:
	//     A reference to this CXTPSyntaxEditCtrl object.
	//-----------------------------------------------------------------------
	const CXTPSyntaxEditCtrl& operator=(const CXTPSyntaxEditCtrl& src);

	//-----------------------------------------------------------------------
	// Summary:
	//     Restores the cursor.
	// Remarks:
	//     This method is mainly used when changing the font.
	//-----------------------------------------------------------------------
	void RestoreCursor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables overwrite mode.
	// Parameters:
	//     bOverwriteMode - [in] TRUE to enable overwrite mode,
	//                           FALSE to disable overwrite mode.
	//-----------------------------------------------------------------------
	void SetOverwriteMode(BOOL bOverwriteMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caret for overwrite mode.
	// Parameters:
	//     bWideCaret - [in] TRUE to set thin caret for overwrite mode,
	//                       FALSE to set wide caret for overwrite mode.
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetWideCaret(BOOL bWideCaret, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if thin caret is set for overwrite mode.
	// Returns:
	//     TRUE if thin caret is set for overwrite mode,
	//     FALSE if wide caret is set for overwrite mode.
	//-----------------------------------------------------------------------
	BOOL GetWideCaret() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the caret is hidden/visible.
	// Returns:
	//     TRUE if the caret is hidden, FALSE if the caret is visible.
	//-----------------------------------------------------------------------
	BOOL IsHideCaret() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Hides/shows the caret.
	// Parameters:
	//     bHide - TRUE to hide the caret, FALSE to show the caret.
	// Remarks:
	//     It may be useful to hide the caret in read-only mode.
	//-----------------------------------------------------------------------
	void SetHideCaret(BOOL bHide);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects the word at a specified point.
	// Parameters:
	//     point - [in] Point.
	//-----------------------------------------------------------------------
	void SelectWord(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current caret position to a specified row and column.
	// Parameters:
	//     nTextRow       - [in] Row number for the current caret position.
	//     nDispCol       - [in] Column number for the current caret position.
	//     bRowColNotify  - [in] TRUE to send an XTP_EDIT_NM_ROWCOLCHANGED
	//                           notification, FALSE otherwise.
	//     bEnsureVisible - [in] TRUE to ensure the visibility of the specified
	//                           row and column, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetCurCaretPos(int nTextRow, int nDispCol, BOOL bRowColNotify = TRUE,
						BOOL bEnsureVisible = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the buffer manager of the smart edit control.
	// Returns:
	//     A pointer to the buffer manager of the smart edit control.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager* GetEditBuffer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated configuration manager.
	// Returns:
	//     A pointer to the associated configuration manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditConfigurationManager* GetLexConfigurationManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated paint manager.
	// Returns:
	//     A pointer to the associated paint manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated paint manager.
	// Parameters:
	//     pPaintManager - [in] Pointer to a CXTPSyntaxEditPaintManager object.
	//-----------------------------------------------------------------------
	virtual void SetPaintManager(CXTPSyntaxEditPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the draw text processor.
	// Returns:
	//     A pointer to the draw text processor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDrawTextProcessor& GetDrawTextProcessor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts either a string or a vertical text block at a specified row and column.
	// Parameters:
	//     szText           - [in] Pointer to the string to be inserted.
	//     iRow             - [in] Row identifier.
	//     iCol             - [in] Column identifier.
	//     bDeleteSelection - [in] TRUE to delete the selected text at insertion,
	//                             FALSE otherwise.
	//                             The default value for this parameter is TRUE.
	//-----------------------------------------------------------------------
	void InsertString(LPCTSTR szText, int iRow, int iCol, BOOL bDeleteSelection = TRUE);
	void InsertTextBlock(
		LPCTSTR szText, int iRow, int iCol,
		BOOL bDeleteSelection = TRUE); // <combine
									   // CXTPSyntaxEditCtrl::InsertString@LPCTSTR@int@int@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the name of the main configuration file.
	// Returns:
	//     The name of the main configuration file.
	// See also:
	//     CXTPSyntaxEditCtrl::SetConfigFile
	//-----------------------------------------------------------------------
	CString GetConfigFile() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the path of the executable file module.
	// Returns:
	//     The path of the executable file module.
	//-----------------------------------------------------------------------
	static CString AFX_CDECL GetModulePath();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the the path to the main configuration file.
	// Parameters:
	//     szPath          - [in] Path to be set.
	//     bConfigFileMode - [in] Flag for configuration file mode.
	// Remarks:
	//     The configuration will be reloaded.
	// Returns:
	//     TRUE if the path was found, otherwise FALSE.
	// See also:
	//     CXTPSyntaxEditCtrl::GetConfigFile
	//-----------------------------------------------------------------------
	BOOL SetConfigFile(LPCTSTR szPath, BOOL bConfigFileMode = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font for the edit control.
	// Parameters:
	//     pLogfont   - [in] Pointer to a LOGFONT structure.
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	//-----------------------------------------------------------------------
	void SetFontIndirect(LPLOGFONT pLogfont, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current cursor position to a specified row and column.
	// Parameters:
	//     nTextRow        - [in] Row to be set in document coordinates.
	//     nDispCol        - [in] Column to be set in document coordinates.
	//     bRemainSelected - [in] TRUE for the selected text to remain selected,
	//                            FALSE for the selected text to be deselected.
	//     bForceVisible   - [in] TRUE to ensure the visibility of the specified
	//                            row and column, FALSE otherwise.
	// See Also:
	//     GetCurCol, GetCurRow
	//-----------------------------------------------------------------------
	void SetCurPos(int nTextRow, int nDispCol, BOOL bRemainSelected = FALSE,
				   BOOL bForceVisible = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current column.
	// Returns:
	//     The identifier of the current column.
	//-----------------------------------------------------------------------
	int GetCurCol() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current row.
	// Returns:
	//     The identifier of the current row.
	//-----------------------------------------------------------------------
	int GetCurRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows in the current document.
	// Returns:
	//     The number of rows in the current document.
	//-----------------------------------------------------------------------
	int GetRowCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects all text.
	//-----------------------------------------------------------------------
	void SelectAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the edit control client area.
	// Parameters:
	//     pDC    - [in] Pointer to a CDC object.
	//     rcRect - [in] Reference to a CRect object.
	// See Also:
	//     PrintPage
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, const CRect& rcRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints the edit control page.
	// Parameters:
	//     pDC    - [in] Pointer to a CDC object.
	//     rcRect - [in] Reference to a CRect object.
	//     nFlags - [in] Additional printing options. The following are supported:
	//                   DT_CALCRECT, DT_SINGLELINE, DT_WORDBREAK
	// Remarks:
	//     The top row is used as the first page line.
	// Returns:
	//     The number of printed rows.
	// See Also:
	//     Draw
	//-----------------------------------------------------------------------
	virtual int PrintPage(CDC* pDC, const CRect& rcRect, int nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the rectangles of the edit control regions.
	// Parameters:
	//     prcBookMarks - [out] Pointer to a CRect object to receive
	//                          the bookmarks area rectangle.
	//                          The value of this parameter may be NULL.
	//     prcLineNum   - [out] Pointer to a CRect object to receive
	//                          the line numbers area rectangle.
	//                          The value of this parameter may be NULL.
	//     prcNodes     - [out] Pointer to a CRect object to receive
	//                          the line nodes area rectangle.
	//                          The value of this parameter may be NULL.
	//     prcText      - [out] Pointer to a CRect object to receive
	//                          the text area rectangle.
	//                          The value of this parameter may be NULL.
	//     prcClient    - [in] Pointer to a CRect object containing the base edit
	//                         control rectangle to be used to calculate the other
	//                         rectangles. Set the value of this parameter to NULL
	//                         to use the edit control's window client rectangle.
	// Remarks:
	//     Empty rectangles will be returned for invisible/disabled regions.
	// Returns:
	//     The rectangle used to calculate the other rectangles.
	//-----------------------------------------------------------------------
	virtual CRect CalcEditRects(CRect* prcBookMarks = NULL, CRect* prcLineNum = NULL,
								CRect* prcNodes = NULL, CRect* prcText = NULL,
								const CRect* prcClient = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection exists.
	// Returns:
	//     TRUE if the selection exists, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelectionExist() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Pastes from the clipboard to the document.
	//-----------------------------------------------------------------------
	void Paste();

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the selected text to the clipboard.
	//-----------------------------------------------------------------------
	void Copy();

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the selected text to the clipboard and
	//     deletes the selected text from the document.
	//-----------------------------------------------------------------------
	void Cut();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if it is possible to perform the redo action.
	// Returns:
	//     TRUE if it is possible to perform the redo action, otherwise FALSE.
	// See Also:
	//     Redo
	//-----------------------------------------------------------------------
	BOOL CanRedo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if it is possible to perform the undo action.
	// Returns:
	//     TRUE if it is possible to perform the undo action, otherwise FALSE.
	// See Also:
	//     Undo
	//-----------------------------------------------------------------------
	BOOL CanUndo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs a redo operation.
	// Parameters:
	//     nActionsCount - [in] Number of actions to redo simultaneously.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     CanRedo
	//-----------------------------------------------------------------------
	BOOL Redo(int nActionsCount = 1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs a undo operation.
	// Parameters:
	//     nActionsCount - [in] Number of actions to undo simultaneously.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     CanUndo
	//-----------------------------------------------------------------------
	BOOL Undo(int nActionsCount = 1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the list of text for undo operations.
	// Returns:
	//     A reference to the list of text for undo operations.
	//-----------------------------------------------------------------------
	const CStringList& GetUndoTextList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the list of text for redo operations.
	// Returns:
	//     A reference to the list of text for redo operations.
	//-----------------------------------------------------------------------
	const CStringList& GetRedoTextList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes the character at a specified location.
	// Parameters:
	//     iRow - [in] Row identifier in document coordinates.
	//     iCol - [in] Column identifier in document coordinates.
	//     pos  - [in] Position qualifier.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     XTPSyntaxEditDeletePos enum.
	//-----------------------------------------------------------------------
	BOOL DeleteChar(int iRow, int iCol, XTPSyntaxEditDeletePos pos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes a specified range of text.
	// Parameters:
	//     iRowFrom - [in] Start row in document coordinates.
	//     iColFrom - [in] Start column in document coordinates.
	//     iRowTo   - [in] End row in document coordinates.
	//     iColTo   - [in] End column in document coordinates.
	//     nFlags   - [in] Mode flag. Possible values are:
	//                     smfRedraw, smfForceRedraw, smfTextAsBlock.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     XTPSyntaxEditFlags
	//-----------------------------------------------------------------------
	BOOL DeleteBuffer(int iRowFrom, int iColFrom, int iRowTo, int iColTo, int nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes the currently selected text.
	// Parameters:
	//     bRedraw - [in] TRUE to redraw the document,
	//                    FALSE otherwise.
	//                    The default value of this parameter is TRUE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DeleteSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the editor text.
	// Parameters:
	//     nMaxLen - [in] Maximum text length to get (in TCHARs).
	//                    Set this parameter to -1 to get all text.
	//                    The default value for this parameter is -1.
	//     memFile - [out] Reference to a CMemFile object to receive the text.
	// Remarks:
	//     CWnd::GetWindowText can also be used to get the editor text.
	// Returns:
	//     The first function returns the editor text if successful,
	//     otherwise an empty string.
	//     The second function returns TRUE if successful,
	//     otherwise FALSE.
	// See Also:
	//     SetText, CWnd::SetWindowText
	//-----------------------------------------------------------------------
	CString GetText(int nMaxLen = -1) const;
	BOOL GetText(CMemFile& memFile,
				 int nMaxLen = -1) const; // <combine CXTPSyntaxEditCtrl::GetText@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the editor text.
	// Parameters:
	//     pcszText - [in] Pointer to the editor text to be set.
	// Remarks:
	//     CWnd::SetWindowText can also be used to set the editor text.
	// See Also:
	//     GetText, CWnd::GetWindowText
	//-----------------------------------------------------------------------
	void SetText(LPCTSTR pcszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the top row of the visible window.
	// Parameters:
	//     iRow - [in] Identifier of the row to be set.
	// See Also:
	//     GetTopRow
	//-----------------------------------------------------------------------
	void SetTopRow(int iRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the top row of the visible window.
	// Returns:
	//     The identifier of the top row of the visible window.
	// See Also:
	//     SetTopRow
	//-----------------------------------------------------------------------
	int GetTopRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates and sets the scroll bars and other related areas
	//     depending on the text size and current position.
	//-----------------------------------------------------------------------
	void RecalcScrollBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions and resizes control bars in the client area of a window.
	// Parameters:
	//     nIDFirst     - [in] Indentifer of the first in a range of control bars
	//                         to reposition and resize.
	//     nIDLast      - [in] Identifier of the last in a range of control bars
	//                         to reposition and resize.
	//     nIDLeftOver  - [in] Identifier of the pane to fill the rest of
	//                         the client area.
	//     nFlag        - [in] This parameter can be one of the following values:
	//                         * CWnd::reposDefault: Performs the layout of
	//                                               the control bars. lpRectParam
	//                                               is not used and can be NULL.
	//                         * CWnd::reposQuery: The layout of the control bars
	//                                             is not done. Instead, lpRectParam
	//                                             is initialized with the size of
	//                                             the client area as if the layout
	//                                             had actually been done.
	//                         * CWnd::reposExtra: Adds the values of lpRectParam
	//                                             to the client area of nIDLast
	//                                             and also performs the layout.
	//     lpRectParam  - [in] Pointer to a RECT structure. The usage of this
	//                         structure depends on the value of nFlag.
	//     lpRectClient - [in] Pointer to a RECT structure containing the available
	//                         client area. If NULL, then the window's client area
	//                         will be used.
	//     bStretch     - [in] TRUE to stretch the bar to the size of the frame,
	//                         FALSE otherwise.
	// Remarks:
	//     The nIDFirst and nIDLast parameters define a range of control bar
	//     identifiers to be repositioned in the client area.
	//
	//     The nIDLeftOver parameter specifies the identifier of the child window
	//     (normally the view) which is repositioned and resized to fill the rest
	//     of the client area that is not filled by control bars.
	//-----------------------------------------------------------------------
	void RepositionBars(UINT nIDFirst, UINT nIDLast, UINT nIDLeftOver,
						UINT nFlag = CWnd::reposDefault, LPRECT lpRectParam = NULL,
						LPCRECT lpRectClient = NULL, BOOL bStretch = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to a specified sibling scroll bar.
	// Parameters:
	//     nBar - [in] Type of scrollbar to be retrieved.
	//                 This parameter can be one of the following values:
	//                 * SB_HORZ: Horizontal scroll bar.
	//                 * SB_VERT: Vertical scrollbar.
	// Remarks:
	//     This member function does not operate on scroll bars created
	//     when the WS_HSCROLL or WS_VSTROLL bits are set during the creation
	//     of a window.
	//
	///    The CWnd implementation of this method simply returns NULL.
	//
	//     Derived classes, such as CView, implement the described functionality.
	// Returns:
	//     A pointer to the specified sibling scroll bar, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the scroll bar.
	// Parameters:
	//     nBar    - [in] Scroll bar identifier.
	//     bEnable - [in] TRUE to enable the scroll bar,
	//                    FALSE to disable the scroll bar.
	// Remarks:
	//     If the window has a sibling scroll bar control, then that
	//     scroll bar is used, otherwise the window's own scroll bar is used.
	// See Also:
	//     ShowScrollBar
	//-----------------------------------------------------------------------
	void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides a specified scroll bar.
	// Parameters:
	//     nBar  - [in] Specifies if the scroll bar is a control or a part of
	//                  a window's non-client area. If the scroll bar is
	//                  a part of the non-client area, then the value of this
	//                  parameter also indicates if the scroll bar is positioned
	//                  horizontally, vertically, or both.
	//                  The value of this parameter must be one of the following:
	//                  * SB_BOTH: Specifies the horizontal and vertical
	//                             scroll bars of the window.
	//                  * SB_HORZ: Specifies that the window is a horizontal
	//                             scroll bar.
	//                  * SB_VERT: Specifies that the window is a vertical
	//                             scroll bar.
	//     bShow - [in] TRUE to show the scroll bar, FALSE to hide the scroll bar.
	// Remarks:
	//     An application should not call ShowScrollBar to hide a scroll bar
	//     while processing a scroll bar notification message.
	// See Also:
	//     EnableScrollBarCtrl
	//-----------------------------------------------------------------------
	void ShowScrollBar(UINT nBar, BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum and maximum position values for a specified scroll bar.
	// Parameters:
	//     nBar    - [in] Specifies the scrollbar to be set.
	//                    The value of this parameter must be one of the following:
	//                    * SB_HORZ: Sets the range of the horizontal
	//                               scroll bar of the window.
	//                    * SB_VERT: Sets the range of the vertical
	//                               scroll bar of the window.
	//     nMinPos - [in] Specifies the minimum scrolling position.
	//     nMaxPos - [in] Specifies the maximum scrolling position.
	//     bRedraw - [in] TRUE to redraw the scrollbar, FALSE otherwise.
	// Remarks:
	//     This method can also be used to hide or show standard scroll bars.
	//
	//     An application should not call this function to hide a scroll bar
	//     while processing a scroll-bar notification message.
	//
	//     If the call to SetScrollRange immediately follows a call to the
	//     SetScrollPos member function, the bRedraw parameter in the
	//     SetScrollPos member function should be set to 0 to prevent the
	//     scroll bar from being drawn twice.
	//
	//     The default range for a standard scroll bar is 0 through 100.
	//     The default range for a scroll bar control is empty (both the
	//     nMinPos and nMaxPos values are 0).
	//
	//     The difference between the values specified by nMinPos and nMaxPos
	//     must not be greater than INT_MAX.
	//-----------------------------------------------------------------------
	void SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current position of a specified scroll box and, if requested,
	//     redraws the scroll bar to reflect the new position of the scroll box.
	// Parameters:
	//     nBar    - [in] Specifies the scroll bar to be set.
	//                    The value of this parameter must be one of the following:
	//                    * SB_HORZ: Sets the position of the scroll box in
	//                               the horizontal scroll bar of the window.
	//                    * SB_VERT: Sets the position of the scroll box in
	//                               the vertical scroll bar of the window.
	//     nPos    - [in] Specifies the new position of the scroll box. The value
	//                     of this parameter must be within the scrolling range.
	//     bRedraw - [in] TRUE to redraw the scroll bar, FALSE otherwise.
	// Remarks:
	//     Setting bRedraw to FALSE is useful whenever the scroll bar will
	//     be redrawn by a subsequent call to another function.
	// Returns:
	//     The previous position of the scroll box.
	//-----------------------------------------------------------------------
	int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the information that the SCROLLINFO structure maintains
	//     about a specified scroll bar.
	// Parameters:
	//     nBar         - [in] Specifies the scroll bar to be set.
	//                         The value of this parameter must be one of the following:
	//                         * SB_HORZ: Sets the position of the scroll box in
	//                                    the horizontal scroll bar of the window.
	//                         * SB_VERT: Sets the position of the scroll box in
	//                                    the vertical scroll bar of the window.
	//     lpScrollInfo - [in] Pointer to a SCROLLINFO structure.
	//     bRedraw      - [in] TRUE to redraw the scroll bar, FALSE otherwise.
	// Remarks:
	//     You must provide the values required by the SCROLLINFO structure
	//     parameters, including the flag values.
	//
	//     The SCROLLINFO structure contains information about a scroll bar,
	//     including the minimum and maximum scrolling positions, the page
	//     size, and the position of the scroll box (the thumb). See the
	//     SCROLLINFO structure topic in the Platform SDK for more information
	//     about changing the structure defaults.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetScrollInfo, GetScrollLimit
	//-----------------------------------------------------------------------
	BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the scroll limit of a specified scroll bar.
	// Parameters:
	//     nBar - [in] Specified the type of scroll bar.
	//                 The value of this parameter must be one of the following:
	//                 * SB_HORZ: Retrieves the scroll limit of
	//                            the horizontal scroll bar.
	//                 * SB_VERT: Retrieves the scroll limit of
	//                            the vertical scroll bar.
	// Returns:
	//     The scroll limit of the specified scrollbar.
	//-----------------------------------------------------------------------
	int GetScrollLimit(int nBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the information of a specified scroll bar.
	// Parameters:
	//     nBar         - [in] Specifies if the scroll bar is a control or a part of
	//                         a window's non-client area. If the scroll bar is
	//                         a part of the non-client area, then the value of this
	//                         parameter also indicates if the scroll bar is positioned
	//                         horizontally, vertically, or both.
	//                         The value of this parameter must be one of the following:
	//                         * SB_CTL: Contains the parameters for a scroll bar
	//                                   control. The m_hWnd data member must be
	//                                   the handle of the scroll bar control.
	//                         * SB_HORZ: Specifies that the window is a horizontal
	//                                    scroll bar.
	//                         * SB_VERT: Specifies that the window is a vertical
	//                                    scroll bar.
	//     lpScrollInfo - [out] Pointer to a SCROLLINFO structure. See the Platform SDK
	//                          for more information about this structure.
	//     nMask        - [in] Specifies the scroll bar parameters to retrieve.
	//                         The default specifies a combination of SIF_PAGE,
	//                         SIF_POS, SIF_TRACKPOS, and SIF_RANGE. See SCROLLINFO
	//                         for more information on the nMask values.
	// Remarks:
	//     Call this member function to retrieve the information that the
	//     SCROLLINFO structure maintains about a scroll bar.
	//     GetScrollInfo enables applications to use 32-bit scroll positions.
	//
	//     The SCROLLINFO structure contains information about a scroll bar,
	//     including the minimum and maximum scrolling positions, the page
	//     size, and the position of the scroll box (the thumb). See the
	//     SCROLLINFO structure topic in the Platform SDK for more information
	//     about changing the structure defaults.
	//
	//     The MFC Windows message handlers that indicate scroll-bar position,
	//     CWnd::OnHScroll and CWnd::OnVScroll, provide only 16 bits of position
	//     data. GetScrollInfo and SetScrollInfo provide 32 bits of scroll-bar
	//     position data. Thus, an application can call GetScrollInfo while
	//     processing either CWnd::OnHScroll or CWnd::OnVScroll to obtain 32-bit
	//     scroll-bar position data.
	// Returns:
	//     TRUE if the message retrieved any values, otherwise FALSE.
	// See Also:
	//     SetScrollInfo
	//-----------------------------------------------------------------------
	BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Scrolls the specified client area with additional features.
	// Parameters:
	//     dx           - [in] Specifies the amount, in device units, of horizontal scrolling.
	//                         This parameter must have a negative value to scroll to the left.
	//     dy           - [in] Specifies the amount, in device units, of vertical scrolling.
	//                         This parameter must have a negative value to scroll up.
	//     lpRectScroll - [in] Points to a RECT structure that specifies the portion
	//                         of the client area to be scrolled. If this parameter
	//                         is NULL, the entire client area is scrolled.
	//     lpRectClip   - [in] Points to a RECT structure that specifies the clipping
	//                         rectangle to scroll. This structure takes precedence
	//                         over the rectangle pointed to by lpRectScroll. Only
	//                         bits inside this rectangle are scrolled. Bits outside
	//                         this rectangle are not affected even if they are in
	//                         the lpRectScroll rectangle. If this parameter is NULL,
	//                         no clipping is performed on the scroll rectangle.
	//     prgnUpdate   - [in] Identifies the region that is modified to hold the
	//                         region invalidated by scrolling. This parameter
	//                         may be NULL.
	//     lpRectUpdate - [out] Points to a RECT structure that will receive the
	//                         boundaries of the rectangle invalidated by scrolling.
	//                         This parameter may be NULL.
	//     flags        - [in] Can have one of the following values:
	//                         * SW_ERASE: When specified with SW_INVALIDATE,
	//                                     erases the newly invalidated region by
	//                                     sending a WM_ERASEBKGND message to the
	//                                     window.
	//                         * SW_INVALIDATE: Invalidates the region identified
	//                                          by prgnUpdate after scrolling.
	//                         * SW_SCROLLCHILDREN: Scrolls all child windows that
	//                                              intersect the rectangle pointed
	//                                              to by lpRectScroll by the number
	//                                              of pixels specified in dx and dy.
	//                                              Windows sends a WM_MOVE message to
	//                                              all child windows that intersect
	//                                              lpRectScroll, even if they do not
	//                                              move. The caret is repositioned when
	//                                              a child window is scrolled and the
	//                                              cursor rectangle intersects the
	//                                              scroll rectangle.
	// Returns:
	//     If the function succeeds, then the return value is
	//     SIMPLEREGION (rectangular invalidated region),
	//     COMPLEXREGION (nonrectangular invalidated region; overlapping rectangles),
	//     or NULLREGION (no invalidated region).
	//
	//     If the function fails, then the return value is ERROR.
	//     To get extended error information, call GetLastError.
	//-----------------------------------------------------------------------
	int ScrollWindowEx(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, CRgn* prgnUpdate,
					   LPRECT lpRectUpdate, UINT flags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Scrolls the specified client area.
	// Parameters:
	//     xAmount    - [in] Specifies the amount, in device units, of horizontal
	//                       scrolling. This parameter must be a negative value
	//                       to scroll to the left.
	//     yAmount    - [in] Specifies the amount, in device units, of vertical
	//                       scrolling. This parameter must be a negative value
	//                       to scroll up.
	//     lpRect     - [in] Points to a CRect object or RECT structure that
	//                       specifies the portion of the client area to be scrolled.
	//                       If lpRect is NULL, the entire client area is scrolled.
	//                       The caret is repositioned if the cursor rectangle
	//                       intersects the scroll rectangle.
	//     lpClipRect - [in] Points to a CRect object or RECT structure that
	//                       specifies the clipping rectangle to scroll.
	//                       Only bits inside this rectangle are scrolled.
	//                       Bits outside this rectangle are not affected
	//                       even if they are in the lpRect rectangle.
	//                       If lpClipRect is NULL, no clipping is performed
	//                       on the scroll rectangle.
	//-----------------------------------------------------------------------
	void ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect = NULL, LPCRECT lpClipRect = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the range of a specified scroll bar.
	// Parameters:
	//     nBar     - [in] Specifies the scrollbar to examine.
	//                     The value of this parameter must be one of the following:
	//                     * SB_HORZ - Retrieves the position of
	//                                 the horizontal scroll bar.
	//                     * SB_VERT - Retrieves the position of
	//                                 the vertical scroll bar.
	//     lpMinPos - [in] Pointer to an int to receive the minimum position.
	//     lpMaxPos - [in] Pointer to an int to receive the maximum position.
	//-----------------------------------------------------------------------
	void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of a specified scroll box.
	// Parameters:
	//     nBar     - [in] Specifies the scrollbar to examine.
	//                     The value of this parameter must be one of the following:
	//                     * SB_HORZ - Retrieves the position of
	//                                 the horizontal scroll bar.
	//                     * SB_VERT - Retrieves the position of
	//                                 the vertical scroll bar.
	// Returns:
	//     The position of the scroll box in the specified scroll bar if successful,
	//     otherwise 0.
	//-----------------------------------------------------------------------
	int GetScrollPos(int nBar) const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Scrolls the window by the specified offsets along the x- and y- axes.
	// Parameters:
	//     yPos -  [in] Scroll vertically in rows.
	//     xPos -  [in] Scroll Horizontally in columns.
	// ----------------------------------------------------------------------
	void Scroll(int xPos, int yPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures the visibility of a specified row.
	// Parameters:
	//     nTextRow - [in] Row identifier in document coordinates.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL EnsureVisibleRow(int nTextRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures the visibility of a specified location.
	// Parameters:
	//     nDispRow - [in] Row identifier in display coordinates.
	//     nDispCol - [in] Column identifier in display coordinates.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL EnsureVisibleCol(int nDispRow, int nDispCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the window.
	// Parameters:
	//     pParent     - [in] Pointer to the parent window (must not be NULL).
	//     bHorzScroll - [in] TRUE to use a horizontal scroll bar, FALSE otherwise.
	//     bVertScroll - [in] TRUE to use a vertical scroll bar, FALSE otherwise.
	//     pBuffer     - [in] Pointer to the buffer for the control (used by splitter).
	//     lpCS        - [in] Pointer to the create context of the window.
	//     nID         - [in] Control identifier.
	// Returns:
	//     TRUE if the window was created successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Create(CWnd* pParent, BOOL bHorzScroll, BOOL bVertScroll,
				CXTPSyntaxEditBufferManager* pBuffer = NULL, CCreateContext* lpCS = NULL,
				UINT nID = 0);

	using CWnd::Create;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds/removes a breakpoint to/from a specified row.
	// Parameters:
	//     nRow - [in] Row identifier in document coordinates.
	// Remarks:
	//     If the row does not have a breakpoint, then
	//     a breakpoint will be added.
	//     If the row does have a breakpoint, then
	//     the breakpoint will be removed.
	//-----------------------------------------------------------------------
	void AddRemoveBreakPoint(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds/removes a bookmark to/from a specified row.
	// Parameters:
	//     nRow - [in] Row identifier in document coordinates.
	// Remarks:
	//     If the row does not have a bookmark, then
	//     a bookmark will be added.
	//     If the row does have a bookmark, then
	//     a bookmark will be removed.
	//-----------------------------------------------------------------------
	void AddRemoveBookmark(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands/collapses the block at a specified row.
	// Parameters:
	//     nRow - [in] Row identifier in document coordinates.
	// Remarks:
	//     If the block at the specified row is expanded, then
	//     it will be collapsed.
	//     If the block at the specified row is collapsed, then
	//     it will be expanded.
	//-----------------------------------------------------------------------
	void CollapseExpandBlock(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the breakpoint from a specified row.
	// Parameters:
	//     nRow - [in] Row identifier in document coordinates.
	//-----------------------------------------------------------------------
	void DeleteBreakpoint(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the bookmark from a specified row.
	// Parameters:
	//     nRow - [in] Row identifier in document coordinates.
	//-----------------------------------------------------------------------
	void DeleteBookmark(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the current position to the previous bookmark.
	//-----------------------------------------------------------------------
	void PrevBookmark();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the current position to the next bookmark.
	//-----------------------------------------------------------------------
	void NextBookmark();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current document has breakpoints.
	// Returns:
	//     TRUE if the current document has breakpoints, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasBreakpoints() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current document has bookmarks.
	// Returns:
	//     TRUE if the current document has bookmarks, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasBookmarks() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified row has a specified mark.
	// Parameters:
	//     nRow   - [in] Row identifier in document coordinates.
	//     lmType - [in] Mark type identifier.
	//     pParam - [in] Pointer to XTP_EDIT_LMPARAM.
	//                   The default value for this parameter is NULL.
	// Returns:
	//     TRUE if the specified row has the specified mark, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_LINEMARKTYPE, XTP_EDIT_LMPARAM
	//-----------------------------------------------------------------------
	BOOL HasRowMark(int nRow, const XTP_EDIT_LINEMARKTYPE& lmType,
					XTP_EDIT_LMPARAM* pParam = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all expanded collapsible nodes.
	// See Also:
	//     ExpandAll
	//-----------------------------------------------------------------------
	void CollapseAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all collapsed nodes.
	// See Also:
	//     CollapseAll
	//-----------------------------------------------------------------------
	void ExpandAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the array of available theme names.
	// Returns:
	//     A reference to the array of available theme names.
	//-----------------------------------------------------------------------
	CStringArray& GetThemes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a theme with a specified name to the editor.
	// Parameters:
	//     strTheme - [in] Name of the theme to be applied.
	//-----------------------------------------------------------------------
	void ApplyTheme(CString strTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current theme name.
	// Returns:
	//     The current theme name.
	//-----------------------------------------------------------------------
	CString GetCurrentTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the colors for the current document.
	// Remarks:
	//     Refreshing colors causes configuration files to be reread and
	//     the current document to be reparsed.
	//-----------------------------------------------------------------------
	virtual void RefreshColors();

	//-----------------------------------------------------------------------
	// Summary:
	//     Invalidates a specified row.
	// Parameters:
	//     nRow - [in] Row identifier in document coordinates.
	//-----------------------------------------------------------------------
	void InvalidateRow(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Invalidates a specified set of rows.
	// Parameters:
	//     nRowFrom - [in] First row identifier in document coordinates.
	//     nRowTo   - [in] Last row identifier in document coordinates.
	//-----------------------------------------------------------------------
	void InvalidateRows(int nRowFrom = -1, int nRowTo = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified row is valid.
	// Parameters:
	//     nDispRow - [in] Row identifier in document coordinates.
	// Returns:
	//     TRUE if the specified row is valid, otherwise FALSE.
	// See Also:
	//     SetRowValid
	//-----------------------------------------------------------------------
	BOOL IsRowValid(int nDispRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Validates a specified row.
	// Parameters:
	//     nDispRow - [in] Row identifier in document coordinates.
	// See Also:
	//     IsRowValid
	//-----------------------------------------------------------------------
	void SetRowValid(int nDispRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the edit control is actived/deactivated.
	// Returns:
	//     TRUE if the edit control is actived,
	//     FALSE if the edit control is deactivated.
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Activates/deactives the edit control.
	// Parameters:
	//     bIsActive - TRUE to activate the edit control,
	//                 FALSE to deactivate the edit control.
	//-----------------------------------------------------------------------
	virtual void SetActive(BOOL bIsActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if scroll bars are created in the parent window.
	// Returns:
	//     TRUE if scroll bars are created in the parent window, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCreateScrollbarOnParent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if scroll bars should be created in the parent window.
	// Parameters:
	//     nSet - TRUE to create scroll bars in the parent window, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetCreateScrollbarOnParent(BOOL nSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the document can be edited.
	// Returns:
	//     TRUE if the document can be edited, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CanEditDoc() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets if the editor is in read-only mode.
	// Returns:
	//     TRUE if the editor is in read-only mode, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsReadOnly() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the editor should be in read-only mode.
	// Parameters:
	//     bReadOnly - TRUE to set the editor to read-only mode,
	//                 FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void SetReadOnly(BOOL bReadOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//    Gets if the editor is in view-only mode.
	// Remarks:
	//    View-only mode is the same as read-only except with a normal background,
	//    no cursor (caret), no selectable text, and no copy/paste.
	// Returns:
	//    TRUE if the edit is in view-only mode, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsViewOnly() const;

	//-----------------------------------------------------------------------
	// Summary:
	//    Sets if the editor should be in view-only mode.
	// Parameters:
	//    bViewOnly - TRUE to set the editor to view-only mode,
	//                FALSE otherwise.
	// Remarks:
	//    View-only mode is the same as read-only except with a normal background,
	//    no cursor (caret), no selectable text, and no copy/paste.
	//-----------------------------------------------------------------------
	void SetViewOnly(BOOL bViewOnly);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if editor accelerators are enabled/disabled.
	// Remarks:
	//     Standard editor accelerators include:
	//     Ctrl+C, Ctrl+V, Ctrl+X, Ctrl+Y, Ctrl+Z, Ctrl+Shift+Z, Ctrl+A,
	//     Ctrl+Ins, Shift+Ins, Alt+Backspace.
	// Remarks:
	//     You can support any of these editing operations externally
	//     by calling member functions.
	//
	//     By default, this feature is disabled.
	// Returns:
	//     TRUE if editor accelerators are enabled,
	//     FALSE if editor accelerators are disabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabledEditAccelerators() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables editor accelerators.
	// Parameters:
	//     bEnable - TRUE to enable editor accelerators,
	//               FALSE to disable editor accelerators.
	// Remarks:
	//     By default, this feature is disabled.
	//-----------------------------------------------------------------------
	void EnableEditAccelerators(BOOL bEnable);

	BOOL HasFocus() const;

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual void OnFinalRelease();

	virtual BOOL _InitEditControl();
	virtual DWORD ProcessCollapsedRowsBeroreDraw(int nTextRow, int& rnSkipRowsCount);
	virtual BOOL _IsVirtualSpaceActive() const;
	virtual void _EnsureVisible(int nTextRow, int nDispCol);

	virtual BOOL SetValueInt(LPCTSTR lpszValue, int nNewValue, int& nRefValue, BOOL bUpdateReg);
	virtual BOOL SetValueBool(LPCTSTR lpszValue, BOOL bNewValue, BOOL& bRefValue, BOOL bUpdateReg);

	void SetInternalRowBkColor(int nRow, COLORREF crBack);
	void SetInternalRowColor(int nRow, COLORREF crText);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wBufferSize, LPARAM lpBuffer);
	afx_msg LRESULT OnGetTextLen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputLanguage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDragCopy();
	afx_msg void OnDragMove();
	afx_msg UINT OnGetDlgCode();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the rectangle(s) for a collapsible node icon.
	// Parameters:
	//     nDispRow    - [in] Window row identifier for which to print.
	//     rcNode      - [out] Reference to a CRect object to receive
	//                         the node icon rectangle.
	//     prcNodeFull - [out] Pointer to a CRect object to receive
	//                         the full node area rectangle.
	//-----------------------------------------------------------------------
	void GetLineNodeRect(int nDispRow, CRect& rcNode, CRect* prcNodeFull = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the edit bar length.
	// Parameters:
	//     pDC - [in] Pointer to a device context.
	//-----------------------------------------------------------------------
	void CalculateEditbarLength(CDC* pDC = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the scroll bars
	//     depending on the text size and current position.
	//-----------------------------------------------------------------------
	void _RecalcScrollBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the maximum width of the horizontal scrollbar
	//     depending on the visible text size.
	//-----------------------------------------------------------------------
	void _RecalcHScrollMaxWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates the IME status.
	//-----------------------------------------------------------------------
	void _UpdateIMEStatus();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds tabs and spaces to specified text until it reaches
	//     a specified display column.
	// Parameters:
	//     rstrTextToIns - [in, out] Reference to the text to be added to.
	//     nDispCol      - [in] Identifier of the display column to reach.
	//-----------------------------------------------------------------------
	void FillTabs(CString& rstrTextToIns, int nDispCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills and/or calculates number of tabs or blank spaces required.
	// Parameters:
	//     szText            - [in] Text to be inserted.
	//     strTextToIns      - [out] Return string with all the indentation.
	//     iNewRow           - [out] New row.
	//     iNewCol           - [out] New absolute column.
	//     iNewDispCol       - [out] New display column.
	//     iEditRowFrom      - [out] Start row for XTP_EDIT_NM_EDITCHANGED.
	//     iEditRowTo        - [out] End row for XTP_EDIT_NM_EDITCHANGED.
	//     iChainActionCount - [out] Chain action count for undo.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL CreateInsertText(LPTSTR szText, CString& strTextToIns, int& iNewRow, int& iNewCol,
						  int& iNewDispCol, int& iEditRowFrom, int& iEditRowTo,
						  int& iChainActionCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends an XTP_EDIT_NM_EDITCHANGED notification with specified parameters.
	// Parameters:
	//     iRowFrom - [in] Start row identifier.
	//     iRowTo   - [in] End row identifier.
	//     nActions - [in] Action mask.
	//-----------------------------------------------------------------------
	void NotifyEditChanged(int iRowFrom, int iRowTo, UINT nActions);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a notification when the current text position is changed.
	// Parameters:
	//     nRow - [in] Row to be notified (in document coordinates).
	//     nCol - [in] Column to be notified.
	//-----------------------------------------------------------------------
	void NotifyCurRowCol(int nRow, int nCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends any notifications to the parent.
	// Parameters:
	//     uCode - [in] Notification code.
	// Returns:
	//     The LRESULT value returned by the parent notification handler.
	//-----------------------------------------------------------------------
	virtual LRESULT NotifyParent(UINT uCode) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends any mouse notifications to the parent.
	// Parameters:
	//     uCode  - [in] Mouse notification code (e.g. XTP_EDIT_NM_LBUTTONDOWN).
	//     nFlags - [in] Mouse event flags.
	//     point  - [in] Reference to the mouse event cursor position.
	// Returns:
	//     The LRESULT value returned by the parent notification handler.
	// See Also:
	//     XTP_EDIT_NM_LBUTTONDOWN, XTP_EDIT_NM_LBUTTONDBLCLICK, XTP_EDIT_NM_LBUTTONUP,
	//     XTP_EDIT_NM_RBUTTONDOWN, XTP_EDIT_NM_RBUTTONUP, XTP_EDIT_NM_MOUSEMOVE.
	//-----------------------------------------------------------------------
	virtual LRESULT NotifyMouseEvent(UINT uCode, UINT nFlags, const CPoint& point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the parent that the user clicked in the bookmark area.
	// Parameters:
	//     nRow     - [in] Document row.
	//     nDispRow - [in] Visible row.
	// Returns:
	//     TRUE if the action was handled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL NotifyMarginLBtnClick(int nRow, int nDispRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Initializes the selection of all sibling views.
	//-----------------------------------------------------------------------
	virtual void NotifySelInit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the horizontal scroll bar.
	//-----------------------------------------------------------------------
	void RecalcHorzScrollPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the vertical scroll bar.
	//-----------------------------------------------------------------------
	void RecalcVertScrollPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     Searches for the next/previous word position.
	// Parameters:
	//     nFindWhat    - [in] XTP_EDIT_FINDWORD_NEXT to find the next word,
	//                         XTP_EDIT_FINDWORD_PREV to find the previous word.
	//     nTextRow     - [in] Row identifier.
	//     nCol         - [in] Column identifier (column is one-based).
	//     lcPos_str    - [in] Text position (column is zero-based).
	//     rlcWordStart - [out] Reference to an XTP_EDIT_LINECOL object
	//                          to receive the word start position.
	//     rlcWordEnd   - [out] Reference to an XTP_EDIT_LINECOL object
	//                          to receive the word end position.
	//     rbOverSpace  - [out] Reference to a BOOL to receive a value of
	//                          TRUE if the search began on a space character or
	//                          FALSE if the search began on a tab character.
	// Returns:
	//     TRUE if the word was found, otherwise FALSE.
	//-----------------------------------------------------------------------
	void FindWord(UINT nFindWhat);
	BOOL FindWordEx(UINT nFindWhat, int nTextRow, int nCol, XTP_EDIT_LINECOL& rlcWordStart,
					XTP_EDIT_LINECOL& rlcWordEnd,
					BOOL& rbOverSpace); // <combine CXTPSyntaxEditCtrl::FindWord@UINT>
	BOOL FindWordEx_str(UINT nFindWhat, XTP_EDIT_LINECOL lcPos_str, XTP_EDIT_LINECOL& rlcWordStart,
						XTP_EDIT_LINECOL& rlcWordEnd,
						BOOL& rbOverSpace); // <combine CXTPSyntaxEditCtrl::FindWord@UINT>

	//-----------------------------------------------------------------------
	// Summary:
	//     Attempts to match text using the specified parameters.
	// Parameters:
	//     nRow            - [in] Row to match for.
	//     szLineText      - [in] Line text.
	//     szMatchText     - [in] Text to be matched.
	//     nStartPos       - [in] Start position.
	//     bMatchWholeWord - [in] TRUE to match whole words,
	//                            FALSE otherwise.
	//     bMatchCase      - [in] TRUE to match case,
	//                            FALSE otherwise.
	//     bSearchForward  - [in] TRUE to search forward,
	//                            FALSE otherwise.
	//     bRedraw         - [in] TRUE to redraw the control after the search,
	//                            FALSE otherwise.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL MatchText(int nRow, LPCTSTR szLineText, LPCTSTR szMatchText, int nStartPos,
				   BOOL bMatchWholeWord, BOOL bMatchCase, BOOL bSearchForward, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the effective parent window.
	// Returns:
	//     A pointer to the effective parent window.
	//-----------------------------------------------------------------------
	CWnd* GetEffectiveParent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculate and returns the display column for another display column
	//     specified in the iCol parameter.
	// Parameters:
	//     szText - [in] The text on which the calculation should be made.
	//     iCol   - [in] The display column for which calculation is needed.
	// Returns:
	//     The display column as integer value.
	//-----------------------------------------------------------------------
	int CalcValidDispCol(LPCTSTR szText, int iCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the absolute column for a specified display column.
	// Parameters:
	//     szText     - [in] Text for calculation.
	//     nDispCol   - [in] Display column position.
	// Returns:
	//     The calculated absolute column.
	//-----------------------------------------------------------------------
	int CalcAbsCol(LPCTSTR szText, int nDispCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the display column for a specified absolute column.
	// Parameters:
	//     szText     - [in] Text for calculation.
	//     nActualCol - [in] Absolute column position.
	// Returns:
	//     The calculated display column.
	//-----------------------------------------------------------------------
	int CalcDispCol(LPCTSTR szText, int nActualCol) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the absolute column for a specified display column.
	// Parameters:
	//     nTextRow   - [in] Row identifier.
	//     nDispCol   - [in] Display column position.
	// Returns:
	//     The calculated absolute column.
	//-----------------------------------------------------------------------
	int CalcAbsCol(int nTextRow, int nDispCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the display column for a specified absolute column.
	// Parameters:
	//     nTextRow   - [in] Row identifier.
	//     nActualCol - [in] Absolute column position.
	// Returns:
	//     The calculated display column.
	//-----------------------------------------------------------------------
	int CalcDispCol(int nTextRow, int nActualCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a pointer to the line marks manager associated
	//     with the current text buffer.
	// Returns:
	//     A pointer to the line marks manager associated
	//     with the current text buffer.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLineMarksManager* GetLineMarksManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text value for a specified row.
	// Parameters:
	//     nRow     - [in] Row identifier.
	//     pcszText - [in] New row text.
	//     bCanUndo - [in] TRUE if this can be undone, FALSE otherwise.
	// Remarks:
	//     The parser is notified to reparse the changed text.
	//-----------------------------------------------------------------------
	void SetRowText(int nRow, LPCTSTR pcszText, BOOL bCanUndo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a new row.
	// Parameters:
	//     nRow     - [in] Row identifier to insert at.
	//     pcszText - [in] New row text.
	//     bCanUndo - [in] TRUE if this can be undone, FALSE otherwise.
	// Remarks:
	//     The parser is notified to reparse the changed text.
	//-----------------------------------------------------------------------
	void InsertRow(int nRow, LPCTSTR pcszText, BOOL bCanUndo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified row.
	// Parameters:
	//     nRow     - [in] Row identifier.
	//     bCanUndo - [in] TRUE if this can be undone, otherwise FALSE.
	// Remarks:
	//     The parser is notified to reparse the changed text.
	//-----------------------------------------------------------------------
	void RemoveRow(int nRow, BOOL bCanUndo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text colors of a specified row.
	// Parameters:
	//     nTextRow   - [in] Row identifier.
	//     nColFrom   - [in] Zero-based start column number.
	//     nColTo     - [in] Zero-based end column number.
	//                       A value of -1 indicates the line end.
	//     clrDefault - [in] Constant reference to an XTP_EDIT_COLORVALUES structure
	//                       containing the default text colors. By default,
	//                       CXTPSyntaxEditPaintManager::m_clrValues is passed.
	//     rcBlocks   - [out] Pointer to a CXTPSyntaxEditTextBlockList object
	//                        to receive the text colors.
	// Remarks:
	//     The default implementation calls CXTPSyntaxEditLexTextSchema::GetRowColors.
	//     You may override this method and create your own fast implementation
	//     for the simple colors schemas.
	//-----------------------------------------------------------------------
	virtual void GetRowColors(int nTextRow, int nColFrom, int nColTo,
							  const XTP_EDIT_COLORVALUES& clrDefault,
							  CXTPSyntaxEditTextBlockList* rBlocks);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collapsable text blocks of a specified row.
	// Parameters:
	//     nTextRow  - [in] Row identifier.
	//     nArBlocks - [out] Reference to a CXTPSyntaxEditRowsBlockArray object
	//                       to receive the collapsable text blocks.
	// Remarks:
	//     The default implementation calls CXTPSyntaxEditLexTextSchema::GetCollapsableBlocksInfo.
	//     You may override this method and create your own fast implementation.
	//-----------------------------------------------------------------------
	virtual void GetCollapsableBlocksInfo(int nTextRow,
										  CXTPSyntaxEditRowsBlockArray& rArBlocks) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text of a specified line.
	// Parameters:
	//     nRow       - [in] Row identifier.
	//     bAddCRLF   - [in] Specifies how to process the end of text lines.
	//                       The default value of this parameter is FALSE.
	//     iCRLFStyle - [in] Style of the end of text lines.
	//                       The default value of this parameter is -1.
	//     strBuffer  - [out] Reference to the text buffer to receive
	//                        the text of the specified line.
	// Returns:
	//     The text of the specified line.
	//-----------------------------------------------------------------------
	CString GetLineText(int nRow, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1) const;
	void GetLineText(
		int nRow, CString& strBuffer, BOOL bAddCRLF = FALSE,
		int iCRLFStyle = -1) const; // <combine CXTPSyntaxEditCtrl::GetLineText@int@BOOL@int@const>

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Expands the character set by putting spaces in the position of tabs.
	// Parameters:
	//     pDC      - [in] Pointer to a CDC object.
	//     pszChars - [in] Text to be processed.
	//     nCurPos  - [in] Current position in the line.
	//     strBufer - [in] Reference to the buffer for text
	//                     to be created after expansion.
	// Returns:
	//     The length of the processed text after expansion.
	//-----------------------------------------------------------------------
	int ExpandChars(CDC* pDC, LPCTSTR pszChars, int nCurPos, CString& strBufer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the collapsed text sign rectangle (like for [..]).
	// Parameters:
	//     pDC        - [in] Pointer to a CDC object.
	//     pCoDrawBlk - [in] Pointer to an XTP_EDIT_COLLAPSEDBLOCK structure.
	//     rcCoBlk    - [in, out] Reference to a rectangle with
	//                            a collapsed block text area.
	//                            The right border is updated by this function.
	// See Also:
	//     XTP_EDIT_COLLAPSEDBLOCK
	//-----------------------------------------------------------------------
	void ProcessCollapsedText(CDC* pDC, XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk, CRect& rcCoBlk);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs the same functionality as ProcessCollapsedText,
	//     but has additional checks for input parameters.
	// Parameters:
	//     pDC        - [in] Pointer to a CDC object.
	//     pCoDrawBlk - [in] Pointer to an XTP_EDIT_COLLAPSEDBLOCK structure.
	//     txtBlk     - [in] Pointer to an XTP_EDIT_TEXTBLOCK structure.
	//     rcCoBlk    - [in, out] Reference to a rectangle with
	//                            a collapsed block text area.
	//                            The right border is updated by this function.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_COLLAPSEDBLOCK, XTP_EDIT_TEXTBLOCK
	//-----------------------------------------------------------------------
	BOOL ProcessCollapsedTextEx(CDC* pDC, XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk,
								const XTP_EDIT_TEXTBLOCK& txtBlk, CRect& rcCoBlk);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collapsed text of a specified collapsed block.
	// Parameters:
	//     pCoDrawBlk     - [in] Pointer to an XTP_EDIT_COLLAPSEDBLOCK structure.
	//     nMaxLinesCount - [in] Maximum lines of text.
	//                           The default value of this parameter is 150.
	// Returns:
	//     The collapsed text of the specified collapsed block.
	// See Also:
	//     XTP_EDIT_COLLAPSEDBLOCK
	//-----------------------------------------------------------------------
	CString GetCollapsedText(XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk, int nMaxLinesCount = 150) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collapsed block, if any, at a specified point.
	// Parameters:
	//     ptMouse - [in] Reference to the mouse coordinates
	//                    to check for a collapsed block.
	// Returns:
	//     A pointer to the XTP_EDIT_COLLAPSEDBLOCK structure, if any,
	//     at the specified point, otherwise NULL.
	// See Also:
	//     XTP_EDIT_COLLAPSEDBLOCK
	//-----------------------------------------------------------------------
	XTP_EDIT_COLLAPSEDBLOCK* GetBlockFromPt(const CPoint& ptMouse) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows a tooltip with the collapsed text of the collapsed block
	//     at a specified point.
	// Parameters:
	//     ptMouse - [in] Reference to the mouse coordinates
	//                    to check for a collapsed block.
	// See Also:
	//     CXTPSyntaxEditToolTipCtrl, m_wndToolTip
	//-----------------------------------------------------------------------
	void ShowCollapsedToolTip(const CPoint& ptMouse);

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands a collapsed block if it is double clicked.
	// Parameters:
	//     ptMouse - [in] Reference to the mouse coordinates
	//                    to check for a collapsed block.
	// Returns:
	//     The number of rows that the collapsed block was expanded,
	//     otherwise 0 if the collapsed block was not expanded.
	// See Also:
	//     CXTPSyntaxEditToolTipCtrl
	//-----------------------------------------------------------------------
	int ProcessCollapsedBlockDblClick(const CPoint& ptMouse);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates sibling views while in split mode.
	// Parameters:
	//     dwUpdate - [in] View update flags.
	//-----------------------------------------------------------------------
	void UpdateScrollPos(DWORD dwUpdate = XTP_EDIT_UPDATE_ALL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the modification flag for the currently opened document.
	// Parameters:
	//     bModified - [in] TRUE to set the modification flag to modified,
	//                      FALSE otherwise.
	//                      The default value of this parameter is TRUE.
	//-----------------------------------------------------------------------
	void SetDocModified(BOOL bModified = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the maximum width of specified text.
	// Parameters:
	//     szText - [in] Text for calculation.
	// Returns:
	//     The maximum width of the specified text.
	//-----------------------------------------------------------------------
	int CalcMaximumWidth(LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default configuration file path.
	// Returns:
	//     The default configuration file path.
	//-----------------------------------------------------------------------
	CString GetDefaultCfgFilePath();

	//-----------------------------------------------------------------------
	// Summary:
	//     Internal editing event handler.
	// Parameters:
	//     nRow - [in] Row identifier.
	//     nCol - [in] Column identifier.
	//-----------------------------------------------------------------------
	void OnBeforeEditChanged(int nRow, int nCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Internal editing event handler.
	// Parameters:
	//     nRowFrom    - [in] Start row identifier.
	//     nColFrom    - [in] Start column identifier.
	//     nRowTo      - [in] End row identifier.
	//     nColTo      - [in] End column identifier.
	//     eEditAction - [in] Action identifier.
	//-----------------------------------------------------------------------
	void OnEditChanged(int nRowFrom, int nColFrom, int nRowTo, int nColTo, int eEditAction);

	//-----------------------------------------------------------------------
	// Summary:
	//     Internal editing event handler.
	// Parameters:
	//     LCFrom      - [in] Start coordinates.
	//     LCTo        - [in] End coordinates.
	//     eEditAction - [in] Action identifier.
	// See Also:
	//     XTP_EDIT_LINECOL
	//-----------------------------------------------------------------------
	void OnEditChanged(const XTP_EDIT_LINECOL& LCFrom, const XTP_EDIT_LINECOL& LCTo,
					   int eEditAction);

	//-----------------------------------------------------------------------
	// Summary:
	//     Internal editing event handler.
	// Parameters:
	//     code   - [in] Event code.
	//     wParam - [in] Event parameter 1.
	//     lParam - [in] Event parameter 2.
	// See Also:
	//     XTP_NOTIFY_CODE
	//-----------------------------------------------------------------------
	void OnParseEvent(XTP_NOTIFY_CODE code, WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a WM_NOTIFY message with an XTP_EDIT_NM_PARSEEVENT code to the parent window.
	// Parameters:
	//     code   - [in] Event code from the XTPSyntaxEditOnParseEvent enumeration.
	//     wParam - [in] Event parameter 1.
	//     lParam - [in] Event parameter 2.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// See Also:
	//     XTPSyntaxEditOnParseEvent
	//-----------------------------------------------------------------------
	BOOL NotifyParseEvent(XTP_NOTIFY_CODE code, WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Internal editing event handler.
	// Parameters:
	//     code   - [in] Event code from the XTPSyntaxEditOnCfgChangedEvent enumeration.
	//     wParam - [in] Event parameter 1.
	//     lParam - [in] Event parameter 2.
	// See Also:
	//     XTPSyntaxEditOnCfgChangedEvent
	//-----------------------------------------------------------------------
	void OnLexCfgWasChanged(XTP_NOTIFY_CODE code, WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current document row.
	// Returns:
	//     The identifier of the current document row.
	// See Also:
	//     SetCurrentDocumentRow
	//-----------------------------------------------------------------------
	int GetCurrentDocumentRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current document row.
	// Parameters:
	//     nRow - [in] Row identifier.
	// See Also:
	//     GetCurrentDocumentRow
	//-----------------------------------------------------------------------
	void SetCurrentDocumentRow(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current visible row.
	// Returns:
	//     The identifier of the current visible row.
	//-----------------------------------------------------------------------
	int GetCurrentVisibleRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates document row basing in visible row.
	// Parameters:
	//     nVisibleRow - [in] Visible row.
	// Returns:
	//     Integer value of document row.
	// See Also:
	//     GetCurrentVisibleRow
	//-----------------------------------------------------------------------
	int GetDocumentRow(int nVisibleRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates document row basing in global visible row.
	// Parameters:
	//     nStartDocumentRow - [in] Start document row.
	//     nRowDelta         - [in] Visible rows count.
	// Returns:
	//     Integer value of document row.
	//-----------------------------------------------------------------------
	int CalculateDocumentRow(int nStartDocumentRow, int nRowDelta);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates visible row basing in document row.
	// Parameters:
	//     nDocumentRow - [in] Document row.
	// Returns:
	//     Integer value of visible row.
	//-----------------------------------------------------------------------
	int GetVisibleRow(int nDocumentRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates global visible row basing in document row.
	// Parameters:
	//     nStartDocumentRow - [in] Start row.
	//     nDocumentRow      - [in] Count of document rows.
	// Returns:
	//     Integer value of global visible row. Returns 1 if value less than 1.
	// See Also:
	//     CalculateVisibleRow_
	//-----------------------------------------------------------------------
	int CalculateVisibleRow(int nStartDocumentRow, int nDocumentRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates global visible row basing in document row.
	// Parameters:
	//     nStartDocumentRow - [in] Start row.
	//     nDocumentRow      - [in] Count of document rows.
	// Returns:
	//     Integer value of global visible row. Can return values less than 1 if
	//     nStartDocumentRow bigger than nDocumentRow.
	// See Also:
	//     CalculateVisibleRow
	//-----------------------------------------------------------------------
	int CalculateVisibleRow_(int nStartDocumentRow, int nDocumentRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum number of visible rows.
	// Parameters:
	//     nMaxDocRow - [in] Maximum document row up to which to calculate
	//                       the visible rows. If a value of -1 is passed,
	//                       then the visible rows will be calculated until
	//                       the end of the document.
	//                       The default value of this parameter is -1.
	// Returns:
	//     The maximum number of visible rows.
	//-----------------------------------------------------------------------
	int GetVisibleRowsCount(int nMaxDocRow = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the current row up a specified number of rows.
	// Parameters:
	//     nCount - [in] Number of rows.
	// Returns:
	//     The identifier of the current visible row.
	//-----------------------------------------------------------------------
	int MoveCurrentVisibleRowUp(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the current row down a specified number of rows.
	// Parameters:
	//     nCount - [in] Number of rows.
	// Returns:
	//     The identifier of the current visible row.
	//-----------------------------------------------------------------------
	int MoveCurrentVisibleRowDown(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shifts the current row up a specified number of rows.
	// Parameters:
	//     nCount       - [in] Number of rows.
	//     bChangeCaret - [in] Caret movement flag.
	//                         The default value of this parameter is FALSE.
	// Returns:
	//     TRUE if the screen was changed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ShiftCurrentVisibleRowUp(int nCount, BOOL bChangeCaret = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shifts the current row down a specified number of rows.
	// Parameters:
	//     nCount       - [in] Number of rows.
	//     bChangeCaret - [in] Caret movement flag.
	//                         The default value of this parameter is FALSE.
	// Returns:
	//     TRUE if the screen was changed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ShiftCurrentVisibleRowDown(int nCount, BOOL bChangeCaret = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length of a collapsed block.
	// Parameters:
	//     nStartRow - [in] Start row of the collapsed block.
	//     rnLen     - [out] Reference to an int to receive
	//                       the length of the collapsed block.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetCollapsedBlockLen(int nStartRow, int& rnLen) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row node type.
	// Parameters:
	//     nRow   - [in] Row identifier.
	//     dwType - [out] Reference to a DWORD to receive
	//                    the row node type.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetRowNodes(int nRow, DWORD& dwType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads data into an auto-complete list.
	//-----------------------------------------------------------------------
	virtual void SetAutoCompleteList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes line marks depending on the change type.
	// Parameters:
	//     pEditChanged - [in] Pointer to an XTP_EDIT_NMHDR_EDITCHANGED structure.
	// See Also:
	//     LPXTP_EDIT_NMHDR_EDITCHANGED
	//-----------------------------------------------------------------------
	virtual void RefreshLineMarks(XTP_EDIT_NMHDR_EDITCHANGED* pEditChanged);

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws line marks.
	//-----------------------------------------------------------------------
	virtual void RedrawLineMarks();

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs undo/redo operations and common control properties adjustment.
	// Parameters:
	//     nActionsCount - [in] Number of actions to undo/redo.
	//     bUndoRedo     - [in] TRUE to perform undo operations,
	//                          FALSE to perform redo operations.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DoUndoRedo(int nActionsCount, BOOL bUndoRedo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the case of the selection.
	// Parameters:
	//     bUpper - [in] TRUE to change the selection case to upper,
	//                   FALSE to change the selection case to lower.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DoChangeSelectionCase(BOOL bUpper);

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the tabification of the selection.
	// Parameters:
	//     bTabify - [in] TRUE to change spaces to tabs,
	//                    FALSE to change tabs to spaces.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DoChangeSelectionTabify(BOOL bTabify);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads options from the registry.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetRegValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     Internally updates drawing information
	//     (e.g. each character position) in the draw text processor.
	// Parameters:
	//     nTextRow - Row identifier.
	// Remarks:
	//     This method draws the specified row on the memory device context.
	//-----------------------------------------------------------------------
	void UpdateRowInfoInternally(int nTextRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the scroll direction for automatic scrolling
	//     (when selecting by mouse).
	// Parameters:
	//     pnCols - [out] Pointer to an int to receive
	//                    the columns count to scroll. May be NULL.
	//     pnRows - [out] Pointer to an int to receive
	//                    the rows count to scroll. May be NULL.
	// Returns:
	//     Flags from the CXTPSyntaxEditCtrl::XTPScrolDirection enumeration.
	//-----------------------------------------------------------------------
	virtual DWORD GetAutoscrollDirection(int* pnCols = NULL, int* pnRows = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Subtracts the selection size from a specified position.
	// Parameters:
	//     lcDispPos - Position.
	// Returns:
	//     The subtracted position.
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL _SubtractSelSizeFromPos(XTP_EDIT_LINECOL lcDispPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets variables for auto-indent.
	// Parameters:
	//     nBaseDocRow   - [in] Text row to calculate auto-indent start.
	//     nDispCol_prev - [in] Previous display column (before enter pressed).
	//-----------------------------------------------------------------------
	virtual void DoAutoIndentIfNeed(int nBaseDocRow, int nDispCol_prev);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the parent window that the scroll bar state has changed.
	// Parameters:
	//     dwScrollBar - [in] Defines the scroll bar(s) to change state as
	//                        WS_HSCROLL and WS_VSCROLL flags.
	//     dwState     - [in] Defines the scroll bar(s) state as
	//                        WS_HSCROLL and WS_VSCROLL flags.
	// Remarks:
	//     The default implementation sends an
	//     XTP_EDIT_NM_ENABLESCROLLBAR notification to the parent.
	//-----------------------------------------------------------------------
	virtual void _EnableScrollBarNotify(DWORD dwScrollBar, DWORD dwState);

	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

private:
	void RedrawScrollBar(int nBar);
	void InvalidateAll();

protected:
	CXTPSyntaxEditOptions* m_pOptions;
	/////////////////////////////////////////////////////////////////////////
	// Variables
	/////////////////////////////////////////////////////////////////////////
	BOOL m_bVertScrollBarEnabled;
	BOOL m_bHorzScrollBarEnabled;

	CXTPSyntaxEditRowColorMap m_mapInternalRowBkColor;   // Map for row back colors.
	CXTPSyntaxEditRowColorMap m_mapInternalRowForeColor; // Map for row colors.

	CXTPNotifySink* m_pSink;

	BOOL m_bVertScrollBar;	 // TRUE if vertical scrollbar is enabled.
	BOOL m_bHorzScrollBar;	 // TRUE if horizontal scrollbar is enabled.
	BOOL m_bScrolling;		   // TRUE if scrolling by mouse wheel.
	BOOL m_bCaseSensitive;	 // TRUE if case sensitive.
	BOOL m_bEnableOleDrag;	 // TRUE if OLE drag drop is enabled.
	BOOL m_bIsSmartIndent;	 // TRUE if smart indentation is enabled.
	BOOL m_bRightButtonDrag;   // TRUE if the user is dragging through right button.
	BOOL m_bIsScrollingEndRow; // Maintains an internal logic to draw the last row which is partly
							   // visible.
	BOOL m_bTokensLoaded;	  // TRUE if if tokens are loaded.
	BOOL m_bDroppable;		   // TRUE if the current position is droppable.
	BOOL m_bDragging;		   // TRUE if in dragging mode.
	BOOL m_bIsDragging;		   // TRUE if dragging, FALSE otherwise.

	BOOL m_bPageDirty;				 // TRUE if the whole page is dirty.
	BOOL m_bIsActive;				 // TRUE if the application is active.
	BOOL m_bFocused;				 // TRUE if focused.
	BOOL m_bCreateScrollbarOnParent; // TRUE if parent window scroll bars are used, otherwise FALSE.
	BOOL m_bAllowExpandCollapse;
	BOOL m_bActivateOnFocus; // Flag to activate control on focus set (like CXTPSyntaxEditView do).

	int m_nTopCalculatedRow;	// Top row of the last range of rows that have scrollbars properly
								// calculated.
	int m_nBottomCalculatedRow; // Bottom row of the last range of rows that have scrollbars
								// properly calculated.
	int m_nEditbarLength;		// Edit bar length including gutter and line number.
	int m_nMarginLength;		// Margin length.
	int m_nLineNumLength;		// Line numbers length.
	int m_nNodesWidth;			// The width of the nodes signs.

	int m_nInsertTabCount;		   // Temporarily stores the number of tabs to be inserted for
								   // auto-indentation.
	int m_nInsertSpaceCount;	   // Temporarily stores the number of spaces to be inserted for
								   // auto-indentation.
	int m_nAutoIndentCol;		   // Contains the column for auto-indentation.
	int m_nTopRow;				   // Top row for display.
	int m_nCurrentDocumentRow;	 // Current row in the document.
	int m_nCurrentCol;			   // Current absolute column.
	int m_nDispCol;				   // Current display column.
	int m_nWheelScroll;			   // Lines to scroll on mouse wheel.
	int m_nAverageLineLen;		   // Average length of line.
	int m_nCollapsedTextRowsCount; // The total amount of collapsed blocks in the document.

	DWORD m_dwInsertPos;	  // Current insert position.
	DWORD m_dwLastRedrawTime; // Stores last redrawing time.

#	ifndef _UNICODE
	// DBCS Support (specially for IME)
	BYTE m_chPrevLeadByte; // For non-UNICODE version, IME sends double byte chars as 2 WM_CHAR
						   // messages. This member keeps a lead byte until second byte receive to
						   // process them together.
#	endif

	CPoint m_ptDropPos;   // Drop position.
	CPoint m_ptPrevMouse; // Mouse position of the previous tip showing.

	CBitmap m_bmpCache; // Current view cached picture.

	CXTPSyntaxEditSelection* m_pSelection; // Current selection.

	//-----------------------------------------------------------------------
	// Summary:
	//      This enum defines flags for auto-scroll direction.
	//-----------------------------------------------------------------------
	enum XTPScrolDirection
	{
		xtpLeft   = 0x0001, // Defines left direction.
		xtpRight  = 0x0010, // Defines right direction.
		xtpTop	= 0x0100, // Defines top direction.
		xtpBottom = 0x1000, // Defines bottom direction.
	};
	DWORD m_dwAutoScrollDirection; // Active auto-scroll direction.

	CXTPSyntaxEditBufferManager* m_pBuffer;		 // The buffer manager.
	CXTPSyntaxEditPaintManager* m_pPaintManeger; // The paint manager.
	CWnd* m_pParentWnd;							 // Pointer to the parent window.

	CUIntArray m_arCollapsedTextRows; // The array of rows with beginning of collapsed blocks.

	CXTPSyntaxEditToolTipCtrl* m_pToolTip;			// Tooltip window for collapsed blocks text.
	CXTPSyntaxEditAutoCompleteWnd* m_pAutoComplete; // Auto-complete pop-up window.

	CString m_strDefaultCfgFilePath; // Configuration file full name.

	CString m_strSyntaxScheme; // The syntax scheme.
	CString m_strColorScheme;  // The color scheme.

	int m_nHScrollMaxWidth;		 // Maximum horizontal scrollbar position.
	BOOL m_bWndCreateInProgress; // Internal flag to indicate that window creation is in progress.

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the screen schema cache.
	// Parameters:
	//     nForRow - Row index.
	// Returns:
	//     A pointer to the screen schema cache.
	//-----------------------------------------------------------------------
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextBlock* GetOnScreenSch(int nForRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears the screen schema cache.
	// Parameters:
	//     nRowFrom - [in] Row index to clear the cache from.
	//-----------------------------------------------------------------------
	void ClearOnScreenSchCache(int nRowFrom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the average line length between
	//     specified starting and ending rows.
	// Parameters:
	//     nRowStart - [in] Row index to calculate from.
	//     nRowEnd   - [in] Row index to calculate to.
	// Returns:
	//     The average line length between the
	//     specified starting and ending rows.
	//-----------------------------------------------------------------------
	virtual UINT CalcAveDataSize(int nRowStart, int nRowEnd);

	// -----------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered, otherwise FALSE.
	// -----------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//===========================================================================
	// Summary:
	//     CXTPSyntaxEditCtrl::CTextSearchCache
	//===========================================================================
	class CTextSearchCache;

	CTextSearchCache* m_fcCollapsable; // Collapsible rows cache.
	CTextSearchCache* m_fcRowColors;   // Row colors cache.

	class CScreenSearchBlock;
	class CScreenSearchCache;

	CScreenSearchCache* m_arOnScreenSchCache; // On-screen schema cache for this control.
	CByteArray m_arValidDispRows;			  // Array with indexes of valid displayed rows.

	//===========================================================================
	// Summary:
	//     CXTPSyntaxEditCtrl::CAverageVal
	//===========================================================================
	class CAverageVal
	{
	public:
		// -----------------------------------------------------------------
		// Summary:
		//     Constructs a CAverageVal object.
		// Parameter:
		//     nDataSize - Data size.
		// -----------------------------------------------------------------
		CAverageVal(int nDataSize = 100);

		// -----------------------------------------------------------------
		// Summary:
		//     Adds a value to the data collection.
		// Parameter:
		//     uVal - Value to be added.
		// -----------------------------------------------------------------
		void AddValue(UINT uVal);

		// -----------------------------------------------------------------
		// Summary:
		//     Gets the average value the data in this object.
		// Parameter:
		//     uDefaultIfNoData - Default value.
		// Returns:
		//     The average value of the data in this object.
		// -----------------------------------------------------------------
		UINT GetAverageValue(UINT uDefaultIfNoData = 0);

	protected:
		int m_nDataSize;	 // Data size.
		int m_nNextIndex;	// Next index.
		CUIntArray m_arData; // Data collection.
	};

	CAverageVal m_aveRedrawScreenTime; // Redraw screen time.

	//===========================================================================
	CXTPSyntaxEditDrawTextProcessor* m_pDrawTextProcessor; // Pointer to the draw text processor.

	CImmWrapper* m_pImmWrapper; // Imm wrapper.
	BOOL m_bIMEsupported;		// TRUE if IME is supported, FALSE otherwise.

	// for direct assignment (without SynaxEditControl files to read!)
	CString m_sIniSet;
	// String with default or passed type of rules,
	// e.g. _T("[Schemes]\r\nCPP\r\n[Themes]\r\nDefault\r\nAlternative\r\n")

	BOOL m_bConfigFileMode;
	// TRUE if control read Config file with links to detailed setting (Syntax and Color schemes)
	// FALSE if Syntax and Color schemes passed as strings using special functions

	CString m_sPassedIniSet;
	// String with passed type of rules,
	// e.g. _T("[Schemes]\r\nVBS\r\n[Themes]\r\nDefault\r\nAlternative\r\n")

public:
	BOOL m_bDisableRedraw;
	// If TRUE - control window is not redraw, it is draw last window content (draw cached bitmap).
	// FALSE by default.

	BOOL m_bUseMonitor;
	// TRUE if control use special thread to synchronize syntax rules with files with Syntax and
	// Color schemes FALSE if control works "off-line"

	//------------------------------------------------------------------
	// Summary:
	//     Gets the config file mode.
	// Returns:
	//     A boolean value that specifies the current config file mode.
	//------------------------------------------------------------------
	BOOL IsConfigFileMode()
	{
		return m_bConfigFileMode;
	}

	//------------------------------------------------------------------
	// Summary:
	//     Gets the syntax scheme.
	// Returns:
	//     A string containing the syntax scheme.
	//------------------------------------------------------------------
	CString GetSyntaxScheme()
	{
		return m_strSyntaxScheme;
	}

	//------------------------------------------------------------------
	// Summary:
	//     Gets the color scheme.
	// Returns:
	//     A string containing the color scheme.
	//------------------------------------------------------------------
	CString GetColorScheme()
	{
		return m_strColorScheme;
	}

	CString m_sCustomTitle;
	// User-defined app title. Can also be used in Print Job as a unique identifier.

	//------------------------------------------------------------------
	// Summary:
	//     Sets the passed ini set.
	// Parameters:
	//     sPassedIniSet - Passed ini set to be set.
	//------------------------------------------------------------------
	void SetPassedIniSet(CString sPassedIniSet)
	{
		m_sPassedIniSet = sPassedIniSet;
	}

	//------------------------------------------------------------------
	// Summary:
	//     Sets the syntax and color schemes.
	// Parameters:
	//     sSyntaxScheme - String containing the syntax scheme.
	//     sColorScheme  - String containing the color scheme.
	//     bScheme       - Additional flag to call in-place editor.
	//     bColors       - Additional flag to call in-place editor.
	//------------------------------------------------------------------
	void SetSyntaxAndColorScheme(CString sSyntaxScheme, CString sColorScheme, BOOL bScheme = FALSE,
								 BOOL bColors = FALSE);

	//------------------------------------------------------------------
	// Summary:
	//     Reloads the syntax string mode.
	//------------------------------------------------------------------
	void ReloadSyntaxStringMode();

	//------------------------------------------------------------------
	// Summary:
	//     Reloads the syntax config file mode.
	//------------------------------------------------------------------
	void ReloadSyntaxConfigFileMode();
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPSyntaxEditCtrl::InvalidateRow(int nDispRow)
{
	InvalidateRows(nDispRow, nDispRow);
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetAutoIndent() const
{
	return m_pOptions->m_bAutoIndent;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetSyntaxColor() const
{
	return m_pOptions->m_bSyntaxColor;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetSelMargin() const
{
	return m_pOptions->m_bSelMargin;
}
AFX_INLINE CXTPSyntaxEditBufferManager* CXTPSyntaxEditCtrl::GetEditBuffer() const
{
	return m_pBuffer;
}
AFX_INLINE CXTPSyntaxEditPaintManager* CXTPSyntaxEditCtrl::GetPaintManager() const
{
	return m_pPaintManeger;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetHorzScrollBar() const
{
	return m_bHorzScrollBar;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetVertScrollBar() const
{
	return m_bVertScrollBar;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsActive() const
{
	return m_bIsActive;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsCreateScrollbarOnParent() const
{
	return m_bCreateScrollbarOnParent;
}
AFX_INLINE void CXTPSyntaxEditCtrl::SetCreateScrollbarOnParent(BOOL bSet)
{
	m_bCreateScrollbarOnParent = bSet;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsEnabledVirtualSpace() const
{
	return m_pOptions->m_bVirtualSpace;
}
AFX_INLINE CXTPSyntaxEditAutoCompleteWnd* CXTPSyntaxEditCtrl::GetAutoCompleteWnd() const
{
	return m_pAutoComplete;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsReadOnly() const
{
	return m_pOptions->m_bReadOnly;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetLineNumbers() const
{
	return m_pOptions->m_bLineNumbers;
}
AFX_INLINE void CXTPSyntaxEditCtrl::SetReadOnly(BOOL bReadOnly)
{
	m_pOptions->m_bReadOnly = bReadOnly;
	InvalidateAll();
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsViewOnly() const
{
	return m_pOptions->m_bViewOnly;
}
AFX_INLINE void CXTPSyntaxEditCtrl::SetViewOnly(BOOL bViewOnly)
{
	m_pOptions->m_bViewOnly  = bViewOnly;
	m_pOptions->m_bReadOnly  = bViewOnly;
	m_pOptions->m_bHideCaret = bViewOnly;

	InvalidateAll();
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::HasFocus() const
{
	return m_bFocused;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsHideCaret() const
{
	return m_pOptions->m_bHideCaret;
}
AFX_INLINE void CXTPSyntaxEditCtrl::SetHideCaret(BOOL bHide)
{
	m_pOptions->m_bHideCaret = bHide;
	if (m_hWnd)
		Invalidate(FALSE);
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetCollapsibleNodes() const
{
	return m_pOptions->m_bDrawNodes;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsEnabledEditAccelerators() const
{
	return m_pOptions->m_bEnableEditAccelerators;
}
AFX_INLINE void CXTPSyntaxEditCtrl::EnableEditAccelerators(BOOL bEnable)
{
	m_pOptions->m_bEnableEditAccelerators = bEnable;
}

AFX_INLINE BOOL CXTPSyntaxEditCtrl::_IsVirtualSpaceActive() const
{
	BOOL bVirtualSpace = IsEnabledVirtualSpace();
	return bVirtualSpace;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsEnabledWhiteSpace() const
{
	return m_pOptions->m_bEnableWhiteSpace;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::GetTabWithSpace() const
{
	return m_pOptions->m_bTabWithSpace;
}
AFX_INLINE int CXTPSyntaxEditCtrl::GetTopRow() const
{
	return m_nTopRow;
}
AFX_INLINE BOOL CXTPSyntaxEditCtrl::IsRightButtonDrag() const
{
	return m_bRightButtonDrag;
}
AFX_INLINE void CXTPSyntaxEditCtrl::CancelRightButtonDrag()
{
	m_bRightButtonDrag = FALSE;
}
AFX_INLINE void CXTPSyntaxEditCtrl::SetRightButtonDrag(BOOL bRightButtonDrag)
{
	m_bRightButtonDrag = bRightButtonDrag;
}
AFX_INLINE int CXTPSyntaxEditCtrl::GetCurCol() const
{
	return m_nDispCol;
}
AFX_INLINE int CXTPSyntaxEditCtrl::GetCurAbsCol() const
{
	return m_nCurrentCol;
}

/////////////////////////////////////////////////////////////////////////////

// CXTPSyntaxEditStringDlg dialog

class _XTP_EXT_CLASS CXTPSyntaxEditStringDlg : public CDialog
{
	DECLARE_DYNAMIC(CXTPSyntaxEditStringDlg)

public:
	CXTPSyntaxEditStringDlg(CWnd* pParent = NULL); // Standard constructor.
	virtual ~CXTPSyntaxEditStringDlg();
	CString m_Syntax;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:
	virtual INT_PTR DoModal();
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnEditSelectAll();
	CEdit m_edit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITSYNTAXEDITCTRL_H__)
