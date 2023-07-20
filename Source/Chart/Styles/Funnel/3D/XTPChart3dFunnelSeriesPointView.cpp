// XTPChart3dFunnelSeriesPointView.cpp
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
#include "GraphicLibrary/OpenGL/GL.h"
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

#include "Chart/Styles/Funnel/Base/XTPChartFunnelDiagramBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesLabelBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesPointViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesViewBase.h"

#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesStyle.h"
#include "Chart/Styles/Funnel/3D/Commands/XTPChart3dFunnelDeviceCommandEvents.h"

#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesPointView.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesPointView.h"

#include "Chart/Styles/Funnel/3D/Commands/OpenGL/XTPChartOpenGLFunnelDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dFunnelSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChart3dFunnelSeriesPointView, CXTPChartFunnelSeriesPointViewBase);

CXTPChart3dFunnelSeriesPointView::CXTPChart3dFunnelSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartFunnelSeriesPointViewBase(pPoint, pParentView)
	, m_p2dSeriesPointView(new CXTPChartFunnelSeriesPointView(pPoint, pParentView, FALSE))
//, m_dWidth(100.)
//, m_dHeight(100.)
//, m_dFrom(0)
//, m_dTo(0)
{
	Set3D();

	m_pCommandSubscriptionList =
		new CXTPSubscriptionList<CXTPChartDeviceCommand, CXTPChartDeviceCommandEvents>(*this);
}

CXTPChart3dFunnelSeriesPointView::~CXTPChart3dFunnelSeriesPointView()
{
	SAFE_DELETE(m_p2dSeriesPointView);
	SAFE_DELETE(m_pCommandSubscriptionList);
}

void CXTPChart3dFunnelSeriesPointView::CalculateLayout(int nIndex, int nCount, double dFrom,
													   double dTo)
{
	CXTPChartFunnelSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyleBase,
															 GetSeriesView()->GetStyle());

	CXTPChartRectF rcInnerRect(0, 0,
							   CXTPChart3dDiagram::m_fChartBoxSize
								   / static_cast<float>(pStyle->GetHeightToWidthRatio()),
							   CXTPChart3dDiagram::m_fChartBoxSize);

	if (pStyle->IsEqualHeight())
	{
		m_dHeight = (rcInnerRect.Height
					 - static_cast<float>((nCount - 1) * pStyle->GetPointDistance()))
					/ static_cast<float>(nCount);
		m_dWidth = m_dHeight * 0.618;

		m_rc.X = rcInnerRect.X;
		m_rc.Y = static_cast<float>(CXTPMathUtils::Round(
			rcInnerRect.Y + (m_dHeight + pStyle->GetPointDistance()) * nIndex));

		float bottom = static_cast<float>(CXTPMathUtils::Round(
			rcInnerRect.Y + (m_dHeight + pStyle->GetPointDistance()) * nIndex + m_dHeight));
		m_rc.Height  = bottom - m_rc.Y;
		m_rc.Width   = rcInnerRect.Width;

		m_dFrom = dFrom;
		m_dTo   = dTo;
	}
	else
	{
		m_rc.X = rcInnerRect.X;

		double dPosFrom = rcInnerRect.Height * dFrom;
		double dPosTo   = rcInnerRect.Height * dTo;

		m_rc.Y = static_cast<float>(CXTPMathUtils::Round(rcInnerRect.Y + dPosFrom));

		float bottom = static_cast<float>(CXTPMathUtils::Round(rcInnerRect.Y + dPosTo));

		if (nIndex != nCount - 1)
			bottom -= static_cast<float>(pStyle->GetPointDistance());

		m_rc.Height = max(1.0f, bottom - m_rc.Y);
		m_rc.Width  = rcInnerRect.Width;

		dPosTo = m_rc.GetBottom() - rcInnerRect.Y;

		m_dHeight = rcInnerRect.Height / 3;
		m_dWidth  = m_dHeight * 0.618;

		if (dPosFrom < rcInnerRect.Height - static_cast<double>(static_cast<int>(m_dHeight)))
		{
			m_dFrom = (m_dWidth / rcInnerRect.Width - 1) / (rcInnerRect.Height - m_dHeight)
						  * dPosFrom
					  + 1;
		}
		else
		{
			m_dFrom = m_dWidth / rcInnerRect.Width;
		}

		if (dPosTo < rcInnerRect.Height - static_cast<double>(static_cast<int>(m_dHeight)))
		{
			m_dTo = (m_dWidth / rcInnerRect.Width - 1) / (rcInnerRect.Height - m_dHeight) * dPosTo
					+ 1;
		}
		else
		{
			m_dTo = m_dWidth / rcInnerRect.Width;
		}
	}
}

CXTPChartDeviceCommand*
	CXTPChart3dFunnelSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChart3dFunnelSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChart3dFunnelSeriesStyle,
															GetSeriesView()->GetStyle());
	ASSERT(NULL != pStyle);

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartDeviceCommand* pFunnelCommand = new CXTPChartOpenGLFunnelDeviceCommand(
		m_ptShapeCenter, m_rc, m_dWidth, m_dHeight, m_dFrom, m_dTo, GetColor(),
		pStyle->Is3dEdgeSmoothingEnabled());
	pFunnelCommand->Subscribe(this, m_pCommandSubscriptionList);

	pStyle->ApplyTo(pFunnelCommand);

	pCommand->AddChildCommand(pFunnelCommand);

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChart3dFunnelSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	return m_p2dSeriesPointView->CreateLegendDeviceCommand(pDC, rcBounds);
}

// CXTPChartDeviceCommandEvents overrides

void CXTPChart3dFunnelSeriesPointView::OnUpdateBounds(CXTPChartDeviceCommand* pCommand,
													  CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pCommand);
	m_rcBounds = rcBounds;
}

void CXTPChart3dFunnelSeriesPointView::OnSetConnectorPoints(CXTPChartDeviceCommand* pCommand,
															CXTPPoint3d ptShapeCenter,
															CXTPPoint3d ptOuterCenter)
{
	UNREFERENCED_PARAMETER(pCommand);

	m_ptShapeCenter = ptShapeCenter;
	m_ptOuterCenter = ptOuterCenter;
}
