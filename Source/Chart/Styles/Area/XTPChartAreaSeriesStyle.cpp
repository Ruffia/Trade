// XTPChartAreaSeriesStyle.cpp
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
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisRange.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"
#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Area/XTPChartAreaSeriesStyle.h"
#include "Chart/Styles/Area/XTPChartAreaSeriesView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartAreaSeriesStyle, CXTPChartPointSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartAreaSeriesStyle::CXTPChartAreaSeriesStyle()
{
	m_nTransparency = 135;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);

	m_nFillAcrossXAxis = FALSE;
}

CXTPChartAreaSeriesStyle::~CXTPChartAreaSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);

	SAFE_RELEASE(m_pFillStyle);
}

CXTPChartSeriesView* CXTPChartAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries,
														  CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartAreaSeriesView(pSeries, pDiagramView);
}

void CXTPChartAreaSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("Transparency"), m_nTransparency, 0);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartAreaSeriesView

CXTPChartAreaSeriesView::CXTPChartAreaSeriesView(CXTPChartSeries* pSeries,
												 CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView, TRUE)
{
}

CXTPChartSeriesPointView* CXTPChartAreaSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartColor CXTPChartAreaSeriesView::GetActualColor() const
{
	CXTPChartColor clr = m_pSeries->GetColor();
	return CXTPChartColor((BYTE)((CXTPChartAreaSeriesStyle*)GetStyle())->GetTransparency(),
						  clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartAreaSeriesView::GetActualColor2() const
{
	CXTPChartColor clr = m_pSeries->GetColor2();
	return CXTPChartColor((BYTE)((CXTPChartAreaSeriesStyle*)GetStyle())->GetTransparency(),
						  clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartAreaSeriesView::GetBorderActualColor() const
{
	CXTPChartAreaSeriesStyle* pStyle = (CXTPChartAreaSeriesStyle*)GetStyle();

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = m_pSeries->GetColor().GetDarkColor();
	return clr;
}

CXTPChartDeviceCommand*
	CXTPChartAreaSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	CXTPChartAreaSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartAreaSeriesStyle,
													   m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.top + 4));

	CXTPChartPoints arrPoints;
	arrPoints.Add(
		CXTPChartPointF(static_cast<int>(rcBounds.left), static_cast<int>(rcBounds.bottom + 1)));
	arrPoints.Add(CXTPChartPointF(ptCenter.X, float(rcBounds.top + 4)));
	arrPoints.Add(
		CXTPChartPointF(static_cast<int>(rcBounds.right), static_cast<int>(rcBounds.bottom + 1)));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints,
																		  GetActualColor(),
																		  GetActualColor2()));

	CXTPChartColor clrBorder = GetBorderActualColor();
	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(arrPoints[0], arrPoints[1],
																	  clrBorder, 1));
	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(arrPoints[1], arrPoints[2],
																	  clrBorder, 1));

	int nSize = __min(rcBounds.Width(), rcBounds.Height());
	pCommand->AddChildCommand(
		pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(),
												 m_pSeries->GetColor2(),
												 m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartAreaSeriesStyle* pStyle = (CXTPChartAreaSeriesStyle*)GetStyle();

	CXTPChartAxis* pAxisY = GetAxisViewY()->GetAxis();

	CXTPChartColor clrBorder = GetBorderActualColor();

	double dZero = 0;
	if (pStyle->GetFillAcrossXAxis())
	{
		dZero = pAxisY->GetRange()->GetMinValue();
	}
	else
	{
		dZero = max(0, pAxisY->GetRange()->GetMinValue());
	}

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointSeriesPointView* pPointView = (CXTPChartPointSeriesPointView*)
														m_pPointsView->GetAt(0);

		CXTPChartPointF pointPrev = GetScreenPoint(
			pPointView->GetPoint()->GetInternalArgumentValue(), pPointView->m_dInternalValue);

		CXTPChartPoints arrPoints;
		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		for (int i = 1; i < nCount; i++)
		{
			pPointView				  = (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);
			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			arrPoints.Add(pointPrev);

			pointPrev = pointNext;
		}

		arrPoints.Add(pointPrev);

		pPointView = (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(nCount - 1);

		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints,
																			  GetActualColor(),
																			  GetActualColor2()));

		arrPoints.RemoveAt(0);
		arrPoints.RemoveAt(arrPoints.GetSize() - 1);

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidPolylineDeviceCommand(
					arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAreaSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartAreaSeriesStyle, "Transparency", 105, GetTransparency,
						SetTransparency, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPChartAreaSeriesStyle, "FillStyle", 5, OleGetFillStyle, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAreaSeriesStyle, "Border", 6, OleGetBorder, SetNotSupported,
						VT_DISPATCH)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAreaSeriesStyle, CXTPChartPointSeriesStyle)
	INTERFACE_PART(CXTPChartAreaSeriesStyle, XTPDIID__DChartAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAreaSeriesStyle, XTPDIID__DChartAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartAreaSeriesStyle,
							  "Codejock.ChartAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartAreaSeriesStyle);

LPDISPATCH CXTPChartAreaSeriesStyle::OleGetFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

LPDISPATCH CXTPChartAreaSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

#endif
