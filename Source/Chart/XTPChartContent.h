// XTPChartContent.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTCONTENT_H__)
#	define __XTPCHARTCONTENT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDeviceContext;
class CXTPChartDiagram;
class CXTPChartTitleCollection;
class CXTPChartElementView;
class CXTPChartSeriesCollection;
class CXTPChartLegend;
class CXTPChartPanelCollection;
class CXTPChartSeries;
class CXTPChartAppearance;
class CXTPChartLegendView;
class CXTPPropExchange;
class CXTPMarkupContext;
class CXTPChartContentView;
class CXTPChartDiagramView;
class CXTPChartBorder;
class CXTPChartDeviceCommand;

enum XTPChartPanelDirection
{
	xtpChartPanelVertical,
	xtpChartPanelHorizontal
};

//===========================================================================
// Summary:
//     This class represents chart content, which is a kind of CXTPChartElement.
//     This class has a panel collection, a series collection, a title collection
//     and a legend.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartContent : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartContent)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartContent object.
	//-----------------------------------------------------------------------
	CXTPChartContent();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartContent object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartContent();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the title collection object.
	// Returns:
	//     A pointer to a CXTPChartTitleCollection object.
	//-----------------------------------------------------------------------
	CXTPChartTitleCollection* GetTitles() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the series collection object.
	// Returns:
	//     A pointer to a CXTPChartSeriesCollection object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesCollection* GetSeries() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the chart legend object.
	// Returns:
	//     A pointer to a CXTPChartLegend object.
	//-----------------------------------------------------------------------
	CXTPChartLegend* GetLegend() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the primary diagram of the chart. It is
	//     the diagram of the first panel.
	// Returns:
	//     A pointer to a CXTPChartDiagram object.
	//-----------------------------------------------------------------------
	CXTPChartDiagram* GetPrimaryDiagram() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the panels of the chart.
	// Returns:
	//     A pointer to a CXTPChartPanelCollection object.
	//-----------------------------------------------------------------------
	CXTPChartPanelCollection* GetPanels() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the appearance of the chart.
	// Returns:
	//     A pointer to a CXTPChartAppearance object.
	//-----------------------------------------------------------------------
	CXTPChartAppearance* GetAppearance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the border of the chart.
	// Returns:
	//     A pointer to a CXTPChartBorder object.
	//-----------------------------------------------------------------------
	CXTPChartBorder* GetBorder() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the chart content.
	// Parameters:
	//     pDC      - A pointer to the chart device context object.
	//     rcBounds - The bound of the chart content.
	//-----------------------------------------------------------------------
	void DrawContent(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the device command tree.
	// Parameters:
	//     hDC      - The GDI device context HANDLE.
	//     rcBounds - The rectangle of the device context.
	// Returns:
	//     A pointer to a CXTPChartDeviceCommand object - root of Command tree.
	//-----------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceComand(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the device context for the drawing.
	// Parameters:
	//     hDC      - The GDI device context HANDLE.
	//     rcBounds - The rectangle of the device context.
	// Returns:
	//     A pointer to a CXTPChartDeviceContext object.
	//-----------------------------------------------------------------------
	CXTPChartDeviceContext* CreateDeviceContext(CXTPChartContainer* pContainer, HDC hDC,
												CRect rcBounds, BOOL bWindowDC);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the background color of the chart content.
	// Returns:
	//     A CXTPChartColor object which describes an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the background color of the chart content.
	// Parameters:
	//     clr - Color of the background.
	//-----------------------------------------------------------------------
	void SetBackgroundColor(const CXTPChartColor& clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the appearance background color of the chart content.
	// Returns:
	//     A CXTPChartColor object which describes an ARGB value.
	// Remarks:
	//     Call SetBackgroundColor to set a custom background color. If a custom background
	//     color is not set, then a color from current appearance will be used.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the appearance border color.
	// Returns:
	//     A CXTPChartColor object which describes an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function whenever a change occurs in the chart content.
	//     This call causes a redraw.
	// Parameters:
	//     updateOptions - Options which indicate the kind of change performed.
	//-----------------------------------------------------------------------
	virtual void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of content
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store property data to or load
	//     property data from storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the view of the chart content.
	// Parameters:
	//     pDC      - Pointer to the chart device context object.
	//     rcBounds - The bounding rectangle.
	//-----------------------------------------------------------------------
	virtual CXTPChartContentView* CreateView(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update the color of the points from the palette.
	//-----------------------------------------------------------------------
	void UpdateDiagram();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update the chart content whenever there is a change
	//     in the series style.
	// Parameters:
	//     pSeries - A pointer to a chart series object.
	//-----------------------------------------------------------------------
	virtual void OnSeriesStyleChanged(CXTPChartSeries* pSeries);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the distance between 2 panels.
	// Returns:
	//     Distance between 2 panels.
	// Remarks:
	//     10 pixels is used by default.
	//-----------------------------------------------------------------------
	int GetPanelDistance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the distance between 2 panels
	// Parameters:
	//     nDistance - New distance to be set.
	//-----------------------------------------------------------------------
	void SetPanelDistance(int nDistance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the layout direction of panels - either
	//     top-to-bottom or left-to-right.
	// Returns:
	//     XTPChartPanelDirection value indicating the layout direction of panels
	//     (xtpChartPanelVertical/xtpChartPanelHorizontal)   //
	//-----------------------------------------------------------------------
	XTPChartPanelDirection GetPanelDirection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the layout direction of panels - either
	//     top-to-bottom or left-to-right.
	// Parameters:
	//     nDirection - XTPChartPanelDirection value indicating the layout direction of panels
	//                  (xtpChartPanelVertical/xtpChartPanelHorizontal)   //
	//-----------------------------------------------------------------------
	void SetPanelDirection(XTPChartPanelDirection nDirection);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for tab captions.
	// Parameters:
	//     bEnable - TRUE to enable markup.
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the markup context.
	// Returns:
	//     A pointer to a CXTPMarkupContext object.
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

protected:
	void AddContainer(CXTPChartContainer* pContainer);
	void RemoveContainer(CXTPChartContainer* pContainer);
	BOOL HasVisible3dSeries() const;

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartContent);
	DECLARE_OLECREATE_EX(CXTPChartContent)

	afx_msg LPDISPATCH OleGetSeries();
	afx_msg LPDISPATCH OleGetDiagrams();
	afx_msg LPDISPATCH OleGetTitles();
	afx_msg LPDISPATCH OleGetAppearance();
	afx_msg LPDISPATCH OleGetLegend();
	afx_msg LPDISPATCH OleGetBorder();

	afx_msg BOOL OleGetEnableMarkup();
	afx_msg void OleSetEnableMarkup(BOOL bEnabled);
	afx_msg void OleDoPropExchange(LPDISPATCH lpPropExchage);
	afx_msg OLE_COLOR OleGetBackgroundColor();
	afx_msg void OleSetBackgroundColor(OLE_COLOR clr);

	COleControl* GetOleControl();

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CXTPChartPanelCollection* m_pPanels;  // The panel collection.
	CXTPChartTitleCollection* m_pTitles;  // The title collection.
	CXTPChartSeriesCollection* m_pSeries; // The series collection.
	CXTPChartLegend* m_pLegend;			  // The legend object.

	CXTPChartAppearance* m_pAppearance; // The chart appearance object.

	CXTPChartBorder* m_pBorder; // The chart border.

	CXTPChartColor m_clrBackground; // The chart background color.

	CArray<CXTPChartContainer*, CXTPChartContainer*> m_arrContainers; // The chart containers

	CXTPMarkupContext* m_pMarkupContext; // Markup context

	int m_nPanelDistance;					  // Panel distance
	XTPChartPanelDirection m_nPanelDirection; // Panel direction.

	friend class CXTPChartSeriesCollection;
	friend class CXTPChartSeries;
	friend class CXTPChartControl;
};

AFX_INLINE CXTPChartTitleCollection* CXTPChartContent::GetTitles() const
{
	return m_pTitles;
}

AFX_INLINE CXTPChartSeriesCollection* CXTPChartContent::GetSeries() const
{
	return m_pSeries;
}
AFX_INLINE CXTPChartLegend* CXTPChartContent::GetLegend() const
{
	return m_pLegend;
}
AFX_INLINE CXTPChartPanelCollection* CXTPChartContent::GetPanels() const
{
	return m_pPanels;
}
AFX_INLINE CXTPChartAppearance* CXTPChartContent::GetAppearance() const
{
	return m_pAppearance;
}
AFX_INLINE CXTPMarkupContext* CXTPChartContent::GetMarkupContext() const
{
	return m_pMarkupContext;
}
AFX_INLINE int CXTPChartContent::GetPanelDistance() const
{
	return m_nPanelDistance;
}
AFX_INLINE void CXTPChartContent::SetPanelDistance(int nDistance)
{
	m_nPanelDistance = nDistance;
	OnChartChanged();
}
AFX_INLINE XTPChartPanelDirection CXTPChartContent::GetPanelDirection() const
{
	return m_nPanelDirection;
}
AFX_INLINE void CXTPChartContent::SetPanelDirection(XTPChartPanelDirection nDirection)
{
	m_nPanelDirection = nDirection;
	OnChartChanged();
}
AFX_INLINE void CXTPChartContent::SetBackgroundColor(const CXTPChartColor& clr)
{
	m_clrBackground = clr;
	OnChartChanged();
}
AFX_INLINE CXTPChartBorder* CXTPChartContent::GetBorder() const
{
	return m_pBorder;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTCONTENT_H__)
