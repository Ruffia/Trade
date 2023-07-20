// XTPCalendarTimeLineViewTimeScale.h: interface for the
// CXTPCalendarTimeLineViewTimeScale class.
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
#ifndef _XTPCALENDARTIMELINEVIEWTIMESCALE_H__
#define _XTPCALENDARTIMELINEVIEWTIMESCALE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

struct XTPCALENDARHITTESTINFO_TIMELINE_VIEW;
class CXTPCalendarTimeLineView;
class CXTPCalendarTLV_TimeScaleParams;
class CXTPCalendarTimeLineViewTimeScalePart;

//============================================================================
// Summary:
//     Predefined set of different scales based on time intervals.
//============================================================================
enum XTPEnumCalendarDateItem
{
	xtpDTScale_Unknown = 0, // Typed unknown value.

	xtpDTScale_Sec		= 0x0001, // Sec scale.
	xtpDTScale_Min		= 0x0002, // Min scale.
	xtpDTScale_Hour		= 0x0004, // Hour scale.
	xtpDTScale_Day		= 0x0010, // Day scale.
	xtpDTScale_WorkWeek = 0x0015, // WorkWeek scale.
	xtpDTScale_Week		= 0x0020, // Week scale.
	xtpDTScale_Month	= 0x0040, // Month scale.
	xtpDTScale_Year		= 0x0080, // Year scale.
};

//============================================================================
// Summary:
//     Timeline's view scale.
//============================================================================
struct XTP_CALENDAR_TIMELINE_VIEW_SCALE
{
	int m_eType;  // One of the values from enum XTPEnumCalendarDateItem.
	int m_nValue; // Number of days to show (e.g. 1, 5, 7, 31).
};

//============================================================================
// Summary:
//     This class implements a specific view portion of the Calendar
//     control called Timeline view scale.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewTimeScale : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pView - Pointer to the associated CXTPCalendarTimeLineView object.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineViewTimeScale(CXTPCalendarTimeLineView* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTimeLineViewTimeScale();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the owner view object.
	// Returns:
	//     A CXTPCalendarTimeLineView pointer to the owner view object.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineView* GetView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get time scale params.
	// Returns:
	//     Pointer to a CXTPCalendarTLV_TimeScaleParams object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTLV_TimeScaleParams* GetTimeScaleParams() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to set time scale params
	//     from the predefined set XTPEnumCalendarTimeScaleParamsID.
	// Parameters:
	//     eTimeScaleParamsID - One of the time scale parameters defined by
	//                          XTPEnumCalendarTimeScaleParamsID.
	//-----------------------------------------------------------------------
	virtual void SetTimeScaleParams(int eTimeScaleParamsID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to set time scale params
	//     from another CXTPCalendarTLV_TimeScaleParams object.
	// Parameters:
	//     pSParams - Pointer to a CXTPCalendarTLV_TimeScaleParams object.
	//-----------------------------------------------------------------------
	virtual void SetTimeScaleParams2(CXTPCalendarTLV_TimeScaleParams* pSParams);

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get start view date value.
	// Returns:
	//     Start view date value as a COleDateTime object.
	//-----------------------------------------------------------------------
	virtual COleDateTime GetStartViewDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to set start view date value.
	// Parameters:
	//     dt - New start view date value.
	//-----------------------------------------------------------------------
	virtual void SetStartViewDate(COleDateTime dt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust time scale layout
	//     depending on the provided parent rectangle.
	// Parameters:
	//     rcParentRect - Provided parent rectangle.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CRect rcParentRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get client rectangle according
	//     to passed vertical start position.
	// Parameters:
	//     nYstart - Vertical start position.
	// Returns:
	//     Client rectangle as a CRect object.
	//-----------------------------------------------------------------------
	virtual CRect GetClientRect(int nYstart = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get time scale rectangle.
	// Returns:
	//     Time scale rectangle as CRect object.
	//-----------------------------------------------------------------------
	virtual CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the time scale
	//     for the specified device context.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns additional
	//     info in an XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point to
	//                test.
	//     pHitTest - A pointer to an XTP_CALENDAR_HITTESTINFO structure that
	//                contains information about the point to test.
	// Returns:
	//     A BOOL. TRUE if the item is found, otherwise FALSE.
	// See Also:
	//     XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	CRect m_rcScale; // internal member to keep time scale rectangle

protected:
	CXTPCalendarTimeLineView* m_pParentView; // Internal pointer to parent view.
	COleDateTime m_dtStartViewDate;			 // Internal datetime object to keep start view date.
	CRect m_rcRect;							 // Internal member used for layout adjustment.
	CXTPCalendarTLV_TimeScaleParams* m_pTimeScaleParams; // Internal pointer to
														 // CXTPCalendarTLV_TimeScaleParams.
};

//============================================================================
// Summary:
//     This class implements Timeline scale params.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams : public CXTPCmdTarget
{
	friend class CXTPCalendarTimeLineViewTimeScale;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     nTimeScaleParamsID - Passed new params ID.
	//     pTimeScale - Pointer to a CXTPCalendarTimeLineViewTimeScale object.
	//-----------------------------------------------------------------------
	CXTPCalendarTLV_TimeScaleParams(int nTimeScaleParamsID,
									CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams();

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get the current parent time scale object.
	// Returns:
	//     Pointer to the current parent time scale object.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineViewTimeScale* GetParentTimeScale() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get the current time scale params ID.
	// Returns:
	//     An int that contains the current time scale params ID.
	//-----------------------------------------------------------------------
	virtual int GetTimeScaleParamsID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get the current time scale interval.
	// Returns:
	//     Interval as an XTP_CALENDAR_TIMELINE_VIEW_SCALE object.
	//-----------------------------------------------------------------------
	virtual XTP_CALENDAR_TIMELINE_VIEW_SCALE GetTimeLineViewScale() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get the current time scale interval.
	// Returns:
	//     Interval as a COleDateTimeSpan object.
	//-----------------------------------------------------------------------
	virtual COleDateTimeSpan GetScaleInterval() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get the current time scale inteval width.
	// Returns:
	//     An int that contains the current time scale inteval width.
	//-----------------------------------------------------------------------
	virtual int GetScaleIntervalWidth_px() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Access function to get current time per pixel value.
	// Returns:
	//     Current time per pixel value as a COleDateTimeSpan object.
	//-----------------------------------------------------------------------
	virtual COleDateTimeSpan GetTimePerPixel() const;

	//-----------------------------------------------------------------------
	//  Summary:
	//     Used to calculate scale interval initial position.
	//  Parameters:
	//     dtStart - start time as a COleDateTime object.
	//  Returns:
	//     Scale interval initial position as a COleDateTime object.
	//-----------------------------------------------------------------------
	virtual COleDateTime CalcScaleIntervalBegin(COleDateTime dtStart);

	//-----------------------------------------------------------------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart*
	//     CXTPCalendarTLV_TimeScaleParams_WorkWeek::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     A pointer to a CXTPCalendarTimeLineViewTimeScalePart object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const = 0;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the time scale interval m_scaleInterval.
	// Parameters:
	//     eType - Passed to m_eType.
	//     nValue - Passed to m_nValue.
	//-----------------------------------------------------------------------
	void SetScaleInterval(int eType, int nValue);

	XTP_CALENDAR_TIMELINE_VIEW_SCALE m_scaleInterval; // Internal instance with m_eType and
													  // m_nValue.

	int m_nScaleIntervalWidth_px; // Internal value for scale interval (by x).

private:
	CXTPCalendarTimeLineViewTimeScale* m_pTimeScale; // Pointer to time scale object.
	int m_nTimeScaleParamsID; // Internal value of current time scale param ID.
};

//===========================================================================
// Summary:
//     This class implements Timeline scale params for Day case.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_Day : public CXTPCalendarTLV_TimeScaleParams
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - Pointer to a CXTPCalendarTimeLineViewTimeScale object.
	//-----------------------------------------------------------------------
	CXTPCalendarTLV_TimeScaleParams_Day(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_Day();

	//-----------------------------------------------------------------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart*
	//     CXTPCalendarTLV_TimeScaleParams_WorkWeek::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     A pointer to a CXTPCalendarTimeLineViewTimeScalePart object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;
};

//============================================================================
// Summary:
//     This class implements Timeline scale params for Week case.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_Week : public CXTPCalendarTLV_TimeScaleParams
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - Pointer to a CXTPCalendarTimeLineViewTimeScale object.
	//-----------------------------------------------------------------------
	CXTPCalendarTLV_TimeScaleParams_Week(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_Week();

	//-----------------------------------------------------------------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart*
	//     CXTPCalendarTLV_TimeScaleParams_WorkWeek::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     A pointer to a CXTPCalendarTimeLineViewTimeScalePart object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;
};

//===========================================================================
// Summary:
//     This class implements Timeline scale params for WorkWeek case.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_WorkWeek
	: public CXTPCalendarTLV_TimeScaleParams
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - Pointer to a CXTPCalendarTimeLineViewTimeScale object.
	//-----------------------------------------------------------------------
	CXTPCalendarTLV_TimeScaleParams_WorkWeek(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_WorkWeek();

	//-----------------------------------------------------------------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart*
	//     CXTPCalendarTLV_TimeScaleParams_WorkWeek::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     A pointer to a CXTPCalendarTimeLineViewTimeScalePart object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;
};

//===========================================================================
// Summary:
//     This class implements Timeline scale params for Month case.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_Month : public CXTPCalendarTLV_TimeScaleParams
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - Pointer to a CXTPCalendarTimeLineViewTimeScale object.
	//-----------------------------------------------------------------------
	CXTPCalendarTLV_TimeScaleParams_Month(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_Month();

	//-----------------------------------------------------------------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     A pointer to a CXTPCalendarTimeLineViewTimeScalePart object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;

	// virtual CString FormatLowerScaleLabel(COleDateTime dtDateTime);
	// virtual void DrawLowerScale(CDC* pDC, DATE dtStart, const CRect& rcRect);
};

class _XTP_EXT_CLASS CXTPCalendarTimeLineViewTimeScaleGrid
{
public:
	CXTPCalendarTimeLineViewTimeScaleGrid(CXTPCalendarTimeLineView* pView);
	virtual ~CXTPCalendarTimeLineViewTimeScaleGrid();

	void AdjustLayout(CRect rcParentRect);
	CRect GetRect() const;
	void Draw(CDC* pDC);

protected:
	CXTPCalendarTimeLineView* m_pParentView; // Internal pointer to parent view.
	CRect m_rcRect;
};

AFX_INLINE CXTPCalendarTimeLineView* CXTPCalendarTimeLineViewTimeScale::GetView() const
{
	return m_pParentView;
}
AFX_INLINE CXTPCalendarTLV_TimeScaleParams*
	CXTPCalendarTimeLineViewTimeScale::GetTimeScaleParams() const
{
	return m_pTimeScaleParams;
}
AFX_INLINE COleDateTime CXTPCalendarTimeLineViewTimeScale::GetStartViewDate() const
{
	return m_dtStartViewDate;
}
AFX_INLINE void CXTPCalendarTimeLineViewTimeScale::SetStartViewDate(COleDateTime dt)
{
	m_dtStartViewDate = dt;
}
AFX_INLINE CRect CXTPCalendarTimeLineViewTimeScale::GetRect() const
{
	return m_rcRect;
}
AFX_INLINE CXTPCalendarTimeLineViewTimeScale*
	CXTPCalendarTLV_TimeScaleParams::GetParentTimeScale() const
{
	return m_pTimeScale;
}
AFX_INLINE XTP_CALENDAR_TIMELINE_VIEW_SCALE
	CXTPCalendarTLV_TimeScaleParams::GetTimeLineViewScale() const
{
	return m_scaleInterval;
}
AFX_INLINE int CXTPCalendarTLV_TimeScaleParams::GetScaleIntervalWidth_px() const
{
	return m_nScaleIntervalWidth_px;
}
AFX_INLINE int CXTPCalendarTLV_TimeScaleParams::GetTimeScaleParamsID() const
{
	return m_nTimeScaleParamsID;
}

//============================================================================

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // (_XTPCALENDARTIMELINEVIEWTIMESCALE_H__)
