// XTPChartDiagram2DSeriesStyle.cpp
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
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPChartDiagram2DSeriesStyle, CXTPChartSeriesStyle)

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DSeriesStyle

CXTPChartDiagram2DSeriesStyle::CXTPChartDiagram2DSeriesStyle()
{
	m_bSecondaryAxisX = FALSE;
	m_bSecondaryAxisY = FALSE;
	m_nAxisIndexX	 = 1;
	m_nAxisIndexY	 = 1;
}

CXTPChartDiagram2DSeriesStyle::~CXTPChartDiagram2DSeriesStyle()
{
}

void CXTPChartDiagram2DSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Bool(pPX, _T("SecondaryAxisX"), m_bSecondaryAxisX, FALSE);
	PX_Bool(pPX, _T("SecondaryAxisY"), m_bSecondaryAxisY, FALSE);
	PX_Int(pPX, _T("AxisIndexX"), m_nAxisIndexX, 1);
	PX_Int(pPX, _T("AxisIndexY"), m_nAxisIndexY, 1);
}

CXTPChartDiagram* CXTPChartDiagram2DSeriesStyle::CreateDiagram()
{
	return new CXTPChartDiagram2D();
}

BOOL CXTPChartDiagram2DSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	return DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pDiagram) != NULL;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartDiagram2DSeriesStyle, CXTPChartSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DSeriesStyle, "SecondaryAxisX", 30, IsSecondaryAxisX,
						SetSecondaryAxisX, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DSeriesStyle, "SecondaryAxisY", 31, IsSecondaryAxisY,
						SetSecondaryAxisY, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DSeriesStyle, "AxisIndexX", 32, GetAxisIndexX,
						SetAxisIndexX, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DSeriesStyle, "AxisIndexY", 33, GetAxisIndexY,
						SetAxisIndexY, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartDiagram2DSeriesStyle, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartDiagram2DSeriesStyle, XTPDIID__DChartDiagram2DSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartDiagram2DSeriesStyle, XTPDIID__DChartDiagram2DSeriesStyle)

#endif
