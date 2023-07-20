// XTPChartDiagramView.cpp
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPTypeId.h"

#include "Chart/Resource.h"
#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartTitle.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagramView

IMPLEMENT_DYNAMIC(CXTPChartDiagramView, CXTPChartElementView);

CXTPChartDiagramView::CXTPChartDiagramView(CXTPChartDiagram* pDiagram,
										   CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pDiagram = pDiagram;
	m_rcBounds.SetRectEmpty();
	m_pLabelsView = NULL;
	m_pSeriesView = NULL;

	m_hcurNormalHand = XTPResourceManager()->LoadCursor(XTP_IDC_CHART_NORMALHAND);
	m_hcurDragHand   = XTPResourceManager()->LoadCursor(XTP_IDC_CHART_DRAGHAND);

	m_pTitlesView = new CXTPChartElementView(this);
}

void CXTPChartDiagramView::CreateView(CXTPChartDeviceContext* pDC)
{
	m_pDiagram->GetTitles()->CreateView(pDC, m_pTitlesView);
}

void CXTPChartDiagramView::CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_pDiagram->GetTitles()->CalculateView(pDC, rcBounds, m_pTitlesView);

	m_rcBounds = rcBounds;
	if (NULL != m_pSeriesView)
	{
		m_pDiagram->CalculateSeriesLayout(pDC, this);
	}
}

void CXTPChartDiagramView::UpdateRange(CXTPChartDeviceContext* /*pDC*/)
{
}

CXTPChartDeviceCommand* CXTPChartDiagramView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pDiagram, m_rcBounds);

	pCommand->AddChildCommand(CXTPChartElementView::CreateDeviceCommand(pDC));

	return pCommand;
}

void CXTPChartDiagramView::OnMouseWheel(UINT /*nFlags*/, short /*zDelta*/, CPoint /*pt*/)
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dDiagramView

IMPLEMENT_DYNAMIC(CXTPChart3dDiagramView, CXTPChartDiagramView);

CXTPChart3dDiagramView::CXTPChart3dDiagramView(CXTPChartDiagram* pDiagram,
											   CXTPChartElementView* pParentView)
	: CXTPChartDiagramView(pDiagram, pParentView)
{
	ASSERT_VALID(pDiagram);

	m_p3dDiagram = DYNAMIC_DOWNCAST(CXTPChart3dDiagram, pDiagram);
	ASSERT(NULL != m_p3dDiagram);
}

CXTPChartDeviceCommand* CXTPChart3dDiagramView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext, pDC);
	ASSERT(NULL != p3DDC);

	m_p3dDiagram->SetArcBallBounds(p3DDC->GetBounds().Width(), p3DDC->GetBounds().Height());

	p3DDC->SetModelDistanceLimits(CXTPChart3dDiagram::m_fMinModelDistance,
								  CXTPChart3dDiagram::m_fMaxModelDistance);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pDiagram, m_rcBounds);
	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create3dCameraDeviceCommand(
		m_p3dDiagram->GetCameraPosition(), CXTPPoint3f(0, 0, 0), CXTPPoint3f(0, 1, 0)));
	pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create3dMultiplyMatrixDeviceCommand(
		m_p3dDiagram->GetViewRotationMatrix(), FALSE));
	pCommand->AddChildCommand(CXTPChartElementView::CreateDeviceCommand(pDC));

	return pCommand;
}

void CXTPChart3dDiagramView::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPChartDiagramView::OnMouseMove(nFlags, point);

	if (m_p3dDiagram->RotateArcBallToPoint(point))
	{
		ASSERT(NULL != m_pContainer);
		m_pContainer->OnChartChanged(xtpChartUpdateRedraw);
	}
}

void CXTPChart3dDiagramView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPChartDiagramView::OnLButtonDown(nFlags, point);

	ASSERT(NULL != m_pContainer);
	m_pContainer->SetCapture(this);

	if (m_p3dDiagram->SetStartArcBallRotationPoint(point))
	{
		m_pContainer->BeginFrequentUpdates();

		::SetCursor(m_hcurDragHand);
	}
}

void CXTPChart3dDiagramView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CXTPChartDiagramView::OnLButtonUp(nFlags, point);

	if (NULL != m_pContainer && m_pContainer->IsBeingFrequentlyUpdated())
	{
		m_pContainer->EndFrequentUpdates();
	}
}

void CXTPChart3dDiagramView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	CXTPChartDiagramView::OnMouseWheel(nFlags, zDelta, point);

	m_p3dDiagram->ZoomArcBall((static_cast<float>(zDelta) / WHEEL_DELTA) * 15);

	ASSERT(NULL != m_pContainer);
	m_pContainer->OnChartChanged(xtpChartUpdateRedraw);
}

BOOL CXTPChart3dDiagramView::OnSetCursor(CPoint /*point*/)
{
	BOOL bHandled = FALSE;

	if (NULL != m_pContainer && m_pContainer->IsBeingFrequentlyUpdated())
	{
		::SetCursor(m_hcurDragHand);
		bHandled = TRUE;
	}

	return bHandled;
}
