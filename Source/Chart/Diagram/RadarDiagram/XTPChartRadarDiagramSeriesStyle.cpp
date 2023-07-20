// XTPChartRadarDiagramSeriesStyle.cpp
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
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPChartRadarDiagramSeriesStyle, CXTPChartSeriesStyle)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarDiagramSeriesStyle

CXTPChartRadarDiagramSeriesStyle::CXTPChartRadarDiagramSeriesStyle()
{
}

CXTPChartRadarDiagramSeriesStyle::~CXTPChartRadarDiagramSeriesStyle()
{
}

void CXTPChartRadarDiagramSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);
}

CXTPChartDiagram* CXTPChartRadarDiagramSeriesStyle::CreateDiagram()
{
	return new CXTPChartRadarDiagram();
}

BOOL CXTPChartRadarDiagramSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	return DYNAMIC_DOWNCAST(CXTPChartRadarDiagram, pDiagram) != NULL;
}
