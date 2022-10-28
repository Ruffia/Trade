#include "StdAfx.h"
#include "ContainerProperty.h"
#include "ChartContainer.h"
#include "DataLabel.h"

CContainerProperty::CContainerProperty():
    m_colBkground((ARGB)Color::White), 
	m_colBorder((ARGB)Color::Black),
	m_colZoomBorder((ARGB)Color::Red), 
	m_colLegPnts((ARGB)Color::Red),
	m_colDataViewPnts((ARGB)Color::BlueViolet), 
	m_colLegBkgnd(Color(0xFFFFFFE0)),
	m_colLegBorder((ARGB)Color::Black),
	m_bTracking(false), m_bHasNamesLeg(false), m_bUserEnabled(true),
	m_bAxBoundaries(false), m_precision(3),
	m_tmpZX(0.0), m_tmpZY(0.0),
	m_epsX(0.0)
{

}


CContainerProperty::~CContainerProperty()
{

}

CContainerProperty::CContainerProperty(const CContainerProperty& rhs)
{
	m_colBkground   = rhs.m_colBkground;
	m_colBorder     = rhs.m_colBorder;
	m_colZoomBorder = rhs.m_colZoomBorder;
	m_colLegPnts    = rhs.m_colLegPnts;
	m_colDataViewPnts = rhs.m_colDataViewPnts;
	m_colLegBkgnd   = rhs.m_colLegBkgnd;
	m_colLegBorder  = rhs.m_colLegBorder;

	m_bTracking        = rhs.m_bTracking;    // Call ShowLegend after wnd creation to see
	m_bHasNamesLeg     = rhs.m_bHasNamesLeg;
	m_bUserEnabled     = rhs.m_bUserEnabled;
	
	m_epsX             = rhs.m_epsX;         // Neighbourhood to search for nearest pnts
	m_bAxBoundaries    = rhs.m_bAxBoundaries; 
	m_precision       = rhs.m_precision;
}

void CContainerProperty::SetContainer(CChartContainer* pContianer)
{
	m_pContainer = pContianer;
}


void CContainerProperty::SetContainerPrecision(int precision, bool bRedraw)
{
	m_precision = precision;
	if (bRedraw)
	{
		if (m_pContainer->IsLabWndVisible(false)) // This version of Update redraws dataWnd only
			m_pContainer->UpdateDataLegend(false);  //(m_mapSelPntsD, m_pDataWnd->m_mapLabs);
		else if (m_bAxBoundaries)
			m_pContainer->RefreshWnd();
		if (m_pContainer->m_dataViewChartIdx != -1)
			m_pContainer->UpdateDataView(m_pContainer->m_dataViewChartIdx, F_NODATACHANGE);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Setting colors

void CContainerProperty::SetBkColor(Color col, bool bRedraw)
{
	m_colBkground = col;
	if (bRedraw)
		m_pContainer->RefreshWnd();
}

void CContainerProperty::SetBorderColor(Color col, bool bRedraw)
{
	m_colBorder = col;
	if (bRedraw)
		m_pContainer->RefreshWnd();
}


void CContainerProperty::SetLegPntsColor(Color col, bool bRedraw)
{
	m_colLegPnts = col;
	if (bRedraw && m_pContainer->IsLabWndVisible(false))
		m_pContainer->RefreshWnd();
}


void CContainerProperty::SetDataViewPntsColor(Color col, bool bRedraw)
{
	m_colDataViewPnts = col;
	if (bRedraw && (!m_pContainer->m_mapDataViewPntsD.empty()))
		m_pContainer->RefreshWnd();
}


void CContainerProperty::SetLegBorderColor(Color col, bool bRedraw)
{
	m_colLegBorder = col;
	if (m_pContainer->m_pLegWnd != NULL)
	{
		m_pContainer->m_pLegWnd->SetBorderColor(col);
		if (bRedraw && m_pContainer->m_pLegWnd->IsWindowVisible())
			m_pContainer->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
	}

	if (m_pContainer->m_pDataWnd != NULL)
	{
		m_pContainer->m_pDataWnd->SetBorderColor(col);
		if (bRedraw && m_pContainer->m_pDataWnd->IsWindowVisible())
			m_pContainer->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
	}
}


void CContainerProperty::SetLegBkgndColor(Color col, bool bRedraw)
{
	m_colLegBkgnd = col;
	if (m_pContainer->m_pLegWnd != NULL)
	{
		m_pContainer->m_pLegWnd->SetBkColor(col);
		if (bRedraw && m_pContainer->m_pLegWnd->IsWindowVisible())
			m_pContainer->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
	}

	if (m_pContainer->m_pDataWnd != NULL)
	{
		m_pContainer->m_pDataWnd->SetBkColor(col);
		if (bRedraw && m_pContainer->m_pDataWnd->IsWindowVisible())
			m_pContainer->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
	}
}


void CContainerProperty::EnableUser(bool bEnable, bool bClearState)
{
	m_bUserEnabled = bEnable;
	if (!m_bUserEnabled && bClearState && !m_pContainer->m_mapCharts.empty())
	{
		m_pContainer->m_chartSelIdx = m_pContainer->SelectChart(-1);
		m_bTracking = false;
		m_bHasNamesLeg = false;
		m_pContainer->m_dataLegPntD = PointD(DBL_MAX, DBL_MAX);
		m_pContainer->m_pDataWnd = m_pContainer->DestroyLabWnd(m_pContainer->m_pDataWnd, false);
		m_pContainer->m_pLegWnd  = m_pContainer->DestroyLabWnd(m_pContainer->m_pLegWnd);
		m_pContainer->m_pChartDataView = m_pContainer->DestroyChartDataView();
		m_pContainer->ResetCharts(true);
	}
}