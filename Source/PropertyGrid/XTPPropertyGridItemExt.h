// XTPPropertyGridItemExt.h
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
#if !defined(__XTPPROPERTYGRIDITEMEXT_H__)
#	define __XTPPROPERTYGRIDITEMEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropertyGridItemDate;

//===========================================================================
// Summary:
//     Month calendar pop-up used for CXTPPropertyGridItemDate item.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceMonthCal : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceMonthCal object.
	// Parameters:
	//     pItem - Pointer to the parent item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceMonthCal(CXTPPropertyGridItemDate* pItem)
	{
		m_pItem = pItem;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimal size required to display one month.
	// Parameters:
	//     pRect - Pointer to a RECT structure to receive the minimal
	//             size required to display one month. This parameter
	//             must be a valid address and cannot be NULL.
	// Remarks:
	//     This member function implements the behavior of the
	//     CMonthCalCtrl::GetMinReqRect function.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	BOOL GetMinReqRect(RECT* pRect) const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called if the user hits the Enter/Return key
	//     while an in-place calendar is displayed or if an item in the
	//     in-place calendar is selected.
	// See Also:
	//     OnCancel
	//-------------------------------------------------------------------------
	void OnAccept();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called if the user hits the Escape Key or clicks
	//     either another part of the property grid or another application
	//     while the in-place calendar is displayed.
	// See Also:
	//     OnAccept
	//-------------------------------------------------------------------------
	void OnCancel();

	//{{AFX_CODEJOCK_PRIVATE
	virtual void PostNcDestroy();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSelect(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPPropertyGridItemDate* m_pItem; // Parent item.
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemDate is a CXTPPropertyGridItem derived class.
//     It is used to create a Date item in a Property Grid control.
//
// Remarks:
//     When the in-place button for a date item is pressed,
//     a CXTPPropertyGridInplaceMonthCal object is created, which
//     is a User Interface that allows the user to easily select a date.
//
// Example:
//     This sample illustrates how to add an item of type Date to your grid:
// <code>
// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
//
// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
// //because this information is ignored.
// COleDateTime dates(2004, 12, 26, 0, 0, 0);
//
// //Adds a date item to the property grid and set the date to the value stored in dates
// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new
// CXTPPropertyGridItemDate(_T("Date item"), dates)));
//
// //Set the date format to MM/DD/YYYY
// pDate->SetDateFormat("%m/%d/%Y");
//
// //Changes the date to 3/22/2001
// COleDateTime newDate(2003, 5, 12, 0, 0, 0);
//
// //Changes the date of the Date item.
// pDate->SetDate(newDate);
//
// //Gets the currently set date
// TRACE(_T("Current Date= %d/%d/%d\n"), pDate->GetMonth(), pDate->GetDay(), pDate->GetYear());
// </code>
// See Also:
//     SetDate, GetMonth, GetDay, GetYear, SetDateFormat
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemDate : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemDate object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     oleDate    - Reference to a COleDateTime object containing
	//                  the initial date value of the item.
	//     pBindDate  - If not NULL, then the value of the item
	//                  is bound the value of this variable.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemDate(LPCTSTR strCaption, const COleDateTime& oleDate,
							 COleDateTime* pBindDate = NULL);
	CXTPPropertyGridItemDate(
		UINT nID, const COleDateTime& oleDate,
		COleDateTime* pBindDate =
			NULL); // <COMBINE
				   // CXTPPropertyGridItemDate::CXTPPropertyGridItemDate@LPCTSTR@const
				   // COleDateTime&@COleDateTime*>

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the date value for the item.
	// Parameters:
	//     oleDate - Reference to a COleDateTime object containing
	//               the date value. This value must include the
	//               month, day, and year and all time information
	//               should be set to zero.
	// Example:
	//     This sample illustrates how to set the currently selected
	//     date in and item of type CXTPPropertyGridItemDate:
	// <code>
	// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
	//
	// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
	// //because this information is ignored.
	// COleDateTime dates(2004, 12, 26, 0, 0, 0);
	//
	// //Adds a date item to the property grid and set the date to the value stored in dates
	// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new
	// CXTPPropertyGridItemDate(_T("Date item"), dates)));
	//
	// //Set the date format to MM/DD/YYYY
	// pDate->SetDateFormat("%m/%d/%Y");
	//
	// //Changes the date to 3/22/2001
	// COleDateTime newDate(2003, 5, 12, 0, 0, 0);
	//
	// //Changes the date of the Date item.
	// pDate->SetDate(newDate);
	//
	// //Gets the currently set date
	// TRACE(_T("Current Date= %d/%d/%d\n"), pDate->GetMonth(), pDate->GetDay(), pDate->GetYear());
	// </code>
	//-----------------------------------------------------------------------
	virtual void SetDate(const COleDateTime& oleDate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the time in the COleDateTime object to be represented
	//     as a SYSTEMTIME data structure. The resulting time will be
	//     stored in the SYSTEMTIME data structure specified by sysTime.
	// Parameters:
	//     sysTime - Reference to a SYSTEMTIME data structure to
	//               receive the resulting time.
	// Remarks:
	//     The SYSTEMTIME data structure initialized by this method
	//     will have its wMilliseconds member set to zero.
	// Returns:
	//     TRUE if the time in the COleDateTime object was successfully
	//     stored in the SYSTEMTIME data structure specified by sysTime.
	// Example:
	//     This sample code illustrates how to use the GetAsSystemTime member:
	// <code>
	// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
	//
	// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
	// //because this information is ignored.
	// COleDateTime dates(2004, 12, 26, 0, 0, 0);
	//
	// //Adds a date item to the property grid and set the date to the value stored in dates
	// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new
	// CXTPPropertyGridItemDate(_T("Date item"), dates)));
	//
	// //Creates a SYSTEMTIME object
	// SYSTEMTIME sysTime;
	//
	// //Stores the day, month, and year into the SYSTEMTIME structure, all other members will be 0.
	// if (pDate->GetAsSystemTime(sysTime))
	//     TRACE(_T("SysTime Current Date= %d/%d/%d\n"), sysTime.wMonth, sysTime.wDay,
	//     sysTime.wYear);
	// </code>
	//-----------------------------------------------------------------------
	virtual BOOL GetAsSystemTime(SYSTEMTIME& sysTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the date value of the item.
	// Returns:
	//     A reference to a COleDateTime object containing the date value.
	//-----------------------------------------------------------------------
	virtual const COleDateTime& GetDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the day of the date value of the item.
	// Returns:
	//     The day of the date value of the item.
	//-----------------------------------------------------------------------
	virtual long GetDay();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the day for the date value of the item.
	// Parameters:
	//     nDay - Day to be set.
	//-----------------------------------------------------------------------
	virtual void SetDay(long nDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the month of the date value of the item.
	// Returns:
	//     The month of the date value of the item.
	//-----------------------------------------------------------------------
	virtual long GetMonth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the month for the date value of the item.
	// Parameters:
	//     nMonth - Month to be set.
	//-----------------------------------------------------------------------
	virtual void SetMonth(long nMonth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the year of the date value of the item.
	// Returns:
	//     The year of the date value of the item.
	//-----------------------------------------------------------------------
	virtual long GetYear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the year for the date value of the item.
	// Parameters:
	//     nYear - Year to be set.
	//-----------------------------------------------------------------------
	virtual void SetYear(long nYear);

	//-----------------------------------------------------------------------
	// Summary:
	//     Parses the month, day, and year from a specified COleDateTime object.
	// Parameters:
	//     dt       - Reference to a COleDateTime object containing
	//                the month, day, and year.
	//     strValue - The date format:
	//                * \\%m - Month as decimal number (01 - 12).
	//                * \\%d - Day of month as decimal number (01 - 31).
	//                * \\%Y - Year with century, as decimal number.
	//                Format string for 05/22/2004 is "\\%m\\%d\\%Y".
	// Returns:
	//     TRUE if a valid date was extracted,
	//     FALSE if either an invalid or a NULL date was extracted.
	//-----------------------------------------------------------------------
	virtual BOOL ParseDateTime(COleDateTime& dt, LPCTSTR strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the date format.
	// Parameters:
	//     strFormat - The date format:
	//                 * \\%m - Month as decimal number (01 - 12).
	//                 * \\%d - Day of month as decimal number (01 - 31).
	//                 * \\%Y - Year with century, as decimal number.
	//                 Format string for 05/22/2004 is "\\%m\\%d\\%Y".
	// Example:
	//     This sample code illustrates how to change the date format of
	//     of type CXTPPropertyGridItemDate:
	// <code>
	// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
	//
	// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
	// //because this information is ignored.
	// COleDateTime dates(2004, 12, 26, 0, 0, 0);
	//
	// //Adds a date item to the property grid and set the date to the value stored in dates
	// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new
	// CXTPPropertyGridItemDate(_T("Date item"), dates)));
	//
	// //Set the date format to MM/DD/YYYY
	// pDate->SetDateFormat("%m/%d/%Y");
	// </code>
	//-----------------------------------------------------------------------
	virtual void SetDateFormat(LPCTSTR strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the displayed text for a NULL date.
	// Parameters:
	//     lpszNullValue - Text to be set.
	//-----------------------------------------------------------------------
	void AllowNullDate(LPCTSTR lpszNullValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the displayed text for a NULL date.
	// Returns:
	//     The displayed text for a NULL date.
	//-----------------------------------------------------------------------
	CString GetAllowNullDateString() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified COleDateTime object.
	// Parameters:
	//     pBindDate - Pointer to a COleDateTime object.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToDate member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties. Bound variables allow the property grid to
	//     store data in variables. When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value. The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE: If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	//-----------------------------------------------------------------------
	void BindToDate(COleDateTime* pBindDate);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Formats a specified COleDateTime object.
	// Parameters:
	//     oleDate - Reference to the COleDateTime object to be formatted.
	//-----------------------------------------------------------------------
	virtual CString Format(const COleDateTime& oleDate);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override this method to show an item-specific dialog.
	// Parameters:
	//     pButton - Pointer to the button that was pressed.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display the date picker dialog.
	//-------------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value of the item.
	//     Override this method to add new functionality. You should call
	//     the base class version of this function from your override.
	// Parameters:
	//     strValue - Value to be set.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the item is changed.
	//     Override this method if needed.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	void OnValueChanged(CString strValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first checked to see if it
	//     is bound to a variable. If it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//     OnBeforeInsert is called when an item is inserted,
	//     when a category is inserted, when a category or item is expanded,
	//     and when the sort property has changed.
	//-------------------------------------------------------------------------
	virtual void OnBeforeInsert();

private:
	void Init(const COleDateTime& oleDate);

protected:
	COleDateTime m_oleDate;	// Currently selected date. Only Month, Day, and Year are used.
	CString m_strNullValue;	// String value to be displayed for the Date item when the
							   // current date is NULL.
	COleDateTime* m_pBindDate; // Pointer to the variable bound to this item.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemDate)

	void OleSetValue(const VARIANT* varValue);
	const VARIANT OleGetValue();
	void BindDispatch();

	afx_msg void OleSetDateFormat(LPCTSTR pcszFormat);

//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPPropertyGridInplaceMonthCal;

	DECLARE_DYNAMIC(CXTPPropertyGridItemDate)
};

AFX_INLINE const COleDateTime& CXTPPropertyGridItemDate::GetDate() const
{
	return m_oleDate;
}

#	ifdef __AFXCTL_H__
//{{AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Picture item of ActiveX PropertyGrid
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemPicture : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a XTPPropertyGridItemPicture object.
	// Parameters:
	//     strCaption - Text caption of this item.
	//                  This is the text displayed in the left column of
	//                  the property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemPicture(LPCTSTR strCaption);

public:
	virtual CPictureHolder& GetPicture();
	virtual void SetPicturePath(LPCTSTR lpszPath);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override the method to show an item-specific dialog.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display browse folder dialog so they can select a picture.
	// See Also: XTPPropertyGridItemFlags, CXTPPropertyGridItem::SetFlags
	//-------------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is drawn. Override this function if
	//     needed.
	// Parameters:
	//     dc - Reference to the device context to be used for rendering an image
	//     of the item.
	//     rcValue - Bounding rectangle of the item.
	// Returns:
	//     TRUE if item is self-drawn.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

protected:
	CPictureHolder m_olePicture;
	CString m_strPicturePath;
	int m_nPreviewWidth;

#		ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemPicture)

	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();
	BSTR OleGetPicturePath();
#		endif
};

AFX_INLINE CPictureHolder& CXTPPropertyGridItemPicture::GetPicture()
{
	return m_olePicture;
}
//}}AFX_CODEJOCK_PRIVATE
#	endif

//===========================================================================
// Summary:
//     CXTPPropertyGridItemMultilineString is a CXTPPropertyGridItem derived class.
//     It is used to create an item with an in-place multi-line edit box.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemMultilineString : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemMultilineString object.
	// Parameters:
	//     lpszCaption - Caption of the item.
	//     nID         - Identifier of the item.
	//     strValue    - Initial value of the item.
	//     pBindString - If not NULL, then the value of the item
	//                   is bound the value of this variable.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemMultilineString(LPCTSTR lpszCaption, LPCTSTR strValue = NULL,
										CString* pBindString = NULL);
	CXTPPropertyGridItemMultilineString(
		UINT nID, LPCTSTR strValue = NULL,
		CString* pBindString =
			NULL); // <COMBINE
				   // CXTPPropertyGridItemMultilineString::CXTPPropertyGridItemMultilineString@LPCTSTR@LPCTSTR@CString*>

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override this method to show an item-specific dialog.
	// Parameters:
	//     pButton - Pointer to the button that was pressed.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display the date picker dialog.
	//-------------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
};

AFX_INLINE BOOL CXTPPropertyGridInplaceMonthCal::GetMinReqRect(RECT* pRect) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, MCM_GETMINREQRECT, 0, (LPARAM)pRect);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDITEMEXT_H__)
