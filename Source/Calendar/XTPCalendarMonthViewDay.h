// XTPCalendarMonthViewDay.h: interface for the CXTPCalendarMonthViewDay class.
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
#if !defined(_XTPCALENDARMONTHVIEWDAY_H__)
#	define _XTPCALENDARMONTHVIEWDAY_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE
class CXTPCalendarMonthView;
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Forward definition of the structure.
//===========================================================================
struct XTP_CALENDAR_HITTESTINFO;

//===========================================================================
// Summary:
//     This class represents a single resource view portion of the Calendar
//     Month View.
// Remarks:
//     It represents a specific view of the CalendarView's associated events
//     data grouped by one day and one resource group. And provides basic
//     functionality on this data using user input through keyboard and mouse.
//
// See Also:
//     CXTPCalendarMonthViewDay
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarMonthViewGroup
	: public CXTPCalendarViewGroupT<CXTPCalendarMonthViewDay, CXTPCalendarMonthViewEvent,
									CXTPCalendarMonthViewGroup>
{
public:
	//------------------------------------------------------------------------
	// Summary:
	//     Base class type definition.
	//------------------------------------------------------------------------
	typedef CXTPCalendarViewGroupT<CXTPCalendarMonthViewDay, CXTPCalendarMonthViewEvent,
								   CXTPCalendarMonthViewGroup>
		TBase;

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//     Construct CXTPCalendarMonthViewGroup object.
	// Parameters:
	//     pViewDay - Pointer to CXTPCalendarMonthViewDay object.
	//-----------------------------------------------------------------------
	CXTPCalendarMonthViewGroup(CXTPCalendarMonthViewDay* pViewDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarMonthViewGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the bounding rectangle of
	//     the tooltip space (currently this is only "click to add new appointment").
	//     Tooltip rectangle is calculated based on the current mouse position.
	// Parameters:
	//      ptHit : mouse position point
	//      hitInfo : info to get
	// Returns:
	//     A CRect object with relative coordinates of the tooltip.
	// Remarks:
	//     This member function returns a rectangle calculated as a bottom
	//     cell rect of the day area where user currently holds a mouse on.
	//-----------------------------------------------------------------------
	virtual CRect GetTooltipRect(const CPoint& ptHit,
								 const XTP_CALENDAR_HITTESTINFO& hitInfo) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to itself.
	// Remarks:
	//     Call this member function to get the "this" pointer.
	// Returns:
	//     Pointer to a CXTPCalendarMonthViewGroup.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarMonthViewGroup* GetPThis();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill a
	//     XTP_CALENDAR_HITTESTINFO structure.
	// Parameters:
	//     pHitTest - Pointer to XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     Call this member function to gather hit-test information.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const;
};

AFX_INLINE CXTPCalendarMonthViewGroup* CXTPCalendarMonthViewGroup::GetPThis()
{
	return this;
}

//===========================================================================
// Summary:
//     This class represents a day view portion of the CalendarMonthView.
// Remarks:
//     It represents a specific view of the CalendarView's associated events
//     data grouped by one day and provides basic functionality on this
//     data using user input through keyboard and mouse.
//
//          CXTPCalendarMonthViewDay is based on CXTPCalendarViewDayT template
//          class. It inherits basic behavior from its parent and overlaps/add some
//          methods to get its own behavior and look.
//
// See Also:
//     CXTPCalendarViewDay, CXTPCalendarViewDayT
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarMonthViewDay
	: public CXTPCalendarViewDayT<CXTPCalendarMonthView, CXTPCalendarMonthViewGroup,
								  XTP_CALENDAR_HITTESTINFO, CXTPCalendarMonthViewDay>
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarMonthViewEvent;
	DECLARE_DYNAMIC(CXTPCalendarMonthViewDay)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//------------------------------------------------------------------------
	// Summary:
	//     Base class type definition.
	//------------------------------------------------------------------------
	typedef CXTPCalendarViewDayT<CXTPCalendarMonthView, CXTPCalendarMonthViewGroup,
								 XTP_CALENDAR_HITTESTINFO, CXTPCalendarMonthViewDay>
		TBase;

	// --------------------------------------------------------------------
	// Summary:
	//     Default object constructor. Construct a CXTPCalendarMonthViewDay
	//     object.
	// Parameters:
	//     pMonthView :     Pointer to CXTPCalendarMonthView object.
	//     nWeekIndex :     An int that contains a week view identifier.
	//     nWeekDayIndex :  An int that contains a day view identifier.
	// See Also:
	//     ~CXTPCalendarMonthViewDay()
	// --------------------------------------------------------------------
	CXTPCalendarMonthViewDay(const CXTPCalendarMonthView* pMonthView, int nWeekIndex,
							 int nWeekDayIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarMonthViewDay();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	//     depending on the provided rectangle and then calls
	//     AdjustLayout() for all sub-items.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     rcDay - A CRect object that contains the coordinates for
	//             drawing the view.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to calling AdjustLayout().
	//     AdjustLayout2 is called by calendar control instead of AdjustLayout
	//     when theme is set.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcDay);
	virtual void AdjustLayout2(
		CDC* pDC, const CRect& rcDay); //<COMBINE
									   // CXTPCalendarMonthViewDay::AdjustLayout@CDC*@const
									   // CRect&>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view contents using
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Remarks:
	//     Call AdjustLayout() before Draw()
	// See Also:
	//     AdjustLayout(CRect rcDay)
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill a
	//     XTP_CALENDAR_HITTESTINFO structure.
	// Parameters:
	//     pInfo - Pointer to XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     Call this member function to gather hit-test information.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at the specified position index, and returns additional
	//     info in the XTP_CALENDAR_HITTESTINFO template object.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     Implements standard functionality for the HitTestEx method.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to calculate the day date bounding
	//     rectangle.
	// Returns:
	//     A CRect object denoting the day date bounding rectangle.
	// Remarks:
	//     Call this member function to determine the day view header's
	//     bounding rectangle.
	//-----------------------------------------------------------------------
	CRect CalcDayDateRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to calculate the day view area in
	//     which to draw event views.
	// Remarks:
	//     Call this member function to determine the day view area that is
	//     used for drawing the event views.
	// Returns:
	//     CRect object with rectangle coordinates.
	//-----------------------------------------------------------------------
	CRect GetDayEventsRect() const;

	virtual void SetSelected();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process left mouse button events.
	// Parameters:
	//     nFlags  - A UINT that is used to indicates whether various virtual
	//               keys are down.
	//     point   - A CPoint that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     presses the left mouse button.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes mouse movement events.
	// Parameters:
	//     nFlags  - A UINT that is used to indicate whether various virtual
	//               keys are down.
	//     point   - A CPoint that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     moves the mouse cursor or stylus.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to itself.
	// Remarks:
	//     Call this member function to get the "this" pointer.
	// Returns:
	//     Pointer to a CXTPCalendarMonthViewDay.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarMonthViewDay* GetPThis();

	int m_nWeekIndex;	// Week identifier.
	int m_nWeekDayIndex; // Day identifier.
};

//===========================================================================

AFX_INLINE CXTPCalendarMonthViewDay* CXTPCalendarMonthViewDay::GetPThis()
{
	return this;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPCALENDARMONTHVIEWDAY_H__)
