// XTPChartCandleStickSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesPoint.h"

#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Financial/XTPChartHighLowSeriesStyle.h"
#include "Chart/Styles/Financial/XTPChartCandleStickSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum ValueIndex
{
	chartLow,
	chartHigh,
	chartOpen,
	chartClose
};

IMPLEMENT_SERIAL(CXTPChartCandleStickSeriesStyle, CXTPChartHighLowSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartCandleStickSeriesStyle

CXTPChartCandleStickSeriesStyle::CXTPChartCandleStickSeriesStyle()
{
}

CXTPChartCandleStickSeriesStyle::~CXTPChartCandleStickSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartCandleStickSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartCandleStickSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartCandleStickSeriesView

CXTPChartCandleStickSeriesView::CXTPChartCandleStickSeriesView(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartCandleStickSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartCandleStickSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartCandleStickSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartHighLowSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartHighLowSeriesStyle, GetStyle());

	CPoint ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y);

	for (int i = 0; i < 2; i++)
	{
		CXTPChartColor color = i == 0 ? pStyle->GetUpColor() : pStyle->GetDownColor();

		int nHeight = rcBounds.Height() - 3;
		int x		= i == 0 ? ptCenter.x - 4 : ptCenter.x + 4;
		int y		= i == 0 ? rcBounds.top : rcBounds.top + 3;

		CXTPPoint2i pointHight(x, y);
		CXTPPoint2i pointLow(x, pointHight.Y + nHeight);
		CXTPPoint2i pointOpen(x, pointHight.Y + nHeight / 3);
		CXTPPoint2i pointClose(x, pointHight.Y + nHeight * 2 / 3);
		int nWidth = __min(rcBounds.Width(), rcBounds.Height());
		;

		CXTPChartRectF rc(static_cast<float>(pointOpen.X) - static_cast<float>(nWidth) / 2.0f,
						  static_cast<float>(pointOpen.Y), static_cast<float>(nWidth),
						  static_cast<float>(pointClose.Y) - static_cast<float>(pointOpen.Y));
		rc.X += 2 / 2.0f;
		rc.Width -= 2;
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dBoundedRectangleDeviceCommand(rc, color, 2));

		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPChartPointF(pointHight),
			CXTPChartPointF((float)pointHight.X, (float)min(pointOpen.Y, pointClose.Y)), color, 2));

		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPChartPointF(pointLow),
			CXTPChartPointF((float)pointHight.X, (float)max(pointOpen.Y, pointClose.Y)), color, 2));
	}

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartCandleStickSeriesPointView

CXTPChartCandleStickSeriesPointView::CXTPChartCandleStickSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{
}

CXTPChartPointF CXTPChartCandleStickSeriesPointView::GetScreenPoint(int nIndex)
{
	CXTPChartCandleStickSeriesView* pView = (CXTPChartCandleStickSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_pPoint->GetValue(nIndex));
}

CXTPChartDeviceCommand*
	CXTPChartCandleStickSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPPoint2i pointOpen  = GetScreenPoint(chartOpen).Round();
	CXTPPoint2i pointClose = GetScreenPoint(chartClose).Round();

	CXTPPoint2i pointLow   = GetScreenPoint(chartLow).Round();
	CXTPPoint2i pointHight = GetScreenPoint(chartHigh).Round();

	BOOL bUpColor = TRUE;
	BOOL bFilled  = TRUE;

	if (pointOpen.Y > pointClose.Y)
	{
		CXTPPoint2i p = pointClose;
		pointClose	= pointOpen;
		pointOpen	 = p;
		bFilled		  = FALSE;
	}

	CXTPChartSeriesPoint* pPrevPoint = m_pPoint->GetPreviousPoint();
	if (pPrevPoint)
	{
		if (pPrevPoint->GetValue(chartClose) > m_pPoint->GetValue(chartClose))
		{
			bUpColor = FALSE;
		}
	}
	CXTPChartCandleStickSeriesView* pView = (CXTPChartCandleStickSeriesView*)GetSeriesView();

	CXTPChartDiagram2DAxisView* pAxisView = pView->GetAxisViewX();
	CXTPChartCandleStickSeriesStyle* pStyle =
		(CXTPChartCandleStickSeriesStyle*)GetSeriesView()->GetStyle();

	int nWidth = (int)(pAxisView->DistanceToPoint(1) * 0.5);
	if (nWidth < 5)
		nWidth = 5;

	int nLineThickness = pStyle->GetLineThickness();

	nWidth = (nWidth & ~1) + nLineThickness;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartRectF rc(static_cast<float>(pointOpen.X) - static_cast<float>(nWidth) / 2.0f,
					  static_cast<float>(pointOpen.Y), static_cast<float>(nWidth),
					  static_cast<float>(pointClose.Y) - static_cast<float>(pointOpen.Y));
	if (static_cast<int>(rc.Height) < nLineThickness)
	{
		rc.Height = (float)nLineThickness;
		rc.Y -= (float)nLineThickness / 2;
	}

	CXTPChartColor color = bUpColor ? pStyle->GetUpColor() : pStyle->GetDownColor();

	if (bFilled)
	{
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dSolidRectangleDeviceCommand(rc, color));
	}
	else
	{
		rc.X += static_cast<float>(nLineThickness) / 2.0f;
		rc.Width -= static_cast<float>(nLineThickness);
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dBoundedRectangleDeviceCommand(rc, color,
																				 nLineThickness));
	}

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPChartPointF(pointHight),
		CXTPChartPointF((float)pointHight.X, (float)min(pointOpen.Y, pointClose.Y)), color,
		nLineThickness));

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPChartPointF(pointLow),
		CXTPChartPointF((float)pointHight.X, (float)max(pointOpen.Y, pointClose.Y)), color,
		nLineThickness));

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartCandleStickSeriesStyle, CXTPChartHighLowSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartCandleStickSeriesStyle, CXTPChartHighLowSeriesStyle)
	INTERFACE_PART(CXTPChartCandleStickSeriesStyle, XTPDIID__DChartCandleStickSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartCandleStickSeriesStyle, XTPDIID__DChartCandleStickSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartCandleStickSeriesStyle,
							  "Codejock.ChartCandleStickSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartCandleStickSeriesStyle);

#endif
