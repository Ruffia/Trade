// XTPCalendarDayViewEvent.h: interface for the CXTPCalendarDayViewEvent class.
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
#if !defined(_XTPCALENDARVIEWEVENT_H__)
#	define _XTPCALENDARVIEWEVENT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCalendarEvent;
class CXTPCalendarControl;
class CXTPCalendarDayViewDay;
class CXTPCalendarDayViewGroup;

//===========================================================================
// Summary:
//     This class represents an event view portion of the CalendarDayViewDay.
// Remarks:
//     This class is used to represent a specific view of the event associated
//     data according to various view types and display settings. This class also
//     provides basic functionality on the data by using user input through the
//     keyboard and mouse.
//
//          CXTPCalendarDayViewEvent is based on the CXTPCalendarDayEventT template
//          class. It inherits basic behavior from the CXTPCalendarDayEventT template
//          class. Furthermore, CXTPCalendarDayViewEvent overlaps and adds some
//          new functionality to get its own behavior and appearance.
//
// See Also:
//     CXTPCalendarViewEvent, CXTPCalendarViewEventT
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarDayViewEvent
	: public CXTPCalendarViewEventT<CXTPCalendarDayViewGroup>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarDayViewEvent)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//------------------------------------------------------------------------
	// Remarks:
	//     Base class type definition.
	//------------------------------------------------------------------------
	typedef CXTPCalendarViewEventT<CXTPCalendarDayViewGroup> TBase;

	// -----------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEvent     - Pointer to CXTPCalendarEvent.
	//     pViewGroup - Pointer to CXTPCalendarDayViewGroup object.
	// See Also:
	//     ~CXTPCalendarDayViewDay()
	// -----------------------------------------------------
	CXTPCalendarDayViewEvent(CXTPCalendarEvent* pEvent, CXTPCalendarDayViewGroup* pViewGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarDayViewEvent();

	// ------------------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout depending
	//     on the provided rectangle and then calls AdjustLayout() for all
	//     sub-items.
	// Parameters:
	//     pDC               - Pointer to a valid device context.
	//     rcEventMax        - A CRect object that contains the rectangle dimensions
	//                         for drawing the view.
	//     nEventPlaceNumber - An int that is used as a sequential place number.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to adjust layout.
	//     AdjustLayout2 is called by calendar control instead of AdjustLayout
	//     when theme is set.
	// ------------------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber);
	virtual void
		AdjustLayout2(CDC* pDC, const CRect& rcEventMax,
					  int nEventPlaceNumber); //<COMBINE
											  // CXTPCalendarDayViewEvent::AdjustLayout@CDC*@const
											  // CRect&@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform additional adjustments
	//     in some kinds of views.
	// Remarks:
	//     Call this member function to perform additional adjustments after
	//     all adjustment activities are completed.
	//-----------------------------------------------------------------------
	virtual void OnPostAdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view content using
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Remarks:
	//     Call AdjustLayout() before calling Draw().
	// See Also:
	//     AdjustLayout(CRect rcDay)
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill an
	//     XTP_CALENDAR_HITTESTINFO structure.
	// Parameters:
	//     point - A CPoint object that contains the point to test.
	//     pInfo - A pointer to an XTP_CALENDAR_HITTESTINFO structure.
	// Returns:
	//     TRUE if the test is successful, FALSE otherwise.
	// Remarks:
	//     Call this member function to gather hit test information from
	//     the day view.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint point, XTP_CALENDAR_HITTESTINFO* pInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if an event is "Multi Day".
	// Returns:
	//     TRUE if events is "Multi Day". FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsMultidayEvent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the event is visible.
	// Remarks:
	//     This function calls IsVisibleEx(TRUE, TRUE, FALSE);
	// Returns:
	//     A BOOL that contains the value of the visibility of the event.
	//     TRUE if the event is visible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the value of the visibility flag.
	// Parameters:
	//     bCheckTop     - A BOOL that indicates if the top bound visibility is checked.
	//     bCheckBottom  - A BOOL that indicates if the bottom bound visibility is checked.
	//     bTopAndBottom - A BOOL that indicates if the top and bottom bound visibility is checked.
	// Remarks:
	//     Checks visibility of boundaries.
	// Returns:
	//     A BOOL that contains the value of the visibility flag.
	//     TRUE if boundaries are visible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsVisibleEx(BOOL bCheckTop, BOOL bCheckBottom, BOOL bTopAndBottom) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create and show the subject editor.
	// Remarks:
	//     A subject is created only for the master event if the event is
	//     multi-day. If this member function is called for a slave event,
	//     then the editing process is passed to the master event.
	// Returns:
	//     A pointer to an CXTPCalendarViewEvent object for which the
	//     editor is created.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* StartEditSubject();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event's bounding rectangle.
	// Remarks:
	//     Call this member function to get the event's bounding rectangle coordinates.
	// Returns:
	//     A CRect object containing the bounding rectangle coordinates.
	//-----------------------------------------------------------------------
	CRect GetEventsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event's view position identifier.
	// Remarks:
	//     The position identifier is a sequential number used to identify the event views.
	// Returns:
	//     An int that contains the value of the position identifier.
	//-----------------------------------------------------------------------
	int GetEventPlacePos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the count of the event
	//     view positions.
	// Remarks:
	//     Call this member function to count the number of event view positions.
	// Returns:
	//     An int that contains the number of view positions.
	//-----------------------------------------------------------------------
	int GetEventPlaceCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the event view place identifier.
	// Parameters:
	//     nPlace  - An int that contains the place identifier value.
	// Remarks:
	//     Call this member function to set the event view place identifier.
	//-----------------------------------------------------------------------
	void SetEventPlacePos(int nPlace);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the event view places count.
	// Parameters:
	//     nPlaceCount - An int that contains the number of view places.
	// Remarks:
	//     Call this member function to set event view places count.
	//-----------------------------------------------------------------------
	void SetEventPlaceCount(int nPlaceCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the event time
	//     should not be displayed.
	// Returns:
	//     A BOOL that contains the HideExentTime value.
	//     TRUE - if event time should not be displayed. Otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHideEventTime() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain timeframe's bounding rectangle.
	// Remarks:
	//     Call this member function to get timeframe's bounding rectangle coordinates.
	// Returns:
	//     A CRect object containing the bounding rectangle coordinates.
	//-----------------------------------------------------------------------
	CRect GetTimeframeRect() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the editor window font.
	// Returns:
	//     Pointer to a CFont object that contains the editor window font.
	//-----------------------------------------------------------------------
	virtual CFont* GetSubjectEditorFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the styles of the
	//     editor window.
	// Returns:
	//     A DWORD that contains the value of the window styles.
	//-----------------------------------------------------------------------
	virtual DWORD GetSubjectEditorStyles() const;

	// ------------------------------------------------------------
	// Summary:
	//     This function is used to change the event place number.
	// Parameters:
	//     nNewPlace :  New event place number.
	// ------------------------------------------------------------
	virtual void ChangeEventPlace(int nNewPlace);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform adjustments of the subject
	//     editor area.
	// Remarks:
	//     Call this member function to perform adjustment of subject editor
	//     rectangle.
	//-----------------------------------------------------------------------
	virtual void AdjustSubjectEditor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform adjustments of the subject
	//     editor area. Inputs bUpdateView - A BOOL. TRUE if event view
	//     update is required.
	// Parameters:
	//     bUpdateView :  TRUE if event view update is required, otherwise FALSE.
	// Remarks:
	//     Call this member function to perform adjustment of subject editor
	//     rectangle.
	//-----------------------------------------------------------------------
	virtual void AdjustSubjectEditorEx(BOOL bUpdateView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the slot number of event in all day/
	//     multiday events.
	// Returns:
	//     An integer specifying the slot number.
	//-----------------------------------------------------------------------
	int GetSlotNumber() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function set the slot number of event in all day/
	//     multiday events.
	// Parameter:
	//     nSlotNumber - An integer specifying the slot number.
	//-----------------------------------------------------------------------
	void SetSlotNumber(int nSlotNumber);

public:
	CRect m_rcTimeframe; // Rectangle area to display times.

	BOOL m_bHideEventTime; // Flag determines if event time should not be displayed.
	BOOL m_bShowArrow;	 // TRUE if event in AllDay area should have arrow to indicate more events
						   // follows
private:
	int m_nSlotNumber;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPCalendarDayViewEvent::SetSlotNumber(int nSlotNumber)
{
	m_nSlotNumber = nSlotNumber;
}
AFX_INLINE int CXTPCalendarDayViewEvent::GetSlotNumber() const
{
	return m_nSlotNumber;
}
AFX_INLINE int CXTPCalendarDayViewEvent::GetEventPlacePos() const
{
	return (int)LOWORD((DWORD)m_nEventPlaceNumber);
}
AFX_INLINE int CXTPCalendarDayViewEvent::GetEventPlaceCount() const
{
	return (int)HIWORD((DWORD)m_nEventPlaceNumber);
}
AFX_INLINE void CXTPCalendarDayViewEvent::SetEventPlacePos(int nPlace)
{
	m_nEventPlaceNumber = (int)MAKELONG((WORD)nPlace, HIWORD((DWORD)m_nEventPlaceNumber));
}
AFX_INLINE void CXTPCalendarDayViewEvent::SetEventPlaceCount(int nPlaceCount)
{
	m_nEventPlaceNumber = (int)MAKELONG(LOWORD((DWORD)m_nEventPlaceNumber), (WORD)nPlaceCount);
}
AFX_INLINE DWORD CXTPCalendarDayViewEvent::GetSubjectEditorStyles() const
{
	if (IsMultidayEvent())
	{
		return TBase::GetSubjectEditorStyles();
	}
	return (ES_NOHIDESEL | ES_MULTILINE | ES_AUTOVSCROLL);
}

AFX_INLINE BOOL CXTPCalendarDayViewEvent::IsHideEventTime() const
{
	return m_bHideEventTime;
}
AFX_INLINE CRect CXTPCalendarDayViewEvent::GetTimeframeRect() const
{
	return m_rcTimeframe;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPCALENDARVIEWEVENT_H__)
