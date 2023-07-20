// XTPChartPyramidSeriesStyleBase.cpp
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

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesStyleBase

IMPLEMENT_DYNAMIC(CXTPChartPyramidSeriesStyleBase, CXTPChartSeriesStyle);

CXTPChartPyramidSeriesStyleBase::CXTPChartPyramidSeriesStyleBase()
{
	m_nPointDistance	  = 4;
	m_dHeightToWidthRatio = 1;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle315);

	m_bColorEach = TRUE;
}

CXTPChartPyramidSeriesStyleBase::~CXTPChartPyramidSeriesStyleBase()
{
	SAFE_RELEASE(m_pFillStyle);
	SAFE_RELEASE(m_pBorder);
}

void CXTPChartPyramidSeriesStyleBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("PointDistance"), m_nPointDistance, 0);

	PX_Double(pPX, _T("HeightToWidthRatio"), m_dHeightToWidthRatio, 1);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPyramidSeriesStyleBase, CXTPChartSeriesStyle)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPyramidSeriesStyleBase, "PointDistance", 100, m_nPointDistance,
							OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPyramidSeriesStyleBase, "HeightToWidthRatio", 101,
							m_dHeightToWidthRatio, OleChartChanged, VT_R8)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPyramidSeriesStyleBase, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartPyramidSeriesStyleBase, XTPDIID__DChartPyramidSeriesStyle, Dispatch)
END_INTERFACE_MAP()

#endif
