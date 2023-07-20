// XTPChartStackedAreaSeriesStyle.cpp
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
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

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
#include "Chart/Styles/Area/XTPChartStackedAreaSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartStackedAreaSeriesStyle, CXTPChartAreaSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartStackedAreaSeriesStyle::CXTPChartStackedAreaSeriesStyle()
{
	m_nTransparency = 135;
	m_dStackHeight  = 0;
}

CXTPChartStackedAreaSeriesStyle::~CXTPChartStackedAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartStackedAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStackedAreaSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedAreaSeriesView

CXTPChartStackedAreaSeriesView::CXTPChartStackedAreaSeriesView(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartStackedAreaSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartStackedAreaSeriesPointView(pPoint, pParentView);
}

void CXTPChartStackedAreaSeriesView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	CXTPChartAreaSeriesView::UpdateRange(pDC);

	CXTPChartDiagramView* pDiagramView = GetDiagramView();

	for (int i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		if (DYNAMIC_DOWNCAST(CXTPChartStackedAreaSeriesStyle, pView->GetSeries()->GetStyle()))
		{
			if (pView != this)
				return;

			break;
		}
	}

	CMap<double, double, double, double> mapValues;

	for (int j = 0; j < pDiagramView->GetSeriesView()->GetCount(); j++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(j);

		if (!DYNAMIC_DOWNCAST(CXTPChartStackedAreaSeriesStyle, pView->GetSeries()->GetStyle()))
			continue;

		CXTPChartStackedAreaSeriesView* pStackedAreaSeriesView = (CXTPChartStackedAreaSeriesView*)
			pView;

		for (int i = 0; i < pStackedAreaSeriesView->m_pPointsView->GetCount(); i++)
		{
			CXTPChartStackedAreaSeriesPointView* pPointView =
				(CXTPChartStackedAreaSeriesPointView*)pStackedAreaSeriesView->m_pPointsView->GetAt(
					i);

			double dValue = pPointView->GetPoint()->GetValue(0);

			pPointView->m_dValueFrom = mapValues[pPointView->GetPoint()->GetInternalArgumentValue()];
			pPointView->m_dInternalValue = pPointView->m_dValueTo = pPointView->m_dValueFrom
																	+ dValue;

			mapValues[pPointView->GetPoint()->GetInternalArgumentValue()] = pPointView->m_dValueTo;
		}
	}

	double dStackHeight = ((CXTPChartStackedAreaSeriesStyle*)GetStyle())->GetStackHeight();
	if (dStackHeight != 0)
	{
		for (int j = 0; j < pDiagramView->GetSeriesView()->GetCount(); j++)
		{
			CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(
				j);

			if (DYNAMIC_DOWNCAST(CXTPChartStackedAreaSeriesStyle, pView->GetSeries()->GetStyle()))
			{
				CXTPChartStackedAreaSeriesView* pStackedAreaSeriesView =
					(CXTPChartStackedAreaSeriesView*)pView;

				for (int i = 0; i < pStackedAreaSeriesView->m_pPointsView->GetCount(); i++)
				{
					CXTPChartStackedAreaSeriesPointView* pPointView =
						(CXTPChartStackedAreaSeriesPointView*)
							pStackedAreaSeriesView->m_pPointsView->GetAt(i);

					double dLimit = mapValues[pPointView->GetPoint()->GetInternalArgumentValue()];

					pPointView->m_dValueFrom	 = pPointView->m_dValueFrom * dStackHeight / dLimit;
					pPointView->m_dInternalValue = pPointView->m_dValueTo = pPointView->m_dValueTo
																			* dStackHeight / dLimit;
				}
			}
		}
	}
}

CXTPChartDeviceCommand*
	CXTPChartStackedAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartStackedAreaSeriesStyle* pStyle = (CXTPChartStackedAreaSeriesStyle*)GetStyle();

	CXTPChartColor clrBorder = GetBorderActualColor();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartStackedAreaSeriesPointView* pPointView;

		CXTPChartPoints arrPoints;

		int i;

		for (i = 0; i < nCount; i++)
		{
			pPointView = (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(
				pPointView->GetPoint()->GetInternalArgumentValue(), pPointView->m_dValueTo);

			arrPoints.Add(point);
		}

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidPolylineDeviceCommand(
					arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));

		for (i = nCount - 1; i >= 0; i--)
		{
			pPointView = (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(
				pPointView->GetPoint()->GetInternalArgumentValue(), pPointView->m_dValueFrom);

			arrPoints.Add(point);
		}

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints,
																			  GetActualColor(),
																			  GetActualColor2()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartStackedAreaSeriesPointView::CXTPChartStackedAreaSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartStackedAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStackedAreaSeriesStyle, "StackHeight", 104, OleGetStackHeight,
						OleSetStackHeight, VT_R8)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartStackedAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartStackedAreaSeriesStyle, XTPDIID__DChartStackedAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStackedAreaSeriesStyle, XTPDIID__DChartStackedAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartStackedAreaSeriesStyle,
							  "Codejock.ChartStackedAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartStackedAreaSeriesStyle);

double CXTPChartStackedAreaSeriesStyle::OleGetStackHeight()
{
	return GetStackHeight();
}

void CXTPChartStackedAreaSeriesStyle::OleSetStackHeight(double dStackHeight)
{
	SetStackHeight(dStackHeight);
}

#endif
