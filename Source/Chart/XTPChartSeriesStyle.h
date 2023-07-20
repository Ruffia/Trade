// XTPChartSeriesStyle.h
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
#if !defined(__XTPCHARTSERIESSTYLE_H__)
#	define __XTPCHARTSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDiagram;
class CXTPChartSeriesLabel;
class CXTPChartSeriesView;
class CXTPChartSeries;
class CXTPChartDiagramView;
class CXTPChartAxis;
class CXTPChartSeriesStyleAppearance;
class CXTPChartErrorBar;
class CXTPChartDeviceCommand;

//===========================================================================
// Summary:
//     CXTPChartSeriesStyle is a CXTPChartElement derived class. It is the
//     base class for all Styles for the Chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSeriesStyle : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartSeriesStyle);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesStyle();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get/set the series label object.
	// Parameters:
	//     pLabel - A new label object to set.
	// Returns:
	//     Pointer to CXTPChartSeriesLabel that contains the properties for the label.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesLabel* GetLabel() const;
	virtual CXTPChartSeriesLabel*
		SetLabel(CXTPChartSeriesLabel* pLabel); // <combine CXTPChartSeriesStyle::GetLabel@const>

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get/set the error bar object.
	// Parameters:
	//     pErrorBar - A pointer to a new error bar object to set.
	// Returns:
	//     Pointer to CXTPChartErrorBar that contains the properties for the error bar.
	//-------------------------------------------------------------------------
	virtual CXTPChartErrorBar* GetErrorBar() const;
	virtual CXTPChartErrorBar* SetErrorBar(
		CXTPChartErrorBar* pErrorBar); // <combine CXTPChartSeriesStyle::GetErrorBar@const>

	//-------------------------------------------------------------------------
	// Summary:
	//     Implementations can override this method to customize the creation of
	//     a diagram object.
	// Returns:
	//     New diagram object that will contain the series.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagram* CreateDiagram();

	//-------------------------------------------------------------------------
	// Summary:
	//     Implementations should override this method to customize the logic of
	//     checking diagram compatibility with the chart style. By default,
	//     all diagrams are deemed incompatible, i.e. return FALSE.
	// Parameters:
	//     pDiagram - Diagram to check.
	// Returns:
	//     TRUE if the series can be inserted in the diagram object.
	//-------------------------------------------------------------------------
	virtual BOOL IsStyleDiagram(CXTPChartDiagram* pDiagram) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the color of the series.
	// Returns:
	//     The color of the series. If default appearance color is used, then
	//     CXTPChartColor::Empty will be returned.
	//-------------------------------------------------------------------------
	virtual CXTPChartColor GetColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set the color of the series.
	// Parameters:
	//     clr - New Color to be set. It can be CXTPChartColor::Empty to use the
	//           default appearance color.
	//-------------------------------------------------------------------------
	virtual void SetColor(CXTPChartColor clr);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set individual color for each point of the series.
	// Parameters:
	//     bColorEach - TRUE if individual color should be used for each point of the series.
	//-------------------------------------------------------------------------
	virtual void SetColorEach(BOOL bColorEach);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to check if individual color is used for
	//     each point of the series.
	//-------------------------------------------------------------------------
	virtual BOOL IsColorEach() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains rotation parameters for 3D chart.
	// Returns:
	//      Rotation parameters for 3D chart.
	// See also:
	//      Set3dRotation
	//-----------------------------------------------------------------------
	virtual CXTPChart3dRotation Get3dRotation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets rotation parameters for 3D chart.
	// Parameters:
	//      rotation - Rotation parameters in degrees.
	//      dYaw - Yaw angle value in degrees.
	//      dPitch - Pitch angle value in degrees.
	//      dRoll - Roll angle value in degrees.
	// See also:
	//      GetRotation
	//-----------------------------------------------------------------------
	virtual void Set3dRotation(const CXTPChart3dRotation& rotation);
	virtual void Set3dRotation(double dYaw, double dPitch,
							   double dRoll); // <combine CXTPChartSeriesStyle::Set3dRotation@const
											  // CXTPChart3dRotation&>

	//-----------------------------------------------------------------------
	// Summary:
	//      Enables/disables 3D edge smoothing.
	// Parameters:
	//      bEnable - TRUE to enable 3D edge smoothing, FALSE to disable 3D edge smoothing.
	// Returns:
	//      TRUE if edge smoothing is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Is3dEdgeSmoothingEnabled() const;
	virtual void Enable3dEdgeSmoothing(
		BOOL bEnable = TRUE); // <combine CXTPChartSeriesStyle::Is3dEdgeSmoothingEnabled@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get/set the transparency of the chart if it is
	//     supported by the chart type.
	// Parameters:
	//     nTransparency - An integer value denoting the transparency of the chart with
	//                     a range from 0-255.
	// Returns:
	//     An integer value denoting the transparency of the chart with a range from 0-255.
	//-----------------------------------------------------------------------
	virtual int GetTransparency() const;
	virtual void SetTransparency(int nTransparency); // <combine
													 // CXTPChartSeriesStyle::GetTransparency@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      Provides default implementation for axis side margins correction.
	// Parameters:
	//      pAxis - Axis pointer.
	//      nMinValue - Minimal acceptable value.
	//      nMaxValue - Maximal acceptable value.
	//      nCorrection - Result new correction value.
	//-----------------------------------------------------------------------
	virtual void CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue, double nMaxValue,
										double& nCorrection);

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains associated style appearance pointer.
	// Returns:
	//      Style appearance pointer
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesStyleAppearance* GetStyleAppearance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Applies the style to the command provided in case the command
	//      needs style specific details for correct drawing.
	// Parameters:
	//      pCommand - Command pointer for which the style is to be applied.
	//-----------------------------------------------------------------------
	virtual void ApplyTo(CXTPChartDeviceCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the index of a point element for which an internal value
	//      can be otained.
	// Returns:
	//      The index of a point element for which an internal value
	//      can be otained
	//-----------------------------------------------------------------------
	virtual int GetSeriesPointValueIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called to create a new view associated with the current style.
	// Parameters:
	//     pSeries - Series associated with the style.
	//     pDiagramView - Parent diagram view.
	// Returns:
	//     Pointer to a new CXTPChartSeriesView object.
	//----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView) = 0;

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartSeriesStyle);

	afx_msg void OleChartChanged();
	afx_msg LPDISPATCH OleGetLabel();
	afx_msg LPDISPATCH OleGetErrorBar();
	afx_msg BOOL OleGetColorEach();
	afx_msg void OleSetColorEach(BOOL bColorEach);
	afx_msg OLE_COLOR OleGetColor();
	afx_msg void OleSetColor(OLE_COLOR clr);
	afx_msg LPDISPATCH OleGet3dRotation();
	afx_msg void OleSet3dRotation(LPDISPATCH pRotationDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	friend class CXTPChartSeries;

	CXTPChartSeriesLabel* m_pLabel; // Label object
	CXTPChartErrorBar* m_pErrorBar; // Error bar object

	CXTPChartColor m_clrColor; // Custom color of series

	BOOL m_bColorEach; // TRUE to color each point.

	int m_nTransparency; // The transparency value in the range 0..255.

	CXTPChart3dRotation m_Rotation3d; // 3D rotation
	BOOL m_bSmooth3dEdges;			  // 3D edge smoothing
};

AFX_INLINE CXTPChartDiagram* CXTPChartSeriesStyle::CreateDiagram()
{
	return NULL;
}

AFX_INLINE CXTPChartSeriesLabel* CXTPChartSeriesStyle::GetLabel() const
{
	return m_pLabel;
}

AFX_INLINE CXTPChartErrorBar* CXTPChartSeriesStyle::GetErrorBar() const
{
	return m_pErrorBar;
}

AFX_INLINE CXTPChartColor CXTPChartSeriesStyle::GetColor() const
{
	return m_clrColor;
}

AFX_INLINE void CXTPChartSeriesStyle::SetColor(CXTPChartColor clr)
{
	m_clrColor = clr;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartSeriesStyle::IsColorEach() const
{
	return m_bColorEach;
}

AFX_INLINE void CXTPChartSeriesStyle::SetColorEach(BOOL bColorEach)
{
	m_bColorEach = bColorEach;
	OnChartChanged();
}

AFX_INLINE CXTPChart3dRotation CXTPChartSeriesStyle::Get3dRotation() const
{
	return m_Rotation3d;
}

AFX_INLINE void CXTPChartSeriesStyle::Set3dRotation(double dYaw, double dPitch, double dRoll)
{
	Set3dRotation(CXTPChart3dRotation(dYaw, dPitch, dRoll));
}

AFX_INLINE void CXTPChartSeriesStyle::Set3dRotation(const CXTPChart3dRotation& rotation)
{
	m_Rotation3d = rotation;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartSeriesStyle::Is3dEdgeSmoothingEnabled() const
{
	return m_bSmooth3dEdges;
}

AFX_INLINE void CXTPChartSeriesStyle::Enable3dEdgeSmoothing(BOOL bEnable /*= TRUE*/)
{
	m_bSmooth3dEdges = bEnable;
	OnChartChanged();
}

AFX_INLINE int CXTPChartSeriesStyle::GetTransparency() const
{
	return m_nTransparency;
}

AFX_INLINE void CXTPChartSeriesStyle::SetTransparency(int nTransparency)
{
	ASSERT("Transparency value must be within range [0..255]" && 0 <= nTransparency
		   && nTransparency <= 255);
	m_nTransparency = nTransparency;
	OnChartChanged();
}

AFX_INLINE int CXTPChartSeriesStyle::GetSeriesPointValueIndex() const
{
	return 0;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTSERIESSTYLE_H__)
