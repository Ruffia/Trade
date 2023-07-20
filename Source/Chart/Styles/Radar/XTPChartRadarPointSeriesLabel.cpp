// XTPChartRadarPointSeriesLabel.cpp
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
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesLabel.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesLabel
IMPLEMENT_SERIAL(CXTPChartRadarPointSeriesLabel, CXTPChartSeriesLabel,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRadarPointSeriesLabel::CXTPChartRadarPointSeriesLabel()
{
	m_nAngle = 45;
}

CXTPChartRadarPointSeriesLabel::~CXTPChartRadarPointSeriesLabel()
{
}

void CXTPChartRadarPointSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Int(pPX, _T("Angle"), m_nAngle, 45);
}

CXTPChartElementView* CXTPChartRadarPointSeriesLabel::CreateView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesLabelView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesLabelView

CXTPChartRadarPointSeriesLabelView::CXTPChartRadarPointSeriesLabelView(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{
}

CRect CXTPChartRadarPointSeriesLabelView::GetPaneRect() const
{
	CXTPChartSeriesView* pSeriseView = m_pPointView->GetSeriesView();

	CXTPChartRadarDiagramView* pDiagramView = (CXTPChartRadarDiagramView*)
												  pSeriseView->GetDiagramView();

	return pDiagramView->GetBounds();
}

CXTPChartPointF CXTPChartRadarPointSeriesLabelView::GetAnchorPoint() const
{
	return ((CXTPChartRadarPointSeriesPointView*)m_pPointView)->GetScreenPoint();
}

double CXTPChartRadarPointSeriesLabelView::GetAnchorAngle() const
{
	double lineAngle = ((CXTPChartRadarPointSeriesLabel*)m_pLabel)->GetAngle();

	return lineAngle;
}

void CXTPChartRadarPointSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CXTPChartPointF ptAnchorPoint = GetAnchorPoint();

	CalculateLayout(pDC, ptAnchorPoint, GetAnchorAngle(), text);
}

void CXTPChartRadarPointSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC,
														 const CXTPChartPointF& ptAnchorPoint,
														 double dAnchorAngle,
														 const CXTPChartString& text)
{
	double dLineAngle = CXTPMathUtils::Degree2Radian(dAnchorAngle);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptAnchorPoint.X, (int)ptAnchorPoint.Y)))
		return;

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartPointF ptStartPoint(CXTPMathUtils::Round(ptAnchorPoint.X),
								 CXTPMathUtils::Round(ptAnchorPoint.Y));
	CXTPChartPointF ptFinishPoint(
		CXTPMathUtils::Round(ptAnchorPoint.X + (cos(dLineAngle) * m_pLabel->GetLineLength())),
		CXTPMathUtils::Round(ptAnchorPoint.Y - (sin(dLineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF rcInnerBounds;
	CXTPChartRectF rcBounds =
		CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
			ptFinishPoint, dLineAngle, szText, nBorderThickness, rcInnerBounds);

	CXTPChartSeriesView* pSeriseView		= m_pPointView->GetSeriesView();
	CXTPChartRadarDiagramView* pDiagramView = (CXTPChartRadarDiagramView*)
												  pSeriseView->GetDiagramView();
	pDiagramView->CheckLabelBounds(rcBounds);
}

CXTPChartDeviceCommand*
	CXTPChartRadarPointSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CXTPChartPointF ptAnchorPoint = GetAnchorPoint();

	return CreateDeviceCommand(pDC, ptAnchorPoint, GetAnchorAngle(), text);
}

CXTPChartDeviceCommand* CXTPChartRadarPointSeriesLabelView::CreateDeviceCommand(
	CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle,
	const CXTPChartString& text)
{
	if (text.IsEmpty() || text == _T(" "))
		return NULL;

	double dLineAngle = CXTPMathUtils::Degree2Radian(dAnchorAngle);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptAnchorPoint.X, (int)ptAnchorPoint.Y)))
		return NULL;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pLabel);

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartColor clrTextColor = GetActualTextColor();

	CXTPChartPointF ptStartPoint(CXTPMathUtils::Round(ptAnchorPoint.X),
								 CXTPMathUtils::Round(ptAnchorPoint.Y));
	CXTPChartPointF ptFinishPoint(
		CXTPMathUtils::Round(ptAnchorPoint.X + (cos(dLineAngle) * m_pLabel->GetLineLength())),
		CXTPMathUtils::Round(ptAnchorPoint.Y - (sin(dLineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF rcInnerBounds;
	CXTPChartRectF rcBounds =
		CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
			ptFinishPoint, dLineAngle, szText, nBorderThickness, rcInnerBounds);

	CXTPChartPointF ptLabelPoint = rcInnerBounds.GetLocation();

	CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
	pCommand->AddChildCommand(
		m_pLabel->GetFillStyle()->CreateDeviceCommand(rcBounds, clrBackColor, clrBackColor));

	painter.SetLocation(ptLabelPoint);

	pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

	if (m_pLabel->GetShowLines())
	{
		CXTPChartColor clrConnectorColor = GetActualConnectorColor();

		CXTPChartSeriesLabelLineConnectorPainter linePainter(ptStartPoint, ptFinishPoint, rcBounds);
		pCommand->AddChildCommand(linePainter.CreateDeviceCommand(pDC, clrConnectorColor));
	}

	if (nBorderThickness)
	{
		CXTPChartColor clrBorderColor = GetActualBorderColor();

		pCommand->AddChildCommand(
			m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(rcBounds, clrBorderColor));
	}

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarPointSeriesLabel, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartRadarPointSeriesLabel, "Angle", 107, m_nAngle, OleChartChanged,
							VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarPointSeriesLabel, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartRadarPointSeriesLabel, XTPDIID_ChartRadarPointSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarPointSeriesLabel, XTPDIID_ChartRadarPointSeriesLabel)

#endif
