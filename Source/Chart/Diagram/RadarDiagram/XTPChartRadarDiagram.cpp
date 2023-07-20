// XTPChartRadarDiagram.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxis.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartRadarDiagram, CXTPChartDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRadarDiagram::CXTPChartRadarDiagram()
{
	m_pAxisX = new CXTPChartRadarAxisX(this);

	m_pAxisY = new CXTPChartRadarAxisY(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillEmpty);

	m_nStartAngle = 0;

	m_nStyle = xtpChartRadarDiagramCircle;
}

CXTPChartRadarDiagram::~CXTPChartRadarDiagram()
{
	SAFE_RELEASE(m_pAxisX);
	SAFE_RELEASE(m_pAxisY);
	SAFE_RELEASE(m_pFillStyle);
}

CXTPChartDiagramView* CXTPChartRadarDiagram::CreateView(CXTPChartDeviceContext* /*pDC*/,
														CXTPChartElementView* pParent)
{
	return new CXTPChartRadarDiagramView(this, pParent);
}

CXTPChartDiagram2DAppearance* CXTPChartRadarDiagram::GetAppearance() const
{
	return CXTPChartElement::GetAppearance()->GetDiagram2DAppearance();
}

void CXTPChartRadarDiagram::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram::DoPropExchange(pPX);

	PX_Color(pPX, _T("BackgroundColor"), m_clrBackgroundColor);
	PX_Color(pPX, _T("BackgroundColor2"), m_clrBackgroundColor2);
	PX_Color(pPX, _T("BorderColor"), m_clrBorderColor);

	PX_Int(pPX, _T("StartAngle"), m_nStartAngle);
	PX_Enum(pPX, _T("Style"), m_nStyle, xtpChartRadarDiagramCircle);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("BackgroundFillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

	CXTPPropExchangeSection secAxisX(pPX->GetSection(_T("AxisX")));
	m_pAxisX->DoPropExchange(&secAxisX);

	CXTPPropExchangeSection secAxisY(pPX->GetSection(_T("AxisY")));
	m_pAxisY->DoPropExchange(&secAxisY);
}

void CXTPChartRadarDiagram::UpdateLayout(CXTPChartDeviceContext* /*pDC*/,
										 CXTPChartDiagramView* /*pView*/, CRect /*rcBounds*/)
{
}

CXTPChartColor CXTPChartRadarDiagram::GetBackgroundColor() const
{
	return m_clrBackgroundColor;
}

CXTPChartColor CXTPChartRadarDiagram::GetBackgroundColor2() const
{
	return m_clrBackgroundColor2;
}

CXTPChartColor CXTPChartRadarDiagram::GetBorderColor() const
{
	return m_clrBorderColor;
}

CXTPChartColor CXTPChartRadarDiagram::GetActualBackgroundColor() const
{
	if (!m_clrBackgroundColor.IsEmpty())
		return m_clrBackgroundColor;

	return GetAppearance()->BackgroundColor;
}

CXTPChartColor CXTPChartRadarDiagram::GetActualBackgroundColor2() const
{
	if (!m_clrBackgroundColor2.IsEmpty())
		return m_clrBackgroundColor2;

	return GetAppearance()->BackgroundColor2;
}

CXTPChartColor CXTPChartRadarDiagram::GetActualBorderColor() const
{
	if (!m_clrBorderColor.IsEmpty())
		return m_clrBorderColor;

	return GetAppearance()->BorderColor;
}

CXTPChartFillStyle* CXTPChartRadarDiagram::GetActualFillStyle() const
{
	if (m_pFillStyle->GetFillMode() != xtpChartFillEmpty)
		return m_pFillStyle;

	return GetAppearance()->BackgroundFillStyle;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarDiagram, CXTPChartDiagram)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "AxisX", 1, OleGetAxisX, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "AxisY", 2, OleGetAxisY, SetNotSupported,
						VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BackgroundColor", 7, OleGetBackgroundColor,
						OleSetBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BackgroundColor2", 8, OleGetBackgroundColor2,
						OleSetBackgroundColor2, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BorderColor", 9, OleGetBorderColor,
						OleSetBorderColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BackgroundFillStyle", 10, OleGetBackgroundFillStyle,
						SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "StartAngle", 11, GetStartAngle, SetStartAngle,
						VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "Style", 12, GetStyle, SetStyle, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarDiagram, CXTPChartDiagram)
	INTERFACE_PART(CXTPChartRadarDiagram, XTPDIID__DChartRadarDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarDiagram, XTPDIID__DChartRadarDiagram)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRadarDiagram,
							  "Codejock.ChartRadarDiagram." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRadarDiagram);

LPDISPATCH CXTPChartRadarDiagram::OleGetAxisX()
{
	return XTPGetDispatch(GetAxisX());
}

LPDISPATCH CXTPChartRadarDiagram::OleGetAxisY()
{
	return XTPGetDispatch(GetAxisY());
}

OLE_COLOR CXTPChartRadarDiagram::OleGetBackgroundColor()
{
	return GetBackgroundColor().ToOleColor();
}

void CXTPChartRadarDiagram::OleSetBackgroundColor(OLE_COLOR clr)
{
	SetBackgroundColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartRadarDiagram::OleGetBackgroundColor2()
{
	return GetBackgroundColor2().ToOleColor();
}

void CXTPChartRadarDiagram::OleSetBackgroundColor2(OLE_COLOR clr)
{
	SetBackgroundColor2(CXTPChartColor::FromOleColor(clr));
}

LPDISPATCH CXTPChartRadarDiagram::OleGetBackgroundFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

OLE_COLOR CXTPChartRadarDiagram::OleGetBorderColor()
{
	return GetBorderColor().ToOleColor();
}

void CXTPChartRadarDiagram::OleSetBorderColor(OLE_COLOR clr)
{
	SetBorderColor(CXTPChartColor::FromOleColor(clr));
}

#endif
