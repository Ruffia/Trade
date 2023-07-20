// XTPChartGdiDeviceContext.h
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
#if !defined(__XTPCHARTGDIDEVICECONTEXT_H__)
#	define __XTPCHARTGDIDEVICECONTEXT_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace Gdiplus
{
class GpGraphics;
class GpFont;
class Graphics;
} // namespace Gdiplus

class CXTPChartContainer;
class CXTPChartDeviceCommand;

class _XTP_EXT_CLASS CXTPChartGdiDeviceContext : public CXTPChartDeviceContext
{
	DECLARE_DYNAMIC(CXTPChartGdiDeviceContext);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiDeviceContext object.
	// Parameters:
	//     pContainer - Pointer to a CXTPChartContainer object.
	//     hDC        - Handle to the windows device context.
	//     p3dDC      - Pointer to a CXTPChartDeviceContext object.
	//     Graphics   - Pointer to a GDI+ graphics object.
	//-----------------------------------------------------------------------
	CXTPChartGdiDeviceContext(CXTPChartContainer* pContainer, HDC hDC,
							  CXTPChartDeviceContext* p3dDC = NULL);
	CXTPChartGdiDeviceContext(
		CXTPChartContainer* pContainer, Gdiplus::Graphics*,
		HDC hDC); // <combine
				  // CXTPChartGdiDeviceContext::CXTPChartGdiDeviceContext@CXTPChartContainer*@HDC@CXTPChartDeviceContext*>

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartGdiDeviceContext object, handles cleanup.
	//-------------------------------------------------------------------------
	~CXTPChartGdiDeviceContext();

	//-------------------------------------------------------------------------
	// Summary:
	//     Obsolete.
	//-------------------------------------------------------------------------
	static void AFX_CDECL Register(BOOL bReserved);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to trigger the drawing.
	// Parameters:
	//     pCommand - A pointer to a chart device command object.
	//-------------------------------------------------------------------------
	virtual void Execute(CXTPChartDeviceCommand* pCommand);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the GDI+ graphics class.
	// Returns:
	//     A pointer to a GpGraphics object.
	//-------------------------------------------------------------------------
	Gdiplus::GpGraphics* GetGraphics() const;

	virtual BOOL GetNativeDrawing() const;

	virtual void SetNativeDrawing(BOOL bNativeDrawing);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the measurements of texts rendered in a
	//     device context using a specific font.
	// Parameters:
	//     pText - The string to be measured.
	//     pFont - Pointer to a chart font object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSizeF MeasureString(const CXTPChartString* pText, CXTPChartFont* pFont);

	static void AFX_CDECL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

	virtual CXTPChartSizeF MeasureMarkupElement(CXTPMarkupUIElement* pMarkupUIElement,
												CXTPChartFont* pFont);

protected:
	ULONG_PTR m_nGdiplusToken;			// The GDI+ token.
	Gdiplus::GpGraphics* m_pGpGraphics; // Pointer to GDI+ graphics object.
	CXTPChartDeviceContext* m_p3dDC;	// Optional 3D DC where all nested 3D commands has to be
										// executed
};

AFX_INLINE Gdiplus::GpGraphics* CXTPChartGdiDeviceContext::GetGraphics() const
{
	return m_pGpGraphics;
}
AFX_INLINE BOOL CXTPChartGdiDeviceContext::GetNativeDrawing() const
{
	return TRUE;
}
AFX_INLINE void CXTPChartGdiDeviceContext::SetNativeDrawing(BOOL /*bNativeDrawing*/)
{
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTGDIDEVICECONTEXT_H__)
//}}AFX_CODEJOCK_PRIVATE
