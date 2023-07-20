// XTPChartPointSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartErrorBar.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesLabel.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"
#include "Chart/Styles/Point/XTPChartPointSeriesErrorBar.h"
#include "Chart/Styles/Point/XTPChartMarker.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartPointSeriesStyle, CXTPChartDiagram2DSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesStyle

CXTPChartPointSeriesStyle::CXTPChartPointSeriesStyle()
{
	m_pMarker = new CXTPChartMarker(this);

	SetLabel(new CXTPChartPointSeriesLabel());
	SetErrorBar(new CXTPChartPointSeriesErrorBar());
}

CXTPChartPointSeriesStyle::~CXTPChartPointSeriesStyle()
{
	SAFE_RELEASE(m_pMarker);
}

void CXTPChartPointSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection secMarker(pPX->GetSection(_T("Marker")));
	m_pMarker->DoPropExchange(&secMarker);
}

CXTPChartSeriesView* CXTPChartPointSeriesStyle::CreateView(CXTPChartSeries* pSeries,
														   CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartPointSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesView

CXTPChartPointSeriesView::CXTPChartPointSeriesView(CXTPChartSeries* pSeries,
												   CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartPointSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartPointSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle,
														m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));
	int nSize = min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(
		pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(),
												 m_pSeries->GetColor2(),
												 m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesPointView

CXTPChartPointSeriesPointView::CXTPChartPointSeriesPointView(CXTPChartSeriesPoint* pPoint,
															 CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{
}

CXTPChartPointF CXTPChartPointSeriesPointView::GetScreenPoint() const
{
	CXTPChartPointSeriesView* pView = (CXTPChartPointSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_dInternalValue);
}

CXTPChartDeviceCommand*
	CXTPChartPointSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color	   = GetColor();
	CXTPChartColor color2	  = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();

	if (CXTPMathUtils::IsNan(m_dInternalValue))
		return NULL;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartPointF point = GetScreenPoint();

	pCommand->AddChildCommand(((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())
								  ->GetMarker()
								  ->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPointSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle,
														GetSeriesView()->GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));
	int nSize = min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize,
																	   GetColor(), GetColor2(),
																	   GetColor().GetDarkColor()));

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPointSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartPointSeriesStyle, "Marker", 4, OleGetMarker, SetNotSupported,
						VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPointSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartPointSeriesStyle, XTPDIID__DChartPointSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPointSeriesStyle, XTPDIID__DChartPointSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartPointSeriesStyle,
							  "Codejock.ChartPointSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartPointSeriesStyle);

LPDISPATCH CXTPChartPointSeriesStyle::OleGetMarker()
{
	return XTPGetDispatch(m_pMarker);
}

#endif
