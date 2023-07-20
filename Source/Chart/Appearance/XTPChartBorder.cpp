// XTPChartBorder.cpp
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPTypeId.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartBorder

CXTPChartBorder::CXTPChartBorder(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

	m_bVisible   = TRUE;
	m_clrColor   = CXTPChartColor::Empty;
	m_nThickness = 1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartBorder::~CXTPChartBorder()
{
}

CXTPChartDeviceCommand* CXTPChartBorder::CreateDeviceCommand(const CXTPChartRectF& rcBounds,
															 const CXTPChartColor& color)
{
	if (m_bVisible && m_nThickness > 0)
	{
		return CXTPChartDrawingObjectFactory::Create2dBoundedRectangleDeviceCommand(rcBounds, color,
																					m_nThickness);
	}

	return NULL;
}

CXTPChartDeviceCommand* CXTPChartBorder::CreateInnerBorderDeviceCommand(
	const CXTPChartRectF& rcBounds, const CXTPChartColor& color)
{
	if (m_bVisible && m_nThickness > 0)
	{
		return CXTPChartDrawingObjectFactory::Create2dInnerBorderDeviceCommand(rcBounds, color,
																			   m_nThickness);
	}

	return NULL;
}

void CXTPChartBorder::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Int(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Color(pPX, _T("Color"), m_clrColor);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartBorder, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartBorder, "Thickness", 1, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartBorder, "Visible", 2, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartBorder, "Color", 3, OleGetColor, OleSetColor, VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartBorder, CXTPCmdTarget)
	INTERFACE_PART(CXTPChartBorder, XTPDIID_ChartBorder, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBorder, XTPDIID_ChartBorder)

OLE_COLOR CXTPChartBorder::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

void CXTPChartBorder::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

#endif
