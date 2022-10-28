///////////////////////////////////////////////////////////////////////////////
// Chart.h

#include "ChartDef.h"
#include "Util.h"
#include <vector>
#include "ChartProperty.h"
#include "Axis.h"
#pragma once

using namespace Gdiplus;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Class CChart 

class MatrixD;
class CChartContainer;
class CDataWnd;

class CChart
{
public:
// Ctor and destructor
  CChart(void);
  ~CChart(void);
// Operations and Implementation
  CChart* CloneChart(bool bCloneState = false);

// Access to data members
// Read only; software takes care of assigning chartIdx
  int GetChartIdx(void) const {return m_nIdx;}

  // Selection
  bool IsChartSelected() const {return m_bSelected;}
  void SelectChart(bool bSel) { m_bSelected = bSel;}

// Minmax X and Y
  double GetMinValX(void) const {return m_fMinValX;}
  void SetMinValX(double fMinValX) {m_fMinValX = fMinValX;}

  double GetMaxValX(void) const {return m_fMaxValX;}
  void SetMaxValX(double fMaxValX) {m_fMaxValX = fMaxValX;}

  double GetMinValY(void) const { return m_fMinValY;}
  void SetMinValY(double fMinValY) {m_fMinValY = fMinValY;}

  double GetMaxValY(void) const {return m_fMaxValY;}
  void SetMaxValY(double fMaxValY) {m_fMaxValY = fMaxValY;}

  // Loc scale Y
  double GetLocScaleY(void) const {return m_fLocScaleY;}
  void SetLocScaleY(double locScaleY) {m_fLocScaleY = locScaleY;}

// Data
  size_t GetDataSize() const { return m_vDataPnts.size();}
// Get whether the chart is ready to be drawn
  bool HasData(void) const { return (m_vDataPnts.size() > 0);}

  void ResetChartData(void) { m_vDataPnts.clear();}

// Implementation
  void SetChartAttr(ChartProperty* pAttr) {m_pProperty = pAttr;}
  void SetIndex(int nIdx){ m_nIdx = nIdx;}
//protected:
  // Returns iterators that points to the last point.X <= startX and first point.X >= endX
  PAIR_ITS GetStartEndDataIterators(V_CHARTDATAD& vDataPnts, double startX, double endX);
public:
  size_t AppendChartData(V_CHARTDATAD& vData);
  bool TruncateChartData(double startX, double endX);

  PAIR_ITNEAREST GetNearestPointD(const PointD& origPntD, double dist, PointD& selPnt);

// Used to get chart name and visuals for data and names legends
  TUPLE_LABEL GetSelValString(const PointD selPntD, string_t nameX, int precision, 
                                                                      val_label_str_fn pLabXValStrFnPtr);
// Draws the curve and, if the distance between chart points allows, draws data points
  virtual bool DrawChartCurve(V_CHARTDATAD& vDataPnts, GraphicsPath* grPathPtr, Graphics* grPtr, float dpiRatio = 1.0f);

// Casts points in the range startX, endX to PointsF that are positions of data pnts in 
// the ChartContainer window
  virtual bool ConvertChartData(V_CHARTDATAD& vDataPnts, V_CHARTDATAF& vChDatF, MatrixD* pMatrixD, double startX, double endX);

// Get visible chart name and color
  bool GetVisibleChartNameAndVisuals(TUPLE_LABEL& tuple_res); 

  void SetProperty(ChartProperty* pProperty) { m_pProperty = pProperty;}
  ChartProperty* GetProperty() {return m_pProperty;}

  void SetAxis(CAxis* pAxis) { m_pAxis = pAxis;}
  CAxis* GetAxis() {return m_pAxis;}

  void SetChartContainer(CChartContainer* pContainer);

  void SetZoomBorderColor(Color col);
  bool HasVisiblePntsDInYBand(int chartIdx, double minY, double maxY);

  MatrixD* GetTransformMatrixD(double startX, double endX, double minY, double maxY);
  void SetTransformMatrixD(void); 

  bool ZoomMoveContainerX(double startX, double endX, bool bRedraw);
  bool ZoomMoveContainerY(double startY, double endY, bool bMoving, bool bRedraw);

  CH_MODE ZoomContainerX(double startX, double endX, bool bRedraw = false);
  CH_MODE ZoomContainerY(double startY, double endY, bool bRedraw);

  // If deltaX < 0 the chart curves are moving to the left
  CH_MODE MoveContainerChartsX(double deltaX, bool bRedraw = false);

  // Delta < 0 moved up
  CH_MODE MoveContainerChartsY(double deltaY, bool bRedraw = false);

  // Save charts as a picture
  Status SaveContainerImage(string_t pathName = string_t(_T("")));
  // Save/Load to/from XML files
  HRESULT SaveChartData(string_t pathName, bool bAll = false);

  // Visibility
  bool IsChartVisible() const {return m_bVisible;}
  void SetChartVisibility(bool bVisible) 
  { 
	  m_bVisible = bVisible;
	  m_pAxis->SetVisibility(m_bVisible);
  }

  bool ToggleChartVisibility() 
  {
	  m_bVisible = !m_bVisible; 
	  return m_bVisible;
  }

public:
   // To set members to min/max of arguments and m_startX, m_endX, m_minExtY, m_maxExtY
   void UpdateExtX(double minExtX, double maxExtX, bool bRedraw = false);
   void UpdateExtY(double minExtY, double maxExtY, bool bRedraw = false);

   PAIR_DBLS SetExtX(bool bRedraw = false);
   PAIR_DBLS SetExtY(bool bRedraw = false);

   void OnMnMoveUp(void);
   void OnMnMoveDown(void);

   // Notification on actions from the popup menu
   LRESULT SendNotification(UINT code, int chartIdx = -1);

   void SetDrawRect(const RectF& drawRF);

public:
	// Uses m_dataLegPntD; it is supposed to be used only programmatically
	CDataWnd* ShowDataLegend(double dataLegX); 

// Data members
public:
  int m_nIdx; 
  bool m_bSelected;
  double m_fLocScaleY;    // Additional PointD.Y multiplier; normally 1.0
  double m_fMinValX;      // Min X coordinate
  double m_fMaxValX;      // Max X coordinate
  double m_fMinValY;      // Min Y coord; used to calculate scaleY
  double m_fMaxValY;      // Max Y coord; used to calculate scaleY

  V_CHARTDATAD m_vDataPnts;

  CChartContainer* m_pContainer;
  CAxis* m_pAxis;
  ChartProperty* m_pProperty;
  RectF  m_drawRF;        // »­Í¼ÇøÓò
  bool   m_bVisible;
};

