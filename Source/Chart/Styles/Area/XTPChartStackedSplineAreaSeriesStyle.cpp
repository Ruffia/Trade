// XTPChartStackedSplineAreaSeriesStyle.cpp
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

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Area/XTPChartAreaSeriesStyle.h"
#include "Chart/Styles/Area/XTPChartAreaSeriesView.h"
#include "Chart/Styles/Area/XTPChartStackedAreaSeriesStyle.h"
#include "Chart/Styles/Area/XTPChartStackedSplineAreaSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartStackedSplineAreaSeriesStyle, CXTPChartStackedAreaSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartStackedSplineAreaSeriesStyle::CXTPChartStackedSplineAreaSeriesStyle()
{
	m_nTransparency = 135;
	m_dStackHeight  = 0;
}

CXTPChartStackedSplineAreaSeriesStyle::~CXTPChartStackedSplineAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartStackedSplineAreaSeriesStyle::CreateView(
	CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStackedSplineAreaSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedSplineAreaSeriesView

CXTPChartStackedSplineAreaSeriesView::CXTPChartStackedSplineAreaSeriesView(
	CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartStackedAreaSeriesView(pSeries, pDiagramView)
{
}

CXTPChartDeviceCommand*
	CXTPChartStackedSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartStackedSplineAreaSeriesStyle* pStyle = (CXTPChartStackedSplineAreaSeriesStyle*)
		GetStyle();

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
		{
			CXTPChartDeviceCommand* pSolidSplineDeviceCommand =
				CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(
					arrPoints, clrBorder, pStyle->GetBorder()->GetThickness());
			pCommand->AddChildCommand(pSolidSplineDeviceCommand);
		}

		for (i = nCount - 1; i >= 0; i--)
		{
			pPointView = (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(
				pPointView->GetPoint()->GetInternalArgumentValue(), pPointView->m_dValueFrom);

			arrPoints.Add(point);
		}

		pCommand->AddChildCommand(
			pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(),
															  GetActualColor2(), TRUE));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartStackedSplineAreaSeriesStyle, CXTPChartStackedAreaSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartStackedSplineAreaSeriesStyle, CXTPChartStackedAreaSeriesStyle)
	INTERFACE_PART(CXTPChartStackedSplineAreaSeriesStyle,
				   XTPDIID__DChartStackedSplineAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStackedSplineAreaSeriesStyle,
						XTPDIID__DChartStackedSplineAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartStackedSplineAreaSeriesStyle,
							  "Codejock.ChartStackedSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartStackedSplineAreaSeriesStyle);

#endif
