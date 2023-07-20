// XTPSyntaxEditView.h : header file
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
#if !defined(__XTPSYNTAXEDITSYNTAXEDITVIEW_H__)
#	define __XTPSYNTAXEDITSYNTAXEDITVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditFindReplaceDlg;

namespace XTPSyntaxEditLexAnalyser
{
class CXTPSyntaxEditLexParser;
}

//===========================================================================
// Summary:
//     Describes printing options for the Calendar control.
// Remarks:
//     Use this class when you want to programmatically customize printing
//     options for the Calendar control.
//
//     See each property description to examine which customization options
//     are available.
// See Also:
//     CXTPPrintOptions
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditViewPrintOptions : public CXTPPrintOptions
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditViewPrintOptions)
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditViewPrintOptions();

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditViewPrintOptions);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     This class represents a View portion of the Edit Control. It extends
//     the functionality provided by the CView class from MFC's document-view
//     model implementation. The CXTPSyntaxEditView class works together with
//     CXTPSyntaxEditDoc and provides facilities for it to be displayed on
//     a screen or printed on a printer.
// See Also:
//     CXTPSyntaxEditDoc
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditView : public CView
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditDoc;
	DECLARE_DYNCREATE(CXTPSyntaxEditView)
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected object constructor. Used by dynamic creation.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditView() object, handles cleanup and
	//     de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditView();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font for the edit control.
	// Parameters:
	//     pLogfont   - [in] Pointer to a LOGFONT structure.
	//     bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                       By default, this parameter is FALSE.
	//-----------------------------------------------------------------------
	void SetFontIndirect(LOGFONT* pLogFont, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables line numbering.
	// Parameters:
	//     bEnable - [in] TRUE to enable line numbering,
	//                    FALSE to disable line numbering.
	//-----------------------------------------------------------------------
	void SetLineNumbers(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the horizontal scrollbar is enabled.
	// Returns:
	//     TRUE if the horizontal scrollbar is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetHorzScrollBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the vertical scrollbar is enabled.
	// Returns:
	//     TRUE if the vertical scrollbar is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetVertScrollBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the horizontal and vertical scrollbars.
	// Parameters:
	//     bHorz         - [in] TRUE to enable the horizontal scrollbar,
	//                          FALSE to disable the horizontal scrollbar.
	//     bVert         - [in] TRUE to enable the vertical scrollbar,
	//                          FALSE to disable the vertical scrollbar.
	//     bUpdateReg    - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                          By default, this parameter is FALSE.
	//     bRecalcLayout - [in] TRUE to recalculate the layout,
	//                          FALSE otherwise.
	// Returns:
	//     TRUE if the scrollbars were updated, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetScrollBars(BOOL bHorz, BOOL bVert, BOOL bUpdateReg = FALSE, BOOL bRecalcLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the gutter.
	// Parameters:
	//     bEnable - [in] TRUE to enable the gutter,
	//                    FALSE to disable the gutter.
	//-----------------------------------------------------------------------
	void SetSelMargin(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables auto-indent.
	// Parameters:
	//     bEnable - [in] TRUE to enable auto-indent,
	//                    FALSE to disable auto-indent.
	//-----------------------------------------------------------------------
	void SetAutoIndent(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables syntax colorization.
	// Parameters:
	//     bEnable - [in] TRUE to enable syntax colorization,
	//                    FALSE to disable syntax colorization.
	//-----------------------------------------------------------------------
	void SetSyntaxColor(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates all active sibling views.
	//-----------------------------------------------------------------------
	void UpdateAllViews();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the view ready for redraw.
	//-----------------------------------------------------------------------
	void SetDirty();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the smart edit control.
	// Returns:
	//     A reference to the smart edit control.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditCtrl& GetEditCtrl();

	const CXTPSyntaxEditCtrl& GetEditCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the smart edit control.
	// Parameters:
	//     pControl - Pointer to the smart edit control to be set.
	// Remarks:
	//     The default edit control window will be destroyed.
	//     InternalRelease will be called for the external control object
	//     in the view object destructor.
	// See Also:
	//     GetEditCtrl
	//-----------------------------------------------------------------------
	virtual void SetEditCtrl(CXTPSyntaxEditCtrl* pControl);

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
	//     Refreshes the whole document and recalculates the scrollbar.
	//-----------------------------------------------------------------------
	void Refresh();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the top row.
	// Parameters:
	//     iRow - [in] Row to be set.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetTopRow(int iRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the top row.
	// Returns:
	//     The identifier of the top row.
	//-----------------------------------------------------------------------
	int GetTopRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates sibling views while in split mode.
	// Parameters:
	//     pSender  - [in] Pointer to a sender object (or NULL).
	//     dwUpdate - [in] View update flags.
	//-----------------------------------------------------------------------
	void UpdateScrollPos(CView* pSender, DWORD dwUpdate = XTP_EDIT_UPDATE_ALL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the document.
	// Returns:
	//     A pointer to the document.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDoc* GetDocument() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit buffer manager.
	// Returns:
	//     A pointer to the edit buffer manager.
	// See also:
	//     CXTPSyntaxEditBufferManager
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager* GetDataManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated lexical parser.
	// Returns:
	//     A pointer to the associated lexical parser.
	//-----------------------------------------------------------------------
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* GetLexParser() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the attached Find/Replace dialog.
	// Returns:
	//     A pointer to the attached Find/Replace dialog.
	//-----------------------------------------------------------------------
	virtual CXTPSyntaxEditFindReplaceDlg* GetFindReplaceDlg();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the document title.
	// Returns:
	//     The document title.
	//-----------------------------------------------------------------------
	virtual CString GetPageTitle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the print options of this view.
	// Returns:
	//     A pointer to the print options of this view.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditViewPrintOptions* GetPrintOptions() const;

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:
	afx_msg void OnPaint();
	virtual void OnDraw(CDC* pDC); // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_EDITCHANGING message
	//     which is sent before editing is applied.
	// Parameters:
	//     pNMHDR  - [in] Pointer to a NMHDR structure.
	//     pResult - [out] Pointer to a LRESULT value.
	//                     TRUE to cancel editing,
	//                     FALSE to allow editing.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     OnEditChanged, XTP_EDIT_NM_EDITCHANGING, XTP_EDIT_NM_EDITCHANGED
	//-----------------------------------------------------------------------
	virtual BOOL OnEditChanging(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_EDITCHANGED message
	//     which is sent when editing is applied.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NMHDR_EDITCHANGED structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_EDITCHANGING, XTP_EDIT_NM_EDITCHANGED,
	//     XTP_EDIT_NMHDR_EDITCHANGED
	//-----------------------------------------------------------------------
	virtual BOOL OnEditChanged(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_SETDOCMODIFIED message
	//     which is sent when the modified state of the document (edit control) is changed.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NMHDR_DOCMODIFIED structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_SETDOCMODIFIED, XTP_EDIT_NMHDR_DOCMODIFIED
	//-----------------------------------------------------------------------
	virtual BOOL OnSetDocModified(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_DRAWBOOKMARK message
	//     which is sent before the bookmark is drawn.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_SENMBOOKMARK structure.
	//     pResult - [out] Pointer to a LRESULT value.
	//                     TRUE to cancel default processing (bookmark drawing),
	//                     FALSE to allow default processing.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_DRAWBOOKMARK, XTP_EDIT_SENMBOOKMARK
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawBookmark(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_ROWCOLCHANGED message
	//     which is sent before the current row or column is changed.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NMHDR_ROWCOLCHANGED structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_ROWCOLCHANGED, XTP_EDIT_NMHDR_ROWCOLCHANGED
	//-----------------------------------------------------------------------
	virtual BOOL OnRowColChanged(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_UPDATESCROLLPOS message
	//     which is sent before the scroll bar position is changed.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NM_UPDATESCROLLPOS structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_UPDATESCROLLPOS
	//-----------------------------------------------------------------------
	virtual BOOL OnUpdateScrollPos(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_ENABLESCROLLBAR message
	//     which is sent before the scroll bar state is changed.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NMHDR_ENABLESCROLLBAR structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_ENABLESCROLLBAR
	//-----------------------------------------------------------------------
	virtual BOOL OnEnableScrollBar(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_INSERTKEY message
	//     which is sent when the 'Ins' (VK_INSERT) key is down (insert mode changed).
	// Parameters:
	//     pNMHDR  - [in] Pointer to a NMHDR structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_INSERTKEY
	//-----------------------------------------------------------------------
	virtual BOOL OnInsertKey(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_SELINIT message
	//     which is sent when the selection is reset.
	// Parameters:
	//     pNMHDR  - [in] Pointer to a NMHDR structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_SELINIT
	//-----------------------------------------------------------------------
	virtual BOOL OnSelInit(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_STARTOLEDRAG message
	//     which is sent when OLE dragging is started.
	// Parameters:
	//     pNMHDR  - [in] Pointer to a NMHDR structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_STARTOLEDRAG
	//-----------------------------------------------------------------------
	virtual BOOL OnStartOleDrag(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_MARGINCLICKED message
	//     which is sent when the left mouse button is clicked on the bookmarks area.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NMHDR_MARGINCLICKED structure.
	//     pResult - [out] Pointer to a LRESULT value.
	//                     TRUE to cancel default processing (breakpoint set/reset),
	//                     FALSE to allow default processing.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_MARGINCLICKED, XTP_EDIT_NMHDR_MARGINCLICKED
	//-----------------------------------------------------------------------
	virtual BOOL OnMarginClicked(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is the default handler for the XTP_EDIT_NM_PARSEEVENT message
	//     which is sent when a parser event occurs.
	// Parameters:
	//     pNMHDR  - [in] Pointer to an XTP_EDIT_NMHDR_PARSEEVENT structure.
	//     pResult - This parameter is unused for this handler.
	// Returns:
	//     TRUE if the message was handled successfully, otherwise FALSE.
	// See Also:
	//     XTP_EDIT_NM_PARSEEVENT, XTP_EDIT_NMHDR_PARSEEVENT
	//-----------------------------------------------------------------------
	virtual BOOL OnParseEvent(NMHDR* pNMHDR, LRESULT* pResult);

protected:
#	ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//     Overwrite this method to customize read-only file handling.
	// Returns:
	//     TRUE if a read-only file can be changed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CanChangeReadonlyFile();

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditRepeat();
	afx_msg void OnUpdateEditRepeat(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFilePageSetup();
	afx_msg void OnUpdateKeyIndicator(CCmdUI* pCmdUI);

	virtual void _EditFindReplace(BOOL bReplaceDlg);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to apply an update to all sibling views.
	// Parameters:
	//     pNMHDR_EC    - [in] Pointer to the parameters header.
	//     bTextChanged - [in] TRUE if document text was changed, FALSE otherwise.
	//-----------------------------------------------------------------------
	void UpdateSiblings(XTP_EDIT_NMHDR_EDITCHANGED* pNMHDR_EC = NULL, BOOL bTextChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a sibling view must be updated.
	// Parameters:
	//      pSender  - [in] Source to be updated from.
	//      dwUpdate - [in] Type of update. Allowed values are:
	//                      XTP_EDIT_UPDATE_HORZ, XTP_EDIT_UPDATE_VERT, XTP_EDIT_UPDATE_DIAG.
	// See Also
	//     CXTPSyntaxEditView
	//-----------------------------------------------------------------------
	void UpdateSiblingScrollPos(CXTPSyntaxEditView* pSender, DWORD dwUpdate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the view of a specified pane of the splitter.
	// Parameters:
	//     nRow - [in] Row of the splitter frame.
	//     nCol - [in] Column of the splitter frame.
	// Returns:
	//     A pointer to the view of the specified pane of the splitter.
	// See Also:
	//     CXTPSyntaxEditView
	//-----------------------------------------------------------------------
	CXTPSyntaxEditView* GetSplitterView(int nRow, int nCol);

	//-----------------------------------------------------------------------
	// Summary:
	//      Starts OLE drag.
	//-----------------------------------------------------------------------
	void StartOleDrag();

	LOGFONT m_lfPrevFont; // Temporarily stores editor font.
	int m_nPrevTopRow;	// Temporarily stores previous top row.

protected:
	int m_nParserThreadPriority_WhenActive;   // Parser priority (active state).
	int m_nParserThreadPriority_WhenInactive; // Parser priority (inactive state).

	BOOL m_bDraggingOver;		 // TRUE if dragover is going on, FALSE otherwise.
	BOOL m_bDraggingStartedHere; // TRUE if dragging is started in this view, FALSE otherwise.

	BOOL m_bScrollBars; // Whether to create control with own scrollbars, or
						// let to manage scrolling for parent window.

	//-----------------------------------------------------------------------

	static CXTPSyntaxEditView* ms_pTargetView; // Target view pointer to be filled in by drag-drop
											   // routine.
	static BOOL ms_bDroppedHere;	 // TRUE if text is dropped in this view, FALSE otherwise
	static POINT ms_ptDropPos;		 // Drop mouse position.
	static DWORD_PTR ms_dwSignature; // Signature used during drag-drop operation.

	BOOL m_bOleDragging;		 // TRUE if OLE dragging is enabled, FALSE otherwise.
	BOOL m_bFilesDragging;		 // TRUE when dragging files, FALSE otherwise.
	COleDropTarget m_dropTarget; // OLE drop target.

	CSize m_szPage; // Page size for printing or preview.
	int m_iTopRow;  // Top row for display.

	CWnd* m_pParentWnd;
	CXTPSyntaxEditCtrl* m_pEditCtrl; // Edit control instance.

	static CXTPSyntaxEditFindReplaceDlg* m_pFindReplaceDlg; // Pointer to find or replace dialog.

	BOOL m_bOnSizeRunning; // TRUE if the OnSize message handler is in progress, FALSE otherwise.

	CXTPSyntaxEditViewPrintOptions* m_pPrintOptions; // Printing options.
	CUIntArray m_aPageStart;						 // Printed page start row indexes.

public:
	BOOL m_bPrintDirect;		   // TRUE if the Print Dialog is bypassed. FALSE by default.
	BOOL m_bResizeControlWithView; // FALSE if the attached Calendar control will not
								   // be resized with the view. TRUE by default.
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPSyntaxEditBufferManager* CXTPSyntaxEditView::GetEditBuffer() const
{
	return GetEditCtrl().GetEditBuffer();
}
AFX_INLINE CXTPSyntaxEditConfigurationManager* CXTPSyntaxEditView::GetLexConfigurationManager() const
{
	return GetEditCtrl().GetLexConfigurationManager();
}
AFX_INLINE CXTPSyntaxEditViewPrintOptions* CXTPSyntaxEditView::GetPrintOptions() const
{
	return m_pPrintOptions;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITSYNTAXEDITVIEW_H__)
