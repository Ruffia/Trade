// XTPChartFunnelSeriesPointViewBase.cpp
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

#include "Common/XTPTypeId.h"
#include "Common/XTPMathUtils.h"
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
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Common/Math/XTPMathUtils.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesPointViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesViewBase.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelDiagramSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChartFunnelSeriesPointViewBase, CXTPChartSeriesPointView);

CXTPChartFunnelSeriesPointViewBase::CXTPChartFunnelSeriesPointViewBase(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
	, m_dValue(0)
	, m_dFrom(0)
	, m_dTo(0)
	, m_dHeight(0)
{
}

CXTPChartFunnelSeriesPointViewBase::CXTPChartFunnelSeriesPointViewBase(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView, BOOL bAddToParent)
	: CXTPChartSeriesPointView(pPoint, pParentView, bAddToParent)
	, m_dValue(0)
	, m_dFrom(0)
	, m_dTo(0)
	, m_dHeight(0)
{
}

CXTPChartFunnelSeriesPointViewBase::~CXTPChartFunnelSeriesPointViewBase()
{
}

CXTPChartColor CXTPChartFunnelSeriesPointViewBase::GetBorderActualColor() const
{
	CXTPChartFunnelSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyleBase,
															 GetSeriesView()->GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = GetColor().GetDarkColor();
	return clr;
}

void CXTPChartFunnelSeriesPointViewBase::CalculateLayout(int nIndex, int nCount, double dFrom,
														 double dTo)
{
	CXTPChartFunnelSeriesViewBase* pView   = (CXTPChartFunnelSeriesViewBase*)GetSeriesView();
	CXTPChartFunnelSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyleBase,
															 GetSeriesView()->GetStyle());

	CXTPChartRectF rcInnerRect = pView->GetInnerBounds();

	if (pStyle->IsEqualHeight())
	{
		m_dHeight = (static_cast<double>(rcInnerRect.Height)
					 - static_cast<double>(nCount - 1) * pStyle->GetPointDistance())
					/ static_cast<double>(nCount);
		m_dWidth = m_dHeight * 0.618;

		m_rc.X = rcInnerRect.X;
		m_rc.Y = (float)CXTPMathUtils::Round(rcInnerRect.Y
											 + (m_dHeight + pStyle->GetPointDistance()) * nIndex);

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

		if (dPosFrom < rcInnerRect.Height - static_cast<float>(m_dHeight))
		{
			m_dFrom = (m_dWidth / rcInnerRect.Width - 1) / (rcInnerRect.Height - m_dHeight)
						  * dPosFrom
					  + 1;
		}
		else
		{
			m_dFrom = m_dWidth / rcInnerRect.Width;
		}

		if (dPosTo < rcInnerRect.Height - static_cast<float>(m_dHeight))
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

CXTPChartDeviceCommand* CXTPChartFunnelSeriesPointViewBase::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1	= GetColor();
	CXTPChartColor color2	= GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	return ((CXTPChartFunnelSeriesViewBase*)GetSeriesView())
		->CreateLegendDeviceCommand(pDC, rcBounds, color1, color2, clrBorder);
}
