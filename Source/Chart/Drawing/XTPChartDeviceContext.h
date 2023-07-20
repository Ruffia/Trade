// XTPChartDeviceContext.h
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
#if !defined(__XTPCHARTDEVICECONTEXT_H__)
#	define __XTPCHARTDEVICECONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartContainer;
class CXTPChartDeviceCommand;
class CXTPMarkupUIElement;
class CXTPChartFont;
class CXTPChartDrawingObjectFactory;
class CXTPOpenGLTriangleRenderList;

//===========================================================================
// Summary:
//     This class abstracts a device context. A device context stores,
//     retrieves, and modifies the attributes of graphic objects and
//     specifies graphic modes. This class is a kind of CXTPCmdTarget to
//     enhance the MFC command routing.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDeviceContext : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPChartDeviceContext);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDeviceContext object.
	// Parameters:
	//     pContainer - Parent container pointer.
	//     hDC - Handle to the windows device context.
	//     bIs3D - TRUE identifies device context used for 3D rendering.
	//     pDrawingObjectFactory - Drawing object factory pointer.
	// See also:
	//     CXTPChart2dDrawingObjectFactory, CXTPChart3dDrawingObjectFactory
	//-----------------------------------------------------------------------
	CXTPChartDeviceContext(CXTPChartContainer* pContainer, HDC hDC, BOOL bIs3D,
						   CXTPChartDrawingObjectFactory* pDrawingObjectFactory);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDeviceContext object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartDeviceContext() = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Initializes devices context.
	// Returns:
	//     TRUE if initialization is successful.
	//-------------------------------------------------------------------------
	virtual BOOL Initialize();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to trigger the drawing.
	// Parameters:
	//     pCommand - A pointer to chart device command object.
	//-------------------------------------------------------------------------
	virtual void Execute(CXTPChartDeviceCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the derived device context is a 3D device context.
	// Returns:
	//      TRUE if the derived device context is a 3D device context, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL Is3D() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the windows device context.
	// Returns:
	//     A handle to the windows device context.
	//-------------------------------------------------------------------------
	HDC GetHDC() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Provides access to the parent container object.
	// Returns:
	//      Parent container object pointer.
	//-----------------------------------------------------------------------
	CXTPChartContainer* GetContainer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Provides access to the drawing object factory used by
	//      the device context implementation.
	// Returns:
	//      Drawing object factory instance pointer.
	//-----------------------------------------------------------------------
	CXTPChartDrawingObjectFactory* GetDrawingObjectFactory() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the measurements of texts rendered in a
	//     device context using a specific font.
	// Parameters:
	//     pText - The string to be measured.
	//     pFont - Pointer to chart font object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSizeF MeasureString(const CXTPChartString* pText, CXTPChartFont* pFont) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//      Measures the markup element size provided with the font set as the
	//      default font for the associated markup context.
	// Parameters:
	//      pMarkupUIElement - Markup element pointer to measure.
	//      pFont - A default markup context font pointer.
	// Returns:
	//      Measured markup element size or empty size in case of an error.
	//-----------------------------------------------------------------------
	virtual CXTPChartSizeF MeasureMarkupElement(CXTPMarkupUIElement* pMarkupUIElement,
												CXTPChartFont* pFont) = 0;

protected:
	const BOOL m_bIs3D;				  // TRUE identifies device context used for 3D rendering
	HDC m_hDC;						  // The windows device context.
	CXTPChartContainer* m_pContainer; // Parent container pointer.
	CXTPChartDrawingObjectFactory* m_pDrawingObjectFactory; // Drawing object factory pointer.
};

AFX_INLINE BOOL CXTPChartDeviceContext::Is3D() const
{
	return m_bIs3D;
}

AFX_INLINE HDC CXTPChartDeviceContext::GetHDC() const
{
	return m_hDC;
}

AFX_INLINE CXTPChartContainer* CXTPChartDeviceContext::GetContainer() const
{
	return m_pContainer;
}

AFX_INLINE CXTPChartDrawingObjectFactory* CXTPChartDeviceContext::GetDrawingObjectFactory() const
{
	return m_pDrawingObjectFactory;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTDEVICECONTEXT_H__)
