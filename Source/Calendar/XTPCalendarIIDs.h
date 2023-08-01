// DO NOT MODIFY MANUALLY
// Generated on Mon 03/06/2023 at  3:56:14.26 using cj-update-iids.cmd

// XTPCalendarIIDs.h: GUID definitions for Calendar.odl.
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

#ifndef __XTPCalendarIIDs_H__
#define __XTPCalendarIIDs_H__

// clang-format off

#define XTP_DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
       EXTERN_C __declspec(selectany) const IID XTP##name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}; \
       const unsigned long  XTP##name##__Data1   = (l);  \
       const unsigned short XTP##name##__Data2   = (w1); \
       const unsigned short XTP##name##__Data3   = (w2); \
       const unsigned char  XTP##name##__Data4_0 = (b1); \
       const unsigned char  XTP##name##__Data4_1 = (b2); \
       const unsigned char  XTP##name##__Data4_2 = (b3); \
       const unsigned char  XTP##name##__Data4_3 = (b4); \
       const unsigned char  XTP##name##__Data4_4 = (b5); \
       const unsigned char  XTP##name##__Data4_5 = (b6); \
       const unsigned char  XTP##name##__Data4_6 = (b7); \
       const unsigned char  XTP##name##__Data4_7 = (b8);

XTP_DEFINE_GUID(LIBID_XtremeCalendarControl,0x79EB16A5,0x917F,0x4145,0xAB,0x5F,0xD3,0xAE,0xA6,0x06,0x12,0xD8);
XTP_DEFINE_GUID(DIID_CalendarViewGroup,0xC0DE2210,0x778B,0x4af6,0xB9,0x20,0xAE,0x78,0x97,0x12,0xCF,0x73);
XTP_DEFINE_GUID(DIID_CalendarFlagsSet,0xC0DE2210,0x972D,0x41c0,0x93,0xDB,0x0E,0xCF,0xEB,0x85,0xD5,0x36);
XTP_DEFINE_GUID(DIID_ICalendarThemeRect,0xC0DE2210,0xF59E,0x420B,0xAF,0x82,0xA4,0xB8,0x4D,0x1D,0xC8,0x6F);
XTP_DEFINE_GUID(DIID_ICalendarThemeImageList,0xC0DE2210,0xE371,0x4690,0xAC,0xF6,0x53,0x8F,0x88,0xC3,0x37,0x41);
XTP_DEFINE_GUID(DIID_CalendarThemeDayViewCellParams,0xC0DE2210,0x83F7,0x4448,0xA6,0x41,0x50,0x67,0x6D,0xC8,0x7C,0x35);
XTP_DEFINE_GUID(DIID_CalendarThemeDayViewTimeScaleCellParams,0xC0DE2210,0x386B,0x4219,0xA3,0x4C,0x9C,0x6C,0x99,0x90,0x0B,0x85);
XTP_DEFINE_GUID(DIID_CalendarThemeColorGradient,0xC0DE2210,0x71A3,0x4549,0xA6,0x20,0xCB,0xF2,0x03,0xB4,0x54,0x78);
XTP_DEFINE_GUID(DIID_CalendarThemeFontColor,0xC0DE2210,0x8ABD,0x48E2,0xB1,0xE9,0x8D,0x82,0x85,0x9D,0x6E,0xC6);
XTP_DEFINE_GUID(DIID_CalendarThemeFormulaMulDivC,0xC0DE2210,0xB877,0x419C,0x94,0x42,0xB7,0xF8,0x22,0x82,0xA2,0xB0);
XTP_DEFINE_GUID(DIID_CalendarThemeHeaderBackground,0xC0DE2210,0x1165,0x4B99,0xAC,0xB2,0xE5,0xB4,0x72,0x10,0x8A,0x01);
XTP_DEFINE_GUID(DIID_CalendarThemeHeaderText,0xC0DE2210,0x30A0,0x4AF5,0x92,0x39,0x85,0x18,0xF1,0x34,0xFF,0x44);
XTP_DEFINE_GUID(DIID_CalendarThemeDayViewTimeScale,0xC0DE2210,0xDC4C,0x4B93,0x9A,0xA1,0x9A,0xB5,0x7D,0x42,0x0E,0xDF);
XTP_DEFINE_GUID(DIID_CalendarThemeDayViewAllDayEvents,0xC0DE2210,0x6F1D,0x4C02,0xA4,0x0B,0x40,0x61,0x10,0x63,0x58,0x42);
XTP_DEFINE_GUID(DIID_CalendarThemeDayViewCell,0xC0DE2210,0xDAFB,0x4FE2,0x9F,0x7D,0xDE,0x11,0x4E,0x7F,0x78,0x04);
XTP_DEFINE_GUID(DIID_CalendarThemeDayViewCellColors,0xC0DE2210,0x3F5C,0x4896,0xB1,0x6D,0x89,0x8F,0x1C,0xB5,0x01,0x9F);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007Header,0xC0DE2210,0x9DAB,0x4DE2,0x83,0x77,0x8D,0x98,0x92,0x1C,0x30,0x2B);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007Event,0xC0DE2210,0x3769,0x4BBF,0x83,0x29,0xDB,0xEC,0xF9,0x2C,0xBA,0xB6);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007EventFontsColors,0xC0DE2210,0xF7C0,0x4142,0xB5,0x4B,0xEB,0x64,0xA2,0x4B,0xB2,0x44);
XTP_DEFINE_GUID(DIID_CalendarThemeEventIconsToDraw,0xC0DE2210,0x8A74,0x49B8,0xB7,0x19,0x2A,0xFC,0x09,0x4A,0xD4,0xAA);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007PrevNextEventButtons,0xC0DE2210,0x3AEC,0x48C6,0x90,0x2A,0x6F,0xE3,0xBE,0x41,0xEB,0xF8);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007DayView,0xC0DE2210,0xEE51,0x4DE3,0xB8,0x20,0xD3,0x36,0xE1,0x16,0x21,0xDF);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007DayViewDay,0xC0DE2210,0x3A4B,0x4843,0x9D,0xE3,0x36,0x07,0x41,0x89,0x08,0x7B);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007DayViewDayGroup,0xC0DE2210,0x3499,0x45E1,0x84,0x13,0x2A,0xCE,0x54,0x46,0xF7,0x6F);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007DayViewEvent,0xC0DE2210,0x4B52,0x48B1,0x89,0x36,0x57,0xD5,0x6D,0x0F,0xC3,0x49);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007DayViewEventMultiDay,0xC0DE2210,0x8076,0x4C63,0xA1,0xED,0xF6,0xB2,0x9E,0x13,0x8E,0x41);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007DayViewEventSingleDay,0xC0DE2210,0x8C7A,0x43E4,0xB1,0x32,0x7E,0x68,0x70,0x17,0x4B,0x8C);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007MonthView,0xC0DE2210,0x08BB,0x409E,0x8E,0x09,0x23,0x53,0x6B,0x4F,0x3A,0xAE);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007MonthViewDay,0xC0DE2210,0x4A01,0x462F,0x92,0x12,0x92,0x3F,0x25,0xCB,0xCD,0x94);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007MonthViewEvent,0xC0DE2210,0xEB47,0x480A,0xB8,0xEA,0x73,0xD2,0x8E,0x8D,0xC9,0x44);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007MonthViewEventMultiDay,0xC0DE2210,0x7E94,0x4D71,0x8B,0xD9,0x1B,0x38,0x4B,0xF4,0xA0,0x8B);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007MonthViewEventSingleDay,0xC0DE2210,0x9BDB,0x433C,0x95,0x7E,0xA0,0xE3,0xB5,0x89,0xF0,0x84);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007MonthViewWeekHeader,0xC0DE2210,0x85B8,0x4846,0xA2,0xCB,0xA2,0x32,0x9B,0xE9,0x59,0x3C);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007WeekView,0xC0DE2210,0x71FA,0x4524,0x90,0x6C,0x1A,0x1E,0x5A,0x04,0x97,0xD4);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007WeekViewDay,0xC0DE2210,0x7F55,0x4A5B,0xBE,0x02,0x18,0xBF,0x6F,0xD4,0xD5,0x4A);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007WeekViewEvent,0xC0DE2210,0x068C,0x4F6D,0x96,0xE7,0x6B,0xA2,0x6C,0x4E,0x26,0xD5);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007WeekViewEventMultiDay,0xC0DE2210,0x913D,0x4350,0xB9,0x54,0x5A,0xDE,0x6D,0x65,0xBE,0xF6);
XTP_DEFINE_GUID(DIID_CalendarThemeOffice2007WeekViewEventSingleDay,0xC0DE2210,0x98CF,0x48B4,0x98,0xC8,0x9D,0xF4,0x7F,0x2C,0xAF,0xB1);
XTP_DEFINE_GUID(DIID_ICalendarThemeOffice2007,0xC0DE2210,0x5D67,0x4d85,0x80,0xAA,0xB5,0x44,0xF1,0xF3,0x43,0x05);
XTP_DEFINE_GUID(CLSID_CalendarThemeOffice2007,0xC0DE2210,0x53E6,0x4486,0xA9,0xA6,0x27,0x4A,0x2D,0xB3,0xB8,0x2D);
XTP_DEFINE_GUID(CLSID_CalendarThemeImageList,0xC0DE2210,0xDF5D,0x4434,0x96,0x90,0x78,0x8F,0xD4,0x71,0xCF,0x5C);
XTP_DEFINE_GUID(CLSID_CalendarThemeRect,0xC0DE2210,0xB8F4,0x4204,0xAE,0xA1,0x2A,0x25,0x94,0x49,0x36,0x56);
XTP_DEFINE_GUID(DIID__DCalendarControl,0xC0DE2210,0xAD0A,0x47AC,0x95,0xEB,0xB5,0x4A,0x21,0x8A,0x7B,0x9C);
XTP_DEFINE_GUID(DIID__DCalendarCaptionBar,0xC0DE2210,0x0C5A,0x4DEC,0x81,0x40,0x3F,0x8E,0x7F,0x3B,0x19,0x44);
XTP_DEFINE_GUID(DIID__DCalendarCaptionBarEvents,0xC0DE2210,0x1617,0x41DB,0xBF,0xFF,0x2F,0x84,0x2A,0xAF,0x02,0xDA);
XTP_DEFINE_GUID(CLSID_CalendarCaptionBar,0xC0DE2210,0xE602,0x42FE,0x94,0x1F,0xFF,0x71,0xA7,0xF0,0x85,0x3A);
XTP_DEFINE_GUID(DIID_CalendarEvent,0xC0DE2210,0x21F7,0x41CF,0x90,0x8E,0x56,0x9C,0x0B,0x11,0x78,0x5F);
XTP_DEFINE_GUID(DIID_CalendarViewDay,0xC0DE2210,0xA582,0x48D5,0x8C,0x45,0xE8,0xE6,0x5A,0x06,0x51,0x34);
XTP_DEFINE_GUID(DIID_CalendarView,0xC0DE2210,0x7D83,0x44B5,0xB8,0x2C,0x2A,0xA8,0xA8,0x3F,0x6C,0x0D);
XTP_DEFINE_GUID(DIID_CalendarEvents,0xC0DE2210,0x0FDB,0x4A59,0x94,0xD3,0xDB,0xBD,0xCE,0xDF,0xE0,0x38);
XTP_DEFINE_GUID(DIID_CalendarDataProvider,0xC0DE2210,0xA643,0x4376,0xB1,0x1D,0x2B,0x36,0x31,0x76,0x4C,0xD5);
XTP_DEFINE_GUID(DIID_CalendarCustomProperties,0xC0DE2210,0x11DA,0x4a0c,0xBB,0x5F,0xDA,0x28,0x32,0xFC,0x22,0x72);
XTP_DEFINE_GUID(DIID_ICalendarResources,0xC0DE2210,0x4205,0x4f0a,0xB8,0x33,0xD4,0x92,0x01,0xDF,0x22,0x83);
XTP_DEFINE_GUID(DIID_CalendarViewEvent,0xC0DE2210,0x3767,0x4DDC,0x95,0xCC,0x98,0x64,0x98,0x2A,0x7B,0x34);
XTP_DEFINE_GUID(DIID_CalendarViewEvents,0xC0DE2210,0x69D9,0x4b45,0xA4,0x87,0x4F,0x74,0xFC,0x46,0xCC,0x65);
XTP_DEFINE_GUID(DIID_CalendarViewGroups,0xC0DE2210,0xE33A,0x4622,0x95,0x19,0x65,0xCA,0x98,0xD4,0xB5,0x70);
XTP_DEFINE_GUID(DIID_CalendarHitTestInfo,0xC0DE2210,0x344D,0x4925,0xBC,0x7E,0x69,0x0D,0xDD,0xAF,0x11,0x96);
XTP_DEFINE_GUID(DIID_CalendarSystemTime,0xC0DE2210,0x591D,0x4490,0x9A,0xA3,0xBB,0x3E,0xDC,0x27,0x4E,0x29);
XTP_DEFINE_GUID(DIID_CalendarTimeZone,0xC0DE2210,0x4857,0x425e,0xA8,0x64,0x00,0x90,0x9C,0xD0,0xB7,0xBF);
XTP_DEFINE_GUID(DIID_CalendarTimeZones,0xC0DE2210,0x7AAE,0x4cd8,0x97,0x8A,0x15,0xF8,0x61,0xE5,0x2D,0x14);
XTP_DEFINE_GUID(DIID_CalendarEditOperationParameters,0xC0DE2210,0xCC9B,0x4DF0,0xB3,0x5C,0xD6,0x3A,0x2F,0x75,0xEC,0x32);
XTP_DEFINE_GUID(DIID_CalendarSchedule,0xC0DE2210,0xA609,0x48c5,0xBF,0x08,0xC4,0xFC,0xE7,0xF4,0x2D,0xD2);
XTP_DEFINE_GUID(DIID_CalendarSchedules,0xC0DE2210,0x6EDB,0x4518,0xA2,0x46,0x48,0xAC,0x5E,0x69,0xFB,0x7E);
XTP_DEFINE_GUID(DIID_CalendarScheduleIDs,0xC0DE2210,0x1151,0x42ef,0x87,0xEF,0xBF,0x37,0xE2,0xA1,0x94,0x83);
XTP_DEFINE_GUID(DIID_ICalendarResource,0xC0DE2210,0x4E72,0x468c,0x96,0xA8,0xC0,0x5F,0x96,0xED,0x71,0x3A);
XTP_DEFINE_GUID(CLSID_CalendarResource,0xC0DE2210,0x3FA6,0x4067,0x93,0xEA,0x6E,0xC6,0x7A,0x64,0x66,0xD3);
XTP_DEFINE_GUID(CLSID_CalendarResources,0xC0DE2210,0xB13E,0x400B,0x93,0xA8,0xC3,0x46,0x1B,0x03,0xE5,0x20);
XTP_DEFINE_GUID(DIID_CalendarResourceDescription,0xC0DE2210,0x0E47,0x4e84,0x8C,0x33,0xFE,0x96,0x4E,0xA1,0x58,0x6E);
XTP_DEFINE_GUID(DIID_ICalendarResourcesManager,0xC0DE2210,0x8B53,0x48d1,0x9C,0x49,0xE0,0x83,0x59,0xC4,0x78,0xDC);
XTP_DEFINE_GUID(CLSID_CalendarResourcesManager,0xC0DE2210,0x6091,0x4CE5,0xBB,0x25,0x19,0xF7,0xBA,0xB4,0xF9,0x60);
XTP_DEFINE_GUID(DIID_CalendarViewSelection,0xC0DE2210,0xCFC8,0x4ea9,0x8E,0x6E,0xFF,0xC1,0x9A,0x43,0x7A,0x44);
XTP_DEFINE_GUID(DIID_CalendarDayViewCellParams,0xC0DE2210,0x1E84,0x4107,0x9E,0xA1,0x70,0xBF,0xCC,0xE2,0xDC,0x2E);
XTP_DEFINE_GUID(DIID_CalendarGetItemTextParams,0xC0DE2210,0xDA6C,0x4012,0x8C,0x88,0x01,0x60,0x40,0x35,0x54,0xC6);
XTP_DEFINE_GUID(DIID_CalendarDayView,0xC0DE2210,0x3914,0x44B0,0xAE,0x00,0x72,0x10,0x61,0x1D,0xEA,0xE2);
XTP_DEFINE_GUID(DIID_CalendarWeekView,0xC0DE2210,0x6BA7,0x4DC1,0xA3,0x9E,0x00,0x89,0x12,0x51,0xE4,0xC7);
XTP_DEFINE_GUID(DIID_CalendarMonthView,0xC0DE2210,0x0650,0x449F,0x8A,0x20,0x0C,0x5F,0x41,0x0B,0xA7,0xC3);
XTP_DEFINE_GUID(DIID_CalendarTimeLineView,0xC0DE2210,0x4CFE,0x4f7a,0x8E,0xBC,0x13,0x13,0x5C,0x0F,0xAF,0x4F);
XTP_DEFINE_GUID(DIID_CalendarEventLabel,0xC0DE2210,0x2327,0x49f0,0x94,0x7A,0x2B,0x36,0xFA,0xEC,0x73,0x0A);
XTP_DEFINE_GUID(DIID_CalendarEventLabels,0xC0DE2210,0x5C8D,0x47ee,0xBE,0x61,0xFC,0x09,0xC5,0x6C,0xDD,0x3A);
XTP_DEFINE_GUID(DIID_CalendarIconIDs,0xC0DE2210,0x6DD5,0x4f39,0xA0,0x3B,0xFD,0x05,0xED,0x5A,0xE4,0x41);
XTP_DEFINE_GUID(DIID_ICalendarEventCategory,0xC0DE2210,0xDC14,0x4a33,0x98,0x31,0x5E,0x81,0x59,0xFB,0xF3,0xFA);
XTP_DEFINE_GUID(CLSID_CalendarEventCategory,0xC0DE2210,0x2F70,0x4F66,0xBA,0xD7,0x22,0x41,0x80,0x73,0xF3,0x5F);
XTP_DEFINE_GUID(DIID_ICalendarEventCategories,0xC0DE2210,0xDDFB,0x4f5e,0x8F,0xD8,0xA3,0xA1,0xC2,0xC0,0xD1,0x3D);
XTP_DEFINE_GUID(CLSID_CalendarEventCategories,0xC0DE2210,0xF8A9,0x4BA6,0x9A,0x90,0xA4,0xF7,0xAA,0x2E,0xD1,0x91);
XTP_DEFINE_GUID(DIID_CalendarEventCategoryIDs,0xC0DE2210,0xE8F3,0x4ed6,0x99,0x5A,0x75,0x0D,0x7C,0x4C,0x1F,0xDE);
XTP_DEFINE_GUID(DIID_CalendarRecurrencePatternOptions,0xC0DE2210,0x84D0,0x43AB,0xB1,0x4D,0xCA,0x1D,0x81,0x51,0x12,0x1F);
XTP_DEFINE_GUID(DIID_CalendarRecurrencePattern,0xC0DE2210,0x4840,0x41B5,0x94,0xA1,0x7B,0x52,0xC6,0x5B,0x19,0x07);
XTP_DEFINE_GUID(DIID_CalendarOptions,0xC0DE2210,0xD641,0x4FF5,0xAE,0xC9,0x50,0x73,0xCA,0xBE,0x48,0x08);
XTP_DEFINE_GUID(DIID_CalendarReminder,0xC0DE2210,0x3036,0x4c8c,0x9F,0x49,0xE1,0xBC,0xA9,0xE5,0xDB,0x14);
XTP_DEFINE_GUID(DIID_CalendarReminders,0xC0DE2210,0xBACB,0x441a,0x9D,0x37,0xE1,0xB0,0x89,0xFB,0x04,0x0C);
XTP_DEFINE_GUID(DIID_CalendarPrintOptions,0xC0DE2210,0x26C8,0x40cc,0x8B,0x85,0x45,0x39,0x09,0x8E,0x18,0xA1);
XTP_DEFINE_GUID(DIID_CalendarUserAction,0xC0DE2210,0x6103,0x4759,0x95,0x5D,0x8E,0x83,0x75,0x50,0xA5,0xE1);
XTP_DEFINE_GUID(DIID_ICalendarDialogs,0xC0DE2210,0x6F7A,0x459A,0x8C,0xFF,0xC0,0x95,0x6A,0xBA,0xA4,0xA8);
XTP_DEFINE_GUID(CLSID_CalendarDialogs,0xC0DE2210,0x30E7,0x4833,0x91,0xF5,0x6F,0xC9,0xA5,0x42,0x81,0xD4);
XTP_DEFINE_GUID(DIID__DCalendarControlEvents,0xC0DE2210,0xC1C3,0x4645,0xB6,0x40,0x65,0x12,0x1C,0xD9,0xDA,0x4C);
XTP_DEFINE_GUID(CLSID_CalendarControl,0xC0DE2210,0x0EF8,0x4AB4,0xAF,0x68,0x4E,0x7C,0x7D,0x4E,0x53,0xF9);
XTP_DEFINE_GUID(DIID_DatePickerSelectionBlock,0xC0DE2210,0xEF34,0x4631,0xB8,0xA5,0x7A,0xDC,0x20,0x0A,0x34,0xD8);
XTP_DEFINE_GUID(DIID_DatePickerSelection,0xC0DE2210,0x32B8,0x485A,0x9D,0x2E,0x9B,0x7C,0x10,0xBE,0x94,0x73);
XTP_DEFINE_GUID(DIID_IDatePickerDayMetrics,0xC0DE2210,0x40BE,0x4ce2,0xBE,0x64,0xAA,0x74,0x3B,0xBE,0x44,0xBD);
XTP_DEFINE_GUID(CLSID_DatePickerDayMetrics,0xC0DE2210,0x7084,0x45f3,0xA4,0x35,0xE9,0xED,0xD7,0x57,0x7B,0x9B);
XTP_DEFINE_GUID(DIID_DatePickerPaintManager,0xC0DE2210,0x3FC8,0x4cbb,0xB8,0xD6,0xA8,0x4B,0xDE,0x8A,0xB5,0x67);
XTP_DEFINE_GUID(DIID__DDatePicker,0xC0DE2210,0x23F2,0x4a5c,0xAC,0xB7,0xFF,0x70,0x58,0xB5,0x2F,0x63);
XTP_DEFINE_GUID(DIID__DDatePickerEvents,0xC0DE2210,0x896D,0x4B48,0xAB,0x33,0x0D,0xDE,0x41,0x81,0x5A,0x1B);
XTP_DEFINE_GUID(CLSID_DatePicker,0xC0DE2210,0x5809,0x4D82,0x92,0x89,0x9A,0x31,0xFB,0xF7,0x11,0x34);
XTP_DEFINE_GUID(DIID_ICalendarGlobalSettings,0xC0DE2210,0x282E,0x40C7,0xBC,0x25,0x85,0x08,0xA7,0x1C,0xD4,0x79);
XTP_DEFINE_GUID(CLSID_CalendarGlobalSettings,0xC0DE2210,0xBECB,0x4A1B,0x88,0xC0,0x2B,0xCF,0x8C,0x0A,0x98,0x90);

#undef XTP_DEFINE_GUID

// clang-format on

#endif // __XTPCalendarIIDs_H__
