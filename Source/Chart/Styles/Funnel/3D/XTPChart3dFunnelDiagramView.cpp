// XTPChart3dFunnelDiagramView.cpp
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPFramework.h"
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
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesLabel.h"

#include "Chart/Styles/Funnel/Base/XTPChartFunnelDiagramBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesLabelBase.h"

#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelDiagram.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelDiagramView.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesLabel.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dFunnelDiagramView

IMPLEMENT_DYNAMIC(CXTPChart3dFunnelDiagramView, CXTPChartDiagramView);

CXTPChart3dFunnelDiagramView::CXTPChart3dFunnelDiagramView(CXTPChartDiagram* pDiagram,
														   CXTPChartElementView* pParent)
	: CXTPChart3dDiagramView(pDiagram, pParent)
{
	Set3D();
	m_pSeriesView = new CXTPChartElementView(this);
	m_pLabelsView = new CXTPChart3dFunnelSeriesLabelsView(this);
}
