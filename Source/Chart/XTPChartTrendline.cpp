// XTPChartTrendline.cpp
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

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/XTPSystemHelpers.h"

#include "Chart/XTPChartDefines.h"
#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartTrendline.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Styles/Line/XTPChartFastLineSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesTrendlinePointCollection

IMPLEMENT_DYNAMIC(CXTPChartSeriesTrendlinePointCollection, CXTPChartSeriesVirtualPointCollection);

CXTPChartSeriesTrendlinePointCollection::CXTPChartSeriesTrendlinePointCollection()
	: CXTPChartSeriesVirtualPointCollection(0, 0, 0, NULL)
	, m_pTrendline(NULL)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeriesTrendlinePointCollection::~CXTPChartSeriesTrendlinePointCollection()
{
}

CXTPChartSeriesPoint* CXTPChartSeriesTrendlinePointCollection::GetAt(int nIndex) const
{
	CXTPChartSeriesPoint* pVirtualPoint = NULL;

	const CXTPChartSeries* pSource				  = m_pTrendline->GetSource();
	const CXTPChartSeriesPointCollection* pPoints = pSource->GetPoints();
	const CXTPChartSeriesPoint* pPoint			  = pPoints->GetAt(nIndex);

	if (xtpChartScaleQualitative == pSource->GetArgumentScaleType())
	{
		m_pVirtualPoint->m_dArgument		 = nIndex;
		m_pVirtualPoint->m_dInternalArgument = nIndex;
	}
	else
	{
		m_pVirtualPoint->m_dArgument		 = pPoint->m_dArgument;
		m_pVirtualPoint->m_dInternalArgument = pPoint->m_dInternalArgument;
	}
	m_pVirtualPoint->m_nIndex  = nIndex;
	m_pVirtualPoint->m_nLength = 1;

	switch (m_pTrendline->GetType())
	{
		case xtpChartSeriesTrendlineTypeLinear:
		{
			double dSlope, dIntercept;
			if (xtpChartScaleQualitative == pSource->GetArgumentScaleType())
			{
				pPoints->GetSlopeQualitative(0, dSlope, dIntercept);
				m_pVirtualPoint->m_dValues[0] = dSlope * (nIndex + 1) + dIntercept;
			}
			else
			{
				pPoints->GetSlopeNumerical(0, dSlope, dIntercept);
				m_pVirtualPoint->m_dValues[0] = dSlope * (pPoint->m_dArgument) + dIntercept;
			}
			pVirtualPoint = m_pVirtualPoint;
		}
		break;

		case xtpChartSeriesTrendlineTypeMovingAverageSimple:
		{
			int nPeriod = m_pTrendline->GetPeriod();

			if (nIndex + 1 >= nPeriod)
			{
				double dMovingAverage = pPoints->GetMovingAverage(0, nIndex - nPeriod + 1, nIndex);

				m_pVirtualPoint->m_dValues[0] = dMovingAverage;
				pVirtualPoint				  = m_pVirtualPoint;
			}
		}
		break;

		case xtpChartSeriesTrendlineTypeMinimum:
		{
			m_pVirtualPoint->m_dValues[0] = pPoints->GetMinimum(0);
			pVirtualPoint				  = m_pVirtualPoint;
		}
		break;

		case xtpChartSeriesTrendlineTypeMaximum:
		{
			m_pVirtualPoint->m_dValues[0] = pPoints->GetMaximum(0);
			pVirtualPoint				  = m_pVirtualPoint;
		}
		break;
		case xtpChartSeriesTrendlineTypeNone: break;

		default: { ASSERT(FALSE);
		}
		break;
	}

	return pVirtualPoint;
}

int CXTPChartSeriesTrendlinePointCollection::GetCount() const
{
	int nCount = 0;

	if ((NULL != m_pTrendline) && (NULL != m_pTrendline->GetSource())
		&& (NULL != m_pTrendline->GetSource()->GetPoints()))
	{
		nCount = m_pTrendline->GetSource()->GetPoints()->GetCount();
	}

	return nCount;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesTrendline

IMPLEMENT_DYNAMIC(CXTPChartSeriesTrendline, CXTPChartSeries)

CXTPChartSeriesTrendline::CXTPChartSeriesTrendline()
	: CXTPChartSeries()
	, m_type(xtpChartSeriesTrendlineTypeNone)
	, m_nPeriod(2)
	, m_pSource(NULL)
{
	CXTPChartSeriesTrendlinePointCollection* pPointCollection =
		new CXTPChartSeriesTrendlinePointCollection();
	pPointCollection->m_pTrendline = this;
	SetPoints(pPointCollection);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeriesTrendline::~CXTPChartSeriesTrendline()
{
	CMDTARGET_RELEASE(m_pSource);
}

void CXTPChartSeriesTrendline::Release()
{
	SetSource(NULL);
	CXTPChartSeries::Release();
}

void CXTPChartSeriesTrendline::SetSource(CXTPChartSeries* pSource)
{
	if (NULL != m_pSource)
	{
		CMDTARGET_RELEASE(m_pSource);
	}

	if (NULL != pSource)
	{
		CMDTARGET_ADDREF(pSource);
		m_pSource = pSource;
		SetArgumentScaleType(pSource->GetArgumentScaleType());
	}

	OnChartChanged();
}

void CXTPChartSeriesTrendline::SetType(XTPChartSeriesTrendlineType type)
{
	m_type = type;

	switch (m_type)
	{
		case xtpChartSeriesTrendlineTypeNone: SetStyle(NULL); break;
		case xtpChartSeriesTrendlineTypeLinear:
		case xtpChartSeriesTrendlineTypeMovingAverageSimple:
		case xtpChartSeriesTrendlineTypeMinimum:
		case xtpChartSeriesTrendlineTypeMaximum:
			SetStyle(new CXTPChartFastLineSeriesStyle());
			break;

		default:
			ASSERT(FALSE); // Type not handled
			break;
	}

	OnChartChanged();
}

#ifdef _XTP_ACTIVEX

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesTrendline, XTPDIID__DChartSeriesTrendline)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartSeriesTrendline,
							  "Codejock.ChartSeriesTrendline." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartSeriesTrendline);

BEGIN_INTERFACE_MAP(CXTPChartSeriesTrendline, CXTPChartSeries)
	INTERFACE_PART(CXTPChartSeriesTrendline, XTPDIID__DChartSeriesTrendline, Dispatch)
END_INTERFACE_MAP()

BEGIN_DISPATCH_MAP(CXTPChartSeriesTrendline, CXTPChartSeries)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesTrendline, "Series", 0, OleGetSeries, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesTrendline, "Source", 100, OleGetSource, OleSetSource,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesTrendline, "Type", 101, GetType, SetType, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesTrendline, "Period", 102, GetPeriod, SetPeriod, VT_I4)
END_DISPATCH_MAP()

LPDISPATCH CXTPChartSeriesTrendline::OleGetSeries()
{
	return XTPGetDispatch(this);
}

LPDISPATCH CXTPChartSeriesTrendline::OleGetSource()
{
	return XTPGetDispatch(GetSource());
}

void CXTPChartSeriesTrendline::OleSetSource(LPDISPATCH lpSource)
{
	if (NULL != lpSource)
	{
		CXTPChartSeries* pSeries = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPChartSeries,
																CXTPChartSeries::FromIDispatchSafe(
																	lpSource));
		if (NULL == pSeries)
		{
			AfxThrowOleException(E_INVALIDARG);
		}

		// AddRef will be done in SetSource
		SetSource(pSeries);
	}
}

#endif
