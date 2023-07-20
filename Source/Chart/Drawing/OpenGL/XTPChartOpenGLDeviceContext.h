// XTPChartOpenGLDeviceContext.h
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
#if !defined(__XTPCHARTOPENGLDEVICECONTEXT_H__)
#	define __XTPCHARTOPENGLDEVICECONTEXT_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

template<class T>
class CXTPMatrix;
class CXTPChartContainer;
class CXTPChartDeviceCommand;

class _XTP_EXT_CLASS CXTPChartOpenGLDeviceContext : public CXTPChart3dDeviceContext
{
	DECLARE_DYNAMIC(CXTPChartOpenGLDeviceContext);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartOpenGLDeviceContext object.
	// Parameters:
	//     hDC - Handle to the windows device context.
	//-----------------------------------------------------------------------
	CXTPChartOpenGLDeviceContext(CXTPChartContainer* pContainer, HDC hDC, CRect rcBounds);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartOpenGLDeviceContext object, handles cleanup.
	//-------------------------------------------------------------------------
	~CXTPChartOpenGLDeviceContext();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to trigger the drawing.
	// Parameters:
	//     pCommand - A pointer to chart device command object.
	//-------------------------------------------------------------------------
	virtual void Execute(CXTPChartDeviceCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//      Initializes device context.
	//-----------------------------------------------------------------------
	virtual BOOL Initialize();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the measurements of texts rendered in a
	//     device context using a specific font.
	// Parameters:
	//     pText - The string to be measured.
	//     pFont - Pointer to chart font object.
	// Returns:
	//     Measured string size.
	//-------------------------------------------------------------------------
	virtual CXTPChartSizeF MeasureString(const CXTPChartString* pText, CXTPChartFont* pFont);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the measurements of a markup element rendered in a
	//     device context using a specific font.
	// Parameters:
	//     pText - The string to be measured.
	//     pFont - Pointer to chart font object.
	// Returns:
	//     Measured element size.
	//-------------------------------------------------------------------------
	virtual CXTPChartSizeF MeasureMarkupElement(CXTPMarkupUIElement* pMarkupUIElement,
												CXTPChartFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//      Rotates current matrix.
	// Parameters:
	//      rotation - Matrix rotation arguments.
	//-----------------------------------------------------------------------
	virtual void Rotate(const CXTPChart3dRotation& rotation);

	//-----------------------------------------------------------------------
	// Summary:
	//      Projects 3D coordinates to 2D space point coordinates.
	// Parameters:
	//      pt3d - 3D space point coordinates.
	//      pt2d - Result 2D space point coordinates.
	//      pdWinZ - If specified the result value is within range 0..1
	//               where 0 defines the near plane and 1 defines the far
	//               plane defined by the view frustum.
	//      box - 3D box to map to the outer 2D rectangle.
	//      rc - Resuylt 2D outer rectangle for the 3D box specified.
	// Returns:
	//      TRUE if coordinates are mapped successfully, FALSE otherwise.
	// See also:
	//      Unproject
	//-----------------------------------------------------------------------
	virtual BOOL Project(const CXTPPoint3d& pt3d, CPoint& pt2d, double* pdWinZ = NULL);
	virtual BOOL Project(const CXTPChart3dBox& box,
						 CRect& rc); // <combine CXTPChartOpenGLDeviceContext::Project@const
									 // CXTPPoint3d&@CPoint&@double*>

	//-----------------------------------------------------------------------
	// Summary:
	//      Projects 2D space point coordinates to 3D space point coordinate
	//      on the specified plane.
	// Parameters:
	//      pt2d - 2D space point coordinates.
	//      pt3d - Result 3D space point coordinates.
	//      dWinZ - Determines how far the point is from the near plane.
	//              The value must be within 0..1 range where 0 determines
	//              the near frustum plane and 1 determines the far plane.
	// Returns:
	//      TRUE is the coordinates are mapped successfully, FALSE otherwise.
	// See also:
	//      Project
	//-----------------------------------------------------------------------
	virtual BOOL Unproject(const CPoint& pt2d, CXTPPoint3d& pt3d, double dWinZ = 0.);

	//-----------------------------------------------------------------------
	// Summary:
	//      Transforms vector or box coordinates.
	// Parameters:
	//      v - A vector to transform.
	//      box - A box to transform.
	//      matrix - OpenGL matrix to be used for vector transformation.
	//      pTranslation - Optional translation offsets to add to the vector or box vertices.
	//      pRotation - Optional vector opr box vertices rotation parameters.
	//-----------------------------------------------------------------------
	virtual void Transform(CXTPPoint3d& v, const CXTPPoint3d* pTranslation = NULL,
						   const CXTPChart3dRotation* pRotation = NULL);
	virtual void Transform(CXTPChart3dBox& box, const CXTPPoint3d* pTranslation = NULL,
						   const CXTPChart3dRotation* pRotation =
							   NULL); // <combine
									  // CXTPChartOpenGLDeviceContext::Transform@CXTPPoint3d&@const
									  // CXTPPoint3d*@const CXTPChart3dRotation*>
	virtual void Transform(CXTPPoint3d& v,
						   const CXTPMatrix<double>&
							   matrix); // <combine
										// CXTPChartOpenGLDeviceContext::Transform@CXTPPoint3d&@const
										// CXTPPoint3d*@const CXTPChart3dRotation*>
	virtual void Transform(CXTPChart3dBox& box,
						   const CXTPMatrix<double>&
							   matrix); // <combine
										// CXTPChartOpenGLDeviceContext::Transform@CXTPPoint3d&@const
										// CXTPPoint3d*@const CXTPChart3dRotation*>

	//-----------------------------------------------------------------------
	// Summary:
	//      Performs depth test for 2D point provided.
	// Parameters:
	//      point - Viewport offset for which depth test is to be performed.
	//      dWinZ - If succeedes the value contains the distance from the near (0)
	//              plane of the view frustum to the fist visible point.
	// Returns:
	//      TRUE if depth test is successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL DepthTest(CPoint point, double& dWinZ);

	//-----------------------------------------------------------------------
	// Summary:
	//      Clamps transparency value in range [0..255] used in chart style
	//      to OpenGL alpha value in the range [0..1].
	// Parameters:
	//      nTransparency - Transparency value in the range [0..255].
	// Returns:
	//      OpenGL alpha value.
	//-----------------------------------------------------------------------
	static float ClampTransparency(int nTransparency);

private:
	BOOL InitializePixelFormat();
	void SetOpenGLParameters();
	BOOL OnPreRender();
	void OnPostRender();
	double ComputeAntialiasingJitterFactor() const;
	void Execute3dCommandTree(CXTPChartDeviceCommand* pCommand);
	void Execute2dCommandTree(CXTPChartDeviceCommand* pCommand);
	CXTPMatrix<double>* BuildTransformationMatrix(const CXTPPoint3d* pTranslation	  = NULL,
												  const CXTPChart3dRotation* pRotation = NULL);
	void Render3dCommand(CXTPChartDeviceCommand* pCommand, double& dJitterFactor,
						 BOOL bAntialiased = FALSE);
	void RenderTriangles(double& dJitterFactor, BOOL bAntialiased = FALSE);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	HGLRC m_hGLContext;
	int m_nExecuteNesting;
	CList<CXTPChartDeviceCommand*, CXTPChartDeviceCommand*> m_2dCommands;
	BOOL m_bHave2dCommands;
	BOOL m_bIn2dCommandExecutionMode;
	CXTPChartDeviceContext* m_p2dDC;

	static const CXTPPoint3d m_arJitter[];
	static const float m_fJitterAccumStep;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE float CXTPChartOpenGLDeviceContext::ClampTransparency(int nTransparency)
{
	ASSERT(0 <= nTransparency && nTransparency <= 255);
	return (1.f - static_cast<float>(nTransparency) / 255);
}
#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTOPENGLDEVICECONTEXT_H__)
//}}AFX_CODEJOCK_PRIVATE
