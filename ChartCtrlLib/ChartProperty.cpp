#include "StdAfx.h"
#include "ChartProperty.h"

string_t __stdcall GetLabelValStr(double val, int precision, bool bAddEqSign)
{
	sstream_t stream_t;
	stream_t << std::setprecision(precision) << val;
	return bAddEqSign ? string_t(_T("= ")) + stream_t.str() : stream_t.str();
}


ChartProperty::ChartProperty(bool bShowPnts,string_t label, string_t labelY, 
			  int precisionY, DashStyle dashStyle, float penWidth, float tension, Color col):
   m_bShowPnts(bShowPnts),
   m_label(label),
   m_labelY(labelY),
   m_precisionY(precisionY),
   m_dashStyle(dashStyle),
   m_fPenWidth(penWidth),
   m_fTension(tension),
   m_colChart(col)
{

}


ChartProperty::ChartProperty() : m_bShowPnts(true), 
	m_colChart((ARGB)Color::Black), m_dashStyle(DashStyleSolid), 
	m_fPenWidth(2), m_fTension(0.0f), 
	m_precisionY(3), 
	m_label(_T("")), m_labelY(string_t(_T("Y"))),
	m_pLabYValStrFn(&GetLabelValStr)
{

}