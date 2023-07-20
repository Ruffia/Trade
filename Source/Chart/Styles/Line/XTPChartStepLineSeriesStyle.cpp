// XTPChartStepLineSeriesStyle.cpp
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

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Appearance/XTPChartLineStyle.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"
#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Chart/Styles/Line/XTPChartLineSeriesStyle.h"
#include "Chart/Styles/Line/XTPChartStepLineSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartStepLineSeriesStyle, CXTPChartLineSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepLineSeriesStyle

CXTPChartStepLineSeriesStyle::CXTPChartStepLineSeriesStyle()
{
	m_bInvertedStep = FALSE;
}

CXTPChartStepLineSeriesStyle::~CXTPChartStepLineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartStepLineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															  CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStepLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartStepLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartLineSeriesStyle::DoPropExchange(pPX);

	PX_Bool(pPX, _T("InvertedStep"), m_bInvertedStep, FALSE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepLineSeriesView

CXTPChartStepLineSeriesView::CXTPChartStepLineSeriesView(CXTPChartSeries* pSeries,
														 CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView, TRUE)
{
}
CXTPChartSeriesPointView* CXTPChartStepLineSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartStepLineSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartStepLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand;

	CXTPChartStepLineSeriesStyle* pStyle = (CXTPChartStepLineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointF pointPrev;
		CXTPChartPointSeriesPointView* pPointPrev = NULL;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartPointSeriesPointView* pPointView = (CXTPChartPointSeriesPointView*)
															m_pPointsView->GetAt(i);

			if (CXTPMathUtils::IsNan(pPointView->m_dInternalValue))
			{
				pPointPrev = NULL;
				continue;
			}

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

				pCommand->AddChildCommand(
					pStyle->GetLineStyle()->CreateDeviceCommand(pointPrev, pointMiddle,
																m_pSeries->GetColor()));

				pCommand->AddChildCommand(
					pStyle->GetLineStyle()->CreateDeviceCommand(pointMiddle, pointNext,
																m_pSeries->GetColor()));
			}

			pointPrev  = pointNext;
			pPointPrev = pPointView;
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartStepLineSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle				   = STATIC_DOWNCAST(CXTPChartPointSeriesStyle,
														 GetSeries()->GetStyle());
	CXTPChartStepLineSeriesStyle* pStepLineSeriesStyle = (CXTPChartStepLineSeriesStyle*)GetStyle();

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));
	int nSize = __min(rcBounds.Width(), rcBounds.Height());
	;

	pCommand->AddChildCommand(pStepLineSeriesStyle->GetLineStyle()->CreateDeviceCommand(
		CXTPPoint3d(rcBounds.left, rcBounds.bottom), CXTPPoint3d(rcBounds.left, ptCenter.Y),
		m_pSeries->GetColor()));
	pCommand->AddChildCommand(pStepLineSeriesStyle->GetLineStyle()->CreateDeviceCommand(
		CXTPPoint3d(rcBounds.left, ptCenter.Y), CXTPPoint3d(rcBounds.right, ptCenter.Y),
		m_pSeries->GetColor()));
	pCommand->AddChildCommand(pStepLineSeriesStyle->GetLineStyle()->CreateDeviceCommand(
		CXTPPoint3d(rcBounds.right, ptCenter.Y), CXTPPoint3d(rcBounds.right, rcBounds.top),
		m_pSeries->GetColor()));

	pCommand->AddChildCommand(
		pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(),
												 m_pSeries->GetColor2(),
												 m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepLineSeriesPointView

CXTPChartStepLineSeriesPointView::CXTPChartStepLineSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{
}

CXTPChartDeviceCommand* CXTPChartStepLineSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle,
														GetSeriesView()->GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));
	int nSize = __min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPPoint3d(rcBounds.left, rcBounds.bottom), CXTPPoint3d(rcBounds.left, ptCenter.Y),
		GetSeriesView()->m_pSeries->GetColor(), 1));
	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPPoint3d(rcBounds.left, ptCenter.Y), CXTPPoint3d(rcBounds.right, ptCenter.Y),
		GetSeriesView()->m_pSeries->GetColor(), 1));
	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
		CXTPPoint3d(rcBounds.right, ptCenter.Y), CXTPPoint3d(rcBounds.right, rcBounds.top),
		GetSeriesView()->m_pSeries->GetColor(), 1));

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize,
																	   GetColor(), GetColor2(),
																	   GetColor().GetDarkColor()));

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartStepLineSeriesStyle, CXTPChartLineSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStepLineSeriesStyle, "InvertedStep", 200, GetInvertedStep,
						SetInvertedStep, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartStepLineSeriesStyle, CXTPChartLineSeriesStyle)
	INTERFACE_PART(CXTPChartStepLineSeriesStyle, XTPDIID__DChartStepLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStepLineSeriesStyle, XTPDIID__DChartStepLineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartStepLineSeriesStyle,
							  "Codejock.ChartStepLineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartStepLineSeriesStyle);

#endif
