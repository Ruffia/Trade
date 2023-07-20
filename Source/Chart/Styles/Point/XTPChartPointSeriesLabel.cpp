// XTPChartPointSeriesLabel.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
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
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Styles/Point/XTPChartPointSeriesLabel.h"
#include "Chart/Styles/Point/XTPChartPointSeriesLabelView.h"
#include "Chart/Styles/Point/XTPChartPointSeriesView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesLabel
IMPLEMENT_SERIAL(CXTPChartPointSeriesLabel, CXTPChartDiagram2DSeriesLabel,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPointSeriesLabel::CXTPChartPointSeriesLabel()
{
	m_nAngle = 45;
}

CXTPChartPointSeriesLabel::~CXTPChartPointSeriesLabel()
{
}

void CXTPChartPointSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesLabel::DoPropExchange(pPX);

	PX_Int(pPX, _T("Angle"), m_nAngle, 45);
}

CXTPChartElementView* CXTPChartPointSeriesLabel::CreateView(CXTPChartDeviceContext* pDC,
															CXTPChartSeriesPointView* pPointView,
															CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesLabelView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesLabelView

CXTPChartPointSeriesLabelView::CXTPChartPointSeriesLabelView(CXTPChartSeriesLabel* pLabel,
															 CXTPChartSeriesPointView* pPointView,
															 CXTPChartElementView* pParentView)
	: CXTPChartDiagram2DSeriesLabelView(pLabel, pPointView, pParentView)
{
}

CXTPChartPointF CXTPChartPointSeriesLabelView::GetAnchorPoint() const
{
	return ((CXTPChartPointSeriesPointView*)m_pPointView)->GetScreenPoint();
}

double CXTPChartPointSeriesLabelView::GetAnchorAngle() const
{
	CXTPChartDiagram2DSeriesView* pView = (CXTPChartDiagram2DSeriesView*)
											  m_pPointView->GetSeriesView();

	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pView->GetDiagram();

	double lineAngle = ((CXTPChartPointSeriesLabel*)m_pLabel)->GetAngle();

	if (pView->GetAxisViewX()->GetAxis()->IsReversed())
		lineAngle = 180 - lineAngle;

	if (pView->GetAxisViewY()->GetAxis()->IsReversed())
		lineAngle = -lineAngle;

	if (pDiagram->IsRotated())
	{
		lineAngle = 90 - lineAngle;
	}
	return lineAngle;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPointSeriesLabel, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPointSeriesLabel, "Angle", 107, m_nAngle, OleChartChanged,
							VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPointSeriesLabel, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartPointSeriesLabel, XTPDIID_ChartPointSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPointSeriesLabel, XTPDIID_ChartPointSeriesLabel)

#endif
