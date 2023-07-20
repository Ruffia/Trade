// XTPChart3dPyramidSeriesPointView.cpp
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

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidDiagramBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesPointViewBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"

#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesStyle.h"
#include "Chart/Styles/Pyramid/3D/Commands/XTPChart3dPyramidDeviceCommandEvents.h"

#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesPointView.h"
#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidSeriesPointView.h"

#include "Chart/Styles/Pyramid/3D/Commands/OpenGL/XTPChartOpenGLPyramidDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChart3dPyramidSeriesPointView, CXTPChartPyramidSeriesPointViewBase);

CXTPChart3dPyramidSeriesPointView::CXTPChart3dPyramidSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPyramidSeriesPointViewBase(pPoint, pParentView)
	, m_p2dSeriesPointView(new CXTPChartPyramidSeriesPointView(pPoint, pParentView, FALSE))
	, m_dWidth(100.)
	, m_dHeight(100.)
	, m_dFrom(0)
	, m_dTo(0)
{
	Set3D();

	m_pCommandSubscriptionList =
		new CXTPSubscriptionList<CXTPChartDeviceCommand, CXTPChartDeviceCommandEvents>(*this);
}

CXTPChart3dPyramidSeriesPointView::~CXTPChart3dPyramidSeriesPointView()
{
	SAFE_DELETE(m_p2dSeriesPointView);
	SAFE_DELETE(m_pCommandSubscriptionList);
}

void CXTPChart3dPyramidSeriesPointView::CalculateLayout(int nIndex, int nCount, double dFrom,
														double dTo)
{
	const double MinimalSliceHeight = 0.01;

	CXTPChartPyramidSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyleBase,
															  GetSeriesView()->GetStyle());

	m_dFrom = dFrom;
	m_dTo   = dTo;
	if (nIndex != nCount - 1)
	{
		m_dTo -= pStyle->GetPointDistance() / 100.;
		if ((m_dTo - m_dFrom) < MinimalSliceHeight)
		{
			m_dTo = m_dFrom + MinimalSliceHeight;
		}
	}

	double dRatio = pStyle->GetHeightToWidthRatio();
	if (dRatio > CXTPMathUtils::m_dEPS)
	{
		m_dWidth = m_dHeight / dRatio;
	}
}

CXTPChartDeviceCommand*
	CXTPChart3dPyramidSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChart3dPyramidSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesStyle,
															 GetSeriesView()->GetStyle());
	ASSERT(NULL != pStyle);

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartDeviceCommand* pPyramidCommand =
		new CXTPChartOpenGLPyramidDeviceCommand(m_ptShapeCenter, m_dWidth, m_dHeight, m_dFrom,
												m_dTo, color1, pStyle->Is3dEdgeSmoothingEnabled());
	pPyramidCommand->Subscribe(this, m_pCommandSubscriptionList);

	pStyle->ApplyTo(pPyramidCommand);

	pCommand->AddChildCommand(pPyramidCommand);

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChart3dPyramidSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	return m_p2dSeriesPointView->CreateLegendDeviceCommand(pDC, rcBounds);
}

// CXTPChartDeviceCommandEvents overrides

void CXTPChart3dPyramidSeriesPointView::OnUpdateBounds(CXTPChartDeviceCommand* pCommand,
													   CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pCommand);
	m_rcBounds = rcBounds;
}

void CXTPChart3dPyramidSeriesPointView::OnSetConnectorPoints(CXTPChartDeviceCommand* pCommand,
															 CXTPPoint3d ptShapeCenter,
															 CXTPPoint3d ptOuterCenter)
{
	UNREFERENCED_PARAMETER(pCommand);

	m_ptShapeCenter = ptShapeCenter;
	m_ptOuterCenter = ptOuterCenter;
}
