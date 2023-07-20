// XTPChartSeriesStyle.cpp
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/XTPChartDefines.h"
#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartErrorBar.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesStyle

IMPLEMENT_DYNAMIC(CXTPChartSeriesStyle, CXTPChartElement)

CXTPChartSeriesStyle::CXTPChartSeriesStyle()
	: m_pLabel(NULL)
	, m_pErrorBar(NULL)
	, m_clrColor(CXTPChartColor::Empty)
	, m_bColorEach(FALSE)
	, m_nTransparency(0)
	, m_Rotation3d(0, 0, 90)
	, m_bSmooth3dEdges(TRUE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeriesStyle::~CXTPChartSeriesStyle()
{
	SAFE_RELEASE(m_pLabel);
	SAFE_RELEASE(m_pErrorBar);
}

CXTPChartSeriesLabel* CXTPChartSeriesStyle::SetLabel(CXTPChartSeriesLabel* pLabel)
{
	SAFE_RELEASE(m_pLabel);

	m_pLabel = pLabel;

	if (NULL != m_pLabel)
	{
		m_pLabel->m_pOwner = this;
	}

	return m_pLabel;
}

CXTPChartErrorBar* CXTPChartSeriesStyle::SetErrorBar(CXTPChartErrorBar* pErrorBar)
{
	SAFE_RELEASE(m_pErrorBar);

	m_pErrorBar = pErrorBar;

	if (NULL != m_pErrorBar)
	{
		m_pErrorBar->m_pOwner = this;
	}

	return m_pErrorBar;
}

CXTPChartSeriesStyleAppearance* CXTPChartSeriesStyle::GetStyleAppearance() const
{
	return GetAppearance()->GetSeriesStyleAppearance();
}

void CXTPChartSeriesStyle::ApplyTo(CXTPChartDeviceCommand* pCommand)
{
	if (NULL != pCommand)
	{
		pCommand->ApplySeriesStyle(this);
	}
}

BOOL CXTPChartSeriesStyle::IsStyleDiagram(CXTPChartDiagram* /*pDiagram*/) const
{
	return FALSE;
}

void CXTPChartSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue,
												  double nMaxValue, double& nCorrection)
{
	UNREFERENCED_PARAMETER(pAxis);
	UNREFERENCED_PARAMETER(nMinValue);
	UNREFERENCED_PARAMETER(nMaxValue);
	UNREFERENCED_PARAMETER(nCorrection);
}

void CXTPChartSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Bool(pPX, _T("ColorEach"), m_bColorEach);

	if (pPX->IsLoading())
		SAFE_RELEASE(m_pLabel);
	PX_Object(pPX, _T("Label"), m_pLabel, RUNTIME_CLASS(CXTPChartSeriesLabel));
	if (pPX->IsLoading())
		m_pLabel->m_pOwner = this;

	PX_Int(pPX, _T("Transparency"), m_nTransparency, 0);
	PX_Rotation(pPX, _T("Rotation3D"), m_Rotation3d, CXTPChart3dRotation(0, 0, 0));
	PX_Bool(pPX, _T("SmoothEdges3D"), m_bSmooth3dEdges, FALSE);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSeriesStyle, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "Label", 1, OleGetLabel, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "ErrorBar", 20, OleGetErrorBar, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "ColorEach", 2, OleGetColorEach, OleSetColorEach,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "Color", 3, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "Transparency", 105, GetTransparency, SetTransparency,
						VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "SmoothEdges", 111, Is3dEdgeSmoothingEnabled,
						Enable3dEdgeSmoothing, VT_BOOL)
	DISP_FUNCTION_ID(CXTPChartSeriesStyle, "GetRotation", 120, OleGet3dRotation, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesStyle, "SetRotation", 121, OleSet3dRotation, VT_EMPTY,
					 VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartSeriesStyle, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeriesStyle, XTPDIID_ChartSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesStyle, XTPDIID_ChartSeriesStyle)

void CXTPChartSeriesStyle::OleChartChanged()
{
	OnChartChanged();
}

LPDISPATCH CXTPChartSeriesStyle::OleGetLabel()
{
	return XTPGetDispatch(m_pLabel);
}

LPDISPATCH CXTPChartSeriesStyle::OleGetErrorBar()
{
	return XTPGetDispatch(m_pErrorBar);
}

BOOL CXTPChartSeriesStyle::OleGetColorEach()
{
	return IsColorEach();
}

void CXTPChartSeriesStyle::OleSetColorEach(BOOL bColorEach)
{
	SetColorEach(bColorEach);
}

OLE_COLOR CXTPChartSeriesStyle::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

void CXTPChartSeriesStyle::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

LPDISPATCH CXTPChartSeriesStyle::OleGet3dRotation()
{
	return XTPGetDispatch(&m_Rotation3d, TRUE);
}

void CXTPChartSeriesStyle::OleSet3dRotation(LPDISPATCH pRotationDisp)
{
	if (NULL != pRotationDisp)
	{
		CXTPChart3dRotation* pRotation = XTP_DYNAMIC_DOWNCAST_REMOTE_(
			CXTPChart3dRotation, CXTPChart3dRotation::FromIDispatchSafe(pRotationDisp));
		if (NULL == pRotation)
		{
			AfxThrowOleException(E_INVALIDARG);
		}

		Set3dRotation(*pRotation);
	}
}

#endif
