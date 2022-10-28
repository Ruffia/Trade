///////////////////////////////////////////////////////////////////////////////
//
// CChart.cpp
//
///////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ChartDef.h"
#include "Gdiplus.h"
#include "DataLabel.h"
#include "Chart.h"
#include "ChartProperty.h"
#include "ChartContainer.h"
#include "ChartsXMLSerializer.h"
#include "Util.h"


using namespace std;
using namespace Gdiplus;

///////////////////////////////////////////////////////////////////////////////
// struct CChart

CChart::CChart(void) : m_nIdx(-1),m_fMinValX(DBL_MAX), m_fMaxValX(-DBL_MAX), m_fMinValY(DBL_MAX ),m_fMaxValY(-DBL_MAX),
	m_pAxis(NULL),m_pProperty(NULL),m_fLocScaleY(1.0),m_bVisible(true)
{
}

CChart::~CChart(void)
{
	if (m_pProperty)
	{
		delete m_pProperty;
		m_pProperty = NULL;
	}

	if (m_pAxis)
	{
		delete m_pAxis;
		m_pAxis = NULL;
	}
}

CChart* CChart::CloneChart(bool bCloneState)
{
  CChart* chartPtr = new CChart();
  *(chartPtr->m_pProperty) = *m_pProperty;

  chartPtr->m_nIdx        = m_nIdx;
  if (!bCloneState)
  {
	  chartPtr->m_bSelected   = false;
	  chartPtr->m_pProperty->m_bShowPnts   = true;
	  chartPtr->m_fLocScaleY  = 1.0;
  }

  chartPtr->m_fMinValX      = m_fMinValX;
  chartPtr->m_fMaxValX      = m_fMaxValX;
  chartPtr->m_fMinValY      = m_fMinValY;
  chartPtr->m_fMaxValY      = m_fMaxValY;

  chartPtr->m_vDataPnts   = m_vDataPnts;
  *(chartPtr->m_pAxis)    = *m_pAxis;

  return chartPtr;
}


PAIR_ITS CChart::GetStartEndDataIterators(V_CHARTDATAD& vDataPnts, double startX, double endX)
{
  PAIR_ITS pair_its = find_border_pnts(vDataPnts.begin(), vDataPnts.end(),
                                                    not_inside_range<double, false>(startX, endX));
  return pair_its;
}

size_t CChart::AppendChartData(V_CHARTDATAD& vData)
{
  if (vData.empty())
    return 0;

// Find first element to append
  V_CHARTDATAD::iterator itStart = vData.begin();
  if (!m_vDataPnts.empty())
  {
    PointD oldEndPntD = m_vDataPnts.back();
    double oldEndX = oldEndPntD.X;
    itStart = find_if(itStart, vData.end(), greater_or_equal<double, false>(oldEndX));
    if (itStart != vData.end()) // Find the first not a dublicate
    { 
      while ((itStart != vData.end())&&(oldEndPntD.X == itStart->X))
      {
        if (oldEndPntD.Y != itStart->Y)
          break;
        ++itStart;
      }
    }
    if (itStart == vData.end())
      return 0;       // All data points are duplicates
  }

// Resize and copy
  size_t oldSize = m_vDataPnts.size();
  size_t sizeNew = oldSize + distance(itStart, vData.end());
  m_vDataPnts.resize(sizeNew);   
  copy(itStart, vData.end(), m_vDataPnts.begin() + oldSize);
  
// Correct minmaxes
  m_fMinValX = m_vDataPnts.front().X; // Just in case; it was not changed
  m_fMaxValX = m_vDataPnts.back().X;
  std::pair<V_CHARTDATAD::iterator, V_CHARTDATAD::iterator> pair_minmaxY =
    minmax_element(m_vDataPnts.begin(), m_vDataPnts.end(), less_pnt<double, true>());
  m_fMinValY = pair_minmaxY.first->Y;
  m_fMaxValY = pair_minmaxY.second->Y;
  return sizeNew - oldSize; 
}

bool CChart::TruncateChartData(double startX, double endX)
{
  if (startX >= endX)           // Wrong parameters
    return false;
  if (!HasData())
    return false;
  if ((m_fMinValX >= startX) && (m_fMaxValX <= endX))
    return false;               // Nothing to truncate
  
  PAIR_ITS pair_its = GetStartEndDataIterators(m_vDataPnts, startX, endX);
// We need this last point to draw the right end of the chart
  if (pair_its.second != m_vDataPnts.end())
    ++pair_its.second;

  size_t dataSize = distance(pair_its.first, pair_its.second);
  if (dataSize == 0)
  {
    m_vDataPnts.clear();
    m_fMinValX = DBL_MAX;
    m_fMaxValX = -DBL_MAX;
    m_fMinValY = DBL_MAX;
    m_fMaxValY = -DBL_MAX;
  }
  else
  {
    V_CHARTDATAD vDataD(dataSize);
    copy(pair_its.first, pair_its.second, vDataD.begin());

    m_vDataPnts.swap(vDataD);

    m_fMinValX = m_vDataPnts.front().X; 
    m_fMaxValX = m_vDataPnts.back().X;
    std::pair<V_CHARTDATAD::iterator, V_CHARTDATAD::iterator> pair_minmaxY =
           minmax_element(m_vDataPnts.begin(), m_vDataPnts.end(), less_pnt<double, true>());
    m_fMinValY = pair_minmaxY.first->Y;
    m_fMaxValY = pair_minmaxY.second->Y;
  }
  return true;
}

PAIR_ITNEAREST CChart::GetNearestPointD(const PointD& origPntD, double dist, PointD& selPntD)
{
  V_CHARTDATAD::iterator it = m_vDataPnts.begin(), itE = m_vDataPnts.end();
  int nmbMultPntsD = 0;
  double leftX = origPntD.X - dist/2.0;
  double rightX = origPntD.X + dist/2.0;
// Find the first point in distance range from the origPntsD.X, if it exists 
  it = find_if(it, itE, coord_in_range<double, false>(leftX, rightX));
  if (it != itE)  // Find closest to origPntD.X
  {
    it = find_nearest(it, itE, nearest_to<double, false>(origPntD, dist));
    if (it != itE)          // Impossible; will return found_if at least
    {
      selPntD = *it;        // Get number of multivalued points (the same X's, different Y's)
      nmbMultPntsD = count_if(m_vDataPnts.begin(), it, count_in_range<double, false>(selPntD.X, selPntD.X));
      return make_pair(it, nmbMultPntsD + 1);
    }
  }
  return make_pair(itE, 0);
}

// Formats string and prepares chart visuals for the screen; for the print use the container helper
TUPLE_LABEL CChart::GetSelValString(const PointD selPntD, string_t nameX, 
                                              int precision, val_label_str_fn pLabValXStrFnPtr)
{
  TUPLE_LABEL tuple_label;
  get<IDX_LNAME>(tuple_label)  = m_pProperty->m_label; 
  get<IDX_LNAMEX>(tuple_label) = nameX;
  bool bAddEqSign = nameX.empty() ? false : true;
  get<IDX_LX>(tuple_label)     = pLabValXStrFnPtr(selPntD.X, precision, bAddEqSign);
  get<IDX_LNAMEY>(tuple_label) = m_pProperty->m_labelY;
  bAddEqSign = m_pProperty->m_labelY.empty() ? false : true;
  get<IDX_LY>(tuple_label)     = m_pProperty->m_pLabYValStrFn(selPntD.Y, m_pProperty->m_precisionY, bAddEqSign);

  int alpha = max(m_pProperty->m_colChart.GetAlpha(), ALPHA_MINFORLABEL);  
  Color labCol = SetAlpha(m_pProperty->m_colChart, alpha);
  get<IDX_LCOLOR>(tuple_label) = labCol;

  get<IDX_LDASH>(tuple_label) = m_pProperty->m_dashStyle;
  get<IDX_LPEN>(tuple_label)  = m_pProperty->m_fPenWidth;

  return tuple_label;
}


void CChart::SetDrawRect(const RectF& drawRF)
{ 
	m_drawRF = drawRF;
	m_pAxis->SetDrawRect(drawRF);
}

bool CChart::DrawChartCurve(V_CHARTDATAD& vDataPntsD, GraphicsPath* grPathPtr, Graphics* grPtr, float dpiRatio)
{
  if (vDataPntsD.size()== 0)    // Just for safe programming; the function is never called on count zero
    return false;

  m_pAxis->RestoreModeX();  // Might be in the middle of settung zoon borders; annuls the mode
  m_pAxis->RestoreModeY();

  // Draw axes
  if (!m_pContainer->HasChartWithData(-1))
  {
	  m_pAxis->DrawGrid(PointF(0.0f, 0.0f), grPtr,1.0);
	  return false;
  }

  // Proceed normally even with one point data
  const double startX = m_pAxis->m_vecGridXGraduation[0];
  const double endX = m_pAxis->m_vecGridXGraduation[m_pAxis->m_vecGridXGraduation.size() - 1];

  const double minExtY = 0;
  const double maxExtY = max(m_pAxis->m_vecGridYGraduation[0] ,m_pAxis->m_vecGridYGraduation[m_pAxis->m_vecGridYGraduation.size() - 1]);
  PointF pntOrigF = m_pAxis->DrawAxes(startX, endX, minExtY, maxExtY, grPtr);
  // Draw a grid    
  m_pAxis->DrawGrid(pntOrigF, grPtr,1.0);
  // Update scales
  std::pair<double, double> pair_scales = m_pAxis->UpdateScales(m_drawRF, startX, endX, minExtY, maxExtY);
  m_pAxis->m_scX = pair_scales.first;
  m_pAxis->m_scY = pair_scales.second;
  // Set transform matrix
  MatrixD matrixD;
  matrixD.Translate(pntOrigF.X, pntOrigF.Y);
  matrixD.Scale(m_pAxis->m_scX, -m_pAxis->m_scY);
  // Account for not even axes placement
  if (m_pAxis->m_axXPos != BETWEEN ||m_pAxis->m_axYPos != MIDDLE)
  {
	  double translateX = (m_pAxis->m_axXPos == RIGHT) ? -endX : (m_pAxis->m_axXPos == LEFT) ? -startX : 0.0;
	  double translateY = (m_pAxis->m_axYPos == BOTTOM) ? -minExtY : (m_pAxis->m_axYPos == TOP) ? -maxExtY : 0.0;
	  matrixD.Translate(translateX, translateY);
  }
  if (m_pAxis->m_pMatrixD != NULL)
	  delete m_pAxis->m_pMatrixD;
  m_pAxis->m_pMatrixD = matrixD.Clone();     // Copy constructor is private ??


  V_CHARTDATAF vDataPntsF;
// Convert the pntsD to the screen pntsF
  if (!ConvertChartData(vDataPntsD, vDataPntsF, m_pAxis->m_pMatrixD, startX, endX)) 
    return false;

  V_CHARTDATAF::iterator itF = vDataPntsF.begin();
  V_CHARTDATAF::pointer ptrDataPntsF = vDataPntsF.data();
  size_t vSize = vDataPntsF.size();

// Add the curve to grPath
  Pen pen(m_pProperty->m_colChart, m_pProperty->m_fPenWidth*dpiRatio);
  pen.SetDashStyle(m_pProperty->m_dashStyle);
  if (!m_pProperty->m_bShowPnts&&(vSize == 2))   // Are outside or at boundaries of clipping area
  {                                 // Make special semi-transparent dash pen
    Color col(SetAlpha(m_pProperty->m_colChart, ALPHA_NOPNT));
    pen.SetColor(col);
  }

  if (m_pProperty->m_dashStyle != DashStyleCustom)
  {
    if (vSize > 1)
    {
      grPtr->DrawCurve(&pen, ptrDataPntsF, vSize, m_pProperty->m_fTension);

      if (m_bSelected && (dpiRatio == 1.0f))  // Mark the chart as selectes on screen only
      {
        Pen selPen(Color(SetAlpha(m_pProperty->m_colChart, ALPHA_SELECT)), (m_pProperty->m_fPenWidth + PEN_SELWIDTH)*dpiRatio);
        grPtr->DrawCurve(&selPen, ptrDataPntsF, vSize, m_pProperty->m_fTension);
      }
    }

// Now add the points
    if (m_pProperty->m_bShowPnts || (vSize == 1))
    {
      itF = adjacent_find(vDataPntsF.begin(), vDataPntsF.end() , 
                         lesser_adjacent_interval<PointF, false>(PointF(dpiRatio*CHART_PNTSTRSH, 0.0f)));
      if (itF == vDataPntsF.end())    // All intervals are greater than CHART_PNTSTRSH   
      {
        itF = vDataPntsF.begin();    // Base
        for (; itF != vDataPntsF.end(); ++itF)
        {
          RectF rPntF = RectFFromCenterF(*itF, dpiRatio*CHART_DTPNTSZ, 
                                                                dpiRatio*CHART_DTPNTSZ);
          grPathPtr->AddEllipse(rPntF);
        }
      }
    }
  }
  else
  {
    PointF pntF;
    PointF pntFX(dpiRatio*CHART_DTPNTSZ/2, 0.0f);
    PointF pntFY(0.0f, dpiRatio*CHART_DTPNTSZ/2);

    for (; itF != vDataPntsF.end(); ++itF)
    {
      pntF = *itF;
      grPathPtr->StartFigure();
      grPathPtr->AddLine(pntF - pntFX, pntF + pntFX);
      grPathPtr->StartFigure();
      grPathPtr->AddLine(pntF - pntFY, pntF + pntFY);
    }
    if (vSize == 1)
    {
      grPathPtr->StartFigure();
      grPathPtr->AddEllipse(RectFFromCenterF(pntF, 2.0f*pntFX.X, 2.0f*pntFY.Y));
    }
  }

  if (grPathPtr->GetPointCount() > 0)          // Has points to draw
  {  
    pen.SetWidth(1.0f*dpiRatio);
    pen.SetDashStyle(DashStyleSolid);
    grPtr->DrawPath(&pen, grPathPtr);
    if (((m_pProperty->m_dashStyle == DashStyleCustom)||(vSize == 1))&& m_bSelected && (dpiRatio == 1.0f))
    {
      pen.SetColor(Color(SetAlpha(m_pProperty->m_colChart, ALPHA_SELECT)));
      pen.SetWidth(m_pProperty->m_fPenWidth + PEN_SELWIDTH);
      grPtr->DrawPath(&pen, grPathPtr);
    }
    grPathPtr->Reset();
  }
  return true;
}

bool CChart::ConvertChartData(V_CHARTDATAD& vDataPnts, V_CHARTDATAF& vDataPntsF, MatrixD* pMatrixD, double startX, double endX)
{
  ENSURE(vDataPnts.size() > 0);
  V_CHARTDATAD::iterator itFirst, itLast;
  if (vDataPnts.size() == 1)
  {
    itFirst = vDataPnts.begin();
    itLast = vDataPnts.end();
  }
  else
    std::tie(itFirst, itLast) = GetStartEndDataIterators(vDataPnts, startX, endX);
  if (itLast != vDataPnts.end())    // Last iterator points after the last point
    ++itLast;

  int vSize = distance(itFirst, itLast);
  if (vSize == 0)
    return false;
// Transform to screen pointsF
  vDataPntsF.resize(vSize);
  transform(itFirst, itLast, vDataPntsF.begin(), TRANSFORM_TO_PNTF(m_fLocScaleY, pMatrixD));

  return true;
}

bool CChart::GetVisibleChartNameAndVisuals(TUPLE_LABEL& tuple_res)
{ 
  if (m_bVisible && HasData())
  {
    int alpha = m_pProperty->m_colChart.GetAlpha();
    alpha = max(alpha, 128);
    string_t emptyStr(_T(""));
    tuple_res = make_tuple(m_pProperty->m_label, emptyStr, emptyStr, emptyStr, emptyStr, SetAlpha(m_pProperty->m_colChart,alpha), m_pProperty->m_dashStyle, m_pProperty->m_fPenWidth);
    return true;
  }
// Does not change the tuple_res  
  return false;
}


void CChart::SetChartContainer(CChartContainer* pContainer)
{
	m_pContainer = pContainer;
}

void CChart::UpdateExtX(double minExtX, double maxExtX, bool bRedraw)
{
	if (maxExtX < minExtX)                   // Possible if from app
		return;

	double initStartX = m_pAxis->GetInitialStartX();   // Old initial m_startX, m_endX
	double initEndX   = m_pAxis->GetInitialEndX();

	double startX, endX;

	if (initStartX > initEndX)  // The container is empty
	{
		startX = minExtX;
		endX   = maxExtX;
	}
	else
	{
		startX = min(minExtX, initStartX);
		endX   = max(maxExtX, initEndX);
	}

	if (startX == endX)
	{
		endX += fabs(startX*0.01);
	}


	m_pAxis->m_startX = startX;
	m_pAxis->m_endX   = endX;

	if (bRedraw)
	{
		if (m_pContainer->m_pProperty->m_bTracking&& m_pContainer->IsLabWndExist(true))
			m_pContainer->UpdateDataLegend(false);
		else
			m_pContainer->RefreshWnd();
	}
}


void CChart::UpdateExtY(double minExtY, double maxExtY, bool bRedraw)
{
	if (maxExtY < minExtY)                   // Possible if from app
		return;

	double initMinY = m_pAxis->GetInitialMinExtY();   // Old initial m_startX, m_endX
	double initMaxY = m_pAxis->GetInitialMaxExtY();

	double startY, endY;

	if (initMinY > initMaxY)  // The container is empty
	{
		startY = minExtY;
		endY   = maxExtY;
	}
	else
	{
		startY = min(minExtY, initMinY);
		endY   = max(maxExtY, initMaxY);
	}

	if (startY == endY)
	{
		double delta = fabs(startY*0.01);
		startY -= delta*4.0;
		endY   += delta;
	}

	m_pAxis->m_minExtY = startY;
	m_pAxis->m_maxExtY = endY;

	if (bRedraw)
	{
		if (m_pContainer->m_pProperty->m_bTracking&& m_pContainer->IsLabWndExist(false))
			m_pContainer->UpdateDataLegend(true);
		else
			m_pContainer->RefreshWnd();
	}
}


PAIR_DBLS CChart::SetExtX(bool bRedraw)
{
	double startX, endX;
	std::tie(startX, endX) = PAIR_DBLS(m_pAxis->m_startX,m_pAxis->m_endX);
	if (startX == endX)
		endX += fabs(startX*0.01);

	m_pAxis->m_startX = startX;
	m_pAxis->m_endX   = endX;

	if (bRedraw)
	{
		if (m_pContainer->m_pProperty->m_bTracking&& m_pContainer->IsLabWndExist(true))
			m_pContainer->UpdateDataLegend(false);
		else
			m_pContainer->RefreshWnd();
	}

	return make_pair(startX, endX);
}


PAIR_DBLS CChart::SetExtY(bool bRedraw)
{
	double minY, maxY;
	std::tie(minY, maxY) = PAIR_DBLS(m_pAxis->GetMinY(),m_pAxis->GetMaxY());

	if (minY == maxY)
	{
		double delta = fabs(maxY*0.01);
		minY -= delta*4.0;
		maxY += delta;
	}

	m_pAxis->m_minExtY = minY;
	m_pAxis->m_maxExtY = maxY;

	if (bRedraw)
	{
		if (m_pContainer->m_pProperty->m_bTracking&& m_pContainer->IsLabWndExist(false))
			m_pContainer->UpdateDataLegend(true);
		else
			m_pContainer->RefreshWnd();
	}

	return make_pair(minY, maxY);
}


void CChart::SetZoomBorderColor(Color col)
{
	if (m_pContainer->m_pProperty->m_colZoomBorder.GetValue() != col.GetValue())
	{
		m_pContainer->m_pProperty->m_colZoomBorder = col;
		if (m_pAxis->m_chModeX == MODE_ZOOMINGX)
		{
			PointD tmpPntD(m_pContainer->m_pProperty->m_tmpZX, 0.0);      
			PointF tmpPntF = m_pAxis->m_pMatrixD->TransformToPntF(1.0, tmpPntD);
			CPoint tmpPnt  = CPointFromPntF(tmpPntF);
			m_pContainer->DrawZoomBorderX(tmpPnt.x);
		}
		else if (m_pAxis->m_chModeY == MODE_ZOOMINGY)
		{
			PointD tmpPntD(0.0, m_pContainer->m_pProperty->m_tmpZY);      
			PointF tmpPntF = m_pAxis->m_pMatrixD->TransformToPntF(1.0, tmpPntD);
			CPoint tmpPnt  = CPointFromPntF(tmpPntF);
			m_pContainer->DrawZoomBorderY(tmpPnt.y);
		}
	}
}


bool CChart::HasVisiblePntsDInYBand(int chartIdx, double bottomY, double topY)
{
	bool bHasPntsD = false;
	CRect clR;
	m_pContainer->GetClientRect(&clR);
	CPoint pntTop(clR.left, clR.top);
	CPoint pntBottom(clR.right, clR.bottom);
	PointD pntTopD, pntBottomD;

	MatrixD* pMatrixD = GetTransformMatrixD(m_pAxis->m_startX, m_pAxis->m_endX, bottomY, topY);
	m_pContainer->MousePntToPntD(pntTop, pntTopD, pMatrixD);
	m_pContainer->MousePntToPntD(pntBottom, pntBottomD, pMatrixD);
	delete pMatrixD;

	auto chartIt = chartIdx == -1 ? m_pContainer->m_mapCharts.cbegin() : m_pContainer->m_mapCharts.find(chartIdx);

	for (; chartIt != m_pContainer->m_mapCharts.cend(); ++chartIt)
	{
		CChart* chartPtr = chartIt->second;
		if (!IsChartVisible())
			continue;

		double locScY = chartPtr->GetLocScaleY();
		double topY = pntTopD.Y;
		double bottomY = pntBottomD.Y;
		// Select visible X-part of the chart
		V_CHARTDATAD::iterator itB, itE;
		std::tie(itB, itE) = chartPtr->GetStartEndDataIterators(chartPtr->m_vDataPnts, m_pAxis->m_startX, m_pAxis->m_endX);
		itB = find_if(itB, itE, 
			[topY, bottomY, locScY](const PointD& pntD) {return in_range(bottomY, topY, locScY*pntD.Y);});
		if (itB != itE)
		{
			bHasPntsD = true;
			break;
		}
		if (chartIdx != -1)
			break;
	}

	return bHasPntsD;
}


void CChart::SetTransformMatrixD(void) 
{ 
	if (m_pAxis->m_pMatrixD != NULL) 
		delete m_pAxis->m_pMatrixD;

	m_pAxis->m_pMatrixD = GetTransformMatrixD(m_pAxis->m_startX, m_pAxis->m_endX, m_pAxis->m_minExtY, m_pAxis->m_maxExtY);
}



MatrixD* CChart::GetTransformMatrixD(double startX, double endX, double minY, double maxY)
{
	CDC* pDC = m_pContainer->GetDC();
	Graphics gr(pDC->m_hDC);

	RectF rGdiF;
	gr.GetVisibleClipBounds(&rGdiF);                 // The same as the clip rect

	rGdiF.Width -= 1.0f;                          
	rGdiF.Height -= 1.0f;
	rGdiF.Inflate(-DR_SPACE, -DR_SPACE);
	// Axis X: Its Y Position and Offset; also Y coord of orig point
	PAIR_XAXPOS axXPosY = m_pAxis->GetXAxisPos(rGdiF, minY, maxY);
	// Axis Y: Its X Position and ofset; also X-coord of orig. point
	PAIR_YAXPOS axYPosX = m_pAxis->GetYAxisPos(rGdiF, startX, endX); 

	std::pair<double, double> pair_scales = m_pAxis->UpdateScales(rGdiF, 
		startX, endX, minY, maxY);
	double scX = pair_scales.first;
	double scY = pair_scales.second;

	MatrixD matrixD;
	matrixD.Translate(axYPosX.second, axXPosY.second); 
	matrixD.Scale(scX, -scY);

	if ((axYPosX.first != BETWEEN)||(axXPosY.first != MIDDLE))
	{
		double translateX = (axYPosX.first == RIGHT) ? -endX : 
			(axYPosX.first == LEFT) ? -startX : 0.0;
		double translateY = (axXPosY.first == BOTTOM) ? -minY : 
			(axXPosY.first == TOP) ? -maxY : 0.0;
		matrixD.Translate(float(translateX), float(translateY)); 
	}

	m_pContainer->ReleaseDC(pDC);
	return matrixD.Clone();
}


bool CChart::ZoomMoveContainerX(double startX, double endX, bool bRedraw)
{
	if (startX == endX)
		return false;

	double fStartX = m_pAxis->GetInitialStartX();
	double fEndX   = m_pAxis->GetInitialEndX();

	// Don't zoom/move an empty space: one of the points should be inside
	if (in_range(fStartX, fEndX, startX) || in_range(fStartX, fEndX, endX))
	{
		m_pAxis->m_startX = min(startX, endX);
		m_pAxis->m_endX   = max(startX, endX);
		m_pAxis->m_chModeX = MODE_ZOOMX;   // All prepared but not rendered
		if (bRedraw && IsWindow(m_pContainer->m_hWnd) && m_pContainer->IsWindowVisible() && m_pContainer->HasChartWithData(-1, true))
		{
			if (m_pContainer->m_pProperty->m_bTracking && m_pContainer->IsLabWndExist(false))
				m_pContainer->UpdateDataLegend(true);
			else
				m_pContainer->RefreshWnd();
		}
		return true;
	}
	return false;
}


bool CChart::ZoomMoveContainerY(double startY, double endY, bool bMoving, bool bRedraw)
{
	if (startY == endY)
		return false;

	if (startY > endY)
		swap(startY, endY);

	// Don't zoom/move an empty space: one of the points should be inside the band
	if (HasVisiblePntsDInYBand(-1, startY, endY))
	{
		m_pAxis->m_minExtY = startY;
		m_pAxis->m_maxExtY = endY;
		if (bMoving && ((m_pAxis->m_chModeY == MODE_FULLY)||(m_pAxis->m_chModeY == MODE_MOVEDY))) 
			m_pAxis->m_chModeY = MODE_MOVEY;
		else
			m_pAxis->m_chModeY = MODE_ZOOMY;   // All prepared but not rendered

		if (bRedraw && IsWindow(m_pContainer->m_hWnd) && m_pContainer->IsWindowVisible())
		{
			if (bMoving && (m_pAxis->m_chModeY == MODE_FULLY))
				m_pAxis->m_chModeY = MODE_MOVEDY;
			else
				m_pAxis->m_chModeY = MODE_ZOOMEDY;   // Had zoom Y before

			if (m_pContainer->m_pProperty->m_bTracking && m_pContainer->IsLabWndExist(false))
				m_pContainer->UpdateDataLegend(true);
			else
				m_pContainer->RefreshWnd();
		}
		return true;
	}
	return false;
}


CH_MODE CChart::ZoomContainerX(double startX, double endX, bool bRedraw)
{
	if (true == ZoomMoveContainerX(startX, endX, false))
	{
		if (bRedraw && IsWindow(m_pContainer->m_hWnd) && m_pContainer->IsWindowVisible())
		{
			if (m_pContainer->m_pProperty->m_bTracking && m_pContainer->IsLabWndExist(false))
				m_pContainer->UpdateDataLegend(true);
			else
				m_pContainer->RefreshWnd();
			m_pAxis->m_chModeX = MODE_ZOOMEDX; // Old extensionX is in m_dqHist
		}
		else
			m_pAxis->m_chModeX = MODE_ZOOMX;   // Wait for redrawing
	}
	else
		m_pAxis->m_chModeX = MODE_FULLX;

	return m_pAxis->m_chModeX;
}


CH_MODE CChart::ZoomContainerY(double startY, double endY, bool bRedraw)
{
	// Get real minmax ext;
	double rangeY = endY - startY;      // It is called from menu, so startY < endY
	endY    = startY + 0.9*rangeY;      // 0.9 screen height down from the top
	startY += 0.1*rangeY;               // 0.1 screen height up from the bottom

	if (true == ZoomMoveContainerY(startY, endY, false, false))  // Sets chModeY to MODE_ZOOMY on success
	{
		if (bRedraw && IsWindow(m_pContainer->m_hWnd) && m_pContainer->IsWindowVisible())
		{
			if (m_pContainer->m_pProperty->m_bTracking && m_pContainer->IsLabWndExist(false))
				m_pContainer->UpdateDataLegend(true);
			else
				m_pContainer->RefreshWnd();

			m_pAxis->m_chModeY = MODE_ZOOMEDY; // Old extensionX is in m_dqHist
		}

	}

	return m_pAxis->m_chModeY;
}



CH_MODE CChart::MoveContainerChartsX(double deltaX, bool bRedraw)
{
	if (true == ZoomMoveContainerX(m_pAxis->m_startX - deltaX, m_pAxis->m_endX - deltaX, false))
	{
		if (bRedraw && IsWindow(m_pContainer->m_hWnd) && m_pContainer->IsWindowVisible())
		{
			if (m_pContainer->m_pProperty->m_bTracking && m_pContainer->IsLabWndExist(false))
				m_pContainer->UpdateDataLegend(true);
			else
				m_pContainer->RefreshWnd();
			m_pAxis->m_chModeX = MODE_ZOOMEDX; // Old extensionX is in m_dqHist
		}
		else
			m_pAxis->m_chModeX = MODE_ZOOMX;   // Wait for redrawing
	}
	else
		m_pAxis->m_chModeX = MODE_FULLX;

	return m_pAxis->m_chModeX;
}


CH_MODE CChart::MoveContainerChartsY(double deltaY, bool bRedraw)
{
	if (true == ZoomMoveContainerY(m_pAxis->m_minExtY - deltaY, m_pAxis->m_maxExtY - deltaY, true, false)) // Sets modeY to MODE_MOVEY
	{                                                                                    // or MODE_ZOOMY (if was zoomed previously
		if (bRedraw && IsWindow(m_pContainer->m_hWnd) && m_pContainer->IsWindowVisible())
		{
			if (m_pContainer->m_pProperty->m_bTracking && m_pContainer->IsLabWndExist(false))
				m_pContainer->UpdateDataLegend(true);
			else
				m_pContainer->RefreshWnd();

			if (m_pAxis->m_chModeY == MODE_MOVEY)
				m_pAxis->m_chModeY = MODE_MOVEDY;
			else
				m_pAxis->m_chModeY = MODE_ZOOMEDY;
		}
	}

	return m_pAxis->m_chModeY;
}





LRESULT CChart::SendNotification(UINT code, int chartIdx)
{
	NMCHART nmchart;
	nmchart.hdr.hwndFrom = m_pContainer->m_hWnd;
	nmchart.hdr.idFrom = m_pContainer->GetDlgCtrlID();
	nmchart.hdr.code = code;
	nmchart.chartIdx = chartIdx;

	switch (code)
	{
	case CODE_VISIBILITY: nmchart.bState = m_pContainer->IsChartVisible(chartIdx);            break;
	case CODE_SHOWPNTS:   nmchart.bState = m_pContainer->AreChartPntsAllowed(chartIdx).first; break;
	case CODE_EXTX:  
	case CODE_EXTY:
		nmchart.minX   = m_pAxis->GetStartX();
		nmchart.maxX   = m_pAxis->GetEndX();
		nmchart.minY   = m_pAxis->GetMinY();
		nmchart.maxY   = m_pAxis->GetMaxY();
		break;
	case CODE_REFRESH:    nmchart.minX   = m_pAxis->GetInitialStartX();
		nmchart.maxX   = m_pAxis->GetInitialEndX();
		nmchart.minY   = m_pAxis->GetInitialMinExtY();
		nmchart.maxY   = m_pAxis->GetInitialMaxExtY();
		break;
	case CODE_SAVEIMAGE:
	case CODE_SAVEDIMAGE: 
	case CODE_SAVECHARTS:
	case CODE_SAVEDCHARTS: 
	case CODE_PRINTING:
	case CODE_PRINTED:    
	case CODE_SCY:        break;
	case CODE_TRACKING:   nmchart.bState = m_pContainer->m_pProperty->m_bTracking;
		break;
	default:              return 0;
	}

	CWnd* parentPtr = (CWnd*)m_pContainer->GetParent();
	if (parentPtr != NULL)
		return parentPtr->SendMessage(WM_NOTIFY, WPARAM(nmchart.hdr.hwndFrom), LPARAM(&nmchart));
	return 0;
}


Status CChart::SaveContainerImage(string_t pathName)
{
	USES_CONVERSION;
	if (!m_pContainer->HasChartWithData(-1,true))   // Repeat to provide for standalone use
		return GenericError; 

	Status status = Aborted;
	UINT  num;        // number of image encoders
	UINT  size;       // size, in bytes, of the image encoder array

	// How many encoders are there? How big (in bytes) is the array of all ImageCodecInfo objects?
	GetImageEncodersSize(&num, &size);
	// Create a buffer large enough to hold the array of ImageCodecInfo objects that will be returned by GetImageEncoders.
	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));;
	// GetImageEncoders creates an array of ImageCodecInfo objects and copies that array into a previously allocated buffer. 
	GetImageEncoders(num, size, pImageCodecInfo);
	// Get filter string
	sstream_t stream_t;
	string_t str_t, tmp_t;
	string_t szFilter;
	CLSID clsID;
	typedef std::map<string_t, CLSID> MAP_CLSID;
	typedef MAP_CLSID::value_type TYPE_VALCLSID;
	typedef MAP_CLSID::iterator IT_CLSID;

	MAP_CLSID mapCLSID;

	for(UINT j = 0; j < num; ++j)
	{ 
		stream_t << pImageCodecInfo[j].MimeType <<_T("\n");   
		getline(stream_t, str_t);
		size_t delPos = str_t.find(TCHAR('/'), 0);
		str_t.erase(0, delPos + 1);
		clsID = pImageCodecInfo[j].Clsid;
		mapCLSID.insert(TYPE_VALCLSID(str_t, clsID));
		tmp_t = str_t;
		std::transform(tmp_t.begin(), tmp_t.end(), tmp_t.begin(), [](const TCHAR&tch) ->TCHAR {return (TCHAR)toupper(tch);});
		szFilter += tmp_t + string_t(_T(" File|*.")) + str_t + string_t(_T("|")); 
	}
	szFilter += string_t(_T("|"));
	free(pImageCodecInfo);

	if (pathName.empty())     // Let the user choose
	{
		TCHAR szWorkDirPath[255];
		GetModuleFileName(NULL, szWorkDirPath, 255);
		PathRemoveFileSpec(szWorkDirPath);

		string_t dirStr(szWorkDirPath);
		size_t lastSlash = dirStr.find_last_of(_T("\\")) + 1;
		dirStr.erase(lastSlash, dirStr.size() - lastSlash);
		dirStr += string_t(_T("Images"));
		szFilter += string_t(_T("|"));
		CFileDialog fileDlg(FALSE, _T("BMP File"), _T("*.bmp"), 
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_EXPLORER,
			szFilter.c_str(), m_pContainer);

		fileDlg.m_ofn.lpstrInitialDir = dirStr.c_str();
		fileDlg.m_ofn.lpstrTitle = _T("Save As Image");

		string_t strTitle(_T("Save "));

		if (fileDlg.DoModal() == IDOK)
		{
			pathName = string_t(fileDlg.GetPathName());
		}
		else
			return Ok;
	}

	if (pathName.empty())
		return InvalidParameter;

	size_t pos = pathName.find(_T("."));
	if (pos == string_t::npos)
		return GenericError;

	string_t szExt = pathName.substr(pos);
	pos = szFilter.find(szExt);
	if (pos == string_t::npos)
		return UnknownImageFormat;
	szExt.erase(0, 1);

	IT_CLSID it = mapCLSID.find(szExt);
	if (it != mapCLSID.end())
	{
		SendNotification(CODE_SAVEIMAGE);
		CRect clR;
		m_pContainer->GetClientRect(&clR);
		Rect rGdi = CRectToGdiRect(clR);
		Bitmap bmp(rGdi.Width, rGdi.Height);
		m_pContainer->DrawContainerToBmp(rGdi, bmp);

		clsID = it->second;
		status = bmp.Save(A2W(pathName.c_str()), &clsID);
		SendNotification(CODE_SAVEDIMAGE);
	}
	else status = UnknownImageFormat; 
	return status;
}

HRESULT CChart::SaveChartData(string_t pathName, bool bAll)
{
	HRESULT hr = S_FALSE;
	// See what you are going to save
	if (m_pContainer->m_mapCharts.empty())
		return hr;
	if (!bAll &&(NULL == m_pContainer->GetVisibleChart(m_pContainer->m_chartSelIdx)))
		return hr;

	// Select file
	if (pathName.empty())       // The user should enter the path
	{
		TCHAR szFilters[]= _T("XML Files (*.xml)|*.xml||");
		TCHAR szWorkDirPath[255];
		GetModuleFileName(NULL, szWorkDirPath, 255);
		PathRemoveFileSpec(szWorkDirPath);

		string_t dirStr(szWorkDirPath);
		size_t lastSlash = dirStr.find_last_of(_T("\\")) + 1;
		dirStr.erase(lastSlash, dirStr.size() - lastSlash);
		dirStr += string_t(_T("Charts"));

		CFileDialog fileDlg(FALSE, _T("xml"), _T("*.xml"), 
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_EXPLORER,
			szFilters, m_pContainer);
		fileDlg.m_ofn.lpstrInitialDir = dirStr.c_str();
		fileDlg.m_ofn.lpstrTitle = _T("Save Charts");

		string_t strTitle(_T("Save "));

		if (fileDlg.DoModal() == IDOK)
		{
			pathName = string_t(fileDlg.GetPathName());
		}
	}
	if (pathName.empty())
		return S_OK;

	size_t extPos = pathName.rfind(string_t(_T(".xml")));
	if (extPos != string_t::npos)
	{
		SendNotification(CODE_SAVECHARTS);
		CChartContainer* pContainer = m_pContainer->CloneChartContainer(string_t(_T("")), true);
		SendNotification(CODE_SAVEDCHARTS);
		string_t chartName;
		if (m_pContainer->m_chartSelIdx != -1)
			chartName = pContainer->GetChartName(m_pContainer->m_chartSelIdx);
		hr = CChartsXMLSerializer::ChartDataToXML(pathName.c_str(), pContainer, chartName, bAll);
		delete pContainer;
	}
	else
		hr = S_OK;

	return hr;
}


CDataWnd* CChart::ShowDataLegend(double dataLegX)
{
	if (!IsWindow(m_pContainer->m_hWnd)||!m_pContainer->IsWindowVisible())  // Means container wnd was created and is visible
		return m_pContainer->m_pDataWnd;
	if (m_pContainer->m_mapCharts.empty())                    // No charts, no legends
		return m_pContainer->m_pDataWnd;     

	MAP_LABSTR tmpMapStr;
	m_pContainer->m_dataLegPntD = PointD(dataLegX, m_pAxis->m_minExtY);
	// After this point mapSelData might have some points out the range Y, but mapLabs might be empty  
	size_t selPnts = m_pContainer->PrepareDataLegend(m_pContainer->m_dataLegPntD, CHART_DTPNTSZ/m_pAxis->m_scX, 
		tmpMapStr, m_pContainer->m_mapSelPntsD, false);
	if (selPnts > 0)
	{
		// Set m_pDataLeg
		CDataWnd* pDataWnd = NULL;
		if (m_pContainer->m_pDataWnd == NULL)
		{
			pDataWnd = new CDataWnd(m_pContainer->m_pProperty->m_colLegBkgnd, m_pContainer->m_pProperty->m_colLegBorder);
			if (pDataWnd == NULL)
				return NULL;
			m_pContainer->m_pDataWnd = pDataWnd;
		}
		if (m_pContainer->m_pDataWnd)
		{
			m_pContainer->m_pDataWnd->m_mapLabs.swap(tmpMapStr);
			PointF dataLegPntF = m_pAxis->m_pMatrixD->TransformToPntF(1.0, m_pContainer->m_dataLegPntD); 
			CPoint pnt = CPointFromPntF(dataLegPntF);
			m_pContainer->m_pDataWnd->CreateLegend(m_pContainer, pnt, true);
			m_pContainer->m_pProperty->m_bTracking = true;

			m_pContainer->RefreshWnd();
			m_pContainer->m_pDataWnd->ShowLegendWnd();  // Garanteed m_pDataWnd exists
			return m_pContainer->m_pDataWnd;
		}
	}

	m_pContainer->m_pProperty->m_bTracking = false;
	m_pContainer->m_pDataWnd = m_pContainer->DestroyLabWnd(m_pContainer->m_pDataWnd);
	return m_pContainer->m_pDataWnd;
}