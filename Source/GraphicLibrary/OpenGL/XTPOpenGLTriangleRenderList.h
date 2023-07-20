// XTPOpenGLTriangleRenderList.h
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
#if !defined(__XTPOPENGLTRIANGLERENDERLIST_H__)
#	define __XTPOPENGLTRIANGLERENDERLIST_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPOpenGLTriangleRenderList
{
public:
	CXTPOpenGLTriangleRenderList();
	~CXTPOpenGLTriangleRenderList();

	BOOL IsEmpty() const;
	void SetCurrentMatrices();
	void SetCurrentColor(float r, float g, float b, float a);
	void Add(const CXTPPoint3d& a, const CXTPPoint3d& an, const CXTPPoint3d& b,
			 const CXTPPoint3d& bn, const CXTPPoint3d& c, const CXTPPoint3d& cn);
	void Commit();
	void RenderAll();
	void Reset();
	void SetJitter(const CXTPPoint3d& offset);

private:
	struct State
	{
		UINT nRefs;
		GLfloat mView[16];

		void Activate(const CXTPPoint3d& jitter);
	};

	struct Color
	{
		UINT nRefs;
		float r;
		float g;
		float b;
		float a;

		void Activate();
	};

	struct Triangle
	{
		State* pState;
		Color* pColor;
		double dDist;
		struct Vertices
		{
			CXTPPoint3d v, n;
		} a, b, c;
	};

	static int __cdecl Compare(const void* x, const void* y);
	void PushRenderState(Triangle& triangle);
	void PopRenderState();
	void RenderTriangle(const CXTPPoint3d& a, const CXTPPoint3d& an, const CXTPPoint3d& b,
						const CXTPPoint3d& bn, const CXTPPoint3d& c, const CXTPPoint3d& cn);
	void ClearTriangleList();

	CArray<Triangle*, Triangle*> m_list;
	CList<State, const State&> m_states;
	CList<Color, const Color&> m_colors;
	CList<State*, State*> m_RenderStateQueue;
	CList<Color*, Color*> m_RenderColorQueue;
	CXTPPoint3d m_jitter;
	BOOL m_bCommitted;
};

AFX_INLINE BOOL CXTPOpenGLTriangleRenderList::IsEmpty() const
{
	return m_list.GetSize() == 0;
}

AFX_INLINE void CXTPOpenGLTriangleRenderList::SetJitter(const CXTPPoint3d& offset)
{
	m_jitter = offset;
}

//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPOPENGLTRIANGLERENDERLIST_H__)
