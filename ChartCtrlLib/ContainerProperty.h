#pragma once
#include "ChartDef.h"
#include "Util.h"
using namespace Gdiplus;
using namespace std;

class CChartContainer;

class CContainerProperty
{
public:
	CContainerProperty();

	CContainerProperty(const CContainerProperty& src);

	~CContainerProperty();

	void SetContainer(CChartContainer* pContianer);
public:
	Color GetBkColor(void) const {return m_colBkground;}
	Color GetBorderColor(void) const {return m_colBorder;}
	Color GetZoomBorderColor(void) const { return m_colZoomBorder;}
	Color GetLegPntsColor(void) const {return m_colLegPnts;}
	Color GetDataViewPntsColor(void) const { return m_colDataViewPnts;}
	Color GetLegBkgndColor(void) const {return m_colLegBkgnd;}
	Color GetLegBorderColor(void) const {return m_colLegBorder;}
	// Access to colors, set
	void SetBkColor(Color col, bool bRedraw = false);
	void SetBorderColor(Color col, bool bRedraw = false);
	void SetLegPntsColor(Color col, bool bRedraw = false);
	void SetDataViewPntsColor(Color col, bool bRedraw = false);
	void SetLegBkgndColor(Color col, bool bRedraw = false);
	void SetLegBorderColor(Color col, bool bRedraw = false);

	// Access to precision (number of significant digits)
	int GetContainerPrecisionX(void) const {return m_precision;}
	void SetContainerPrecision(int precision, bool bRedraw = false);

	// State of the popup menu
	bool IsUserEnabled(void) const {return m_bUserEnabled;}
	void EnableUser(bool bEnable, bool bClearState = true);

	bool HasNamesLeg(void) const {return m_bHasNamesLeg;}
	void SetNamesLegFlag(bool bHasNamesLeg) { m_bHasNamesLeg = bHasNamesLeg;}

public:
	bool m_bTracking;         // Cross cursor and data legend are enabled
	bool m_bHasNamesLeg;       // Window for name legend was not created
	bool m_bUserEnabled;      // Enabling/Disabling the popup menu
	bool m_bAxBoundaries;     // Show the X axis left and right boundaries
	int  m_precision;         // Number of decimal digits after decimal point for data labels
	double m_epsX;            // Neighbourhood to search for nearest point

	Color m_colBkground;
	Color m_colBorder;
	Color m_colZoomBorder;
	Color m_colLegPnts;
	Color m_colDataViewPnts;
	Color m_colLegBkgnd;
	Color m_colLegBorder;

	double m_tmpZX;           // Temporary placeholder for zoom startX 
	double m_tmpZY;           // Temporary placexolder for zoom startY

	CChartContainer* m_pContainer;
};