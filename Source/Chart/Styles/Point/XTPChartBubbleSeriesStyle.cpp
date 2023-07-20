// XTPChartBubbleSeriesStyle.cpp
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

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
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Styles/Point/XTPChartPointSeriesView.h"
#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartBubbleSeriesStyle.h"
#include "Chart/Styles/Point/XTPChartBubbleSeriesLabel.h"
#include "Chart/Styles/Point/XTPChartMarker.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartBubbleSeriesStyle

IMPLEMENT_SERIAL(CXTPChartBubbleSeriesStyle, CXTPChartPointSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartBubbleSeriesStyle::CXTPChartBubbleSeriesStyle()
{
	m_bColorEach = TRUE;

	m_nTransparency = 165;

	m_dMinSize = 1.5;
	m_dMaxSize = 3.5;

	SetLabel(new CXTPChartBubbleSeriesLabel());
}

CXTPChartBubbleSeriesStyle::~CXTPChartBubbleSeriesStyle()
{
}

void CXTPChartBubbleSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesStyle::DoPropExchange(pPX);

	PX_Double(pPX, _T("MinSize"), m_dMinSize, 0);
	PX_Double(pPX, _T("MaxSize"), m_dMaxSize, 0);
}

CXTPChartSeriesView* CXTPChartBubbleSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartBubbleSeriesView(pSeries, pDiagramView);
}

void CXTPChartBubbleSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis, double /*nMinValue*/,
														double /*nMaxValue*/, double& nCorrection)
{
	if (!pAxis->IsValuesAxis())
		nCorrection = max(nCorrection, m_dMaxSize / 2);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBubbleSeriesView

CXTPChartBubbleSeriesView::CXTPChartBubbleSeriesView(CXTPChartSeries* pSeries,
													 CXTPChartDiagramView* pDiagramView)
	: CXTPChartPointSeriesView(pSeries, pDiagramView)
{
	m_dMinValue = 0;
	m_dMaxValue = 0;
}
CXTPChartSeriesPointView* CXTPChartBubbleSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBubbleSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartBubbleSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	return CXTPChartSeriesView::CreateDeviceCommand(pDC);
}

void CXTPChartBubbleSeriesView::CreatePointsView(CXTPChartDeviceContext* pDC)
{
	CXTPChartSeriesView::CreatePointsView(pDC);

	for (int i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartBubbleSeriesPointView* pPointView = (CXTPChartBubbleSeriesPointView*)
														 m_pPointsView->GetAt(i);

		double dSize = pPointView->GetPoint()->GetValue(1);

		if (i == 0)
			m_dMinValue = m_dMaxValue = dSize;
		else
		{
			m_dMinValue = min(m_dMinValue, dSize);
			m_dMaxValue = max(m_dMaxValue, dSize);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBubbleSeriesPointView

CXTPChartBubbleSeriesPointView::CXTPChartBubbleSeriesPointView(CXTPChartSeriesPoint* pPoint,
															   CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{
}

CXTPChartColor CXTPChartBubbleSeriesPointView::GetActualColor() const
{
	CXTPChartColor clr = GetColor();
	return CXTPChartColor(
		(BYTE)((CXTPChartBubbleSeriesStyle*)GetSeriesView()->GetStyle())->GetTransparency(),
		clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartBubbleSeriesPointView::GetActualColor2() const
{
	CXTPChartColor clr = GetColor2();
	return CXTPChartColor(
		(BYTE)((CXTPChartBubbleSeriesStyle*)GetSeriesView()->GetStyle())->GetTransparency(),
		clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartDeviceCommand*
	CXTPChartBubbleSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color	   = GetActualColor();
	CXTPChartColor color2	  = GetActualColor2();
	CXTPChartColor colorBorder = color.GetDarkColor();

	CXTPChartPointF point = GetScreenPoint();

	CXTPChartBubbleSeriesView* pSeriesView = (CXTPChartBubbleSeriesView*)GetSeriesView();
	CXTPChartBubbleSeriesStyle* pStyle	 = (CXTPChartBubbleSeriesStyle*)pSeriesView->GetStyle();

	CXTPChartDiagram2DAxisView* pAxisView = pSeriesView->GetAxisViewX();

	double dSize;
	double dValue = m_pPoint->GetValue(1);

	if (pStyle->GetMinSize() < CXTPMathUtils::m_dEPS
		&& pSeriesView->m_dMaxValue > CXTPMathUtils::m_dEPS)
	{
		dSize = dValue * pStyle->GetMaxSize() / pSeriesView->m_dMaxValue;
	}
	else if (fabs(pSeriesView->m_dMaxValue - pSeriesView->m_dMinValue) < CXTPMathUtils::m_dEPS)
	{
		dSize = pStyle->GetMinSize();
	}
	else
	{
		dSize = pStyle->GetMinSize()
				+ (dValue - pSeriesView->m_dMinValue)
					  * (pStyle->GetMaxSize() - pStyle->GetMinSize())
					  / (pSeriesView->m_dMaxValue - pSeriesView->m_dMinValue);
	}

	if (fabs(dSize) < CXTPMathUtils::m_dEPS)
		dSize = 0.5;

	int nWidth = (int)pAxisView->DistanceToPoint(dSize);

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	pCommand->AddChildCommand(
		((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())
			->GetMarker()
			->CreateDeviceCommand(pDC, point, nWidth, color, color2, colorBorder));

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartBubbleSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartBubbleSeriesStyle, "MinSize", 101, OleGetMinSize, OleSetMinSize,
						VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartBubbleSeriesStyle, "MaxSize", 102, OleGetMaxSize, OleSetMaxSize,
						VT_R8)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartBubbleSeriesStyle, CXTPChartPointSeriesStyle)
	INTERFACE_PART(CXTPChartBubbleSeriesStyle, XTPDIID__DChartBubbleSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBubbleSeriesStyle, XTPDIID__DChartBubbleSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartBubbleSeriesStyle,
							  "Codejock.ChartBubbleSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartBubbleSeriesStyle);

double CXTPChartBubbleSeriesStyle::OleGetMinSize()
{
	return m_dMinSize;
}

void CXTPChartBubbleSeriesStyle::OleSetMinSize(double dMinSize)
{
	m_dMinSize = dMinSize;
	OnChartChanged();
}

double CXTPChartBubbleSeriesStyle::OleGetMaxSize()
{
	return m_dMaxSize;
}

void CXTPChartBubbleSeriesStyle::OleSetMaxSize(double dMaxSize)
{
	m_dMaxSize = dMaxSize;
	OnChartChanged();
}

#endif
