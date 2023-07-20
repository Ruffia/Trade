// XTPChartLegendItem.cpp
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
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPChartLegendItem::CXTPChartLegendItem()
	: m_nLineThickness(UseDefaultLineThickness)
	, m_nDashStyle(xtpChartDashStyleSolid)
{
}

void CXTPChartLegendItem::SetLineThickness(int nLineThickness)
{
	m_nLineThickness = nLineThickness;
}

int CXTPChartLegendItem::GetActualLineThickness() const
{
	int nActualLineThickness = 1;

	if (UseDefaultLineThickness != m_nLineThickness)
	{
		nActualLineThickness = m_nLineThickness;
	}

	return nActualLineThickness;
}

void CXTPChartLegendItem::SetDashStyle(XTPChartDashStyle nDashStyle)
{
	m_nDashStyle = nDashStyle;
}

XTPChartDashStyle CXTPChartLegendItem::GetDashStyle() const
{
	return m_nDashStyle;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartLegendItem, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartLegendItem, "LineThickness", 1, GetActualLineThickness,
						SetLineThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartLegendItem, "DashStyle", 2, GetDashStyle, SetDashStyle, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartLegendItem, CXTPChartElement)
	INTERFACE_PART(CXTPChartLegendItem, XTPDIID_ChartLegendItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLegendItem, XTPDIID_ChartLegendItem)

#endif
