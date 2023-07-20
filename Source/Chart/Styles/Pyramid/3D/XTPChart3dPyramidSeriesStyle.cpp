// XTPChart3dPyramidSeriesStyle.cpp
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
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidDiagram.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesStyle.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesView.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesLabel.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesStyle

IMPLEMENT_SERIAL(CXTPChart3dPyramidSeriesStyle, CXTPChartPyramidSeriesStyleBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChart3dPyramidSeriesStyle::CXTPChart3dPyramidSeriesStyle()
	: m_nBaseEdgeCount(3)
{
	Set3D();
	SetLabel(new CXTPChart3dPyramidSeriesLabel());
}

CXTPChartDiagram* CXTPChart3dPyramidSeriesStyle::CreateDiagram()
{
	return new CXTPChart3dPyramidDiagram();
}

CXTPChartSeriesView* CXTPChart3dPyramidSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChart3dPyramidSeriesView(pSeries, pDiagramView);
}

BOOL CXTPChart3dPyramidSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (DYNAMIC_DOWNCAST(CXTPChart3dPyramidDiagram, pDiagram) == NULL)
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}

void CXTPChart3dPyramidSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT_VALID(pPX);

	CXTPChartPyramidSeriesStyleBase::DoPropExchange(pPX);

	PX_Int(pPX, _T("BaseEdgeCount3D"), m_nBaseEdgeCount, 0);
}

void CXTPChart3dPyramidSeriesStyle::SetBaseEdgeCount(int nEdgeCount)
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

BEGIN_DISPATCH_MAP(CXTPChart3dPyramidSeriesStyle, CXTPChartPyramidSeriesStyleBase)
	DISP_PROPERTY_NOTIFY_ID(CXTPChart3dPyramidSeriesStyle, "BaseEdgeCount", 110, m_nBaseEdgeCount,
							OleChartChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChart3dPyramidSeriesStyle, CXTPChartPyramidSeriesStyleBase)
	INTERFACE_PART(CXTPChart3dPyramidSeriesStyle, XTPDIID__DChartPyramidSeriesStyle, Dispatch)
	INTERFACE_PART(CXTPChart3dPyramidSeriesStyle, XTPDIID__DChart3dPyramidSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChart3dPyramidSeriesStyle, XTPDIID__DChartPyramidSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChart3dPyramidSeriesStyle,
							  "Codejock.Chart3dPyramidSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_Chart3dPyramidSeriesStyle);

#endif
