// XTPChartAxisGridLines.cpp
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

#include "Chart/XTPChartDefines.h"
#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Diagram/Axis/XTPChartAxisGridLines.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartLineStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPChartAxisGridLines::CXTPChartAxisGridLines(CXTPChartAxis* pAxis)
{
	m_pOwner		= pAxis;
	m_bVisible		= TRUE;
	m_bMinorVisible = FALSE;

	m_pLineStyle	  = new CXTPChartLineStyle(this);
	m_pMinorLineStyle = new CXTPChartLineStyle(this);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisGridLines::~CXTPChartAxisGridLines()
{
	SAFE_RELEASE(m_pLineStyle);
	SAFE_RELEASE(m_pMinorLineStyle);
}

CXTPChartColor CXTPChartAxisGridLines::GetColor() const
{
	if (!m_clrColor.IsEmpty())
		return m_clrColor;

	return GetAxis()->GetAppearance()->GridLinesColor;
}

CXTPChartColor CXTPChartAxisGridLines::GetMinorColor() const
{
	if (!m_clrMinorColor.IsEmpty())
		return m_clrMinorColor;

	return GetAxis()->GetAppearance()->GridLinesMinorColor;
}

void CXTPChartAxisGridLines::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("MinorVisible"), m_bMinorVisible, FALSE);

	CXTPPropExchangeSection secLineStyle(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&secLineStyle);

	CXTPPropExchangeSection secMinorLineStyle(pPX->GetSection(_T("MinorLineStyle")));
	m_pMinorLineStyle->DoPropExchange(&secMinorLineStyle);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisGridLines, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "MinorVisible", 2, IsMinorVisible, SetMinorVisible,
						VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "LineStyle", 3, OleGetLineStyle, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "MinorLineStyle", 4, OleGetMinorLineStyle,
						SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisGridLines, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisGridLines, XTPDIID_ChartAxisGridLines, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisGridLines, XTPDIID_ChartAxisGridLines)

LPDISPATCH CXTPChartAxisGridLines::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}
LPDISPATCH CXTPChartAxisGridLines::OleGetMinorLineStyle()
{
	return XTPGetDispatch(m_pMinorLineStyle);
}

#endif
