// XTPChart3dPieSeriesView.cpp
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
#include "Chart/Types/XTPChartPie.h"

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
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesLabelBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesPointViewBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesViewBase.h"

#include "Chart/Styles/Pie/3D/Commands/XTPChart3dPieDeviceCommandEvents.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieDiagram.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieSeriesPointView.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieSeriesView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPieSeriesView

CXTPChart3dPieSeriesView::CXTPChart3dPieSeriesView(CXTPChartSeries* pSeries,
												   CXTPChartDiagramView* pDiagramView)
	: CXTPChartPieSeriesViewBase(pSeries, pDiagramView)
{
	Set3D();
}

void CXTPChart3dPieSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	SAFE_DELETE(m_pDomain);

	UNREFERENCED_PARAMETER(pDC);

	CalculateValues();

	m_pDomain = CreateDiagramDomain(pDC, rcBounds);

	double dAngle = 0;
	int nCount	= m_pPointsView->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPChart3dPieSeriesPointView* pPointView = STATIC_DOWNCAST(CXTPChart3dPieSeriesPointView,
																	m_pPointsView->GetAt(i));
		CXTPChart3dPieSeriesPointView* pPrevPointView = STATIC_DOWNCAST(
			CXTPChart3dPieSeriesPointView, m_pPointsView->GetAt(0 < i ? i - 1 : nCount - 1));
		CXTPChart3dPieSeriesPointView* pNextPointView = STATIC_DOWNCAST(
			CXTPChart3dPieSeriesPointView, m_pPointsView->GetAt(i < (nCount - 1) ? i + 1 : 0));

		pPointView->m_bStartGap = pPrevPointView->GetPoint()->m_bSpecial;
		pPointView->m_bEndGap   = pNextPointView->GetPoint()->m_bSpecial;

		pPointView->CalculateLayout(dAngle);
	}
}

CXTPChartSeriesPointView* CXTPChart3dPieSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChart3dPieSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChart3dPieSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	return CXTPChartSeriesView::CreateDeviceCommand(pDC);
}

CXTPChartDeviceCommand*
	CXTPChart3dPieSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcBounds);

	// No special command. 2D implementation is used.
	return NULL;
}
