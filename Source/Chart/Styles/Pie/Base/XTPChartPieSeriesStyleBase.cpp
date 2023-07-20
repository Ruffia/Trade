// XTPChartPieSeriesStyle.cpp
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
#include "Common/XTPXMLHelpers.h"
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
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieDiagram.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieDiagram.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartAppearance.h"

#include "Chart/Styles/Pie/XTPChartPieSeriesStyle.h"
#include "Chart/Styles/Pie/XTPChartPieDiagram.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesLabel.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesPointView.h"
#include "Chart/Styles/Pie/XTPChartPieSeriesView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesStyleBase

IMPLEMENT_DYNAMIC(CXTPChartPieSeriesStyleBase, CXTPChartSeriesStyle)

CXTPChartPieSeriesStyleBase::CXTPChartPieSeriesStyleBase()
{
	m_nHolePercent			   = 0;
	m_nExplodedDistancePercent = 10;
	m_nRotation				   = 0;
	m_bColorEach			   = TRUE;

	m_pBorder = new CXTPChartBorder(this);
}

CXTPChartPieSeriesStyleBase::~CXTPChartPieSeriesStyleBase()
{
	SAFE_RELEASE(m_pBorder);
}

int CXTPChartPieSeriesStyleBase::GetDepth() const
{
	return 0;
}

void CXTPChartPieSeriesStyleBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("HolePercent"), m_nHolePercent, 0);
	PX_Int(pPX, _T("ExplodedDistancePercent"), m_nExplodedDistancePercent, 10);
	PX_Int(pPX, _T("Rotation"), m_nRotation, 0);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);
}

BOOL CXTPChartPieSeriesStyleBase::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	BOOL bResult = FALSE;
	if (NULL != DYNAMIC_DOWNCAST(CXTPChartPieDiagram, pDiagram)
		|| NULL != DYNAMIC_DOWNCAST(CXTPChart3dPieDiagram, pDiagram))
	{
		bResult = (pDiagram->GetSeries().GetSize() == 0);
	}

	return bResult;
}

CXTPChartColor CXTPChartPieSeriesStyleBase::GetBorderActualColor() const
{
	if (!m_pBorder->GetColor().IsEmpty())
		return m_pBorder->GetColor();

	return GetAppearance()->GetPieStyleAppearance()->BorderColor;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPieSeriesStyleBase, CXTPChartSeriesStyle)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesStyleBase, "HolePercent", 100, m_nHolePercent,
							OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesStyleBase, "ExplodedDistancePercent", 101,
							m_nExplodedDistancePercent, OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesStyleBase, "Rotation", 102, m_nRotation,
							OleChartChanged, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartPieSeriesStyleBase, "Border", 103, OleGetBorder, SetNotSupported,
						VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPieSeriesStyleBase, CXTPChartSeriesStyle)
END_INTERFACE_MAP()

LPDISPATCH CXTPChartPieSeriesStyleBase::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

#endif
