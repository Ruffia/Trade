// XTPChartSplineSeriesStyle.cpp
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

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartLineStyle.h"
#include "Common/Math/XTPMathUtils.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"

#include "Chart/Styles/Line/XTPChartLineSeriesStyle.h"
#include "Chart/Styles/Line/XTPChartSplineSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineSeriesStyle

IMPLEMENT_SERIAL(CXTPChartSplineSeriesStyle, CXTPChartLineSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartSplineSeriesStyle::CXTPChartSplineSeriesStyle()
{
}

CXTPChartSplineSeriesStyle::~CXTPChartSplineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartSplineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartSplineSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineSeriesView

CXTPChartSplineSeriesView::CXTPChartSplineSeriesView(CXTPChartSeries* pSeries,
													 CXTPChartDiagramView* pDiagramView)
	: CXTPChartLineSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartSplineSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartSplineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartSplineSeriesStyle* pStyle = (CXTPChartSplineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();

	CXTPChartPoints points;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartPointSeriesPointView* pPointView = (CXTPChartPointSeriesPointView*)
														m_pPointsView->GetAt(i);
		if (CXTPMathUtils::IsNan(pPointView->m_dInternalValue))
		{
			if (points.GetSize() > 1)
			{
				pCommand->AddChildCommand(
					pStyle->GetLineStyle()->CreateSplineDeviceCommand(points,
																	  m_pSeries->GetColor()));
			}
			points.RemoveAll();
			continue;
		}

		CXTPChartPointF pointNext = pPointView->GetScreenPoint();
		points.Add(pointNext);
	}

	if (points.GetSize() > 1)
	{
		pCommand->AddChildCommand(
			pStyle->GetLineStyle()->CreateSplineDeviceCommand(points, m_pSeries->GetColor()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSplineSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartSplineSeriesStyle, "LineStyle", 100, OleGetLineStyle,
						SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartSplineSeriesStyle, CXTPChartPointSeriesStyle)
	INTERFACE_PART(CXTPChartSplineSeriesStyle, XTPDIID__DChartSplineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSplineSeriesStyle, XTPDIID__DChartSplineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartSplineSeriesStyle,
							  "Codejock.ChartSplineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartSplineSeriesStyle);

LPDISPATCH CXTPChartSplineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}

#endif
