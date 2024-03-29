// XTPCalendarMonthViewEvent.h: interface for the CXTPCalendarMonthViewEvent class.
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
#if !defined(_XTPCALENDARMONTHVIEWEVENT_H__)
#	define _XTPCALENDARMONTHVIEWEVENT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCalendarEvent;
class CXTPCalendarControl;
class CXTPCalendarMonthViewDay;
class CXTPCalendarMonthViewGroup;
struct XTP_CALENDAR_HITTESTINFO;

//===========================================================================
// Summary:
//     This class represents an event view portion of the CalendarMonthViewDay.
// Remarks:
//     It represents a specific view of the event associated data according
//     to various view types and display settings and provides basic
//     functionality on this data using user input through keyboard and mouse.
//
//          CXTPCalendarMonthViewEvent is based on the CXTPCalendarViewEventT template
//          class. It inherits basic behavior from the CXTPCalendarViewEventT template
//          class. Furthermore, CXTPCalendarMonthViewEvent overlaps and adds some
//          new functionality to get its own behavior and appearance.
//
// See Also:
//     CXTPCalendarViewEvent, CXTPCalendarViewEventT
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarMonthViewEvent
	: public CXTPCalendarViewEventT<CXTPCalendarMonthViewGroup>
{
	//{{AFX_CODEJOCK_PRIVATE
	// friend class CXTPCalendarPaintManager::CMonthViewEventPart;
	friend class CXTPCalendarMonthViewDay;

	DECLARE_DYNAMIC(CXTPCalendarMonthViewEvent)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//------------------------------------------------------------------------
	// Remarks:
	//     Base class type definition.
	//------------------------------------------------------------------------
	typedef CXTPCalendarViewEventT<CXTPCalendarMonthViewGroup> TBase;

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEvent   - Pointer to CXTPCalendarEvent.
	//     pViewGroup - Pointer to CXTPCalendarMonthViewGroup object.
	// See Also:
	//     ~CXTPCalendarWeekViewDay()
	//-----------------------------------------------------------------------
	CXTPCalendarMonthViewEvent(CXTPCalendarEvent* pEvent, CXTPCalendarMonthViewGroup* pViewGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarMonthViewEvent();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view content using
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
	//     point - A CPoint that contains the point to test.
	//     pInfo - A pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Returns:
	//     TRUE if the test is successful, FALSE else.
	// Remarks:
	//     Call this member function to gather hit test information from
	//     the day view.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint point, XTP_CALENDAR_HITTESTINFO* pInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	//     depending on the provided bounding rectangle and then calls
	//     AdjustLayout() for all sub-items.
	// Parameters:
	//     pDC               - Pointer to a valid device context.
	//     rcEventMax        - A CRect object that contains the coordinates
	//                         for drawing the view.
	//     nEventPlaceNumber - An int that contains the sequential place number.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to calling AdjustLayout().
	//     AdjustLayout2 is called by calendar control instead of AdjustLayout
	//     when theme is set.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber);
	virtual void
		AdjustLayout2(CDC* pDC, const CRect& rcEventMax,
					  int nEventPlaceNumber); //<COMBINE
											  // CXTPCalendarMonthViewEvent::AdjustLayout@CDC*@const
											  // CRect&@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the value of the view
	//     visible flag.
	// Remarks:
	//     Call this member function to determine the value of the visible flag.
	// Returns:
	//     A BOOL. TRUE if the view is visible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the value of the
	//     "show end time" flag.
	// Remarks:
	//     Call this member function to determine the value of the
	//     "show end time" flag.
	// Returns:
	//     A BOOL. TRUE if the view shows the end time value. FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsShowEndTime() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the value of the
	//     "show time as clock" flag.
	// Remarks:
	//     Call this member function to determine the value of the
	//     "show time as clock" flag.
	// Returns:
	//     A BOOL. TRUE if the view shows the time as a clock. FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsTimeAsClock() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform additional adjustments.
	// Remarks:
	//     Call this member function to perform additional adjustments after
	//     all adjustment activities are completed.
	//-----------------------------------------------------------------------
	virtual void OnPostAdjustLayout();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the editor window font.
	// Returns:
	//     A pointer to a CFont object that contains the editor window font.
	//-----------------------------------------------------------------------
	virtual CFont* GetSubjectEditorFont() const;

private:
};

//===========================================================================

/////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPCALENDARMONTHVIEWEVENT_H__)
