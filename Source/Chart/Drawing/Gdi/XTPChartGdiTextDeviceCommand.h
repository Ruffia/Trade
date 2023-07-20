// XTPChartGdiTextDeviceCommand.h
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
#if !defined(__XTPCHARTGDITEXTDEVICECOMMAND_H__)
#	define __XTPCHARTGDITEXTDEVICECOMMAND_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//     This class represents a text device command, which is a kind of CXTPChartDeviceCommand.
//     This class handles the smoothing of text in a chart and does so by using antialising.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGdiTextAntialiasingDeviceCommand
	: public CXTPChartTextAntialiasingDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiTextAntialiasingDeviceCommand object.
	// Parameters:
	//     bAntialias   - TRUE to enable antialiasing, FALSE to disable antialiasing.
	//                    The default value for bAntialias is TRUE.
	//-----------------------------------------------------------------------
	CXTPChartGdiTextAntialiasingDeviceCommand(BOOL bAntialias = TRUE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called before a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC   - The chart device context.
	//-----------------------------------------------------------------------
	void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called after a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC   - The chart device context.
	//-----------------------------------------------------------------------
	void AfterExecute(CXTPChartDeviceContext* pDC);
};

//===========================================================================
// Summary:
//     This class represents a bounded text device command, which is a kind of
//     CXTPChartDeviceCommand. It handles the rendering of bounded texts in a chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGdiBoundedTextDeviceCommand : public CXTPChartBoundedTextDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiBoundedTextDeviceCommand object.
	// Parameters:
	//     strText   - The text to be rendered.
	//     pFont     - The font used to render.
	//     color     - The color of the text.
	//     rectangle - The text bounds.
	//-----------------------------------------------------------------------
	CXTPChartGdiBoundedTextDeviceCommand(const CXTPChartString& strText, CXTPChartFont* pFont,
										 const CXTPChartColor& color,
										 const CXTPChartRectF& rectangle);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function does the actual drawing of the chart element to which
	//     this device command is associated with. Here it renders the text inside
	//     a rectangle.
	// Parameters:
	//     pDC      - The device context of the chart.
	//-----------------------------------------------------------------------
	virtual void ExecuteInternal(CXTPChartDeviceContext* pDC);
};

class _XTP_EXT_CLASS CXTPChartGdiMarkupElementDeviceCommand
	: public CXTPChartMarkupElementDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiMarkupElementDeviceCommand object.
	// Parameters:
	//     strText   - The text to be rendered.
	//     pFont     - The font used to render.
	//     color     - The color of the text.
	//     rectangle - The text bounds.
	//-----------------------------------------------------------------------
	CXTPChartGdiMarkupElementDeviceCommand(CXTPMarkupUIElement* pMarkupUIElement,
										   CXTPChartFont* pFont, const CXTPChartColor& color,
										   const CXTPChartRectF& rectangle);

public:
	virtual void ExecuteOverride(CXTPChartDeviceContext* pDC);

	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTGDITEXTDEVICECOMMAND_H__)
//}}AFX_CODEJOCK_PRIVATE
