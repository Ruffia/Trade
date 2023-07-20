// XTPOpenGLTriangleRenderList.cpp
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

#include "GraphicLibrary/OpenGL/GL.h"
#include "GraphicLibrary/OpenGL/GLU.h"

#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Math/XTPMatrix.h"

#include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLTriangleRenderList.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLListPool.h"


#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// Experimental
//#define XTP_ENABLE_OPENGL_TRIANGLE_RENDER_LIST_SORTING

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPOpenGLTriangleRenderList::CXTPOpenGLTriangleRenderList()
{
	Reset();
}

CXTPOpenGLTriangleRenderList::~CXTPOpenGLTriangleRenderList()
{
	ASSERT(m_RenderStateQueue.IsEmpty());
	ASSERT(m_RenderColorQueue.IsEmpty());
	ClearTriangleList();
}

void CXTPOpenGLTriangleRenderList::ClearTriangleList()
{
	for (INT_PTR i = 0; i < m_list.GetSize(); ++i) {
		SAFE_DELETE(m_list[i]);
	}
	m_list.RemoveAll();
}

void CXTPOpenGLTriangleRenderList::SetCurrentMatrices()
{
	State currState;
	currState.nRefs = 0;
	glGetFloatv(GL_MODELVIEW_MATRIX, currState.mView);

	if (!m_states.IsEmpty())
	{
		const State& lastState = m_states.GetTail();
		if (0 == memcmp(lastState.mView, currState.mView, sizeof(currState.mView)))
			return;
	}

	m_states.AddTail(currState);

	m_bCommitted = FALSE;
}

void CXTPOpenGLTriangleRenderList::SetCurrentColor(float r, float g, float b, float a)
{
	Color currColor = { 0, r, g, b, a };
	if (!m_colors.IsEmpty())
	{
		const Color& lastColor = m_colors.GetTail();
		if (0 == memcmp(&lastColor, &currColor, sizeof(Color)))
			return;
	}

	m_colors.AddTail(currColor);

	m_bCommitted = FALSE;
}

void CXTPOpenGLTriangleRenderList::Add(const CXTPPoint3d& a, const CXTPPoint3d& an,
									   const CXTPPoint3d& b, const CXTPPoint3d& bn,
									   const CXTPPoint3d& c, const CXTPPoint3d& cn)
{
	ASSERT("SetCurrentMatrices must be called at least once" && !m_states.IsEmpty());
	ASSERT("SetCurrentColor must be called at least once" && !m_colors.IsEmpty());

	State* pState = &m_states.GetTail();
	Color* pColor = &m_colors.GetTail();

	double dDist = 0;
#ifdef XTP_ENABLE_OPENGL_TRIANGLE_RENDER_LIST_SORTING
	CXTPPoint3d ptCenter((a.X + b.X + c.X) / 3, (a.Y + b.Y + c.Y) / 3, (a.Z + b.Z + c.Z) / 3);

	ptCenter.X = CXTPMathUtils::ComputeVectorDotProduct(ptCenter.X, ptCenter.Y, ptCenter.Z,
														pState->mView[0], pState->mView[4],
														pState->mView[8]);
	ptCenter.Y = CXTPMathUtils::ComputeVectorDotProduct(ptCenter.X, ptCenter.Y, ptCenter.Z,
														pState->mView[1], pState->mView[5],
														pState->mView[9]);
	ptCenter.Z = CXTPMathUtils::ComputeVectorDotProduct(ptCenter.X, ptCenter.Y, ptCenter.Z,
														pState->mView[2], pState->mView[6],
														pState->mView[10]);
	dDist	   = CXTPMathUtils::ComputeVectorLength(ptCenter.X, ptCenter.Y, ptCenter.Z);
#endif

	Triangle* triangle = new Triangle();
	triangle->dDist	   = dDist;
	triangle->pState   = pState;
	triangle->pColor   = pColor;
	triangle->a.v	   = a;
	triangle->a.n	   = an;
	triangle->b.v	   = b;
	triangle->b.n	   = bn;
	triangle->c.v	   = c;
	triangle->c.n	   = cn;
	m_list.Add(triangle);

	m_bCommitted = FALSE;
}

void CXTPOpenGLTriangleRenderList::Commit()
{
	if (m_bCommitted)
		return;

	m_bCommitted = TRUE;

	INT_PTR nCount = m_list.GetSize();
	if (nCount <= 1)
		return;

#ifdef XTP_ENABLE_OPENGL_TRIANGLE_RENDER_LIST_SORTING
	qsort(m_list.GetData(), static_cast<size_t>(nCount), sizeof(void*), Compare);
#endif
}

void CXTPOpenGLTriangleRenderList::RenderAll()
{
	ASSERT(m_bCommitted);

	for (INT_PTR i = m_list.GetSize(); 0 < i;)
	{
		Triangle* triangle = m_list[--i];
		PushRenderState(*triangle);
		RenderTriangle(triangle->a.v, triangle->a.n, triangle->b.v, triangle->b.n, triangle->c.v,
					   triangle->c.n);
		PopRenderState();
	}
}

void CXTPOpenGLTriangleRenderList::Reset()
{
	ClearTriangleList();
	m_states.RemoveAll();
	m_colors.RemoveAll();
	m_RenderStateQueue.RemoveAll();
	m_RenderColorQueue.RemoveAll();
	m_jitter = CXTPPoint3d();
}

int __cdecl CXTPOpenGLTriangleRenderList::Compare(const void* x, const void* y)
{
	const Triangle* px = *reinterpret_cast<const Triangle* const*>(x);
	const Triangle* py = *reinterpret_cast<const Triangle* const*>(y);

	if (px->dDist < py->dDist)
		return -1;

	if (px->dDist > py->dDist)
		return 1;

	return 0;
}

void CXTPOpenGLTriangleRenderList::State::Activate(const CXTPPoint3d& jitter)
{
	int nMode = 0;
	glGetIntegerv(GL_MATRIX_MODE, &nMode);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mView);

	if (!jitter.IsZero())
		glTranslated(jitter.X, jitter.Y, jitter.Z);

	glMatrixMode(static_cast<GLenum>(nMode));
}

void CXTPOpenGLTriangleRenderList::Color::Activate()
{
	glColor4f(r, g, b, a);
}

void CXTPOpenGLTriangleRenderList::PushRenderState(Triangle& triangle)
{
	if (!m_RenderColorQueue.IsEmpty() ? m_RenderColorQueue.GetTail() != triangle.pColor : TRUE)
	{
		m_RenderColorQueue.AddTail(triangle.pColor);
		triangle.pColor->Activate();
	}

	m_RenderColorQueue.GetTail()->nRefs++;

	if (!m_RenderStateQueue.IsEmpty() ? m_RenderStateQueue.GetTail() != triangle.pState : TRUE)
	{
		m_RenderStateQueue.AddTail(triangle.pState);
		triangle.pState->Activate(m_jitter);
	}

	m_RenderStateQueue.GetTail()->nRefs++;
}

void CXTPOpenGLTriangleRenderList::PopRenderState()
{
	ASSERT(!m_RenderColorQueue.IsEmpty());
	ASSERT(!m_RenderStateQueue.IsEmpty());

	UINT& nColorRefs = m_RenderColorQueue.GetTail()->nRefs;
	ASSERT(0 < nColorRefs);
	if (0 == --nColorRefs)
	{
		m_RenderColorQueue.RemoveTail();
		if (!m_RenderColorQueue.IsEmpty())
			m_RenderColorQueue.GetTail()->Activate();
	}

	UINT& nStateRefs = m_RenderStateQueue.GetTail()->nRefs;
	ASSERT(0 < nStateRefs);
	if (0 == --nStateRefs)
	{
		m_RenderStateQueue.RemoveTail();
		if (!m_RenderStateQueue.IsEmpty())
			m_RenderStateQueue.GetTail()->Activate(m_jitter);
	}
}

void CXTPOpenGLTriangleRenderList::RenderTriangle(const CXTPPoint3d& a, const CXTPPoint3d& an,
												  const CXTPPoint3d& b, const CXTPPoint3d& bn,
												  const CXTPPoint3d& c, const CXTPPoint3d& cn)
{
	glBegin(GL_TRIANGLES);
	CXTPOpenGLUtils::Vertex(a, an);
	CXTPOpenGLUtils::Vertex(b, bn);
	CXTPOpenGLUtils::Vertex(c, cn);
	glEnd();
}
