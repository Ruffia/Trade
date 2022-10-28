#pragma once
#include "ChartDef.h"
#include "Util.h"
using namespace Gdiplus;
using namespace std;

string_t __stdcall GetLabelValStr(double val, int precision, bool bAddEqSign);


class ChartProperty
{
public:
	ChartProperty(bool bShowPnts,string_t label, string_t labelY, 
		int precisionY, DashStyle dashStyle, float penWidth, float tension, Color col);

	ChartProperty();

public:
	// Label 
	string_t GetChartName(void) const {return m_label;}
	void SetChartName(string_t name) {m_label = name;}
	// YAxis
	string_t GetAxisYName(void) const {return m_labelY;}
	void SetAxisYName(string_t labelY) {m_labelY = labelY.empty() ? string_t(_T("Y")) : labelY;}

	val_label_str_fn GetLabYValStrFnPtr(void) const {return m_pLabYValStrFn;}
	void SetLabYValStrFn(val_label_str_fn pLabYValStrFn) {m_pLabYValStrFn = pLabYValStrFn;}

	// Show chart points
	bool ArePntsAllowed(void) const {return m_bShowPnts;}
	void AllowPnts(bool bAllowed) { m_bShowPnts = bAllowed;}

	// Color
	Color GetChartColor(void) const {return m_colChart;}
	void SetChartColor(Color colChart) {m_colChart = colChart;}

	// Dash style
	DashStyle GetChartDashStyle(void) const {return m_dashStyle;}
	void SetDashStyle(DashStyle dashStyle) {m_dashStyle = dashStyle;}

	// Tension
	float GetTension(void) const {return m_fTension;}
	void SetTension(float fTension) {m_fTension = fTension;}

	// Pen width
	float GetPenWidth(void) const {return m_fPenWidth;}
	void SetPenWidth(float penWidth) {m_fPenWidth = penWidth;}

	// Precision on Y-axis
	int GetPrecisionY(void) const {return m_precisionY;}
	void SetPrecisionY(int precisionY) {m_precisionY = precisionY;}

public:
	bool m_bShowPnts;

	Color m_colChart;
	DashStyle m_dashStyle;
	float m_fPenWidth;
	float m_fTension;       // Smoothiness of the curve

	int m_precisionY;       // Precision of Y-axis

	string_t m_label;       // Chart's Name
	string_t m_labelY;      // Y-axis unit name
	val_label_str_fn m_pLabYValStrFn;  // Pointer to the Y-value string generator
};