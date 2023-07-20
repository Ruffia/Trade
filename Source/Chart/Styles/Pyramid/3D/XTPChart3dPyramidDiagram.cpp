// XTPChart3dPyramidDiagram.cpp
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
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidDiagramBase.h"

#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidDiagram.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidDiagramView.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidDiagram

IMPLEMENT_SERIAL(CXTPChart3dPyramidDiagram, CXTPChart3dDiagram,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChart3dPyramidDiagram::CXTPChart3dPyramidDiagram()
{
	Set3D();
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPChartDiagramView* CXTPChart3dPyramidDiagram::CreateView(CXTPChartDeviceContext* pDC,
															CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChart3dPyramidDiagramView(this, pParent);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChart3dPyramidDiagram, CXTPChart3dDiagram)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChart3dPyramidDiagram, CXTPChart3dDiagram)
	INTERFACE_PART(CXTPChart3dPyramidDiagram, XTPDIID__DChartPyramidDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChart3dPyramidDiagram, XTPDIID__DChartPyramidDiagram)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChart3dPyramidDiagram,
							  "Codejock.Chart3dPyramidDiagram." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartPyramidDiagram);

#endif
