// XTPChart3dPieSeriesPointView.cpp
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
#include "Common/XTPXMLHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
#include "Common/PropExchange/XTPPropExchangeRegistry.h"
#include "Common/PropExchange/XTPPropExchangeIniFile.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/Types/XTPChartPie.h"
#include "Chart/Types/3D/XTPChart3dPie.h"

#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartContentView.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesLabelBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesPointViewBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesViewBase.h"

#include "Chart/Styles/Pie/3D/Commands/XTPChart3dPieDeviceCommandEvents.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieSeriesPointView.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesPointView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPieSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChart3dPieSeriesPointView, CXTPChartPieSeriesPointViewBase);

CXTPChart3dPieSeriesPointView::CXTPChart3dPieSeriesPointView(CXTPChartSeriesPoint* pPoint,
															 CXTPChartElementView* pParentView)
	: CXTPChartPieSeriesPointViewBase(pPoint, pParentView)
	, m_p2dSeriesPointView(new CXTPChartPieSeriesPointView(pPoint, pParentView, FALSE))
	, m_bStartGap(FALSE)
	, m_bEndGap(FALSE)
{
	Set3D();
	m_pCommandSubscriptionList =
		new CXTPSubscriptionList<CXTPChartDeviceCommand, CXTPChartDeviceCommandEvents>(*this);
}

CXTPChart3dPieSeriesPointView::~CXTPChart3dPieSeriesPointView()
{
	SAFE_DELETE(m_p2dSeriesPointView);
	SAFE_DELETE(m_pCommandSubscriptionList);
}

void CXTPChart3dPieSeriesPointView::CalculateLayout(double& dStartAngle)
{
	SAFE_DELETE(m_pPie);

	CXTPChartPieSeriesViewBase* pView = (CXTPChartPieSeriesViewBase*)GetSeriesView();

	CXTPChartEllipse ellipse(CXTPPoint3d(), CXTPChart3dDiagram::m_fChartBoxSize / 2,
							 CXTPChart3dDiagram::m_fChartBoxSize / 2);

	double pieArea		= ellipse.GetArea() * m_dValue;
	double dFinishAngle = ellipse.CalcEllipseSectorFinishAngle(pieArea, dStartAngle);
	int nDepth			= ((CXTPChartPieSeriesStyleBase*)pView->GetStyle())->GetDepth();
	int nHolePercent	= ((CXTPChartPieSeriesStyleBase*)pView->GetStyle())->GetHolePercent();

	if (m_pPoint->m_bSpecial)
	{
		CXTPChartPieSeriesStyleBase* pStyle = ((CXTPChartPieSeriesStyleBase*)pView->GetStyle());
		CXTPChart3dPie pie(dStartAngle, dFinishAngle, ellipse, nDepth, nHolePercent);

		double angle	= pie.GetHalfAngle();
		double radius   = ellipse.CalcEllipseRadius(angle);
		double explodeX = pie.GetCenterPoint().X
						  + pStyle->GetExplodedDistancePercent() / 100.0 * radius * cos(angle);
		double explodeY = pie.GetCenterPoint().Y
						  - pStyle->GetExplodedDistancePercent() / 100.0 * radius * sin(angle);

		CXTPPoint3d explodeLocation(explodeX, explodeY);
		CXTPChartEllipse explodeEllipse(explodeLocation, ellipse.m_dMajorSemiaxis,
										ellipse.m_dMinorSemiaxis);
		m_pPie = new CXTPChart3dPie(dStartAngle, dFinishAngle, explodeEllipse, nDepth, nHolePercent,
									TRUE, TRUE);
	}
	else
	{
		m_pPie = new CXTPChart3dPie(dStartAngle, dFinishAngle, ellipse, nDepth, nHolePercent,
									m_bStartGap, m_bEndGap);
	}

	m_basePoint = CXTPChartPointF(0, 0);

	dStartAngle = dFinishAngle;
}

CXTPChartDeviceCommand*
	CXTPChart3dPieSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = NULL;
	UNREFERENCED_PARAMETER(pDC);

	if (NULL != m_pPie)
	{
		CXTPChartPieSeriesStyleBase* pStyle = DYNAMIC_DOWNCAST(CXTPChartPieSeriesStyleBase,
															   GetSeriesView()->GetStyle());
		ASSERT(NULL != pStyle);

		pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pPoint);

		CXTPChartColor color1				= GetColor();
		CXTPChartColor color2				= GetColor2();
		CXTPChartDeviceCommand* pPieCommand = m_pPie->CreatePieDeviceCommand(color1, color2,
																			 m_basePoint);
		pPieCommand->Subscribe(this, m_pCommandSubscriptionList);

		pStyle->ApplyTo(pPieCommand);

		pCommand->AddChildCommand(pPieCommand);
	}

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChart3dPieSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	return m_p2dSeriesPointView->CreateLegendDeviceCommand(pDC, rcBounds);
}

// CXTPChartDeviceCommandEvents overrides

void CXTPChart3dPieSeriesPointView::OnUpdateBounds(CXTPChartDeviceCommand* pCommand, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pCommand);
	m_rcBounds = rcBounds;
}

void CXTPChart3dPieSeriesPointView::OnSetConnectorPoints(CXTPChartDeviceCommand* pCommand,
														 CXTPPoint3d ptShapeCenter,
														 CXTPPoint3d ptOuterCenter)
{
	UNREFERENCED_PARAMETER(pCommand);

	m_ptShapeCenter = ptShapeCenter;
	m_ptOuterCenter = ptOuterCenter;
}
