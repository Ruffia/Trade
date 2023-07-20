// XTPChart2dPyramidSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidDiagramBase.h"
#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidDiagram.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidSeriesStyle.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"
#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidSeriesView.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"
#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidSeriesLabel.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesStyle

IMPLEMENT_SERIAL(CXTPChartPyramidSeriesStyle, CXTPChartPyramidSeriesStyleBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartPyramidSeriesStyle::CXTPChartPyramidSeriesStyle()
{
	SetLabel(new CXTPChartPyramidSeriesLabel());
}

CXTPChartDiagram* CXTPChartPyramidSeriesStyle::CreateDiagram()
{
	return new CXTPChartPyramidDiagram();
}

CXTPChartSeriesView* CXTPChartPyramidSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartPyramidSeriesView(pSeries, pDiagramView);
}

BOOL CXTPChartPyramidSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (DYNAMIC_DOWNCAST(CXTPChartPyramidDiagram, pDiagram) == NULL)
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPyramidSeriesStyle, CXTPChartPyramidSeriesStyleBase)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPyramidSeriesStyle, CXTPChartPyramidSeriesStyleBase)
	INTERFACE_PART(CXTPChartPyramidSeriesStyle, XTPDIID__DChartPyramidSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPyramidSeriesStyle, XTPDIID__DChartPyramidSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartPyramidSeriesStyle,
							  "Codejock.ChartPyramidSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartPyramidSeriesStyle);

#endif
