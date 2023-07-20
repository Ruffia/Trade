// XTPChartAxisTickMarks.cpp
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

#include "Chart/Diagram/Axis/XTPChartAxisTickMarks.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPChartAxisTickMarks::CXTPChartAxisTickMarks(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

	m_bVisible		  = TRUE;
	m_bMinorVisible   = TRUE;
	m_nThickness	  = 1;
	m_nMinorThickness = 1;
	m_nLength		  = 5;
	m_nMinorLength	= 2;

	m_bCrossAxis = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisTickMarks::~CXTPChartAxisTickMarks()
{
}
void CXTPChartAxisTickMarks::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("MinorVisible"), m_bMinorVisible, TRUE);
	PX_Int(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Int(pPX, _T("MinorThickness"), m_nMinorThickness, 1);
	PX_Int(pPX, _T("Length"), m_nLength, 5);
	PX_Int(pPX, _T("MinorLength"), m_nMinorLength, 2);
	PX_Bool(pPX, _T("CrossAxis"), m_bCrossAxis, FALSE);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisTickMarks, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "MinorVisible", 2, IsMinorVisible, SetMinorVisible,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "Thickness", 3, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "MinorThickness", 4, GetMinorThickness,
						SetMinorThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "Length", 5, GetLength, SetLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "MinorLength", 6, GetMinorLength, SetMinorLength,
						VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "CrossAxis", 7, IsCrossAxis, SetCrossAxis, VT_BOOL)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisTickMarks, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisTickMarks, XTPDIID_ChartAxisTickMarks, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisTickMarks, XTPDIID_ChartAxisTickMarks)

#endif
