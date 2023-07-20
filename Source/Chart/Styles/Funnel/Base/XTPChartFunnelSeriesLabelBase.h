// XTPChartFunnelSeriesLabelBase.h
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
#if !defined(__XTPCHARTFUNNELSERIESLABELBASE_H__)
#	define __XTPCHARTFUNNELSERIESLABELBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartFunnelSeriesPointView;
class CXTPChartFunnelSeriesStyleBase;

//===========================================================================
// Summary:
//     This enumeration tells the various positions available for the funnel label.
//===========================================================================
enum XTPChartFunnelLabelPosition
{
	xtpChartFunnelLabelLeft,
	xtpChartFunnelLabelRight,
	xtpChartFunnelLabelCenter,
};

//===========================================================================
// Summary:
//     CXTPChartFunnelSeriesLabelBase is a kind of CXTPChartSeriesLabel. This class
//     abstracts the label of a funnel series.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesLabelBase : public CXTPChartSeriesLabel
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesLabelBase)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesLabelBase object.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesLabelBase();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFunnelSeriesLabelBase object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFunnelSeriesLabelBase();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the label position.
	// Returns:
	//     A funnel label position object describing the position of the funnel label.
	//-------------------------------------------------------------------------
	XTPChartFunnelLabelPosition GetPosition() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the label position.
	// Parameters:
	//     nPosition - A funnel label position object describing the position of
	//                 the funnel label.
	//-------------------------------------------------------------------------
	void SetPosition(XTPChartFunnelLabelPosition nPosition);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the basic style of the funnel series.
	// Returns:
	//     A pointer to a CXTPChartFunnelSeriesStyle object.
	//-------------------------------------------------------------------------
	CXTPChartFunnelSeriesStyleBase* GetStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the label is inside the funnel.
	// Returns:
	//     A BOOL value. TRUE if the label is inside the funnel, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL IsInside() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view of the funnel series.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pPointView - The series point view.
	// Returns:
	//     Returns a pointer to the associated label view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartSeriesPointView* pPointView,
											 CXTPChartElementView* pParentView) = 0;

	XTPChartFunnelLabelPosition m_nPosition; // The funnel label position.

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPunnelSeriesLabelBase);
	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE XTPChartFunnelLabelPosition CXTPChartFunnelSeriesLabelBase::GetPosition() const
{
	return m_nPosition;
}
AFX_INLINE void CXTPChartFunnelSeriesLabelBase::SetPosition(XTPChartFunnelLabelPosition nPosition)
{
	m_nPosition = nPosition;
	OnChartChanged();
}

//===========================================================================
// Summary:
//      This class abstracts the view of a funnel series label.
//===========================================================================
class CXTPChartFunnelSeriesLabelViewBase : public CXTPChartSeriesLabelView
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesLabelViewBase);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs a CXTPChartFunnelSeriesLabelViewBase object.
	// Parameters:
	//      pLabel - Associated label model pointer.
	//      pPointView - Point view object pointer associated with the label.
	//      pParentView - Parent view pointer.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesLabelViewBase(CXTPChartSeriesLabel* pLabel,
									   CXTPChartSeriesPointView* pPointView,
									   CXTPChartElementView* pParentView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains label position.
	// Returns:
	//      Label position.
	//-----------------------------------------------------------------------
	XTPChartFunnelLabelPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Creates view drawing device command for the device context specified.
	// Parameters:
	//      pDC - Device context pointer for which the drawing command is to be created.
	// Returns:
	//      A new instance of view drawing device command or NULL if no command
	//      is provided.
	//-----------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC) = 0;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTFUNNELSERIESLABELBASE_H__)
