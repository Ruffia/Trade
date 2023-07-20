// XTPGridView.h: interface for the CXTPGridView class.
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
#if !defined(__XTPGRIDVIEW_H__)
#	define __XTPGRIDVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridViewPrintOptions;
class CXTPGridControl;
class CXTPGridPaintManager;

//===========================================================================
// Summary:
//     Identifier for a Grid control child window of CXTPGridView.
// Example:
// <code>
// int CXTPGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
//     if (CView::OnCreate(lpCreateStruct) == -1)
//         return -1;
//
//     if (!m_wndGrid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WM_VSCROLL,
//         CRect(0, 0, 0, 0), this, XTP_ID_GRID_CONTROL))
//     {
//         TRACE(_T("Failed to create tab control window\n"));
//         return -1;
//     }
//     return 0;
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED
//     CXTPGridView, CXTPGridControl, CXTPGridHeader
//===========================================================================
const UINT XTP_ID_GRID_CONTROL = 100;

//===========================================================================
// Summary:
//     The CXTPGridView class provides an implementation of the Grid control as a
//     view class. Use the GetGridCtrl method to access methods of CXTPGridControl.
// See Also:
//     CXTPGridControl
//===========================================================================
class _XTP_EXT_CLASS CXTPGridView : public CView
{
	DECLARE_DYNCREATE(CXTPGridView)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor used by dynamic creation.
	//-----------------------------------------------------------------------
	CXTPGridView();

	BOOL m_bSwitchMode;	// Tells whether to switch between modes (icon and grid).
	BOOL m_bShowRowNumber; // TRUE to show the row number, FALSE otherwise.
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the Grid control theme.
	// Parameters:
	//     paintTheme     - Grid control theme; must be one of the values
	//                      listed in the Remarks section.
	//     bEnableMetrics - TRUE to allow the theme to override control metrics.
	// Remarks:
	//     paintTheme can be one of the following values:
	//      * <b>xtpGridThemeDefault</b>: Enables default theme.
	//      * <b>xtpGridThemeOfficeXP</b>: Enables Office XP style theme.
	//      * <b>xtpGridThemeOffice2003</b>: Enables Office 2003 style theme.
	//      * <b>xtpGridThemeOffice2007</b>: Enables Office 2007 style theme.
	//      * <b>xtpGridThemeOffice2010</b>: Enables Office 2010 style theme.
	//      * <b>xtpGridThemeOffice2013</b>: Enables Office 2013 style theme.
	//      * <b>xtpGridThemeVisualStudio2005</b>: Enables Visual Studio 2005 style theme.
	//      * <b>xtpGridThemeVisualStudio2010</b>: Enables Visual Studio 2010 style theme.
	//      * <b>xtpGridThemeVisualStudio2012Light</b>: Enables VS 2012 Light style
	//                                                    Property Grid theme.
	//      * <b>xtpGridThemeVisualStudio2012Dark</b>: Enables VS 2012 Dark style
	//                                                   Property Grid theme.
	//      * <b>xtpGridThemeResource</b>: Enables visual style theme.
	//-----------------------------------------------------------------------
	void SetTheme(XTPGridPaintTheme paintTheme, BOOL bEnableMetrics = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Grid control.
	// Returns:
	//     A reference to the Grid control object.
	//-----------------------------------------------------------------------
	virtual CXTPGridControl& GetGridCtrl() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridCtrl)
	virtual CXTPGridControl& GetReportCtrl() const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the Grid control.
	// Parameters:
	//     pGrid - Pointer to the Grid control object to be set.
	//-----------------------------------------------------------------------
	void SetGridCtrl(CXTPGridControl* pGrid);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridCtrl)
	void SetReportCtrl(CXTPGridControl* pGrid);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager of the Grid control.
	// Returns:
	//     A pointer to the paint manager of the Grid control.
	//-----------------------------------------------------------------------
	CXTPGridPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the print options of this view.
	// Returns:
	//     A pointer to a CXTPGridViewPrintOptions object describing
	//     the print options of this view.
	//-----------------------------------------------------------------------
	CXTPGridViewPrintOptions* GetPrintOptions() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridView object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPGridView();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Prints a single page of the Grid control.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     pInfo       - Pointer to a CPrintInfo structure that describes the
	//                   current print job.
	//     rcPage      - Page bounding rectangle.
	//     nRowFrom    - First row to print.
	//     nColumnFrom - First column to print.
	//     nColumnTo   - Next column after the last column to print.
	// Remarks:
	//     This method prints the page header, page footer, and calls
	//     the PrintGrid method.
	// Returns:
	//     The index of the last printed row.
	//-----------------------------------------------------------------------
	virtual int PrintPage(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, int nRowFrom, int nColumnFrom,
						  int nColumnTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints the Grid control (columns, header, and rows) on the page.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     pInfo       - Pointer to a CPrintInfo structure that describes the
	//                   current print job.
	//     rcPage      - Page bounding rectangle.
	//     nRowFrom    - First row to print.
	//     nColumnFrom - First column to print.
	//     nColumnTo   - Next column after the last column to print.
	// Remarks:
	//     This method calls the PrintHeader and PrintRows methods.
	// Returns:
	//     The index of the last printed row.
	//-----------------------------------------------------------------------
	virtual int PrintGrid(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, int nRowFrom, int nColumnFrom,
						  int nColumnTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints the header of each page.
	// Parameters:
	//     pDC      - Pointer to a device context for page output.
	//     rcHeader - Header bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void PrintHeader(CDC* pDC, CRect rcHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints the footer of each page.
	// Parameters:
	//     pDC      - Pointer to a device context for page output.
	//     rcFooter - Footer bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void PrintFooter(CDC* pDC, CRect rcFooter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints the page header of the Grid control.
	// Parameters:
	//     pDC                      - Pointer to a device context for page output.
	//     pInfo                    - Pointer to a CPrintInfo structure that
	//                                describes the current print job.
	//     rcPage                   - Page print area.
	//     bOnlyCalculate           - TRUE to only calculate,
	//                                FALSE to calculate and print.
	//     nPageNumber              - The printed page number.
	//     nNumberOfPages           - The total number of printed pages.
	//     nHorizontalPageNumber    - The printed horizontal page number.
	//     nNumberOfHorizontalPages - The total number of printed horizontal pages.
	// Returns:
	//     The header height in device units.
	//-----------------------------------------------------------------------
	virtual int PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, BOOL bOnlyCalculate,
								int nPageNumber, int nNumberOfPages, int nHorizontalPageNumber,
								int nNumberOfHorizontalPages);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints the page footer of the Grid control.
	// Parameters:
	//     pDC                      - Pointer to a device context for page output.
	//     pInfo                    - Pointer to a CPrintInfo structure that
	//                                describes the current print job.
	//     rcPage                   - Page print area.
	//     bOnlyCalculate           - TRUE to only calculate,
	//                                FALSE to calculate and print.
	//     nPageNumber              - The printed page number.
	//     nNumberOfPages           - The total number of printed pages.
	//     nHorizontalPageNumber    - The printed horizontal page number.
	//     nNumberOfHorizontalPages - The total number of printed horizontal pages.
	// Returns:
	//     The footer height in device units.
	//-----------------------------------------------------------------------
	virtual int PrintPageFooter(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, BOOL bOnlyCalculate,
								int nPageNumber, int nNumberOfPages, int nHorizontalPageNumber,
								int nNumberOfHorizontalPages);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the external scrollbar control.
	// Parameters:
	//     pScrollBar - Pointer to a CScrollBar object.
	//     bHor       - Orientation flag; TRUE for a horizontal scrollbar,
	//                  FALSE for a vertical scrollbar.
	// See Also:
	//     CScrollBar
	//-----------------------------------------------------------------------
	void SetScrollBarCtrl(CScrollBar* pScrollBar, BOOL bHor = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the external slider control.
	// Parameters:
	//     pSlider - Pointer to a CSliderCtrl object.
	//-----------------------------------------------------------------------
	void SetSliderCtrl(CSliderCtrl* pSlider);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prints either the header or footer divider.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     rc          - Reference to the bounding rectangle of the divider.
	//     nHeaderRows - TRUE to print the header divider,
	//                   FALSE to print the footer divider.
	// Returns:
	//     The height of the printed divider.
	//-----------------------------------------------------------------------
	int PrintFixedRowsDivider(CDC* pDC, const CRect& rc, BOOL bHeaderRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prepares an array of columns for printing.
	// Parameters:
	//     pDC   - Pointer to a device context for page output.
	//     pInfo - Printing information.
	// Returns:
	//     The number of columns prepared for printing.
	//-----------------------------------------------------------------------
	int SetupStartCol(CDC* pDC, CPrintInfo* pInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified scrollbar control.
	// Parameters:
	//     nBar - Orientation flag; SB_HORZ to retrieve a horizontal scrollbar,
	//            SB_VERT to retrieve a vertical scrollbar.
	// Returns:
	//     A pointer to a CScrollBar object.
	//-----------------------------------------------------------------------
	CScrollBar* GetScrollBarCtrl(int nBar) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the visibility and positions of scrollbars.
	//-----------------------------------------------------------------------
	void UpdateScrollBars();

protected:
	//{{AFX_CODEJOCK_PRIVATE

#	ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#	endif

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPGridView)
	virtual BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnFilePageSetup();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CBitmap m_bmpGrayDC; // A temporary bitmap to convert a colored Grid control image to 'gray'
						 // colors when printing in black and white mode.
	CXTPGridControl m_wndGrid; // Child Grid control window.
	CXTPGridControl* m_pGrid;  // Child Grid control pointer.
	BOOL m_bPrintSelection;	// TRUE if only printing the currently selected rows in the grid,
							   // FALSE if printing the entire grid.

	CXTPGridViewPrintOptions* m_pPrintOptions; // Printing options.

	CXTPScrollBarCtrl m_wndVScrollBar;
	CXTPScrollBarCtrl m_wndHScrollBar;

	CScrollBar* m_pScrollBarVer; // Pointer to external vertical scrollbar control.
	CScrollBar* m_pScrollBarHor; // Pointer to external horizontal scrollbar control.

	//{{AFX_CODEJOCK_PRIVATE
	CSliderCtrl* m_pSlider;

	friend class CGridControlCtrl;

	int m_nColumnStart; // Starting index of the current printed range of columns.
	int m_nColumnEnd;   // Ending (or next starting) index of the current range of columns.
	UINT m_nStartIndex; // Index of the current printed virtual page
						// (in CXTPGridPaintManager::m_arStartCol).

	CUIntArray m_aPageStart; // Array of indexes for the top rows of every printed page.
	BOOL m_bPaginated;

	//}}AFX_CODEJOCK_PRIVATE

public:
	BOOL m_bAllowCut;			   // If TRUE, then the cut operation is allowed.
	BOOL m_bAllowPaste;			   // If TRUE, then the paste operation is allowed.
	BOOL m_bPrintDirect;		   // If TRUE, then the print dialog is bypassed. FALSE by default.
	BOOL m_bResizeControlWithView; // If FALSE, then the attached Grid control will not be resized
								   // with the view. TRUE by default.
};

AFX_INLINE CXTPGridViewPrintOptions* CXTPGridView::GetPrintOptions() const
{
	return m_pPrintOptions;
}

AFX_INLINE void CXTPGridView::SetReportCtrl(CXTPGridControl* pGrid)
{
	SetGridCtrl(pGrid);
}

AFX_INLINE CXTPGridControl& CXTPGridView::GetReportCtrl() const
{
	return GetGridCtrl();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDVIEW_H__)
