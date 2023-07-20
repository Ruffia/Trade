// XTPOpenGLUtils.h
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
#if !defined(__XTPOPENGLUTILS_H__)
#	define __XTPOPENGLUTILS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

template<class T>
class CXTPMatrix;

//===========================================================================
// Summary:
//     Provides a set of common OpenGL utilities.
//===========================================================================
class _XTP_EXT_CLASS CXTPOpenGLUtils
{
public:
	//{{AFX_CODEJOCK_PRIVATE
#	ifdef _DEBUG
	// Debugging
	static void DebugAxes(double dSize);
	static void DebugBox(const CXTPPoint3d& ptMin, const CXTPPoint3d& ptMax, BOOL bSolid = FALSE);
	static void DebugNormal(const CXTPPoint3d& a, const CXTPPoint3d& b, const CXTPPoint3d& c,
							const CXTPPoint3d& n, double dLen = 5.);
	static void DebugNormal(const CXTPPoint3d& v, const CXTPPoint3d& n, double dLen = 5.);
#	endif /*_DEBUG*/
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a vertex from point coordinates.
	// Parameters:
	//     ptVertex - Vertex coordinates.
	//-----------------------------------------------------------------------
	static void Vertex(const CXTPPoint3d& ptVertex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a vertex and its normal from point coordinates.
	// Parameters:
	//     ptVertex - Vertex coordinates.
	//     vNormal  - Normal coordinates.
	//-----------------------------------------------------------------------
	static void Vertex(const CXTPPoint3d& ptVertex, const CXTPPoint3d& vNormal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Rotates the active matrix using Euler angles.
	// Parameters:
	//     dYaw   - Yaw angle value in degrees.
	//     dPitch - Pitch angle value in degrees.
	//     dRoll  - Roll angle value in degrees.
	//-----------------------------------------------------------------------
	static void Rotate(double dYaw, double dPitch, double dRoll);

	//-----------------------------------------------------------------------
	// Summary:
	//     Projects 3D space coordinates between the near and far planes to 
	//     2D space coordinates of the current viewport.
	// Parameters:
	//     pt3d   - Reference to a CPoint object containing the 
	//              3D space coordinates between the near and far planes.
	//     pt2d   - Reference to a CPoint object to receive the resulting
	//              2D space point coordinates of the current viewport.
	//     pdWinZ - Z-plane coordinate of the specified 3D point (i.e. the position 
	//              between the near plane (0) and the far plane (1)); can be NULL.
	// Returns:
	//     TRUE if the coordinates are mapped successfully.
	// See also:
	//     Unproject
	//-----------------------------------------------------------------------
	static BOOL Project(const CXTPPoint3d& pt3d, CPoint& pt2d, double* pdWinZ = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unprojects 2D space coordinates of the current viewport to
	//     3D space coordinates between the near and far planes.
	// Parameters:
	//     pt2d  - Reference to a CPoint object containing the 
	//             2D space coordinates of the current viewport.
	//     pt3d  - Reference to a CPoint object to receive the resulting
	//             3D space coordinates between the near and far planes.
	//     dWinZ - Z-plane coordinate of the specified 3D point (i.e. the position 
	//             between the near plane (0) and the far plane (1)).
	// Returns:
	//     TRUE if the coordinates are mapped successfully.
	// See also:
	//     Project
	//-----------------------------------------------------------------------
	static BOOL Unproject(const CPoint& pt2d, CXTPPoint3d& pt3d, double dWinZ = 0.);

	//-----------------------------------------------------------------------
	// Summary: 
	//     Multiplies a vector by a specified matrix. 
	// Parameters: 
	//     v      - Vector to multiply by the matrix. 
	//     matrix - Matrix by which the vector is multiplied.
	//-----------------------------------------------------------------------
	static void MultiplyByMatrix(CXTPPoint3d& v, const CXTPMatrix<double>& matrix);

	//-----------------------------------------------------------------------
	// Summary: 
	//     Multiplies a vector by a specified OpenGL matrix. 
	// Parameters:
	//     v    - Vector to multiply by the OpenGL matrix. 
	//     name - Name of an OpenGL matrix by which the vector is multiplied. 
	//-----------------------------------------------------------------------
	static void MultiplyByMatrix(CXTPPoint3d& v, GLenum name);

	//-----------------------------------------------------------------------
	// Summary:
	//     Computes a normal for a specified triangle.
	// Parameters:
	//     vA  - 1st triangle point.
	//     vB  - 2nd triangle point.
	//     vC  - 3rd triangle point.
	//     ccw - Determines which side of the triangle that the normal vector
	//           will be directed from. By default, triangle points are assumed
	//           to be in clockwise order which determines the front face of
	//           the triangle for which the normal is computed. In order to
	//           reverse a normal vector, this value has to be false which means
	//           counter-clockwise vertex unwinding. 
	// Returns:
	//     Computed normal vector.
	//-----------------------------------------------------------------------
	static CXTPPoint3d ComputeTriangleNormal(const CXTPPoint3d& vA, const CXTPPoint3d& vB,
											 const CXTPPoint3d& vC, bool ccw = false);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPOPENGLUTILS_H__)
