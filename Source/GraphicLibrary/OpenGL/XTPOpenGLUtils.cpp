// XTPOpenGLUtils.cpp
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

#include "stdafx.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <float.h>
#include <math.h>
#include "GraphicLibrary/OpenGL/GL.h"
#include "GraphicLibrary/OpenGL/GLU.h"

#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Math/XTPMathUtils.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Math/XTPMatrix.h"

#include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

#ifdef _DEBUG

///////////////////////////////////////////////////////////////////////////////
// Debugging

void CXTPOpenGLUtils::DebugAxes(double dSize)
{
	glPushAttrib(GL_CURRENT_BIT);

	glBegin(GL_LINES);

	// Draw axes
	glColor3f(1, 0, 0);
	glVertex3d(-dSize, 0, 0);
	glVertex3d(dSize, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3d(0, -dSize, 0);
	glVertex3d(0, dSize, 0);

	glColor3f(0, 0, 1);
	glVertex3d(0, 0, -dSize);
	glVertex3d(0, 0, dSize);

	// Draw positive plane
	glColor3f(1, 0, 0);
	glVertex3d(dSize, 0, 0);
	glVertex3d(0, dSize, 0);

	glColor3f(0, 1, 0);
	glVertex3d(0, dSize, 0);
	glVertex3d(0, 0, dSize);

	glColor3f(0, 0, 1);
	glVertex3d(0, 0, dSize);
	glVertex3d(dSize, 0, 0);

	glEnd();

	glPopAttrib();
}

void CXTPOpenGLUtils::DebugBox(const CXTPPoint3d& ptMin, const CXTPPoint3d& ptMax,
							   BOOL bSolid /*= FALSE*/)
{
	glPushAttrib(GL_CURRENT_BIT);

	glColor4f(.8f, .8f, .8f, .25f);

	if (!bSolid)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3d(ptMin.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMin.Z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex3d(ptMin.X, ptMin.Y, ptMax.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMax.Z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3d(ptMin.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMin.Y, ptMax.Z);

		glVertex3d(ptMin.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMax.Z);

		glVertex3d(ptMax.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMax.Z);

		glVertex3d(ptMax.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMax.Z);
		glEnd();
	}
	else
	{
		// Top face
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		glVertex3d(ptMin.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMin.Z);
		glEnd();

		// Bottom face
		glBegin(GL_QUADS);
		glNormal3d(0, -1, 0);
		glVertex3d(ptMin.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMin.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMin.Z);
		glEnd();

		// Back face
		glBegin(GL_QUADS);
		glNormal3d(0, 0, -1);
		glVertex3d(ptMin.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMin.Z);
		glEnd();

		// Front face
		glBegin(GL_QUADS);
		glNormal3d(0, 0, 1);
		glVertex3d(ptMin.X, ptMin.Y, ptMax.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMax.Z);
		glEnd();

		// Left face
		glBegin(GL_QUADS);
		glNormal3d(-1, 0, 0);
		glVertex3d(ptMin.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMin.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMin.X, ptMin.Y, ptMax.Z);
		glEnd();

		// Right face
		glBegin(GL_QUADS);
		glNormal3d(1, 0, 0);
		glVertex3d(ptMax.X, ptMin.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMin.Z);
		glVertex3d(ptMax.X, ptMax.Y, ptMax.Z);
		glVertex3d(ptMax.X, ptMin.Y, ptMax.Z);
		glEnd();
	}

	glPopAttrib();
}

void CXTPOpenGLUtils::DebugNormal(const CXTPPoint3d& a, const CXTPPoint3d& b, const CXTPPoint3d& c,
								  const CXTPPoint3d& n, double dLen /*= 5.*/)
{
	CXTPPoint3d ptCenter((a.X + b.X + c.X) / 3, (a.Y + b.Y + c.Y) / 3, (a.Z + b.Z + c.Z) / 3);
	DebugNormal(ptCenter, n, dLen);
}

void CXTPOpenGLUtils::DebugNormal(const CXTPPoint3d& v, const CXTPPoint3d& n, double dLen /*= 5.*/)
{
	CXTPPoint3d normal = n;
	double l		   = sqrt(n.X * n.X + n.Y * n.Y + n.Z * n.Z);
	if (0 != l)
	{
		normal.X /= l;
		normal.Y /= l;
		normal.Z /= l;
	}

	normal.X *= dLen;
	normal.Y *= dLen;
	normal.Z *= dLen;

	glPushAttrib(GL_CURRENT_BIT);

	glColor4f(0, 0, 0, .5f);

	glBegin(GL_LINES);
	glVertex3d(v.X, v.Y, v.Z);
	glVertex3d(v.X + normal.X, v.Y + normal.Y, v.Z + normal.Z);
	glEnd();

	glPopAttrib();
}

#endif /*_DEBUG*/

///////////////////////////////////////////////////////////////////////////////
// Primitives

void CXTPOpenGLUtils::Vertex(const CXTPPoint3d& ptVertex)
{
	glVertex3d(ptVertex.X, ptVertex.Y, ptVertex.Z);
}

void CXTPOpenGLUtils::Vertex(const CXTPPoint3d& ptVertex, const CXTPPoint3d& vNormal)
{
	glNormal3d(vNormal.X, vNormal.Y, vNormal.Z);
	Vertex(ptVertex);
}

///////////////////////////////////////////////////////////////////////////////
// Math

CXTPPoint3d CXTPOpenGLUtils::ComputeTriangleNormal(const CXTPPoint3d& vA, const CXTPPoint3d& vB,
												   const CXTPPoint3d& vC, bool ccw /*= false*/)
{
	CXTPPoint3d vNormal;
	CXTPMathUtils::ComputeTriangleNormal(vA.X, vA.Y, vA.Z, vB.X, vB.Y, vB.Z, vC.X, vC.Y, vC.Z,
										 vNormal.X, vNormal.Y, vNormal.Z,
										 !glIsEnabled(GL_NORMALIZE), ccw);
	return vNormal;
}

///////////////////////////////////////////////////////////////////////////////
// Matrix operations

void CXTPOpenGLUtils::Rotate(double dYaw, double dPitch, double dRoll)
{
	if (0 != dYaw)
	{
		glRotated(dYaw, 0, 1, 0);
	}

	if (0 != dPitch)
	{
		glRotated(dPitch, 0, 0, 1);
	}

	if (0 != dRoll)
	{
		glRotated(dRoll, 1, 0, 0);
	}
}

BOOL CXTPOpenGLUtils::Project(const CXTPPoint3d& pt3d, CPoint& pt2d, double* pdWinZ /*= NULL*/)
{
	BOOL bSuccess = FALSE;

	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	GLdouble projMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	CXTPPoint3d ptWnd;
	if (GL_TRUE
		== gluProject(pt3d.X, pt3d.Y, pt3d.Z, modelMatrix, projMatrix, viewport, &ptWnd.X, &ptWnd.Y,
					  &ptWnd.Z))
	{
		pt2d.x = static_cast<int>(ptWnd.X);
		pt2d.y = viewport[3] - static_cast<int>(ptWnd.Y);

		if (NULL != pdWinZ)
		{
			*pdWinZ = ptWnd.Z;
		}

		bSuccess = TRUE;
	}

	return bSuccess;
}

BOOL CXTPOpenGLUtils::Unproject(const CPoint& pt2d, CXTPPoint3d& pt3d, double dWinZ /*= 0.*/)
{
	BOOL bSuccess = FALSE;

	GLdouble modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	GLdouble projMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	CXTPPoint3d ptWnd;
	bSuccess = (GL_TRUE
				== gluUnProject(pt2d.x, viewport[3] - pt2d.y, dWinZ, modelMatrix, projMatrix,
								viewport, &pt3d.X, &pt3d.Y, &pt3d.Z));

	return bSuccess;
}

void CXTPOpenGLUtils::MultiplyByMatrix(CXTPPoint3d& v, const CXTPMatrix<double>& matrix)
{
	CXTPMatrix<double> in(1, 4);
	in.SetAt(0, 0, v.X);
	in.SetAt(0, 1, v.Y);
	in.SetAt(0, 2, v.Z);
	in.SetAt(0, 3, 1.);

	CXTPMatrix<double>* pResult = matrix.Multiply(in);
	v.X							= pResult->GetAt(0, 0);
	v.Y							= pResult->GetAt(0, 1);
	v.Z							= pResult->GetAt(0, 2);
	delete pResult;
}

void CXTPOpenGLUtils::MultiplyByMatrix(CXTPPoint3d& v, GLenum name)
{
	CXTPMatrix<double> matrix(4, 4);
	glGetDoublev(name, matrix.GetData());
	MultiplyByMatrix(v, matrix);
}
