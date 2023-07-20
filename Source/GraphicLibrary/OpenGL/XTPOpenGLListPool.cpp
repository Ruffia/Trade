// XTPOpenGLListPool.cpp
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

#include "Common/XTPTypeId.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include "GraphicLibrary/OpenGL/GL.h"
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "GraphicLibrary/OpenGL/XTPOpenGLListPool.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CXTPOpenGLListPool* AFX_CDECL XTPOpenGLListPool()
{
	return &CXTPOpenGLListPool::Instance();
}

CXTPOpenGLListPool::CXTPOpenGLListPool()
	: m_nMax(0)
	, m_bCommitExpected(FALSE)
{
}

CXTPOpenGLListPool::~CXTPOpenGLListPool()
{
	ASSERT(!m_bCommitExpected && "An OpenGL list has not been committed");
}

GLuint CXTPOpenGLListPool::AllocateList(GLenum mode)
{
	ASSERT(!m_bCommitExpected && "An OpenGL list has not been committed");

	GLuint id = 0;
	BOOL bIsMax = FALSE;

	if (0 < m_reclaimed.GetCount())
	{
		id = m_reclaimed.GetHead();
		m_reclaimed.RemoveHead();
	}
	else
	{
		ASSERT(m_nMax != ~GLuint(0) && "Too much lists generated. Consider resource optimization.");
		if (m_nMax != ~GLuint(0))
		{
			bIsMax = TRUE;
			id = m_nMax + 1;
		}
	}

	if (0 < id)
	{
		glNewList(id, mode);
		if (GL_NO_ERROR == glGetError())
		{
			m_allocated.AddTail(id);
			m_bCommitExpected = TRUE;

			if (bIsMax)
			{
				m_nMax = id;
			}
		}
		else
		{
			id = 0;
		}
	}

	return id;
}

void CXTPOpenGLListPool::CommitList()
{
	ASSERT(m_bCommitExpected && "No OpenGL list is expected to be committed");

	if (m_bCommitExpected)
	{
		glEndList();
		m_bCommitExpected = FALSE;
	}
}

void CXTPOpenGLListPool::ReclaimList(GLuint id)
{
	ASSERT(!m_bCommitExpected && "An OpenGL list has not been committed");

	POSITION pos = m_allocated.Find(id);
	ASSERT(NULL != pos && "The list requested is not allocated and thus cannot be reclaimed");
	if (NULL != pos)
	{
		m_allocated.RemoveAt(pos);
		m_reclaimed.AddTail(id);

		glDeleteLists(id, 1);
	}
}
