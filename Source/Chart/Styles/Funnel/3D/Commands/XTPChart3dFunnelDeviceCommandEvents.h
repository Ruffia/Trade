// XTPChart3dFunnelDeviceCommandEvents.h
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
#if !defined(__XTPCHART3DFUNNELDEVICECOMMANDEVENTS_H__)
#	define __XTPCHART3DFUNNELDEVICECOMMANDEVENTS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDeviceCommand;

//-----------------------------------------------------------------------
// Summary:
//      Defines interface for 3D chart funnel device command event sink.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPChart3dFunnelDeviceCommandEvents
	: public CXTPChartDeviceCommandEvents
	, public CXTPTypeIdProvider<CXTPChart3dFunnelDeviceCommandEvents, CXTPChartDeviceCommandEvents>
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs a CXTPChart3dFunnelDeviceCommandEvents object.
	//-----------------------------------------------------------------------
	CXTPChart3dFunnelDeviceCommandEvents();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Gets called when funnel connector points are computed.
	// Parameters:
	//      pCommand - The sender command.
	//      ptShapeCenter - The coordinates of the physical shape center.
	//      ptOuterCenter - The coordinates of the most outer horizontal shape center point.
	//-----------------------------------------------------------------------
	virtual void OnSetConnectorPoints(CXTPChartDeviceCommand* pCommand, CXTPPoint3d ptShapeCenter,
									  CXTPPoint3d ptOuterCenter);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DFUNNELDEVICECOMMANDEVENTS_H__)
