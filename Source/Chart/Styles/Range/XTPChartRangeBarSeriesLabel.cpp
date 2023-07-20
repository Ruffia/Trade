// XTPChartRangeBarSeriesLabel.cpp
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
#include "Common/XTPSystemHelpers.h"
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
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/Styles/Bar/XTPChartBarSeriesView.h"

#include "Chart/Styles/Range/XTPChartRangeBarSeriesLabel.h"
#include "Chart/Styles/Range/XTPChartRangeBarSeriesView.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesLabel
IMPLEMENT_SERIAL(CXTPChartRangeBarSeriesLabel, CXTPChartDiagram2DSeriesLabel,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRangeBarSeriesLabel::CXTPChartRangeBarSeriesLabel()
{
	m_nPosition = xtpChartRangeBarLabelOutside;
	m_nType		= xtpChartRangeBarTwoLabels;

	m_nLineLength = 5;
}

CXTPChartRangeBarSeriesLabel::~CXTPChartRangeBarSeriesLabel()
{
}

void CXTPChartRangeBarSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartRangeBarLabelOutside);
	PX_Enum(pPX, _T("Type"), m_nType, xtpChartRangeBarTwoLabels);
}

CXTPChartElementView* CXTPChartRangeBarSeriesLabel::CreateView(CXTPChartDeviceContext* pDC,
															   CXTPChartSeriesPointView* pPointView,
															   CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeBarSeriesLabelView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesLabelView

CXTPChartRangeBarSeriesLabelView::CXTPChartRangeBarSeriesLabelView(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartDiagram2DSeriesLabelView(pLabel, pPointView, pParentView)
{
}

double CXTPChartRangeBarSeriesLabelView::GetAnchorAngle(BOOL bMinValue) const
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	CXTPChartDiagram2DSeriesView* pView = (CXTPChartDiagram2DSeriesView*)
											  m_pPointView->GetSeriesView();

	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pView->GetDiagram();

	BOOL bTop = TRUE;
	if (bMinValue)
		bTop = !bTop;

	if (pView->GetAxisViewY()->GetAxis()->IsReversed())
		bTop = !bTop;

	if (pLabel->GetPosition() == xtpChartRangeBarLabelInside)
		bTop = !bTop;

	if (m_pPointView->GetPoint()->GetValue(0) > m_pPointView->GetPoint()->GetValue(1))
		bTop = !bTop;

	if (pDiagram->IsRotated())
	{
		if (!bTop)
			return 180;
		return 0;
	}
	else
	{
		if (!bTop)
			return 270;
		return 90;
	}
}

CXTPChartPointF CXTPChartRangeBarSeriesLabelView::GetAnchorPoint(BOOL bMinValue) const
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	if (pLabel->GetType() == xtpChartRangeBarOneLabel)
	{
		CXTPChartRectF rc(((CXTPChartRangeBarSeriesPointView*)m_pPointView)->GetScreenRect());
		return rc.GetCenter();
	}
	else
	{
		return ((CXTPChartRangeBarSeriesPointView*)m_pPointView)->GetScreenPoint(bMinValue);
	}
}

void CXTPChartRangeBarSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	if (pLabel->GetType() == xtpChartRangeBarOneLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0) + _T(", ")
						  + m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(0), strText);
	}
	else if (pLabel->GetType() == xtpChartRangeBarTwoLabels)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(TRUE),
														   GetAnchorAngle(TRUE), strText);

		strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(FALSE),
														   GetAnchorAngle(FALSE), strText);
	}
	else if (pLabel->GetType() == xtpChartRangeBarMinValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(TRUE),
														   GetAnchorAngle(TRUE), strText);
	}
	else if (pLabel->GetType() == xtpChartRangeBarMaxValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(FALSE),
														   GetAnchorAngle(FALSE), strText);
	}
}

CXTPChartDeviceCommand*
	CXTPChartRangeBarSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	if (pLabel->GetType() == xtpChartRangeBarOneLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0) + _T(", ")
						  + m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(0),
																	  strText);
	}

	if (pLabel->GetType() == xtpChartRangeBarTwoLabels)
	{
		CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		pCommand->AddChildCommand(
			CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(TRUE),
																   GetAnchorAngle(TRUE), strText));

		strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		pCommand->AddChildCommand(
			CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(FALSE),
																   GetAnchorAngle(FALSE), strText));

		return pCommand;
	}

	if (pLabel->GetType() == xtpChartRangeBarMinValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(TRUE),
																	  GetAnchorAngle(TRUE),
																	  strText);
	}

	if (pLabel->GetType() == xtpChartRangeBarMaxValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(FALSE),
																	  GetAnchorAngle(FALSE),
																	  strText);
	}

	return 0;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRangeBarSeriesLabel, CXTPChartDiagram2DSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartRangeBarSeriesLabel, "Position", 106, m_nPosition,
							OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartRangeBarSeriesLabel, "Type", 107, m_nType, OleChartChanged,
							VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRangeBarSeriesLabel, CXTPChartDiagram2DSeriesLabel)
	INTERFACE_PART(CXTPChartRangeBarSeriesLabel, XTPDIID_ChartRangeBarSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRangeBarSeriesLabel, XTPDIID_ChartRangeBarSeriesLabel)

#endif
