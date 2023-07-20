// XTPChartFunnelSeriesLabelBase.cpp
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPMathUtils.h"
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Appearance/XTPChartBorder.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Common/Math/XTPMathUtils.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesLabelBase.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelSeriesLabelBase

IMPLEMENT_DYNAMIC(CXTPChartFunnelSeriesLabelBase, CXTPChartSeriesLabel)

CXTPChartFunnelSeriesLabelBase::CXTPChartFunnelSeriesLabelBase()
{
	m_nPosition   = xtpChartFunnelLabelRight;
	m_nLineLength = 20;
}

CXTPChartFunnelSeriesLabelBase::~CXTPChartFunnelSeriesLabelBase()
{
}

CXTPChartFunnelSeriesStyleBase* CXTPChartFunnelSeriesLabelBase::GetStyle() const
{
	return (CXTPChartFunnelSeriesStyleBase*)m_pOwner;
}

BOOL CXTPChartFunnelSeriesLabelBase::IsInside() const
{
	XTPChartFunnelLabelPosition position = GetPosition();
	return position == xtpChartFunnelLabelCenter;
}

void CXTPChartFunnelSeriesLabelBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartFunnelLabelRight);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelSeriesLabelViewBase

IMPLEMENT_DYNAMIC(CXTPChartFunnelSeriesLabelViewBase, CXTPChartSeriesLabelView);

CXTPChartFunnelSeriesLabelViewBase::CXTPChartFunnelSeriesLabelViewBase(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{
}

XTPChartFunnelLabelPosition CXTPChartFunnelSeriesLabelViewBase::GetPosition() const
{
	return ((CXTPChartFunnelSeriesLabelBase*)m_pLabel)->GetPosition();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartFunnelSeriesLabelBase, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesLabelBase, "Position", 106, m_nPosition,
							OleChartChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartFunnelSeriesLabelBase, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartFunnelSeriesLabelBase, XTPDIID_ChartFunnelSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFunnelSeriesLabelBase, XTPDIID_ChartFunnelSeriesLabel)

#endif
