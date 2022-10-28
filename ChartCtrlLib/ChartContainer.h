////////////////////////////////////////////////////////////////////////////// 
//
//  ChartContainer.h
//
///////////////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;
#include "ChartDef.h"
#include "ChartProperty.h"
#include "ContainerProperty.h"
#include "AxisManager.h"

#pragma once

using namespace Gdiplus;

///////////////////////////////////////////////////////////////////////////////
// Struct for notifications from CChartDataView

typedef struct  
  {
    NMHDR nmhdr;
    bool bAdd;
    int chartIdx;
    size_t dataID;
    PointT<double> dataPntD;
  } STRUCT_NOTIFY, *LP_STRUCT_NOTIFY;

///////////////////////////////////////////////////////////////////////////////
// class CChartContainer

class CChart;
class CDataWnd;
class CChartDataView;

class MatrixD;

class CChartContainer : public CWnd
{
	DECLARE_DYNAMIC(CChartContainer)

  friend class CDataView;

public:
	CChartContainer(string_t name = string_t(_T("")));
	virtual ~CChartContainer();

private:
  CChartContainer(const CChartContainer& src);
public:
  CChartContainer& operator =(const CChartContainer& rhs);
  bool CreateChartCtrlWnd(DWORD dwExStyle, DWORD dwStyle, const CRect& wndRect, 
                                                       CWnd* pParent, UINT nID);

public: 
  CChartContainer* CloneChartContainer(string_t name = string_t(_T("")), bool bCloneState = false);
protected:
  CChart* CloneChart(int chartIdx, bool bCloneState = false);

// Access to colors, get
public:

  int GetDataViewChartIdx(void) const { return m_dataViewChartIdx;}
  void SetDataViewChartIdx(int chartIdx) {m_dataViewChartIdx = chartIdx;}
  const MAP_SELPNTSD* GetDataViewSelPntsD(void) const {return &m_mapDataViewPntsD;}
  size_t SetDataViewSelPnts(int chartIdx, const MAP_SELPNTSD& mapSelPntsD,
                                                        V_CHARTDATAD* pChartData = NULL);
  bool ClearDataViewChartIdx(void);

  string_t GetContainerName(void) const {return m_name;}
  void SetContainerName(string_t name) { m_name = name;}

  string_t GetAxisXName(void) const {return m_labelX;}
  void SetAxisXName(string_t nameX, bool bRedraw = false); 
// X-axis string formatting function (container)
  val_label_str_fn GetLabXValStrFnPtr(void) const {return m_pLabValStrFnPtr;}
  void SetLabXValStrFn(val_label_str_fn pLabValStrFn, bool bRedraw = false);

  bool IsContainerEmpty(void) const {return m_mapCharts.empty();}
  bool HasChartWithData(int chartIdx, bool bVisibleOnly = false) const;
  int HasChartDifferentPnts(int chartIdx, bool bVisibleOnly = false) const;
 
  size_t GetContainerSize(void) const {return m_mapCharts.size();}
  MAP_CHARTS* GetMapCharts(void) {return &m_mapCharts;}
  MAP_CHARTS::iterator GetFirstChart(void) {return m_mapCharts.begin();}
  MAP_CHARTS::const_iterator GetFirstChart(void) const {return m_mapCharts.cbegin();}
  MAP_CHARTS::iterator GetChartsEnd(void) {return m_mapCharts.end();}
  MAP_CHARTS::const_iterator GetChartsEnd(void) const {return m_mapCharts.cend();}
  MAP_CHARTS::iterator FindChart(int chartIdx) {return m_mapCharts.find(chartIdx);}
  MAP_CHARTS::const_iterator FindChart(int chartIdx) const {return m_mapCharts.find(chartIdx);}

public:
// Calculates the transformation matrix for given client rectangle 

 
// Iterates over charts in the container and returns min/max X or Y. Does not change the container's
// m_startX, m_endX, m_minExtY, m_maxExtY
public:
// Called by parent or owner when parent's size is changing
  void OnChangedSize(int cx, int cy);

// Drawing the container
  void RefreshWnd(void);  // Redraws the container, excludes dataWnd

  HRESULT LoadCharts(LPCTSTR fileName, const MAP_CHARTCOLS& mapContent, bool bClearCharts = false);
  HRESULT ReplaceContainerCharts(LPCTSTR fileName);
  HRESULT GetChartNamesFromXMLFile(LPCTSTR fileName, MAP_CHARTCOLS& mapContent);
  HRESULT GetChartNamesFromXMLFile(LPCTSTR fileName, MAP_NAMES& mapNames);
// TODO: constantnes of the get functions
  bool ExportChartData(string_t chartName, V_CHARTDATAD& vDataPnts) const;
  bool ExportChartData(string_t chartName, std::vector<std::pair<double, double> >& vPairs) const;
  bool ExportChartData(string_t chartName, std::vector<double>& vX, std::vector<double>& vY) const;

///////////////////////////////////////////////////////////////////////////////
// Access to chart and container state variables

// Returns the chart's minmax for X if bY == false, or for Y (if true), and true.
// in the third element. Returns false in the third element if the chart does not exist. 
  std::tuple<double, double, bool> GetChartBoundaries(int chartIdx, bool bY = false) const; 
// Visibility
public:
  CChart* GetVisibleChart(int chartIdx) const;  // Returns pointer to the (first) visible chart or NULL
public:
  bool IsChartVisible(int chartIdx) const;
  bool SetChartVisibility(int chartIdx, bool bVisible, bool bRedraw = false); 
  int ToggleChartVisibility(int chartIdx);// Returns 1 if set visible, 0 if not,
                                          // -1 if the chart does not exist
// Chart Pen Width  
  std::pair<float, bool> GetChartPenWidth(int chartIdx) const;
  bool UpdateChartPenWidth(int chartIdx, float penWidth, bool bVisibleOnly, bool bRedraw = false);// Changes all or selected pen width

// Tension
  std::pair<float, bool> GetChartTension(int chartIdx) const;
  bool SetChartTension(int chartIdx, float tension, bool bVisibleOnly, bool bRedraw = false);

// Show points
  std::pair<bool, bool> AreChartPntsAllowed(int chartIdx) const;
  bool AllowChartPnts(int chartIdx, bool bAllowed, bool bVisibleOnly, bool bRedraw = false);

// Chart Color:Returns false as the second member of the pair if the chart does not exist
  std::pair<Color, bool> GetChartColor(int chartIdx) const;
  bool SetChartColor(int chartIdx, Color col, bool bVisibleOnly, bool bRedraw = false);

// Chart Dash Style
  std::pair<DashStyle, bool> GetChartDashStyle(int chartIdx) const;
  bool SetChartDashStyle(int chartIdx, DashStyle dashStyle, bool bVisibleOnly, bool bRedraw = false);

// Get the selected chart
  int HasSelectedChart(void) const;       // Returns chartIdx or -1
  int SelectChart(int chartIdx, bool bRedraw = false);
  int SelectChartByMouseClick(CPoint pnt, bool bRedtaw = false);

// Chart Y axis precision
  int GetChartPrecisionY(int chartIdx) const;
  bool SetChartPrecisionY(int chartIdx, int precisionY, bool bVisibleOnly, bool bRedraw = false);

// Chart Name
  string_t GetChartName(int chartIdx) const;
  bool ChangeChartName(int chartIdx, string_t name, bool bRedraw = false);

// Chart Y data name
  string_t GetChartYValName(int chartIdx) const;
  bool ChangeChartYValName(int chartIdx, string_t nameY, bool bVisibleOnly, bool bRedraw = false);

// Function to get YVal string
  val_label_str_fn GetLabYValStrFnPtr(int chartIdx) const;
  bool SetLabYValStrFn(int chartIdx, val_label_str_fn m_pLabYValStrFn, bool bVisibleOnly, bool bRedraw = false);

// Get Chart: Returns chart pointer or NULL
public:
  CChart* GetChart(int chartIdx, bool bVisibleOnly = false) const;        
  CChart* FindChartByName(string_t nameStr) const;
public:   
  V_CHARTDATAD* GetChartData(int chartIdx);
  int GetChartIdx(string_t chartName) const;

// Local scale Y (m_locScY)
  std::pair<double, bool> GetChartLocScaleY(int chartIdx) const;
  bool UpdateChartLocScaleY(int chartIdx, double locScY, bool bVisibleOnly, bool bRedraw = false);

////////////////////////////////////////////////////////////////////////////////////
// Chart interface functions

public:
 int AddChart(ChartProperty* pAttr, CChart* pChart, V_CHARTDATAD& vData, bool bRedraw = false);
 // Overload for time series
 int AddChart(ChartProperty* pAttr, CChart* pChart, std::vector<double>& vTmSeries, double startX = 0.0, double stepX = 1.0, bool bRedraw = false);
 // Overload for vector of pairs
 int AddChart(ChartProperty* pAttr, CChart* pChart, std::vector<std::pair<double, double> >& vXYData, bool bRedraw = false);
// Overload for vectors of doubles
 int AddChart(ChartProperty* pAttr, CChart* pChart, std::vector<double>& vX, std::vector<double>& vY, bool bRedraw = false);
 
 // Appends points to the end of chart data vector. Discards all new points with X < m_endX
  bool AppendChartData(int chartIdx, V_CHARTDATAD& vData, bool bUpdate = false);
// Overload for time series
  bool AppendChartData(int chartIdx,  std::vector<double>& vTmSeries, double startX = 0.0,double stepX = 1.0, bool bUpdate = false);
// Overload for vector of pairs
  bool AppendChartData(int chartIdx,  std::vector<std::pair<double, double> >& vXYData,bool bUpdate = false);
// Overload for vector of doubles
  bool AppendChartData(int chartIdx,  std::vector<double>& vX, std::vector<double>& vY,bool bUpdate = false);
// Replaces chart data vector
  bool ReplaceChartData(int chartIdx, V_CHARTDATAD& vData, bool bClip = false, bool bUpdate = false,
                                           bool bVerbose = false, bool bRedraw = false);
// Overload for time series
  bool ReplaceChartData(int chartIdx,  std::vector<double>& vTmSeries, double startX = 0.0, 
     double stepX = 1.0, bool bClip = false, bool bUpdate = false, bool bVerbose = false, bool bRedraw = false);
// Overload for vector of pairs
  bool ReplaceChartData(int chartIdx,  std::vector<std::pair<double, double> >& vXYData, 
                    bool bClip = false, bool bUpdate = false, bool bVerbose = false, bool bRedraw = false);
// Overload for vector of doubles
  bool ReplaceChartData(int chartIdx,  std::vector<double>& vX, std::vector<double>& vY, 
                        bool bClip = false, bool bUpdate = false, bool bVerbose = false, bool bRedraw = false);
  
  void ResetCharts(bool bResetLocY = false);
  void ResetChartContainer(bool bKeepCharts = false, bool bResetCols = false);
  bool TruncateChart(int chartIdx, double startX, double endX, bool bUpdateExt = false, bool bRedraw = false);
  bool RemoveChart(int chartIdx, bool bCorrectMinMax, bool bRedraw);

  void UpdateDataViewPnts(int chartIdx, size_t dataID, PointD dataPntD, bool bAdd);
  void UpdateDataViewPnts(int chartIdx, const MAP_SELPNTSD& mapSelPntsD);

  void EqualizeVertRanges(double spaceMult, bool bRedraw = false);

///////////////////////////////////////////////////////////////////////////////
//  Legend functions
public:
  PointD GetDataLegPntD(void) const { return m_dataLegPntD;}
// To set the SelData pnt from a program
  bool IsLabWndVisible(bool bLeg) const;  // true - m_pLegWnd, false - m_pDataWnd
  bool IsLabWndExist(bool bLeg) const;    // same
  bool ShowAxisXBoundaries(bool bSet, bool bRedraw = false);
  
  CDataWnd* ShowNamesLegend(void);
  bool ShowChartPoints(int chartIdx, bool bShow, bool bRedraw = false);
public:
  CDataWnd* ShowDataLegend(CPoint pnt);
// Uses the selected points to correct the data label and shows it in the appropriate position
  void UpdateDataLegend(bool bChangeMatrix); 
  void CopyDataLegendWnd(void);
// Clear chart data view
public:
  CDataWnd* DestroyLabWnd(CDataWnd* pDataWnd, bool bRedraw = true);
  bool HideLabWnds(bool bRedraw = false);
  CChartDataView* DestroyChartDataView(void);
// To programmatically show a data view
  bool ShowDataView(const string_t& szChartName, bool bRefresh);

public:
// Updates container wnd and child windows
  void UpdateContainerWnds(int chartIdx = -1, bool bMatrix = false, DATAVIEW_FLAGS dataChange = F_NODATACHANGE);   
// To get max chart Idx in the mapCharts
  int GetMaxChartIdx(void) const;
 public:
// Translate the mouse coordinates to the data space (CPoint to PointD)
  void MousePntToPntD(CPoint pnt, PointD& dataPntD, MatrixD* pMatrixD, double locScY = 1.0);

// Zoom X on mouse left clicks + MK_SHIFT
  double BeginZoomingX(CPoint pnt, bool bDraw);                // First click
  double PrepareZoomX(CPoint pnt, double startX, bool bDraw);  // Second click
// Zoom Y on mouse left clicks + MK_SHIFT
  double BeginZoomingY(CPoint pnt, bool bDraw);                // First double click
  double PrepareZoomY(CPoint pnt, double startY, bool bDraw);  // Second click
 

// Legend 
  size_t PrepareDataLegend(CPoint pnt, MAP_LABSTR& mapLabels, MAP_SELPNTSD& mapSelPntsD);
  size_t PrepareDataLegend(PointD origPntD, double epsX, MAP_LABSTR& mapLabels, 
                                                    MAP_SELPNTSD& mapSelPntsD, bool bChangeMatrix);
  size_t PrepareNamesLegend(MAP_LABSTR& mapLabels);
  size_t UpdateDataLegend(MAP_SELPNTSD& mapSelPntsD, MAP_LABSTR& m_mapLabs); 

// To show data view
 public:
  bool ShowDataView(CChart* chartPtr, bool bClearMap, bool bRefresh = true);
  bool UpdateDataView(int chartIdx, DATAVIEW_FLAGS flagsData);
// To use when selected chart is appended, truncated or its data were replaced
  size_t UpdateDataViewPntsDMap(void);

// This returns chartPtr->m_loScY as a first, and deltaLocScY as a second; delta = 1% of Y range
 PAIR_DBLS GetLocScYDelta(CChart* chartPtr);

  void MoveChartsByArrowKeysX(UINT nChar);
  void ChangeLocScaleYByArrowKeys(UINT nChar);
  void MoveChartsByArrowKeysY(UINT nChar);

// Drawing functions
  void DrawBkground(RectF rClientF, Color colBkground, 
                              Color colBorder, Graphics* grPtr);
  bool DrawSelPntsD(CChart* chartPtr, const MAP_SELPNTSD& mapSelPntsD, 
                GraphicsPath* grPathPtr, Graphics* grPtr, RectF drawRF, float dpiRatio = 1.0f);
  bool DrawDataViewPntsD(const MAP_SELPNTSD& mapDataViewPntsD, double startX, double endX, 
              GraphicsPath* grPathPtr, Graphics* grPtr, float dpiRatio = 1.0f);
  void DrawSelLine(RectF clRF, Graphics* grPtr, int selChartIdx = -1, float dpiRatio = 1.0f);
  void DrawAxXBoundaryStr(RectF boundRF, Graphics* grPtr, float dpiRtio = 1.0f);
  void DrawZoomBorderX(CPoint pnt);
  void DrawZoomBorderY(CPoint pnt);
  virtual void DrawDataCurve( RectF rGdiF, Graphics* grPtr );
  void DrawContainerToBmp(Rect rGdi, Bitmap& clBmp);

// Print's helpers
  int GetScreenDpi(void);

  TUPLE_PRNLEGLAYOUT CalcNamesPntLayout(RectF namesRF, const MAP_PRNDATA& mapPrnData, Graphics* grPtr, float dpiRatio);
  void PrintCrossLine(PointF pntStartF, PointF pntEndF, SizeF sizeCrossF, Pen* penPtr, Graphics* grPtr, float dpiRatio); 
  void GetSelValString(RectF drawRF, CChart* chartPtr, double pageScY, MAP_PRNDATA& mapPrnData, float dpiRatio);
  size_t PrintChartNames(RectF namesRF, const MAP_PRNDATA& mapPrnData, 
                         const TUPLE_PRNLEGLAYOUT& tuple_prnLayout, 
                         Graphics* grPtr, float dpiRatio, size_t mapOffs = 0);

  string_t PrintPageHeader(RectF rGdiF, const CChartContainer* pContainer, string_t tmStr, Graphics* grPtr);
 public:
    static void PrintCharts(CChartContainer* pContainer, int scrDpiX, HDC printDC);

protected:
	DECLARE_MESSAGE_MAP()

// Message handlers
  afx_msg void OnPaint(void);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg UINT OnGetDlgCode();

  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

  afx_msg void OnTimer(UINT_PTR nIDEvent);

  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnSetFocus(CWnd* pOldWnd);

// Context Menu Command Handlers
  afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
  afx_msg void OnMnShowLegend(void);
  afx_msg void OnMnShowAxBoundaries(void);
  afx_msg void OnMnRefresh(void);
  afx_msg void OnMnViewData(void);
  afx_msg void OnMnSaveCharts(void);
  afx_msg void OnMnSaveImage(void);
  afx_msg void OnMnPrintCharts(void);
  afx_msg void OnMnZoomInX(void);  // ZoomIn from the context menu
  afx_msg void OnMnMoveRight(void);
  afx_msg void OnMnMoveLeft(void);
  afx_msg void OnMnZoomInY(void);
  afx_msg void OnMnMoveUp(void);
  afx_msg void OnMnMoveDown(void);
  afx_msg void OnMnEnableTracking(void);
  afx_msg void OnMnShowPnts(void);
  afx_msg void OnMnShowChart(UINT nCmdID);

#ifdef _TEST_
  afx_msg void OnMnTest(void);
#endif

// Data Members
 public:
  int  m_chartSelIdx;       // ID of the chart selected to scale Y locally
  int  m_dataViewChartIdx;  // ID of the chart displayed in data view
  PointD m_dataLegPntD;     // Transformed to the data point space point of sel line
  
 public:
  MAP_CHARTS m_mapCharts;   // Container's content
  MAP_SELPNTSD m_mapSelPntsD;       // Points nearest to the sel line for data window
  MAP_SELPNTSD m_mapDataViewPntsD;  // Chart's data points selected in the data view window 

  CDataWnd* m_pDataWnd;
  CDataWnd* m_pLegWnd;
  CChartDataView* m_pChartDataView;

public:
  CContainerProperty* m_pProperty;

protected:
  string_t m_name;          // Container's name
  string_t m_labelX;        // X-axis name

  val_label_str_fn m_pLabValStrFnPtr;
};

