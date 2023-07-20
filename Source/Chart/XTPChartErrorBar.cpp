// XTPChartErrorBar.cpp
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
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/Appearance/XTPChartLineStyle.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartErrorBar.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartErrorBar

IMPLEMENT_SERIAL(CXTPChartErrorBar, CXTPChartElement, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartErrorBar::CXTPChartErrorBar()
	: m_bVisible(FALSE)
	, m_endStyle(xtpChartErrorBarEndStyleCap)
	, m_type(xtpChartErrorBarTypeNone)
	, m_direction(xtpChartErrorBarDirectionBoth)
	, m_nCapLength(5)
	, m_dAmount(0)
	, m_dValuePlus(0)
	, m_dValueMinus(0)
	, m_pLineStyle(NULL)
	, m_clrLine(CXTPChartColor::Black)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_pLineStyle = new CXTPChartLineStyle(this);
}

CXTPChartErrorBar::~CXTPChartErrorBar()
{
	SAFE_DELETE(m_pLineStyle);
}

CXTPChartElementView* CXTPChartErrorBar::CreateView(CXTPChartDeviceContext* pDC,
													CXTPChartSeriesPointView* pPointView,
													CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartErrorBarView(this, pPointView, pParentView);
}

XTPChartErrorBarPosition CXTPChartErrorBar::GetPosition() const
{
	XTPChartErrorBarPosition position = xtpChartErrorBarPositionNone;

	switch (m_type)
	{
		case xtpChartErrorBarTypeFixedValue:
		case xtpChartErrorBarTypePercentage:
		case xtpChartErrorBarTypeStandardDeviation:
		case xtpChartErrorBarTypeStandardError:
		case xtpChartErrorBarTypeCustom: position = xtpChartErrorBarPositionTop; break;
	}

	return position;
}

/////////////////////////////////////////////////////////////////////////////

CXTPChartErrorBarView::CXTPChartErrorBarView(CXTPChartErrorBar* pErrorBar,
											 CXTPChartSeriesPointView* pPointView,
											 CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
	, m_pErrorBar(pErrorBar)
	, m_pPointView(pPointView)
{
}

CXTPChartDeviceCommand* CXTPChartErrorBarView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pErrorBar);

	CXTPChartDiagram2DSeriesView* pSeriesView = static_cast<CXTPChartDiagram2DSeriesView*>(
		m_pPointView->GetSeriesView());

	CXTPChartDiagram2D* pDiagram = static_cast<CXTPChartDiagram2D*>(pSeriesView->GetDiagram());

	double dCenter, dPlus, dMinus;
	GetValues(dCenter, dPlus, dMinus);

	CXTPChartPointF ptPointCenter =
		pSeriesView->GetScreenPoint(m_pPointView->GetPoint()->GetInternalArgumentValue(), dCenter);

	CXTPChartPointF ptPointPlus =
		pSeriesView->GetScreenPoint(m_pPointView->GetPoint()->GetInternalArgumentValue(), dPlus);

	CXTPChartPointF ptPointMinus =
		pSeriesView->GetScreenPoint(m_pPointView->GetPoint()->GetInternalArgumentValue(), dMinus);

	double dLineAngle = CXTPMathUtils::Degree2Radian(GetAnchorAngle(m_pErrorBar->GetPosition()));
	double dLineLengthPlus;
	double dLineLengthMinus;

	if (pDiagram->IsRotated())
	{
		dLineLengthPlus  = ::fabs(ptPointPlus.X - ptPointCenter.X);
		dLineLengthMinus = ::fabs(ptPointMinus.X - ptPointCenter.X);
	}
	else
	{
		dLineLengthPlus  = ::fabs(ptPointPlus.Y - ptPointCenter.Y);
		dLineLengthMinus = ::fabs(ptPointMinus.Y - ptPointCenter.Y);
	}

	const double dCos = ::cos(dLineAngle);
	const double dSin = ::sin(dLineAngle);

	const int nCapLength = m_pErrorBar->m_nCapLength;

	CXTPChartPointF ptStartPoint(CXTPMathUtils::Round(ptPointCenter.X),
								 CXTPMathUtils::Round(ptPointCenter.Y));

	CXTPChartPointF ptFinishPointPlus(
		CXTPMathUtils::Round(ptPointCenter.X + (dCos * dLineLengthPlus)),
		CXTPMathUtils::Round(ptPointCenter.Y - (dSin * dLineLengthPlus)));

	CXTPChartPointF ptFinishPointMinus(
		CXTPMathUtils::Round(ptPointCenter.X - (dCos * dLineLengthMinus)),
		CXTPMathUtils::Round(ptPointCenter.Y + (dSin * dLineLengthMinus)));

	CXTPChartPointF ptFinishPointPlusCapStart(
		CXTPMathUtils::Round(ptFinishPointPlus.X - (dSin * nCapLength)),
		CXTPMathUtils::Round(ptFinishPointPlus.Y - (dCos * nCapLength)));
	CXTPChartPointF ptFinishPointPlusCapEnd(
		CXTPMathUtils::Round(ptFinishPointPlus.X + (dSin * nCapLength)),
		CXTPMathUtils::Round(ptFinishPointPlus.Y + (dCos * nCapLength)));

	CXTPChartPointF ptFinishPointMinusCapStart(
		CXTPMathUtils::Round(ptFinishPointMinus.X - (dSin * nCapLength)),
		CXTPMathUtils::Round(ptFinishPointMinus.Y - (dCos * nCapLength)));
	CXTPChartPointF ptFinishPointMinusCapEnd(
		CXTPMathUtils::Round(ptFinishPointMinus.X + (dSin * nCapLength)),
		CXTPMathUtils::Round(ptFinishPointMinus.Y + (dCos * nCapLength)));

	// Plus
	if (xtpChartErrorBarDirectionPlus == m_pErrorBar->GetDirection()
		|| xtpChartErrorBarDirectionBoth == m_pErrorBar->GetDirection())
	{
		pCommand->AddChildCommand(
			m_pErrorBar->GetLineStyle()->CreateDeviceCommand(ptStartPoint, ptFinishPointPlus,
															 m_pErrorBar->GetLineColor()));

		if (xtpChartErrorBarEndStyleCap == m_pErrorBar->GetEndStyle())
		{
			pCommand->AddChildCommand(m_pErrorBar->GetLineStyle()->CreateDeviceCommand(
				ptFinishPointPlusCapStart, ptFinishPointPlusCapEnd, m_pErrorBar->GetLineColor()));
		}
	}

	// Minus
	if (xtpChartErrorBarDirectionMinus == m_pErrorBar->GetDirection()
		|| xtpChartErrorBarDirectionBoth == m_pErrorBar->GetDirection())
	{
		pCommand->AddChildCommand(
			m_pErrorBar->GetLineStyle()->CreateDeviceCommand(ptStartPoint, ptFinishPointMinus,
															 m_pErrorBar->GetLineColor()));

		if (xtpChartErrorBarEndStyleCap == m_pErrorBar->GetEndStyle())
		{
			pCommand->AddChildCommand(m_pErrorBar->GetLineStyle()->CreateDeviceCommand(
				ptFinishPointMinusCapStart, ptFinishPointMinusCapEnd, m_pErrorBar->GetLineColor()));
		}
	}

	return pCommand;
}

double CXTPChartErrorBarView::GetAnchorAngle(XTPChartErrorBarPosition position) const
{
	CXTPChartDiagram2DSeriesView* pView = static_cast<CXTPChartDiagram2DSeriesView*>(
		m_pPointView->GetSeriesView());

	CXTPChartDiagram2D* pDiagram = static_cast<CXTPChartDiagram2D*>(pView->GetDiagram());

	if (xtpChartErrorBarPositionTop == position)
	{
		double dValue = m_pPointView->GetInternalValue();

		if (pDiagram->IsRotated())
		{
			BOOL bTop = dValue >= 0;
			if (pView->GetAxisViewY()->GetAxis()->IsReversed())
				bTop = !bTop;

			if (!bTop)
				return 180;
			return 0;
		}
		else
		{
			BOOL bTop = dValue >= 0;
			if (pView->GetAxisViewY()->GetAxis()->IsReversed())
				bTop = !bTop;

			if (!bTop)
				return 270;
			return 90;
		}
	}

	return 0;
}

void CXTPChartErrorBarView::GetValues(double& dCenter, double& dPlus, double& dMinus) const
{
	CXTPChartElementView* pPointsView = m_pPointView->GetParentView();

	CXTPChartDiagram2DSeriesView* pSeriesView = static_cast<CXTPChartDiagram2DSeriesView*>(
		pPointsView->GetParentView());

	CXTPChartSeriesPointCollection* pPoints = pSeriesView->m_pSeries->GetPoints();

	const double dInternalValue = m_pPointView->GetInternalValue();

	switch (m_pErrorBar->m_type)
	{
		case xtpChartErrorBarTypeFixedValue:
		{
			dCenter = dInternalValue;
			dPlus   = dInternalValue + m_pErrorBar->m_dAmount;
			dMinus  = dInternalValue - m_pErrorBar->m_dAmount;
		}
		break;

		case xtpChartErrorBarTypePercentage:
		{
			dCenter = dInternalValue;
			dPlus   = dInternalValue + (dInternalValue / 100.0 * m_pErrorBar->m_dAmount);
			dMinus  = dInternalValue - (dInternalValue / 100.0 * m_pErrorBar->m_dAmount);
		}
		break;

		case xtpChartErrorBarTypeStandardDeviation:
		{
			const double dArithmeticMean	= pPoints->GetArithmeticMean(0);
			const double dStandardDeviation = pPoints->GetStandardDeviation(0);
			dCenter							= dArithmeticMean;
			dPlus							= dArithmeticMean + dStandardDeviation;
			dMinus							= dArithmeticMean - dStandardDeviation;
		}
		break;

		case xtpChartErrorBarTypeStandardError:
		{
			const double dStandardError = pPoints->GetStandardDeviation(0);
			dCenter						= dInternalValue;
			dPlus						= dInternalValue + dStandardError;
			dMinus						= dInternalValue - dStandardError;
		}
		break;

		case xtpChartErrorBarTypeCustom:
		{
			dCenter = dInternalValue;
			dPlus   = dInternalValue + m_pErrorBar->m_dValuePlus;
			dMinus  = dInternalValue - m_pErrorBar->m_dValueMinus;
		}
		break;
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartErrorBar, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "Visible", 0, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "EndStyle", 1, GetEndStyle, SetEndStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "Type", 2, GetType, SetType, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "Direction", 3, GetDirection, SetDirection, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "Amount", 4, GetAmount, SetAmount, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "ValuePlus", 5, GetValuePlus, SetValuePlus, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "ValueMinus", 6, GetValueMinus, SetValueMinus, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "LineStyle", 7, OleGetLineStyle, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartErrorBar, "LineColor", 8, OleGetLineColor, OleSetLineColor,
						VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartErrorBar, CXTPChartElement)
	INTERFACE_PART(CXTPChartErrorBar, XTPDIID_ChartSeriesErrorBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartErrorBar, XTPDIID_ChartSeriesErrorBar)

LPDISPATCH CXTPChartErrorBar::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}

OLE_COLOR CXTPChartErrorBar::OleGetLineColor()
{
	return m_clrLine.ToOleColor();
}

void CXTPChartErrorBar::OleSetLineColor(OLE_COLOR clr)
{
	m_clrLine = (CXTPChartColor::FromOleColor(clr));
}

#endif
