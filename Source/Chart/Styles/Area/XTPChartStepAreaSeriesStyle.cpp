// XTPChartStepAreaSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisRange.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"

#include "Chart/Styles/Area/XTPChartAreaSeriesStyle.h"
#include "Chart/Styles/Area/XTPChartAreaSeriesView.h"
#include "Chart/Styles/Area/XTPChartStepAreaSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartStepAreaSeriesStyle, CXTPChartAreaSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepAreaSeriesStyle

CXTPChartStepAreaSeriesStyle::CXTPChartStepAreaSeriesStyle()
{
	m_bInvertedStep = FALSE;
}

CXTPChartStepAreaSeriesStyle::~CXTPChartStepAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartStepAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															  CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStepAreaSeriesView(pSeries, pDiagramView);
}

void CXTPChartStepAreaSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartAreaSeriesStyle::DoPropExchange(pPX);

	PX_Bool(pPX, _T("InvertedStep"), m_bInvertedStep, FALSE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepAreaSeriesView

CXTPChartStepAreaSeriesView::CXTPChartStepAreaSeriesView(CXTPChartSeries* pSeries,
														 CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries, pDiagramView)
{
}

CXTPChartDeviceCommand* CXTPChartStepAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartStepAreaSeriesStyle* pStyle = (CXTPChartStepAreaSeriesStyle*)GetStyle();

	CXTPChartAxis* pAxisY = GetAxisViewY()->GetAxis();

	CXTPChartColor clrBorder = GetBorderActualColor();

	double dZero = max(0, pAxisY->GetRange()->GetMinValue());

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointSeriesPointView* pPointView = (CXTPChartPointSeriesPointView*)
														m_pPointsView->GetAt(0);
		CXTPChartPointSeriesPointView* pPointPrev = pPointView;

		CXTPChartPointF pointPrev = pPointView->GetScreenPoint();

		CXTPChartPoints arrPoints;
		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		arrPoints.Add(pointPrev);

		for (int i = 1; i < nCount; i++)
		{
			pPointView = (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (pPointPrev)
			{
				CXTPChartPointF pointMiddle;
				if (!pStyle->GetInvertedStep())
				{
					pointMiddle = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
												 pPointPrev->m_dInternalValue);
				}
				else
				{
					pointMiddle = GetScreenPoint(pPointPrev->GetPoint()->GetInternalArgumentValue(),
												 pPointView->m_dInternalValue);
				}

				arrPoints.Add(pointMiddle);
				arrPoints.Add(pointNext);
			}

			pointPrev  = pointNext;
			pPointPrev = pPointView;
		}

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

BEGIN_DISPATCH_MAP(CXTPChartStepAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStepAreaSeriesStyle, "InvertedStep", 200, GetInvertedStep,
						SetInvertedStep, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartStepAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartStepAreaSeriesStyle, XTPDIID__DChartStepAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStepAreaSeriesStyle, XTPDIID__DChartStepAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartStepAreaSeriesStyle,
							  "Codejock.ChartStepAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartStepAreaSeriesStyle);

#endif
