// XTPChartBarSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/XTPChartSeriesPoint.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisRange.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Styles/Bar/XTPChartBarSeriesView.h"
#include "Chart/Styles/Bar/XTPChartBarSeriesStyle.h"
#include "Chart/Styles/Bar/XTPChartStackedBarSeriesStyle.h"
#include "Chart/Styles/Bar/XTPChartBarSeriesLabel.h"

#include "Chart/XTPChartErrorBar.h"
#include "Chart/Styles/Bar/XTPChartBarSeriesErrorBar.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesStyle

IMPLEMENT_SERIAL(CXTPChartBarSeriesStyle, CXTPChartDiagram2DSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartBarSeriesStyle::CXTPChartBarSeriesStyle()
{
	m_dBarWidth   = 0.6;
	m_bSideBySide = TRUE;

	m_nBarDistanceFixed = 1;

	m_pBorder	= new CXTPChartBorder(this);
	m_pFillStyle = new CXTPChartFillStyle(this);

	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle270);

	SetLabel(new CXTPChartBarSeriesLabel());
	SetErrorBar(new CXTPChartBarSeriesErrorBar());
}

CXTPChartBarSeriesStyle::~CXTPChartBarSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);
	SAFE_RELEASE(m_pFillStyle);
}

void CXTPChartBarSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesStyle::DoPropExchange(pPX);

	PX_Double(pPX, _T("BarWidth"), m_dBarWidth, 0);
	PX_Int(pPX, _T("BarDistanceFixed"), m_nBarDistanceFixed, 1);

	PX_Bool(pPX, _T("SideBySide"), m_bSideBySide, TRUE);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);
}

CXTPChartSeriesView* CXTPChartBarSeriesStyle::CreateView(CXTPChartSeries* pSeries,
														 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartBarSeriesView(pSeries, pDiagramView);
}

void CXTPChartBarSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis, double /*nMinValue*/,
													 double /*nMaxValue*/, double& nCorrection)
{
	if (!pAxis->IsValuesAxis())
		nCorrection = max(nCorrection, 0.5);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesView

CXTPChartBarSeriesView::CXTPChartBarSeriesView(CXTPChartSeries* pSeries,
											   CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{
	m_nBarCount   = 0;
	m_nBarIndex   = -1;
	m_bFirstStack = TRUE;
}

CXTPChartSeriesPointView* CXTPChartBarSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBarSeriesPointView(pPoint, pParentView);
}

void CXTPChartBarSeriesView::BeforeUpdateRange(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDiagramView* pDiagramView = GetDiagramView();
	int i;

	for (i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartBarSeriesView* pView =
			(CXTPChartBarSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		CXTPChartBarSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartBarSeriesStyle,
														   pView->GetSeries()->GetStyle());
		if (!pStyle)
			continue;

		if (pView != this)
			return;

		break;
	}

	int nBarCount = 0;

	CMap<int, int, int, int> mapGroupToIndex;
	for (i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartBarSeriesView* pView =
			(CXTPChartBarSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		CXTPChartBarSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartBarSeriesStyle,
														   pView->GetSeries()->GetStyle());
		if (!pStyle)
			continue;

		CXTPChartStackedBarSeriesStyle* pStackedStyle =
			DYNAMIC_DOWNCAST(CXTPChartStackedBarSeriesStyle, pStyle);

		if (pStackedStyle)
		{
			int nIndex = -1;

			if (mapGroupToIndex.Lookup(pStackedStyle->GetStackGroup(), nIndex))
			{
				pView->m_nBarIndex   = nIndex;
				pView->m_bFirstStack = FALSE;
			}
			else
			{
				mapGroupToIndex[pStackedStyle->GetStackGroup()] = nBarCount;

				pView->m_nBarIndex   = nBarCount;
				pView->m_bFirstStack = TRUE;

				nBarCount++;
			}
		}
		else
		{
			pView->m_nBarIndex   = nBarCount;
			pView->m_bFirstStack = TRUE;

			if (pStyle->IsSideBySide())
				nBarCount++;
		}
	}

	for (i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartBarSeriesView* pView =
			(CXTPChartBarSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		CXTPChartBarSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartBarSeriesStyle,
														   pView->GetSeries()->GetStyle());
		if (!pStyle)
			continue;

		if (pStyle->IsSideBySide())
		{
			pView->m_nBarCount = nBarCount;
		}
		else
		{
			pView->m_nBarIndex = 0;
			pView->m_nBarCount = 1;
		}
	}
}

CXTPChartDeviceCommand*
	CXTPChartBarSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartBarSeriesStyle* pStyle = (CXTPChartBarSeriesStyle*)GetSeries()->GetStyle();

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(rcBounds,
																		  m_pSeries->GetColor(),
																		  m_pSeries->GetColor2()));

	CXTPChartDeviceCommand* pInnerBorderDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dInnerBorderDeviceCommand(
			rcBounds, m_pSeries->GetColor().GetDarkColor(), 1);
	pCommand->AddChildCommand(pInnerBorderDeviceCommand);

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesPointView

CXTPChartBarSeriesPointView::CXTPChartBarSeriesPointView(CXTPChartSeriesPoint* pPoint,
														 CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{
}

CXTPChartRectF CXTPChartBarSeriesPointView::GetScreenRect() const
{
	CXTPChartBarSeriesView* pSeriesView   = (CXTPChartBarSeriesView*)GetSeriesView();
	CXTPChartBarSeriesStyle* pSeriesStyle = (CXTPChartBarSeriesStyle*)GetSeriesView()->GetStyle();

	CXTPChartDiagram2DAxisView* pAxisXView = pSeriesView->GetAxisViewX();
	CXTPChartDiagram2DAxisView* pAxisYView = pSeriesView->GetAxisViewY();

	if (pSeriesView->m_nBarIndex != -1)
	{
		int nCount = pSeriesView->m_nBarCount;
		int nIndex = pSeriesView->m_nBarIndex;

		double dOffset = (1 + pSeriesStyle->GetBarDistanceFixed()) / pAxisXView->GetScale();

		double dBarWidth = (pSeriesStyle->GetBarWidth() - dOffset * (nCount - 1)) / nCount;

		CXTPChartPointF ptTopLeft(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue()
																  - pSeriesStyle->GetBarWidth() / 2
																  + dBarWidth * nIndex
																  + dOffset * nIndex,
															  GetInternalValue()));

		double dZero = max(0, pAxisYView->GetAxis()->GetRange()->GetMinValue());

		CXTPChartPointF ptBottomRight(
			pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue()
											- pSeriesStyle->GetBarWidth() / 2 + dBarWidth * nIndex
											+ dOffset * nIndex + dBarWidth,
										dZero));

		CXTPChartRectF rc(ptTopLeft, ptBottomRight);
		rc.Width++;
		rc.Height++;

		return rc;
	}

	return CXTPChartRectF(0, 0, 0, 0);
}

CXTPChartDeviceCommand* CXTPChartBarSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDiagramView* pDiagramView = GetSeriesView()->GetDiagramView();
	CXTPChartDiagram2D* pDiagram	   = (CXTPChartDiagram2D*)pDiagramView->GetDiagram();
	CXTPChartBarSeriesStyle* pStyle =
		(CXTPChartBarSeriesStyle*)GetSeriesView()->GetSeries()->GetStyle();

	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRectF rc(GetScreenRect());

	CXTPChartDeviceCommand* pContainer =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand(FALSE);
	CXTPChartDeviceCommand* pCommand = pContainer->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pPoint, rc));

	if (pDiagram->IsRotated())
	{
		CXTPChartFillStyle* pFillStyle = pStyle->GetFillStyle()->CreateRotatedStyle();

		pCommand->AddChildCommand(pFillStyle->CreateDeviceCommand(rc, GetColor(), GetColor2()));

		pFillStyle->InternalRelease();
	}
	else
	{
		pCommand->AddChildCommand(
			pStyle->GetFillStyle()->CreateDeviceCommand(rc, GetColor(), GetColor2()));
	}

	CXTPChartColor clrBorder = pStyle->GetBorder()->GetColor();
	if (clrBorder.IsEmpty())
		clrBorder = GetColor().GetDarkColor();

	pCommand->AddChildCommand(pStyle->GetBorder()->CreateInnerBorderDeviceCommand(rc, clrBorder));

	return pContainer;
}

CXTPChartDeviceCommand* CXTPChartBarSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartBarSeriesStyle* pStyle =
		(CXTPChartBarSeriesStyle*)GetSeriesView()->GetSeries()->GetStyle();

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	pCommand->AddChildCommand(
		pStyle->GetFillStyle()->CreateDeviceCommand(rcBounds, GetColor(), GetColor2()));

	CXTPChartDeviceCommand* pInnerBorderDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dInnerBorderDeviceCommand(rcBounds,
																		GetColor().GetDarkColor(),
																		1);
	pCommand->AddChildCommand(pInnerBorderDeviceCommand);

	return pCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartBarSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "BarWidth", 4, GetBarWidth, SetBarWidth, VT_R8)

	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "FillStyle", 5, OleGetFillStyle, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "Border", 6, OleGetBorder, SetNotSupported,
						VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "SideBySide", 7, IsSideBySide, SetSideBySide,
						VT_BOOL)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartBarSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartBarSeriesStyle, XTPDIID__DChartBarSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBarSeriesStyle, XTPDIID__DChartBarSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartBarSeriesStyle,
							  "Codejock.ChartBarSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartBarSeriesStyle);

LPDISPATCH CXTPChartBarSeriesStyle::OleGetFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

LPDISPATCH CXTPChartBarSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

#endif
