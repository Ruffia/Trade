// XTPChartHighLowSeriesStyle.cpp
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

#include "Chart/Appearance/XTPChartAppearance.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Financial/XTPChartHighLowSeriesStyle.h"
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

//////////////////////////////////////////////////////////////////////////
// CXTPChartHighLowSeriesStyle

IMPLEMENT_SERIAL(CXTPChartHighLowSeriesStyle, CXTPChartDiagram2DSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartHighLowSeriesStyle::CXTPChartHighLowSeriesStyle()
{
	m_nLineThickness = 2;

	m_clrDownColor = CXTPChartColor::Empty;
	m_clrUpColor   = CXTPChartColor::Empty;
}

CXTPChartHighLowSeriesStyle::~CXTPChartHighLowSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartHighLowSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartHighLowSeriesView(pSeries, pDiagramView);
}

CXTPChartColor CXTPChartHighLowSeriesStyle::GetUpColor() const
{
	if (!m_clrUpColor.IsEmpty())
		return m_clrUpColor;

	return GetAppearance()->GetFinanceStyleAppearance()->UpColor;
}
CXTPChartColor CXTPChartHighLowSeriesStyle::GetDownColor() const
{
	if (!m_clrDownColor.IsEmpty())
		return m_clrDownColor;

	return GetAppearance()->GetFinanceStyleAppearance()->DownColor;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartHighLowSeriesView

CXTPChartHighLowSeriesView::CXTPChartHighLowSeriesView(CXTPChartSeries* pSeries,
													   CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartHighLowSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartHighLowSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartHighLowSeriesView::CreateLegendDeviceCommand(
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

		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPChartPointF(pointHight), CXTPChartPointF(pointLow), color, 2));

		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPChartPointF(pointOpen),
			CXTPChartPointF((float)(pointOpen.X - nWidth / 2), (float)pointOpen.Y), color, 2));

		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPChartPointF(pointClose),
			CXTPChartPointF((float)(pointClose.X + nWidth / 2), (float)pointClose.Y), color, 2));
	}

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartHighLowSeriesPointView

CXTPChartHighLowSeriesPointView::CXTPChartHighLowSeriesPointView(CXTPChartSeriesPoint* pPoint,
																 CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{
}
CXTPChartPointF CXTPChartHighLowSeriesPointView::GetScreenPoint(int nIndex)
{
	CXTPChartHighLowSeriesView* pView = (CXTPChartHighLowSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_pPoint->GetValue(nIndex));
}

CXTPChartDeviceCommand*
	CXTPChartHighLowSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPPoint2i pointOpen  = GetScreenPoint(chartOpen).Round();
	CXTPPoint2i pointClose = GetScreenPoint(chartClose).Round();

	CXTPPoint2i pointLow   = GetScreenPoint(chartLow).Round();
	CXTPPoint2i pointHight = GetScreenPoint(chartHigh).Round();

	BOOL bUpColor = TRUE;

	CXTPChartSeriesPoint* pPrevPoint = m_pPoint->GetPreviousPoint();
	if (pPrevPoint)
	{
		if (pPrevPoint->GetValue(chartClose) > m_pPoint->GetValue(chartClose))
		{
			bUpColor = FALSE;
		}
	}

	CXTPChartHighLowSeriesView* pView = (CXTPChartHighLowSeriesView*)GetSeriesView();

	CXTPChartDiagram2DAxisView* pAxisView = pView->GetAxisViewX();
	CXTPChartHighLowSeriesStyle* pStyle = (CXTPChartHighLowSeriesStyle*)GetSeriesView()->GetStyle();

	int nWidth = (int)(pAxisView->DistanceToPoint(1) * 0.5);
	if (nWidth < 5)
		nWidth = 5;

	int nLineThickness = pStyle->GetLineThickness();

	nWidth = (nWidth & ~1) + nLineThickness * 3;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartColor color = bUpColor ? pStyle->GetUpColor() : pStyle->GetDownColor();

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPChartPointF(pointHight), CXTPChartPointF(pointLow), color, nLineThickness));

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPChartPointF(pointOpen),
		CXTPChartPointF((float)(pointOpen.X - nWidth / 2), (float)pointOpen.Y), color,
		nLineThickness));

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPChartPointF(pointClose),
		CXTPChartPointF((float)(pointClose.X + nWidth / 2), (float)pointClose.Y), color,
		nLineThickness));

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartHighLowSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartHighLowSeriesStyle, "LineThickness", 100, GetLineThickness,
						SetLineThickness, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartHighLowSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartHighLowSeriesStyle, XTPDIID__DChartHighLowSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartHighLowSeriesStyle, XTPDIID__DChartHighLowSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartHighLowSeriesStyle,
							  "Codejock.ChartHighLowSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartHighLowSeriesStyle);

#endif
