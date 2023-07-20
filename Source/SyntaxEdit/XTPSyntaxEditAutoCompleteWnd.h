// XTPSyntaxEditAutoCompleteWnd.h: interface for the CXTPSyntaxEditAutoCompleteWnd class.
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
#if !defined(__XTPSYNTAXEDITAUTOCOMPLETEWND_H__)
#	define __XTPSYNTAXEDITAUTOCOMPLETEWND_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditCtrl;
class CXTPImageManager;

//===========================================================================
// Summary:
//     The XTP_EDIT_ACDATA structure defines items' displayed data and provides
//     text and icons for each displayed line in the auto-complete window.
//===========================================================================
struct XTP_EDIT_ACDATA
{
	int m_nIcon;	   // The icon ID.
	CString m_strText; // The item text.

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	// Parameters:
	//     nIcon   - The icon ID.
	//     strText - The item text.
	//-----------------------------------------------------------------------
	XTP_EDIT_ACDATA(int nIcon, CString strText)
	{
		m_nIcon   = nIcon;
		m_strText = strText;
	}
};

//===========================================================================
// Summary:
//     The CXTPSyntaxEditACDataArray type defines a custom array to store
//     auto-complete items' displayed data.
// See Also:
//     PXTP_EDIT_ACDATA
//===========================================================================
typedef CArray<XTP_EDIT_ACDATA*, XTP_EDIT_ACDATA*> CXTPSyntaxEditACDataArray;

//===========================================================================
// Summary:
//     The CXTPSyntaxEditACGrid type defines a custom array to store and manage visible
//     rectangle arrays that represent lines of data in the auto-complete window.
//===========================================================================
typedef CArray<CRect, CRect> CXTPSyntaxEditACGrid;

//===========================================================================
// Summary:
//     This class implements the auto-complete window. It provides facilities
//     to display predefined strings of data in the sorted order. Then
//     the user can navigate over the list of strings and choose desired text
//     to complete their input. The auto-complete window provides some ways
//     to navigate by using vertical scroll bar, up/down keys, and mouse.
//     If the user types alphanumeric keys after the auto-complete window has
//     appeared, it will use the user input as search criteria and try to
//     find text from its predefined strings of data that are like user input.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditAutoCompleteWnd : public CWnd
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditCtrl;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditAutoCompleteWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditAutoCompleteWnd object, handles cleanup
	//     and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditAutoCompleteWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an auto-complete window and sets its predefined properties.
	// Parameters:
	//     pParentWnd - [in] Pointer to the parent window.
	// Remarks:
	//     Before creating an auto-complete window, you should construct
	//     a CXTPSyntaxEditAutoCompleteWnd object by calling its constructor.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd);

	using CWnd::Create;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the list of data to be displayed in the auto-complete window.
	// Parameters:
	//     parrData - [in] Reference to an array of data.
	// Remarks:
	//     The auto-complete window displays a list of data for the user to
	//     choose from.
	//
	//     If the auto-complete window already has a list of displayed data,
	//     then the existing list will be replaced by the new one.
	//
	//     New data will be sorted before being added to the auto-complete
	//     window.
	// See Also:
	//    CXTPSyntaxEditACDataArray
	//-----------------------------------------------------------------------
	virtual void SetList(CXTPSyntaxEditACDataArray& parrData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the list of open tags for the auto-complete window.
	// Parameters:
	//     strOpenTags - [in] String containing open tags.
	// Remarks:
	//     Open tags are treated as commands to be displayed and activated
	//     by the auto-complete window.
	//
	//     Each tag may consist of one or more symbols.
	//
	//     Tags must be separated by the "~" sign.
	//
	//     If the auto-complete window already has a list of tags,
	//     then the existing list will be replaced by the new one.
	//-----------------------------------------------------------------------
	virtual void SetOpenTags(CString strOpenTags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified string contains open tags.
	// Parameters:
	//     strToTest - [in] String to be tested.
	// Returns:
	//     TRUE if the specified string contains open tags, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsOpenTag(CString strToTest);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the list of close tags for the auto-complete window.
	// Parameters:
	//     strCloseTags - [in] String containing close tags.
	// Remarks:
	//     Close tags are treated as commands to be displayed and activated
	//     by the auto-complete window.
	//
	//     Each tag may consist of one or more symbols.
	//
	//     Tags must be separated by the "~" sign.
	//
	//     If the auto-complete window already has a list of tags,
	//     then the existing list will be replaced by the new one.
	//-----------------------------------------------------------------------
	virtual void SetCloseTags(CString strCloseTags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified string contains close tags.
	// Parameters:
	//     strToTest - [in] String to be tested.
	// Returns:
	//     TRUE if the specified string contains close tags, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCloseTag(CString strToTest);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows the auto-complete window.
	// Parameters:
	//     pt        - [in] Point where the upper left corner of
	//                 the auto-complete window should appear.
	//     strSearch - [in] Search string.
	// Remarks:
	//     Shows auto-complete window at specified point and try to filter
	//     dataset by previously typed piece of word and highlighted line
	//     according search string.
	//-----------------------------------------------------------------------
	virtual void Show(CPoint pt, CString strSearch = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Hides the auto-complete window.
	//-----------------------------------------------------------------------
	virtual void Hide();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the auto-complete window is visible and active.
	// Returns:
	//     TRUE if the auto-complete window is visible and active, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates display settings from system settings.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width of the auto-complete window.
	// Returns:
	//     The width, in pixels, of the auto-complete window.
	//-----------------------------------------------------------------------
	int GetWndWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the width for the auto-complete window.
	// Parameters:
	//     nWidth - Width, in pixels, to be set.
	//-----------------------------------------------------------------------
	void SetWndWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the image manager for the items.
	// Returns:
	//     The image manager for the items.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the char list that are tag delimiters.
	// Parameters:
	//     strDelims - char list that are tag delimiters.
	//-----------------------------------------------------------------------
	void SetDelimiters(CString strDelims);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the char list that are tag delimiters.
	// Returns:
	//     Char list that are tag delimiters.
	//-----------------------------------------------------------------------
	CString GetDelimiters();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the integer identfier of the text line at a specified point.
	// Parameters:
	//     ptTest - [in] Point to be tested.
	// Returns:
	//     The integer identifier of the text line at the specified point.
	//-----------------------------------------------------------------------
	int HitTest(CPoint ptTest) const;

	//{{AFX_CODEJOCK_PRIVATE
	// message handlers
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnPaint();
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CString m_strSearch;	  // Temporarily stores chain of characters from user input
							  // to perform an incremental search.
	CString m_strCloseTags;   // Stores set of close tags divided by the "~" sign
							  // (input close tags lead to close the auto-complete window).
	CString m_strTmpCloseTag; // Temporarily stores chain of characters from user input
							  // to recognize close tags.
	CString m_strOpenTags;	// Stores open tags.

	CString m_strTmpOpenTag; // Temporarily stores chain of characters from user input
							 // to recognize open tags.

	CXTPSyntaxEditCtrl* m_pParentWnd; // Pointer to the parent window.

	CXTPFont m_xtpFontBasic; // Font to display text.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontBasic, m_xtpFontBasic, GetBasicFontHandle);

	COLORREF m_clrWindow;		 // Standard window color.
	COLORREF m_clrHighLight;	 // Standard window highlight color.
	COLORREF m_clrWindowText;	// Standard windows text color.
	COLORREF m_clrHighLightText; // Standard windows highlight text color.

	CXTPSyntaxEditACDataArray m_arrACData; // Stores list of choices (line of text and image).

	CXTPSyntaxEditACDataArray m_arrACDataFiltered; // Stores filtered list of choices (line of text
												   // and image).

	CXTPSyntaxEditACGrid m_arrGrid; // Stores list of rectangle areas which were
									// displayed text/image strings from m_arrACData.

	int m_nBordersHeight; // Stores summary thickness of vertical window borders.
	int m_nLineHeight;	// Line height.
	int m_nLines;		  // Number of visible lines.
	int m_nWndHeight;	 // Height of the window.
	int m_nWndWidth;	  // Width of the window.

	int m_nHighLightLine;	 // Current highlight line identifier.
	int m_nFirstDisplayedStr; // Current first visible (upper) line.

	BOOL m_bFixedBottom; // Stores auto-complete window position (i.e. above or below at the
						 // cursor).

	BOOL m_bActive; // Active flag.

	BOOL m_bFilteredMode; // Flag to identify the current mode (filtered/full).

	BOOL m_bHighLight; // This flag determines if the selected string is highlighted.

	int m_nStartReplacePos; // Position of the first char in the Edit Control which will be replaced
							// by the chosen text.
	int m_nEndReplacePos;   // Position of the last char in the Edit Control which will be replaced
							// by the chosen text.
	CXTPImageManager* m_pImageManager; // Images for auto-complete list.

	CString m_strDelims; // Stores char list that are tag delimiters.

private:
	BOOL AdjusLayout(int nHeightMax = 0);
	void _AdjustWndRect(CRect& rrcWndRect);

	void RemoveAll();

	BOOL ScrollTo(int nNewLine);

	void UpdateFilteredList();

	void ReturnSelected(BOOL bAdjust = FALSE);

	void Sort();

	static int AFX_CDECL CompareACData(const XTP_EDIT_ACDATA** p1, const XTP_EDIT_ACDATA** p2);

	int Search(CString strSearch = _T(""));

	int Filter(CString strSearch = _T(""));

	static int AFX_CDECL CompareACDataToSearch(const XTP_EDIT_ACDATA** ppKey,
											   const XTP_EDIT_ACDATA** ppElem);

protected:
	CXTPSyntaxEditACDataArray m_InsertItems;
	CXTPSyntaxEditACDataArray m_RemoveItems;

public:
	void InsertItem(int iconID, CString str);
	void RemoveItem(CString str);

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditAutoCompleteWnd)

	afx_msg LPDISPATCH OleGetIcons();
	afx_msg void OleSetIcons(LPDISPATCH lpDispatch);
	afx_msg void OleInsertItem(int nID, const LPCTSTR str);
	afx_msg void OleRemoveItem(const LPCTSTR str);
	afx_msg bool OleIsExistItem(const LPCTSTR str);
	afx_msg void OleSetDelimiters(LPCTSTR pcszConfigFileName);
	afx_msg BSTR OleGetDelimiters();

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

/////////////////////////////////////////////////////////////////////////////
AFX_INLINE int CXTPSyntaxEditAutoCompleteWnd::GetWndWidth() const
{
	return m_nWndWidth;
}
AFX_INLINE BOOL CXTPSyntaxEditAutoCompleteWnd::IsActive() const
{
	return m_bActive;
}
AFX_INLINE void CXTPSyntaxEditAutoCompleteWnd::InsertItem(int iconID, CString str)
{
	m_InsertItems.Add(new XTP_EDIT_ACDATA(iconID, str));
}
AFX_INLINE void CXTPSyntaxEditAutoCompleteWnd::RemoveItem(CString str)
{
	m_RemoveItems.Add(new XTP_EDIT_ACDATA(-1, str));
}
AFX_INLINE CXTPImageManager* CXTPSyntaxEditAutoCompleteWnd::GetImageManager() const
{
	return m_pImageManager;
}
AFX_INLINE void CXTPSyntaxEditAutoCompleteWnd::SetDelimiters(CString strDelims)
{
	m_strDelims = strDelims;
}
AFX_INLINE CString CXTPSyntaxEditAutoCompleteWnd::GetDelimiters()
{
	return m_strDelims;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITAUTOCOMPLETEWND_H__)
