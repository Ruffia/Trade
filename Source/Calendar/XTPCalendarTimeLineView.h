// XTPCalendarTimeLineView.h: interface for the CXTPCalendarTimeLineView class.
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
#if !defined(_XTPCALENDARTIMELINEVIEW_H__)
#	define _XTPCALENDARTIMELINEVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	pragma warning(disable : 4100)

class CXTPCalendarTimeLineView;
class CXTPCalendarTimeLineViewGroups;
class CXTPCalendarTimeLineViewTimeScale;
class CXTPCalendarTimeLineViewTimeScaleGrid;

//===========================================================================
// Summary:
//     Set of parameters transferred from calendar items to
//     mouse handlers.
// Remarks:
//     This structure groups together all essential parameters used in
//     item processing activities. It is a basic structure for all
//     particular calendar's structures.
// See also:
//     XTP_CALENDAR_HITTESTINFO
//===========================================================================
struct XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW : public XTP_CALENDAR_HITTESTINFO
{
	POINT pt;		 // Point where the mouse was clicked in client coordinates.
	COleDateTime dt; // A datetime object to fill and return.

	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	// Remarks:
	//     Initializes an XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW object to the
	//     empty values.
	//-----------------------------------------------------------------------
	XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW()
	{
		pt.x = pt.y = -1;
		dt			= COleDateTime::GetCurrentTime();
	}
};

//===========================================================================
// Summary:
//     This class implements a specific view portion of the Calendar
//     control - called Timeline view.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineView : public CXTPCalendarView
{
	friend class CXTPCalendarControl;
	friend class CXTPDatePickerControl;

	DECLARE_DYNAMIC(CXTPCalendarTimeLineView)

public:
	//------------------------------------------------------------------------
	// Remarks:
	//     Base class type definition.
	//------------------------------------------------------------------------
	typedef CXTPCalendarView TBase;

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pCalendarControl - Pointer to a CXTPCalendarControl object.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineView(CXTPCalendarControl* pCalendarControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles class members deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTimeLineView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the view type flag.
	// Returns:
	//     An XTPCalendarViewType object that contains the view type flag.
	// See Also:
	//     XTPCalendarViewType
	//-----------------------------------------------------------------------
	virtual XTPCalendarViewType GetViewType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to populate the view with data
	//     for all items contained in the view.
	//-----------------------------------------------------------------------
	virtual void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	//     depending on the window's client dimensions and calls.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcView   - A CRect that contains the bounding rectangle dimensions
	//                of the view.
	//     bCallPostAdjustLayout - TRUE for calling additional post-adjustments.
	// Remarks:
	//     Call Populate() prior to adjusting layout. Adjust layout of all sub-items.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs all of the drawing logic of the
	//     calendar view.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs all of the drawing logic of the
	//     calendar view.
	//     Draw2 is called by the calendar control instead of Draw when
	//     a theme is set.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw2(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the date of the day view by
	//     the day's index.
	// Parameters:
	//     nIndex  - An int that contains the day view index in the view collection.
	// Remarks:
	//     Indexed number starts from 0 and cannot be negative.
	// Returns:
	//     A COleDateTime object that contains the day view date and time.
	//-----------------------------------------------------------------------
	virtual COleDateTime GetViewDayDate(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the start view date.
	// Returns:
	//     A COleDateTime object that contains the day view date and time.
	//-----------------------------------------------------------------------
	virtual COleDateTime GetStartViewDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the start view date.
	// Parameters:
	//     dt - Start view date as COleDateTime.
	//-----------------------------------------------------------------------
	virtual void SetStartViewDate(COleDateTime dt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the current time scale ID.
	// Returns:
	//     An int that contains the current time scale ID.
	//-----------------------------------------------------------------------
	virtual int GetTimeScaleID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the current time scale
	// Parameters:
	//     eTimeScaleParamsID - An int or XTPEnumCalendarTimeScaleParamsID.
	//-----------------------------------------------------------------------
	virtual void SetTimeScaleID(int eTimeScaleParamsID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the current time scale as an object.
	// Returns:
	//     A pointer to CXTPCalendarTimeLineViewTimeScale.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineViewTimeScale* GetTimeScale() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns
	//     additional info in an XTP_CALENDAR_HITTESTINFO structure.
	// Parameters:
	//     pt       - Coordinates of a point to test.
	//     pHitTest - Pointer to an XTP_CALENDAR_HITTESTINFO structure.
	// Returns:
	//     TRUE if an item is found, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns
	//     additional info in an XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW structure.
	// Parameters:
	//     pt       - Coordinates of a point to test.
	//     pHitTest - Pointer to an XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW structure.
	// Returns:
	//     TRUE if an item is found, otherwise FALSE.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW* pHitTest) const;

	using CXTPCalendarView::HitTestEx;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to retrieve information about a
	//     scrollbar's state.
	// Parameters:
	//     pSI - A pointer to a scrollbar information structure.
	// Returns:
	//     A BOOL. TRUE if the vertical scrollbar is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetScrollBarInfoV(SCROLLINFO* pSI) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to retrieve information about a
	//     scrollbar's state.
	// Parameters:
	//     pSI          - A pointer to a scrollbar information structure.
	//     pnScrollStep - A pointer to an int to receive scroll step. Can be NULL.
	// Returns:
	//     A BOOL. TRUE if the horizontal scrollbar is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetScrollBarInfoH(SCROLLINFO* pSI, int* pnScrollStep = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to scroll the content of a view.
	// Parameters:
	//     nPos     - Position value.
	//     nPos_raw - Raw position value.
	//-----------------------------------------------------------------------
	virtual void ScrollH(int nPos, int nPos_raw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to scroll the content of a view.
	// Parameters:
	//     nPos     - Position value.
	//     nPos_raw - Raw position value.
	//-----------------------------------------------------------------------
	virtual void ScrollV(int nPos, int nPos_raw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Scrolls the specified date into view and selects it if required.
	// Parameters:
	//     date -     A datetime value to scroll into view.
	//     bSelect -  A flag to select the date or not.
	//-----------------------------------------------------------------------
	virtual void ShowDay(const COleDateTime& date, BOOL bSelect = TRUE)
	{
		SetStartViewDate(date);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function shows a pop-up DatePicker control.
	// Parameters:
	//     dt -     The initial pop-up datetime selection.
	//     point -  The point to show the pop-up.
	//-----------------------------------------------------------------------
	void ShowCalendarPopup(COleDateTime dt, CPoint point);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the framework when metrics
	//     must be updated.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the framework before the
	//     view is destroyed.
	//-----------------------------------------------------------------------
	virtual void OnBeforeDestroy();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to force scroll to the end of the scroll range.
	// Parameters:
	//     nBar - Standard 0 - 1 values (for vertical or horizontal scrollbar).
	//-----------------------------------------------------------------------
	virtual void OnScrollEnd(int nBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the bounding rectangle of
	//     the tooltip space.
	//     Tooltip rectangle is calculated based on the current mouse position.
	// Parameters:
	//      ptHit - CPoint& of mouse position.
	//      hitInfo - XTP_CALENDAR_HITTESTINFO.
	// Returns:
	//     A CRect object with relative coordinates of the tooltip.
	// Remarks:
	//     This member function returns a rectangle calculated as an event
	//     rect which the user currently holds a mouse on.
	//-----------------------------------------------------------------------
	CRect GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get all events in a given range.
	// Parameters:
	//     dtStartDay -  The start of the range.
	//     dtEndDay -    The end of the range.
	// Returns:
	//     A CXTPCalendarEventsPtr object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEventsPtr _GetEvents(COleDateTime dtStartDay, COleDateTime dtEndDay);

	CXTPCalendarTimeLineViewTimeScale* m_pTimeScale; // Pointer to attached Time Scale object.
	CXTPCalendarTimeLineViewTimeScaleGrid* m_pTimeScaleGrid;

	int m_nHScrollPos; // keep HScroll position.
	int m_nVScrollPos; // Keep VScroll position.

private:
	CRect m_rcLastPopulate; // Last populated view rectangle - used for refresh by demand.

public:
	CXTPCalendarTimeLineViewGroups* m_pGroups; // Array of events organized in groups.

	BOOL m_bTimeLineSeparateGroup;			   // Flag to use separatore line between groups or not.
	int m_nTimelineMaxPixelsForEvent_ScaleDay; // Width in pixels for 1 event for day scale.
	int m_nTimelineMaxPixelsForEvent_ScaleWeek; //  Width in pixels for 1 event for week scale.
	int m_nTimelineMaxPixelsForEvent;			//  Width in pixels for 1 event for month scale.

	COLORREF m_clrEventBar;		   // Event Bar Color.
	COLORREF m_clrEventBarLine;	// Event Bar Line Color.
	COLORREF m_clrEventBorderLine; // Event Border Line Color.

	COLORREF m_clrTimeScaleBackground; // Time Scale Background Color.
	COLORREF m_clrTimeScaleBorder;	 // Time Scale Border Color.
	COLORREF m_clrTimeScaleHighlight;  // Time Scale Highlight Color.
	COLORREF m_clrTimeScaleText;	   // Time Scale Text Color.

	COLORREF m_clrSelectedBackground; // Selected Background Color.
	COLORREF m_clrSelectedText;		  // Selected Text Color.

	COLORREF m_clrBackground; // Background Color.
	COLORREF m_clrText;		  // Text Color.

	COLORREF m_clrWeekendBackground; // Weekend Background Color.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPCalendarTimeLineView);

public:
	afx_msg void OleSetMaxPixelsForEvent(long nPixels);
	afx_msg long OleGetMaxPixelsForEvent();
	afx_msg void OleSetMaxPixelsForEvent_ScaleDay(long nPixels);
	afx_msg long OleGetMaxPixelsForEvent_ScaleDay();
	afx_msg void OleSetMaxPixelsForEvent_ScaleWeek(long nPixels);
	afx_msg long OleGetMaxPixelsForEvent_ScaleWeek();

	afx_msg BOOL OleGetTimeLineSeparateGroup();
	afx_msg void OleSetTimeLineSeparateGroup(BOOL bSet);

	afx_msg OLE_COLOR OleGetTimeLineEventBarColor();
	afx_msg void OleSetTimeLineEventBarColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineEventBarLineColor();
	afx_msg void OleSetTimeLineEventBarLineColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineEventBorderLineColor();
	afx_msg void OleSetTimeLineEventBorderLineColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineScaleBackgroundColor();
	afx_msg void OleSetTimeLineScaleBackgroundColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineScaleBorderColor();
	afx_msg void OleSetTimeLineScaleBorderColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineScaleHighlightColor();
	afx_msg void OleSetTimeLineScaleHighlightColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineScaleTextColor();
	afx_msg void OleSetTimeLineScaleTextColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineSelectedBackgroundColor();
	afx_msg void OleSetTimeLineSelectedBackgroundColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineWeekendBackgroundColor();
	afx_msg void OleSetTimeLineWeekendBackgroundColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineSelectedTextColor();
	afx_msg void OleSetTimeLineSelectedTextColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineBackgroundColor();
	afx_msg void OleSetTimeLineBackgroundColor(OLE_COLOR clrOleColor);

	afx_msg OLE_COLOR OleGetTimeLineTextColor();
	afx_msg void OleSetTimeLineTextColor(OLE_COLOR clrOleColor);

	DATE OleGetTimeScaleMinTime();
	void OleSetTimeScaleMinTime(DATE dtNewScaleMinTime);

	DATE OleGetTimeScaleMaxTime();
	void OleSetTimeScaleMaxTime(DATE dtNewScaleMaxTime);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
AFX_INLINE XTPCalendarViewType CXTPCalendarTimeLineView::GetViewType() const
{
	return xtpCalendarTimeLineView;
}

AFX_INLINE CXTPCalendarTimeLineViewTimeScale* CXTPCalendarTimeLineView::GetTimeScale() const
{
	return m_pTimeScale;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPCALENDARTIMELINEVIEW_H__)
