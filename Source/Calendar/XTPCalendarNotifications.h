// XTPCalendarNotifications.h: unique notifications identifiers for
// CXTPCalendarControl class.
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
#if !defined(_XTPCALENDARNOTIFICATIONS_H__)
#	define _XTPCALENDARNOTIFICATIONS_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000
//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCalendarViewEvents;
class CXTPCalendarDayViewGroup;
typedef DWORD XTP_NOTIFY_CODE;

//## *** CXTPCalendarData notification IDs ***

//---------------------------------------------------------------------------
// Summary: New event has just been added to the data provider.
//          <b>Sender</b> - CXTPCalendarData, CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = EventID, lParam = CXTPCalendarEvent*
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAREVENTWASADDED = (WM_XTP_CALENDAR_BASE + 1);

//---------------------------------------------------------------------------
// Summary: Event has just been deleted from the data provider.
//          <b>Sender</b> - CXTPCalendarData, CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = EventID, lParam = CXTPCalendarEvent*
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAREVENTWASDELETED = (WM_XTP_CALENDAR_BASE + 2);

//---------------------------------------------------------------------------
// Summary: Event has just been changed in the data provider.
//          <b>Sender</b> - CXTPCalendarData, CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = EventID, lParam = CXTPCalendarEvent*
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAREVENTWASCHANGED = (WM_XTP_CALENDAR_BASE + 3);

//{{AFX_CODEJOCK_PRIVATE
//
// ***
// These handlers are remain for compatibility with previous versions.
// In new versions XTP_NC_CALENDAREVENTWASxxx should be used instead.
// Pattern can be retrieved from lParam = CXTPCalendarEvent*
// ***
//
//---------------------------------------------------------------------------
// Summary: New recurrence pattern has just been added to the data provider.
//          <b>Sender</b> - CXTPCalendarData, CXTPCalendarControl
// Remarks: <b>Parameters</b>   - wParam = RatternID, lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARPATTERNWASADDED = (WM_XTP_CALENDAR_BASE + 4);

//---------------------------------------------------------------------------
// Summary: Recurrence pattern has just been deleted from the data provider.
//          <b>Sender</b> - CXTPCalendarData, CXTPCalendarControl
// Remarks: <b>Parameters</b>   - wParam = RatternID, lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARPATTERNWASDELETED = (WM_XTP_CALENDAR_BASE + 5);

//---------------------------------------------------------------------------
// Summary: Recurrence pattern has just been changed in the data provider.
//          <b>Sender</b> - CXTPCalendarData, CXTPCalendarControl
// Remarks: <b>Parameters</b>   - wParam = RatternID, lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARPATTERNWASCHANGED = (WM_XTP_CALENDAR_BASE + 6);
//}}AFX_CODEJOCK_PRIVATE

//---------------------------------------------------------------------------
// Summary: Current Calendar Control view type(day, week, ...) or content
//          was changed.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b>   - wParam = 0, lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARVIEWWASCHANGED = (WM_XTP_CALENDAR_BASE + 7);

//---------------------------------------------------------------------------
// Summary: A Context Menu should be displayed for the Calendar Control.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARCONTEXTMENU = (WM_XTP_CALENDAR_BASE + 8);

//---------------------------------------------------------------------------
// Summary: On LButtonDown event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARLBUTTONDOWN = (WM_XTP_CALENDAR_BASE + 9);

//---------------------------------------------------------------------------
// Summary: On LButtonDblClick event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARLBUTTONDBLCLICK = (WM_XTP_CALENDAR_BASE + 10);

//---------------------------------------------------------------------------
// Summary: On LButtonUp event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARLBUTTONUP = (WM_XTP_CALENDAR_BASE + 11);

//---------------------------------------------------------------------------
// Summary: On MouseMove event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARMOUSEMOVE = (WM_XTP_CALENDAR_BASE + 12);

//---------------------------------------------------------------------------
// Summary: On KeyDown event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'key was pressed', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARKEYDOWN = (WM_XTP_CALENDAR_BASE + 13);

//---------------------------------------------------------------------------
// Summary: On RButtonDown event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARRBUTTONDOWN = (WM_XTP_CALENDAR_BASE + 14);

//---------------------------------------------------------------------------
// Summary: Calendar Control Day view time scale was changed.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b>   - wParam = 0, lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARVIEWTIMESCALEWASCHANGED = (WM_XTP_CALENDAR_BASE + 15);

//---------------------------------------------------------------------------
// Summary: Calendar Control options was changed.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b>
//                  wParam = -1 or XTPCalendarViewType enum value.
//                           Specify global option (-1) or which kind of view
//                           changed option is related.
//                  lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAROPTIONSWASCHANGED = (WM_XTP_CALENDAR_BASE + 16);

//---------------------------------------------------------------------------
// Summary: Calendar Control data provider (resources) was changed.
//          SetDataProvider() or SetResources() methods were called.
//          <b>Sender</b> - CXTPCalendarControl, CXTPCalendarView.
// Remarks: <b>Parameters</b>   wParam = 0 if Calendar control resources were changed,
//                                       1 if CalendarView resources were changed.
//                                       2 if CalendarViewDay resources were changed.
//                              lParam = NULL if wParam == 0,
//                                       CXTPCalendarView*    if wParam == 1,
//                                       CXTPCalendarViewDay* if wParam == 2.
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARDATAPROVIDERWASCHANGED = (WM_XTP_CALENDAR_BASE + 17);

//---------------------------------------------------------------------------
// Summary: alias of XTP_NC_CALENDARDATAPROVIDERWASCHANGED notification code
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_RESOURCES_WHERE_CHANGED =
	XTP_NC_CALENDARDATAPROVIDERWASCHANGED;

//---------------------------------------------------------------------------
// Summary: This notification is sent to determine if the specified editing operation
//          is disabled.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          This notification may be sent several times before the edit operation
//          to update the user interface state in the right way (set corresponding
//          mouse cursor for example).
//          The XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION notification is sent
//          only once before edit operation.
//
//          <b>Parameters</b>
//              wParam - [in] A pointer to XTP_EVENT_EDIT_OPERATION_PARAMS structure
//                       or to derived structure depends on eOperation member value.
//              lParam - [out] A pointer to the return result: BOOL*.
//                       Set TRUE if to disable specified editing operation,
//                       otherwise FALSE.
//                       Initial value is FALSE.
// See Also: XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION, XTPCalendarEditOperation,
//           XTP_EVENT_EDIT_OPERATION_PARAMS, XTP_EVENT_PASTE_OPERATION_PARAMS,
//           XTP_EVENT_IN_PLACE_CREATE_EVENT_OPERATION_PARAMS
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED =
	(WM_XTP_CALENDAR_BASE + 18);

//---------------------------------------------------------------------------
// Summary: This notification is sent to customize or disable a specified editing
//           operation.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          This notification is sent only once before an edit operation.
//          The XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION notification is sent
//          every time before this notification.
//
//          <b>Parameters</b>
//              wParam - [in] A pointer to XTP_EVENT_EDIT_OPERATION_PARAMS structure
//                       or to derived structure depends on eOperation member value.
//              lParam - [out] A pointer to the return result: BOOL*.
//                       Set TRUE if specified editing operation is handled,
//                       otherwise FALSE.
//                       Initial value is FALSE.
// See Also: XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED, XTPCalendarEditOperation,
//           XTP_EVENT_EDIT_OPERATION_PARAMS, XTP_EVENT_PASTE_OPERATION_PARAMS,
//           XTP_EVENT_IN_PLACE_CREATE_EVENT_OPERATION_PARAMS
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION = (WM_XTP_CALENDAR_BASE
																			+ 19);

//---------------------------------------------------------------------------
// Summary: This structure is used as a generic and base class for custom
//          edit event notifications.
// See Also: XTPCalendarEditOperation,
//           XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
//           XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
//           XTP_EVENT_PASTE_OPERATION_PARAMS,
//           XTP_EVENT_IN_PLACE_CREATE_EVENT_OPERATION_PARAMS
//---------------------------------------------------------------------------
struct XTP_EVENT_EDIT_OPERATION_PARAMS
{
	XTPCalendarEditOperation eOperation; // Edit operation.
	CXTPCalendarViewEvents* pEventViews; // Event views array with events set for edit operation.
	int nRefCounter;					 // Reserved. For internal use only.
};

//---------------------------------------------------------------------------
// Summary: This structure is used with notifications for custom edit event
//          notifications when <b>eOperation</b> is <b>xtpCalendarEO_Paste</b>.
// See Also: XTPCalendarEditOperation,
//           XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
//           XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
//           XTP_EVENT_EDIT_OPERATION_PARAMS
//---------------------------------------------------------------------------
struct XTP_EVENT_PASTE_OPERATION_PARAMS : public XTP_EVENT_EDIT_OPERATION_PARAMS
{
	CXTPCalendarEvent* pEvent;			// A pointer to an event prepared for paste.
	BOOL bPasteWillChangeExistingEvent; // If this member is TRUE, the new event will not be added,
										// just existing event will be updated. Used for creating
										// recurrence event exceptions by cut operation.
};

//---------------------------------------------------------------------------
// Summary: This structure is used with notifications for custom edit event
//          notifications when <b>eOperation</b> is <b>xtpCalendarEO_InPlaceCreateEvent</b>.
// See Also: XTPCalendarEditOperation,
//           XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
//           XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
//           XTP_EVENT_EDIT_OPERATION_PARAMS
//---------------------------------------------------------------------------
struct XTP_EVENT_IN_PLACE_CREATE_EVENT_OPERATION_PARAMS : public XTP_EVENT_EDIT_OPERATION_PARAMS
{
	LPCTSTR pcszInitialSubject; // A pointer to the string which contains initial subject value for
								// new event (generally initial char).
};

//---------------------------------------------------------------------------
// Summary: This structure is used with notifications for custom edit event
//          notifications when <b>eOperation</b> is <b>xtpCalendarEO_DragXXX</b>.
// Remarks:
//          Used only with XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED
//          notification.
// See Also: XTPCalendarEditOperation,
//           XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
//           XTP_EVENT_EDIT_OPERATION_PARAMS
//---------------------------------------------------------------------------
struct XTP_EVENT_DRAG_OPERATION_PARAMS : public XTP_EVENT_EDIT_OPERATION_PARAMS
{
	CXTPCalendarEvent* pDraggingEventOriginal; // A pointer to original dragging event
	CXTPCalendarEvent* pDraggingEvent;		   // A pointer to event before dragging operation.
	CXTPCalendarEvent* pDraggingEventNew;	  // A pointer to event after dragging operation.
};

//---------------------------------------------------------------------------
// Summary: This notification is sent when the selection is changed.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          <b>Parameters</b>
//              wParam - [in] A value from XTPCalendarSelectionChanged
//                       enum.
//              lParam - unused
// See Also: XTPCalendarSelectionChanged
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_SELECTION_CHANGED = (WM_XTP_CALENDAR_BASE + 20);

//---------------------------------------------------------------------------
// Summary: On RButtonUp event.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks: <b>Parameters</b> - wParam = 'Clicked Point', lParam = 0
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDARRBUTTONUP = (WM_XTP_CALENDAR_BASE + 21);

//---------------------------------------------------------------------------
// Summary: Enumeration of possible Calendar Reminder notification actions.
// Remarks: Members of this enumerations are used as an additional parameters
//          of the XTP_NC_CALENDAR_ON_REMINDERS notification
// See Also: XTP_NC_CALENDAR_ON_REMINDERS notification
//---------------------------------------------------------------------------
enum XTPCalendarRemindersAction
{
	xtpCalendarRemindersMonitoringStarted = 1, // Sends after monitoring has started (end of
											   // executing
											   // CXTPCalendarRemindersManager::StartMonitoring)
	xtpCalendarRemindersMonitoringStopped = 2, // Sends when monitoring has stopped
											   // (CXTPCalendarRemindersManager::StopMonitoring)
	xtpCalendarRemindersFire = 3,			   // Main notification, when reminder is executed.

	xtpCalendarReminderSnoozed = 4,		// When 1 pending reminder was snoozed. lParam =
										// CXTPCalendarReminder*
	xtpCalendarReminderDismissed = 5,   // When 1 pending reminder was dismissed. lParam =
										// CXTPCalendarReminder*
	xtpCalendarReminderDismissedAll = 6 // When all pending reminders were dismissed.
};

//---------------------------------------------------------------------------
// Summary: On RButtonUp event.
//          <b>Sender</b> - CXTPCalendarRemindersManager
// Remarks: <b>Parameters</b> - wParam = XTPCalendarRemindersAction, lParam = 0 or
// CXTPCalendarReminder*. See XTPCalendarRemindersAction.
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_ON_REMINDERS = (WM_XTP_CALENDAR_BASE + 22);

//---------------------------------------------------------------------------
// Summary: Doubled custom parameter for notifications.
//          Used when single WPARAM is not enough.
// See Also: WPARAM
//---------------------------------------------------------------------------
struct XTP_CALENDAR_WPARAM2
{
	WPARAM wParam1; // First custom parameter
	WPARAM wParam2; // Second custom parameter
};

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoRetrieveDayEvents.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//      <b>Parameters</b>
//           wParam - [in] A day date as XTP_DATE_VALUE;
//           lParam - [in/out] A pointer to pointer to events array
//                    as CXTPCalendarEventsPtr*;
//
// See Also: CXTPCalendarCustomDataProvider::DoRetrieveDayEvents
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoRetrieveDayEvents = (WM_XTP_CALENDAR_BASE + 23);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoRemoveAllEvents.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//
// See Also: CXTPCalendarCustomDataProvider::DoRemoveAllEvents
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoRemoveAllEvents = (WM_XTP_CALENDAR_BASE + 24);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoRead_Event.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//      <b>Parameters</b>
//           wParam - [in] An event ID to read as DWORD;
//           lParam - [in/out] A pointer to pointer to event
//                    as CXTPCalendarEventPtr*;
//
// See Also: CXTPCalendarCustomDataProvider::DoRead_Event
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoRead_Event = (WM_XTP_CALENDAR_BASE + 25);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoRead_RPattern.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//      <b>Parameters</b>
//           wParam - [in] An recurrence pattern ID to read as DWORD;
//           lParam - [in/out] A pointer to pointer to recurrence pattern
//                    as CXTPCalendarRecurrencePatternPtr*;
//
// See Also: CXTPCalendarCustomDataProvider::DoRead_RPattern
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoRead_RPattern = (WM_XTP_CALENDAR_BASE + 26);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoCreate_Event.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer as XTP_CALENDAR_WPARAM2* where :
//                    wParam1 - [in] A pointer to calendar event object
//                              as CXTPCalendarEvent*;
//                    wParam2 - [out] A pointer to newly created event ID as DWORD*;
//           lParam - [out] A pointer to result of operation as BOOL*;
//
// See Also: CXTPCalendarCustomDataProvider::DoCreate_Event
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoCreate_Event = (WM_XTP_CALENDAR_BASE + 27);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoUpdate_Event.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer to calendar event object as CXTPCalendarEvent*;
//           lParam - [out] A pointer to result of operation as BOOL*;
//
// See Also: CXTPCalendarCustomDataProvider::DoUpdate_Event
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoUpdate_Event = (WM_XTP_CALENDAR_BASE + 28);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoDelete_Event.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer to calendar event object as CXTPCalendarEvent*;
//           lParam - [out] A pointer to result of operation as BOOL*;
//
// See Also: CXTPCalendarCustomDataProvider::DoDelete_Event
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoDelete_Event = (WM_XTP_CALENDAR_BASE + 29);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoCreate_RPattern.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer as XTP_CALENDAR_WPARAM2* where :
//                    wParam1 - [in] A pointer to recurrence pattern object
//                              as CXTPCalendarRecurrencePattern*;
//                    wParam2 - [out] A pointer to newly created
//                              recurrence pattern ID as DWORD*;
//           lParam - [out] A pointer to result of operation as BOOL*;
//
// See Also: CXTPCalendarCustomDataProvider::DoCreate_RPattern
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoCreate_RPattern = (WM_XTP_CALENDAR_BASE + 30);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoUpdate_RPattern.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer to recurrence pattern object
//                         as CXTPCalendarRecurrencePattern*;
//           lParam - [out] A pointer to result of operation as BOOL*;
//
// See Also: CXTPCalendarCustomDataProvider::DoUpdate_RPattern
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoUpdate_RPattern = (WM_XTP_CALENDAR_BASE + 31);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoDelete_RPattern.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer to recurrence pattern object
//                         as CXTPCalendarRecurrencePattern*;
//           lParam - [out] A pointer to result of operation as BOOL*;
//
// See Also: CXTPCalendarCustomDataProvider::DoDelete_RPattern
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoDelete_RPattern = (WM_XTP_CALENDAR_BASE + 32);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoGetAllEvents_raw.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - Unused (=0);
//           lParam - [in/out] A pointer to pointer to events array
//                    as CXTPCalendarEventsPtr*;
//
// See Also: CXTPCalendarCustomDataProvider::DoGetAllEvents_raw
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoGetAllEvents_raw = (WM_XTP_CALENDAR_BASE + 33);

//---------------------------------------------------------------------------
// Summary:  This notification is used as an analog of the overridable method
//           CXTPCalendarData::DoGetUpcomingEvents.
//           <b>Sender</b> - CXTPCalendarCustomDataProvider, CXTPCalendarControl.
// Remarks:
//           Send by a custom data provider. Calendar control reflects this
//           notification as well as other notifications.
//  <b>Parameters</b>
//           wParam - [in] A pointer as XTP_CALENDAR_WPARAM2* where :
//                    wParam1 - [in] As COleDateTime*; Start date and time
//                              of a specified period;
//                    wParam2 - [in] As COleDateTimeSpan*; A duration of a
//                              specified period;
//           lParam - [in/out] A pointer to pointer to events array
//                    as CXTPCalendarEventsPtr*;
//
// See Also: CXTPCalendarCustomDataProvider::DoGetUpcomingEvents
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_DoGetUpcomingEvents = (WM_XTP_CALENDAR_BASE + 34);

//---------------------------------------------------------------------------
// Summary: This notification is sent when the selection is changed.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          <b>Parameters</b>
//              wParam - [in] CXTPCalendarViewGroup* - A pointer to Group View.
//              lParam - [in/out] CString* pointer to caption string.
// See Also: CXTPCalendarViewGroup::GetCaption
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_GETGROPVIEWCAPTION = (WM_XTP_CALENDAR_BASE + 35);

//---------------------------------------------------------------------------
// Summary: This notification is sent when calendar is populating.
//          lParam contains events which will be drawn for the group specified
//          by wParam.
//          You may add, remove or modify some events in the CXTPCalendarEvents
//          collection.
//
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          <b>Parameters</b>
//              wParam - [in] CXTPCalendarViewGroup* - A pointer to Group View.
//              lParam - [in/out] CXTPCalendarEvents* - A pointer to events array to draw.
// See Also: CXTPCalendarControl::Populate, XTP_NC_CALENDAR_PREPOPULATE_DAY.
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_PREPOPULATE = (WM_XTP_CALENDAR_BASE + 36);

//===========================================================================
// Summary:
//     Set of parameters transferred from calendar items for an
//     XTP_NC_CALENDAR_BEFORE_DRAW_DAYVIEWCELL notification.
// Remarks:
//     This structure defines day view cell (exclude all day events area) drawing
//     attributes.
// See Also: XTP_NC_CALENDAR_BEFORE_DRAW_DAYVIEWCELL
//===========================================================================
struct XTP_CALENDAR_DAYVIEWCELL_PARAMS
{
	int nIndex;		  // [in] Cell index.
	DATE dtBeginTime; // [in] Begin cell date and time.
	BOOL bSelected;   // [in] Is cell selected.

	COLORREF clrBackgroundColor; // [in/out] Background cell color.
};

//---------------------------------------------------------------------------
// Summary: This notification is sent for each calendar day view cell before draw.
//          You may customize a clrBackgroundColor member of the provided
//          XTP_CALENDAR_DAYVIEWCELL_PARAMS structure.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          <b>Parameters</b>
//              wParam - [in] CXTPCalendarViewGroup* - A pointer to Group View.
//              lParam - [in/out] XTP_CALENDAR_DAYVIEWCELL_PARAMS* - A pointer to
//                       cell parameters.
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_BEFORE_DRAW_DAYVIEWCELL = (WM_XTP_CALENDAR_BASE + 37);

//===========================================================================
// Summary:
//     Set of parameters transferred from calendar items for an
//     XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT notification.
// Remarks:
//     This structure defines day view cell (exclude all day events area) drawing
//     attributes.
// See Also: XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT
//===========================================================================
struct XTP_CALENDAR_THEME_DAYVIEWCELL_PARAMS
{
	int nIndex;		  // [in] Cell index.
	DATE dtBeginTime; // [in] Begin cell date and time.
	BOOL bSelected;   // [in] Is cell selected.
	BOOL bWorkCell;   // [in] The cell is work (or non-work).

	CXTPCalendarDayViewGroup* pViewGroup; // [in] Owner group.
};

//===========================================================================
// Summary:
//     Set of parameters transferred from calendar items for an
//     XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT notification.
// Remarks:
//     This structure defines day timescale cell drawing
//     attributes.
// See Also: XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT
//===========================================================================
struct XTP_CALENDAR_THEME_DAYVIEWTIMESCALECELL_PARAMS
{
	int nTimeScale; // [in] Time scale number {1 - main, 2 - additional}

	int nIndex;		  // [in] Cell index.
	DATE dtBeginTime; // [in] Begin cell time.

	int nMinutes; // [in] SubCell Minutes value {0, 5, 10, ...} or -1 for cell hour.
};

//===========================================================================
// Summary:
//     This enum defines a set of calendar theme items for which a
//     XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT notification is sent.
// See Also: XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT
//===========================================================================
enum XTPCalendarBeforeDrawThemeObject
{
	xtpCalendarBeforeDraw_DayViewTimeScale = 0x00000001, // dwParam2 - [in] nTimeScale = {1 - main,
														 // 2 - additional}
	xtpCalendarBeforeDraw_DayViewTimeScaleCaption = 0x00000002, // dwParam2 - [in] nTimeScale = {1 -
																// main, 2 - additional};
	xtpCalendarBeforeDraw_DayViewTimeScaleCell =
		0x00000004, // dwParam2 - [in] XTP_CALENDAR_THEME_DAYVIEWTIMESCALECELL_PARAMS*
	xtpCalendarBeforeDraw_DayViewDay	  = 0x00000010, // dwParam2 - [in] CXTPCalendarDayViewDay*
	xtpCalendarBeforeDraw_DayViewDayGroup = 0x00000020, // dwParam2 - [in] CXTPCalendarDayViewGroup*
	xtpCalendarBeforeDraw_DayViewCell	 = 0x00000040, // dwParam2 - [in]
														// XTP_CALENDAR_THEME_DAYVIEWCELL_PARAMS *
	xtpCalendarBeforeDraw_DayViewEvent = 0x00000080,	// dwParam2 - [in] CXTPCalendarDayViewEvent*

	xtpCalendarBeforeDraw_AllDayViewEvent = 0x00000100, // dwParam2 - [in] CXTPCalendarDayViewEvent*

	xtpCalendarBeforeDraw_MonthViewDay   = 0x00001000, // dwParam2 - [in] CXTPCalendarMonthViewDay*
	xtpCalendarBeforeDraw_MonthViewEvent = 0x00002000, // dwParam2 - [in]
													   // CXTPCalendarMonthViewEvent*
	xtpCalendarBeforeDraw_MonthViewWeekDayHeader = 0x00004000, // dwParam2 - [in] WeekDay = {1-Sun,
															   // 2-Mon, ...}, -1 for common part;
	xtpCalendarBeforeDraw_MonthViewWeekHeader = 0x00008000, // dwParam2 - [in] WeekIndex={0...N}, -1
															// for common part;

	xtpCalendarBeforeDraw_WeekViewDay   = 0x00100000, // dwParam2 - [in] CXTPCalendarWeekViewDay*
	xtpCalendarBeforeDraw_WeekViewEvent = 0x00200000  // dwParam2 - [in] CXTPCalendarWeekViewEvent*
};

//---------------------------------------------------------------------------
// Summary: This notification is sent for calendar theme items before draw.
//          You may customize the corresponding theme part for each object in this
//          message. Any object part changes made in this notification
//          handler will be discarded once the object draw has finished.
//          This lets you dynamically customize each calendar item.
//
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          This notification send only for theme objects defined in BeforeDrawFlags.
//          See Also: CXTPCalendarTheme::GetBeforeDrawFlags,
//                    CXTPCalendarTheme::SetBeforeDrawFlags
//          <b>Parameters</b>
//              wParam - [in] An object ID from XTPCalendarBeforeDrawThemeObject enum.
//              lParam - [in] An additional parameter defined for each item in
//                            XTPCalendarBeforeDrawThemeObject.
//
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_BEFORE_DRAW_THEMEOBJECT = (WM_XTP_CALENDAR_BASE + 38);

//===========================================================================
// Summary:
//     Set of parameters transferred from calendar items for an
//     XTP_NC_CALENDAR_GETITEMTEXT notification.
// Remarks:
//     This structure defines an calendar item, item data and in-out item text
//     to customize.
// See Also:
//     XTP_NC_CALENDAR_GETITEMTEXTL,
//     XTPCalendarGetItemText, XTPCalendarGetItemTextEx.
//===========================================================================
struct XTP_CALENDAR_GETITEMTEXT_PARAMS
{
	int nItem;		   // [in] One of the values from enums XTPCalendarGetItemText,
					   // XTPCalendarGetItemTextEx;
	CString* pstrText; // [in, out] - Pointer to item text;

	CXTPCalendarViewEvent* pViewEvent; // [in] Pointer to a ViewEvent object or NULL.
	CXTPCalendarViewDay* pViewDay;	 // [in] Pointer to a ViewDay object or NULL.

	int nWeekDay; // [in] Zero or Week day number as: 1 - Sunday, 2 - Monday, etc, 7 - Saturday.
	int nWeekNum; // [in] Zero or current visible range first week number.
};

//-----------------------------------------------------------------------
// Summary:
//     This notification is sent for calendar items which define
//     an AskItemTextFlags property.
// Remarks:
//          <b>Parameters</b>
//              wParam - [in/out] XTP_CALENDAR_GETITEMTEXT_PARAMS*.
//              lParam - 0.
// See Also:
//     XTPCalendarGetItemTextEx, XTP_CALENDAR_GETITEMTEXT_PARAMS,
//     CXTPCalendarControl::GetAskItemTextFlags,
//     CXTPCalendarPaintManager::GetAskItemTextFlags,
//     CXTPCalendarPaintManager::SetAskItemTextFlags,
//     CXTPCalendarTheme::GetAskItemTextFlags,
//     CXTPCalendarTheme::SetAskItemTextFlags
//-----------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_GETITEMTEXT = (WM_XTP_CALENDAR_BASE + 39);

//-----------------------------------------------------------------------
// Summary:
//     This enum defines a set of user actions which can be customized.
// See Also: XTP_NC_CALENDAR_USERACTION, XTP_CALENDAR_USERACTION_PARAMS
//-----------------------------------------------------------------------
enum XTPCalendarUserAction
{
	xtpCalendarUserAction_OnExpandDay = 0x80000000, // Expand Day button was clicked for month or
													// week view.
	xtpCalendarUserAction_OnScrollDay = 0x40000000, // Scroll Day button was clicked for day view.
	xtpCalendarUserAction_OnScrollAllDayEvensUp   = 0x30000000, // Scroll All Day button up action.
	xtpCalendarUserAction_OnScrollAllDayEvensDown = 0x20000000 // Scroll All Day button down action.
};

//===========================================================================
// Summary:
//     Set of parameters transferred from calendar items for an
//     XTP_NC_CALENDAR_USERACTION notification.
// See Also:
//     XTPCalendarUserAction
//===========================================================================
struct XTP_CALENDAR_USERACTION_PARAMS
{
	//=======================================================================
	// Summary:
	//     Defines parameters for xtpCalendarUserAction_OnExpandDay user action.
	//=======================================================================
	struct EXPANDDAYBUTTONACTION_PARAMS
	{
		XTPCalendarExpandDayButton nButton; // Defines Month view or Week View expand button
		CXTPCalendarViewDay* pViewDay;		// Pointer to a CXTPCalendarDayViewDay object or NULL.
	};

	//=======================================================================
	// Summary:
	//     Defines parameters for xtpCalendarUserAction_OnScrollDay user action.
	//=======================================================================
	struct SCROLLDAYBUTTONACTION_PARAMS
	{
		XTPCalendarScrollDayButton nButton;   // Defines Month view or Week View expand button
		CXTPCalendarDayViewGroup* pViewGroup; // Pointer to a CXTPCalendarDayViewGroup object or
											  // NULL.
	};

	XTPCalendarUserAction m_eAction; // A value from XTPCalendarUserAction

	// reserved for other actions in the feature
	union {
		EXPANDDAYBUTTONACTION_PARAMS m_ExpandDayButton; // Parameters for
														// xtpCalendarUserAction_OnExpandDay user
														// action.
		SCROLLDAYBUTTONACTION_PARAMS m_ScrollDayButton; // Parameters for
														// xtpCalendarUserAction_OnScrollDay user
														// action.
	};
};

//-----------------------------------------------------------------------
// Summary:
//    This notification is sent when a user action defined in XTPCalendarUserAction enum happens.
//          <b>Parameters</b>
//              wParam - [in] A pointer to XTP_CALENDAR_USERACTION_PARAMS structure.
//              lParam - [out] A pointer to the return result: BOOL*.
//                       Set TRUE if specified action is handled (to Cancel default processing) ,
//                       otherwise FALSE.
//                       Initial value is FALSE.
// See Also:
//    XTP_CALENDAR_USERACTION_PARAMS, XTPCalendarUserAction
//-----------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_USERACTION = (WM_XTP_CALENDAR_BASE + 40);

//---------------------------------------------------------------------------
// Summary: This notification is sent when the calendar is populating.
//          lParam contains events which will be drawn for the group specified
//          by wParam.
//          You may add, remove or modify some events in CXTPCalendarEvents
//          collection.
//
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          <b>Parameters</b>
//              wParam - [in] CXTPCalendarViewDay* - A pointer to View Day.
//              lParam - unused.
// See Also: CXTPCalendarControl::Populate, XTP_NC_CALENDAR_PREPOPULATE
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_PREPOPULATE_DAY = (WM_XTP_CALENDAR_BASE + 41);

//---------------------------------------------------------------------------
// Summary: This notification is sent when the calendar paint theme changes.
//          <b>Sender</b> - CXTPCalendarControl
// Remarks:
//          <b>Parameters</b>
//              wParam - unused.
//              lParam - unused.
// See Also: XTP_NC_CALENDARVIEWWASCHANGED
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_THEME_CHANGED = (WM_XTP_CALENDAR_BASE + 42);

//---------------------------------------------------------------------------
// Summary: This notification is sent on a CaptionBar part click.
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_CAPTIONBARPART_CLICK = (WM_XTP_CALENDAR_BASE + 43);

//---------------------------------------------------------------------------
// Summary: This notification is sent before a CaptionBar part click.
//---------------------------------------------------------------------------
static const XTP_NOTIFY_CODE XTP_NC_CALENDAR_CAPTIONBARPART_BEFORE_CLICK = (WM_XTP_CALENDAR_BASE
																			+ 44);

// Next Free ID: (WM_XTP_CALENDAR_BASE + 45)
/////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPCALENDARNOTIFICATIONS_H__)
