// XTPChartSplineAreaSeriesStyle.cpp
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
#include "Common/Math/XTPMathUtils.h"

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

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Area/XTPChartAreaSeriesStyle.h"
#include "Chart/Styles/Area/XTPChartAreaSeriesView.h"
#include "Chart/Styles/Area/XTPChartSplineAreaSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartSplineAreaSeriesStyle::CXTPChartSplineAreaSeriesStyle()
{
}

CXTPChartSplineAreaSeriesStyle::~CXTPChartSplineAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartSplineAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartSplineAreaSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineAreaSeriesView

CXTPChartSplineAreaSeriesView::CXTPChartSplineAreaSeriesView(CXTPChartSeries* pSeries,
															 CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartSplineAreaSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartSplineAreaSeriesStyle* pStyle = (CXTPChartSplineAreaSeriesStyle*)GetStyle();

	CXTPChartAxis* pAxisY = GetAxisViewY()->GetAxis();

	CXTPChartColor clrBorder = GetBorderActualColor();

	double dZero = max(0, pAxisY->GetRange()->GetMinValue());

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointSeriesPointView* pPointView = (CXTPChartPointSeriesPointView*)
														m_pPointsView->GetAt(0);

		CXTPChartPoints arrPoints;
		CXTPChartPointF bottomPoint(
			GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		arrPoints.Add(bottomPoint);

		for (int i = 0; i < nCount; i++)
		{
			pPointView				  = (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);
			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			arrPoints.Add(pointNext);
		}

		pPointView = (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(nCount - 1);
		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		pCommand->AddChildCommand(
			pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(),
															  GetActualColor2()));

		arrPoints.RemoveAt(0);
		arrPoints.RemoveAt(arrPoints.GetSize() - 1);

		CXTPChartDeviceCommand* pSolidSplineDeviceCommand =
			CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(
				arrPoints, clrBorder, pStyle->GetBorder()->GetThickness());
		pCommand->AddChildCommand(pSolidSplineDeviceCommand);
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartSplineAreaSeriesStyle, XTPDIID__DChartSplineAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSplineAreaSeriesStyle, XTPDIID__DChartSplineAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartSplineAreaSeriesStyle,
							  "Codejock.ChartSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartSplineAreaSeriesStyle);

#endif
