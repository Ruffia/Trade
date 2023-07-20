// XTPChartRangeSplineAreaSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesLabel.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"
#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Area/XTPChartAreaSeriesStyle.h"
#include "Chart/Styles/Area/XTPChartAreaSeriesView.h"
#include "Chart/Styles/Area/XTPChartRangeSplineAreaSeriesStyle.h"

#include "Chart/Styles/Point/XTPChartPointSeriesLabel.h"
#include "Chart/Styles/Point/XTPChartPointSeriesLabelView.h"
#include "Chart/Styles/Area/XTPChartRangeAreaSeriesLabel.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRangeSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartRangeSplineAreaSeriesStyle::CXTPChartRangeSplineAreaSeriesStyle()
{
	SetLabel(new CXTPChartRangeAreaSeriesLabel());
}

CXTPChartRangeSplineAreaSeriesStyle::~CXTPChartRangeSplineAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartRangeSplineAreaSeriesStyle::CreateView(
	CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRangeSplineAreaSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeSplineAreaSeriesPointView

CXTPChartRangeSplineAreaSeriesPointView::CXTPChartRangeSplineAreaSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{
}

void CXTPChartRangeSplineAreaSeriesPointView::UpdateMinMaxRange(double& nMinValue,
																double& nMaxValue) const
{
	if (m_pPoint->GetValueLength() > 1)
	{
		double nValue = m_pPoint->GetValue(1);

		nMaxValue = max(nMaxValue, nValue);
		nMinValue = min(nMinValue, nValue);
	}
}

CXTPChartDeviceCommand*
	CXTPChartRangeSplineAreaSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color	   = GetColor();
	CXTPChartColor color2	  = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();

	if (CXTPMathUtils::IsNan(m_dInternalValue))
		return NULL;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartPointSeriesView* pView = (CXTPChartPointSeriesView*)GetSeriesView();

	CXTPChartPointF point = pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(),
												  m_pPoint->GetValue(0));
	pCommand->AddChildCommand(((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())
								  ->GetMarker()
								  ->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	point = pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_pPoint->GetValue(1));
	pCommand->AddChildCommand(((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())
								  ->GetMarker()
								  ->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeSplineAreaSeriesView

CXTPChartRangeSplineAreaSeriesView::CXTPChartRangeSplineAreaSeriesView(
	CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries, pDiagramView)
{
}

CXTPChartSeriesPointView* CXTPChartRangeSplineAreaSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeSplineAreaSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartRangeSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartRangeSplineAreaSeriesStyle* pStyle = (CXTPChartRangeSplineAreaSeriesStyle*)GetStyle();

	CXTPChartColor clrBorder = GetBorderActualColor();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartSeriesPointView* pPointView;

		CXTPChartPoints arrPoints;

		int i;

		for (i = 0; i < nCount; i++)
		{
			pPointView = (CXTPChartSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point =
				GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
							   pPointView->GetPoint()->GetValue(1));

			arrPoints.Add(point);
		}

		if (pStyle->GetBorder()->IsVisible())
		{
			CXTPChartDeviceCommand* pSolidSplineDeviceCommand =
				CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(
					arrPoints, clrBorder, pStyle->GetBorder()->GetThickness());
			pCommand->AddChildCommand(pSolidSplineDeviceCommand);
		}

		CXTPChartPoints arrBottomPoints;

		for (i = nCount - 1; i >= 0; i--)
		{
			pPointView = (CXTPChartSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point =
				GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
							   pPointView->GetPoint()->GetValue(0));

			arrPoints.Add(point);
			arrBottomPoints.Add(point);
		}
		if (pStyle->GetBorder()->IsVisible())
		{
			CXTPChartDeviceCommand* pSolidSplineDeviceCommand =
				CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(
					arrBottomPoints, clrBorder, pStyle->GetBorder()->GetThickness());
			pCommand->AddChildCommand(pSolidSplineDeviceCommand);
		}

		pCommand->AddChildCommand(
			pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(),
															  GetActualColor2(), TRUE));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRangeSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRangeSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartRangeSplineAreaSeriesStyle, XTPDIID__DChartRangeSplineAreaSeriesStyle,
				   Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRangeSplineAreaSeriesStyle,
						XTPDIID__DChartRangeSplineAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRangeSplineAreaSeriesStyle,
							  "Codejock.ChartRangeSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRangeSplineAreaSeriesStyle);

#endif
