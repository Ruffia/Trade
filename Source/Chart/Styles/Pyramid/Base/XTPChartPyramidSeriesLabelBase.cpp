// XTPChartPyramidSeriesLabelBase.cpp
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
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesLabel.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesLabelBase

IMPLEMENT_DYNAMIC(CXTPChartPyramidSeriesLabelBase, CXTPChartSeriesLabel);

CXTPChartPyramidSeriesLabelBase::CXTPChartPyramidSeriesLabelBase()
{
	m_nPosition = xtpChartPyramidLabelLeft;
}

CXTPChartPyramidSeriesLabelBase::~CXTPChartPyramidSeriesLabelBase()
{
}

CXTPChartPyramidSeriesStyleBase* CXTPChartPyramidSeriesLabelBase::GetStyle() const
{
	return (CXTPChartPyramidSeriesStyleBase*)m_pOwner;
}

void CXTPChartPyramidSeriesLabelBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartPyramidLabelRight);
}

BOOL CXTPChartPyramidSeriesLabelBase::IsInside() const
{
	XTPChartPyramidLabelPosition position = GetPosition();
	return position == xtpChartPyramidLabelCenter;
}

//////////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesLabelViewBase

IMPLEMENT_DYNAMIC(CXTPChartPyramidSeriesLabelViewBase, CXTPChartSeriesLabelView);

CXTPChartPyramidSeriesLabelViewBase::CXTPChartPyramidSeriesLabelViewBase(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{
}

XTPChartPyramidLabelPosition CXTPChartPyramidSeriesLabelViewBase::GetPosition() const
{
	return static_cast<CXTPChartPyramidSeriesLabelBase*>(m_pLabel)->GetPosition();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPyramidSeriesLabelBase, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPyramidSeriesLabelBase, "Position", 106, m_nPosition,
							OleChartChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPyramidSeriesLabelBase, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartPyramidSeriesLabelBase, XTPDIID_ChartPyramidSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPyramidSeriesLabelBase, XTPDIID_ChartPyramidSeriesLabel)

#endif
