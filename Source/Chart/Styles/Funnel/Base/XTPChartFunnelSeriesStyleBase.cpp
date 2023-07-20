// XTPChartFunnelSeriesStyleBase.cpp
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

#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelDiagramBase.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelDiagram.h"
//#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelDiagram.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelSeriesStyleBase

IMPLEMENT_DYNAMIC(CXTPChartFunnelSeriesStyleBase, CXTPChartSeriesStyle)

CXTPChartFunnelSeriesStyleBase::CXTPChartFunnelSeriesStyleBase()
{
	m_nPointDistance	  = 4;
	m_dHeightToWidthRatio = 1;
	m_bEqualHeight		  = FALSE;
	m_bColorEach		  = TRUE;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle90);
}

CXTPChartFunnelSeriesStyleBase::~CXTPChartFunnelSeriesStyleBase()
{
	SAFE_RELEASE(m_pBorder);
	SAFE_RELEASE(m_pFillStyle);
}

void CXTPChartFunnelSeriesStyleBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("PointDistance"), m_nPointDistance, 0);

	PX_Double(pPX, _T("HeightToWidthRatio"), m_dHeightToWidthRatio, 1);

	PX_Bool(pPX, _T("EqualHeight"), m_bEqualHeight, FALSE);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

BOOL CXTPChartFunnelSeriesStyleBase::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (NULL == DYNAMIC_DOWNCAST(CXTPChartFunnelDiagram, pDiagram))
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartFunnelSeriesStyleBase, CXTPChartSeriesStyle)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesStyleBase, "PointDistance", 100, m_nPointDistance,
							OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesStyleBase, "HeightToWidthRatio", 101,
							m_dHeightToWidthRatio, OleChartChanged, VT_R8)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesStyleBase, "EqualHeight", 102, m_bEqualHeight,
							OleChartChanged, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartFunnelSeriesStyleBase, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartFunnelSeriesStyleBase, XTPDIID__DChartFunnelSeriesStyle, Dispatch)
END_INTERFACE_MAP()

#endif
