// XTPChart2dPieSeriesView.cpp
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

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPTypeId.h"
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
#include "Common/Math/XTPMathUtils.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/Types/XTPChartPie.h"
#include "Chart/Types/2D/XTPChart2dPie.h"

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

#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesPointView.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesView

CXTPChartPieSeriesView::CXTPChartPieSeriesView(CXTPChartSeries* pSeries,
											   CXTPChartDiagramView* pDiagramView)
	: CXTPChartPieSeriesViewBase(pSeries, pDiagramView)
{
}

void CXTPChartPieSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	SAFE_DELETE(m_pDomain);

	UNREFERENCED_PARAMETER(pDC);

	CalculateValues();

	m_pDomain = CreateDiagramDomain(pDC, rcBounds);

	double circleStartAngle = CXTPMathUtils::Degree2Radian(
		((CXTPChartPieSeriesStyleBase*)GetStyle())->GetRotation());

	CXTPChartEllipse ellipse(CXTPPoint3d(), m_pDomain->GetInnerBounds().Width / 2,
							 m_pDomain->GetInnerBounds().Height / 2);

	double dAngle = ellipse.CalcEllipseAngleFromCircleAngle(circleStartAngle);

	for (int i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPieSeriesPointViewBase* pPointView = (CXTPChartPieSeriesPointViewBase*)
														  m_pPointsView->GetAt(i);

		pPointView->CalculateLayout(dAngle);
	}
}

CXTPChartSeriesPointView* CXTPChartPieSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartPieSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pCommand;
}

CXTPChartDeviceCommand*
	CXTPChartPieSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	CXTPChartColor color1 = m_pSeries->GetColor();
	CXTPChartColor color2 = m_pSeries->GetColor2();

	CXTPChartEllipse explodeEllipse(CXTPChartPointF(static_cast<int>(rcBounds.left),
													static_cast<int>(rcBounds.bottom)),
									rcBounds.Width(), rcBounds.Height());

	CXTPChartPie pie(0, CXTPMathUtils::m_dPI / 2, explodeEllipse, 0, 0);
	pCommand->AddChildCommand(pie.CreatePieDeviceCommand(color1, color2, CXTPChartPointF(0, 0)));

	CXTPChartPieSeriesStyleBase* pStyle = DYNAMIC_DOWNCAST(CXTPChartPieSeriesStyleBase, GetStyle());
	if (pStyle)
	{
		if (pStyle->GetBorder()->IsVisible())
		{
			pCommand->AddChildCommand(
				pie.CreateBoundedPieDeviceCommand(pStyle->GetBorderActualColor(), 1,
												  CXTPChartPointF(0, 0)));
		}
	}
	else
	{
		CXTPChartColor clrBorder = GetStyle()->GetAppearance()->GetPieStyleAppearance()->BorderColor;

		pCommand->AddChildCommand(
			pie.CreateBoundedPieDeviceCommand(clrBorder, 1, CXTPChartPointF(0, 0)));
	}

	return pCommand;
}
