// XTPChart3dFunnelSeriesStyle.cpp
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
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Styles/Funnel/Base/XTPChartFunnelDiagramBase.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelDiagram.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesStyle.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesViewBase.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesView.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesLabelBase.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesLabel.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dFunnelSeriesStyle

IMPLEMENT_SERIAL(CXTPChart3dFunnelSeriesStyle, CXTPChartFunnelSeriesStyleBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChart3dFunnelSeriesStyle::CXTPChart3dFunnelSeriesStyle()
	: m_nBaseEdgeCount(3)
{
	Set3D();
	SetLabel(new CXTPChart3dFunnelSeriesLabel());
}

CXTPChartDiagram* CXTPChart3dFunnelSeriesStyle::CreateDiagram()
{
	return new CXTPChart3dFunnelDiagram();
}

CXTPChartSeriesView* CXTPChart3dFunnelSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															  CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChart3dFunnelSeriesView(pSeries, pDiagramView);
}

BOOL CXTPChart3dFunnelSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (DYNAMIC_DOWNCAST(CXTPChart3dFunnelDiagram, pDiagram) == NULL)
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}

void CXTPChart3dFunnelSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT_VALID(pPX);

	CXTPChartFunnelSeriesStyleBase::DoPropExchange(pPX);

	PX_Int(pPX, _T("BaseEdgeCount3D"), m_nBaseEdgeCount, 0);
}

void CXTPChart3dFunnelSeriesStyle::SetBaseEdgeCount(int nEdgeCount)
{
	if (!(0 == nEdgeCount || 3 <= nEdgeCount))
	{
		ASSERT(!"Edge count must be either 0 or equal-greater than 3");
		nEdgeCount = 0;
	}

	m_nBaseEdgeCount = nEdgeCount;
	OnChartChanged();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChart3dFunnelSeriesStyle, CXTPChartFunnelSeriesStyleBase)
	DISP_PROPERTY_NOTIFY_ID(CXTPChart3dFunnelSeriesStyle, "BaseEdgeCount", 110, m_nBaseEdgeCount,
							OleChartChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChart3dFunnelSeriesStyle, CXTPChartFunnelSeriesStyleBase)
	INTERFACE_PART(CXTPChart3dFunnelSeriesStyle, XTPDIID__DChartFunnelSeriesStyle, Dispatch)
	INTERFACE_PART(CXTPChart3dFunnelSeriesStyle, XTPDIID__DChart3dFunnelSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChart3dFunnelSeriesStyle, XTPDIID__DChartFunnelSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChart3dFunnelSeriesStyle,
							  "Codejock.Chart3dFunnelSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_Chart3dFunnelSeriesStyle);

#endif
