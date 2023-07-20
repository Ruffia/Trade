// XTPChartPieSeriesLabel.cpp
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
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
#include "Common/PropExchange/XTPPropExchangeRegistry.h"
#include "Common/PropExchange/XTPPropExchangeIniFile.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/Types/XTPChartPie.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartSeriesPointView.h"

#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Appearance/XTPChartBorder.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Styles/Pie/XTPChartPieDiagram.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesLabel.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesStyle.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesPointView.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesLabelBase

IMPLEMENT_DYNAMIC(CXTPChartPieSeriesLabelBase, CXTPChartSeriesLabel);

CXTPChartPieSeriesLabelBase::CXTPChartPieSeriesLabelBase()
{
	m_nPosition = xtpChartPieLabelOutside;
}

CXTPChartPieSeriesLabelBase::~CXTPChartPieSeriesLabelBase()
{
}

CXTPChartPieSeriesStyleBase* CXTPChartPieSeriesLabelBase::GetStyle() const
{
	return (CXTPChartPieSeriesStyleBase*)m_pOwner;
}

void CXTPChartPieSeriesLabelBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartPieLabelOutside);
}

BOOL CXTPChartPieSeriesLabelBase::IsInside() const
{
	XTPChartPieLabelPosition position = GetPosition();
	return position == xtpChartPieLabelInside || position == xtpChartPieLabelRadial;
}

//////////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesLabelViewBase

IMPLEMENT_DYNAMIC(CXTPChartPieSeriesLabelViewBase, CXTPChartSeriesLabelView);

CXTPChartPieSeriesLabelViewBase::CXTPChartPieSeriesLabelViewBase(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{
}

XTPChartPieLabelPosition CXTPChartPieSeriesLabelViewBase::GetPosition() const
{
	return static_cast<CXTPChartPieSeriesLabelBase*>(m_pLabel)->GetPosition();
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesLabelsViewBase

IMPLEMENT_DYNAMIC(CXTPChartPieSeriesLabelsViewBase, CXTPChartElementView);

CXTPChartPieSeriesLabelsViewBase::CXTPChartPieSeriesLabelsViewBase(CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPieSeriesLabelBase, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesLabelBase, "Position", 106, m_nPosition,
							OleChartChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPieSeriesLabelBase, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartPieSeriesLabelBase, XTPDIID_ChartPieSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieSeriesLabelBase, XTPDIID_ChartPieSeriesLabel)

#endif
