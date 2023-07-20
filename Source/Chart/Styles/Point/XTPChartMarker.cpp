// XTPChartMarker.cpp
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartCircleDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Styles/Point/XTPChartMarker.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPChartMarker, CXTPChartElement)

CXTPChartMarker::CXTPChartMarker(CXTPChartSeriesStyle* pOwner)
{
	m_pOwner = pOwner;

	m_nSize	= 8;
	m_bVisible = TRUE;

	m_nType			 = xtpChartMarkerCircle;
	m_clrColor		 = CXTPChartColor::Empty;
	m_clrBorderColor = CXTPChartColor::Empty;

	m_bBorderVisible = TRUE;

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle315);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartMarker::~CXTPChartMarker()
{
	SAFE_RELEASE(m_pFillStyle);
}

CXTPChartDeviceCommand* CXTPChartMarker::CreateDeviceCommand(CXTPChartDeviceContext* pDC,
															 const CXTPChartPointF& point,
															 CXTPChartColor color,
															 CXTPChartColor color2,
															 CXTPChartColor colorBorder)
{
	return CreateDeviceCommand(pDC, point, m_nSize, color, color2, colorBorder);
}

CXTPChartDeviceCommand* CXTPChartMarker::CreateDeviceCommand(CXTPChartDeviceContext* pDC,
															 const CXTPChartPointF& point,
															 int nWidth, CXTPChartColor color,
															 CXTPChartColor color2,
															 CXTPChartColor colorBorder)
{
	if (!m_bVisible)
		return NULL;

	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	if (m_nType == xtpChartMarkerCircle)
	{
		pCommand->AddChildCommand(
			m_pFillStyle->CreateCircleDeviceCommand(point, nWidth / 2, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedCircleDeviceCommand(point, nWidth / 2,
																				  colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerSquare)
	{
		CXTPChartPoints arrPoints;

		arrPoints.Add(CXTPChartPointF(point.X - static_cast<float>(nWidth) / 2,
									  point.Y - static_cast<float>(nWidth) / 2));
		arrPoints.Add(CXTPChartPointF(point.X + static_cast<float>(nWidth) / 2,
									  point.Y - static_cast<float>(nWidth) / 2));
		arrPoints.Add(CXTPChartPointF(point.X + static_cast<float>(nWidth) / 2,
									  point.Y + static_cast<float>(nWidth) / 2));
		arrPoints.Add(CXTPChartPointF(point.X - static_cast<float>(nWidth) / 2,
									  point.Y + static_cast<float>(nWidth) / 2));

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints,
																				   colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerDiamond)
	{
		CXTPChartPoints arrPoints;

		float d = static_cast<float>(nWidth) / (float)sqrt(2.0) - 1;

		arrPoints.Add(CXTPChartPointF(point.X - d, point.Y));
		arrPoints.Add(CXTPChartPointF(point.X, point.Y - d));
		arrPoints.Add(CXTPChartPointF(point.X + d, point.Y));
		arrPoints.Add(CXTPChartPointF(point.X, point.Y + d));

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints,
																				   colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerTriangle || m_nType == xtpChartMarkerPentagon
			 || m_nType == xtpChartMarkerHexagon)
	{
		int nSide = m_nType == xtpChartMarkerTriangle ? 3
													  : m_nType == xtpChartMarkerPentagon ? 5 : 6;

		CXTPChartPoints arrPoints;
		double d		= static_cast<float>(nWidth) / (float)sqrt(2.0) - 1;
		const double PI = acos(-1.0);

		for (int i = 0; i < nSide; i++)
		{
			arrPoints.Add(CXTPChartPointF(point.X + d * cos(PI / 2 + i * 2 * PI / nSide),
										  point.Y - d * sin(PI / 2 + i * 2 * PI / nSide)));
		}

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints,
																				   colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerStar)
	{
		int nSide = 5;

		CXTPChartPoints arrPoints;
		double d		= static_cast<float>(nWidth) / (float)sqrt(2.0) - 1;
		const double PI = acos(-1.0);

		double angle = 2 * PI / nSide;

		for (int i = 0; i < nSide; i++)
		{
			arrPoints.Add(CXTPChartPointF(point.X + d * cos(PI / 2 + i * angle),
										  point.Y - d * sin(PI / 2 + i * angle)));

			arrPoints.Add(CXTPChartPointF(point.X + d / 2 * cos(PI / 2 + i * angle + angle / 2),
										  point.Y - d / 2 * sin(PI / 2 + i * angle + angle / 2)));
		}

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints,
																				   colorBorder, 1));
		}
	}
	return pCommand;
}

void CXTPChartMarker::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("BorderVisible"), m_bBorderVisible, TRUE);
	PX_Color(pPX, _T("BorderColor"), m_clrBorderColor);
	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Enum(pPX, _T("Type"), m_nType, xtpChartMarkerCircle);
	PX_Int(pPX, _T("Size"), m_nSize, 0);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartMarker, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "Size", 1, GetSize, SetSize, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "Type", 2, GetType, SetType, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "FillStyle", 3, OleGetFillStyle, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "BorderVisible", 4, GetBorderVisible, SetBorderVisible,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "Visible", 5, GetVisible, SetVisible, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartMarker, CXTPChartElement)
	INTERFACE_PART(CXTPChartMarker, XTPDIID_ChartMarker, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartMarker, XTPDIID_ChartMarker)

LPDISPATCH CXTPChartMarker::OleGetFillStyle()
{
	return XTPGetDispatch(m_pFillStyle);
}

#endif
