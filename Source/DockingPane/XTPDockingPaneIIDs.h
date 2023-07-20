// DO NOT MODIFY MANUALLY
// Generated on Mon 03/06/2023 at  3:56:22.79 using cj-update-iids.cmd

// XTPDockingPaneIIDs.h: GUID definitions for DockingPane.odl.
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

#ifndef __XTPDockingPaneIIDs_H__
#define __XTPDockingPaneIIDs_H__

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

XTP_DEFINE_GUID(LIBID_XtremeDockingPane,0x945E8FCC,0x830E,0x45CC,0xAF,0x00,0xA0,0x12,0xD5,0xAE,0x74,0x51);
XTP_DEFINE_GUID(DIID_IPane,0xC0DE2210,0x61BC,0x4172,0x89,0x3C,0xCD,0x34,0x63,0xCF,0x2E,0x01);
XTP_DEFINE_GUID(DIID_IPaneContainer,0xC0DE2210,0x2E69,0x4810,0xB1,0xEB,0xC9,0xD7,0x51,0x12,0xEC,0xC5);
XTP_DEFINE_GUID(DIID_IDockingPaneLayout,0xC0DE2210,0xB860,0x4B3D,0xB4,0xC5,0x11,0x5A,0xD6,0x9A,0x82,0x35);
XTP_DEFINE_GUID(CLSID_DockingPaneLayout,0xC0DE2210,0x93E7,0x42CA,0x9F,0xFD,0x73,0x48,0x32,0x8F,0xDD,0xF0);
XTP_DEFINE_GUID(DIID_IDockingPaneTrackSize,0xC0DE2210,0x2228,0x4D03,0xA6,0xB5,0xE0,0x20,0x4C,0xB3,0x1D,0xC3);
XTP_DEFINE_GUID(CLSID_DockingPaneTrackSize,0xC0DE2210,0x6804,0x4153,0xB4,0x5E,0xE3,0x49,0x37,0x1F,0xFB,0x3E);
XTP_DEFINE_GUID(CLSID_PaneContainer,0xC0DE2210,0x310C,0x42A2,0x8E,0x33,0x41,0x27,0xCA,0xB9,0x75,0x9E);
XTP_DEFINE_GUID(CLSID_Pane,0xC0DE2210,0x0D8F,0x451D,0x8C,0x74,0x36,0xD4,0xAB,0xB6,0x4E,0x5E);
XTP_DEFINE_GUID(DIID_IDockingPanePaintManager,0xC0DE2210,0x5B83,0x4371,0xAA,0x36,0xE4,0x5E,0x7C,0xD8,0x6B,0x3B);
XTP_DEFINE_GUID(CLSID_DockingPanePaintManager,0xC0DE2210,0xFA19,0x43FC,0x84,0xF3,0x45,0x63,0x4D,0x1E,0x79,0x0D);
XTP_DEFINE_GUID(DIID_IDockingPaneOptions,0xC0DE2210,0xE16F,0x4EE6,0x9C,0x6C,0xF8,0x97,0xF2,0xB7,0x06,0xAA);
XTP_DEFINE_GUID(CLSID_DockingPaneOptions,0xC0DE2210,0x065B,0x43BF,0x81,0x75,0x26,0x6F,0x50,0xEA,0xC4,0xB1);
XTP_DEFINE_GUID(DIID_IPaneActionContainer,0xC0DE2210,0x20ED,0x4FEB,0xA0,0xC0,0x04,0x2A,0x23,0xF3,0x87,0xB2);
XTP_DEFINE_GUID(CLSID_PaneActionContainer,0xC0DE2210,0x5773,0x4E88,0x86,0xD6,0x09,0x12,0x8D,0xE3,0xDF,0x2C);
XTP_DEFINE_GUID(DIID__DDockingPane,0xC0DE2210,0xE939,0x4555,0xB3,0xF6,0xFA,0xD1,0xE6,0x3C,0x10,0xE4);
XTP_DEFINE_GUID(DIID__DDockingPaneEvents,0xC0DE2210,0xD406,0x4B24,0x8B,0x76,0xA0,0x48,0xA2,0x31,0xF2,0xB1);
XTP_DEFINE_GUID(CLSID_DockingPane,0xC0DE2210,0xD9CB,0x4B48,0x82,0xFB,0x38,0xA5,0x7D,0x81,0xF4,0x8A);
XTP_DEFINE_GUID(DIID__DDockingPaneFrame,0xC0DE2210,0x4947,0x4D1A,0xB4,0xBB,0x5D,0xEC,0x96,0x5C,0x17,0xA7);
XTP_DEFINE_GUID(DIID__DDockingPaneFrameEvents,0xC0DE2210,0xB0A3,0x4DB3,0xA9,0x51,0x8E,0x52,0xDD,0xE8,0x88,0xA7);
XTP_DEFINE_GUID(CLSID_DockingPaneFrame,0xC0DE2210,0x1D63,0x41A5,0x85,0x2A,0x34,0xFF,0x6C,0x9E,0xA7,0xA9);
XTP_DEFINE_GUID(DIID_IDockingPaneGlobalSettings,0xC0DE2210,0xF12D,0x4505,0xA0,0x24,0xBE,0xD9,0xCF,0xE5,0x8E,0x50);
XTP_DEFINE_GUID(CLSID_DockingPaneGlobalSettings,0xC0DE2210,0x1C32,0x4FCF,0xAD,0x92,0x0A,0xBC,0x82,0x02,0xDD,0xD9);

#undef XTP_DEFINE_GUID

// clang-format on

#endif // __XTPDockingPaneIIDs_H__

