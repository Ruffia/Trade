// XTPChartStackedBarSeriesStyle.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/XTPChartSeriesPoint.h"

#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Styles/Bar/XTPChartBarSeriesView.h"
#include "Chart/Styles/Bar/XTPChartBarSeriesStyle.h"
#include "Chart/Styles/Bar/XTPChartStackedBarSeriesStyle.h"
#include "Chart/Styles/Bar/XTPChartBarSeriesLabel.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedBarSeriesStyle

IMPLEMENT_SERIAL(CXTPChartStackedBarSeriesStyle, CXTPChartBarSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartStackedBarSeriesStyle::CXTPChartStackedBarSeriesStyle()
{
	m_dStackHeight = 0;
	m_nStackGroup  = 0;

	CXTPChartBarSeriesLabel* pLabel = (CXTPChartBarSeriesLabel*)SetLabel(
		new CXTPChartBarSeriesLabel());
	pLabel->SetPosition(xtpChartBarLabelCenter);
}

CXTPChartStackedBarSeriesStyle::~CXTPChartStackedBarSeriesStyle()
{
}

void CXTPChartStackedBarSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartBarSeriesStyle::DoPropExchange(pPX);

	PX_Double(pPX, _T("StackHeight"), m_dStackHeight, 0);
	PX_Int(pPX, _T("StackGroup"), m_nStackGroup, 0);
}

CXTPChartSeriesView* CXTPChartStackedBarSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStackedBarSeriesView(pSeries, pDiagramView);
}

void CXTPChartStackedBarSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis,
															double /*nMinValue*/,
															double /*nMaxValue*/,
															double& nCorrection)
{
	if (!pAxis->IsValuesAxis())
		nCorrection = max(nCorrection, 0.5);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedBarSeriesView

CXTPChartStackedBarSeriesView::CXTPChartStackedBarSeriesView(CXTPChartSeries* pSeries,
															 CXTPChartDiagramView* pDiagramView)
	: CXTPChartBarSeriesView(pSeries, pDiagramView)
{
	m_nBarCount = 0;
	m_nBarIndex = -1;
}

CXTPChartStackedBarSeriesView::~CXTPChartStackedBarSeriesView()
{
}

CXTPChartSeriesPointView* CXTPChartStackedBarSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartStackedBarSeriesPointView(pPoint, pParentView);
}

void CXTPChartStackedBarSeriesView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	CXTPChartBarSeriesView::UpdateRange(pDC);

	CXTPChartDiagramView* pDiagramView = GetDiagramView();

	BOOL bFound = FALSE;

	for (int i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		if (DYNAMIC_DOWNCAST(CXTPChartStackedBarSeriesStyle, pView->GetSeries()->GetStyle()))
		{
			CXTPChartStackedBarSeriesView* pStackedBarSeriesView = (CXTPChartStackedBarSeriesView*)
				pView;

			if (pStackedBarSeriesView->m_bFirstStack && pView == this)
			{
				bFound = TRUE;
				break;
			}
		}
	}
	if (!bFound)
		return;

	CMap<double, double, double, double> mapValues;

	for (int j = 0; j < pDiagramView->GetSeriesView()->GetCount(); j++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(j);

		if (DYNAMIC_DOWNCAST(CXTPChartStackedBarSeriesStyle, pView->GetSeries()->GetStyle()))
		{
			CXTPChartStackedBarSeriesView* pStackedBarSeriesView = (CXTPChartStackedBarSeriesView*)
				pView;
			if (pStackedBarSeriesView->m_nBarIndex != m_nBarIndex)
				continue;

			for (int i = 0; i < pStackedBarSeriesView->m_pPointsView->GetCount(); i++)
			{
				CXTPChartStackedBarSeriesPointView* pPointView =
					(CXTPChartStackedBarSeriesPointView*)
						pStackedBarSeriesView->m_pPointsView->GetAt(i);

				double dValue = pPointView->GetPoint()->GetValue(0);

				pPointView->m_dValueFrom =
					mapValues[pPointView->GetPoint()->GetInternalArgumentValue()];
				pPointView->m_dInternalValue = pPointView->m_dValueTo = pPointView->m_dValueFrom
																		+ dValue;

				mapValues[pPointView->GetPoint()->GetInternalArgumentValue()] =
					pPointView->m_dValueTo;
			}
		}
	}

	double dStackHeight = ((CXTPChartStackedBarSeriesStyle*)GetStyle())->GetStackHeight();
	if (dStackHeight != 0)
	{
		for (int j = 0; j < pDiagramView->GetSeriesView()->GetCount(); j++)
		{
			CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(
				j);

			if (DYNAMIC_DOWNCAST(CXTPChartStackedBarSeriesStyle, pView->GetSeries()->GetStyle()))
			{
				CXTPChartStackedBarSeriesView* pStackedBarSeriesView =
					(CXTPChartStackedBarSeriesView*)pView;
				if (pStackedBarSeriesView->m_nBarIndex != m_nBarIndex)
					continue;

				for (int i = 0; i < pStackedBarSeriesView->m_pPointsView->GetCount(); i++)
				{
					CXTPChartStackedBarSeriesPointView* pPointView =
						(CXTPChartStackedBarSeriesPointView*)
							pStackedBarSeriesView->m_pPointsView->GetAt(i);

					double dLimit = mapValues[pPointView->GetPoint()->GetInternalArgumentValue()];

					pPointView->m_dValueFrom	 = pPointView->m_dValueFrom * dStackHeight / dLimit;
					pPointView->m_dInternalValue = pPointView->m_dValueTo = pPointView->m_dValueTo
																			* dStackHeight / dLimit;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedBarSeriesPointView

CXTPChartStackedBarSeriesPointView::CXTPChartStackedBarSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartBarSeriesPointView(pPoint, pParentView)
{
}

CXTPChartRectF CXTPChartStackedBarSeriesPointView::GetScreenRect() const
{
	CXTPChartStackedBarSeriesView* pSeriesView   = (CXTPChartStackedBarSeriesView*)GetSeriesView();
	CXTPChartStackedBarSeriesStyle* pSeriesStyle = STATIC_DOWNCAST(CXTPChartStackedBarSeriesStyle,
																   GetSeriesView()->GetStyle());

	CXTPChartDiagram2DAxisView* pAxisXView = pSeriesView->GetAxisViewX();

	if (pSeriesView->m_nBarIndex != -1)
	{
		int nCount = pSeriesView->m_nBarCount;
		int nIndex = pSeriesView->m_nBarIndex;

		double dOffset = 3 / pAxisXView->GetScale();

		double dBarWidth = (pSeriesStyle->GetBarWidth() - dOffset * (nCount - 1)) / nCount;

		CXTPChartPointF ptTopLeft(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue()
																  - pSeriesStyle->GetBarWidth() / 2
																  + dBarWidth * nIndex
																  + dOffset * nIndex,
															  m_dValueTo));

		CXTPChartPointF ptBottomRight(
			pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue()
											- pSeriesStyle->GetBarWidth() / 2 + dBarWidth * nIndex
											+ dOffset * nIndex + dBarWidth,
										m_dValueFrom));

		CXTPChartRectF rc(ptTopLeft, ptBottomRight);
		rc.Width++;
		rc.Height++;

		return rc;
	}

	return CXTPChartRectF(0, 0, 0, 0);
}

CXTPChartDeviceCommand*
	CXTPChartStackedBarSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	return CXTPChartBarSeriesPointView::CreateDeviceCommand(pDC);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartStackedBarSeriesStyle, CXTPChartBarSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStackedBarSeriesStyle, "StackHeight", 14, OleGetStackHeight,
						OleSetStackHeight, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartStackedBarSeriesStyle, "StackGroup", 15, GetStackGroup,
						SetStackGroup, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartStackedBarSeriesStyle, CXTPChartBarSeriesStyle)
	INTERFACE_PART(CXTPChartStackedBarSeriesStyle, XTPDIID__DChartStackedBarSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStackedBarSeriesStyle, XTPDIID__DChartStackedBarSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartStackedBarSeriesStyle,
							  "Codejock.ChartStackedBarSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartStackedBarSeriesStyle);

double CXTPChartStackedBarSeriesStyle::OleGetStackHeight()
{
	return GetStackHeight();
}

void CXTPChartStackedBarSeriesStyle::OleSetStackHeight(double dStackHeight)
{
	SetStackHeight(dStackHeight);
}

#endif
