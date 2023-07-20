// XTPChartDiagram2DSeriesView.cpp
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
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DPane.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DSeriesView

CXTPChartDiagram2DSeriesView::CXTPChartDiagram2DSeriesView(CXTPChartSeries* pSeries,
														   CXTPChartDiagramView* pDiagramView,
														   BOOL bSortPoints)
	: CXTPChartSeriesView(pSeries, pDiagramView)
{
	m_bSortPoints = bSortPoints;
	m_pAxisViewX = m_pAxisViewY = NULL;
}

CXTPChartPointF CXTPChartDiagram2DSeriesView::GetScreenPoint(double x, double y) const
{
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)GetDiagramView();

	CXTPChartDiagram2DPaneView* pPaneView = pDiagramView->GetPaneView();

	return pPaneView->GetScreenPoint(this, x, y);
}

int AFX_CDECL CXTPChartDiagram2DSeriesView::ComparePoints(const CXTPChartSeriesPointView** ppPoint1,
														  const CXTPChartSeriesPointView** ppPoint2)
{
	double d = (*ppPoint1)->GetPoint()->GetInternalArgumentValue()
			   - (*ppPoint2)->GetPoint()->GetInternalArgumentValue();

	if (d > 0)
		return 1;
	if (d < 0)
		return -1;
	return 0;
}

void CXTPChartDiagram2DSeriesView::BeforeUpdateRange(CXTPChartDeviceContext* /*pDC*/)
{
}

void CXTPChartDiagram2DSeriesView::AfterUpdateRange(CXTPChartDeviceContext* /*pDC*/)
{
	if (m_bSortPoints)
	{
		typedef int(AFX_CDECL * GENERICCOMPAREFUNC)(const void*, const void*);

		CXTPChartElementView** pChildren = m_pPointsView->GetChildren();

		qsort(pChildren, (size_t)m_pPointsView->GetCount(), sizeof(CXTPChartSeriesPointView*),
			  (GENERICCOMPAREFUNC)ComparePoints);
	}
}
