// XTPChartFastLineSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartAxisRange.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartLineStyle.h"

#include "Chart/Styles/Point/XTPChartPointSeriesView.h"

#include "Chart/Styles/Line/XTPChartFastLineSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartFastLineSeriesStyle, CXTPChartDiagram2DSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartFastLineSeriesStyle

CXTPChartFastLineSeriesStyle::CXTPChartFastLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);

	m_bAntialiasing = TRUE;

	SetLabel(NULL);
}

CXTPChartFastLineSeriesStyle::~CXTPChartFastLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);
}

CXTPChartSeriesView* CXTPChartFastLineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															  CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartFastLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartFastLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartFastLineSeriesView

CXTPChartFastLineSeriesView::CXTPChartFastLineSeriesView(CXTPChartSeries* pSeries,
														 CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{
}

void CXTPChartFastLineSeriesView::CreatePointsView(CXTPChartDeviceContext* /*pDC*/)
{
	m_pPointsView = new CXTPChartElementView(this);
	m_pLabelsView = m_pDiagramView->GetLabelsView();
}

CXTPChartSeriesPointView* CXTPChartFastLineSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartFastLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* /*pDC*/)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartFastLineSeriesStyle* pStyle = (CXTPChartFastLineSeriesStyle*)GetStyle();

	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand(
			pStyle->GetAntialiasing()));

	CXTPChartAxisRange* pRange = GetAxisViewX()->GetAxis()->GetRange();

	int nCount = m_pSeries->GetPoints()->GetCount();
	if (nCount > 1)
	{
		CXTPChartPoints arrPoints;

		double dViewMinValue = pRange->GetViewMinValue();
		double dViewMaxValue = pRange->GetViewMaxValue();

		int l = 0, r = nCount - 1;
		while (l < r)
		{
			int m						 = (l + r) / 2;
			CXTPChartSeriesPoint* pPoint = m_pSeries->GetPoints()->GetAt(m);

			if (NULL != pPoint && pPoint->GetArgumentValue() < dViewMinValue)
			{
				l = m + 1;
			}
			else
			{
				r = m;
			}
		}

		for (int i = max(l - 1, 0); i < nCount; i++)
		{
			CXTPChartSeriesPoint* pPoint = m_pSeries->GetPoints()->GetAt(i);

			if (NULL != pPoint)
			{
				CXTPChartPointF pointNext = GetScreenPoint(pPoint->GetArgumentValue(),
														   pPoint->GetValue(0));
				arrPoints.Add(pointNext);

				if (pPoint->GetArgumentValue() > dViewMaxValue)
					break;
			}
		}

		pCommand->AddChildCommand(
			pStyle->GetLineStyle()->CreateDeviceCommand(arrPoints, m_pSeries->GetColor()));
	}

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartFastLineSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPPoint3d(rcBounds.left, ptCenter.Y), CXTPPoint3d(rcBounds.right, ptCenter.Y),
		m_pSeries->GetColor(), m_pSeries->GetLegendItem()->GetActualLineThickness()));

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartFastLineSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartFastLineSeriesStyle, "LineStyle", 100, OleGetLineStyle,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFastLineSeriesStyle, "Antialiasing", 200, m_bAntialiasing,
							OleChartChanged, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartFastLineSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartFastLineSeriesStyle, XTPDIID__DChartFastLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFastLineSeriesStyle, XTPDIID__DChartFastLineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartFastLineSeriesStyle,
							  "Codejock.ChartFastLineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartFastLineSeriesStyle);

LPDISPATCH CXTPChartFastLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}

#endif
