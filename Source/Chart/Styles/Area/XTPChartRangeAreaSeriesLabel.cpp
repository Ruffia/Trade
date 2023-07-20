// XTPChartRangeAreaSeriesLabel.cpp
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
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/Styles/Point/XTPChartPointSeriesLabel.h"
#include "Chart/Styles/Point/XTPChartPointSeriesLabelView.h"

#include "Chart/Styles/Area/XTPChartRangeAreaSeriesLabel.h"
#include "Chart/Styles/Area/XTPChartAreaSeriesView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeAreaSeriesLabel
IMPLEMENT_SERIAL(CXTPChartRangeAreaSeriesLabel, CXTPChartPointSeriesLabel,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRangeAreaSeriesLabel::CXTPChartRangeAreaSeriesLabel()
{
}

CXTPChartRangeAreaSeriesLabel::~CXTPChartRangeAreaSeriesLabel()
{
}

void CXTPChartRangeAreaSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesLabel::DoPropExchange(pPX);
}

CXTPChartElementView* CXTPChartRangeAreaSeriesLabel::CreateView(CXTPChartDeviceContext* pDC,
																CXTPChartSeriesPointView* pPointView,
																CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeAreaSeriesLabelView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeAreaSeriesLabelView

CXTPChartRangeAreaSeriesLabelView::CXTPChartRangeAreaSeriesLabelView(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesLabelView(pLabel, pPointView, pParentView)
{
}

CXTPChartPointF CXTPChartRangeAreaSeriesLabelView::GetAnchorPoint(BOOL bMinValue) const
{
	CXTPChartAreaSeriesView* pView = (CXTPChartAreaSeriesView*)m_pPointView->GetSeriesView();
	CXTPChartSeriesPoint* pPoint   = m_pPointView->GetPoint();

	return pView->GetScreenPoint(pPoint->GetInternalArgumentValue(),
								 pPoint->GetValue(bMinValue ? 0 : 1));
}

void CXTPChartRangeAreaSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
	CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(),
													   strText);

	strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
	CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(),
													   strText);
}

CXTPChartDeviceCommand*
	CXTPChartRangeAreaSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
	pCommand->AddChildCommand(
		CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(TRUE),
															   GetAnchorAngle(), strText));

	strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
	pCommand->AddChildCommand(
		CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(FALSE),
															   GetAnchorAngle(), strText));

	return pCommand;
}
