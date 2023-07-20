// XTPChart3dPyramidSeriesView.cpp
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

#include "StdAfx.h"

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
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesPointView.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesPointViewBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"

#include "Chart/Styles/Pyramid/3D/Commands/XTPChart3dPyramidDeviceCommandEvents.h"

#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesPointView.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

EXTERN_C CXTPChartDeviceCommand* WINAPI XTP2dPyramidCreateLegendDeviceCommand(
	CXTPChartPyramidSeriesStyleBase* pStyle, CXTPChartDeviceContext* pDC, CRect rcBounds,
	CXTPChartColor color1, CXTPChartColor color2, CXTPChartColor clrBorder);

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesView

CXTPChart3dPyramidSeriesView::CXTPChart3dPyramidSeriesView(CXTPChartSeries* pSeries,
														   CXTPChartDiagramView* pDiagramView)
	: CXTPChartPyramidSeriesViewBase(pSeries, pDiagramView)
{
	Set3D();
}

void CXTPChart3dPyramidSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	SAFE_DELETE(m_pDomain);

	UNREFERENCED_PARAMETER(pDC);

	CalculateValues();

	m_pDomain = CreateDiagramDomain(pDC, rcBounds);

	int nCount = m_pPointsView->GetCount();

	double dFrom = 0;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartPyramidSeriesPointViewBase* pPointView = (CXTPChartPyramidSeriesPointViewBase*)
															  m_pPointsView->GetAt(i);

		pPointView->CalculateLayout(i, nCount, dFrom, dFrom + pPointView->m_dValue);
		dFrom += pPointView->m_dValue;
	}
}

CXTPChartSeriesPointView* CXTPChart3dPyramidSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChart3dPyramidSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChart3dPyramidSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create3dShadeModelDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));
	return pCommand;
}

CXTPChartDeviceCommand* CXTPChart3dPyramidSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcBounds);

	// No special command. 2D implementation is used.
	return NULL;
}

CXTPChartDeviceCommand* CXTPChart3dPyramidSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds, CXTPChartColor color1, CXTPChartColor color2,
	CXTPChartColor clrBorder)
{
	CXTPChartPyramidSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyleBase,
															  GetStyle());
	return XTP2dPyramidCreateLegendDeviceCommand(pStyle, pDC, rcBounds, color1, color2, clrBorder);
}
