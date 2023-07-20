// XTPSyntaxEditPaintManager.h interface for the CXTPSyntaxEditPaintManager class.
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
#if !defined(__XTPSYNTAXEDITPAINTMANAGER_H__)
#	define __XTPSYNTAXEDITPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditCtrl;
class CXTPImageManager;

//{{AFX_CODEJOCK_PRIVATE
#	define XTP_DECLARE_COLOR_PROP(propName)                                                       \
		OLE_COLOR OleGet##propName();                                                              \
		void OleSet##propName(OLE_COLOR oleColor);
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class is used by CXTPSyntaxEditCtrl as the paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditPaintManager : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditPaintManager)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditPaintManager object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initializes default paint manager bitmaps, should be called after
	//     creating the control window.
	//-----------------------------------------------------------------------
	virtual void InitBitmaps();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the edit control text font.
	// Parameters:
	//     pEditCtrl - [in] Pointer to the edit control.
	//     lf        - [in] Reference to an XTP_EDIT_FONTOPTIONS structure.
	// Returns:
	//     TRUE if the edit control text font was updated, otherwise FALSE.
	// See Also:
	//     LOGFONT
	//-----------------------------------------------------------------------
	virtual BOOL UpdateTextFont(CXTPSyntaxEditCtrl* pEditCtrl, const XTP_EDIT_FONTOPTIONS& lf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a line number.
	// Parameters:
	//     pDC       - [in] Pointer to the device context to draw to.
	//     rcRect    - [in] Reference to the rectangle to draw.
	//     nTextRow  - [in] Text row number for which to draw.
	//     pEditCtrl - [in] Pointer to the edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineNumber(CDC* pDC, const CRect& rcRect, int nTextRow,
								CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the line collapsible node icon.
	// Parameters:
	//     pDC        - [in] Pointer to the device context to draw to.
	//     rcNode     - [in] Reference to the node rectangle to draw.
	//     rcNodeFull - [in] Reference to the full node area rectangle.
	//     dwType     - [in] Line node type flags.
	//     nTextRow   - [in] Text row number for which to draw.
	//     pEditCtrl  - [in] Pointer to the edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineNode(CDC* pDC, const CRect& rcNode, const CRect& rcNodeFull, DWORD dwType,
							  int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the line node background.
	// Parameters:
	//     pDC        - [in] Pointer to the device context to draw to.
	//     rcNodeFull - [in] Reference to the full node area rectangle.
	//     dwType     - [in] Line node type flags.
	//     nTextRow   - [in] Text row number for which to draw.
	//     pEditCtrl  - [in] Pointer to the edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineNodeBackground(CDC* pDC, const CRect& rcNodeFull, DWORD dwType,
										int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws frames around collapsed text blocks.
	// Parameters:
	//     pEditCtrl - [in] Pointer to the edit control.
	//     pDC       - [in] Pointer to the device context.
	//-----------------------------------------------------------------------
	virtual void DrawCollapsedTextMarks(CXTPSyntaxEditCtrl* pEditCtrl, CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws line marks.
	// Parameters:
	//     pDC       - [in] Pointer to the device context to draw to.
	//     rcRect    - [in] Reference to the rectangle to draw.
	//     nTextRow  - [in] Text row number for which to draw.
	//     pEditCtrl - [in] Pointer to the edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineMarks(CDC* pDC, const CRect& rcRect, int nTextRow,
							   CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws row text.
	// Parameters:
	//     pDC        - [in] Pointer to the device context to draw to.
	//     rcTextLine - [in] Reference to the rectangle to draw.
	//     nTextRow   - [in] Text row number for which to draw.
	//     nLine      - [in] Screen (page) line number.
	//     pEditCtrl  - [in] Pointer to the edit control.
	// Returns:
	//     The height to draw row text.
	//-----------------------------------------------------------------------
	virtual int DrawLineTextEx(CDC* pDC, const CRect& rcTextLine, int nTextRow, int nLine,
							   CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints row text.
	// Parameters:
	//     pDC        - [in] Pointer to the device context to draw to.
	//     rcTextLine - [in] Reference to the rectangle to draw.
	//     nTextRow   - [in] Text row number for which to draw.
	//     nLine      - [in] Screen (page) line number.
	//     pEditCtrl  - [in] Pointer to the edit control.
	//     nFlags     - [in] Additional printing options.
	//                       The following options are supported:
	//                       DT_CALCRECT, DT_SINGLELINE, DT_WORDBREAK
	// Returns:
	//     The height to print row text.
	//-----------------------------------------------------------------------
	virtual int PrintLineTextEx(CDC* pDC, const CRect& rcTextLine, int nTextRow, int nLine,
								CXTPSyntaxEditCtrl* pEditCtrl, int nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the editor text line and calculates the selection position.
	// Parameters:
	//     txtBlk     - [in] Reference to an XTP_EDIT_TEXTBLOCK structure.
	//     nSelStartX - [in] Start coordinate.
	//     nSelEndX   - [in] End coordinate.
	//     arTxtBlk   - [in] Array of references to XTP_EDIT_TEXTBLOCK structures.
	//     bUseBlk    - [in] Array of references to BOOL values.
	// See Also:
	//     XTP_EDIT_TEXTBLOCK
	//-----------------------------------------------------------------------
	virtual void DrawLineCalcSel(const XTP_EDIT_TEXTBLOCK& txtBlk, int nSelStartX, int nSelEndX,
								 XTP_EDIT_TEXTBLOCK (&arTxtBlk)[4], BOOL (&bUseBlk)[4]);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a line mark.
	// Parameters:
	//     pEditCtrl - [in] Pointer to the edit control.
	//     pBookmark - [in] Pointer to the structure containing mark parameters.
	// See Also:
	//     LPXTP_EDIT_SENMBOOKMARK
	//-----------------------------------------------------------------------
	virtual void DrawLineMark(CXTPSyntaxEditCtrl* pEditCtrl, XTP_EDIT_SENMBOOKMARK* pBookmark);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the line numbers border.
	// Parameters:
	//     pDC       - [in] Pointer to the device context to draw to.
	//     rcLineNum - [in] Reference to the line number rectangle.
	//     clrBorder - [in] Border color.
	//-----------------------------------------------------------------------
	virtual void DrawLineNumbersBorder(CDC* pDC, const CRect& rcLineNum, const COLORREF clrBorder);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the line selection cursor.
	// Parameters:
	//     hCurLine - [in] Handle to the cursor. The cursor must have been
	//                     created by the CreateCursor function or loaded
	//                     by the LoadCursor or LoadImage function. Cursor
	//                     appears in lines selection area.
	// Returns:
	//     The handle of the previous cursor.
	//-----------------------------------------------------------------------
	HCURSOR SetLineSelCursor(HCURSOR hCurLine);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the cursor shape for "copy" cursor.
	// Parameters:
	//     hCurCopy - [in] Handle to the cursor. The cursor must have been
	//                     created by the CreateCursor function or loaded
	//                     by the LoadCursor or LoadImage function.
	// Returns:
	//     The handle of the previous cursor.
	//-----------------------------------------------------------------------
	HCURSOR SetCopyCursor(HCURSOR hCurCopy);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the cursor shape for "move" cursor.
	// Parameters:
	//     hCurMove - [in] Handle to the cursor. The cursor must have been
	//                     created by the CreateCursor function or loaded
	//                     by the LoadCursor or LoadImage function.
	// Returns:
	//     The handle of the previous cursor.
	//-----------------------------------------------------------------------
	HCURSOR SetMoveCursor(HCURSOR hCurMove);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text color.
	// Returns:
	//     The text color.
	// See Also:
	//     SetTextColor
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the background color.
	// Returns:
	//     The background color.
	// See Also:
	//     SetBackColor
	//-----------------------------------------------------------------------
	COLORREF GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the corresponding background color for either
	//     the edit or read-only mode of a specified CXTPSyntaxEditCtrl object.
	// Parameters:
	//     pEditCtrl - Pointer to a CXTPSyntaxEditCtrl object.
	// Returns:
	//     The corresponding background color.
	// See Also:
	//     SetBackColor, GetBackColor
	//-----------------------------------------------------------------------
	COLORREF GetBackColorEx(CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected text color.
	// Returns:
	//     The selected text color.
	// See Also:
	//     SetHiliteTextColor
	//-----------------------------------------------------------------------
	COLORREF GetHiliteTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected background color.
	// Returns:
	//     The selected background color.
	// See Also:
	//     SetHiliteBackColor
	//-----------------------------------------------------------------------
	COLORREF GetHiliteBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected text color for when the control does not have focus.
	// Returns:
	//     The selected text color for when the control does not have focus.
	// See Also:
	//     SetInactiveHiliteTextColor
	//-----------------------------------------------------------------------
	COLORREF GetInactiveHiliteTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected background color for when the control does not have focus.
	// Returns:
	//     The selected background color for when the control does not have focus.
	// See Also:
	//     SetInactiveHiliteBackColor
	//-----------------------------------------------------------------------
	COLORREF GetInactiveHiliteBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the line numbers text color.
	// Returns:
	//     The line numbers text color.
	// See Also:
	//     SetLineNumberTextColor
	//-----------------------------------------------------------------------
	COLORREF GetLineNumberTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the line numbers background color.
	// Returns:
	//     The line numbers background color.
	// See Also:
	//     SetLineNumberBackColor
	//-----------------------------------------------------------------------
	COLORREF GetLineNumberBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the breakpoint text color.
	// Returns:
	//     The breakpoint text color.
	// See Also:
	//     SetBreakpointTextColor
	//-----------------------------------------------------------------------
	COLORREF GetBreakpointTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the breakpoint background color.
	// Returns:
	//     The breakpoint background color.
	// See Also:
	//     SetBreakpointBackColor
	//-----------------------------------------------------------------------
	COLORREF GetBreakpointBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text color
	// Parameters:
	//     crText     - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetTextColor
	//-----------------------------------------------------------------------
	BOOL SetTextColor(COLORREF crText, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the background color.
	// Parameters:
	//     crBack     - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetBackColor
	//-----------------------------------------------------------------------
	BOOL SetBackColor(COLORREF crBack, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the background color for read-only mode.
	// Parameters:
	//     crBack     - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetBackColor
	//-----------------------------------------------------------------------
	BOOL SetReadOnlyBackColor(COLORREF crBack, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected text color.
	// Parameters:
	//     color      - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetHiliteTextColor
	//-----------------------------------------------------------------------
	BOOL SetHiliteTextColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected background color.
	// Parameters:
	//     color      - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetHiliteBackColor
	//-----------------------------------------------------------------------
	BOOL SetHiliteBackColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected text color for when the control does not have focus.
	// Parameters:
	//     color      - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetInactiveHiliteTextColor
	//-----------------------------------------------------------------------
	BOOL SetInactiveHiliteTextColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected background color for when the control does not have focus.
	// Parameters:
	//     color      - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetInactiveHiliteBackColor
	//-----------------------------------------------------------------------
	BOOL SetInactiveHiliteBackColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the line numbers text color.
	// Parameters:
	//     color      - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetLineNumberTextColor
	//-----------------------------------------------------------------------
	BOOL SetLineNumberTextColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the line numbers background color.
	// Parameters:
	//     color      - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetLineNumberBackColor
	//-----------------------------------------------------------------------
	BOOL SetLineNumberBackColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the breakpoint text color
	// Parameters:
	//     crText     - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetBreakpointTextColor
	//-----------------------------------------------------------------------
	BOOL SetBreakpointTextColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the breakpoint background color.
	// Parameters:
	//     crBack     - Color to be set.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetBreakpointBackColor
	//-----------------------------------------------------------------------
	BOOL SetBreakpointBackColor(COLORREF color, BOOL bUpdateReg = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the common font.
	// Returns:
	//     A pointer to a CFont object.
	// See Also:
	//     SetFont
	//-----------------------------------------------------------------------
	CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the editor text font.
	// Returns:
	//     A pointer to a CFont object.
	// See Also:
	//     SetFontText
	//-----------------------------------------------------------------------
	CFont* GetFontText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the line numbers font.
	// Returns:
	//     A pointer to a CFont object.
	// See Also:
	//     SetFontLineNumber
	//-----------------------------------------------------------------------
	CFont* GetFontLineNumber();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip font.
	// Returns:
	//     A pointer to a CFont object.
	// See Also:
	//     SetFontToolTip
	//-----------------------------------------------------------------------
	CFont* GetFontToolTip(); // ToolTip font.

	//-----------------------------------------------------------------------
	// Summary:
	//     Set the common font.
	// Parameters:
	//     pLogfont - Pointer to a LPLOGFONT structure.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetFont
	//-----------------------------------------------------------------------
	BOOL SetFont(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the editor text font.
	// Parameters:
	//     pLogfont - Pointer to a LPLOGFONT structure.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetFontText
	//-----------------------------------------------------------------------
	BOOL SetFontText(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the line numbers font.
	// Parameters:
	//     pLogfont - Pointer to a LPLOGFONT structure.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetFontLineNumber
	//-----------------------------------------------------------------------
	BOOL SetFontLineNumber(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip font.
	// Parameters:
	//     pLogfont - Pointer to a LPLOGFONT structure.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetFontToolTip
	//-----------------------------------------------------------------------
	BOOL SetFontToolTip(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets all editor fonts.
	// Parameters:
	//     pLogfont   - Pointer to a LPLOGFONT structure.
	//     bUpdateReg - TRUE to update this setting in the registry, FALSE otherwise.
	//                  By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     SetFont, SetFontText, SetFontLineNumber, SetFontToolTip
	//-----------------------------------------------------------------------
	BOOL CreateFontIndirect(LPLOGFONT pLogfont, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the cursor to show on the line selection area.
	// Returns:
	//     The HCURSOR handle to the cursor.
	// See Also:
	//     SetLineSelCursor
	//-----------------------------------------------------------------------
	HCURSOR GetCurLine();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the cursor for moving text.
	// Returns:
	//     The HCURSOR handle to the cursor.
	// See Also:
	//     SetMoveCursor
	//-----------------------------------------------------------------------
	HCURSOR GetCurMove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the cursor for copying text.
	// Returns:
	//     The HCURSOR handle to the cursor.
	// See Also:
	//     SetCopyCursor
	//-----------------------------------------------------------------------
	HCURSOR GetCurCopy();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the standard NO cursor.
	// Returns:
	//     The HCURSOR handle to the cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetCurNO();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the standard IBEAM cursor.
	// Returns:
	//     The HCURSOR handle to the cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetCurIBeam();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the standard ARROW cursor.
	// Returns:
	//     The HCURSOR handle to the cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetCurArrow();

	CString m_sLineNumberFormat;
	// String to customize Line Number format (e.g. keep it fixed legnth like %04d)
	// If empty - Control used default - variable legnth Line Numbers)

protected:
	// fonts
	CXTPFont m_xtpFont;			  // Default font for normal text.
	CXTPFont m_xtpFontText;		  // Current text font.
	CXTPFont m_xtpFontLineNumber; // Font for line numbering.
	CXTPFont m_xtpFontToolTip;	// Font for tooltip text.

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_font, m_xtpFont, GetFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontText, m_xtpFontText, GetTextFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontLineNumber, m_xtpFontLineNumber,
										  GetLineNumberFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontToolTip, m_xtpFontToolTip,
										  GetToolTipFontHandle);

	// cursors
	HCURSOR m_hCurLine;  // Cursor to show on the line selection area.
	HCURSOR m_hCurMove;  // Cursor for moving text.
	HCURSOR m_hCurCopy;  // Cursor for copying text.
	HCURSOR m_hCurNO;	// Standard NO cursor.
	HCURSOR m_hCurIBeam; // Standard IBEAM cursor.
	HCURSOR m_hCurArrow; // Standard ARROW cursor.

	enum IconType
	{
		IconBookmark,
		IconBookmarkSelected,
		IconBreakpoint
	};

	CXTPImageManager* m_pImageManager;

	CPen m_penGray; // Standard common gray pen.

	XTP_EDIT_COLORVALUES m_clrValues; // Default color.

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bookmarks image manager.
	// Returns:
	//     A pointer to the bookmarks image manager.
	//-----------------------------------------------------------------------
	virtual CXTPImageManager* GetBookmarks();

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditPaintManager)

	XTP_DECLARE_COLOR_PROP(TextColor)
	XTP_DECLARE_COLOR_PROP(BackColor)
	XTP_DECLARE_COLOR_PROP(ReadOnlyBackColor)
	XTP_DECLARE_COLOR_PROP(LineNumberTextColor)
	XTP_DECLARE_COLOR_PROP(LineNumberBackColor)
	XTP_DECLARE_COLOR_PROP(SelectedTextColor)
	XTP_DECLARE_COLOR_PROP(SelectedBackColor)
	XTP_DECLARE_COLOR_PROP(BreakpontTextColor)
	XTP_DECLARE_COLOR_PROP(BreakpontBackColor)

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

/////////////////////////////////////////////////////////////////////////////
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetTextColor() const
{
	return m_clrValues.crText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetBackColor() const
{
	return m_clrValues.crBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetBackColorEx(CXTPSyntaxEditCtrl* pEditCtrl)
{
	return m_clrValues.GetBackColorEx(pEditCtrl);
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetHiliteTextColor() const
{
	return m_clrValues.crHiliteText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetHiliteBackColor() const
{
	return m_clrValues.crHiliteBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetInactiveHiliteTextColor() const
{
	return m_clrValues.crInactiveHiliteText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetInactiveHiliteBackColor() const
{
	return m_clrValues.crInactiveHiliteBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetLineNumberTextColor() const
{
	return m_clrValues.crLineNumberText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetLineNumberBackColor() const
{
	return m_clrValues.crLineNumberBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetBreakpointTextColor() const
{
	return m_clrValues.crBreakpointText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetBreakpointBackColor() const
{
	return m_clrValues.crBreakpointBack;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITPAINTMANAGER_H__)
