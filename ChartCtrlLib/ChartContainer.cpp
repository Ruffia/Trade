
// ChartContainer.cpp : implementation file
//

#include "stdafx.h"
#include <ctime>
#include "ChartDef.h"
#include "Chart.h"
#include "Axis.h"
#include "ChartProperty.h"
#include "ContainerProperty.h"
#include "DataLabel.h"
#include "ChartContainer.h"
#include "ChartDataView.h"
#include "ChartsXMLSerializer.h"
#include "Util.h"
#include <Shlwapi.h>

using namespace std;
using namespace Gdiplus;


///////////////////////////////////////////////////////////////////////////////
// CChartContainer

IMPLEMENT_DYNAMIC(CChartContainer, CWnd)

CChartContainer::CChartContainer(string_t name) : 
                                     m_chartSelIdx(-1), m_dataViewChartIdx(-1),
                                     m_dataLegPntD(PointD(DBL_MAX, DBL_MAX)), 
                                     m_pDataWnd(NULL), m_pLegWnd(NULL),
                                     m_pChartDataView(NULL), 
                                     m_name(name), m_labelX(string_t(_T("X"))), m_pLabValStrFnPtr(&GetLabelValStr)                                  
{
	m_pProperty = new CContainerProperty;
	m_pProperty->SetContainer(this);
}

CChartContainer::~CChartContainer()
{
  if (m_pChartDataView != NULL)
  {
    if(IsWindow(m_pChartDataView->m_hWnd))
      m_pChartDataView->DestroyWindow();
    delete m_pChartDataView;
    m_pChartDataView = NULL;      // Really you do not need this line
  }

  DestroyLabWnd(m_pDataWnd);
  DestroyLabWnd(m_pLegWnd);

  if (!m_mapCharts.empty())
  {
    MAP_CHARTS::iterator it = m_mapCharts.begin();
    MAP_CHARTS::iterator itE = m_mapCharts.end();
    for (; it != itE; ++it)
    {
      delete it->second;
      it->second = NULL;
    }
    m_mapCharts.clear();  // Again, just in case
  }
}

CChartContainer& CChartContainer::operator=(const CChartContainer& rhs)
{
  if (this != &rhs)
  {
    m_pDataWnd = DestroyLabWnd(m_pDataWnd); // NULL
    m_pLegWnd  = DestroyLabWnd(m_pLegWnd);  //NULL

	*m_pProperty = *(rhs.m_pProperty);

	m_dataLegPntD      = rhs.m_dataLegPntD;  // data legend window
    m_chartSelIdx      = rhs.m_chartSelIdx; 
    m_dataViewChartIdx = -1;  // No Data View for transfer

    m_name            = rhs.m_name;
    m_labelX          = rhs.m_labelX.empty() ? string_t(_T("X")) : rhs.m_labelX;;
    m_pLabValStrFnPtr = rhs.m_pLabValStrFnPtr;

// Copy charts 
    if (!m_mapCharts.empty())
    {
      MAP_CHARTS::iterator itDest = m_mapCharts.begin();
      MAP_CHARTS::iterator itDestE = m_mapCharts.end();
      for (; itDest!= itDestE; ++itDest)
        delete (itDest->second);
      m_mapCharts.clear();
    }

    MAP_CHARTS::const_iterator itCharts = rhs.m_mapCharts.begin();
    MAP_CHARTS::const_iterator itChartsE = rhs.m_mapCharts.end();
    for (; itCharts != itChartsE; ++itCharts)
    {
      CChart* chartPtr = itCharts->second->CloneChart(true);
      m_mapCharts.insert(MAP_CHARTS::value_type(itCharts->first, chartPtr));
    }

    m_mapSelPntsD = rhs.m_mapSelPntsD;
    m_mapDataViewPntsD.clear();

    if (IsWindow(m_hWnd) && IsWindowVisible())
    {
      if (rhs.IsLabWndExist(false))
        CopyDataLegendWnd();
      else
        RefreshWnd();

      if (rhs.IsLabWndExist(true)) 
        ShowNamesLegend();
    }
  }
  return *this;
}

bool CChartContainer::CreateChartCtrlWnd(DWORD dwExStyle, DWORD dwStyle, 
                                        const CRect& wndRect, CWnd* pParent, UINT nID)    
{
  BOOL bRes = CWnd::CreateEx(dwExStyle,
                                AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS),
                                NULL,
                                dwStyle,
                                wndRect,
                                pParent,
                                nID
                             );

  if (bRes)
  {
    if (m_pProperty->m_bTracking&&!m_mapSelPntsD.empty()&&!m_pDataWnd->GetSafeHwnd())
      CopyDataLegendWnd();
    if (m_pProperty->m_bHasNamesLeg&&!m_pLegWnd->GetSafeHwnd())
      ShowNamesLegend();

  }

  return bRes == TRUE ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// Helpers and operations

CChartContainer* CChartContainer::CloneChartContainer(string_t name, bool bCloneState)
{
  CChartContainer* pContainer = new CChartContainer;

  if (name.empty())
    pContainer->m_name   = m_name;
  else
    pContainer->m_name   = name;

  MAP_CHARTS::iterator itCharts = m_mapCharts.begin();
  MAP_CHARTS::iterator itChartsE = m_mapCharts.end();
  for (; itCharts != itChartsE; ++itCharts)
  {
    CChart* cloneChartPtr = itCharts->second->CloneChart(bCloneState);
    pContainer->m_mapCharts.insert(MAP_CHARTS::value_type(cloneChartPtr->GetChartIdx(), cloneChartPtr));
  }

  pContainer->m_pProperty->m_colBkground     = m_pProperty->m_colBkground;
  pContainer->m_pProperty->m_colBorder       = m_pProperty->m_colBorder;
  pContainer->m_pProperty->m_colZoomBorder   = m_pProperty->m_colZoomBorder;
  pContainer->m_pProperty->m_colLegPnts      = m_pProperty->m_colLegPnts;
  pContainer->m_pProperty->m_colDataViewPnts = m_pProperty->m_colDataViewPnts;
  pContainer->m_pProperty->m_colLegBkgnd     = m_pProperty->m_colLegBkgnd;
  pContainer->m_pProperty->m_colLegBorder    = m_pProperty->m_colLegBorder;

  pContainer->m_labelX          = m_labelX;
  pContainer->m_pLabValStrFnPtr = m_pLabValStrFnPtr;

 if (bCloneState)
  {
    pContainer->m_pProperty->m_bTracking     = m_pProperty->m_bTracking;
    pContainer->m_pProperty->m_bHasNamesLeg  = m_pProperty->m_bHasNamesLeg;
    pContainer->m_dataLegPntD   = m_dataLegPntD;
    pContainer->m_pProperty->m_epsX            = m_pProperty->m_epsX;

    pContainer->m_pProperty->m_bUserEnabled  = m_pProperty->m_bUserEnabled;
    pContainer->m_chartSelIdx   = m_chartSelIdx;
    pContainer->m_pProperty->m_bAxBoundaries = m_pProperty->m_bAxBoundaries;


    if (m_pProperty->m_bTracking && IsLabWndExist(false))
      pContainer->m_mapSelPntsD = m_mapSelPntsD;
      
  }

  pContainer->m_pProperty->m_precision     = m_pProperty->m_precision;

  return pContainer;
}

CChart* CChartContainer::CloneChart(int chartIdx, bool bCloneState)
{
  MAP_CHARTS::iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.end())
    return it->second->CloneChart(bCloneState);
  return NULL;
}


size_t CChartContainer::SetDataViewSelPnts(int chartIdx, 
                            const MAP_SELPNTSD& mapSelPntsD, V_CHARTDATAD* pChartData)
{
  if (chartIdx == -1)
    return 0;
  if (chartIdx != m_dataViewChartIdx)
    return 0;
  if (m_pChartDataView != NULL)
  {
    m_pChartDataView->m_mapSelCells = mapSelPntsD;
    if (pChartData == NULL)
    {
      m_pChartDataView->RefreshSelCells();
      if (IsWindow(m_pChartDataView->m_hWnd))
      {
        if (m_pChartDataView->IsIconic())
          m_pChartDataView->ShowWindow(SW_RESTORE);
        m_pChartDataView->RedrawWindow(NULL, NULL, 
                RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE|RDW_ALLCHILDREN);
      }
    }
    else
      ReplaceChartData(chartIdx, *pChartData, false, false, false, true);
  }
  return mapSelPntsD.size();
}

bool CChartContainer::ClearDataViewChartIdx(void)
{
  if (m_pChartDataView != NULL) 
  {
    if (m_dataViewChartIdx != -1)
    {
      m_dataViewChartIdx = -1;
      UpdateDataViewPnts(-1, 0, PointD(0.0, 0.0), false);
    }

    DestroyChartDataView();
    return true;
  }
  return false;
}

void CChartContainer::SetAxisXName(string_t nameX, bool bRedraw)
{
  m_labelX = NormalizeString(nameX, STR_MAXLEN, STR_NORMSIGN);

  if (bRedraw)
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(false);
    else if (m_pProperty->m_bAxBoundaries)
      RefreshWnd();
    if (m_dataViewChartIdx != -1)
      UpdateDataView(m_dataViewChartIdx, F_NODATACHANGE);
  }
}

void CChartContainer::SetLabXValStrFn(val_label_str_fn pLabValStrFn, bool bRedraw)
{
  m_pLabValStrFnPtr = pLabValStrFn;

  if (bRedraw)
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(false);
    else if (m_pProperty->m_bAxBoundaries)
      RefreshWnd();
    if (m_dataViewChartIdx != -1)
      UpdateDataView(m_dataViewChartIdx, F_NODATACHANGE);
  }
}

bool CChartContainer::HasChartWithData(int chartIdx, bool bVisibleOnly) const
{
  MAP_CHARTS::const_iterator it = chartIdx == -1 ? m_mapCharts.cbegin() :m_mapCharts.find(chartIdx);
  bool bHasChart = false;

  for (; it != m_mapCharts.cend(); ++it)
  {
	 CChart* pChart = it->second;
    if (pChart->HasData())
    {
      if (!bVisibleOnly||pChart->IsChartVisible())
        bHasChart = true;
    }
    if (bHasChart ||( chartIdx != -1))
      break;
 } 

  return bHasChart;
}

int CChartContainer::HasChartDifferentPnts(int chartIdx, bool bVisibleOnly) const
{
  MAP_CHARTS::const_iterator it = chartIdx == -1 ? m_mapCharts.cbegin() :
                                                            m_mapCharts.find(chartIdx);
  int resIdx = -1;

  for (; it != m_mapCharts.cend(); ++it)
  { 
    CChart* pChart = it->second;
    if ((pChart->GetMaxValX() > pChart->GetMinValX())||
        (pChart->GetMaxValY() > pChart->GetMinValY())) 
    {
      if (!bVisibleOnly||pChart->IsChartVisible())
        resIdx = pChart->GetChartIdx();
    }
    if ((resIdx > -1) ||( chartIdx != -1))
      break;
  }

  return resIdx;
}


void CChartContainer::OnChangedSize(int cx, int cy)
{
  HideLabWnds();            // Called from parent's/owner's OnSize handler
  MoveWindow(0, 0, cx, cy);
  RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
  SetTimer(2, 50, NULL);
}

void CChartContainer::RefreshWnd(void)
{
  CRect clR, legR, dataR;
  GetClientRect(&clR);

  CRgn rgn1, rgn2, rgn3;
  rgn1.CreateRectRgn(0, 0, clR.right, clR.bottom);
  if (IsLabWndVisible(true))
  {
    m_pLegWnd->GetWindowRect(&legR);
    ScreenToClient(&legR);
    rgn2.CreateRectRgn(legR.left, legR.top, legR.right, legR.bottom);
    rgn1.CombineRgn(&rgn1, &rgn2, RGN_DIFF);
  }
  if (IsLabWndVisible(false))
  {
    m_pDataWnd->GetWindowRect(&dataR);
    ScreenToClient(&dataR);
    rgn3.CreateRectRgn(dataR.left, dataR.top, dataR.right, dataR.bottom);
    rgn1.CombineRgn(&rgn1, &rgn3, RGN_DIFF);
  }

  RedrawWindow(NULL, &rgn1, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);//|RDW_ALLCHILDREN);
}


HRESULT CChartContainer::LoadCharts(LPCTSTR fileName, const MAP_CHARTCOLS& mapContent, bool bClearCharts)
{
  HRESULT hr = CChartsXMLSerializer::XMLToCharts(fileName, this, mapContent, bClearCharts);
  if (hr == S_OK)
  {
    if (IsLabWndExist(false))
      PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, NULL);
 
    UpdateContainerWnds();
  }
  return hr;
}

HRESULT CChartContainer::ReplaceContainerCharts(LPCTSTR fileName)
{
  CChartContainer tmpContainer;
  HRESULT hr =  CChartsXMLSerializer::ReplaceChartsFromXMLFile(fileName, &tmpContainer);
  if (hr == S_OK)
  {
    *this = tmpContainer;
    UpdateContainerWnds();
  }
  return hr;
}

HRESULT CChartContainer::GetChartNamesFromXMLFile(LPCTSTR fileName, MAP_CHARTCOLS& mapContent)
{
  return CChartsXMLSerializer::GetChartNamesFromXMLFile(fileName, mapContent);
}

HRESULT CChartContainer::GetChartNamesFromXMLFile(LPCTSTR fileName, MAP_NAMES& mapNames)
{
  return CChartsXMLSerializer::GetChartNamesFromXMLFile(fileName, mapNames);
}

bool CChartContainer::ExportChartData(string_t chartName, V_CHARTDATAD& vDataPnts) const
{
  CChart* chartPtr = FindChartByName(chartName);
  if (chartPtr != NULL)
  {
    size_t dataSize = chartPtr->m_vDataPnts.size();
    if (dataSize != 0)
    {
      vDataPnts.resize(dataSize);
      copy(chartPtr->m_vDataPnts.cbegin(), chartPtr->m_vDataPnts.cend(), vDataPnts.begin());
    }
    else
      vDataPnts.clear();
    return true;
  }
  return false;
}

bool CChartContainer::ExportChartData(string_t chartName, 
                                std::vector<std::pair<double, double> >& vPairs) const
{
  CChart* chartPtr = FindChartByName(chartName);
  if (chartPtr != NULL)
  {
    size_t dataSize = chartPtr->m_vDataPnts.size();
    if (dataSize != 0)
    {
      vPairs.resize(dataSize);
      for (size_t i = 0; i < dataSize; ++i)
      {
        PointD pntD = chartPtr->m_vDataPnts[i];
        vPairs[i] = make_pair(pntD.X, pntD.Y);
      }
    }
    else
      vPairs.clear();
    return true;
  }
  return false;
}

bool CChartContainer::ExportChartData(string_t chartName, 
                                   std::vector<double>& vX, std::vector<double>& vY) const
{
  CChart* chartPtr = FindChartByName(chartName);
  if (chartPtr != NULL)
  {
    size_t dataSize = chartPtr->m_vDataPnts.size();
    if (dataSize != 0)
    {
      vX.resize(dataSize);
      vY.resize(dataSize);
      for (size_t i = 0; i < dataSize; ++i)
      {
        PointD pntD = chartPtr->m_vDataPnts[i];
        vX[i] = pntD.X;
        vY[i] = pntD.Y;
      }
    }
    else
    {
      vX.clear();
      vY.clear();
    }
    return true;
  }
  return false;
}

// Chart access and manipulation functions
/// 
/// Returns false in the third member of the tuple if the chart does not exist
///
std::tuple<double, double, bool> CChartContainer::GetChartBoundaries(int chartIdx, bool bY) const
{
  MAP_CHARTS::const_iterator itCh = m_mapCharts.find(chartIdx);
  if (itCh != m_mapCharts.cend())
    return bY == false ? make_tuple(itCh->second->GetMinValX(),itCh->second->GetMaxValX(), true):
                         make_tuple(itCh->second->GetMinValY(),itCh->second->GetMaxValY(), true);
  return make_tuple(DBL_MAX, -DBL_MAX, false);
}

CChart* CChartContainer::GetVisibleChart(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = (chartIdx) == -1 ? m_mapCharts.cbegin() :
                                                m_mapCharts.find(chartIdx);
  for (; it != m_mapCharts.cend(); ++it)
  {
    CChart* chartPtr = it->second;
    if (chartPtr->IsChartVisible()&&chartPtr->HasData())
      return chartPtr;
    if (chartIdx != -1)
      break;
  }
  return NULL;
}

bool CChartContainer::IsChartVisible(int chartIdx) const
{
  return GetVisibleChart(chartIdx) == NULL ? false : true;
}

bool CChartContainer::SetChartVisibility(int chartIdx, bool bVisible, bool bRedraw)
{
  bool bRes = false;
  MAP_CHARTS::iterator it = chartIdx == -1 ? m_mapCharts.begin() : m_mapCharts.find(chartIdx);
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
    bRes = true;
	CChart* chartPtr = it->second;
    bool bVisState = chartPtr->IsChartVisible();
    if (bVisState != bVisible)
    {
      chartPtr->SetChartVisibility(bVisible);
      if (bVisible == false) 
      {
        if (it->second->IsChartSelected())
        {
          m_chartSelIdx = -1;
          it->second->SelectChart(false);
        }

        if (m_dataViewChartIdx == it->first)
        {
          if (m_pChartDataView != NULL)
          {
            if (IsWindow(m_pChartDataView->m_hWnd))
              m_pChartDataView->DestroyWindow();
            delete m_pChartDataView;
            m_pChartDataView = NULL;
          }
          UpdateDataViewPnts(-1, 0, PointD(0.0, 0.0), false);
        }
      }
    }
    if (chartIdx != -1)
      break;
  }
    
  if (bRedraw && bRes && IsWindow(m_hWnd) && IsWindowVisible()&&HasChartWithData(chartIdx))
    UpdateContainerWnds();
  
  return bRes;
}

int CChartContainer::ToggleChartVisibility(int chartIdx)
{
  MAP_CHARTS::iterator it = m_mapCharts.find(chartIdx);
  if (it == m_mapCharts.end())
    return -1;

  CChart* chartPtr = it->second;
  bool bVisible = chartPtr->ToggleChartVisibility();
  if (!bVisible)
  {
    if (chartPtr->IsChartSelected())
    {
      m_chartSelIdx = -1;
      chartPtr->SelectChart(false);
    }
    if (m_dataViewChartIdx == chartIdx)
    {
      if (m_pChartDataView != NULL)
      {
        if (IsWindow(m_pChartDataView->m_hWnd))
          m_pChartDataView->DestroyWindow();
        delete m_pChartDataView;
        m_pChartDataView = NULL;
      }
      UpdateDataViewPnts(-1, 0, PointD(0.0, 0.0), false);
    }
  }

  if (chartPtr->HasData()&&IsWindow(m_hWnd) && IsWindowVisible())
    UpdateContainerWnds();

  return bVisible ? 1 : 0;
}

std::pair<float, bool> CChartContainer::GetChartPenWidth(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* chartPtr = it->second;
	  ChartProperty* pAttr = chartPtr->GetProperty();
      return make_pair(pAttr->GetPenWidth(), true);
  }
  return make_pair(0.0f, false);
}

bool CChartContainer::UpdateChartPenWidth(int chartIdx, float penWidth, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;
  MAP_CHARTS::iterator it = (chartIdx == -1) ? m_mapCharts.begin() :
                                             m_mapCharts.find(chartIdx);
  for (; it != m_mapCharts.end(); ++it)
  {
    CChart* chartPtr = it->second;
    ChartProperty* pAttr = chartPtr->GetProperty();
    if (pAttr->GetPenWidth() != penWidth)
    {
      if (!bVisibleOnly ||chartPtr->IsChartVisible())
      {
        pAttr->SetPenWidth(penWidth);
        bRes = true;
      }
    }
    if (chartIdx != -1)
      break;
  }

  if (bRes && bRedraw && IsWindow(m_hWnd) && IsWindowVisible()&&HasChartWithData(chartIdx, true))
    UpdateContainerWnds();

  return bRes;
}

std::pair<float, bool> CChartContainer::GetChartTension(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* chartPtr = it->second;
	  ChartProperty* pAttr = chartPtr->GetProperty();
      return make_pair(pAttr->GetTension(), true);
  }
  return make_pair(FLT_MAX, false);
}

bool CChartContainer::SetChartTension(int chartIdx, float fTension, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;
  if (in_range(0.0f, 1.0f, fTension))
  {
    MAP_CHARTS::iterator it = (chartIdx != -1) ? m_mapCharts.find(chartIdx): m_mapCharts.begin();
    MAP_CHARTS::iterator itE = m_mapCharts.end();
    for (; it != itE; ++it)
    {
		CChart* chartPtr = it->second;
		ChartProperty* pAttr = chartPtr->GetProperty();
		if (pAttr->GetTension() != fTension)
		{
			if (!bVisibleOnly || chartPtr->IsChartVisible())
			{
				pAttr->SetTension(fTension);
				bRes = true;
			}
		}
		if (chartIdx != -1)
			break;
    }

    if (bRes && bRedraw && IsWindow(m_hWnd) && IsWindowVisible() && HasChartWithData(chartIdx, true))
      RefreshWnd();
  }
  return bRes;
}

std::pair<bool, bool> CChartContainer::AreChartPntsAllowed(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* chartPtr = it->second;
	  ChartProperty* pAttr = chartPtr->GetProperty();
      return make_pair(pAttr->ArePntsAllowed(), true);
  }
  return make_pair(false, false);
}

bool CChartContainer::AllowChartPnts(int chartIdx, bool bAllowed, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;
  MAP_CHARTS::iterator it = (chartIdx != -1) ? m_mapCharts.find(chartIdx) :
                                                            m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
    CChart* chartPtr = it->second;
	ChartProperty* pAttr = chartPtr->GetProperty();
    if (pAttr->ArePntsAllowed() != bAllowed)
    {
      if (!bVisibleOnly || chartPtr->IsChartVisible())
      {
        pAttr->AllowPnts(bAllowed);
        bRes = true;
      }
    }
    if (chartIdx != -1)
      break;
  }

  if (bRes && bRedraw && IsWindow(m_hWnd) && IsWindowVisible() && (-1 < HasChartDifferentPnts(chartIdx, true)))
    RefreshWnd();
 
  return bRes;
}

std::pair<Color, bool> CChartContainer::GetChartColor(int chartIdx) const
{
  if (chartIdx != -1)
  {
    MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
    if (it != m_mapCharts.cend())
	{
		CChart* chartPtr = it->second;
		ChartProperty* pAttr = chartPtr->GetProperty();
        return make_pair(pAttr->GetChartColor(), true);
	}
  }

  return make_pair(Color::Black, false);
}

bool CChartContainer::SetChartColor(int chartIdx, Color col, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;

  MAP_CHARTS::iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.end())
  {
    CChart* chartPtr = it->second;
	ChartProperty* pAttr = chartPtr->GetProperty();
    if (pAttr->GetChartColor().GetValue() != col.GetValue())
    {
      if (!bVisibleOnly || chartPtr->IsChartVisible())
      {
        pAttr->SetChartColor(col); 
        if (bRedraw && IsWindow(m_hWnd) && IsWindowVisible() && HasChartWithData(chartIdx, true))
          UpdateContainerWnds();
        bRes = true;
      }
    }
  }
  return bRes;
}

std::pair<DashStyle, bool> CChartContainer::GetChartDashStyle(int chartIdx) const
{
  if (chartIdx != -1)
  {
    MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
    if (it != m_mapCharts.cend())
	{
		CChart* pChart = it->second;
		ChartProperty* pAttr = pChart->GetProperty();
        return make_pair(pAttr->GetChartDashStyle(), true);
	}
  }

  return make_pair(DashStyleSolid, false);
}

bool CChartContainer::SetChartDashStyle(int chartIdx, DashStyle dashStyle, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;
  MAP_CHARTS::iterator it = (chartIdx != -1) ? m_mapCharts.find(chartIdx): m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();

  for (; it != itE; ++it)
  {
    CChart* chartPtr = it->second;
	ChartProperty* pAttr = chartPtr->GetProperty();
    if (pAttr->GetChartDashStyle() != dashStyle)
    {
      if (!bVisibleOnly || chartPtr->IsChartVisible())
      {
        pAttr->SetDashStyle(dashStyle);
        bRes = true;
      }
    }
    if (chartIdx != -1)
      break;
  }

  if (bRes && bRedraw && IsWindow(m_hWnd) && IsWindowVisible() && (-1 <HasChartDifferentPnts(chartIdx, true)))
    UpdateContainerWnds();

  return bRes;
}
  
int CChartContainer::HasSelectedChart(void) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.begin();
  MAP_CHARTS::const_iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
    if (it->second->IsChartSelected())
      return it->first;
  }
  return -1;
}

int CChartContainer::SelectChart(int chartIdx, bool bRedraw)
{
  bool bSel = true;
  CChart* chartPtr = NULL;
  if (m_chartSelIdx != -1)    // Has chart to deselect
  {
    chartPtr = GetChart(m_chartSelIdx);
    if (chartPtr != NULL)     // Somebody programmatically deleted before this call
      chartPtr->SelectChart(false);
    if ((chartIdx == -1)||(chartIdx == m_chartSelIdx))
    {
      m_chartSelIdx = -1;
      bSel = false;
    }
  }

  if (bSel)      // New selection was requested
  {
    chartPtr = GetVisibleChart(chartIdx);
    if (chartPtr != NULL)
    {
      chartPtr->SelectChart(true);
      m_chartSelIdx = chartPtr->GetChartIdx();
    }
    else
      m_chartSelIdx = -1;
  }

  if (bRedraw && IsWindow(m_hWnd) && IsWindowVisible() && HasChartWithData(chartIdx, true))
    RefreshWnd();    

  return m_chartSelIdx;

}

int CChartContainer::SelectChartByMouseClick(CPoint pnt, bool bRedraw)
{
  if (!m_mapCharts.empty())
  {
    PointF epsF(CHART_DTPNTSZ, CHART_PNTSTRSH);
    PointF pntF(float(pnt.x), float(pnt.y));

    CChart* chartPtr;
    MAP_CHARTS::iterator it = m_mapCharts.begin();
    MAP_CHARTS::iterator itE = m_mapCharts.end();

    V_CHARTDATAF vDataPntsF;

    int chartIdx = -1;
    
    for (; it != itE; ++it)
    {
      chartPtr = it->second;
      if (!chartPtr->HasData())
        continue;
      if (!chartPtr->IsChartVisible())
        continue;
// Now look for sel pnts: Convert to the scteen pntsF

	  PointD leftPntD = chartPtr->GetAxis()->m_pMatrixD->TransformToPntD(1.0, pntF - epsF);
	  PointD rightPntD = chartPtr->GetAxis()->m_pMatrixD->TransformToPntD(1.0, pntF + epsF);
      V_CHARTDATAD vDataPntsD = chartPtr->m_vDataPnts;
      chartPtr->ConvertChartData(vDataPntsD, vDataPntsF, chartPtr->GetAxis()->m_pMatrixD, leftPntD.X, rightPntD.X);
      if (vDataPntsF.size() > 0)
      {
        V_CHARTDATAF::iterator itData = find_if(vDataPntsF.begin(), vDataPntsF.end(),
                                                     in_vicinity<double>(pntF, epsF));
        if (itData != vDataPntsF.end())
        {
          chartIdx = chartPtr->GetChartIdx();
          break;
        }
      }
    }
// ChartIdx == -1 if click is not in vicinity of any data point: deselect selected or select forst visible
    SelectChart(chartIdx == -1 ? m_chartSelIdx : chartIdx, bRedraw);
  }

  return m_chartSelIdx;
}

// Chart Y axis precision
int CChartContainer::GetChartPrecisionY(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* chartPtr = it->second;
	  ChartProperty* pAttr = chartPtr->GetProperty();
      return pAttr->GetPrecisionY();
  }
  return INT_MAX;
}

bool CChartContainer::SetChartPrecisionY(int chartIdx, int precisionY, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;
  MAP_CHARTS::iterator it = (chartIdx != -1) ? m_mapCharts.find(chartIdx) :
                                                               m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
	 CChart* chartPtr = it->second;
	 ChartProperty* pAttr = chartPtr->GetProperty();
    if (!bVisibleOnly || chartPtr->IsChartVisible())
    {
      if (pAttr->GetPrecisionY() != precisionY)
      {
        pAttr->SetPrecisionY(precisionY);
        bRes = true;
      }
    }
    if (chartIdx != -1)
      break;
  }

  if (bRes && bRedraw)
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(false);    
    if (chartIdx == -1)
      chartIdx = m_dataViewChartIdx;
    UpdateDataView(chartIdx, F_NODATACHANGE);
  }
  return bRes;
}

string_t CChartContainer::GetChartName(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* pChart = it->second;
	  ChartProperty* pAttr = pChart->GetProperty();
      return pAttr->GetChartName();
  }
  return string_t(_T(""));     // Not a name at all
}

bool CChartContainer::ChangeChartName(int chartIdx, string_t name, bool bRedraw)
{
  MAP_CHARTS::iterator itCh = m_mapCharts.find(chartIdx);
  MAP_CHARTS::iterator it = m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();

  if (itCh != itE)                    // Chart is found
  {
    name = NormalizeString(name, STR_MAXLEN, STR_NORMSIGN); 

    for (; it != itE; ++it)           // Check for uniqueness
    {
		CChart* pChart = it->second;
		ChartProperty* pAttr = pChart->GetProperty();
		if (pAttr->GetChartName() == name)
		  return false;
    }
    itCh->second->GetProperty()->SetChartName(name); // Assign the new nane
    
    if (bRedraw)
      UpdateContainerWnds(chartIdx);

    return true;
  }
  return false;
}

// Chart Y data name
string_t CChartContainer::GetChartYValName(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* pChart = it->second;
	  ChartProperty* pAttr = pChart->GetProperty();
      return pAttr->GetAxisYName();
  }
  return string_t(_T(""));    
}

bool CChartContainer::ChangeChartYValName(int chartIdx, string_t nameY, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;

  nameY = NormalizeString(nameY, STR_MAXLEN, STR_NORMSIGN);
  MAP_CHARTS::iterator it = chartIdx != -1 ? m_mapCharts.find(chartIdx) :
                                                                      m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
	 CChart* pChart = it->second;
	 ChartProperty* pAttr = pChart->GetProperty();
    if (!bVisibleOnly || pChart->IsChartVisible())
    {
      string_t oldNameY = pAttr->GetAxisYName();
      if (oldNameY != nameY)
      {
        pAttr->SetAxisYName(nameY);
        bRes = true;
      }
    }
    if (chartIdx != -1)
      break;
  }
   
  if (bRes &&bRedraw)
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(false);
    UpdateDataView(chartIdx != -1 ? chartIdx : m_dataViewChartIdx, F_NODATACHANGE);
  }
  return bRes;
}

// Function to get Y val strung function
val_label_str_fn CChartContainer::GetLabYValStrFnPtr(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
  {
	  CChart* pChart = it->second;
	  ChartProperty* pAttr = pChart->GetProperty();
      return pAttr->GetLabYValStrFnPtr();
  }
  return NULL;
}

bool CChartContainer::SetLabYValStrFn(int chartIdx, val_label_str_fn pLabYValStrFn, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;

  MAP_CHARTS::iterator it = chartIdx != -1 ? m_mapCharts.find(chartIdx): m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();

  for (; it != itE; ++it)
  {
	CChart* pChart = it->second;
	ChartProperty* pAttr = pChart->GetProperty();
    if (!bVisibleOnly || pChart->IsChartVisible())
    {
      val_label_str_fn oldFnPtr = pAttr->GetLabYValStrFnPtr();
      if (oldFnPtr != pLabYValStrFn)
      {
        pAttr->SetLabYValStrFn(pLabYValStrFn);
        bRes = true;
      }
    }
    if (chartIdx != -1)
      break;
  }
   
  if (bRes && bRedraw)
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(false);
    UpdateDataView(chartIdx != -1 ? chartIdx : m_dataViewChartIdx, F_NODATACHANGE);
  }  
 
  return bRes;
}

CChart* CChartContainer::GetChart(int chartIdx, bool bVisibleOnly) const
{
  CChart* chartPtr = NULL;
  MAP_CHARTS::const_iterator it = chartIdx == -1 ? m_mapCharts.cbegin() : m_mapCharts.find(chartIdx);
  for (; it != m_mapCharts.end(); ++it)
  {
    chartPtr = it->second;
    if (!bVisibleOnly || chartPtr->IsChartVisible())
    {
      if (chartPtr != NULL)
        break;
    }
    if (chartIdx != -1)
      break;
  }
  return chartPtr;
}

V_CHARTDATAD* CChartContainer::GetChartData(int chartIdx)
{
  CChart* chartPtr = GetChart(chartIdx);
  if (chartPtr != NULL)
    return &(chartPtr->m_vDataPnts);
  return NULL;
}

CChart* CChartContainer::FindChartByName(string_t nameStr) const
{
  CChart* chartPtr = NULL;
  MAP_CHARTS::const_iterator it = m_mapCharts.cbegin();
  MAP_CHARTS::const_iterator itE = m_mapCharts.cend();
  for (; it != itE; ++it)
  {
	  CChart* pChart = it->second;
	  ChartProperty* pAttr = pChart->GetProperty();
	  if ( pAttr->GetChartName() == nameStr)
	  {
		  chartPtr = it->second;
		  break;
	  }
  }
  return chartPtr;
}

int CChartContainer::GetChartIdx(string_t chartName) const
{
  CChart* chartPtr = FindChartByName(chartName);
  if (chartPtr != NULL)
    return chartPtr->GetChartIdx();
  return -1;
}

std::pair<double, bool>CChartContainer::GetChartLocScaleY(int chartIdx) const
{
  MAP_CHARTS::const_iterator it = m_mapCharts.find(chartIdx);
  if (it != m_mapCharts.cend())
    return make_pair(it->second->GetLocScaleY(), true);
  return make_pair(0.0, false);
}

bool CChartContainer::UpdateChartLocScaleY(int chartIdx, double locScY, bool bVisibleOnly, bool bRedraw)
{
  bool bRes = false;
  bool bChangeView = false;

  MAP_CHARTS::iterator it = (chartIdx != -1) ? m_mapCharts.find(chartIdx) :
                                                                m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
    CChart* chartPtr = it->second;
    if (!bVisibleOnly || chartPtr->IsChartVisible())
    {
      double locScaleY = chartPtr->GetLocScaleY();
      if (locScaleY != locScY)
      {
        chartPtr->SetLocScaleY(max(0.1, locScY));
        bRes = true;
        if (!bChangeView && bRedraw && chartPtr->IsChartVisible() && chartPtr->HasData())
          bChangeView = true;
      }
    }
    if (chartIdx != -1)
      break;
  }

  if (bChangeView)
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(false);
    else
      RefreshWnd();
  }
  return bRes;
}

// Chart interface functions
int CChartContainer::AddChart(ChartProperty* pAttr, CChart* pChart, V_CHARTDATAD& vData, bool bRedraw)
{
  int chartIdx = GetMaxChartIdx() + 1;
  bool bAddIdx = false;
  if (! pAttr->m_label.empty())
  {
    pAttr->m_label = NormalizeString(pAttr->m_label, STR_MAXLEN, STR_NORMSIGN);
    CChart* twinPtr = FindChartByName(pAttr->m_label);
    if (twinPtr != NULL)
      bAddIdx = true;
  }
  else
  {
    pAttr->m_label = string_t(_T("Cnart"));
    bAddIdx = true;
  }

  if (bAddIdx)
  {
    _TCHAR buffer_t[64];
    _itot_s(chartIdx, buffer_t, 10);  // Chart idx to string
    string_t idxStr(buffer_t);
    pAttr->m_label += string_t(_T("_")) + string_t(buffer_t);
  }
  
  pChart->SetIndex(chartIdx);
  pChart->SetChartAttr(pAttr);
  pChart->SetChartContainer(this);

  size_t dataSize = vData.size();

// Now transfer data and set min max values
  if (dataSize > 0)
  {
    pChart->m_vDataPnts.assign(vData.begin(), vData.end());
    pChart->m_vDataPnts.shrink_to_fit();
 
// It is cheaper to sort right away than to look for max/min x and sort later if needed
    if (dataSize > 1)
      std::sort(pChart->m_vDataPnts.begin(), pChart->m_vDataPnts.end(), less_pnt<double, false>());

    double minValX = pChart->m_vDataPnts.front().X;
    double maxValX = pChart->m_vDataPnts.back().X;

// Find min and max Y; works even for one-point vector
    PAIR_ITS pair_minmaxY = minmax_element(pChart->m_vDataPnts.begin(), pChart->m_vDataPnts.end(), less_pnt<double, true>());
    double minValY = pair_minmaxY.first->Y;
    double maxValY = pair_minmaxY.second->Y;

// Save in the CChart
    pChart->SetMinValX(minValX);
    pChart->SetMaxValX(maxValX);
    pChart->SetMinValY(minValY);
    pChart->SetMaxValY(maxValY);
  }

// Just in case: idx is unique for this container
  if (m_mapCharts.insert(MAP_CHARTS::value_type(pChart->GetChartIdx(), pChart)).second == false)
  {
    delete pChart;
    return -1;
  }

// Now update the container's min maxes, saving the history of X
  if (dataSize > 0)
  {
// Wil automatically take care of previous one-point charts
    pChart->UpdateExtX(pChart->GetMinValX(), pChart->GetMaxValX());
    pChart->UpdateExtY(pChart->GetMinValY(), pChart->GetMaxValY());

    if (IsWindow(m_hWnd) && m_pProperty->m_bTracking && IsLabWndExist(false))
      PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, true);

    if (bRedraw && IsWindow(m_hWnd)&&IsWindowVisible())
      UpdateContainerWnds(-1, true);
  }

  return chartIdx;
}

// Overload for time series
int CChartContainer::AddChart(ChartProperty* pAttr, CChart* pChart, std::vector<double>& vTmSeries, 
                              double startX, double stepX, bool bRedraw)
{
  V_CHARTDATAD vData(vTmSeries.size());
  if (vData.size() > 0)
    transform(vTmSeries.begin(), vTmSeries.end(), vData.begin(), time_series_to_pnt<double>(startX, stepX));

  return AddChart(pAttr,pChart, vData, bRedraw);
}

int CChartContainer::AddChart(ChartProperty* pAttr, CChart* pChart, std::vector<std::pair<double, double> >& vXYData, bool bRedraw)
{
  V_CHARTDATAD vData(vXYData.size());
  if (vData.size() > 0)
    transform(vXYData.begin(), vXYData.end(), vData.begin(), xy_to_pnt<double>());

  return AddChart(pAttr, pChart, vData, bRedraw);
}

int CChartContainer::AddChart(ChartProperty* pAttr, CChart* pChart, std::vector<double>& vX, std::vector<double>& vY, bool bRedraw)
{
  size_t sizeX = vX.size();
  size_t sizeY = vY.size();
  if (sizeX != sizeY)
    return -1;

  V_CHARTDATAD vData(sizeX);
  if (vData.size() > 0)
  {
    for (size_t i = 0; i < sizeX; ++i)
      vData[i] = PointD(vX[i], vY[i]);
  }
  return AddChart(pAttr,pChart, vData, bRedraw);
}

// Appends points to the end of chart data vector. Discards all new points with X < m_endX
bool CChartContainer::AppendChartData( int chartIdx, V_CHARTDATAD& vData, bool bUpdate)
{
  size_t dataSize = vData.size();
  if (dataSize == 0)
    return false;

  MAP_CHARTS::iterator it = m_mapCharts.find(chartIdx); 
  if (it != m_mapCharts.end())
  {
    V_CHARTDATAD vLocData(vData);
// Sort; it is cheaper than search for min/max X and check whether they are at start and end
    if (dataSize > 1)
      sort(vLocData.begin(), vLocData.end(), less_pnt<double, false>());

    CChart* chartPtr = it->second;  // Append data vector
    size_t appended = chartPtr->AppendChartData(vLocData);
    if (appended == 0)
      return false;

    if (IsLabWndExist(false))     // Might update m_pMatrix if m_dataLegPntD is in X range
      PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, true);  

    UpdateDataView(chartIdx, F_APPEND);

    if (bUpdate && chartPtr->IsChartVisible())  // Update and display if only one chart has to be processed
    {
      chartPtr->UpdateExtX(chartPtr->GetMinValX(), chartPtr->GetMaxValX()); // New startX/endX are at the front dqHist
      chartPtr->UpdateExtY(chartPtr->GetMinValY(), chartPtr->GetMaxValY()); // New startY/endY are at the front dqHist

      if (IsWindow(m_hWnd) && IsWindowVisible())
        UpdateContainerWnds(chartIdx, true);
    }
    return true;
  }
  return false;
}

// Overload for time series
bool CChartContainer::AppendChartData(int chartIdx,  std::vector<double>& vTmSeries,  double startX,
                                                                              double stepX, bool bUpdate)
{
  if (vTmSeries.size() == 0)
    return false;

  V_CHARTDATAD vData(vTmSeries.size());
  transform(vTmSeries.begin(), vTmSeries.end(), vData.begin(), time_series_to_pnt<double>(startX, stepX));

  return AppendChartData(chartIdx, vData, bUpdate);
}

// Overload for vector of pairs
bool CChartContainer::AppendChartData(int chartIdx, std::vector<std::pair<double, double> >& vXYData, bool bUpdate)
{
  if (vXYData.size() == 0)
    return false;

  V_CHARTDATAD vData(vXYData.size());
  transform(vXYData.begin(), vXYData.end(), vData.begin(), xy_to_pnt<double>());

  return AppendChartData(chartIdx, vData, bUpdate);
}

// Overload for vectors of doubles
bool CChartContainer::AppendChartData(int chartIdx, std::vector<double>& vX, std::vector<double>& vY, bool bUpdate)
{
  size_t sizeX = vX.size();
  size_t sizeY = vY.size();
  if (sizeX != sizeY)
    return false;
  if (sizeX == 0)
    return false;

  V_CHARTDATAD vData(sizeX);
  for (size_t i = 0; i < sizeX; ++i)
    vData[i] = PointD(vX[i], vY[i]);

  return AppendChartData(chartIdx, vData, bUpdate);
}

bool CChartContainer::ReplaceChartData(int chartIdx, V_CHARTDATAD& vData, bool bClip, bool bUpdate, bool bVerbose, bool bRedraw)
{
  MAP_CHARTS::iterator mapIt = m_mapCharts.find(chartIdx);
  if (mapIt == m_mapCharts.end())
    return false;                           // Wrong Chart ID

 // Warning and reset if (bReset)                               
  if (bVerbose && (AfxMessageBox(CString("You will replace all old chart data. Replace anyway?"), 
                                                      MB_YESNO|MB_ICONQUESTION) == IDNO)) 
    return false;

// Sort input vector; it is cheaper than check first whether it was sorted
  V_CHARTDATAD vLocData(vData);
  if (vLocData.size() > 1)
    sort(vLocData.begin(), vLocData.end(), less_pnt<double, false>());

  CChart* chartPtr = mapIt->second;
  
// Define what take from vData
  V_CHARTDATAD::iterator itStart = vLocData.begin();
  V_CHARTDATAD::iterator itEnd = vLocData.end();

  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  if (bClip) // Asked to keep the new range inside the old startX, endX
  {
    PAIR_ITS pair_its = find_border_pnts(vLocData.begin(), vLocData.end(),
       not_inside_range<double, false>(pAxis->GetInitialStartX(), pAxis->GetInitialEndX()));
    itStart = pair_its.first;
    itEnd   = pair_its.second;
    if (itEnd != vLocData.end())
      ++itEnd;
  }

  chartPtr->m_vDataPnts.assign(itStart, itEnd);  
  chartPtr->m_vDataPnts.shrink_to_fit();

  size_t dataSize = chartPtr->m_vDataPnts.size();

// Set new min/max
  double startX = dataSize == 0 ? DBL_MAX : chartPtr->m_vDataPnts.front().X;
  double endX   = dataSize == 0 ? -DBL_MAX : chartPtr->m_vDataPnts.back().X;
  chartPtr->SetMinValX(startX);
  chartPtr->SetMaxValX(endX);

// Update scales - indirect
  double minY = DBL_MAX, maxY = -DBL_MAX;
  if (dataSize > 0)
  {
    std::pair< V_CHARTDATAD::iterator, V_CHARTDATAD::iterator> pair_minmaxY =
         minmax_element(chartPtr->m_vDataPnts.begin(), chartPtr->m_vDataPnts.end(), less_pnt<double, true>());
    minY = pair_minmaxY.first->Y;
    maxY = pair_minmaxY.second->Y;
  }

  chartPtr->SetMinValY(minY);   
  chartPtr->SetMaxValY(maxY);
  chartPtr->SetLocScaleY(1.0);

// Update m_startX,m_endX, min and max Y
  if (!HasChartWithData(-1))
  {
    ResetChartContainer(true);
  } 
  else
  {
    if (IsLabWndExist(false))     // Might update m_pMatrix if m_dataLegPntD is in X range
      PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, true);  

    UpdateDataView(chartIdx, F_REPLACE);

    if (bUpdate)   // Update and display 
    {
      chartPtr->SetExtX();
      chartPtr->SetExtY();

      if (!chartPtr->HasVisiblePntsDInYBand(-1, pAxis->m_minExtY, pAxis->m_maxExtY))
        ResetCharts();
    }
  }

  if (bRedraw && IsWindow(m_hWnd) && IsWindowVisible())
      UpdateContainerWnds(chartIdx, true);

  return true;
}

// Overload for time series
bool CChartContainer::ReplaceChartData(int chartIdx,  std::vector<double>& vTmSeries, 
                double startX, double stepX, bool bClip, bool bUpdate, bool bVerbose, bool bRedraw)
{
  V_CHARTDATAD vData;
  if (!vTmSeries.empty())
    vData.resize(vTmSeries.size());
  transform(vTmSeries.begin(), vTmSeries.end(), vData.begin(), time_series_to_pnt<double>(startX, stepX));

  return ReplaceChartData(chartIdx, vData, bClip, bUpdate,  bVerbose, bRedraw);
}

// Overload for vector of pairs
bool CChartContainer::ReplaceChartData(int chartIdx,  
    std::vector<std::pair<double, double> >& vXYData, bool bClip, bool bUpdate, bool bVerbose, bool bRedraw)
{
  V_CHARTDATAD vData;
  if (!vXYData.empty())
    vData.resize(vXYData.size());
  transform(vXYData.begin(), vXYData.end(), vData.begin(), xy_to_pnt<double>());

  return ReplaceChartData(chartIdx, vData, bUpdate, bClip, bVerbose, bRedraw);
}
// Overload for vectors of doubles
bool CChartContainer::ReplaceChartData(int chartIdx,  
      std::vector<double>& vX, std::vector<double>& vY, bool bClip, bool bUpdate, bool bVerbose, bool bRedraw)
{

  size_t sizeX = vX.size();
  size_t sizeY = vY.size();
  if (sizeX != sizeY)
    return false;

  V_CHARTDATAD vData;
  if (sizeX > 0)
    vData.resize(sizeX);
  for (size_t i = 0; i < sizeX; ++i)
    vData[i] = PointD(vX[i], vY[i]);

  return ReplaceChartData(chartIdx, vData, bClip, bUpdate, bVerbose, bRedraw);
}

void CChartContainer::ResetCharts(bool bResetLocY)  
{
// Restore startX, endX

  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  pAxis->m_startX = pAxis->GetInitialStartX();
  pAxis->m_endX   = pAxis->GetInitialEndX();

  pAxis->m_minExtY = pAxis->GetInitialMinExtY();
  pAxis->m_maxExtY = pAxis->GetInitialMaxExtY();

// Restore visibility
  MAP_CHARTS::iterator it = m_mapCharts.begin(), itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
    CChart* chartPtr = it->second;
    if (!chartPtr->IsChartVisible())
      chartPtr->ToggleChartVisibility();
    if (bResetLocY)
      chartPtr->SetLocScaleY(1.0);
  }

  pAxis->m_chModeX = MODE_FULLX;
  pAxis->m_chModeY = MODE_FULLY;

  UpdateContainerWnds(-1, true);
}

void CChartContainer::ResetChartContainer(bool bKeepCharts, bool bResetCols)
{
  CChartContainer* pContainer = new CChartContainer;

// Set defaula colors
  if (bResetCols)
  {
    m_pProperty->m_colBkground     = pContainer->m_pProperty->GetBkColor();
    m_pProperty->m_colBorder       = pContainer->m_pProperty->GetBorderColor();
    m_pProperty->m_colZoomBorder   = pContainer->m_pProperty->GetZoomBorderColor();
    m_pProperty->m_colLegPnts      = pContainer->m_pProperty->GetLegPntsColor();
    m_pProperty->m_colDataViewPnts = pContainer->m_pProperty->GetDataViewPntsColor(); 
    m_pProperty->m_colLegBorder    = pContainer->m_pProperty->GetLegBkgndColor();
    m_pProperty->m_colLegBorder    = pContainer->m_pProperty->GetLegBorderColor();
  }

//  Set default values 
  m_pProperty->m_bTracking         = false;
  m_dataLegPntD       = pContainer->GetDataLegPntD();
  m_pProperty->m_bAxBoundaries     = false;
  m_chartSelIdx       = -1;
  m_dataViewChartIdx  = -1;   // Ti be sure, must be -1 if there is no data view
  m_pProperty->m_tmpZX             = 0.0;
  m_pProperty->m_tmpZY             = 0.0;

  delete pContainer;

// Clear maps and vectors
  if (!bKeepCharts && !m_mapCharts.empty())
  {
    MAP_CHARTS::iterator it = m_mapCharts.begin();
    MAP_CHARTS::iterator itE = m_mapCharts.end();
    for (; it != itE; ++it)
      delete it->second;
    m_mapCharts.clear();
  }
  m_mapDataViewPntsD.clear();

// Get rid of the legend wnds
  m_pDataWnd = DestroyLabWnd(m_pDataWnd);  // Deletes m_mapSelPntsD, reset m_dataLegPntD
  m_pLegWnd  = DestroyLabWnd(m_pLegWnd);
// Clear the data view
 DestroyChartDataView();
}

bool CChartContainer::TruncateChart(int chartIdx, double startX, double endX, 
                                                          bool bUpdateExt, bool bRedraw)
{
  if (!HasChartWithData(chartIdx))                                 // No chart with data
    return false;

  if (startX >= endX)                                              // Invalid parameters
    return false;

  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  if ((pAxis->GetInitialStartX() >= startX)&&(pAxis->GetInitialEndX() <= endX))  // Nothing to truncate
    return false;

  startX = max(startX, pAxis->GetInitialStartX());
  endX   = min(endX, pAxis->GetInitialEndX());

  bool bCorrectDataView = false;

  size_t chartCnt = 0;
  MAP_CHARTS::iterator it = (chartIdx == -1) ? m_mapCharts.begin() :
                                                      m_mapCharts.find(chartIdx);
  for (; it != m_mapCharts.end(); ++it)
  {
    CChart* chartPtr = it->second;  // Append data vector 
    if (!chartPtr->HasData())
      continue;
    bool bRes = chartPtr->TruncateChartData(startX, endX);
    if (bRes)
    {
      ++chartCnt;
      if (m_dataViewChartIdx == chartPtr->GetChartIdx())
        bCorrectDataView = true;
    }

    if (chartIdx > -1)
      break;
  }

  if (chartCnt == 0)
    return false;

  if (bUpdateExt) // Update extensions X and Y
  {
    pChart->SetExtX();
    pChart->SetExtY();

    if (!pChart->HasVisiblePntsDInYBand(-1, pAxis->m_minExtY, pAxis->m_maxExtY))
      ResetCharts();
  }

  if (bCorrectDataView)
    UpdateDataView(m_dataViewChartIdx, F_TRUNCATE);

  if (m_pProperty->m_bTracking && IsLabWndExist(false))    // Will calculate and set the new translation matrix
    PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, true);

// Now redraw
  if (bRedraw && IsWindow(m_hWnd) && IsWindowVisible())
  {
    if (IsLabWndVisible(false))
      UpdateDataLegend(true);    // The matrix was not changed yet
    else
      RefreshWnd();               // The matrix will changed in OnPaint()
  }
  return true;
}

bool CChartContainer::RemoveChart(int chartIdx, bool bCorrectMinMax, bool bRedraw)
{
  MAP_CHARTS::iterator mapIt = m_mapCharts.find(chartIdx);
  if (mapIt == m_mapCharts.end())
    return false;

  CChart* chartPtr = mapIt->second;

  delete chartPtr;
  m_mapCharts.erase(chartIdx);

  CAxis* pAxis = chartPtr->GetAxis();
  if (!HasChartWithData(-1))
    ResetChartContainer(true);
  else
  {
    if (m_chartSelIdx == chartIdx)
    {
      m_chartSelIdx = -1;
    }

    if (m_dataViewChartIdx == chartIdx)
    {
     if (m_pChartDataView != NULL)
       DestroyChartDataView();
    }

    if (bCorrectMinMax)
    {
      chartPtr->SetExtX();
      chartPtr->SetExtY();

      if (!chartPtr->HasVisiblePntsDInYBand(-1, pAxis->m_minExtY, pAxis->m_maxExtY))
        ResetCharts();

      if (IsLabWndExist(false))
        PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, bCorrectMinMax);
    }
  }

  if (bRedraw && IsWindow(m_hWnd) && IsWindowVisible())
    UpdateContainerWnds();

  return true;
}

bool CChartContainer::ShowAxisXBoundaries(bool bSet, bool bRedraw)
{
  m_pProperty->m_bAxBoundaries = HasChartWithData(-1) ? bSet : false;
  if (bRedraw)
    RefreshWnd();
  return m_pProperty->m_bAxBoundaries;
}

// The function is called from OnLButtonDown; the container is visible
CDataWnd* CChartContainer::ShowDataLegend(CPoint pnt)
{
  if (m_mapCharts.empty())            // No charts, no legends
    return m_pDataWnd;     

  if (m_pDataWnd == NULL)             // Need to allocate memory
  {
    CDataWnd* pDataWnd = new CDataWnd(m_pProperty->m_colLegBkgnd, m_pProperty->m_colLegBorder);
    if (pDataWnd == NULL)
      return NULL;
    m_pDataWnd = pDataWnd;
  }

// After PrepareDataLegend mapSelData might have some points out the range Y, but mapLabs might be empty  
  if (PrepareDataLegend(pnt, m_pDataWnd->m_mapLabs, m_mapSelPntsD) > 0)
  {                                   // Create dataWnd if needed, and calculate dataWnd rects
    m_pDataWnd->CreateLegend(this, pnt, true);
    RefreshWnd();
    m_pDataWnd->ShowLegendWnd();      // Garanteed m_pDataWnd exists
  }
  else
    m_pDataWnd = DestroyLabWnd(m_pDataWnd);

  return m_pDataWnd;
}


CDataWnd* CChartContainer::ShowNamesLegend(void)
{
  if (IsWindow(m_hWnd)&&IsWindowVisible()&& !m_mapCharts.empty()&& HasChartWithData(-1)) 
  {
    MAP_LABSTR tmpMapStr;
    PrepareNamesLegend(tmpMapStr);
     
// No matter what, allocate memory because there are charts, may be hidden
    CDataWnd* pLegWnd = NULL;
    if (m_pLegWnd == NULL)
    {
      pLegWnd = new CDataWnd(m_pProperty->m_colLegBkgnd, m_pProperty->m_colLegBorder);
      if (pLegWnd == NULL)
        return NULL;
      m_pLegWnd = pLegWnd;
    }
    if (m_pLegWnd)
    {
      m_pLegWnd->m_mapLabs.swap(tmpMapStr);
// Create dataWnd if needed, and calculate dataWnd rects
      if (m_pLegWnd->CreateLegend(this, CPoint(0, 0), false) == true)
      {
        m_pProperty->m_bHasNamesLeg = true;
        RefreshWnd();
        m_pLegWnd->ShowLegendWnd();  // Garanteed m_pLegWnd exists
      }
    }
  }
  else  // It makes no sense to keep memory if there is no container wnd, or charts in container, or chartst with data
    m_pLegWnd = DestroyLabWnd(m_pLegWnd);

  return m_pLegWnd;
}

bool CChartContainer::ShowChartPoints(int chartIdx, bool bShow, bool bRedraw)
{
  bool bRes = false;

  MAP_CHARTS::iterator it = chartIdx == -1 ? m_mapCharts.begin() :
                                                    m_mapCharts.find(chartIdx);
  for (; it != m_mapCharts.end(); ++it)
  {
    CChart* chartPtr = it->second;
	ChartProperty* pAttr = chartPtr->GetProperty();
    if ((chartPtr->GetMaxValX() > chartPtr->GetMinValX())||(chartPtr->GetMaxValY() > chartPtr->GetMinValY())) 
    {
      pAttr->AllowPnts(bShow);
      if (chartPtr->IsChartVisible())
        bRes = true;
    }

    if (chartIdx != -1)
      break;
  }

  if (bRes && bRedraw && IsWindow(m_hWnd) && IsWindowVisible())
    UpdateContainerWnds();
  return bRes;
}


void CChartContainer::UpdateDataLegend(bool bChangeMatrix)
{
	CChart* pChart = GetChart(0,false);
  if (m_pProperty->m_bTracking && IsLabWndExist(false))
  {
    if (bChangeMatrix)
      pChart->SetTransformMatrixD();
    UpdateDataLegend(m_mapSelPntsD, m_pDataWnd->m_mapLabs);  

    RefreshWnd(); // UpdateDataLegend draws dataWnd; RefreshWnd redraws the region excluding dataWnd
  }
}

void CChartContainer::CopyDataLegendWnd(void)
{
  if (m_pDataWnd == NULL)
    m_pDataWnd = new CDataWnd(m_pProperty->m_colLegBkgnd, m_pProperty->m_colLegBorder);
  bool bRes = false;
  if ((m_pDataWnd != NULL)&&!IsWindow(m_pDataWnd->m_hWnd))  
    bRes = m_pDataWnd->CreateLegend(this);
  if (bRes)
    UpdateDataLegend(false);
}

// True for info on m_pLegWnd, false on m_pDataWnd
bool CChartContainer::IsLabWndVisible(bool bLeg) const
{
  CDataWnd* pDataWnd = bLeg ? m_pLegWnd : m_pDataWnd;
  if (pDataWnd != NULL)
    return (IsWindow(pDataWnd->m_hWnd) && pDataWnd->IsWindowVisible())? true : false;

  return false;
}

bool CChartContainer::IsLabWndExist(bool bLeg) const
{
  CDataWnd* pDataWnd = bLeg ? m_pLegWnd : m_pDataWnd;
  if (pDataWnd != NULL)
    return IsWindow(pDataWnd->m_hWnd)? true : false;

  return false;
}

CDataWnd* CChartContainer::DestroyLabWnd(CDataWnd* pWnd, bool bRedraw)
{
  if (pWnd != NULL)
  {
    if (IsWindow(pWnd->m_hWnd))   
    {
      pWnd->DestroyWindow();
      if (pWnd->IsDataLeg())
      {
        m_mapSelPntsD.clear();
        m_dataLegPntD = PointD(DBL_MAX, DBL_MAX);
// To remove selected data line
        if (bRedraw)
          RefreshWnd();
      }
      else
        m_pProperty->m_bHasNamesLeg = false;
    } // IsWindow
    delete pWnd;      // Here mapLegs goes out
    pWnd = NULL;
  }
  return pWnd;
}

void CChartContainer::UpdateContainerWnds(int chartIdx, bool bMatrix, DATAVIEW_FLAGS dataChange)
{
  if (m_pProperty->m_bTracking && IsLabWndExist(false))
  {
    if (HasChartWithData(-1))
      UpdateDataLegend(bMatrix);
    else
    {
      m_pProperty->m_bTracking = false;
      m_pDataWnd = DestroyLabWnd(m_pDataWnd);
    }
  }
  else
    RefreshWnd();

  if (IsLabWndExist(true))
  {
    if (HasChartWithData(-1))
      ShowNamesLegend();
    else
      m_pLegWnd = DestroyLabWnd(m_pLegWnd);
  }

  UpdateDataView(chartIdx, dataChange);
}

bool CChartContainer::HideLabWnds(bool bRedraw)
{
  bool bRes = false;
  if (IsLabWndVisible(false))
  {
    bRes = true;
    m_pDataWnd->ShowWindow(SW_HIDE);
    if (bRedraw)    // To remove sel line
      RefreshWnd();
  }
  if (IsLabWndVisible(true))
  {
    bRes = true;
    m_pLegWnd->ShowWindow(SW_HIDE);
  }
  return bRes;
}

CChartDataView* CChartContainer::DestroyChartDataView(void)
{
  if (m_pChartDataView != NULL)
  {
    if (IsWindow(m_pChartDataView->m_hWnd))
      m_pChartDataView->DestroyWindow();
    m_dataViewChartIdx = -1;
    if (!m_mapDataViewPntsD.empty())
      m_mapDataViewPntsD.clear();
    delete m_pChartDataView;
    m_pChartDataView = NULL;
  }
  return m_pChartDataView;
}

// Protected helpers

// To get max chart Idx in the mapCharts
int CChartContainer::GetMaxChartIdx(void) const
{
  int chartIdx = -1;
  if (!m_mapCharts.empty())
  {
    MAP_CHARTS::const_reverse_iterator rIt = m_mapCharts.rbegin();
    chartIdx = rIt->first;
  }

  return chartIdx;
}

void CChartContainer::MousePntToPntD(CPoint pnt, PointD& dataPntD, MatrixD* pMatrixD, double locScY)
{
  ENSURE(pMatrixD != NULL);

  PointF pntF(static_cast<float>(pnt.x), static_cast<float>(pnt.y));
// Return to a native coordinates; in most uses pntD.Y is not important, hence 1.0
  dataPntD = pMatrixD->TransformToPntD(locScY, pntF);

  // Test for NAN
  if (dataPntD.X != dataPntD.X)
    dataPntD.X = DBL_MAX;
  if (dataPntD.Y != dataPntD.Y)
    dataPntD.Y = DBL_MAX;
}

// Zooming X on mouse left click + MK_SHIFT: first click
double CChartContainer::BeginZoomingX(CPoint pnt, bool bDraw)
{
  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  pAxis->m_chModeX = MODE_ZOOMINGX;
 
  PointD mousePntD;
  MousePntToPntD(pnt, mousePntD, pAxis->m_pMatrixD);
  double startX = mousePntD.X;
  if (bDraw)
    DrawZoomBorderX(pnt);

  return startX;
}

// Zooming on mouse left click + MK_SHIFT: the second click
double CChartContainer::PrepareZoomX(CPoint pnt, double startX, bool bDraw)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  PointD mousePntD;
  MousePntToPntD(pnt, mousePntD, pAxis->m_pMatrixD);
  double endX = mousePntD.X;

  PointF startPntF = pAxis->m_pMatrixD->TransformToPntF(1.0, PointD(m_pProperty->m_tmpZX, 0.0));

  float zoomW = fabs(pnt.x - startPntF.X);
  if (zoomW > DR_SPACE)
  {
    if (true == pChart->ZoomMoveContainerX(startX, endX, false)) // old m_endX, m_startX in m_vHistX
    {
      if (bDraw)
        DrawZoomBorderX(pnt);
    }
  }

  return endX;
}

// Zooming Y on mouse double left click + MK_SHIFT: first click
double CChartContainer::BeginZoomingY(CPoint pnt, bool bDraw)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  PointD mousePntD;
  MousePntToPntD(pnt, mousePntD, pAxis->m_pMatrixD);
  double startY = mousePntD.Y;

  if (bDraw)
  {
    RefreshWnd();         // Because first click of a double draws zoom X boundary
    DrawZoomBorderY(pnt);
  }

  pAxis->m_chModeY = MODE_ZOOMINGY;
  return startY;
}

double CChartContainer::PrepareZoomY(CPoint pnt, double startY, bool bDraw)  // Second click
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  PointD mousePntD;
  MousePntToPntD(pnt, mousePntD, pAxis->m_pMatrixD);
  double endY = mousePntD.Y;

  PointF startPntF = pAxis->m_pMatrixD->TransformToPntF(1.0, PointD(0.0, startY));
  float zoomW = fabs(pnt.y - startPntF.Y);

  if (zoomW > DR_SPACE)
  {
    if (true == pChart->ZoomMoveContainerY(startY, endY, false, false)) // old m_endX, m_startX in m_vHistY
    {
      if (bDraw)
        DrawZoomBorderY(pnt);
    }
  }

  return endY;
}


// Protected data and names legend functions    
size_t CChartContainer::PrepareDataLegend(CPoint pnt, MAP_LABSTR& mapLabels, 
                                                    MAP_SELPNTSD& mapSelPntsD)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  PointD dataLegPntD;
  MousePntToPntD(pnt, dataLegPntD, pAxis->m_pMatrixD);

  double epsX = CHART_DTPNTSZ/pAxis->m_pMatrixD->m_scX; // m11 is the scX

  size_t mapLabelsSize = PrepareDataLegend(dataLegPntD, epsX, mapLabels, mapSelPntsD, false);
  if (mapSelPntsD.size() > 0)
  {
    m_dataLegPntD = dataLegPntD;
    m_pProperty->m_epsX        = epsX;
  }
  else 
    m_dataLegPntD = PointD(DBL_MAX, DBL_MAX);

  return mapLabelsSize;
}

size_t CChartContainer::PrepareDataLegend(PointD origPntD, double epsX, MAP_LABSTR& mapLabels,
                                                     MAP_SELPNTSD& mapSelPntsD, bool bChangeMatrix)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();

  if (bChangeMatrix)
    pChart->SetTransformMatrixD();

  mapSelPntsD.clear();
  mapLabels.clear();

  if(in_range(pAxis->m_startX, pAxis->m_endX, origPntD.X)) // Make sense if pnt was sent by a programmer
  {
    CRect clientR;              // Get Y-boundaries in the data space
    GetClientRect(&clientR);
    CPoint topPnt(0, clientR.top);
    CPoint bottomPnt(0, clientR.bottom);
    PointD topPntD;
    PointD bottomPntD;
    MousePntToPntD(topPnt, topPntD, pAxis->m_pMatrixD);
    MousePntToPntD(bottomPnt, bottomPntD, pAxis->m_pMatrixD);

//Search the chart map
    PointD selPntD;
    
    MAP_CHARTS::iterator it = m_mapCharts.begin();
    MAP_CHARTS::iterator itE = m_mapCharts.end();
    
    for (; it != itE; ++it)
    {
      CChart* chartPtr = it->second;
      bool bVisible = chartPtr->IsChartVisible();

      PAIR_ITNEAREST pair_res = 
                            chartPtr->GetNearestPointD(origPntD, epsX, selPntD);
      int nSel = pair_res.second;
      if (nSel > 0)    // Might be out of the client rect
      {
        V_CHARTDATAD::iterator itSel = pair_res.first;
        for (int i = nSel; i > 0; --i)
        {
          PointD selPntD = *itSel;
          double selY = selPntD.Y * chartPtr->GetLocScaleY();
          int chartIdx = chartPtr->GetChartIdx();
          mapSelPntsD.insert(MAP_SELPNTSD::value_type(chartIdx, selPntD));
          if (bVisible && in_range(bottomPntD.Y, topPntD.Y, selY))    // Remember, Y was reversed
          {
            TUPLE_LABEL tuple_res = chartPtr->GetSelValString(selPntD, m_labelX, m_pProperty->m_precision, m_pLabValStrFnPtr);
            mapLabels.insert(MAP_LABSTR::value_type(chartIdx , tuple_res));
          }
          if (i > 1)
          --itSel;
        }
      }
    }
    
    return mapLabels.size();
  }
  return 0;
}

size_t CChartContainer::PrepareNamesLegend(MAP_LABSTR& mapLabels)
{
  if (m_mapCharts.empty())
    return 0;

  if (!mapLabels.empty())
    mapLabels.clear();

  TUPLE_LABEL tuple_res;
  MAP_CHARTS::iterator it = m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)  
  {
    bool bSelected = it->second->GetVisibleChartNameAndVisuals(tuple_res);
    if (bSelected)
      mapLabels.insert(MAP_LABSTR::value_type(it->first, tuple_res));
  }
  return mapLabels.size();
}

// Refreshes label string map according to charts' visibility and X range
size_t CChartContainer::UpdateDataLegend(MAP_SELPNTSD& mapSelPntsD, MAP_LABSTR& mapLabStr)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  mapLabStr.clear();
  if (!mapSelPntsD.empty()&& in_range(pAxis->m_startX, pAxis->m_endX, m_dataLegPntD.X))
  {
    CRect clRect;
    GetClientRect(&clRect);
    CPoint pntLimYL(0, clRect.bottom);
    CPoint pntLimYR(0, clRect.top);
    PointD pntLimYLD, pntLimYRD;
    MousePntToPntD(pntLimYL, pntLimYLD, pAxis->m_pMatrixD);
    MousePntToPntD(pntLimYR, pntLimYRD, pAxis->m_pMatrixD);

    MAP_SELPNTSD::iterator itSel = mapSelPntsD.begin();
    MAP_SELPNTSD::iterator itSelE = mapSelPntsD.end();
    while(itSel != itSelE)
    {
      int chartIdx = itSel->first;
      CChart* chartPtr = GetChart(chartIdx);
      if (chartPtr != NULL)
      {
        if (chartPtr->IsChartVisible())
        {
          PointD selPntD = itSel->second;
          if (in_range(pAxis->m_startX, pAxis->m_endX, selPntD.X)&&
            in_range(pntLimYLD.Y, pntLimYRD.Y, selPntD.Y*chartPtr->GetLocScaleY()))
          {
            TUPLE_LABEL tuple_res = chartPtr->GetSelValString(selPntD, m_labelX, m_pProperty->m_precision, m_pLabValStrFnPtr);
            mapLabStr.insert(MAP_LABSTR::value_type(chartIdx, tuple_res));
          }
        }
        ++itSel;
      }
      else
       itSel = mapSelPntsD.erase(itSel);
    }
  }

  CPoint origPnt(-1, -1); // Not used on empty mapLabStr
  if (!mapLabStr.empty())
  {
    PointF origPntF = pAxis->m_pMatrixD->TransformToPntF(1.0, m_dataLegPntD);
    origPnt = CPointFromPntF(origPntF);
  }
// Recalc dataWnd window rects and show data wnd or will hide it
  m_pDataWnd->UpdateDataLegend(mapLabStr, this, origPnt);
  return mapLabStr.size();
}


bool CChartContainer::ShowDataView(CChart* chartPtr, bool bClearMap, bool bRefresh)
{
  if (m_pChartDataView == NULL)
    m_pChartDataView = new CChartDataView;

  if (m_pChartDataView != NULL)
  {
    if (!IsWindow(m_pChartDataView->m_hWnd))
    {
      CRect parentWndRect;
      GetParent()->GetWindowRect(&parentWndRect); // App main dlg window

      CRect workRect;
      SystemParametersInfo(SPI_GETWORKAREA, NULL, &workRect, 0);

      int leftX  = parentWndRect.right + DV_SPACE;
      int rightX = leftX + DV_RECTW;
      int topY   = parentWndRect.top - DV_SPACE;
      int bottomY = topY + DV_RECTH;

      CRect dataViewRect(leftX, topY, rightX, bottomY);
      CRect interRect;
      interRect.IntersectRect(&dataViewRect, workRect);
      if (interRect != dataViewRect)
      {
        dataViewRect.right = workRect.right - DV_SPACE;
        dataViewRect.left = max(dataViewRect.right - DV_RECTW, workRect.left + DV_SPACE);
        dataViewRect.top = workRect.top + DV_SPACE;
        dataViewRect.bottom = min(dataViewRect.top + DV_RECTH, workRect.bottom - DV_SPACE);
      }

      BOOL bRes =  m_pChartDataView->CreateEx(0,
                                 AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS),
                                 _T("Chart Data View"), 
                                 WS_POPUPWINDOW|WS_CAPTION|WS_MINIMIZEBOX|WS_VISIBLE,
                                 dataViewRect.left, dataViewRect.top, dataViewRect.Width(), dataViewRect.Height(),
                                 NULL, 
                                 NULL,
                                 NULL);
      if (!bRes)
      {
        delete m_pChartDataView;
        m_pChartDataView = NULL;
        return false;
      }
    }
    else if (m_pChartDataView->IsIconic())
      m_pChartDataView->ShowWindow(SW_RESTORE);

   m_pChartDataView->SetOwner(this);
   int chartIdx = chartPtr->GetChartIdx();
   m_pChartDataView->ShowWaitMessage(chartIdx, chartPtr->m_vDataPnts.size());
   m_pChartDataView->InitParams(chartPtr, bClearMap, this);

    if (m_dataViewChartIdx != chartIdx)
    {
      m_dataViewChartIdx = chartIdx;
      bClearMap = true;
    }

    if (bClearMap)
    {
      m_mapDataViewPntsD.clear();
      if (bRefresh)
        RefreshWnd();
    }
  }
  return true;
}

bool CChartContainer::ShowDataView(const string_t& szChartName, bool bRefresh)
{
  if (m_mapCharts.empty()&&!HasChartWithData(-1, true))
    return false;

  int chartIdx  = GetChartIdx(szChartName);
  if (chartIdx == -1)
    return false;

  int selChartIdx = SelectChart(chartIdx, true);      // Return -1 if the chart is not visible or without data
  if (selChartIdx == -1)
    return false;

  CChart* chartPtr = GetChart(chartIdx);      
  return ShowDataView(chartPtr, bRefresh);
}

bool CChartContainer::UpdateDataView(int chartIdx, DATAVIEW_FLAGS flagsData)
{
  if ((m_pChartDataView != NULL) && (IsWindow(m_pChartDataView->m_hWnd)))
  {
    if ((chartIdx != -1) && (chartIdx == m_dataViewChartIdx))
    {
      CChart* chartPtr = GetChart(chartIdx);
      if ((chartPtr != NULL) && chartPtr->IsChartVisible())
        return m_pChartDataView->UpdateParams(chartPtr, flagsData);
    }
  }
  return false;
}

size_t CChartContainer::UpdateDataViewPntsDMap(void)
{
  if (m_pChartDataView != NULL)
    m_mapDataViewPntsD = m_pChartDataView->m_mapSelCells;
  else
    m_mapDataViewPntsD.clear();

  return m_mapDataViewPntsD.size();
}

// This returns chartPtr->m_loScY as a first, and deltaLocScY as a second; delta = 1% of Y range
PAIR_DBLS CChartContainer::GetLocScYDelta(CChart* chartPtr)
{
  if (IsWindow(m_hWnd))
  {
    CRect clR;
    GetClientRect(&clR);
    double deltaY = 0.01*clR.Height()/chartPtr->GetAxis()->m_scY;
    double locScY = chartPtr->GetLocScaleY();
    double maxY = max(fabs(chartPtr->GetMaxValY()), fabs(chartPtr->GetMinValY()));
    double deltaK = deltaY/maxY;
    return make_pair(locScY, deltaK);
  }
  return make_pair(1.0, 0.0);
}

void CChartContainer::MoveChartsByArrowKeysX(UINT nChar)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  double deltaX = 0.0;
  if (nChar == VK_RIGHT)
    deltaX = MOVE_DELTAX*(pAxis->m_endX - pAxis->m_startX);
  else if (nChar == VK_LEFT)
    deltaX = -MOVE_DELTAX*(pAxis->m_endX - pAxis->m_startX);
  else 
    return;

  pChart->MoveContainerChartsX(deltaX, true);
}

void CChartContainer::ChangeLocScaleYByArrowKeys(UINT nChar)
{
  if (m_chartSelIdx > -1)
  {
    MAP_CHARTS::iterator it = m_mapCharts.find(m_chartSelIdx);
    if (it != m_mapCharts.end())
    {
      CChart* chartPtr= it->second;
      if (chartPtr->IsChartSelected() && chartPtr->IsChartVisible())
      {
        std::pair<double, double> pair_scY = GetLocScYDelta(chartPtr);
        double locScY = pair_scY.first;
        double deltaK = pair_scY.second;

        switch (nChar)
        {
        case VK_DOWN: locScY = max(locScY - deltaK, DELTA_LOCY);      break; 
        case VK_UP:   locScY += deltaK;                               break; 
        case VK_PRIOR: locScY += 10.0*deltaK;                         break; 
        case VK_NEXT:locScY = max(locScY - 10.0*deltaK, DELTA_LOCY);  break;
        }

        chartPtr->SetLocScaleY(locScY);  
        
        if (IsLabWndExist(false))
        {
          if (m_pDataWnd->IsWindowVisible())
            m_pDataWnd->ShowWindow(SW_HIDE);
          SetTimer(1, 500, NULL);             // To fight flicker
        }
        RefreshWnd();
        chartPtr->SendNotification(CODE_SCY, chartPtr->GetChartIdx());
     }
   }
 }
}

void CChartContainer::MoveChartsByArrowKeysY(UINT nChar)
{
// Calculate shift
  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  double shiftValY = 0.0;
  double deltaY = MOVE_DELTAY*(pAxis->m_maxExtY - pAxis->m_minExtY);
  switch (nChar)
  {
    case VK_DOWN:  shiftValY = -deltaY;      break; 
    case VK_UP:    shiftValY =  deltaY;      break; 
    case VK_PRIOR: shiftValY = 10.0*deltaY;  break; 
    case VK_NEXT:  shiftValY = -10.0*deltaY; break;
  }

  pChart->MoveContainerChartsY(shiftValY, true);
  pChart->SendNotification(CODE_EXTY,0);
}



void CChartContainer::DrawBkground(RectF rClientF, Color colBkground, Color colBorder, 
                                                                  Graphics* grPtr)
{
  SolidBrush bkBrush(colBkground);
  grPtr->FillRectangle(&bkBrush, rClientF);

  Pen borderPen(colBorder);
  grPtr->DrawRectangle(&borderPen, rClientF);

  if (this == GetFocus())
  {
    borderPen.SetDashStyle(DashStyleDash);
    grPtr->DrawRectangle(&borderPen, rClientF);
  }
}

bool CChartContainer::DrawSelPntsD(CChart* chartPtr, const MAP_SELPNTSD& mapSelPntsD, 
                GraphicsPath* grPathPtr, Graphics* grPtr, RectF drawRF, float dpiRatio)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  if (!in_range(pAxis->m_startX, pAxis->m_endX, m_dataLegPntD.X))
    return false;
  if (mapSelPntsD.empty() || (chartPtr == NULL))
    return false;

  drawRF.Inflate(DR_SPACE*dpiRatio, DR_SPACE*dpiRatio);

// Get screen pntsF for the given chart
  V_CHARTDATAD vDataPntsD(mapSelPntsD.size());
  V_CHARTDATAF vDataPntsF;

  int chartIdx = chartPtr->GetChartIdx();
// Find first entry
  MAP_SELPNTSD::const_iterator itSel = mapSelPntsD.find(chartIdx); 
  if (itSel != m_mapSelPntsD.cend())                              // The chart has selected points                         
  {
    size_t count = m_mapSelPntsD.count(chartIdx);
    for (size_t i = 0; i < count; ++i)
    {
      PointD selPntD = itSel->second;
      if (in_range(pAxis->m_startX, pAxis->m_endX, selPntD.X))             // Sometimes it will draw out of clip area
      {
        PointF selPntF = pAxis->m_pMatrixD->TransformToPntF(chartPtr->GetLocScaleY(), selPntD);
        if (drawRF.Contains(selPntF))
          vDataPntsF.push_back(selPntF);
      }
      ++itSel;
    }
// Draw
    if (!vDataPntsF.empty())
    {
      V_CHARTDATAF::iterator it = vDataPntsF.begin(), itE = vDataPntsF.end();
  
      for (; it != itE; ++it)
      {
        PointF selPntF = *it;
        RectF rPntF = RectFFromCenterF(selPntF, dpiRatio*CHART_DTPNTSZ, dpiRatio*CHART_DTPNTSZ);
        rPntF.Inflate(dpiRatio*1.5f, dpiRatio*1.5f);
        grPathPtr->AddEllipse(rPntF);
      }

      Pen selPen(m_pProperty->m_colLegPnts, dpiRatio*1.0f);
      grPtr->DrawPath(&selPen, grPathPtr);

      grPathPtr->Reset();
      return true;
    }   // For vDataPntsF
  }
  return false;
}

bool CChartContainer::DrawDataViewPntsD(const MAP_SELPNTSD& mapDataViewPntsD, double startX, 
                          double endX, GraphicsPath* grPathPtr, Graphics* grPtr, float dpiRatio)
{
  if (mapDataViewPntsD.empty())
    return false;
  if (NULL == GetVisibleChart(m_dataViewChartIdx))
    return false;
  
  CChart* chartPtr = GetChart(m_dataViewChartIdx);
  if ((chartPtr == NULL)||!chartPtr->IsChartVisible())
    return false;

// Extract dataPoints from the map into a vector
  V_CHARTDATAD vDataPntsD(mapDataViewPntsD.size());
  transform(mapDataViewPntsD.begin(), mapDataViewPntsD.end(), vDataPntsD.begin(), get_map_value<int, PointD>());
// Exclude elements out of the range and convert to screen pntsF
  V_CHARTDATAF vDataPntsF;
  if (!chartPtr->ConvertChartData(vDataPntsD, vDataPntsF, chartPtr->GetAxis()->m_pMatrixD, startX, endX))
    return false;

  vDataPntsF.shrink_to_fit();

// Finally, draw
  V_CHARTDATAF::iterator it = vDataPntsF.begin();
  V_CHARTDATAF::iterator itE = vDataPntsF.end();
  for (; it != itE; ++it)
  {
    RectF rPntF = RectFFromCenterF(*it, dpiRatio*(CHART_DTPNTSZ + 2.0f), 
                                                    dpiRatio*(CHART_DTPNTSZ+ 2.0f));
    grPathPtr->AddEllipse(rPntF);
  }
  
  Pen selPen(m_pProperty->m_colDataViewPnts, dpiRatio*2.0f);
  grPtr->DrawPath(&selPen, grPathPtr);
  grPathPtr->Reset();

  return true;
}

void CChartContainer::DrawSelLine(RectF clRF, Graphics* grPtr, int selChartIdx, float dpiRatio)
{
  if (m_mapSelPntsD.empty())
    return;
  if (NULL == GetVisibleChart(-1))
    return;

  bool bShowLine = false;
  if (IsLabWndVisible(false))
    bShowLine = true;
  else if (dpiRatio != 1.0f)   // The second check: on a printout there is no dataWnd
  {
    clRF.Inflate(DR_SPACE*dpiRatio, DR_SPACE*dpiRatio);
    PointF upperLeftF(clRF.X, clRF.Y);
    PointF bottomRightF(clRF.GetRight(), clRF.GetBottom());

    auto itSel = selChartIdx > -1 ? m_mapSelPntsD.find(selChartIdx) : m_mapSelPntsD.cbegin();
    auto itSelE = m_mapSelPntsD.cend();
    for (; itSel != itSelE; ++itSel)
    {
      if ((selChartIdx > -1)&&(itSel->first != selChartIdx))
        break;
      PointD pntD = itSel->second;
      CChart* chartPtr = GetChart(itSel->first);
      double locScY = chartPtr->GetLocScaleY();
	  PointD upperLeftD = chartPtr->GetAxis()->m_pMatrixD->TransformToPntD(1.0, upperLeftF);
	  PointD bottomRightD = chartPtr->GetAxis()->m_pMatrixD->TransformToPntD(1.0, bottomRightF);
      PointF pntF = chartPtr->GetAxis()->m_pMatrixD->TransformToPntF(locScY, pntD);
      if (clRF.Contains(pntF))
      {
        bShowLine = true; 
        break;
      }
    }
  }

  if (bShowLine)
  {
// Get client coord of the data line; scY does notmatter
	CChart* chartPtr = GetChart(selChartIdx);
	if(!chartPtr) return;

    PointF pntF = chartPtr->GetAxis()->m_pMatrixD->TransformToPntF(1.0, m_dataLegPntD);
// Again, no problem with printing out of a clipping rectangle
    Pen selPen(m_pProperty->m_colLegPnts, dpiRatio*1.0f);
    grPtr->DrawLine(&selPen, pntF.X, clRF.Y, pntF.X, clRF.GetBottom());
  }
}

void CChartContainer::DrawAxXBoundaryStr(RectF boundRF, Graphics* grPtr, float dpiRatio)
{
  USES_CONVERSION;
  string_t strStartX, strEndX;

  //string_t strSpace(_T(" "));
  //strStartX = m_labelX + strSpace + m_pLabValStrFnPtr(m_startX, m_pProperty->m_precision, bAddEqSign);
  //strEndX   = m_labelX + strSpace + m_pLabValStrFnPtr(m_endX, m_pProperty->m_precision, bAddEqSign);
  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  string_t strSpace(_T(" "));
  strStartX = m_pLabValStrFnPtr(pAxis->m_startX, m_pProperty->m_precision, false);
  strEndX   = m_pLabValStrFnPtr(pAxis->m_endX, m_pProperty->m_precision, false);

  FontFamily fontFamily(L"Verdana");
  Gdiplus::Font axFont(&fontFamily, 7.0f, FontStyleBold);
  grPtr->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

// Measure strings
  boundRF.Inflate(-dpiRatio*2.0f, -dpiRatio*2.0f);
  RectF startRF, endRF;
  PointF startPntF, endPntF;

  grPtr->MeasureString(A2W(strStartX.c_str()), -1, &axFont, boundRF, &startRF); 
  grPtr->MeasureString(A2W(strEndX.c_str()), -1, &axFont, boundRF, &endRF);

  if ((pAxis->m_axYPos == MIDDLE)||(pAxis->m_axYPos == TOP))
  {
    startPntF.Y = pAxis->m_axOffsXF + dpiRatio*DR_SPACE;
    endPntF.Y    = pAxis->m_axOffsXF + dpiRatio*DR_SPACE;
    if ((pAxis->m_axYPos == MIDDLE)&&((endPntF.Y + startRF.Width) > boundRF.GetBottom()))
    {
       startPntF.Y = pAxis->m_axOffsXF - startRF.Height - dpiRatio*(3.0f + DR_SPACE);
       endPntF.Y   = pAxis->m_axOffsXF - endRF.Height - dpiRatio*(3.0f + DR_SPACE);
    }
  }
  else if (pAxis->m_axYPos == BOTTOM)
  {
    startPntF.Y = pAxis->m_axOffsXF - startRF.Height - dpiRatio*(3.0f + DR_SPACE);
    endPntF.Y   = pAxis->m_axOffsXF - endRF.Height - dpiRatio*(3.0f + DR_SPACE);
  }

  if (pAxis->m_axXPos == BETWEEN)
  {
    startPntF.X = startRF.X  + dpiRatio*DR_SPACE;
    endPntF.X   = boundRF.GetRight() - endRF.Width - dpiRatio*DR_SPACE;
  }
  else if (pAxis->m_axXPos == LEFT)
  {
    startPntF.X = startRF.X  + dpiRatio*(3.0f + DR_SPACE);
    endPntF.X   = boundRF.GetRight() - endRF.Width - dpiRatio*DR_SPACE;
  }
  else if (pAxis->m_axXPos == RIGHT)
  {
    startPntF.X = startRF.X  + dpiRatio*DR_SPACE;
    endPntF.X   = boundRF.GetRight() - endRF.Width - dpiRatio*(3.0f + DR_SPACE);
  }
  else
    return;

  startRF.X = startPntF.X;
  startRF.Y = startPntF.Y;
  endRF.X   = endPntF.X;
  endRF.Y   = endPntF.Y;

  SolidBrush axBkBrush(m_pProperty->m_colBkground);
  SolidBrush axLabBr(pAxis->m_colAxis);
  
  grPtr->FillRectangle(&axBkBrush, startRF);
  grPtr->DrawString(A2W(strStartX.c_str()), -1, &axFont, startPntF, &axLabBr);

  grPtr->FillRectangle(&axBkBrush, endRF);
  grPtr->DrawString(A2W(strEndX.c_str()), -1, &axFont, endPntF, &axLabBr);
}

// Used to draw Zoom Border out of OnPaint function
void CChartContainer::DrawZoomBorderX(CPoint pnt)
{
  CDC* pDC = GetDC();
  GraphicsPath grPath;

  CRect clR;
  GetClientRect(&clR);

  CPoint tmpPnt;
  CRect nameR;
  if (IsLabWndVisible(true))
  {
    m_pLegWnd->GetWindowRect(&nameR);
    ScreenToClient(&nameR);
    tmpPnt = CPoint(pnt.x, nameR.CenterPoint().y);
    if (!nameR.PtInRect(tmpPnt))
      nameR.SetRectEmpty();
  }

  CRect dataR;
  if (IsLabWndVisible(false))
  {
    m_pDataWnd->GetWindowRect(&dataR);
    ScreenToClient(&dataR);
    tmpPnt = CPoint(pnt.x, dataR.CenterPoint().y);
    if (!dataR.PtInRect(tmpPnt))
      dataR.SetRectEmpty();
  }

  Point startPnt(pnt.x, clR.top);
  Point endX;
  if (!nameR.IsRectEmpty())
  {
    grPath.AddLine(startPnt, Point(pnt.x, nameR.top));
    grPath.StartFigure();
    startPnt.Y = nameR.bottom;
    if (!dataR.IsRectEmpty())
    {
      grPath.AddLine(startPnt, Point(pnt.x, dataR.top));
      grPath.StartFigure();
      startPnt.Y = dataR.bottom;
    }
    grPath.AddLine(startPnt, Point(pnt.x, clR.bottom));
  }
  else if (!dataR.IsRectEmpty())
  {
    grPath.AddLine(startPnt, Point(pnt.x, dataR.top));
    grPath.StartFigure();
    startPnt.Y = dataR.bottom;
  }
  grPath.AddLine(startPnt, Point(pnt.x, clR.bottom));

  Graphics gr(pDC->m_hDC);
  Pen brdPen(m_pProperty->m_colZoomBorder);
  gr.DrawPath(&brdPen, &grPath);
//  gr.DrawLine(&brdPen, pnt.x, clR.top, pnt.x, dataR.top);//clR.bottom);
  ReleaseDC(pDC);
}

void CChartContainer::DrawZoomBorderY(CPoint pnt)
{
  CDC* pDC = GetDC();
  GraphicsPath grPath;

  CRect clR;
  GetClientRect(&clR);

  CPoint tmpPnt;

  CRect dataR;
  if (IsLabWndVisible(false))
  {
    m_pDataWnd->GetWindowRect(&dataR);
    ScreenToClient(&dataR);
    tmpPnt = CPoint(dataR.CenterPoint().x, pnt.y);
    if (!dataR.PtInRect(tmpPnt))
      dataR.SetRectEmpty();
  }

  CRect nameR;
  if (IsLabWndVisible(true))
  {
    m_pLegWnd->GetWindowRect(&nameR);
    ScreenToClient(&nameR);
    tmpPnt = CPoint(nameR.CenterPoint().x, pnt.y);
    if (!nameR.PtInRect(tmpPnt))
      nameR.SetRectEmpty();
  }

  Point startPnt(clR.left, pnt.y);
  Point endY;

  if (!dataR.IsRectEmpty())
  {
    grPath.AddLine(startPnt, Point(dataR.left, pnt.y));
    grPath.StartFigure();
    startPnt.X = dataR.right;
  }

  if (!nameR.IsRectEmpty())
  {
    grPath.AddLine(startPnt, Point(nameR.left, pnt.y));
    grPath.StartFigure();
    startPnt.X = nameR.right;
    if (!dataR.IsRectEmpty())
    {
      grPath.AddLine(startPnt, Point(dataR.right, pnt.y));
      grPath.StartFigure();
      startPnt.X = dataR.right;
    }
  }

  grPath.AddLine(startPnt, Point(clR.right, pnt.y));

  Graphics gr(pDC->m_hDC);
  Pen brdPen(m_pProperty->m_colZoomBorder);
  gr.DrawPath(&brdPen, &grPath);
  ReleaseDC(pDC);
}


void CChartContainer::DrawContainerToBmp(Rect rGdi, Bitmap& clBmp)
{
  Graphics* grPtr = Graphics::FromImage(&clBmp);  // As memDC
  grPtr->SetSmoothingMode(SmoothingModeAntiAlias);

  RectF rGdiF = GdiRectToRectF(rGdi);
  rGdiF.Width -= 1.0f;
  rGdiF.Height -= 1.0f;
  DrawBkground(rGdiF, m_pProperty->m_colBkground, m_pProperty->m_colBorder, grPtr);

  CChart* pChart = GetChart(0,false);
  CAxis* pAxis = pChart->GetAxis();
  if (m_mapCharts.size() > 0)
  {
    pAxis->RestoreModeX();  // Might be in the middle of settung zoon borders; annuls the mode
    RectF drawRF = rGdiF;   
    drawRF.Inflate(-DR_SPACE, -DR_SPACE);
 
    if (!HasChartWithData(-1))
      pAxis->DrawGrid(PointF(0.0f, 0.0f), grPtr);
    else
    {
// Draw axes
      PointF pntOrigF = pAxis->DrawAxes(pAxis->m_startX, pAxis->m_endX, pAxis->m_minExtY, pAxis->m_maxExtY, grPtr);
// Draw a grid    
      pAxis->DrawGrid(pntOrigF, grPtr);

// Update scales
      std::pair<double, double> pair_scales = 
      pAxis->UpdateScales(drawRF, pAxis->m_startX, pAxis->m_endX, pAxis->m_minExtY, pAxis->m_maxExtY);
      pAxis->m_scX = pair_scales.first;
      pAxis->m_scY = pair_scales.second;
// Set transform matrix
      MatrixD matrixD;
      matrixD.Translate(pntOrigF.X, pntOrigF.Y);
      matrixD.Scale(pAxis->m_scX, -pAxis->m_scY);
// Account for not even axes placement
      if ((pAxis->m_axXPos != BETWEEN)||(pAxis->m_axYPos != MIDDLE))
      {
        double translateX = (pAxis->m_axXPos == RIGHT) ? -pAxis->m_endX : (pAxis->m_axXPos == LEFT) ? -pAxis->m_startX : 0.0;
        double translateY = (pAxis->m_axYPos == BOTTOM) ? -pAxis->m_minExtY : (pAxis->m_axYPos == TOP) ? -pAxis->m_maxExtY : 0.0;
        matrixD.Translate(translateX, translateY);
      }
      if (pAxis->m_pMatrixD != NULL)
        delete pAxis->m_pMatrixD;
      pAxis->m_pMatrixD = matrixD.Clone();     // Copy constructor is private ??

// Begin draw curves
      GraphicsPath grPath;            // Will be used actoss the drawing routines
      MAP_CHARTS::iterator it = m_mapCharts.begin();
      MAP_CHARTS::iterator itE = m_mapCharts.end();
      for (; it != itE; ++it)
      {
        CChart* chartPtr = it->second;
        if (chartPtr->IsChartVisible() == false)
          continue;
        if (chartPtr->m_vDataPnts.size() == 0)
          continue;
        chartPtr->DrawChartCurve(chartPtr->m_vDataPnts,&grPath, grPtr);
        if (m_pProperty->m_bTracking)              // Draw selection points if any
          DrawSelPntsD(chartPtr, m_mapSelPntsD, &grPath, grPtr, drawRF);
      }

      if (m_pProperty->m_bTracking)                // And the sel line too
        DrawSelLine(rGdiF, grPtr);

      if (m_pProperty->m_bAxBoundaries)            // Left and right visible X-coordinates
        DrawAxXBoundaryStr(rGdiF, grPtr);

// Now draw children
      if (m_pLegWnd&&IsWindow(m_pLegWnd->m_hWnd)&&m_pLegWnd->IsWindowVisible())
      {
        CRect legWndRect = m_pLegWnd->GetDataWndRect();
        m_pLegWnd->DrawDataWnd(legWndRect.left, legWndRect.top, grPtr);
      }

      if (m_pDataWnd&&IsWindow(m_pDataWnd->m_hWnd)&&m_pDataWnd->IsWindowVisible())
      {
        CRect dataWndRect = m_pDataWnd->GetDataWndRect();
        m_pDataWnd->DrawDataWnd(dataWndRect.left, dataWndRect.top, grPtr);
      }
    }
  }
  delete grPtr;
}

// Printing
int CChartContainer::GetScreenDpi(void)
{
  CPaintDC containerDC(this);
  int scrDpiX = containerDC.GetDeviceCaps(LOGPIXELSX);
  int scrDpiY = containerDC.GetDeviceCaps(LOGPIXELSY);
  ENSURE(scrDpiX == scrDpiY);
  return scrDpiX;
}

TUPLE_PRNLEGLAYOUT CChartContainer::CalcNamesPntLayout(RectF namesRF, 
                         const MAP_PRNDATA& mapPrnData, Graphics* grPtr, float dpiRatio)
{
	USES_CONVERSION;
// Measure strings
  float maxNamesLength = 0, maxScYLength = 0, maxValStrLength = 0;
  float maxNameXStrLength = 0, maxStrXLength = 0; 
  float maxNameYStrLength = 0, maxStrYLength = 0;

  RectF tmpRF; 

// Get max size of the printing cells
  FontFamily fontFamily(L"Verdana");
  Gdiplus::Font labelFont(&fontFamily, 8.0f, FontStyleBold);
  grPtr->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

  // Measure strings
  MAP_PRNDATA::const_iterator it = mapPrnData.begin();
  MAP_PRNDATA::const_iterator itE = mapPrnData.end();

// Get max strings and str lengths
  while (it != itE)
  {
    TUPLE_PRINT tuple_chart = it->second;
// Names
    string_t tmpStr  = get<IDX_NAME>(tuple_chart);

    if (!tmpStr.empty())
    {
      grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
      if (tmpRF.Width > maxNamesLength )
        maxNamesLength = tmpRF.Width;
    }
// Scales
    tmpStr = get<IDX_SCY>(tuple_chart);
    if (!tmpStr.empty())
    {
      grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
      if (tmpRF.Width > maxScYLength)
        maxScYLength = tmpRF.Width;
    }
// Sel pnts if any
    tmpStr = get<IDX_VAL>(tuple_chart);
    if (!tmpStr.empty())  // If empty, no selected points
    {
      grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
      if (tmpRF.Width > maxValStrLength)
        maxValStrLength = tmpRF.Width;
    }

// X name
    tmpStr = get<IDX_NAMEX>(tuple_chart);
    if (!tmpStr.empty())
    {
      grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
      if (tmpRF.Width > maxNameXStrLength)
        maxNameXStrLength = tmpRF.Width;
    }

// X values
    tmpStr = get<IDX_X>(tuple_chart);
    grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
    if (tmpRF.Width > maxStrXLength)
      maxStrXLength = tmpRF.Width;

// Y name str
    tmpStr = get<IDX_NAMEY>(tuple_chart);
    if (!tmpStr.empty())
    {
      grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
      if (tmpRF.Width > maxNameYStrLength)
        maxNameYStrLength = tmpRF.Width;
    }

// Y values
    tmpStr = get<IDX_Y>(tuple_chart);
    grPtr->MeasureString(A2W(tmpStr.c_str()), -1, &labelFont, namesRF, &tmpRF);
    if (tmpRF.Width > maxStrYLength)
      maxStrYLength = tmpRF.Width;

    ++it;
  }

  // Column offsets, begin from the end
  float endX = namesRF.GetRight();
  float startX = namesRF.GetLeft();

  float valYOffsX = endX - maxStrYLength;
  float nameYStrOffsX = maxNameYStrLength == 0 ? valYOffsX : 
                                valYOffsX  - maxNameYStrLength - dpiRatio*LB_BORDERSPACE;   
  float valXOffsX = nameYStrOffsX - maxStrXLength - dpiRatio*LB_BORDERSPACE;
  float nameXStrOffsX = maxNameXStrLength == 0 ? valXOffsX :
                                valXOffsX - maxNameXStrLength - - dpiRatio*LB_BORDERSPACE;
  float valStrOffsX = maxValStrLength == 0 ? nameXStrOffsX :
                              nameXStrOffsX - maxValStrLength - - dpiRatio*LB_BORDERSPACE;
  float scYOffsX = maxScYLength == 0 ? valStrOffsX :
                                valStrOffsX - maxScYLength - dpiRatio*LB_BORDERSPACE;
  float nameStrOffsX = maxNamesLength == 0 ? scYOffsX :
                                  scYOffsX - maxNamesLength - dpiRatio*LB_BORDERSPACE;
  float bulletOffsX = maxNamesLength == 0 ? nameStrOffsX :
                          nameStrOffsX - dpiRatio*LB_BULLETSZ - dpiRatio*LB_BORDERSPACE;
// Adjust offsets:
  int stepsY = 0;
  if (bulletOffsX >= startX)
  {
    float offsShiftX = (bulletOffsX - startX)/2.0f;      // Actually startX + (bulletOffsX - startX)/2 
    bulletOffsX   -= offsShiftX;
    nameStrOffsX  -= offsShiftX;
    scYOffsX      -= offsShiftX;
    valStrOffsX   -= offsShiftX;
    nameXStrOffsX -= offsShiftX;
    valXOffsX     -= offsShiftX;
    nameYStrOffsX -= offsShiftX;
    valYOffsX     -= offsShiftX;
  }
  else
  {
    float offsShiftX = startX - bulletOffsX;
    bulletOffsX  += offsShiftX;
    nameStrOffsX += offsShiftX;
    scYOffsX     += offsShiftX;

    stepsY = STEPY_VALSTR;

    if (valStrOffsX < startX)                             // Three lines
    {
      float valWidth = max(maxStrXLength, maxStrYLength);  // Beautification
      float nmWidth  = max(maxNameXStrLength, maxNameYStrLength);
      valYOffsX      = endX - valWidth;
      nameYStrOffsX  = maxNameYStrLength == 0 ? valYOffsX :
                          valYOffsX - nmWidth - dpiRatio*LB_BORDERSPACE;
      valXOffsX      = valYOffsX;
      nameXStrOffsX  = maxNameXStrLength == 0 ? valXOffsX :
                      valXOffsX - nmWidth - dpiRatio*LB_BORDERSPACE;
      valStrOffsX    = maxValStrLength == 0 ?  nameXStrOffsX :
           min(nameXStrOffsX, nameYStrOffsX) - maxValStrLength - dpiRatio*LB_BORDERSPACE;
      stepsY |= STEPY_VALY;
    }
  }

  TUPLE_PRNLEGLAYOUT tuple_res;

  get<IDX_BULLETSTART>(tuple_res) = bulletOffsX, 
  get<IDX_NAME>(tuple_res)        = nameStrOffsX;
  get<IDX_SCY>(tuple_res)         = scYOffsX;
  get<IDX_VAL>(tuple_res)         = valStrOffsX;;
  get<IDX_NAMEX>(tuple_res)       = nameXStrOffsX;
  get<IDX_X>(tuple_res)           = valXOffsX;
  get<IDX_NAMEY>(tuple_res)       = nameYStrOffsX;
  get<IDX_Y>(tuple_res)           = valYOffsX;

  get<IDX_STEPSY>(tuple_res)      = stepsY;

  return tuple_res;
}

void CChartContainer::PrintCrossLine(PointF pntStartF, PointF pntEndF, SizeF sizeCrossF, 
                                               Pen* penPtr, Graphics* grPtr, float dpiRatio)
{
  PointF pntXF(dpiRatio*sizeCrossF.Width/2, 0.0f);
  PointF pntYF(0.0f, dpiRatio*sizeCrossF.Height/2);
  float space = dpiRatio*(sizeCrossF.Width + 1.0f);
// Lets calculate number of Crosses
  int CrossNmb = static_cast<int>(floor((pntEndF.X - pntStartF.X + dpiRatio*1.0f)/space));

  GraphicsPath grPath;
  pntStartF.X += pntXF.X;   // Move to the center of the first fugure

  for (int i = 0; i < CrossNmb; ++i)
  {
    grPath.AddLine(pntStartF - pntXF, pntStartF + pntXF);
    grPath.StartFigure();
    grPath.AddLine(pntStartF - pntYF, pntStartF + pntYF);
    grPath.StartFigure();

    pntStartF.X += space;
  }
  grPtr->DrawPath(penPtr, &grPath);  
}

void CChartContainer::GetSelValString(RectF drawRF, CChart* chartPtr, double pageScY, 
                                      MAP_PRNDATA& mapPrnData, float dpiRatio)
{
  string_t emptyStr;
  sstream_t stream_t;
  TUPLE_PRINT tuple_chart;

// Insert pen and brush attributes
  ChartProperty* pAttr = chartPtr->GetProperty();
  get<IDX_COL>(tuple_chart) = SetAlpha(pAttr->GetChartColor(), 0xFF);
  get<IDX_DSTYLE>(tuple_chart) = pAttr->GetChartDashStyle();
  get<IDX_PWIDTH>(tuple_chart)  = pAttr->GetPenWidth();

// Insert the chart name
  get<IDX_NAME>(tuple_chart) = pAttr->GetChartName();

// Insert the Y-scale in unit/inch
  double scaleY     = pageScY/chartPtr->GetLocScaleY();        //Dots per unit
  int chartIdx      = chartPtr->GetChartIdx();
  int precisionY    = pAttr->GetPrecisionY();
  double minY       = chartPtr->GetMinValY();
  double maxY       = chartPtr->GetMaxValY();
  val_label_str_fn valStrFnPtr = pAttr->GetLabYValStrFnPtr();
  drawRF.Inflate(DR_SPACE*dpiRatio, DR_SPACE*dpiRatio);

  stream_t << _T("ScaleY = ") << setprecision(precisionY + 1) << scaleY 
         << _T(" units/inch") << _T("\n");
  get<IDX_SCY>(tuple_chart) = stream_t.str();

// Insert selected points values
  MAP_SELPNTSD::const_iterator it = m_mapSelPntsD.find(chartIdx);
  if (it == m_mapSelPntsD.cend()) // It is names only
  {
    get<IDX_VAL>(tuple_chart)   = emptyStr;
    get<IDX_NAMEX>(tuple_chart) = string_t(_T("MinY"));
    get<IDX_X>(tuple_chart)     = valStrFnPtr(minY, precisionY, true);
    get<IDX_NAMEY>(tuple_chart) = string_t(_T("MaxY"));
    get<IDX_Y>(tuple_chart)     = valStrFnPtr(maxY, precisionY, true);
    mapPrnData.insert(MAP_PRNDATA::value_type(chartIdx, tuple_chart));
  }
  else
  { 
    size_t count = m_mapSelPntsD.count(chartIdx);
    for (size_t cnt = 0; cnt < count; ++cnt)
    {
      if (cnt != 0)
      {
        get<IDX_NAME>(tuple_chart) = emptyStr;
        get<IDX_SCY>(tuple_chart)  = emptyStr;
      }

      get<IDX_VAL>(tuple_chart) = string_t(_T("Sel. Point: "));

      PointD selPntD = it->second;        // Value X, Y
      PointF selPntF = chartPtr->GetAxis()->m_pMatrixD->TransformToPntF(chartPtr->GetLocScaleY(), selPntD);
      if (drawRF.Contains(selPntF))
      {
        get<IDX_NAMEX>(tuple_chart) = m_labelX;
        bool bEqSign = m_labelX.empty() ? false : true;
        get<IDX_X>(tuple_chart) = valStrFnPtr(selPntD.X, m_pProperty->m_precision, bEqSign);
        string_t labelY = pAttr->GetAxisYName();
        get<IDX_NAMEY>(tuple_chart) = labelY;
        bEqSign = labelY.empty() ? false : true;
        get<IDX_Y>(tuple_chart) = valStrFnPtr(selPntD.Y, precisionY, bEqSign);
      }
      else
      {
        get<IDX_VAL>(tuple_chart)   = string_t(_T("Sel. N/A, "));
        get<IDX_NAMEX>(tuple_chart) = string_t(_T("MinY"));
        get<IDX_X>(tuple_chart)     = valStrFnPtr(minY, precisionY, true);
        get<IDX_NAMEY>(tuple_chart) = string_t(_T("MaxY"));
        get<IDX_Y>(tuple_chart)     = valStrFnPtr(maxY, precisionY, true);
     }

      mapPrnData.insert(MAP_PRNDATA::value_type(chartIdx, tuple_chart));
      ++it;
    }
  }
}

size_t CChartContainer::PrintChartNames(RectF namesRF, const MAP_PRNDATA& mapPrnData, 
           const TUPLE_PRNLEGLAYOUT& tuple_prnLayout, Graphics* grPtr, float dpiRatio, size_t mapOffs)
{
  USES_CONVERSION;
// Set the font
  FontFamily fontFamily(L"Verdana");
  Gdiplus::Font labelFont(&fontFamily, 8.0f, FontStyleBold);
  grPtr->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
  float fontHeight = labelFont.GetHeight(grPtr);
  float lineHeight = 1.5f*fontHeight; // In doc. units, 1/300 of inch

// Get offsets
  float lineY    = namesRF.Y;

// Get string layout; do not care whether there are val, nMEx, and nameY  strings
  float bulletStartOffsX = get<IDX_BULLETSTART>(tuple_prnLayout);
  float bulletEndX       = bulletStartOffsX + dpiRatio*LB_BULLETSZ;
  float nmStrOffsX       = get<IDX_NAME>(tuple_prnLayout);;
  float scYStrOffsX      = get<IDX_SCY>(tuple_prnLayout);
  float valStrOffsX      = get<IDX_VAL>(tuple_prnLayout);     
  float nmXOffsX         = get<IDX_NAMEX>(tuple_prnLayout);
  float valXOffsX        = get<IDX_X>(tuple_prnLayout);
  float nmYOffsX         = get<IDX_NAMEY>(tuple_prnLayout);
  float valYOffsX        = get<IDX_Y>(tuple_prnLayout);

  int stepsY             = get<IDX_STEPSY>(tuple_prnLayout);

  Pen bulletPen((ARGB)Color::Black);      // Pen Width is to set later
  SolidBrush txtBr((ARGB)Color::Black);

  MAP_PRNDATA::const_iterator it = mapPrnData.begin();
  MAP_PRNDATA::const_iterator itE = mapPrnData.end();

  advance(it, mapOffs);
  bool bName = mapOffs == 0 ? false : true;   // To begin the new page with full line
  for (; it != itE; ++it)
  {
    TUPLE_PRINT tuple_chart = it->second;     // Get strings prepared
// Set pen and brush attributes
    Color strColor = get<IDX_COL>(tuple_chart);
    bulletPen.SetColor(strColor);
    DashStyle dashStyle = get<IDX_DSTYLE>(tuple_chart);
    bulletPen.SetDashStyle(dashStyle != DashStyleCustom ? dashStyle : DashStyleSolid);
    bulletPen.SetWidth(dashStyle != DashStyleCustom ? 
                                      get<IDX_PWIDTH>(tuple_chart)*dpiRatio : dpiRatio);
    txtBr.SetColor(strColor);

    PointF pntStartF, pntEndF;

    string_t tmpStr = get<IDX_NAME>(tuple_chart); 
    if (!tmpStr.empty())            
    {
      bName = false;
      pntStartF = PointF(bulletStartOffsX, lineY + fontHeight/2.0f);
      pntEndF   = PointF(bulletEndX, lineY + fontHeight/2.0f);

      if (dashStyle != DashStyleCustom)
      {
        grPtr->DrawLine(&bulletPen, pntStartF, pntEndF);
      }
      else
      {
        PrintCrossLine(pntStartF, pntEndF, SizeF(CHART_DTPNTSZ, CHART_DTPNTSZ),  
                                                      &bulletPen, grPtr, dpiRatio);
      }
// Draw name string
      grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(nmStrOffsX, lineY), &txtBr);
// Draw scY string 
      tmpStr = get<IDX_SCY>(tuple_chart);
      grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(scYStrOffsX, lineY), &txtBr);
    }
    else if (bName)
    {
      MAP_PRNDATA::const_reverse_iterator rIt = MAP_PRNDATA::const_reverse_iterator(it);
      for (; rIt != mapPrnData.crend(); ++rIt)
      {
        TUPLE_PRINT tuple_nm = rIt->second;
        tmpStr = get<IDX_NAME>(tuple_nm);
        if (tmpStr.empty())
          continue;
        if (dashStyle != DashStyleCustom)
        {
          grPtr->DrawLine(&bulletPen, pntStartF, pntEndF);
        }
        else
        {
           PrintCrossLine(pntStartF, pntEndF, SizeF(CHART_DTPNTSZ, CHART_DTPNTSZ),  
                                                      &bulletPen, grPtr, dpiRatio);
        }
        grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(nmStrOffsX, lineY), &txtBr);
        tmpStr = get<IDX_SCY>(tuple_nm);
        grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(scYStrOffsX, lineY), &txtBr);
        bName = false;
        break;
      }
    }

    if (stepsY & STEPY_VALSTR)
      lineY += lineHeight;
    
    tmpStr = get<IDX_VAL>(tuple_chart);
    if (!tmpStr.empty())
      grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(valStrOffsX, lineY), &txtBr);
    tmpStr = get<IDX_NAMEX>(tuple_chart);
    if (!tmpStr.empty())
      grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(nmXOffsX, lineY), &txtBr);
    grPtr->DrawString(A2W(get<IDX_X>(tuple_chart).c_str()), -1, &labelFont, 
                                                          PointF(valXOffsX, lineY), &txtBr);
    if (stepsY & STEPY_VALY)
      lineY += lineHeight;

    tmpStr = get<IDX_NAMEY>(tuple_chart);
    if (!tmpStr.empty())
      grPtr->DrawString(A2W(tmpStr.c_str()), -1, &labelFont, PointF(nmYOffsX, lineY), &txtBr);
    grPtr->DrawString(A2W(get<IDX_Y>(tuple_chart).c_str()), -1, &labelFont, 
                                                              PointF(valYOffsX, lineY), &txtBr);

    ++mapOffs;
    lineY += lineHeight;

    if ((lineY + 3*lineHeight) > namesRF.GetBottom())
      return mapOffs;

  }
  return mapOffs;
}

string_t CChartContainer::PrintPageHeader(RectF rGdiF, const CChartContainer* pContainer, 
                                                                         string_t tmStr, Graphics* grPtr)
{
 USES_CONVERSION;
 // Form bounding rectangles
  RectF headerRF = rGdiF;
  headerRF.Height = 80.0f; // Approx. 7 mm 
  headerRF.Inflate(-200.0f, 0.0f);
  headerRF.Offset(0.0f, 250.0f);

// Draw header
  FontFamily fontFamily(L"Verdana");
  Gdiplus::Font headerFont(&fontFamily, 16.0f, FontStyleRegular, UnitPoint);
  grPtr->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

  SolidBrush brHeader((ARGB)Color::Black);
  StringFormat strFormat;
  strFormat.SetAlignment(StringAlignmentNear);
  strFormat.SetLineAlignment(StringAlignmentCenter);

  grPtr->DrawString(A2W(pContainer->m_name.c_str()),-1, &headerFont,  headerRF, &strFormat, &brHeader);

  if (tmStr.empty())
  {
    // Print current local time
    CTime tmD = CTime::GetCurrentTime();
    CString strTmD = tmD.Format(_T("%a, %b %d, %Y, %H:%M"));
    tmStr = string_t(strTmD);
  }

// Draw time string
  strFormat.SetAlignment(StringAlignmentFar);
  grPtr->DrawString(A2W(tmStr.c_str()), -1, &headerFont,  headerRF, &strFormat, &brHeader);

  return tmStr;
}

void CChartContainer::PrintCharts(CChartContainer* pContainer, int scrDpiX, HDC printDC)
{
  if (printDC != NULL)
  {
    pContainer->m_pProperty->m_bAxBoundaries = true;

    CDC* pDC = new CDC;
    pDC->Attach(printDC); 
    Graphics* grPtr = new Graphics(printDC);

    grPtr->SetPageUnit(UnitDocument);
    grPtr->SetSmoothingMode(SmoothingModeAntiAlias);
// Get dpiRatio
    float dpiRatioX = 300.0f/scrDpiX;

    RectF rGdiF;
    grPtr->GetVisibleClipBounds(&rGdiF);                 // The same as the clip rect

// Begin Printing
    pDC->StartDoc(pContainer->m_name.c_str());          // MFC functions
    pDC->StartPage();

// Draw header
    string_t tmStr;
    tmStr = pContainer->PrintPageHeader(rGdiF, pContainer, tmStr, grPtr);

// Format drawing area and draw a border
    RectF drawRF = rGdiF;   
    drawRF.Inflate(-200.0f, -200.0f);
    drawRF.Height = rGdiF.Width*0.75f;
    drawRF.Offset(0.0f, 200.0f);

    SolidBrush bkBr(pContainer->m_pProperty->m_colBkground);
    grPtr->FillRectangle(&bkBr, drawRF);

    Pen pen((ARGB)Color::Black, 3*dpiRatioX);
    pen.SetAlignment(PenAlignmentInset);
    grPtr->DrawRectangle(&pen, drawRF);
// Set clipping region    
    grPtr->SetClip(drawRF);

	CChart* pChart = pContainer->GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();

// Draw charts
    if (pContainer->m_mapCharts.size() > 0)
    {
// Draw axes
      PointF pntOrigF = pAxis->DrawAxes(pAxis->m_startX, pAxis->m_endX, 
                        pAxis->m_minExtY, pAxis->m_maxExtY, grPtr);
// Draw Grid
	  GraphicsPath grPath;            // Will be used actoss the drawing routines
      pAxis->DrawGrid(pntOrigF, grPtr, dpiRatioX);
// Set margins for drawing
      drawRF.Inflate(-DR_SPACE*dpiRatioX, -DR_SPACE*dpiRatioX);
// Update scales
      std::pair <double, double> pair_scales = 
      pAxis->UpdateScales(drawRF, pAxis->m_startX,pAxis->m_endX, pAxis->m_minExtY, pAxis->m_maxExtY);
      pAxis->m_scX = pair_scales.first;
      pAxis->m_scY = pair_scales.second;
// Set transform matrix
      MatrixD matrixD;
      matrixD.Translate(pntOrigF.X, pntOrigF.Y); // Add drawRF
      matrixD.Scale(float(pAxis->m_scX), float(-pAxis->m_scY));
      if ((pAxis->m_axXPos != BETWEEN)||(pAxis->m_axYPos != MIDDLE))
      {
        double translateX = (pAxis->m_axXPos == RIGHT) ? -pAxis->m_endX : 
                        (pAxis->m_axXPos == LEFT) ? -pAxis->m_startX : 0.0;
        double translateY = (pAxis->m_axYPos == BOTTOM) ? -pAxis->m_minExtY : 
                        (pAxis->m_axYPos == TOP) ? -pAxis->m_maxExtY : 0.0;
        matrixD.Translate(float(translateX), float(translateY)); 
      }
      if (pAxis->m_pMatrixD != NULL)
        delete pAxis->m_pMatrixD;
      pAxis->m_pMatrixD = matrixD.Clone();     // Can't copy: copy constructor is private

// Calculate page scale Y to display on the legend
      ENSURE (grPtr->GetPageUnit() == UnitDocument); // Sure it is: set it at the start of printing
      PointD pntStartD, pntEndD;
      CPoint pntStart(int(drawRF.X), int(drawRF.Y)), pntEnd(int(drawRF.GetRight()), int(drawRF.GetBottom()));
      pContainer->MousePntToPntD(pntStart, pntStartD, pAxis->m_pMatrixD);
      pContainer->MousePntToPntD(pntEnd, pntEndD, pAxis->m_pMatrixD);
      double pageScY = 300.0*(pntStartD.Y - pntEndD.Y)/drawRF.Height;  // 300 dpi/inch for the Doc. Mode
  
// Begin draw all visible curves or selected only
      int chartSelIdx = pContainer->HasSelectedChart();      
      MAP_LABSTR mapLabStr;
      size_t mapOffs = 0;
 
      MAP_CHARTS::iterator it;
      if (chartSelIdx == -1)
        it = pContainer->m_mapCharts.begin();
      else 
       it = pContainer->m_mapCharts.find(chartSelIdx);
      MAP_CHARTS::iterator itE = pContainer->m_mapCharts.end();

      MAP_PRNDATA mapPrnData;

      for (; it != itE; ++it)
      {
        CChart* chartPtr = it->second;
        if (chartPtr->IsChartVisible() == false)
          continue;
        if (chartPtr->m_vDataPnts.size() == 0)
          continue;
        chartPtr->DrawChartCurve(chartPtr->m_vDataPnts, &grPath, grPtr, dpiRatioX);
        if (pContainer->m_pProperty->m_bTracking)
          pContainer->DrawSelPntsD(chartPtr, pContainer->m_mapSelPntsD, &grPath, grPtr, drawRF, dpiRatioX);

        pContainer->GetSelValString(drawRF, chartPtr, pageScY, mapPrnData, dpiRatioX);

        if (chartSelIdx > -1)
          break;
      }

      if (pContainer->m_pProperty->m_bTracking)
        pContainer->DrawSelLine(drawRF, grPtr, chartSelIdx, dpiRatioX);

      if (pContainer->m_pProperty->m_bAxBoundaries)
        pContainer->DrawAxXBoundaryStr(drawRF, grPtr);
  // Now print the legend: chart names, etc.
      grPtr->ResetClip();
      RectF namesRF(drawRF.X, drawRF.GetBottom() + 100.0f, drawRF.Width, 0.0f);
      namesRF.Height = rGdiF.GetBottom() - namesRF.Y; // temporary; will be set in PrintChartNames
      TUPLE_PRNLEGLAYOUT tuple_prnLayout = 
        pContainer->CalcNamesPntLayout(namesRF, mapPrnData, grPtr, dpiRatioX);
      mapOffs = pContainer->PrintChartNames(namesRF, mapPrnData, tuple_prnLayout, grPtr, dpiRatioX);
      if (mapOffs < mapPrnData.size())        // Print the rest of the legend on next pages
      {
// Get new namesRF
        namesRF = rGdiF;   
        namesRF.Inflate(-200.0f, -200.0f);
        namesRF.Height = rGdiF.Height - 300.0f;
        namesRF.Offset(0.0f, 300.0f);
// Print extra pages
        while (mapOffs < mapPrnData.size())
        {
          pDC->EndPage();
          pDC->StartPage();
          pContainer->PrintPageHeader(rGdiF, pContainer, tmStr, grPtr);
          mapOffs = pContainer->PrintChartNames(namesRF, mapPrnData, tuple_prnLayout, grPtr, 
                                                                dpiRatioX, mapOffs);
        }
      }
    }
// End printing
    pDC->EndPage();
    pDC->EndDoc();

    delete grPtr;
    pDC->Detach();
    delete pDC;
  }
}


BEGIN_MESSAGE_MAP(CChartContainer, CWnd)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_GETDLGCODE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_LBUTTONUP()
  ON_WM_SETCURSOR()
  ON_WM_MBUTTONDOWN()
  ON_WM_MOUSEWHEEL()
  ON_WM_CONTEXTMENU()
  ON_COMMAND(IDM_CHART_LEGEND, OnMnShowLegend)
  ON_COMMAND(IDM_CHART_BOUNDARIES, OnMnShowAxBoundaries)
  ON_COMMAND(IDM_CHART_REFRESH, OnMnRefresh)
  ON_COMMAND(IDM_CHART_VIEWDATA, OnMnViewData)
  ON_COMMAND(IDM_CHART_SAVE, OnMnSaveCharts)
  ON_COMMAND(IDM_CHART_SAVEIMAGE, OnMnSaveImage)
  ON_COMMAND(IDM_CHART_PRINT, OnMnPrintCharts)

  ON_COMMAND(IDM_CHART_ZOOMINX, OnMnZoomInX) 
  ON_COMMAND(IDM_CHART_MOVERIGHT, OnMnMoveRight)
  ON_COMMAND(IDM_CHART_MOVELEFT, OnMnMoveLeft)
  ON_COMMAND(IDM_CHART_ZOOMINY, OnMnZoomInY)
  ON_COMMAND(IDM_CHART_MOVEUP, OnMnMoveUp)
  ON_COMMAND(IDM_CHART_MOVEDOWN, OnMnMoveDown)
  ON_COMMAND(IDM_CHART_TRACK, OnMnEnableTracking)
  ON_COMMAND(IDM_CHART_SHOWPNTS, OnMnShowPnts)
  ON_COMMAND_RANGE(IDM_CHART_FIRSTCHART, IDM_CHART_LASTCHART, OnMnShowChart)
#ifdef _TEST_
  ON_COMMAND(IDM_CHART_TEST, OnMnTest)
#endif
  ON_WM_KEYDOWN()
  ON_WM_KEYUP()
  ON_WM_TIMER()
  ON_WM_KILLFOCUS()
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CChartContainer message handlers
void CChartContainer::OnPaint()
{
  CPaintDC dc(this);                              // Device context for painting
  Graphics gr(dc.m_hDC);                          // Graphics to paint

  Rect rGdi;
  gr.GetVisibleClipBounds(&rGdi);                 // The same as the clip rect

  Bitmap clBmp(rGdi.Width, rGdi.Height);          // Mem bitmap
  Graphics* grPtr = Graphics::FromImage(&clBmp);  // As memDC
  grPtr->SetSmoothingMode(SmoothingModeAntiAlias);

  RectF rGdiF = GdiRectToRectF(rGdi);
  rGdiF.Width -= 1.0f;
  rGdiF.Height -= 1.0f;
  DrawBkground(rGdiF, m_pProperty->m_colBkground, m_pProperty->m_colBorder, grPtr);

  DrawDataCurve(rGdiF, grPtr);

 // Transfer to the screen
  delete grPtr;
  gr.DrawImage(&clBmp, rGdi);
}

void CChartContainer::OnSize(UINT nType, int cx, int cy)
{
  UNREFERENCED_PARAMETER(cx);
  UNREFERENCED_PARAMETER(cy);

  CWnd* pParent = GetParent();
  if ((pParent == NULL)&&((nType == SIZE_RESTORED)||(nType == SIZE_MAXIMIZED)))
  {
    bool bRes = HideLabWnds();
    if (bRes)
      SetTimer(2, 50, NULL);
  }
}

UINT CChartContainer::OnGetDlgCode()
{
  UINT nCode = CWnd::OnGetDlgCode();
  nCode |= DLGC_WANTARROWS;
  return nCode;
}

void CChartContainer::OnLButtonDown(UINT nFlags, CPoint point)
{
	CChart* pChart = GetChart(0,false);
	if (!pChart) return;
	CAxis* pAxis = pChart->GetAxis();
	if(!pAxis) return;
  if ((m_pProperty->m_bUserEnabled)&&HasChartWithData(-1, true))
  {
    SetFocus();
    if (nFlags & MK_SHIFT)
    {
      if (pAxis->m_chModeY == MODE_ZOOMINGY)
        PrepareZoomY(point, m_pProperty->m_tmpZY, true);     // Zoom Y
      else if ((pAxis->m_chModeX != MODE_ZOOMINGX) && (pAxis->m_chModeX != MODE_ZOOMX))
        m_pProperty->m_tmpZX = BeginZoomingX(point, true);    // Sets m_pAxisManager->m_startX, m_chMode = MODE_ZOOMING
      else if (pAxis->m_chModeX == MODE_ZOOMINGX)
        PrepareZoomX(point, m_pProperty->m_tmpZX, true);     // Sets m_pAxisManager->m_startX, m_pAxisManager->m_endX, m_chMode = MODE_ZOOM
    }
    else if (nFlags & MK_CONTROL)
    {
      m_chartSelIdx = SelectChartByMouseClick(point);
      UpdateContainerWnds();
    }
    else if (m_pProperty->m_bTracking)
      ShowDataLegend(point);
  }

  CWnd::OnLButtonDown(nFlags, point);
}

void CChartContainer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  if ((m_pProperty->m_bUserEnabled)&&HasChartWithData(-1, true))
  {
    if ((nFlags & MK_SHIFT)&&(pAxis->m_chModeY != MODE_ZOOMINGY) && (pAxis->m_chModeY != MODE_ZOOMY))
    {
      m_pProperty->m_tmpZY = BeginZoomingY(point, true);    // Sets m_pAxisManager->m_startX, m_chMode = MODE_ZOOMING
    }
  }

  CWnd::OnLButtonDblClk(nFlags, point);
}

void CChartContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
	CChart* pChart = GetChart(0,false);
	if(!pChart) return;
	CAxis* pAxis = pChart->GetAxis();
	if(!pAxis) return;
  if (m_pProperty->m_bUserEnabled&&HasChartWithData(-1, true))
  {
    if ((pAxis->m_chModeX == MODE_ZOOMX)||(pAxis->m_chModeY == MODE_ZOOMY))
    {
      UpdateContainerWnds(-1, true);
 
      pAxis->m_chModeX =  MODE_FULLX;
      pAxis->m_chModeY =  MODE_FULLY;
    }
  }

  CWnd::OnLButtonUp(nFlags, point);
}

void CChartContainer::OnMButtonDown(UINT nFlags, CPoint point)
{
  if (m_pProperty->m_bUserEnabled)   // Responsibility of the programmer to set visiuals and cursor on m_bUserEnabled
  {
    SetFocus();
    bool bHasVisibleChart = HasChartWithData(-1, true);
    bool bOldTrack = m_pProperty->m_bTracking;
    m_pProperty->m_bTracking = bHasVisibleChart ? !m_pProperty->m_bTracking : false;
    if ((bOldTrack != m_pProperty->m_bTracking) && !m_pProperty->m_bTracking)
    {
      m_pDataWnd = DestroyLabWnd(m_pDataWnd);
      m_dataLegPntD = PointD(DBL_MAX, DBL_MAX);
    }
  }
 
  CWnd::OnMButtonDown(nFlags, point);
}

BOOL CChartContainer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  if (m_pProperty->m_bUserEnabled&&HasChartWithData(-1, true))
  {
    if (nFlags & MK_SHIFT)            // Move 
    {
      double deltaX = 0.0;
      if (zDelta < 0)
        deltaX = MOVE_DELTAX*(pAxis->m_endX - pAxis->m_startX);
      else if (zDelta > 0)
        deltaX = -MOVE_DELTAX*(pAxis->m_endX - pAxis->m_startX);
      else
        return CWnd::OnMouseWheel(nFlags, zDelta, pt);

      pChart->MoveContainerChartsX(deltaX, true);
    }
    else if ((nFlags & MK_CONTROL) &&  (m_chartSelIdx != -1))
    {
      MAP_CHARTS::iterator it = m_mapCharts.find(m_chartSelIdx);
      if (it != m_mapCharts.end())
      {
        CChart* chartPtr = it->second;
        if (chartPtr->IsChartVisible() && chartPtr->IsChartSelected())
        {
          std::pair<double, double> pair_scY = GetLocScYDelta(chartPtr);
          double locScY = pair_scY.first;
          double deltaK = pair_scY.second;

          if (zDelta < 0)
            locScY = locScY = max(locScY - deltaK, DELTA_LOCY);
          else
            locScY += deltaK;

          if (IsLabWndExist(false))
          {
            if (m_pDataWnd->IsWindowVisible())
              m_pDataWnd->ShowWindow(SW_HIDE);
            SetTimer(1, 500, NULL);
          }

          chartPtr->SetLocScaleY(locScY);  
          RefreshWnd();
        }
      }
    }
  }
  return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChartContainer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CChart* pChart = GetChart(0,false);
	CAxis* pAxis = pChart->GetAxis();
  if (m_pProperty->m_bUserEnabled&&HasChartWithData(-1, true))
  {
    if (nChar == VK_DELETE)
    {
      if (IsWindow(m_hWnd)&&IsWindowVisible() &&((pAxis->m_chModeX == MODE_ZOOMINGX)||(pAxis->m_chModeY == MODE_ZOOMINGY)))
        RefreshWnd();
    }
    else if ((nChar == VK_LEFT)||(nChar == VK_RIGHT))    // Move
      MoveChartsByArrowKeysX(nChar);
    else if ((nChar == VK_DOWN)||(nChar == VK_UP)||(nChar == VK_PRIOR)||(nChar == VK_NEXT))
    {
      if (m_chartSelIdx != -1)
        ChangeLocScaleYByArrowKeys(nChar);
      else
        MoveChartsByArrowKeysY(nChar);
    }
  }

  CWnd::OnKeyDown(nChar, nRepCnt, nFlags); 
}

BOOL CChartContainer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  if (nHitTest == HTCLIENT)
  {
    HCURSOR hCursor = NULL;
    if (m_pProperty->m_bTracking)
      hCursor = LoadCursor(NULL, IDC_CROSS);
    else  
      hCursor = LoadCursor(NULL, IDC_ARROW);
    if (hCursor != NULL)
    {
      SetCursor(hCursor);
    }
    return TRUE;

  }
  return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CChartContainer::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent == 1)        // Append
  {
    KillTimer(nIDEvent);

    if (m_pProperty->m_bTracking && IsLabWndExist(false))
      UpdateDataLegend(true); 
  }
  else if (nIDEvent == 2)   // LocScY or window size was changed or size 
  {
    KillTimer(nIDEvent);
    UpdateContainerWnds();
  }
}

void CChartContainer::OnKillFocus(CWnd* pNewWnd)
{
  CWnd::OnKillFocus(pNewWnd);
  RefreshWnd();
}

void CChartContainer::OnSetFocus(CWnd* pOldWnd)
{
  CWnd::OnSetFocus(pOldWnd);
  RefreshWnd();
}

// Context menu functions
void CChartContainer::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
  if (!m_pProperty->m_bUserEnabled)
    return;
  if (m_mapCharts.empty())
    return;  

  bool bHasChartWithData = HasChartWithData(-1, true);

  CMenu popMenu;
  popMenu.CreatePopupMenu();   
  //if (bHasChartWithData)
  //{
  //  UINT chEnabledLeg = IsLabWndVisible(true) ? MF_CHECKED : MF_UNCHECKED;
  //  popMenu.AppendMenu(MF_ENABLED|chEnabledLeg, IDM_CHART_LEGEND, _T("Show Legend"));
  //  popMenu.AppendMenu(MF_SEPARATOR);

  //  UINT chShowBounds = m_pProperty->m_bAxBoundaries ? MF_CHECKED : MF_UNCHECKED;
  //  popMenu.AppendMenu(MF_ENABLED|chShowBounds, IDM_CHART_BOUNDARIES, _T("Show Axis X boundaries"));
  //  popMenu.AppendMenu(MF_SEPARATOR);

  //  popMenu.AppendMenu(MF_ENABLED, IDM_CHART_VIEWDATA, _T("View Chart Data"));
  //}

  //CMenu popSubMenu1;
  //popSubMenu1.CreatePopupMenu();

//  popSubMenu1.AppendMenu(MF_ENABLED, IDM_CHART_SAVE, "Save chart(s)");
//  if(bHasChartWithData)
//  {
//    popSubMenu1.AppendMenu(MF_ENABLED, IDM_CHART_SAVEIMAGE, "Save Charts As Image");
//    popMenu.AppendMenu(MF_SEPARATOR);
//    popSubMenu1.AppendMenu(MF_ENABLED, IDM_CHART_PRINT, "Print charts");
//  }
//
//  string_t subMnStr(_T("Save"));
//  if (bHasChartWithData)
//    subMnStr += string_t(_T("/Print"));
//  subMnStr += string_t(_T(" Charts"));
//
//  popMenu.AppendMenu(MF_STRING|MF_POPUP, UINT(popSubMenu1.m_hMenu), subMnStr.c_str());
//  popMenu.AppendMenu(MF_SEPARATOR);
//
//  if (bHasChartWithData)
//  {
//    CMenu popSubMenu2;
//    popSubMenu2.CreatePopupMenu();
//
//    popSubMenu2.AppendMenu(MF_ENABLED, IDM_CHART_ZOOMINX, _T("Zoom In X "));
//    popSubMenu2.AppendMenu(MF_ENABLED, IDM_CHART_MOVERIGHT, _T("Move Right"));
//    popSubMenu2.AppendMenu(MF_ENABLED, IDM_CHART_MOVELEFT, _T("Move Left"));
//    popSubMenu2.AppendMenu(MF_SEPARATOR);
//    popSubMenu2.AppendMenu(MF_ENABLED, IDM_CHART_ZOOMINY, _T("Zoom In Y"));
//    popSubMenu2.AppendMenu(MF_ENABLED, IDM_CHART_MOVEUP, _T("Move Up"));
//    popSubMenu2.AppendMenu(MF_ENABLED, IDM_CHART_MOVEDOWN, _T("Move Down"));
//
//    popMenu.AppendMenu(MF_STRING|MF_POPUP, UINT(popSubMenu2.m_hMenu), "Zoom/Move");
//    popMenu.AppendMenu(MF_SEPARATOR);
//
//// If the mouse has no middle button replace middle button clicks
//    if (GetSystemMetrics(SM_CMOUSEBUTTONS) < 3)
//    {
//      UINT chTracking = m_pProperty->m_bTracking ? MF_CHECKED : MF_UNCHECKED;
//      popMenu.AppendMenu(MF_ENABLED|chTracking, IDM_CHART_TRACK, _T("Enable Tracking"));
//
//    }
//
//    UINT chShowCharts = MF_CHECKED;         // By default points are shown
//    if (m_chartSelIdx != -1)
//    {
//      CChart* selChartPtr = GetChart(m_chartSelIdx);
//	  ChartProperty* pAttr = selChartPtr->GetProperty();
//      if (!pAttr->ArePntsAllowed())
//        chShowCharts = MF_UNCHECKED;
//    }
//
//    if(HasChartDifferentPnts(-1) > -1)
//    {
//      popMenu.AppendMenu(MF_ENABLED|chShowCharts, IDM_CHART_SHOWPNTS, _T("Show/Hide Chart Pnts"));
//      popMenu.AppendMenu(MF_SEPARATOR);
//    }
//  }
// Add CChart names (m_mapCharts is not empty)
  string_t mnPrefix1(_T("Show "));
  string_t mnSuffix2(_T(" has no data"));
  string_t mnItemName;
  UINT chkFlag = MF_CHECKED;
  UINT cmdID = IDM_CHART_FIRSTCHART;
  MAP_CHARTS::iterator it = m_mapCharts.begin();
  MAP_CHARTS::iterator itE = m_mapCharts.end();
  for (; it != itE; ++it)
  {
	  CChart* chartPtr = it->second;
	  ChartProperty* pAttr = chartPtr->GetProperty();
	bool bAllowedToBeDrawn = it->second->HasData();
	UINT enableFlag = bAllowedToBeDrawn ? MF_ENABLED : MF_DISABLED;
	chkFlag = (chartPtr->IsChartVisible()) ? MF_CHECKED : MF_UNCHECKED;
	if (bAllowedToBeDrawn)
		mnItemName = mnPrefix1 + pAttr->GetChartName();
	else
		mnItemName = pAttr->GetChartName() + mnSuffix2;
	popMenu.AppendMenu(enableFlag|chkFlag, cmdID + it->first, mnItemName.c_str());
  }
#ifdef _TEST_
  popMenu.AppendMenu(MF_SEPARATOR);
  popMenu.AppendMenu(MF_ENABLED, IDM_CHART_TEST, _T("Test"));
#endif
  popMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
  popMenu.Detach();
}

void CChartContainer::OnMnShowLegend(void)
{
  if (IsLabWndVisible(true)) 
    m_pLegWnd = DestroyLabWnd(m_pLegWnd);
  else
    ShowNamesLegend();
}

void CChartContainer::OnMnShowAxBoundaries(void)
{
  m_pProperty->m_bAxBoundaries = !m_pProperty->m_bAxBoundaries;
  RefreshWnd();
}

void CChartContainer::OnMnRefresh(void)
{
  ResetCharts();
  CChart* pChart = GetChart(0,false);
  pChart->SendNotification(CODE_REFRESH,0);
}

void CChartContainer::OnMnViewData(void)
{
  if (m_mapCharts.empty())
  {
    AfxMessageBox(_T("Chart Container Is Empty"));
    return;
  }

// Send chart to the data view dlg
  int chartSelIdx = HasSelectedChart();

  if (chartSelIdx == -1)
    chartSelIdx = SelectChart(-1);

  if (chartSelIdx == -1)
  {
    AfxMessageBox(_T("View Chart Data: Has no visible or selected chart"));
    return;
  }
 
  MAP_CHARTS::iterator it = m_mapCharts.find(chartSelIdx);

  if (it == m_mapCharts.end())
    AfxMessageBox(_T("View Chart Data: Can't find selected chart"));

  ShowDataView(it->second, true);
}

void CChartContainer::OnMnSaveCharts(void)
{
	CChart* pChart = GetChart(0,false);
  if (S_FALSE == pChart->SaveChartData(string_t(_T(""))))  
  {
    AfxMessageBox(_T("Save Failed\n Select the visible chart to save it\n")
                  _T("or remove the selection to save all visible charts\n")
                  _T("and try again"), MB_OK|MB_ICONERROR);
  }
}

void CChartContainer::OnMnSaveImage(void)
{
	CChart* pChart = GetChart(0,false);
  Status stat = pChart->SaveContainerImage();

  if (stat != Ok)
  {
    string_t errStr(_T("Save Image Failed:\n"));
    switch (stat)
    {
    case InvalidParameter:   errStr += string_t(_T("Cannot get file path name")); break;
    case UnknownImageFormat: errStr += string_t(_T("Invalid Image Format"));      break;
    default:                 errStr += string_t(_T("Unknown Error"));             break;
    }
    AfxMessageBox(errStr.c_str(), MB_OK|MB_ICONERROR);
  }
}

void CChartContainer::OnMnPrintCharts(void)
{
  if (!HasChartWithData(-1, true))
  {
    AfxMessageBox(_T("Has no visible charts to print"), MB_OK|MB_ICONERROR);
    return;
  }

  CPrintDialog printDlg(FALSE, PD_USEDEVMODECOPIES|PD_HIDEPRINTTOFILE|PD_NOSELECTION|PD_RETURNDC);
  printDlg.m_pd.Flags &= ~PD_SHOWHELP;
  printDlg.m_pd.nMinPage = printDlg.m_pd.nMaxPage = 1;
  printDlg.m_pd.nFromPage = printDlg.m_pd.nToPage = 1;

  CChart* pChart = GetChart(m_chartSelIdx);
  if (printDlg.DoModal() == IDOK)
  {
    int scrDpiX = GetScreenDpi();
    pChart->SendNotification(CODE_PRINTING,0);
    CChartContainer* pContainer = CloneChartContainer(string_t(_T("")), true);
    pChart->SendNotification(CODE_PRINTED,0);
    PrintCharts(pContainer, scrDpiX, printDlg.GetPrinterDC());
    delete pContainer;
  }
  GlobalFree(printDlg.m_pd.hDevMode);
  GlobalFree(printDlg.m_pd.hDevNames);
}

void CChartContainer::OnMnZoomInX(void)  // ZoomIn from the context menu
{  
  CChart* pChart = GetChart(m_chartSelIdx);
  CAxis* pAxis = pChart->GetAxis();
  if (pAxis->m_startX >= pAxis->m_endX)
    return;

  double fStartZX, fEndZX;
  double extX = pAxis->m_endX - pAxis->m_startX;

  if (pAxis->m_chModeX == MODE_ZOOMINGX)         // Left zoom border was set
  {
    double distStartX = m_pProperty->m_tmpZX - pAxis->m_startX;
    double distEndX   = pAxis->m_endX - m_pProperty->m_tmpZX;
    if (distStartX < distEndX)
    {
      fStartZX = m_pProperty->m_tmpZX;
      fEndZX   = pAxis->m_endX - distStartX;
    }
    else if (distStartX > distEndX)
    {
      fStartZX = pAxis->m_startX + distEndX;
      fEndZX   = m_pProperty->m_tmpZX;
    }
    else
    {
      fStartZX = m_pProperty->m_tmpZX + 0.1*extX;
      fEndZX   = m_pProperty->m_tmpZX - 0.1*extX;
    }
  }
  else
  {
    fStartZX = pAxis->m_startX + 0.1*extX;
    fEndZX   = pAxis->m_endX   - 0.1*extX;
  }

 pChart->ZoomContainerX(fStartZX, fEndZX, true);
 pChart->SendNotification(CODE_EXTX,0);
}

void CChartContainer::OnMnMoveRight(void)
{
// Move right
  CChart* pChart = GetChart(m_chartSelIdx);
  CAxis* pAxis = pChart->GetAxis();
  double deltaX = 10.0*MOVE_DELTAX*(pAxis->m_endX - pAxis->m_startX);
  pChart->MoveContainerChartsX(deltaX, true);
  pChart->SendNotification(CODE_EXTX,0);
}

void CChartContainer::OnMnMoveLeft(void)
{
// Move left
  CChart* pChart = GetChart(m_chartSelIdx);
  CAxis* pAxis = pChart->GetAxis();
  double deltaX = -10.0*MOVE_DELTAX*(pAxis->m_endX - pAxis->m_startX);
  pChart->MoveContainerChartsX(deltaX, true);
  pChart->SendNotification(CODE_EXTX);
}

void CChartContainer::OnMnZoomInY(void)
{
	CChart* pChart = GetChart(m_chartSelIdx);
	CAxis* pAxis = pChart->GetAxis();
  if (pAxis->m_minExtY >= pAxis->m_maxExtY)
    return;

  double fStartZY, fEndZY;
  double extY = pAxis->m_maxExtY - pAxis->m_minExtY;

  if (pAxis->m_chModeY == MODE_ZOOMINGY)         // Left zoom border was set
  {
    double distStartY = m_pProperty->m_tmpZY - pAxis->m_minExtY;
    double distEndY   = pAxis->m_maxExtY - m_pProperty->m_tmpZY;
    if (distStartY < distEndY)
    {
      fStartZY = m_pProperty->m_tmpZY;
      fEndZY   = pAxis->m_maxExtY - distStartY;
    }
    else if (distStartY > distEndY)
    {
      fStartZY = pAxis->m_minExtY + distEndY;
      fEndZY   = m_pProperty->m_tmpZY;
    }
    else
    {
      fStartZY = pAxis->m_minExtY;
      fEndZY   = pAxis->m_maxExtY;
	  fStartZY += 0.1*extY;
	  fEndZY   -= 0.1*extY;
    }
  }
  else
  {
    fStartZY = pAxis->m_minExtY;
    fEndZY   = pAxis->m_maxExtY;
    if (fStartZY != fEndZY)
    {
      if (pAxis->m_chModeY != MODE_FULLY)
      {
        fStartZY += 0.1*extY;
        fEndZY   -= 0.1*extY;
      }
    }
  }

 pChart->ZoomContainerY(fStartZY, fEndZY, true);
 pChart->SendNotification(CODE_EXTY,0);
}

void CChartContainer::OnMnEnableTracking(void)
{
	CChart* pChart = GetChart(m_chartSelIdx);
  m_pProperty->m_bTracking = !m_pProperty->m_bTracking;
  if (!m_pProperty->m_bTracking)
  {
    m_pDataWnd = DestroyLabWnd(m_pDataWnd);
    m_dataLegPntD = PointD(DBL_MAX, DBL_MAX);
  }
 pChart->SendNotification(CODE_TRACKING,0);
}

void CChartContainer::OnMnShowPnts(void)
{
  int chartIdx = -1;
  CChart* chartPtr = NULL;
  if (m_chartSelIdx == -1)
  {
    chartIdx = HasChartDifferentPnts(-1, true);
    if (chartIdx != -1)
    {
      m_chartSelIdx = SelectChart(chartIdx);
    }
  }
  else
    chartIdx = HasChartDifferentPnts(m_chartSelIdx, true);

  if (chartIdx == -1)
  {
    AfxMessageBox(_T("Select a chart with enough data points first"), MB_ICONERROR|MB_OK);
    return;
  }
  
  chartPtr = GetChart(m_chartSelIdx);
  ENSURE(chartPtr != NULL);
  ChartProperty* pAttr = chartPtr->GetProperty();
  bool bShow = pAttr->ArePntsAllowed();
  pAttr->AllowPnts(!bShow);
  UpdateContainerWnds();
  chartPtr->SendNotification(CODE_SHOWPNTS, m_chartSelIdx);
}

void CChartContainer::OnMnShowChart(UINT nCmdID)
{
  int chartIdx = nCmdID - IDM_CHART_FIRSTCHART;
  CChart* pChart = GetChart(chartIdx);
  if(!pChart) return;

  if (ToggleChartVisibility(chartIdx) > -1)
  pChart->SendNotification(CODE_VISIBILITY, chartIdx);
}

void CChartContainer::UpdateDataViewPnts(int chartIdx, size_t dataID, PointD dataPntD, bool bAdd)
{
  bool bRedraw = true;

  if (chartIdx == -1)
  {
    m_mapDataViewPntsD.clear(); 
  }
  else if (m_dataViewChartIdx == chartIdx)
  {
    CChart* chartPtr = GetChart(chartIdx); 
    if (chartPtr != NULL)                   // Chart is not deleted
    {
      if (bAdd)
        m_mapDataViewPntsD.insert(MAP_SELPNTSD::value_type(dataID, dataPntD));
      else
      {
        MAP_SELPNTSD::iterator it = m_mapDataViewPntsD.find(dataID);
        if (it != m_mapDataViewPntsD.end())
          m_mapDataViewPntsD.erase(dataID);
        else
          bRedraw = false;  // Not found the data point
      }
    }
    else                    // Chart was removed, cleanup did not finished
    {
      m_mapDataViewPntsD.clear();
      m_dataViewChartIdx = -1;
      bRedraw = false;      // Chart was redrawn earlier
    }
  }   // Received right chartIdx
  else
    bRedraw = false;

  if (bRedraw && (NULL != GetVisibleChart(m_dataViewChartIdx)))
    RefreshWnd();
}

void CChartContainer::UpdateDataViewPnts(int chartIdx, const MAP_SELPNTSD& mapSelPntsD)
{

  if (chartIdx == m_dataViewChartIdx)
  {
    if (mapSelPntsD.size() > 0)
      m_mapDataViewPntsD = mapSelPntsD;
    else
      m_mapDataViewPntsD.clear();

    if (NULL != GetVisibleChart(chartIdx))
      RefreshWnd();
  }
}

void CChartContainer::EqualizeVertRanges(double spaceMult, bool bRedraw)
{
  MAP_CHARTS mapCharts;
  MAP_CHARTS::const_iterator mapIt = m_mapCharts.cbegin();
  MAP_CHARTS::const_iterator mapItE = m_mapCharts.cend();

  for (; mapIt != mapItE; ++mapIt)
  {
    if (mapIt->second->HasData())
      mapCharts.insert(*mapIt);
  }
  
  CChart* pChart = GetChart(m_chartSelIdx);
  CAxis* pAxis = pChart->GetAxis();

// Get Range
  if (mapCharts.size() > 1)
  {
    typedef std::pair<double, CChart*> PAIR_CHR;
    typedef std::vector<PAIR_CHR> V_CHR;

    V_CHR vChr(mapCharts.size());
    transform(mapCharts.cbegin(), mapCharts.cend(), vChr.begin(),
      [](const std::pair<int, CChart*>& pair_ch) ->PAIR_CHR 
        {CChart* pChart = pair_ch.second; double val = max(fabs(pChart->GetMaxValY()), fabs(pChart->GetMinValY()));
         return make_pair(val, pChart);});

     sort(vChr.begin(), vChr.end(), [](const PAIR_CHR& lhs, const PAIR_CHR& rhs) ->bool {return lhs.first > rhs.first;});

     double chMaxY = vChr[0].first;
     for_each(++vChr.begin(), vChr.end(),
       [&chMaxY, spaceMult](const PAIR_CHR&  pair_chr) ->void {chMaxY = spaceMult*chMaxY; double locScY = chMaxY/pair_chr.first;
                                                    pair_chr.second->SetLocScaleY(locScY);});

    double minY = pAxis->GetMinY();
    double maxY = pAxis->GetMaxY();
    switch (pAxis->m_axYPos)
    {
    case TOP: maxY = 0.0; break;
    case MIDDLE: maxY = max(fabs(minY), maxY); minY = -maxY; break;
    case BOTTOM: minY = 0.0; break;
    }
    pChart->UpdateExtY(minY, maxY, bRedraw);
  }
}

void CChartContainer::DrawDataCurve( RectF rGdiF, Graphics* grPtr )
{
	RectF drawRF = rGdiF;   
	drawRF.X += 35;
	drawRF.Width -= 46;
	drawRF.Height -= 35;

	MAP_CHARTS::iterator it = m_mapCharts.begin();
	MAP_CHARTS::iterator itE = m_mapCharts.end();
	for (; it != itE; ++it)
	{
		CChart* chartPtr = it->second;
		if(!chartPtr) continue;

		CAxis* pAxis = chartPtr->GetAxis();
		if(!pAxis) continue;

		ChartProperty* pAttr = chartPtr->GetProperty();
		if(!pAttr) continue;

		if (chartPtr->IsChartVisible() == false)
			continue;
		if (chartPtr->m_vDataPnts.size() == 0)
			continue;

		GraphicsPath grPath;            // Will be used actoss the drawing routines
		chartPtr->SetDrawRect(drawRF);
		chartPtr->DrawChartCurve(chartPtr->m_vDataPnts, &grPath, grPtr);
		if (m_pProperty->m_bTracking&&IsLabWndExist(false))              // Draw selection points if any
			DrawSelPntsD(chartPtr, m_mapSelPntsD, &grPath, grPtr, drawRF);
	}
}

 
void CChartContainer::OnMnMoveUp(void)
{
	// Move Up
	CChart* pChart = GetChart(m_chartSelIdx);
	CAxis* pAxis = pChart->GetAxis();
	double deltaY = 10.0*MOVE_DELTAY*(pAxis->m_maxExtY - pAxis->m_minExtY);
	pChart->MoveContainerChartsY(deltaY, true);
	pChart->SendNotification(CODE_EXTY,0);
}

void CChartContainer::OnMnMoveDown(void)
{
	CChart* pChart = GetChart(m_chartSelIdx);
	CAxis* pAxis = pChart->GetAxis();
	double deltaY = -10.0*MOVE_DELTAY*(pAxis->m_maxExtY - pAxis->m_minExtY);
	pChart->MoveContainerChartsY(deltaY, true);
	pChart->SendNotification(CODE_EXTY,0);
}





